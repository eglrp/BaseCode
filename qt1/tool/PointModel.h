#pragma once

#include "Point3D.h"
#include "Quaternion.h"

using namespace std;

#define PI 3.14159265358979323846

class PointModel
{
public:
	PointModel();
	~PointModel();

public:
	/* 创建圆柱点云
	* 输入：	radius ------ 半径
	*			height ------ 高
	*			center ------ 中心轴
	*			w_step ------ 圆周方向步长
	*			h_step ------ 高度方向步长
	*/
	static void createCylinder(double radius, double height, vector<Point3D> &points, Point3D center = Point3D(0, 0, 1), double w_step = 0.01, double h_step = 0.01);

	/* 创建圆锥点云
	* 输入：	top    ------ 顶点坐标
	*			cpoint ------ 圆锥内轴线上一点
	*			angle  ------ 母线与中轴线的夹角(弧度值)
	*			height ------ 高
	*			w_step ------ 圆周方向步长
	*			h_step ------ 高度方向步长
	*/
	static void createCone(Point3D top, Point3D cpoint, double angle, double height, vector<Point3D> &points, double w_step = 0.01, double h_step = 0.01);

	/* 创建椭圆点云
	* 输入：	a ------ 长半轴
	*			b ------ 短半轴
	*			o ------ 中心点
	*			axis   ------ 旋转轴
	*			angle  ------ 旋转角
	*			a_step ------ 采样步长,旋转角方向
	*/
	static void createEllipse(double a, double b, Point3D o, vector<Point3D> &points, Point3D axis = Point3D(0, 0, 1), double angle = 0.0, double a_step = 0.01);
};

