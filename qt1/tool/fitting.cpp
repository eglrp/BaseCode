#include "fitting.h"
#include "MoudelWidget.h"
#include <QWheelEvent>
Fitting::Fitting()
{
}


Fitting::~Fitting()
{
}


//*****************************���㷽��*****************************
//����ƽ�淽�̣�����ƽ���������㣬������ķ���������������ƽ�淽��ϵ����
//ʹ�ò�����㷨������ƽ�淽��һ��ʽ��Ax+By+Cz+D=0.
bool Fitting::getPlane(Point3D v1, Point3D v2, Point3D v3, double* coefficient)
{
	if (coefficient == NULL)
		return false;
	if (isCollineation(v1, v2, v3))	//���㹲��
		return false;

	double vc1[3], vc2[3];//��������������ƽ�е�����
	double a, b, c, d;
	Point3D normal;	//������
	double r;		//����ģ
	vc1[0] = v2.x - v1.x; vc1[1] = v2.y - v1.y; vc1[2] = v2.z - v1.z;
	vc2[0] = v3.x - v1.x; vc2[1] = v3.y - v1.y; vc2[2] = v3.z - v1.z;
	normal.x = vc1[1] * vc2[2] - vc2[1] * vc1[2];
	normal.y = vc2[0] * vc1[2] - vc1[0] * vc2[2];
	normal.z = vc1[0] * vc2[1] - vc2[0] * vc1[1];
	r = normal.length();
	a = normal.x / r;	//��һ��
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
	if (v1 == v2)	//ͬһ��
		return false;

	//ֱ�߷�������
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
	double plane[4];//����ƽ�淽��ϵ��

	//��ȡԲ����ƽ��
	if (getPlane(v1, v2, v3, plane) == false)
		return false;

	//������ϵ������ͳ�����з��������Բ�ģ�Բ�ĵ���ľ��뷽��+ƽ�淽�̡�
	double A[3][3];
	double b[3];
	A[0][0] = 2 * (v2.x - v1.x); A[0][1] = 2 * (v2.y - v1.y); A[0][2] = 2 * (v2.z - v1.z);
	A[1][0] = 2 * (v3.x - v1.x); A[1][1] = 2 * (v3.x - v1.y); A[1][2] = 2 * (v3.x - v1.z);
	A[2][0] = plane[0]; A[2][1] = plane[1]; A[2][2] = plane[2];
	b[0] = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[1] = v3.x*v3.x + v3.y*v3.y + v3.z*v3.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[2] = -plane[3];

	double center[3];//Բ������

	gauss_ColumnPrimaryElement(A[0], b, 3, center);//�ⷽ����

	double r = dist(Point3D(center[0], center[1], center[2]), v1);//����뾶

	coefficient[0] = center[0]; //Բ��
	coefficient[1] = center[1];
	coefficient[2] = center[2];
	coefficient[3] = r;			//�뾶
	coefficient[4] = plane[0];	//ƽ��ϵ��
	coefficient[5] = plane[1];
	coefficient[6] = plane[2];
	coefficient[7] = plane[3];

	return true;
}

bool Fitting::getSphere(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient)
{
	if (coefficient == NULL)
		return false;
	double c[4];//ƽ�淽��ϵ��
	getPlane(v1, v2, v3, c);
	//�ж��Ƿ��ĵ㹲��
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v4 - v1)) == 0)
		return false;

	//������ϵ������ͳ�����
	double A[3][3];
	double b[3];
	A[0][0] = 2 * (v2.x - v1.x); A[0][1] = 2 * (v2.y - v1.y); A[0][2] = 2 * (v2.z - v1.z);
	A[1][0] = 2 * (v4.x - v3.x); A[1][1] = 2 * (v4.y - v3.y); A[1][2] = 2 * (v4.z - v3.z);
	A[2][0] = 2 * (v4.x - v1.x); A[2][1] = 2 * (v4.y - v1.y); A[2][2] = 2 * (v4.z - v1.z);
	b[0] = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;
	b[1] = v4.x*v4.x + v4.y*v4.y + v4.z*v4.z - v3.x*v3.x - v3.y*v3.y - v3.z*v3.z;
	b[2] = v4.x*v4.x + v4.y*v4.y + v4.z*v4.z - v1.x*v1.x - v1.y*v1.y - v1.z*v1.z;

	double center[3];//��������

	gauss_ColumnPrimaryElement(A[0], b, 3, center);//�ⷽ����

	double r = dist(Point3D(center[0], center[1], center[2]), v1);//����뾶

	coefficient[0] = center[0];
	coefficient[1] = center[1];
	coefficient[2] = center[2];
	coefficient[3] = r;

	return true;
}

bool Fitting::getEllipse(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, double* coefficient)
{
	double c[4];//����ƽ�淽��ϵ��

	//��ȡ��Բ����ƽ��
	if (getPlane(v1, v2, v3, c) == false)
		return false;

	//�ж��Ƿ��ĵ㹲��
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v4 - v1)) != 0)
		return false;

	//�ж��Ƿ���㹲��
	if (dotProduct(Point3D(c[0], c[1], c[2]), (v5 - v1)) != 0)
		return false;

	Point3D normal_p(c[0], c[1], c[2]);//��Բƽ�淨����
	Point3D normal_xy(0, 0, 1);//xyƽ�淨����
	double angle_p_xy = angleBetween(normal_p, normal_xy);
	double rotationAngle =  angle_p_xy;//��ת�Ƕ�
	Point3D rotationAxis = normal_p * normal_xy;//��ת����

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

	double cc[5];//������Ľ�
	gauss_ColumnPrimaryElement(A[0], b, 5, cc);//�ⷽ����

	if (cc[0] == 0)
		return false;

	double a_ellipse, b_ellipse;//��Բ������Ͷ̰���
	double c_ellipse;//��Բ����
	double o[2];//��Բ����
	double c_2d_coor[4];//ƽ������Բ������������
	double c_3d_coor[6];//��ά��������Բ�Ľ�������
	double angle_a_ellipse;//��Բ����б��

	o[0] = (cc[1] * cc[4] - 2 * cc[2] * cc[3]) / (4 * cc[0] * cc[2] - cc[1] * cc[1]);
	o[1] = (cc[1] * cc[3] - 2 * cc[0] * cc[4]) / (4 * cc[0] * cc[2] - cc[1] * cc[1]);

	a_ellipse = sqrt((2 * (cc[0] * o[0] * o[0] + cc[2] * o[1] * o[1] + cc[1] * o[0] * o[1] - 1)) / (cc[0] + cc[2] + sqrt((cc[0] - cc[2])*(cc[0] - cc[2]) + cc[1] * cc[1])));
	b_ellipse = sqrt((2 * (cc[0] * o[0] * o[0] + cc[2] * o[1] * o[1] + cc[1] * o[0] * o[1] - 1)) / (cc[0] + cc[2] - sqrt((cc[0] - cc[2])*(cc[0] - cc[2]) + cc[1] * cc[1])));
	c_ellipse = sqrt(a_ellipse*a_ellipse - b_ellipse*b_ellipse);
	angle_a_ellipse = (atan(cc[1] / (cc[0] - cc[2]))) / 2;

	c_2d_coor[0] = o[0] - c_ellipse*cos(angle_a_ellipse);//����1
	c_2d_coor[1] = o[1] - c_ellipse*sin(angle_a_ellipse);
	c_2d_coor[2] = o[0] + c_ellipse*cos(angle_a_ellipse);//����2
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

	coefficient[0] = c[0];//ƽ��ϵ�� 
	coefficient[1] = c[1];
	coefficient[2] = c[2];
	coefficient[3] = c[3];
	coefficient[4] = o[0];//��Բ����
	coefficient[5] = o[1];
	coefficient[6] = a_ellipse;//��Բ������Ͷ̰���ͽ���
	coefficient[7] = b_ellipse;
	coefficient[8] = c_ellipse;
	coefficient[9] = angle_a_ellipse;//��Բ�������
	coefficient[10] = c_3d_coor[0];//��Բ������������ά���Ƶ�����
	coefficient[11] = c_3d_coor[1];
	coefficient[12] = c_3d_coor[2];
	coefficient[13] = c_3d_coor[3];
	coefficient[14] = c_3d_coor[4];
	coefficient[15] = c_3d_coor[5];

}

bool Fitting::getCylinder(Point3D v1, Point3D v2, Point3D v3, double* coefficient)
{
	double circular[8];	//ȡ����Բ����
	if (getCircular(v1, v2, v3, circular) == false)
		return false;

	coefficient[0] = circular[0];	//Բ��
	coefficient[1] = circular[1];
	coefficient[2] = circular[2];
	coefficient[3] = circular[4];	//��������
	coefficient[4] = circular[5];
	coefficient[5] = circular[6];
	coefficient[6] = circular[3];	//�뾶

	return true;
}

bool Fitting::getCone(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient)
{
	if (coefficient == NULL)
		return false;

	double circular[8];	//ȡ����Բ����
	if (getCircular(v1, v2, v3, circular) == false)
		return false;

	//�ж��Ƿ��ĵ㹲��
	if (dotProduct(Point3D(circular[4], circular[5], circular[6]), (v4 - v1)) == 0)
		return false;

	double AR = circular[3];//����Բ�뾶
	Point3D O(circular[0], circular[1], circular[2]);//����Բ��O
	double line[6] = { circular[0], circular[1], circular[2], circular[4], circular[5], circular[6] };//��������,���򲻶�
	Point3D dirO(circular[4], circular[5], circular[6]);

	double BR = pointToLine(v4, line);	//СԲ�뾶��B(v4)������Բ�뾶
	double bToO = dist(v4, O);			//B�㵽����ԲԲ�ĵľ���
	double bToBB = sqrt(bToO*bToO - BR*BR);	//B������Բ�ľ���
	double aToBB = abs(AR - BR);			//A��B�ڵ���ԲͶӰ��ľ���,B���ڵ���Բ���������
	double theta = atan(aToBB / bToBB);		//ĸ����������ļн�

	double h = AR*bToBB / aToBB;		//Բ׶�ĸ�
	Point3D OB = v4 - O;				//BO����
	//��ȡָ��Բ׶����ķ�������
	if (BR < AR)//B�ڵ���Բ����
	{
		//���OB�������᷽�������нǴ���90�ȣ��򽫷���������ת180��
		if (angleBetween(OB, dirO)>PI / 2)
		{
			dirO.x = -dirO.x;
			dirO.y = -dirO.y;
			dirO.z = -dirO.z;
		}
	}
	else
	{
		//���OB�������᷽�������н�С��90�ȣ��򽫷���������ת180��
		if (angleBetween(OB, dirO) < PI / 2)
		{
			dirO.x = -dirO.x;
			dirO.y = -dirO.y;
			dirO.z = -dirO.z;
		}
	}
	dirO = dirO.normalize();

	Point3D top = O + dirO *= h;		//Բ׶��������

	coefficient[0] = top.x;	//��������
	coefficient[1] = top.y;
	coefficient[2] = top.z;
	coefficient[3] = O.x;	//Բ׶��������һ��
	coefficient[4] = O.y;
	coefficient[5] = O.z;
	coefficient[6] = theta;	//ĸ����������ļн�
}

//*****************************��������*****************************
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
	// ���ڴ������Ԫ�ص�ֵ  
	double main_element = 0.0;
	// ���ڴ������Ԫ��������  
	int main_line = 0;

	// �м���������ڽ���  
	double temp = 0.0;
	// ѭ������  
	int i, j;

	// �ݶ�A[k, k]Ϊ����Ԫ��  
	main_element = A[k_row * rows + k_row];
	main_line = k_row;

	for (i = k_row + 1; i < rows; ++i)
	{
		if (fabs(A[i * rows + k_row]) > fabs(main_element))
		{
			main_element = A[i * rows + k_row];
			main_line = i;
		}

		// �����k��Ԫ���о���ֵ���Ĳ���a[k��k]���򽻻���������  
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
	// ѭ������  
	int i, j, k;
	// ��˹��Ԫ��������  
	double c;

	// ��Ԫ  
	for (k = 0; k < rows; ++k)
	{
		// ѡȡ����Ԫ��  
		columnPrimaryElement(A, B, rows, k);
		for (i = k + 1; i < rows; ++i)
		{
			// ��ø�˹��Ԫ��������  
			c = A[i * rows + k] / A[k * rows + k];

			for (j = k + 1; j < rows; ++j)
			{
				A[i * rows + j] = A[i * rows + j] - A[k * rows + j] * c;
			}
			B[i] = B[i] - B[k] * c;
		}
	}

	// �н������ж�  
	// ϵ������A���ȵ���A��ά��n(��������������)  
	if (fabs(A[(rows - 1) * rows + (rows - 1)]) < 10e-6)
	{
		// ������Ψһ��  
		printf("������Ψһ��!\n");

		for (i = 0; i < rows; ++i)
		{
			X[i] = 0.0;
		}
		return;
	}

	// �ش����  
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
	// ��ȡ�����ڵ�
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

	// ��ƽ��ֵ
	mu = mu / (double)neighbors.size();
	Matrix Q = P - Matrix::ones(neighbors.size(), 1)*mu;

	// ����ֵ�ֽ�
	Matrix H = ~Q*Q;
	Matrix U, W, V;
	H.svd(U, W, V);

	// ��ȡ����
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

//*****************************�㵽��ľ���*****************************
double Fitting::pointToLine(Point3D v, double* coefficient)
{
	Point3D A, B;//ֱ����һ�㵽ֱ�ߵ��������Լ�ֱ�ߵ�һ����������
	A.x = v.x - coefficient[0];
	A.y = v.y - coefficient[1];
	A.z = v.z - coefficient[2];
	B.x = coefficient[3];
	B.y = coefficient[4];
	B.z = coefficient[5];

	Point3D crossProduct = Point3D(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - B.x*A.y);
	double area = crossProduct.length(); //�ı������
	double bottom = B.length();//�׳�

	return area / bottom;
}

double  Fitting::pointToPlane(Point3D v, double* coefficient)
{
	return abs(coefficient[0] * v.x + coefficient[1] * v.y + coefficient[2] * v.z + coefficient[3]);
}

//*****************************���*****************************
void Fitting::planeFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[4];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	//����ƽ�淽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[4];

		//��ѡ��ĵ��������ѡ������
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//����ƽ�淽��
		if (getPlane(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double dis;				//�㵽ƽ��ľ���
		int inliersCount = 0;	//�ڵ�����
		//double temp = sqrt(coeCur[0] * coeCur[0] + coeCur[1] * coeCur[1] + coeCur[2] * coeCur[2]);

		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToPlane(vertexs[idxFit[i]], coeCur);
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			coeOptimum[0] = coeCur[0];
			coeOptimum[1] = coeCur[1];
			coeOptimum[2] = coeCur[2];
			coeOptimum[3] = coeCur[3];
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 5)
	{
		coe[0] = coeOptimum[0];
		coe[1] = coeOptimum[1];
		coe[2] = coeOptimum[2];
		coe[3] = coeOptimum[3];
		//���������ڵ�
		double dis;				//�㵽ƽ��ľ���
		vector<int> idxInliers;
		//int temp = sqrt(coe[0] * coe[0] + coe[1] * coe[1] + coe[2] * coe[2]);
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(coe[0] * vertexs[idxFit[i]].x + coe[1] * vertexs[idxFit[i]].y + coe[2] * vertexs[idxFit[i]].z + coe[3]);// / temp;
			//�������С����ֵ�������ڵ�
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

	double coeOptimum[6];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	//����ֱ�߷��̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[6];

		//��ѡ��ĵ��������ѡ������
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getLine(vertexs[a], vertexs[b], coeCur) == false)
			continue;

		double dis;				//�㵽ֱ�ߵľ���
		int inliersCount = 0;	//�ڵ�����
		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToLine(vertexs[idxFit[i]], coeCur);
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 6; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 6; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//���������ڵ�
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = pointToLine(vertexs[idxFit[i]], coe);
			//�������С����ֵ�������ڵ�
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

	double coeOptimum[16];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����


	//��Բ���̲��Ե�
	//getEllipse(Point3D(2, 0, 0), Point3D(-2, 0, 0), Point3D(0, 1, 0), Point3D(0, -1, 0), Point3D(1, 1.73205081, 0), coe);

	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeEll[16];//��ǰ��Բ��ƽ��ϵ��

		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];
		int e = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getEllipse(vertex[a], vertex[b], vertex[c], vertex[d], vertex[e], coeEll) == false)
			continue;

		double coePlane[4];//��ǰƽ��ϵ��
		coePlane[0] = coeEll[0];
		coePlane[1] = coeEll[1];
		coePlane[2] = coeEll[2];
		coePlane[3] = coeEll[3];

		Point3D c1(coeEll[10], coeEll[11], coeEll[12]);//��Բ��������
		Point3D c2(coeEll[13], coeEll[14], coeEll[15]);

		double dis_point_plane;//�㵽��Բ������ƽ��ľ���
		double dis_point_c;//�㵽��Բ������������
		double inliersCount = 0;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis_point_plane = pointToPlane(vertex[idxFit[i]], coePlane);
			dis_point_c = dist(vertex[idxFit[i]], c1) + dist(vertex[idxFit[i]], c2) - 2 * coeEll[6];

			//����㵽��Բ����ƽ��ľ��롢�㵽��Բ�������֮��С����ֵ�������ڵ�
			if (dis_point_plane < max_dis && dis_point_c < max_dis)
			{
				inliersCount++;
			}
		}

		// �����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
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

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
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

		double coePlane[4];//��ǰƽ��ϵ��

		coePlane[0] = coe[0];
		coePlane[1] = coe[1];
		coePlane[2] = coe[2];
		coePlane[3] = coe[3];

		Point3D c1(coe[10], coe[11], coe[12]);//��Բ��������
		Point3D c2(coe[13], coe[14], coe[15]);

		//���������ڵ�
		double dis_point_plane;//�㵽��Բ������ƽ��ľ���
		double dis_point_c;//�㵽��Բ������������
		vector<int> idxInliers;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis_point_plane = pointToPlane(vertex[idxFit[i]], coePlane);
			dis_point_c = dist(vertex[idxFit[i]], c1) + dist(vertex[idxFit[i]], c2) - 2 * coe[6];
			//����㵽��Բ����ƽ��ľ��롢�㵽��Բ�������֮��С����ֵ�������ڵ�
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

	double coeOptimum[8];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����

	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[8];//���ƽ�淽�̵�ϵ��
		double p_coeCur[4];//ƽ�淽�̵�ϵ��
		//��ѡ��ĵ��������ѡ���
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getCircular(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double p_dis;				//�㵽ƽ��ľ���
		double s_dis;				//�㵽����ľ���
		int inliersCount = 0;	//�ڵ�����
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coeCur[i + 4];
		}
		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
			s_dis = abs(dist(vertexs[idxFit[i]], Point3D(coeCur[0], coeCur[1], coeCur[2])) - coeCur[3]);
			//�������С����ֵ�������ڵ�
			if ((p_dis < max_dis) && (s_dis < max_dis))
			{
				inliersCount++;
			}
		}

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 8; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 1000)
	{
		for (int i = 0; i < 8; i++)
		{
			coe[i] = coeOptimum[i];
		}
		double p_coeCur[4];//ƽ�淽�̵�ϵ��
		//���������ڵ�
		double p_dis;				//�㵽ƽ��ľ���
		double s_dis;				//�㵽����ľ���
		vector<int> idxInliers;
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coe[i + 4];
		}
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
			s_dis = abs(dist(vertexs[idxFit[i]], Point3D(coe[0], coe[1], coe[2])) - coe[3]);
			//�������С����ֵ�������ڵ�
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

	double coeOptimum[4];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[4];

		//��ѡ��ĵ��������ѡ���
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getSphere(vertexs[a], vertexs[b], vertexs[c], vertexs[d], coeCur) == false)
			continue;

		double dis;				//�㵽����ľ���
		int inliersCount = 0;	//�ڵ�����

		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(dist(Point3D(coeCur[0], coeCur[1], coeCur[2]), vertexs[idxFit[i]]) - coeCur[3]);
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			coeOptimum[0] = coeCur[0];
			coeOptimum[1] = coeCur[1];
			coeOptimum[2] = coeCur[2];
			coeOptimum[3] = coeCur[3];
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 5)
	{
		coe[0] = coeOptimum[0];
		coe[1] = coeOptimum[1];
		coe[2] = coeOptimum[2];
		coe[3] = coeOptimum[3];
		//���������ڵ�
		double dis;
		vector<int> idxInliers;

		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(dist(Point3D(coe[0], coe[1], coe[2]), vertexs[idxFit[i]]) - coe[3]);
			//�������С����ֵ�������ڵ�
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

	double coeOptimum[7];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	double error = 0.0;
	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//Բ������
		double l_coeCur[6];	//�����߷��̵�ϵ��

		//��ѡ��ĵ��������ѡ���
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getCylinder(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double dis;				//�㵽�����ߵľ���
		int inliersCount = 0;	//�ڵ�����
		double d_error = 0.0;	//��ǰ�������
		for (int i = 0; i < 6; i++)
		{
			l_coeCur[i] = coeCur[i];
		}

		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], l_coeCur) - coeCur[6]);//�㵽Բ����ľ���
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				d_error += dis;//��¼���
				inliersCount++;
			}
		}
		d_error /= inliersCount;

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
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

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 7; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//���������ڵ�
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], coe) - coe[6]);//�㵽Բ����ľ���
			//�������С����ֵ�������ڵ�
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

	//------------������ѡ�㼯�ĸ�˹��-----------
	kdtree::KDTree*     M_tree;	// ģ�͵��Ƶ�KD��
	kdtree::KDTreeArray M_data;	//ģ�͵��Ƶ�����
	// ��ģ�͵㿽����M_data
	M_data.resize(idxFit.size());
	for (int i = 0; i < idxFit.size(); i++)
	{
		M_data[i].resize(3);//���ڴ洢M�Ķ���
	}
	for (int m = 0; m < idxFit.size(); m++)//��ȡM�����ж���
	{
		M_data[m][0] = vertexs[idxFit[m]].x;
		M_data[m][1] = vertexs[idxFit[m]].y;
		M_data[m][2] = vertexs[idxFit[m]].z;
	}
	M_tree = new kdtree::KDTree(M_data);//����ģ��M�����KD��
	double* nor = computeNormals(10, M_tree);//��˹���ڵĵ㼯
	vector<Point3D> norGauss;
	for (int i = 0; i < idxFit.size(); i++)
	{
		norGauss.push_back(Point3D(nor[i * 3 + 0], nor[i * 3 + 1], nor[i * 3 + 2]));
	}

	//norGauss = vertexs;
	//--------�������㷨����--------
	Point3D normal;				//���Բ����������Բ�������᷽������
	vector<int> idxCir;			//��˹�������Բ���ڵ�
	double cirOptimum[8];		//��ǰ���ŷ���
	int cirmaxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[8];//���ƽ�淽�̵�ϵ��

		//---------����Բ����---------
		Point3D v1 = norGauss[rand() % norGauss.size()];//��һ��
		Point3D v2 = norGauss[rand() % norGauss.size()];//��һ��
		if (v1 == v2)
			continue;
		Point3D center(0.0, 0.0, 0.0);//Բ������
		double r = 1.0;
		//��ȡԲ����ƽ��
		double plane[4];//����ƽ�淽��ϵ��
		if (getPlane(v1, v2, center, plane) == false)
			continue;
		coeCur[0] = 0;				//Բ��
		coeCur[1] = 0;
		coeCur[2] = 0;
		coeCur[3] = 1.0;			//�뾶
		coeCur[4] = plane[0];		//ƽ��ϵ��
		coeCur[5] = plane[1];
		coeCur[6] = plane[2];
		coeCur[7] = 0;

		//-----------��ȡ��ѷ���----------
		double p_coeCur[4];//ƽ�淽�̵�ϵ��
		double p_dis;				//�㵽ƽ��ľ���
		double s_dis;				//�㵽����ľ���
		int inliersCount = 0;	//�ڵ�����
		for (int i = 0; i < 4; i++)
		{
			p_coeCur[i] = coeCur[i + 4];
		}
		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			p_dis = pointToPlane(norGauss[i], p_coeCur);
			s_dis = abs(dist(norGauss[i], Point3D(coeCur[0], coeCur[1], coeCur[2])) - coeCur[3]);
			//�������С����ֵ�������ڵ�
			if ((p_dis < max_dis) && (s_dis < max_dis))
			{
				inliersCount++;
			}
		}
		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>cirmaxInliersCount)
		{
			cirmaxInliersCount = inliersCount;
			for (int i = 0; i < 8; i++)
			{
				cirOptimum[i] = coeCur[i];
			}
		}
	}
	//---------ȡ��˹����Բ���ڵ�--------
	double p_coeCur[4];//ƽ�淽�̵�ϵ��
	//���������ڵ�
	double p_dis;				//�㵽ƽ��ľ���
	double s_dis;				//�㵽����ľ���
	for (int i = 0; i < 4; i++)
	{
		p_coeCur[i] = cirOptimum[i + 4];
	}
	for (int i = 0; i < idxFit.size(); i++)
	{
		p_dis = pointToPlane(vertexs[idxFit[i]], p_coeCur);
		s_dis = abs(dist(vertexs[idxFit[i]], Point3D(cirOptimum[0], cirOptimum[1], cirOptimum[2])) - cirOptimum[3]);
		//�������С����ֵ�������ڵ�
		if ((p_dis < max_dis) && (s_dis < max_dis))
		{
			idxCir.push_back(idxFit[i]);
		}
	}

	//idxFit = idxCir;
	normal = Point3D(cirOptimum[4], cirOptimum[5], cirOptimum[6]);

	delete M_tree;
	//----------����������Ͱ뾶------------
	double coeOptimum[7];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//Բ������
		double l_coeCur[6];	//�����߷��̵�ϵ��

		//��ѡ��ĵ��������ѡ���
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];

		//���㷽��
		if (getCylinder(vertexs[a], vertexs[b], vertexs[c], coeCur) == false)
			continue;

		double angle = angleBetween(Point3D(coeCur[3], coeCur[4], coeCur[5]), normal);
		if (!(angle > PI - 0.01 || angle < 0.01))
			continue;

		double dis;				//�㵽�����ߵľ���
		int inliersCount = 0;	//�ڵ�����
		double d_error = 0.0;	//��ǰ�������
		for (int i = 0; i < 6; i++)
		{
			l_coeCur[i] = coeCur[i];
		}

		//���������ڵ�
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], l_coeCur) - coeCur[6]);//�㵽Բ����ľ���
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				d_error += dis;//��¼���
				inliersCount++;
			}
		}
		d_error /= inliersCount;

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 7; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 100)
	{
		for (int i = 0; i < 7; i++)
		{
			coe[i] = coeOptimum[i];
		}
		//���������ڵ�
		double dis;
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			dis = abs(pointToLine(vertexs[idxFit[i]], coe) - coe[6]);//�㵽Բ����ľ���
			//�������С����ֵ�������ڵ�
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

	double coeOptimum[7];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����

	kdtree::KDTree*     M_tree;	// ģ�͵��Ƶ�KD��
	kdtree::KDTreeArray M_data;	//ģ�͵��Ƶ�����

	// ��ģ�͵㿽����M_data
	M_data.resize(idxFit.size());
	for (int i = 0; i < idxFit.size(); i++)
	{
		M_data[i].resize(3);//���ڴ洢M�Ķ���
	}

	for (int m = 0; m < idxFit.size(); m++)//��ȡM�����ж���
	{
		M_data[m][0] = vertexs[idxFit[m]].x;
		M_data[m][1] = vertexs[idxFit[m]].y;
		M_data[m][2] = vertexs[idxFit[m]].z;
	}

	M_tree = new kdtree::KDTree(M_data);//����ģ��M�����KD��

	double* N = computeNormals(10, M_tree);

	delete M_tree;

	return N;
}

void Fitting::coneFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter, double max_dis)
{
	if (coe == NULL)
		return;

	double coeOptimum[7];		//��ǰ���ŷ���
	int maxInliersCount = 0;	//��ǰ����ڵ�����,���ŷ����ڵ�����
	//���㷽�̣�������ȡ���ŷ���
	for (int i = 0; i < max_iter; i++)
	{
		double coeCur[7];	//Բ׶����
		double l_coeCur[6];	//�����߷��̵�ϵ��
		//��ѡ��ĵ��������ѡ���
		int a = idxFit[rand() % idxFit.size()];
		int b = idxFit[rand() % idxFit.size()];
		int c = idxFit[rand() % idxFit.size()];
		int d = idxFit[rand() % idxFit.size()];

		//���㷽��
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

		Point3D T(coeCur[0], coeCur[1], coeCur[2]);//Բ׶��������
		Point3D TV, TO;			//���붥��������������᷽��������ָ��Բ׶�ڣ�
		double _theta;			//����������нǣ������жϵ��Ƿ���Բ׶�����·�
		double r;				//�����Ӧ�İ뾶
		double pToL;			//�㵽���������
		double h;				//��ǰ���Ӧ�ĸ�

		double dis;				//�㵽Բ׶��ľ���
		int inliersCount = 0;	//�ڵ�����

		//���������ڵ�
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
			dis = abs(pToL - r);//�㵽Բ׶��ľ���
			//�������С����ֵ�������ڵ�
			if (dis < max_dis)
			{
				inliersCount++;
			}
		}

		//�����ǰ�����ڵ�������࣬�����ŷ��̵�ǰ����
		if (inliersCount>maxInliersCount)
		{
			maxInliersCount = inliersCount;
			for (int i = 0; i < 7; i++)
			{
				coeOptimum[i] = coeCur[i];
			}
		}
	}

	//���ŷ��̵Ķ�������С����ֵ����ѡ��ĵ���û�п�����ϵ�ͼ��
	if (maxInliersCount > idxFit.size() / 100)
	{
		double l_coeCur[6];	//�����߷��̵�ϵ��
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
		//���������ڵ�
		Point3D T(coe[0], coe[1], coe[2]);//Բ׶��������
		Point3D TV, TO;			//���붥��������������᷽��������ָ��Բ׶�ڣ�
		double _theta;			//����������нǣ������жϵ��Ƿ���Բ׶�����·�
		double r;				//�����Ӧ�İ뾶
		double pToL;			//�㵽���������
		double h;				//��ǰ���Ӧ�ĸ�
		double dis;				//�㵽Բ׶��ľ���
		vector<int> idxInliers;
		for (int i = 0; i < idxFit.size(); i++)
		{
			TV = Point3D(vertexs[idxFit[i]] - T);
			TO = Point3D(l_coeCur[3], l_coeCur[4], l_coeCur[5]);
			_theta = angleBetween(TV, TO);
			pToL = pointToLine(vertexs[idxFit[i]], l_coeCur);
			h = pToL / tan(_theta);
			r = h*tan(coe[6]);
			dis = abs(pToL - r);//�㵽Բ׶��ľ���
			//�������С����ֵ�������ڵ�
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