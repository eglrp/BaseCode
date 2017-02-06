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
	*  构造函数
	*  输入： M ....... 指向模型点云的指针
	*         M_num ... 模型点的数量
	*         dim   ... 模型点的维度 (2 或 3)
	*/
	Icp(double *M, vector<int> denser, vector<vector<int>> rel, vector<Point3D> vex, 
		vector<Face> facem, const int M_num, const int dim);

	/********cwh*****s*******/
	vector<int> denseRelation;
	vector<vector<int>> relation;
	vector<Face> faces;
	vector<Point3D> vertex;
	/********cwh*****e*******/

	/* 析构函数 */
	virtual ~Icp();

	/* 设置采样步长 (1. stage)*/
	void setSubsamplingStep(int val) { sub_step = val; }

	/* 设置最大迭代次数 (1. stopping criterion)*/
	void setMaxIterations(int val) { max_iter = val; }

	/* 设置最小收敛阈值 (2. stopping criterion)*/
	void setMinDeltaParam(double  val) { min_delta = val; }

	/*
	* 执行点云配准，计算旋转矩阵R和平移向量t (M = R*T + t)
	* 输入：  T ....... 指向场景点云的指针
	*         T_num ... 场景点的数量
	*         R ....... 返回的旋转矩阵
	*         t ....... 返回的平移向量
	*         indist .. 误差距离 (如果 <=0，则不校验误差)
	* 输出：  R ....... 旋转矩阵
	*         t ....... 平移向量
	*/
	void fit(double *T, const int T_num, Matrix &R, Matrix &t, const double indist, vector<double> &offset);

	int getMaxIter();
	double getMinDelta();
	Matrix getMCentroid();
	double getDelta();
private:
	/* 粗配准，中心点重合 */
	void fitCoarse(double *T, const int T_num, Matrix &t);
	/* 迭代配准 */
	void fitIterate(double *T, const int T_num, Matrix &R, Matrix &t, const std::vector<int> &active);

	/* 纯虚函数，进行一次迭代，返回目标函数值 */
	virtual double               fitStep(double *T, const int T_num, Matrix &R, Matrix &t, const std::vector<int> &active) = 0;
	virtual std::vector<int> getInliers(double *T, const int T_num, const Matrix &R, const Matrix &t, const double indist) = 0;

protected:
	kdtree::KDTree*     M_tree;	// 模型点云的KD树
	kdtree::KDTreeArray M_data;	//模型点云的数据

	int dim;		// 点云的维度 (2 or 3)
	int sub_step;	// 采样步长
	int max_iter;	// 最大迭代次数
	double  min_delta;	// 最小收敛阈值
	double  delta;		// 配准误差

	Matrix M_Centroid; // 点云质心
};

#endif // ICP_H
