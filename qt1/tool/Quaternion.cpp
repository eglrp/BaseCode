#include "Quaternion.h"


Quaternion::Quaternion()
{
	this->x = 1.0;
	this->y = 0.0;
	this->z = 0.0;
	this->r = 0.0;
}


Quaternion::~Quaternion()
{
}

Quaternion::Quaternion(double _x, double _y, double _z, double _r)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->r = _r;
}

Quaternion Quaternion::normalize()
{
	double temp = sqrt(x*x + y*y + z*z + r*r);
	return Quaternion(x / temp, y / temp, z / temp, r / temp);
}

void Quaternion::setVertex(double _x, double _y, double _z, double _r)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->r = _r;
}

Quaternion Quaternion::dot(Quaternion left, Quaternion right)
{
	Quaternion qua;
	double d1, d2, d3, d4;

	d1 = left.r * right.r;
	d2 = -left.x * right.x;
	d3 = -left.y * right.y;
	d4 = -left.z * right.z;
	qua.r = d1 + d2 + d3 + d4;

	d1 = left.r * right.x;
	d2 = right.r * left.x;
	d3 = left.y * right.z;
	d4 = -left.z * right.y;
	qua.x = d1 + d2 + d3 + d4;

	d1 = left.r * right.y;
	d2 = right.r * left.y;
	d3 = left.z * right.x;
	d4 = -left.x * right.z;
	qua.y = d1 + d2 + d3 + d4;

	d1 = left.r * right.z;
	d2 = right.r * left.z;
	d3 = left.x * right.y;
	d4 = -left.y * right.x;
	qua.z = d1 + d2 + d3 + d4;

	return qua;
}

Quaternion Quaternion::operator * (const Quaternion right)
{
	Quaternion qua;
	double d1, d2, d3, d4;

	d1 = this->r * right.r;
	d2 = -this->x * right.x;
	d3 = -this->y * right.y;
	d4 = -this->z * right.z;
	qua.r = d1 + d2 + d3 + d4;

	d1 = this->r * right.x;
	d2 = right.r * this->x;
	d3 = this->y * right.z;
	d4 = -this->z * right.y;
	qua.x = d1 + d2 + d3 + d4;

	d1 = this->r * right.y;
	d2 = right.r * this->y;
	d3 = this->z * right.x;
	d4 = -this->x * right.z;
	qua.y = d1 + d2 + d3 + d4;

	d1 = this->r * right.z;
	d2 = right.r * this->z;
	d3 = this->x * right.y;
	d4 = -this->y * right.x;
	qua.z = d1 + d2 + d3 + d4;

	return qua;
}

Quaternion Quaternion::getRotationalQuaternion(double radian, double _x, double _y, double _z)
{
	Quaternion qua;
	double temp;
	double cr, cv;

	qua.r = qua.x = qua.y = qua.z = 0.0;

	temp = _x *  _x + _y *  _y + _z *  _z;

	if (temp == 0.0)
		return qua;

	temp = 1.0 / sqrt(temp);
	_x *= temp;
	_y *= temp;
	_z *= temp;

	cr = cos(0.5 * radian);
	cv = sin(0.5 * radian);

	qua.r = cr;
	qua.x = cv * _x;
	qua.y = cv * _y;
	qua.z = cv * _z;

	return qua;
}

Quaternion Quaternion::rotational(double radian, double _x, double _y, double _z)
{
	if (radian == 0.0 || (_x == 0.0&&_y == 0.0&&_z == 0.0))
		return Quaternion(x, y, z, r);

	Quaternion p(x, y, z, r);
	Quaternion q = getRotationalQuaternion(radian, _x, _y, _z);
	Quaternion _q = getRotationalQuaternion(-radian, _x, _y, _z);

	p = q*p;
	p = p*_q;

	return p;
}

