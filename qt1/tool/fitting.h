#ifndef FITTING_H
#define FITTING_H

#include <iostream>
#include <vector>
#include "stdafx.h"
#include "Point3D.h"
#include "../icp/kdtree.h"
#include "../icp/matrix.h"

using namespace std;

class Fitting
{
public:
	Fitting();
	~Fitting();

public:
	/*
	* �������ƽ�淽��
	* ���룺	vertexs ------ ����ϵĵ㼯
	*			idxFit ------- ���ص�����õĵ��ڵ㼯�ڵ�����
	*			coe ---------- ���ص�ƽ�淽��ϵ����Ax+By+Cz+D=0
	*			max_iter ----- ����������
	*			max_dis ------ ��ֵ���룬�㵽ƽ���������
	*/
	static void planeFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	static void lineFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.003);
	static void circularFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	static void sphereFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	//Բ�����
	static void cylinderFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	//ʹ�ø�˹�����Ʒ����Բ�����
	static void cylinderFitting1(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	static double* cylinderFitting2(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.00001);

	static void ellipseFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	//Բ׶���
	static void coneFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.005);

	/*
	* ����㵽ֱ�ߵľ��롣
	* ֱ����һ����ֱ�����������ƽ���ı��Σ��������������øߡ�
	*/
	static double pointToLine(Point3D v, double* coefficient);

	/*
	* ����㵽��ľ��롣
	*/
	static double pointToPlane(Point3D v, double* coefficient);

	/*
	* ����ֱ�߷��̣��c��ʽ��
	* ��v1������v2v1��﷽�̡�
	* ϵ��(6)�����ĵ�����+��������
	*/
	static bool getLine(Point3D v1, Point3D v2, double* coefficient);

	/*
	* ����ƽ�淽�̣�����ƽ�淨������Ȼ�����ƽ����һ�㣬�õ㷨ʽ���̡�
	* ���룺	v1,v2,v3 ---------- ƽ������������
	*			coefficient ------- ���ص�ƽ�淽��ϵ����Ax+By+Cz+D=0
	* �����	�Ƿ����ƽ�淽��
	*/
	static bool getPlane(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*
	* ����Բ���̣������淽����ƽ�淽����ɡ�
	* ϵ��(8)��Բ��+�뾶+����ƽ�棨ƽ�淽�̣�
	*/
	static bool getCircular(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*������Բ����
	*
	*/
	static bool getEllipse(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, double* coefficient);

	/*
	* �������淽�̡�
	* ����Բ���������ĵ������ȡ�
	* ϵ��(4)������+�뾶
	*/
	static bool getSphere(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient);

	/*
	* ����Բ�����̣�����������뾶��ɡ�
	* ϵ��(7)�������ߣ�ֱ�߷���.����ʽ��+�뾶
	*/
	static bool getCylinder(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*
	* ����Բ׶���̣��������ߣ�����ʽ����ĸ�ߺ������ߵļн���ɡ�
	* ϵ��(7)�������ߣ������Բ׶��������һ�㣩+ �н�
	*/
	static bool getCone(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient);

	/*
	* ���������������Լ��
	*/
	static int gcd(int a, int b);

	/*
	* ����:ѡȡ����Ԫ��.
	* ����:A		ϵ������A��
	*      B		����������B��
	*      rows		ϵ������A��������
	*      k_row	��Ԫ�����ڵ�������
	* ���:void
	*/
	static void columnPrimaryElement(double *A, double *B, int rows, int k_row);
	/*
	* ����:����Ԫ�ظ�˹��Ԫ����nԪһ�η����飬n�׾���.
	* ����:A		ϵ������A��
	*      B		����������B��
	*      rows		ϵ������A��������
	* ���:X		�������
	*/
	static void gauss_ColumnPrimaryElement(double *A, double *B, int rows, double *X);

	/* ������Ƶĵ㷨����*/
	static void computeNormal(const kdtree::KDTreeResultVector &neighbors, double *M_normal, kdtree::KDTree* M_tree);
	static double* computeNormals(const int num_neighbors, kdtree::KDTree* M_tree);
};

#endif