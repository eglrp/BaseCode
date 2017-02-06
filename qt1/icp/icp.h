#ifndef ICP_H
#define ICP_H

#pragma warning(disable : 4996)

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "tool/Face.h"
#include "tool/Point3D.h"
#include "icp/matrix.h"
#include "kdtree.h"

class Icp {

public:
	/*
	*  ���캯��
	*  ���룺 M ....... ָ��ģ�͵��Ƶ�ָ��
	*         M_num ... ģ�͵������
	*         dim   ... ģ�͵��ά�� (2 �� 3)
	*/
	Icp(double *M, vector<int> denser, vector<vector<int>> rel, vector<Point3D> vex, 
		vector<Face> facem, const int M_num, const int dim);

	/********cwh*****s*******/
	vector<int> denseRelation;
	vector<vector<int>> relation;
	vector<Face> faces;
	vector<Point3D> vertex;
	/********cwh*****e*******/

	/* �������� */
	virtual ~Icp();

	/* ���ò������� (1. stage)*/
	void setSubsamplingStep(int val) { sub_step = val; }

	/* �������������� (1. stopping criterion)*/
	void setMaxIterations(int val) { max_iter = val; }

	/* ������С������ֵ (2. stopping criterion)*/
	void setMinDeltaParam(double  val) { min_delta = val; }

	/*
	* ִ�е�����׼��������ת����R��ƽ������t (M = R*T + t)
	* ���룺  T ....... ָ�򳡾����Ƶ�ָ��
	*         T_num ... �����������
	*         R ....... ���ص���ת����
	*         t ....... ���ص�ƽ������
	*         indist .. ������ (��� <=0����У�����)
	* �����  R ....... ��ת����
	*         t ....... ƽ������
	*/
	void fit(double *T, const int T_num, Matrix &R, Matrix &t, const double indist, vector<double> &offset);

	int getMaxIter();
	double getMinDelta();
	Matrix getMCentroid();
	double getDelta();
private:
	/* ����׼�����ĵ��غ� */
	void fitCoarse(double *T, const int T_num, Matrix &t);
	/* ������׼ */
	void fitIterate(double *T, const int T_num, Matrix &R, Matrix &t, const std::vector<int> &active);

	/* ���麯��������һ�ε���������Ŀ�꺯��ֵ */
	virtual double               fitStep(double *T, const int T_num, Matrix &R, Matrix &t, const std::vector<int> &active) = 0;
	virtual std::vector<int> getInliers(double *T, const int T_num, const Matrix &R, const Matrix &t, const double indist) = 0;

protected:
	kdtree::KDTree*     M_tree;	// ģ�͵��Ƶ�KD��
	kdtree::KDTreeArray M_data;	//ģ�͵��Ƶ�����

	int dim;		// ���Ƶ�ά�� (2 or 3)
	int sub_step;	// ��������
	int max_iter;	// ����������
	double  min_delta;	// ��С������ֵ
	double  delta;		// ��׼���

	Matrix M_Centroid; // ��������
};

#endif // ICP_H
