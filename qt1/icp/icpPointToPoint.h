#ifndef ICP_POINT_TO_POINT_H
#define ICP_POINT_TO_POINT_H

#include "icp.h"
#include "tool/BaseModel.h"
class IcpPointToPoint : public Icp {

public:
	IcpPointToPoint(double *M, vector<int> denser, vector<vector<int>> rel, vector<Point3D> vex, vector<Face> facem,
		const int M_num, const int dim, const int num_neighbors = 10, const double flatness = 5.0) :
		Icp(M, denser, rel, vex, facem, M_num, dim) {}
	virtual ~IcpPointToPoint () {}

private:

  double fitStep (double *T,const int T_num,Matrix &R,Matrix &t,const std::vector<int> &active);
  std::vector<int> getInliers (double *T,const int T_num,const Matrix &R,const Matrix &t,const double indist);
};

#endif // ICP_POINT_TO_POINT_H
