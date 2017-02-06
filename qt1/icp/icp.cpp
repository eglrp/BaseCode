#include "icp.h"
#include"tool/Point3D.h"
#include <windows.h>

using namespace std;

Icp::Icp(double *M, vector<int> denser, vector<vector<int>> rel, vector<Point3D> vex, vector<Face> facem, const int M_num, const int dim) :
dim(dim), sub_step(10), max_iter(200), min_delta(1e-7), denseRelation(denser), relation(rel), vertex(vex), faces(facem) {
	DWORD kd_begin = ::GetTickCount();// KD耗时

	// 检测维度
	if (dim != 2 && dim != 3) {
		cout << "ERROR: LIBICP works only for data of dimensionality 2 or 3" << endl;
		M_tree = 0;
		return;
	}

	// 检测点的数量
	if (M_num < 5) {
		cout << "ERROR: LIBICP works only with at least 5 model points" << endl;
		M_tree = 0;
		return;
	}

	M_Centroid = Matrix::ones(3, 1);
	M_Centroid.val[0][0] = 0;
	M_Centroid.val[1][0] = 0;
	M_Centroid.val[2][0] = 0;

	// 将模型点拷贝到M_data
	M_data.resize(M_num);
	for (int i = 0; i < M_num; i++)
	{
		M_data[i].resize(dim);//用于存储M的顶点
	}

	for (int m = 0; m < M_num; m++)//获取M的所有顶点
	for (int n = 0; n < dim; n++)
	{
		M_data[m][n] = (float)M[m*dim + n];

		M_Centroid.val[n][0] += (float)M[m*dim + n];
	}

	M_Centroid = M_Centroid / M_num;//计算M的质心

	M_tree = new kdtree::KDTree(M_data);//建立模型M顶点的KD树


	DWORD kd_title = ::GetTickCount() - kd_begin;// ICP耗时
	cout << endl << "KDTree耗时： " << kd_title / 1000 << " s " << kd_title % 1000 << " ms " << endl;
}

Icp::~Icp() {
	if (M_tree)
		delete M_tree;
}

double dis(Point3D a, Point3D b)		//Calculate distance between two points
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

Point3D crossmul(Point3D a, Point3D b, Point3D c)		//Calculate crossmul of two vector
{
	double a1 = a.x - c.x, a2 = a.y - c.y, a3 = a.z - c.z;
	double b1 = b.x - c.x, b2 = b.y - c.y, b3 = b.z - c.z;
	return Point3D(a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1);
}

Point3D normalize(Point3D a)		//Normalize the vector
{
	double len = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	a.x /= len;
	a.y /= len;
	a.z /= len;
	return a;
}

bool ptInTriangle(Point3D pt, Point3D tri[], double eps = 1e-9)		//Judge Point is whether in the Triangle
{
	Point3D n1 = crossmul(tri[0], tri[1], pt);
	Point3D n2 = crossmul(tri[1], tri[2], pt);
	Point3D n3 = crossmul(tri[2], tri[0], pt);
	if ((n1.x*n2.x) >= -eps && (n1.x*n3.x) >= -eps && (n1.y*n2.y) >= -eps && (n1.y*n3.y) >= -eps
		&& (n1.z*n2.z) >= -eps && (n1.z*n3.z) >= -eps)
		return true;
	return false;
}

bool ptInSegment(Point3D pt, Point3D a, Point3D b, double eps = 1e-9)		//Judge Point is whether on the Segment
{
	if ((a.x - pt.x)*(pt.x - b.x) >= -eps && (a.y - pt.y)*(pt.y - b.y) >= -eps && (a.z - pt.z)*(pt.z - b.z) >= -eps)
		return true;
	return false;
}

double pt2Segment(Point3D pt, Point3D a, Point3D b)		//Calculate distance between Point to Segment
{
	double A = b.x - a.x, B = b.y - a.y, C = b.z - a.z;
	double D = -(A*pt.x + B*pt.y + C*pt.z);
	double t = -(A*a.x + B*a.y + C*a.z + D) / (A*A + B*B + C*C);
	Point3D p(a.x + A*t, a.y + B*t, a.z + C*t);
	if (ptInSegment(p, a, b))
		return dis(pt, p);
	else
		return min(dis(p, a), dis(p, b));
}

double pt2Triangle(Point3D pt, Point3D tri1, Point3D tri2, Point3D tri3)		//Calculate distance between Point to Triangle
{
	Point3D tri[3];
	tri[0] = tri1, tri[1] = tri2, tri[2] = tri3;
	Point3D n = normalize(crossmul(tri[0], tri[1], tri[2]));
	double A = n.x, B = n.y, C = n.z, D = -(n.x*tri[2].x + n.y*tri[2].y + n.z*tri[2].z);
	double d = (A*pt.x + B*pt.y + C*pt.z + D) / sqrt(A*A + B*B + C*C);
	Point3D p(pt.x - d*n.x, pt.y - d*n.y, pt.z - d*n.z);
	if (ptInTriangle(p, tri))
		return fabs(d);
	else
		return min(pt2Segment(pt, tri[0], tri[1]), min(pt2Segment(pt, tri[1], tri[2]), pt2Segment(pt, tri[0], tri[2])));
}

void Icp::fit(double *T, const int T_num, Matrix &R, Matrix &t, const double indist, vector<double> &offset) {

	offset.clear();

	// 检测释放存在模型点云
	if (!M_tree) {
		cout << "ERROR: No model available." << endl;
		return;
	}

	// 检测场景点云数量
	if (T_num < 5) {
		cout << "ERROR: Icp works only with at least 5 template points" << endl;
		return;
	}

	DWORD a = ::GetTickCount();// ICP耗时

	fitCoarse(T, T_num, t);// 粗配准，中心点重和：计算点云质心


	vector<int> active;// 采集的顶点，用于配准
	active.clear();

	for (int i = 0; i < T_num; i += sub_step)// 采集部分顶点配准
		active.push_back(i);
	fitIterate(T, T_num, R, t, active);// 进行迭代

	if (indist <= 0)// 精确配准，使用所有顶点进行配准
	{
		active.clear();
		for (int i = 0; i < T_num; i += sub_step)
			active.push_back(i);
	}
	else
	{
		active = getInliers(T, T_num, R, t, indist);//应用误差排除
	}
	fitIterate(T, T_num, R, t, active);// 进行迭代

	DWORD fit_title = ::GetTickCount() - a;// ICP耗时
	cout << endl << "fit耗时： " << fit_title / 1000 << " s " << fit_title % 1000 << " ms " << endl;

	std::vector<float>         query(dim);
	kdtree::KDTreeResultVector neighbor;

	// dimensionality 2
	if (dim == 2) {
		// dimensionality 3
	}
	else {

		// 获取旋转矩阵和平移向量
		double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
		double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
		double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
		double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

		// 检测场景点是否是内点
		for (int i = 0; i<T_num; i++) {

			// 将当前场景点变换到最新位置
			double sx = r00*T[i * 3 + 0] + r01*T[i * 3 + 1] + r02*T[i * 3 + 2] + t0; query[0] = sx;
			double sy = r10*T[i * 3 + 0] + r11*T[i * 3 + 1] + r12*T[i * 3 + 2] + t1; query[1] = sy;
			double sz = r20*T[i * 3 + 0] + r21*T[i * 3 + 1] + r22*T[i * 3 + 2] + t2; query[2] = sz;

			// 搜索当前点的最近点
			M_tree->n_nearest(query, 1, neighbor);

			// 模型点
			//			double dx = M_data[neighbor[0].idx][0];
			//			double dy = M_data[neighbor[0].idx][1];
			//			double dz = M_data[neighbor[0].idx][2];

			int pid = denseRelation[neighbor[0].idx];
			Point3D pt(M_data[neighbor[0].idx][0], M_data[neighbor[0].idx][1], M_data[neighbor[0].idx][2]);
			Point3D sr(sx, sy, sz);

			offset.push_back(pt2Triangle(sr, vertex[faces[pid][0]], vertex[faces[pid][1]], vertex[faces[pid][2]]));

		}
	}
}

void Icp::fitIterate(double *T, const int T_num, Matrix &R, Matrix &t, const std::vector<int> &active)
{
	if (active.size() < 5)
		return;

	// 迭代，直到目标函数收敛,或超出最大迭代次数
	for (int iter = 0; iter < max_iter; iter++)
	{
		delta = fitStep(T, T_num, R, t, active);
		if (delta < min_delta)
			break;
	}
}

int Icp::getMaxIter()
{
	return max_iter;
}
double Icp::getMinDelta()
{
	return min_delta;
}

double Icp::getDelta()
{
	return delta;
}

Matrix Icp::getMCentroid()
{
	return M_Centroid;
}

void Icp::fitCoarse(double *T, const int T_num, Matrix &t)
{
	if (T_num < 5)
		return;

	Matrix T_Centroid(3, 1);
	T_Centroid.val[0][0] = 0;
	T_Centroid.val[1][0] = 0;
	T_Centroid.val[2][0] = 0;

	// 获取平移向量
	double t0 = t.val[0][0]; double t1 = t.val[1][0]; double t2 = t.val[2][0];

	int index = 0;
	for (int i = 0; i < T_num; i++, index += 3)
	{
		T_Centroid.val[0][0] += (T[index + 0] + t0);
		T_Centroid.val[1][0] += (T[index + 1] + t1);
		T_Centroid.val[2][0] += (T[index + 2] + t2);
	}

	T_Centroid = T_Centroid / T_num;

	t = t + M_Centroid - T_Centroid;
}