#include "PointModel.h"


PointModel::PointModel()
{
}


PointModel::~PointModel()
{
}

void PointModel::createCylinder(double radius, double height, vector<Point3D> &points, Point3D center, double w_step, double h_step)
{
	if (radius == 0)
		return;

	center = center.normalize();
	int hN = height / h_step;
	int wN = 2 * radius*PI / w_step;

	Point3D c_normal(center.x, center.y, center.z);	//中心轴向量
	Point3D xy_normal(0, 0, 1);					//z轴方向向量
	double rotationAngle = angleBetween(c_normal, xy_normal);
	Point3D rotationAxis = c_normal * xy_normal;
	srand(time(NULL));
	//绘制圆柱，圆柱底面位于xy平面原点，中心轴为z轴
	for (int i = 1; i < hN; i++)		//中心轴方向
	{
		for (int j = 1; j < wN; j++)	//圆周
		{
			//int sw = rand() % wN;
			//int sh = rand() % hN;
			int sw = wN;
			int sh = i;
			double x = radius * cos(2 * PI / sw*j);
			double y = radius * sin(2 * PI / sw*j);
			double z = h_step*sh - height / 2;
			points.push_back(Point3D(x, y, z));
		}
	}
	//将圆柱旋转到相应方向
	if (rotationAngle != 0)
	{
		vector<Point3D> _points;
		for (int i = 0; i < points.size(); i++)
		{
			Quaternion qua(points[i].x, points[i].y, points[i].z);
			qua = qua.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
			_points.push_back(Point3D(qua.x, qua.y, qua.z));
		}
		points = _points;
	}
}

void PointModel::createEllipse(double a, double b, Point3D o, vector<Point3D> &points, Point3D axis, double angle, double a_step)
{
	if (a == 0 || b == 0)
		return;

	int N = 2 * PI / a_step;

	for (int i = 0; i < N; i++)	//圆周
	{
		int sw = N;
		double x = a * cos(2 * PI / sw*i) + o.x;
		double y = b * sin(2 * PI / sw*i) + o.y;
		double z = 0 + o.z;
		points.push_back(Point3D(x, y, z));
	}

	if (angle != 0)
	{
		vector<Point3D> _points;
		for (int i = 0; i < points.size(); i++)
		{
			Quaternion qua(points[i].x, points[i].y, points[i].z);
			qua = qua.rotational(angle, axis.x, axis.y, axis.z);
			_points.push_back(Point3D(qua.x, qua.y, qua.z));
		}
		points = _points;
	}
}

void PointModel::createCone(Point3D top, Point3D cpoint, double angle, double height, vector<Point3D> &points, double w_step, double h_step)
{
	if (height == 0)
		return;
	Point3D center = top -= cpoint;//轴线方向向量，方向为由底指向顶点
	center = center.normalize();
	int hN = height / h_step;	//中心轴的点数量
	double h = 0;				//当前圆锥的高

	Point3D c_normal(center.x, center.y, center.z);	//中心轴向量
	Point3D xy_normal(0, 0, 1);						//z轴方向向量
	double rotationAngle = angleBetween(c_normal, xy_normal);
	Point3D rotationAxis = c_normal * xy_normal;

	//绘制圆锥，先将圆锥的顶点位于原点，中心轴为z轴
	for (int i = 0; i < hN; i++)		//中心轴方向
	{
		double radius = h* tan(angle);
		int wN = 2 * radius*PI / w_step + 1;
		for (int j = 0; j < wN; j++)	//圆周方向
		{
			int sw = wN;
			int sh = i;
			double x = radius * cos(2 * PI / sw*j);
			double y = radius * sin(2 * PI / sw*j);
			double z = h_step*sh;
			points.push_back(Point3D(x, y, z));
		}
		h += (xy_normal *= h_step).length();
	}

	//旋转点云
	if (center != Point3D(0, 0, 1))
	{
		vector<Point3D> _points;
		for (int i = 0; i < points.size(); i++)
		{
			Quaternion qua(points[i].x, points[i].y, points[i].z);
			qua = qua.rotational(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
			_points.push_back(Point3D(qua.x, qua.y, qua.z) += top);//将旋转完成的点云移动至相应位置
		}
		points = _points;
	}
}