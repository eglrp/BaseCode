#ifndef ICP_POINT_TO_PLANE_H
#define ICP_POINT_TO_PLANE_H

#include "icp.h"

class IcpPointToPlane : public Icp {

public:
	IcpPointToPlane(double *M, vector<int> denser, vector<vector<int>> rel, vector<Point3D> vex, vector<Face> facem, const int32_t M_num, const int32_t dim, const int32_t num_neighbors = 10, const double flatness = 5.0) : Icp(M, denser, rel, vex, facem, M_num, dim) {
		M_normal = computeNormals(num_neighbors, flatness);
	}

  virtual ~IcpPointToPlane () {
    delete M_normal;
  }

private:
	double fitStep(double *T, const int32_t T_num, Matrix &R, Matrix &t, const std::vector<int32_t> &active);
	std::vector<int32_t> getInliers(double *T, const int32_t T_num, const Matrix &R, const Matrix &t, const double indist);
  
  /* 计算点云的点法向量*/
  void computeNormal (const kdtree::KDTreeResultVector &neighbors,double *M_normal,const double flatness);
  double* computeNormals (const int num_neighbors,const double flatness);
  
  /* 模型点云的点法向量*/
  double *M_normal;
};

#endif // ICP_POINT_TO_PLANE_H
