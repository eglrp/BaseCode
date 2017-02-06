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
	* 点云拟合平面方程
	* 输入：	vertexs ------ 待拟合的点集
	*			idxFit ------- 返回的拟合用的点在点集内的索引
	*			coe ---------- 返回的平面方程系数，Ax+By+Cz+D=0
	*			max_iter ----- 最大迭代次数
	*			max_dis ------ 阈值距离，点到平面的最大距离
	*/
	static void planeFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	static void lineFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.003);
	static void circularFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	static void sphereFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	//圆柱拟合
	static void cylinderFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	//使用高斯面限制方向的圆柱拟合
	static void cylinderFitting1(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.001);
	static double* cylinderFitting2(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.00001);

	static void ellipseFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.0001);
	//圆锥拟合
	static void coneFitting(const vector<Point3D> &vertexs, vector<int> &idxFit, double* coe, int max_iter = 500, double max_dis = 0.005);

	/*
	* 计算点到直线的距离。
	* 直线外一点与直线上两点组成平行四边形，计算面积进而获得高。
	*/
	static double pointToLine(Point3D v, double* coefficient);

	/*
	* 计算点到面的距离。
	*/
	static double pointToPlane(Point3D v, double* coefficient);

	/*
	* 计算直线方程，點向式。
	* 点v1和向量v2v1表达方程。
	* 系数(6)：过的点坐标+方向向量
	*/
	static bool getLine(Point3D v1, Point3D v2, double* coefficient);

	/*
	* 计算平面方程，计算平面法向量，然后代入平面内一点，得点法式方程。
	* 输入：	v1,v2,v3 ---------- 平面内三点坐标
	*			coefficient ------- 返回的平面方程系数，Ax+By+Cz+D=0
	* 输出：	是否存在平面方程
	*/
	static bool getPlane(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*
	* 计算圆方程，由球面方程与平面方程组成。
	* 系数(8)：圆心+半径+所在平面（平面方程）
	*/
	static bool getCircular(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*计算椭圆方程
	*
	*/
	static bool getEllipse(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, double* coefficient);

	/*
	* 计算球面方程。
	* 利用圆心坐标与四点距离相等。
	* 系数(4)：球心+半径
	*/
	static bool getSphere(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient);

	/*
	* 计算圆柱方程，由中轴线与半径组成。
	* 系数(7)：中轴线（直线方程.点向式）+半径
	*/
	static bool getCylinder(Point3D v1, Point3D v2, Point3D v3, double* coefficient);

	/*
	* 计算圆锥方程，由中轴线（两点式）与母线和中轴线的夹角组成。
	* 系数(7)：中轴线（顶点和圆锥内轴线上一点）+ 夹角
	*/
	static bool getCone(Point3D v1, Point3D v2, Point3D v3, Point3D v4, double* coefficient);

	/*
	* 计算两个数的最大公约数
	*/
	static int gcd(int a, int b);

	/*
	* 功能:选取列主元素.
	* 输入:A		系数矩阵A；
	*      B		常数列向量B；
	*      rows		系数矩阵A的行数；
	*      k_row	主元素所在的行列数
	* 输出:void
	*/
	static void columnPrimaryElement(double *A, double *B, int rows, int k_row);
	/*
	* 功能:列主元素高斯消元法解n元一次方程组，n阶矩阵.
	* 输入:A		系数矩阵A；
	*      B		常数列向量B；
	*      rows		系数矩阵A的行数；
	* 输出:X		结果向量
	*/
	static void gauss_ColumnPrimaryElement(double *A, double *B, int rows, double *X);

	/* 计算点云的点法向量*/
	static void computeNormal(const kdtree::KDTreeResultVector &neighbors, double *M_normal, kdtree::KDTree* M_tree);
	static double* computeNormals(const int num_neighbors, kdtree::KDTree* M_tree);
};

#endif