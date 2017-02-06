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
	/* ����Բ������
	* ���룺	radius ------ �뾶
	*			height ------ ��
	*			center ------ ������
	*			w_step ------ Բ�ܷ��򲽳�
	*			h_step ------ �߶ȷ��򲽳�
	*/
	static void createCylinder(double radius, double height, vector<Point3D> &points, Point3D center = Point3D(0, 0, 1), double w_step = 0.01, double h_step = 0.01);

	/* ����Բ׶����
	* ���룺	top    ------ ��������
	*			cpoint ------ Բ׶��������һ��
	*			angle  ------ ĸ���������ߵļн�(����ֵ)
	*			height ------ ��
	*			w_step ------ Բ�ܷ��򲽳�
	*			h_step ------ �߶ȷ��򲽳�
	*/
	static void createCone(Point3D top, Point3D cpoint, double angle, double height, vector<Point3D> &points, double w_step = 0.01, double h_step = 0.01);

	/* ������Բ����
	* ���룺	a ------ ������
	*			b ------ �̰���
	*			o ------ ���ĵ�
	*			axis   ------ ��ת��
	*			angle  ------ ��ת��
	*			a_step ------ ��������,��ת�Ƿ���
	*/
	static void createEllipse(double a, double b, Point3D o, vector<Point3D> &points, Point3D axis = Point3D(0, 0, 1), double angle = 0.0, double a_step = 0.01);
};

