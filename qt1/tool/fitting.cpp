#include "fitting.h"
#include "MoudelWidget.h"
#include <QWheelEvent>
Fitting::Fitting()
{
}


Fitting::~Fitting()
{
}


//*****************************计算方程*****************************
//计算平面方程，根据平面上三个点，计算面的法向量，进而计算平面方程系数。
//使用叉积计算法向量；平面方程一般式：Ax+By+Cz+D=0.
bool Fitting::getPlane(Point3D v1, Point3D v2, Point3D v3, double* coefficient)
{
	if (coefficient == NULL)
		return false;
	if (isCollineation(v1, v2, v3))	//三点共线
		return false;

	double vc1[3], vc2[3];//三角形内两个不平行的向量
	double a, b, c, d;
	Point3D normal;	//法向量
	double r;		//向量模
	vc1[0] = v2.x - v1.x; vc1[1] = v2.y - v1.y; vc1[2] = v2.z - v1.z;
	vc2[0] = v3.x - v1.x; vc2[1] = v3.y - v1.y; vc2[2] = v3.z - v1.z;
	normal.x = vc1[1] * vc2[2] - vc2[1] * vc1[2];
	normal.y = vc2[0] * vc1[2] - vc1[0] * vc2[2];
	normal.z = vc1[0] * vc2[1] - vc2[0] * vc1[1];
	r = normal.length();
	a = normal.x / r;	//归一化
	b = normal.y / r;
	c = normal.z / r;

	d = -(a*v1.x + b*v1.y + c*v1.z);

	coefficient[0] = a;
	coefficient[1] = b;
	coefficient[2] = c;
	coefficient[3] = d;

	return true;
}

bool Fitting::getLine(Point3D v1, Point3D v2, double* coefficient)
{
	if (coefficient == NULL)
		return false;
	if (v1 == v2)	//同一点
		return false;

	//直线方向向量
	double a = v1.x - v2.x;
	double b = v1.y - v2.y;
	double c = v1.z - v2.z;

	coefficient[0] = v1.x;
	coefficient[1] = v1.y;
	coefficient[2] = v1.z;
	coefficient[3] = a;
	coefficient[4] = b;
	coefficient[5] = c;

	return true;
}

bool Fitting::getCircular(Point3D v1, Point3D v2, Point3D v3, double* coefficient)
{
	double plane[4];//所在平面方程系数

	//获取圆所在平面
	if (getPlane(v1, v2, v3, plane) == false)
		return false;

	//方程组系数矩阵和常数项，列方程组计算圆心，圆心到点的距离方程+平面方程。
	double A[3][3];
	double b[3];
	A[0][0] = 2 * (v2.x - v1.x); A[0][1] = 2 * (v2.y - v1.y); A[0][2] = 2 * (v2.z - v1.z);
	A[1][0] = 2 * (v3.x - v1.x); A[1][1] = 2 * (v3.x - v1.y); A[1][2] = 2 * (v3.x - v1.z);
	A[2][0] = plane[0]; A[2][1] = plane[1]; A[2][2] = plane[2];
	b[0] = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[1] = v3.x*v3.x + v3.y*v3.y + v3.z*v3.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[2] = -plane[3];

	double center[3];//圆心坐标

	gauss_ColumnPrimaryElement(A[0], b, 3, center);//解方程组

	double r = dist(Point3D(center[0], center[1], center[2]), v1);//计算半径

	coefficient[0] = center[0]; //圆心
	coefficient[1] = center[1];
	coefficient[2] = center[2];
	coefficient[3] = r;			//半径
	coefficient[4] = plane[0];	//平面系数
	coefficient[5] = plane[1];
	coefficient[6] = plane[2];
	coefficient[7] = plane[3];

	return true;
}

bool Fitting::getSphere(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient)
{
	if (coefficient == NULL)
		return false;
	double c[4];//平面方程系数
	getPlane(v1, v2, v3, c);
	//判断是否四点共面
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v4 - v1)) == 0)
		return false;

	//方程组系数矩阵和常数项
	double A[3][3];
	double b[3];
	A[0][0] = 2 * (v2.x - v1.x); A[0][1] = 2 * (v2.y - v1.y); A[0][2] = 2 * (v2.z - v1.z);
	A[1][0] = 2 * (v4.x - v3.x); A[1][1] = 2 * (v4.y - v3.y); A[1][2] = 2 * (v4.z - v3.z);
	A[2][0] = 2 * (v4.x - v1.x); A[2][1] = 2 * (v4.y - v1.y); A[2][2] = 2 * (v4.z - v1.z);
	b[0] = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[1] = v4.x*v4.x + v4.y*v4.y + v4.z*v4.z - v3.x*v3.x - v3.y*v3.y - v3.z*v3.z;
	b[2] = v4.x*v4.x + v4.y*v4.y + v4.z*v4.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;

	double center[3];//球心坐标

	gauss_ColumnPrimaryElement(A[0], b, 3, center);//解方程组

	double r = dist(Point3D(center[0], center[1], center[2]), v1);//计算半径

	coefficient[0] = center[0];
	coefficient[1] = center[1];
	coefficient[2] = center[2];
	coefficient[3] = r;

	return true;
}

bool Fitting::getEllipse(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, double* coefficient)
{
	double c[4];//所在平面方程系数

	//获取椭圆所在平面
	if (getPlane(v1, v2, v3, c) == false)
		return false;

	//判断是否四点共面
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v4 - v1)) != 0)
		return false;

	//判断是否五点共面
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v5 - v1)) != 0)
		return false;

	Point3D normal_p(c[0], c[1], c[2]);//椭圆平面法向量
	Point3D normal_xy(0, 0, 1);//xy平面法向量
	double angle_p_xy = angleBetween(normal_p, normal_xy);
	double rotationAngle =  angle_p_xy;//旋转角度
	Point3D rotationAxis = normal_p * normal_xy;//旋转向量

	Quaternion v11(v1.x, v1.y, v1.z);
	Quaternion v22(v2.x, v2.y, v2.z);
	Quaternion v33(v3.x, v3.y, v3.z);
	Quaternion v44(v4.x, v4.y, v4.z);
	Quaternion v55(v5.x, v5.y, v5.z);

	v11 = v11.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	v22 = v22.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	v33 = v33.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	v44 = v44.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	v55 = v55.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

	double A[5][5];
	double b[5];

	A[0][0] = v11.x*v11.x; A[0][1] = v11.x*v11.y; A[0][2] = v11.y*v11.y; A[0][3] = v11.x; A[0][4] = v11.y;
	A[1][0] = v22.x*v22.x; A[1][1] = v22.x*v22.y; A[1][2] = v22.y*v22.y; A[1][3] = v22.x; A[1][4] = v22.y;
	A[2][0] = v33.x*v33.x; A[2][1] = v33.x*v33.y; A[2][2] = v33.y*v33.y; A[2][3] = v33.x; A[2][4] = v33.y;
	A[3][0] = v44.x*v44.x; A[3][1] = v44.x*v44.y; A[3][2] = v44.y*v44.y; A[3][3] = v44.x; A[3][4] = v44.y;
	A[4][0] = v55.x*v55.x; A[4][1] = v55.x*v55.y; A[4][2] = v55.y*v55.y; A[4][3] = v55.x; A[4][4] = v55.y;
	b[0] = -1;
	b[1] = -1;
	b[2] = -1;
	b[3] = -1;
	b[4] = -1;

	double cc[5];//方程组的解
	gauss_ColumnPrimaryElement(A[0], b, 5, cc);//解方程组

	if (cc[0] == 0)
		return false;

	double a_ellipse, b_ellipse;//椭圆长半轴和短半轴
	double c_ellipse;//椭圆焦距
	double o[2];//椭圆中心
	double c_2d_coor[4];//平面中椭圆两个焦点坐标
	double c_3d_coor[6];//三维点云中椭圆的焦点坐标
	double angle_a_ellipse;//椭圆长轴斜率

	o[0] = (cc[1] * cc[4] - 2 * cc[2] * cc[3]) / (4 * cc[0] * cc[2] - cc[1] * cc[1]);
	o[1] = (cc[1] * cc[3] - 2 * cc[0] * cc[4]) / (4 * cc[0] * cc[2] - cc[1] * cc[1]);

	a_ellipse = sqrt((2 * (cc[0] * o[0] * o[0] + cc[2] * o[1] * o[1] + cc[1] * o[0] * o[1] - 1)) / (cc[0] + cc[2] + sqrt((cc[0] - cc[2])*(cc[0] - cc[2]) + cc[1] * cc[1])));
	b_ellipse = sqrt((2 * (cc[0] * o[0] * o[0] + cc[2] * o[1] * o[1] + cc[1] * o[0] * o[1] - 1)) / (cc[0] + cc[2] - sqrt((cc[0] - cc[2])*(cc[0] - cc[2]) + cc[1] * cc[1])));
	c_ellipse = sqrt(a_ellipse*a_ellipse - b_ellipse*b_ellipse);
	angle_a_ellipse = (atan(cc[1] / (cc[0] - cc[2]))) / 2;

	c_2d_coor[0] = o[0] - c_ellipse*cos(angle_a_ellipse);//焦点1
	c_2d_coor[1] = o[1] - c_ellipse*sin(angle_a_ellipse);
	c_2d_coor[2] = o[0] + c_ellipse*cos(angle_a_ellipse);//焦点2
	c_2d_coor[3] = o[1] + c_ellipse*sin(angle_a_ellipse);

	Quaternion c_3d1(c_2d_coor[0], c_2d_coor[1], 0);
	Quaternion c_3d2(c_2d_coor[2], c_2d_coor[3], 0);

	c_3d1 = c_3d1.rotational(-rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	c_3d2 = c_3d2.rotational(-rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

	c_3d_coor[0] = c_3d1.x;
	c_3d_coor[1] = c_3d1.y;
	c_3d_coor[2] = c_3d1.z;
	c_3d_coor[3] = c_3d2.x;
	c_3d_coor[4] = c_3d2.y;
	c_3d_coor[5] = c_3d2.z;

	coefficient[0] = c[0];//平面系数 
	coefficient[1] = c[1];
	coefficient[2] = c[2];
	coefficient[3] = c[3];
	coefficient[4] = o[0];//椭圆中心
	coefficient[5] = o[1];
	coefficient[6] = a_ellipse;//椭圆长半轴和短半轴和焦距
	coefficient[7] = b_ellipse;
	coefficient[8] = c_ellipse;
	coefficient[9] = angle_a_ellipse;//椭圆长轴倾角
	coefficient[10] = c_3d_coor[0];//椭圆两个焦点在三维点云的坐标
	coefficient[11] = c_3d_coor[1];
	coefficient[12] = c_3d_coor[2];
	coefficient[13] = c_3d_coor[3];
	coefficient[14] = c_3d_coor[4];
	coefficient[15] = c_3d_coor[5];

}

bool Fitting::getCylinder(Point3D v1, Point3D v2, Point3D v3, double* coefficient)
{
	double circular[8];	//取到的圆方程
	if (getCircular(v1, v2, v3, circular) == false)
		return false;

	coefficient[0] = circular[0];	//圆心
	coefficient[1] = circular[1];
	coefficient[2] = circular[2];
	coefficient[3] = circular[4];	//方向向量
	coefficient[4] = circular[5];
	coefficient[5] = circular[6];
	coefficient[6] = circular[3];	//半径

	return true;
}

bool Fitting::getCone(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient)
{
	if (coefficient == NULL)
		return false;

	double circular[8];	//取到的圆方程
	if (getCircular(v1, v2, v3, circular) == false)
		return false;

	//判断是否四点共面
	if (dotProduct(Point3D(circular[4], circular[5], circular[6]), (v4 - v1)) == 0)
		return false;

	double AR = circular[3];//底面圆半径
	Point3D O(circular[0], circular[1], circular[2]);//底面圆心O
	double line[6] = { circular[0], circular[1], circular[2], circular[4], circular[5], circular[6] };//中心轴线,方向不定
	Point3D dirO(circular[4], circular[5], circular[6]);

	double BR = pointToLine(v4, line);	//小圆半径，B(v4)点所在圆半径
	double bToO = dist(v4, O);			//B点到底面圆圆心的距离
	double bToBB = sqrt(bToO*bToO - BR*BR);	//B到底面圆的距离
	double aToBB = abs(AR - BR);			//A到B在底面圆投影点的距离,B点在底面圆上面或下面
	double theta = atan(aToBB / bToBB);		//母线与中心轴的夹角

	double h = AR*bToBB / aToBB;		//圆锥的高
	Point3D OB = v4 - O;				//BO向量
	//获取指向圆锥顶点的方向向量
	if (BR < AR)//B在底面圆上面
	{
		//如果OB与中心轴方向向量夹角大于90度，则将方向向量旋转180度
		if (angleBetween(OB, dirO)>PI / 2)
		{
			dirO.x = -dirO.x;
			dirO.y = -dirO.y;
			dirO.z = -dirO.z;
		}
	}
	else
	{
		//如果OB与中心轴方向向量夹角小于90度，则将方向向量旋转180度
		if (angleBetween(OB, dirO) < PI / 2)
		{
			dirO.x = -dirO.x;
			dirO.y = -dirO.y;
			dirO.z = -dirO.z;
		}
	}
	dirO = dirO.normalize();

	Point3D top = O + dirO *= h;		//圆锥顶点坐标

	coefficient[0] = top.x;	//顶点坐标
	coefficient[1] = top.y;
	coefficient[2] = top.z;
	coefficient[3] = O.x;	//圆锥内轴线上一点
	coefficient[4] = O.y;
	coefficient[5] = O.z;
	coefficient[6] = theta;	//母线与中心轴的夹角
}

//*****************************其他功能*****************************
int Fitting::gcd(int a, int b)
{
	if (!b)
		return a;

	int n = a%b;
	while (n != 0)
	{
		a = b;
		b = n;
		n = a % b;
	}
	return b;
}
void Fitting::columnPrimaryElement(double *A, double *B, int rows, int k_row)
{
	// 用于存放列主元素的值  
	double main_element = 0.0;
	// 用于存放列主元素所在行  
	int main_line = 0;

	// 中间变量，用于交换  
	double temp = 0.0;
	// 循环变量  
	int i, j;

	// 暂定A[k, k]为列主元素  
	main_element = A[k_row * rows + k_row];
	main_line = k_row;

	for (i = k_row + 1; i < rows; ++i)
	{
		if (fabs(A[i * rows + k_row]) > fabs(main_element))
		{
			main_element = A[i * rows + k_row];
			main_line = i;
		}

		// 如果第k列元素中绝对值最大的不是a[k，k]，则交换两个方程  
		if (main_line != k_row)
		{
			for (j = k_row; j < rows; ++j)
			{
				temp = A[k_row * rows + j];
				A[k_row * rows + j] = A[main_line * rows + j];
				A[main_line * rows + j] = temp;
			}

			temp = B[k_row];
			B[k_row] = B[main_line];
			B[main_line] = temp;
		}
	}
}

void Fitting::gauss_ColumnPrimaryElement(double *A, double *B, int rows, double *X)
{
	// 循环变量  
	int i, j, k;
	// 高斯消元比例因子  
	double c;

	// 消元  
	for (k = 0; k < rows; ++k)
	{
		// 选取列主元素  
		columnPrimaryElement(A, B, rows, k);
		for (i = k + 1; i < rows; ++i)
		{
			// 求得高斯消元比例因子  
			c = A[i * rows + k] / A[k * rows + k];

			for (j = k + 1; j < rows; ++j)
			{
				A[i * rows + j] = A[i * rows + j] - A[k * rows + j] * c;
			}
			B[i] = B[i] - B[k] * c;
		}
	}

	// 有解条件判断  
	// 系数矩阵A的秩等于A的维数n(即行数或者列数)  
	if (fabs(A[(rows - 1) * rows + (rows - 1)]) < 10e-6)
	{
		// 不存在唯一解  
		printf("不存在唯一解!\n");

		for (i = 0; i < rows; ++i)
		{
			X[i] = 0.0;
		}
		return;
	}

	// 回代求解  
	for (i = rows - 1; i >= 0; --i)
	{
		X[i] = B[i];
		for (j = i + 1; j < rows; ++j)
		{
			X[i] = X[i] - A[i * rows + j] * X[j];
		}
		X[i] = X[i] / A[i * rows + i];
	}
}



void Fitting::computeNormal(const kdtree::KDTreeResultVector &neighbors, double *M_normal, kdtree::KDTree* M_tree)
{
	// 获取所有邻点
	Matrix P(neighbors.size(), 3);
	Matrix mu(1, 3);
	for (int i = 0; i < neighbors.size(); i++)
	{
		double x = M_tree->the_data[neighbors[i].idx][0];
		double y = M_tree->the_data[neighbors[i].idx][1];
		double z = M_tree->the_data[neighbors[i].idx][2];
		P.val[i][0] = x;
		P.val[i][1] = y;
		P.val[i][2] = z;
		mu.val[0][0] += x;
		mu.val[0][1] += y;
		mu.val[0][2] += z;
	}

	// 零平均值
	mu = mu / (double)neighbors.size();
	Matrix Q = P - Matrix::ones(neighbors.size(), 1)*mu;

	// 奇异值分解
	Matrix H = ~Q*Q;
	Matrix U, W, V;
	H.svd(U, W, V);

	// 获取向量
	M_normal[0] = U.val[0][2];
	M_normal[1] = U.val[1][2];
	M_normal[2] = U.val[2][2];

	double l = sqrt(M_normal[0] * M_normal[0] + M_normal[1] * M_normal[1] + M_normal[2] * M_normal[2]);

	M_normal[0] /= l;
	M_normal[1] /= l;
	M_normal[2] /= l;
}

double* Fitting::computeNormals(const int num_neighbors, kdtree::KDTree* M_tree)
{
	double *M_normal = (double*)malloc(M_tree->N * 3 * sizeof(double));
	kdtree::KDTreeResultVector neighbors;
	for (int i = 0; i < M_tree->N; i++)
	{
		M_tree->n_nearest_around_point(i, 0, num_neighbors, neighbors);
		computeNormal(neighbors, M_normal + i * 3, M_tree);
	}

	return M_normal;
}

//*****************************点到面的距离*****************************
double Fitting::pointToLine(Point3D v, double* coefficient)
{
	Point3D A, B;//直线外一点到直线的向量，以及直线的一个方向向量
	A.x = v.x - coefficient[0];
	A.y = v.y - coefficient[1];
	A.z = v.z - coefficient[2];
	B.x = coefficient[3];
	B.y = coefficient[4];
	B.z = coefficient[5];

	Point3D crossProduct = Point3D(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - B.x*A.y);
	double area = crossProduct.length(); //四边形面积
	double bottom = B.length();//底长

	return area / bottom;
}

double  Fitting::pointToPlane(Point3D v, double* coefficient)
{
	return abs(coefficient[0] * v.x + coefficient[1] * v.y + coefficient[2] * v.z + coefficient[3]);
}

//*****************************拟合*****************************
void Fitting::planeFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[4];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	//计算平面方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[4];

		//在选择的点云中随机选择三点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//计算平面方程
		if (getPlane(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double dis;				//点到平面的距离
		int inliersCount = 0;	//内点数量
		//double temp = sqrt(coeCur[0] * coeCur[0] + coeCur[1] * coeCur[1] + coeCur[2] * coeCur[2]);

		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToPlane(vertexs[idxFit[i]], coeCur);
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			coeOptimum[0] = coeCur[0];
			coeOptimum[1] = coeCur[1];
			coeOptimum[2] = coeCur[2];
			coeOptimum[3] = coeCur[3];
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 5)
	{
		coe[0] = coeOptimum[0];
		coe[1] = coeOptimum[1];
		coe[2] = coeOptimum[2];
		coe[3] = coeOptimum[3];
		//遍历搜索内点
		double dis;				//点到平面的距离
		vector<int> idxInliers;
		//int temp = sqrt(coe[0] * coe[0] + coe[1] * coe[1] + coe[2] * coe[2]);
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(coe[0] * vertexs[idxFit[i]].x + coe[1] * vertexs[idxFit[i]].y + coe[2] * vertexs[idxFit[i]].z + coe[3]);// / temp;
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		coe[0] = 0;
		coe[1] = 0;
		coe[2] = 0;
		coe[3] = 0;
	}
}

void Fitting::lineFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[6];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	//计算直线方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[6];

		//在选择的点云中随机选择两点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getLine(vertexs[a], vertexs[b], coeCur) == false)
			continue;

		double dis;				//点到直线的距离
		int inliersCount = 0;	//内点数量
		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToLine(vertexs[idxFit[i]], coeCur);
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 6; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 6; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//遍历搜索内点
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToLine(vertexs[idxFit[i]], coe);
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		for (int i = 0; i < 6; i++)
		{
			coe[i] = 0;
		}
	}
}

void Fitting::ellipseFitting(const vector<Point3D> &vertex, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[16];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量


	//椭圆方程测试点
	//getEllipse(Point3D(2, 0, 0), Point3D(-2, 0, 0), Point3D(0, 1, 0), Point3D(0, -1, 0), Point3D(1, 1.73205081, 0), coe);

	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeEll[16];//当前椭圆和平面系数

		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];
		int e = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getEllipse(vertex[a], vertex[b], vertex[c], vertex[d], vertex[e], coeEll) == false)
			continue;

		double coePlane[4];//当前平面系数
		coePlane[0] = coeEll[0];
		coePlane[1] = coeEll[1];
		coePlane[2] = coeEll[2];
		coePlane[3] = coeEll[3];

		Point3D c1(coeEll[10], coeEll[11], coeEll[12]);//椭圆两个焦点
		Point3D c2(coeEll[13], coeEll[14], coeEll[15]);

		double dis_point_plane;//点到椭圆面所在平面的距离
		double dis_point_c;//点到椭圆两个焦点距离和
		double inliersCount = 0;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis_point_plane = pointToPlane(vertex[idxFit[i]], coePlane);
			dis_point_c = dist(vertex[idxFit[i]], c1) + dist(vertex[idxFit[i]], c2) - 2 * coeEll[6];

			//如果点到椭圆所在平面的距离、点到椭圆焦点距离之和小于阈值，则是内点
			if (dis_point_plane < max_dis && dis_point_c < max_dis)
			{
				inliersCount++;
			}
		}

		// 如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>=maxInliersCount)
		{
			maxInliersCount = inliersCount;
			coeOptimum[0] = coeEll[0];
			coeOptimum[1] = coeEll[1];
			coeOptimum[2] = coeEll[2];
			coeOptimum[3] = coeEll[3];
			coeOptimum[4] = coeEll[4];
			coeOptimum[5] = coeEll[5];
			coeOptimum[6] = coeEll[6];
			coeOptimum[7] = coeEll[7];
			coeOptimum[8] = coeEll[8];
			coeOptimum[9] = coeEll[9];
			coeOptimum[10] = coeEll[10];
			coeOptimum[11] = coeEll[11];
			coeOptimum[12] = coeEll[12];
			coeOptimum[13] = coeEll[13];
			coeOptimum[14] = coeEll[14];
			coeOptimum[15] = coeEll[15];

		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 5)
	{
		coe[0] = coeOptimum[0];
		coe[1] = coeOptimum[1];
		coe[2] = coeOptimum[2];
		coe[3] = coeOptimum[3];
		coe[4] = coeOptimum[4];
		coe[5] = coeOptimum[5];
		coe[6] = coeOptimum[6];
		coe[7] = coeOptimum[7];
		coe[8] = coeOptimum[8];
		coe[9] = coeOptimum[9];
		coe[10] = coeOptimum[10];
		coe[11] = coeOptimum[11];
		coe[12] = coeOptimum[12];
		coe[13] = coeOptimum[13];
		coe[14] = coeOptimum[14];
		coe[15] = coeOptimum[15];

		double coePlane[4];//当前平面系数

		coePlane[0] = coe[0];
		coePlane[1] = coe[1];
		coePlane[2] = coe[2];
		coePlane[3] = coe[3];

		Point3D c1(coe[10], coe[11], coe[12]);//椭圆两个焦点
		Point3D c2(coe[13], coe[14], coe[15]);

		//遍历搜索内点
		double dis_point_plane;//点到椭圆面所在平面的距离
		double dis_point_c;//点到椭圆两个焦点距离和
		vector<int> idxInliers;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis_point_plane = pointToPlane(vertex[idxFit[i]], coePlane);
			dis_point_c = dist(vertex[idxFit[i]], c1) + dist(vertex[idxFit[i]], c2) - 2 * coe[6];
			//如果点到椭圆所在平面的距离、点到椭圆焦点距离之和小于阈值，则是内点
			if (dis_point_plane < max_dis && dis_point_c < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}

		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		coe[0] = 0;
		coe[1] = 0;
		coe[2] = 0;
		coe[3] = 0;
		coe[4] = 0;
		coe[5] = 0;
		coe[6] = 0;
		coe[7] = 0;
		coe[8] = 0;
		coe[9] = 0;
		coe[10] = 0;
		coe[11] = 0;
		coe[12] = 0;
		coe[13] = 0;
		coe[14] = 0;
		coe[15] = 0;
	}



}

void Fitting::circularFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[8];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量

	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[8];//球和平面方程的系数
		double p_coeCur[4];//平面方程的系数
		//在选择的点云中随机选择点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getCircular(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double p_dis;				//点到平面的距离
		double s_dis;				//点到球面的距离
		int inliersCount = 0;	//内点数量
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coeCur[i + 4];
		}
		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
			s_dis = abs(dist(vertexs[idxFit[i]], Point3D(coeCur[0], coeCur[1], coeCur[2])) - coeCur[3]);
			//如果距离小于阈值，则是内点
			if ((p_dis < max_dis) && (s_dis < max_dis))
			{
				inliersCount++;
			}
		}

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 8; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 1000)
	{
		for (int i = 0; i < 8; i++)
		{
			coe[i] = coeOptimum[i];
		}
		double p_coeCur[4];//平面方程的系数
		//遍历搜索内点
		double p_dis;				//点到平面的距离
		double s_dis;				//点到球面的距离
		vector<int> idxInliers;
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coe[i + 4];
		}
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
			s_dis = abs(dist(vertexs[idxFit[i]], Point3D(coe[0], coe[1], coe[2])) - coe[3]);
			//如果距离小于阈值，则是内点
			if ((p_dis < max_dis) && (s_dis < max_dis))
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		for (int i = 0; i < 8; i++)
		{
			coe[i] = 0;
		}
	}
}

void Fitting::sphereFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[4];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[4];

		//在选择的点云中随机选择点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getSphere(vertexs[a], vertexs[b], vertexs[c], vertexs[d], coeCur) == false)
			continue;

		double dis;				//点到球面的距离
		int inliersCount = 0;	//内点数量

		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(dist(Point3D(coeCur[0], coeCur[1], coeCur[2]), vertexs[idxFit[i]]) - coeCur[3]);
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			coeOptimum[0] = coeCur[0];
			coeOptimum[1] = coeCur[1];
			coeOptimum[2] = coeCur[2];
			coeOptimum[3] = coeCur[3];
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 5)
	{
		coe[0] = coeOptimum[0];
		coe[1] = coeOptimum[1];
		coe[2] = coeOptimum[2];
		coe[3] = coeOptimum[3];
		//遍历搜索内点
		double dis;
		vector<int> idxInliers;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(dist(Point3D(coe[0], coe[1], coe[2]), vertexs[idxFit[i]]) - coe[3]);
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		coe[0] = 0;
		coe[1] = 0;
		coe[2] = 0;
	}
}

void Fitting::cylinderFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[7];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	double error = 0.0;
	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//圆柱方程
		double l_coeCur[6];	//中轴线方程的系数

		//在选择的点云中随机选择点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getCylinder(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double dis;				//点到中轴线的距离
		int inliersCount = 0;	//内点数量
		double d_error = 0.0;	//当前迭代误差
		for (int i = 0; i < 6; i++)
		{
			l_coeCur[i] = coeCur[i];
		}

		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], l_coeCur) - coeCur[6]);//点到圆柱面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				d_error += dis;//记录误差
				inliersCount++;
			}
		}
		d_error /= inliersCount;

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			error = d_error;
			for (int i = 0; i < 7; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 7; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//遍历搜索内点
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], coe) - coe[6]);//点到圆柱面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		for (int i = 0; i < 7; i++)
		{
			coe[i] = 0;
		}
	}
}

void Fitting::cylinderFitting1(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	//------------计算所选点集的高斯面-----------
	kdtree::KDTree*     M_tree;	// 模型点云的KD树
	kdtree::KDTreeArray M_data;	//模型点云的数据
	// 将模型点拷贝到M_data
	M_data.resize(idxFit.size());
	for (int i = 0; i < idxFit.size(); i++)
	{
		M_data[i].resize(3);//用于存储M的顶点
	}
	for (int m = 0; m < idxFit.size(); m++)//获取M的所有顶点
	{
		M_data[m][0] = vertexs[idxFit[m]].x;
		M_data[m][1] = vertexs[idxFit[m]].y;
		M_data[m][2] = vertexs[idxFit[m]].z;
	}
	M_tree = new kdtree::KDTree(M_data);//建立模型M顶点的KD树
	double* nor = computeNormals(10, M_tree);//高斯面内的点集
	vector<Point3D> norGauss;
	for (int i = 0; i < idxFit.size(); i++)
	{
		norGauss.push_back(Point3D(nor[i * 3 + 0], nor[i * 3 + 1], nor[i * 3 + 2]));
	}

	//norGauss = vertexs;
	//--------迭代计算法向量--------
	Point3D normal;				//最佳圆环法向量，圆柱中心轴方向向量
	vector<int> idxCir;			//高斯面中组成圆的内点
	double cirOptimum[8];		//当前最优方程
	int cirmaxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[8];//球和平面方程的系数

		//---------计算圆方程---------
		Point3D v1 = norGauss[rand() % norGauss.size()];//任一点
		Point3D v2 = norGauss[rand() % norGauss.size()];//任一点
		if (v1 == v2)
			continue;
		Point3D center(0.0, 0.0, 0.0);//圆心坐标
		double r = 1.0;
		//获取圆所在平面
		double plane[4];//所在平面方程系数
		if (getPlane(v1, v2, center, plane) == false)
			continue;
		coeCur[0] = 0;				//圆心
		coeCur[1] = 0;
		coeCur[2] = 0;
		coeCur[3] = 1.0;			//半径
		coeCur[4] = plane[0];		//平面系数
		coeCur[5] = plane[1];
		coeCur[6] = plane[2];
		coeCur[7] = 0;

		//-----------获取最佳方程----------
		double p_coeCur[4];//平面方程的系数
		double p_dis;				//点到平面的距离
		double s_dis;				//点到球面的距离
		int inliersCount = 0;	//内点数量
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coeCur[i + 4];
		}
		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(norGauss[i], p_coeCur);
			s_dis = abs(dist(norGauss[i], Point3D(coeCur[0], coeCur[1], coeCur[2])) - coeCur[3]);
			//如果距离小于阈值，则是内点
			if ((p_dis < max_dis) && (s_dis < max_dis))
			{
				inliersCount++;
			}
		}
		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>cirmaxInliersCount)
		{
			cirmaxInliersCount = inliersCount;
			for (int i = 0; i < 8; i++)
			{
				cirOptimum[i] = coeCur[i];
			}
		}
	}
	//---------取高斯面内圆的内点--------
	double p_coeCur[4];//平面方程的系数
	//遍历搜索内点
	double p_dis;				//点到平面的距离
	double s_dis;				//点到球面的距离
	for (int i = 0; i < 4; i++)
	{
		p_coeCur[i] = cirOptimum[i + 4];
	}
	for (int i = 0; i < idxFit.size(); i++)
	{
		p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
		s_dis = abs(dist(vertexs[idxFit[i]], Point3D(cirOptimum[0], cirOptimum[1], cirOptimum[2])) - cirOptimum[3]);
		//如果距离小于阈值，则是内点
		if ((p_dis < max_dis) && (s_dis < max_dis))
		{
			idxCir.push_back(idxFit[i]);
		}
	}

	//idxFit = idxCir;
	normal = Point3D(cirOptimum[4], cirOptimum[5], cirOptimum[6]);

	delete M_tree;
	//----------计算中心轴和半径------------
	double coeOptimum[7];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//圆柱方程
		double l_coeCur[6];	//中轴线方程的系数

		//在选择的点云中随机选择点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getCylinder(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double angle = angleBetween(Point3D(coeCur[3], coeCur[4], coeCur[5]), normal);
		if (!(angle > PI - 0.01 || angle < 0.01))
			continue;

		double dis;				//点到中轴线的距离
		int inliersCount = 0;	//内点数量
		double d_error = 0.0;	//当前迭代误差
		for (int i = 0; i < 6; i++)
		{
			l_coeCur[i] = coeCur[i];
		}

		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], l_coeCur) - coeCur[6]);//点到圆柱面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				d_error += dis;//记录误差
				inliersCount++;
			}
		}
		d_error /= inliersCount;

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 7; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 7; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//遍历搜索内点
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], coe) - coe[6]);//点到圆柱面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		for (int i = 0; i < 7; i++)
		{
			coe[i] = 0;
		}
	}
}

double* Fitting::cylinderFitting2(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return NULL;

	double coeOptimum[7];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量

	kdtree::KDTree*     M_tree;	// 模型点云的KD树
	kdtree::KDTreeArray M_data;	//模型点云的数据

	// 将模型点拷贝到M_data
	M_data.resize(idxFit.size());
	for (int i = 0; i < idxFit.size(); i++)
	{
		M_data[i].resize(3);//用于存储M的顶点
	}

	for (int m = 0; m < idxFit.size(); m++)//获取M的所有顶点
	{
		M_data[m][0] = vertexs[idxFit[m]].x;
		M_data[m][1] = vertexs[idxFit[m]].y;
		M_data[m][2] = vertexs[idxFit[m]].z;
	}

	M_tree = new kdtree::KDTree(M_data);//建立模型M顶点的KD树

	double* N = computeNormals(10, M_tree);

	delete M_tree;

	return N;
}

void Fitting::coneFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[7];		//当前最优方程
	int maxInliersCount = 0;	//当前最多内点数量,最优方程内点数量
	//计算方程，迭代获取最优方程
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//圆锥方程
		double l_coeCur[6];	//中轴线方程的系数
		//在选择的点云中随机选择点
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];

		//计算方程
		if (getCone(vertexs[a], vertexs[b], vertexs[c], vertexs[d], coeCur) == false)
			continue;

		for (int i = 0; i < 3; i++)
		{
			l_coeCur[i] = coeCur[i];
		}
		for (int i = 3; i < 6; i++)
		{
			l_coeCur[i] = coeCur[i] - coeCur[i - 3];
		}

		Point3D T(coeCur[0], coeCur[1], coeCur[2]);//圆锥顶点坐标
		Point3D TV, TO;			//点与顶点的向量，中心轴方向向量（指向圆锥内）
		double _theta;			//点与中心轴夹角，用于判断点是否在圆锥顶点下方
		double r;				//当点对应的半径
		double pToL;			//点到中心轴距离
		double h;				//当前点对应的高

		double dis;				//点到圆锥面的距离
		int inliersCount = 0;	//内点数量

		//遍历搜索内点
		for (int i = 0; i < idxFit.size(); i++)
		{
			TV = Point3D(vertexs[idxFit[i]] - T);
			TO = Point3D(l_coeCur[3], l_coeCur[4], l_coeCur[5]);
			_theta = angleBetween(TV, TO);

			if (_theta>PI / 2)
				continue;

			pToL = pointToLine(vertexs[idxFit[i]], l_coeCur);
			h = pToL / tan(_theta);
			r = h*tan(coeCur[6]);
			dis = abs(pToL - r);//点到圆锥面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//如果当前方程内点数量最多，则最优方程当前方程
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 7; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//最优方程的顶点数量小于阈值，则选择的点云没有可以拟合的图形
	if (maxInliersCount > idxFit.size() / 100)
	{
		double l_coeCur[6];	//中轴线方程的系数
		for (int i = 0; i < 7; i++)
		{
			coe[i] = coeOptimum[i];
		}

		for (int i = 0; i < 3; i++)
		{
			l_coeCur[i] = coe[i];
		}
		for (int i = 3; i < 6; i++)
		{
			l_coeCur[i] = coe[i] - coe[i - 3];
		}
		//遍历搜索内点
		Point3D T(coe[0], coe[1], coe[2]);//圆锥顶点坐标
		Point3D TV, TO;			//点与顶点的向量，中心轴方向向量（指向圆锥内）
		double _theta;			//点与中心轴夹角，用于判断点是否在圆锥顶点下方
		double r;				//当点对应的半径
		double pToL;			//点到中心轴距离
		double h;				//当前点对应的高
		double dis;				//点到圆锥面的距离
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			TV = Point3D(vertexs[idxFit[i]] - T);
			TO = Point3D(l_coeCur[3], l_coeCur[4], l_coeCur[5]);
			_theta = angleBetween(TV, TO);
			pToL = pointToLine(vertexs[idxFit[i]], l_coeCur);
			h = pToL / tan(_theta);
			r = h*tan(coe[6]);
			dis = abs(pToL - r);//点到圆锥面的距离
			//如果距离小于阈值，则是内点
			if (dis < max_dis)
			{
				idxInliers.push_back(idxFit[i]);
			}
		}
		idxFit = idxInliers;
	}
	else
	{
		idxFit.clear();
		for (int i = 0; i < 7; i++)
		{
			coe[i] = 0;
		}
	}
}