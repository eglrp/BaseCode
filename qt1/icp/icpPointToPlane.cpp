#include "icpPointToPlane.h"
#include <windows.h>

using namespace std;

// Also see (3d part): "Linear Least-Squares Optimization for Point-to-Plane ICP Surface Registration" (Kok-Lim Low)
double IcpPointToPlane::fitStep (double *T,const int T_num,Matrix &R,Matrix &t,const std::vector<int> &active) {

  // kd����ѯ��Ͳ�ѯ���
  std::vector<float>         query(dim);
  kdtree::KDTreeResultVector result;

  // ��ʼ��ģ�͵㼯�ͳ����㼯
  Matrix p_m(active.size(),dim); // ģ��
  Matrix p_t(active.size(),dim); // ����
  
  // dimensionality 2
  if (dim==2) {
   // dimensionality 3
  } else {
    
    // ��ȡ��ת�����ƽ������
    double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
    double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
    double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
    double t0  = t.val[0][0]; double t1  = t.val[1][0]; double t2  = t.val[2][0];

    // ��ʼ�� A �� b
    Matrix A(active.size(),6);
    Matrix b(active.size(),1);

    // ȷ���������Ƽ�Ķ�Ӧ��
    for (int i=0; i<active.size(); i++) {

      // ��ǰ�������
      int idx = active[i];

      // ��ȡ��ǰ�����������λ��
      query[0] = r00*T[idx*3+0] + r01*T[idx*3+1] + r02*T[idx*3+2] + t0;
      query[1] = r10*T[idx*3+0] + r11*T[idx*3+1] + r12*T[idx*3+2] + t1;
      query[2] = r20*T[idx*3+0] + r21*T[idx*3+1] + r22*T[idx*3+2] + t2;

      // ��ȡ��ǰ��������ģ�͵����е�����㣬��������result
      M_tree->n_nearest(query,1,result);

      // ��ȡ�뵱ǰ�������Ӧ��ģ�͵�
      double dx = M_tree->the_data[result[0].idx][0];
      double dy = M_tree->the_data[result[0].idx][1];
      double dz = M_tree->the_data[result[0].idx][2];

      // ģ�͵�ķ�����
      double nx = M_normal[result[0].idx*3+0];
      double ny = M_normal[result[0].idx*3+1];
      double nz = M_normal[result[0].idx*3+2];

      // ��ǰ������
      double sx = query[0];
      double sy = query[1];
      double sz = query[2];

      // ��С���˷�
      A.val[i][0] = nz*sy-ny*sz;
      A.val[i][1] = nx*sz-nz*sx;
      A.val[i][2] = ny*sx-nx*sy;
      A.val[i][3] = nx;
      A.val[i][4] = ny;
      A.val[i][5] = nz;
      b.val[i][0] = nx*dx+ny*dy+nz*dz-nx*sx-ny*sy-nz*sz;    
    }

    // ������С���˷�
    Matrix A_ = ~A*A;
    Matrix b_ = ~A*b;

    // �����Է���
    if (b_.solve(A_)) {

      // ��ת����
      Matrix R_ = Matrix::eye(3);
      R_.val[0][1] = -b_.val[2][0];
      R_.val[1][0] = +b_.val[2][0];
      R_.val[0][2] = +b_.val[1][0];
      R_.val[2][0] = -b_.val[1][0];
      R_.val[1][2] = -b_.val[0][0];
      R_.val[2][1] = +b_.val[0][0];

      // ��������ת����
      Matrix U,W,V;
      R_.svd(U,W,V);
      R_ = U*~V;  

      // ƽ������
      Matrix t_(3,1);
      t_.val[0][0] = b_.val[3][0];
      t_.val[1][0] = b_.val[4][0];
      t_.val[2][0] = b_.val[5][0];

      // ���㲢����Ŀ�꺯��ֵ
      R = R_*R;
      t = R_*t+t_;
      return max((R_-Matrix::eye(3)).l2norm(),t_.l2norm());
    }
  }
  
  // ���ؼ���ʧ��
  return 0;
}

std::vector<int> IcpPointToPlane::getInliers (double *T,const int T_num,const Matrix &R,const Matrix &t,const double indist) {
  
   // init inlier vector + query point + query result
  vector<int>            inliers;
  std::vector<float>         query(dim);
  kdtree::KDTreeResultVector neighbor;
  
  // dimensionality 2
  if (dim==2) {
  // dimensionality 3
  } else {
    
    // ��ȡ��ת�����ƽ������
    double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
    double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
    double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
    double t0  = t.val[0][0]; double t1  = t.val[1][0]; double t2  = t.val[2][0];

    // ��ⳡ�����Ƿ����ڵ�
    for (int i=0; i<T_num; i++) {

      // ����ǰ������任������λ��
      double sx = r00*T[i*3+0] + r01*T[i*3+1] + r02*T[i*3+2] + t0; query[0] = sx;
      double sy = r10*T[i*3+0] + r11*T[i*3+1] + r12*T[i*3+2] + t1; query[1] = sy;
      double sz = r20*T[i*3+0] + r21*T[i*3+1] + r22*T[i*3+2] + t2; query[2] = sz;

      // ������ǰ��������
      M_tree->n_nearest(query,1,neighbor);

      // ģ�͵�
      double dx = M_tree->the_data[neighbor[0].idx][0];
      double dy = M_tree->the_data[neighbor[0].idx][1];
      double dz = M_tree->the_data[neighbor[0].idx][2];

      // ģ�͵㷨����
      double nx = M_normal[neighbor[0].idx*3+0];
      double ny = M_normal[neighbor[0].idx*3+1];
      double nz = M_normal[neighbor[0].idx*3+2];

      // �����ǰ�����ڵ��򱣴�
      if ((sx-dx)*nx+(sy-dy)*ny+(sz-dz)*nz<indist)
        inliers.push_back(i);
    }
  }
  
  // �����ڵ�
  return inliers;
}

void IcpPointToPlane::computeNormal (const kdtree::KDTreeResultVector &neighbors,double *M_normal,const double flatness) {

  if (dim==2) {

  } else {
    
    // ��ȡ�����ڵ�
    Matrix P(neighbors.size(),3);
    Matrix mu(1,3);
    for (int i=0; i<neighbors.size(); i++) {
      double x = M_tree->the_data[neighbors[i].idx][0];
      double y = M_tree->the_data[neighbors[i].idx][1];
      double z = M_tree->the_data[neighbors[i].idx][2];
      P.val[i][0] = x;
      P.val[i][1] = y;
      P.val[i][2] = z;
      mu.val[0][0] += x;
      mu.val[0][1] += y;
      mu.val[0][2] += z;
    }

    // ��ƽ��ֵ
    mu       = mu/(double)neighbors.size();
    Matrix Q = P - Matrix::ones(neighbors.size(),1)*mu;

    // ����ֵ�ֽ�
    Matrix H = ~Q*Q;
    Matrix U,W,V;
    H.svd(U,W,V);

    // ��ȡ����
    M_normal[0] = U.val[0][2];
    M_normal[1] = U.val[1][2];
    M_normal[2] = U.val[2][2];
  }
}

double* IcpPointToPlane::computeNormals (const int num_neighbors,const double flatness) 
{
	DWORD n_begin = ::GetTickCount();// computeNormals��ʱ


  double *M_normal = (double*)malloc(M_tree->N*dim*sizeof(double));
  kdtree::KDTreeResultVector neighbors;
  for (int i=0; i<M_tree->N; i++) {
    M_tree->n_nearest_around_point(i,0,num_neighbors,neighbors);
    if (dim==2) computeNormal(neighbors,M_normal+i*2,flatness);
    else        computeNormal(neighbors,M_normal+i*3,flatness);
  }


  DWORD n_title = ::GetTickCount() - n_begin;// computeNormals��ʱ
  cout << endl << "computeNormals��ʱ�� " << n_title / 1000 << " s " << n_title % 1000 << " ms " << endl;
  return M_normal;
}
