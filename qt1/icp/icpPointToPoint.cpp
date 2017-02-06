#include "icpPointToPoint.h"

using namespace std;

// Also see (3d part): "Least-Squares Fitting of Two 3-D Point Sets" (Arun, Huang and Blostein)
double IcpPointToPoint::fitStep (double *T,const int T_num,Matrix &R,Matrix &t,const std::vector<int> &active) {
  
  // kd树查询点和查询结果
  std::vector<float>         query(dim);
  kdtree::KDTreeResultVector result;
  
  // 初始化模型点集和场景点集
  Matrix p_m(active.size(),dim); 
  Matrix p_t(active.size(),dim);
  
  // 均值
  Matrix mu_m(1,dim);
  Matrix mu_t(1,dim);
  
  // dimensionality 2
  if (dim==2) {
  // dimensionality 3
  } else {
    
    //  获取旋转矩阵和平移向量
    double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
    double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
    double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
    double t0  = t.val[0][0]; double t1  = t.val[1][0]; double t2  = t.val[2][0];

    // 确立两个点云间的对应点
    for (int i=0; i<active.size(); i++) {

      // 当前点的索引
      int idx = active[i];

      // 获取当前场景点的最新位置
      query[0] = r00*T[idx*3+0] + r01*T[idx*3+1] + r02*T[idx*3+2] + t0;
      query[1] = r10*T[idx*3+0] + r11*T[idx*3+1] + r12*T[idx*3+2] + t1;
      query[2] = r20*T[idx*3+0] + r21*T[idx*3+1] + r22*T[idx*3+2] + t2;

      // 获取当前场景点在模型点云中的最近点，结果存放在result
      M_tree->n_nearest(query,1,result);

      // 处理模型点
      p_m.val[i][0] = M_tree->the_data[result[0].idx][0]; mu_m.val[0][0] += p_m.val[i][0];
      p_m.val[i][1] = M_tree->the_data[result[0].idx][1]; mu_m.val[0][1] += p_m.val[i][1];
      p_m.val[i][2] = M_tree->the_data[result[0].idx][2]; mu_m.val[0][2] += p_m.val[i][2];

      // 处理场景点
      p_t.val[i][0] = query[0]; mu_t.val[0][0] += p_t.val[i][0];
      p_t.val[i][1] = query[1]; mu_t.val[0][1] += p_t.val[i][1];
      p_t.val[i][2] = query[2]; mu_t.val[0][2] += p_t.val[i][2];
    }
  }
  
  // 减去均值
  mu_m = mu_m/(double)active.size();
  mu_t = mu_t/(double)active.size();
  Matrix q_m = p_m - Matrix::ones(active.size(),1)*mu_m;
  Matrix q_t = p_t - Matrix::ones(active.size(),1)*mu_t;

  // 计算旋转矩阵和平移向量
  Matrix H = ~q_t*q_m;
  Matrix U,W,V;
  H.svd(U,W,V);
  Matrix R_ = V*~U;
  Matrix t_ = ~mu_m - R_*~mu_t;
  
  // compose: R|t = R_|t_ * R|t
  R = R_*R;
  t = R_*t+t_;

  // 返回目标函数值
  if (dim==2) return max((R_-Matrix::eye(2)).l2norm(),t_.l2norm());
  else        return max((R_-Matrix::eye(3)).l2norm(),t_.l2norm());
}

std::vector<int> IcpPointToPoint::getInliers (double *T,const int T_num,const Matrix &R,const Matrix &t,const double indist) {

  // init inlier vector + query point + query result
  vector<int>            inliers;
  std::vector<float>         query(dim);
  kdtree::KDTreeResultVector neighbor;
  
  // dimensionality 2
  if (dim==2) {

  // dimensionality 3
  } else {
    
    // 获取旋转矩阵和平移向量
    double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
    double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
    double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
    double t0  = t.val[0][0]; double t1  = t.val[1][0]; double t2  = t.val[2][0];

    // 检测场景点是否是内点
    for (int i=0; i<T_num; i++) {

      // 将当前场景点变换到最新位置
      query[0] = r00*T[i*3+0] + r01*T[i*3+1] + r02*T[i*3+2] + t0;
      query[1] = r10*T[i*3+0] + r11*T[i*3+1] + r12*T[i*3+2] + t1;
      query[2] = r20*T[i*3+0] + r21*T[i*3+1] + r22*T[i*3+2] + t2;

      // 搜索当前点的最近点
      M_tree->n_nearest(query,1,neighbor);

      // 检测是否内点
      if (neighbor[0].dis<indist)
        inliers.push_back(i);
    }
  }
  
  // 返回内点
  return inliers;
}
