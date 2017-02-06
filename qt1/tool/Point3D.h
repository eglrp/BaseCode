#pragma once
#include"stdafx.h"

/*����ȿ��Ա�ʾһ����ά�ĵ㣬Ҳ���Ա�ʾһ����ά�е�����*/
class Point3D{

	/*�����ά���꣬Ҳ���Ա�ʾһ����ά����*/
public:
	double x, y, z;
	
	/*���캯��*/
public:

	/*�޲����Ĺ��캯��*/
	Point3D(){}
	
	/*�������Ĺ��캯��*/
	Point3D(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	/*���ص�Ԫ�����*/
public:
	
	/*��Ԫ���������ά�����ļӷ�*/
	Point3D operator += (const Point3D others) const {
		return Point3D(x + others.x, y + others.y, z + others.z);
	}
	
	/*��Ԫ���������ά�����ļ���*/
	Point3D operator -= (const Point3D others) const {
		return Point3D(x - others.x, y - others.y, z - others.z);
	}
	
	/*��Ԫ���������ά����������*/
	Point3D operator *= (const double times) const {
		return Point3D(x * times, y * times, z * times);
	}
	
	/*��Ԫ���������ά��������������������Ϊ0*/
	Point3D operator /= (const double times) const {
		return Point3D(x / times, y / times, z / times);

	}

	/*��ά���ϵĻ������㷽��*/
public:

	/*�����ĳ���*/
	double length() {
		return sqrt(x * x + y * y + z * z);
	}
	/*������λ��*/
	Point3D normalize() {
		double len = length();
		return Point3D(x / len, y / len, z / len);
	}
};

/*��Ԫ�������������ά�����ļӷ�*/
Point3D operator + (const Point3D a, const Point3D b);

/*��Ԫ�������������ά�����ļӷ�*/
Point3D operator - (const Point3D a, const Point3D b);

/*��Ԫ�������������ά�����Ĳ��*/
Point3D operator * (const Point3D a, const Point3D b);

/*��Ԫ�������������ά��������ȹ�ϵ*/
bool operator == (const Point3D a, const Point3D b);

/*��Ԫ�������������ά��������ȹ�ϵ*/
bool operator != (const Point3D a, const Point3D b);

/*������ά�����ĵ��*/
double dotProduct(const Point3D a, const Point3D b);

/*�����������ɵ������ε����*/
double getTriangleArea(const Point3D a, const Point3D b, const Point3D c);

/*��������֮�乹�ɵĽǶȴ�С*/
double angleBetween(Point3D a, Point3D b);

/*����oe������os���ɵĽǶȴ�С*/
double angleBetween(Point3D o, Point3D s, Point3D e);

/*����֮��ľ���*/
double dist(Point3D a, Point3D b);

/*�ж�a, b, c�����Ƿ��ߣ����߷���true�� ���򷵻�false*/
bool isCollineation(Point3D a, Point3D b, Point3D c);
