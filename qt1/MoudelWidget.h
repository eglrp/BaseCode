#ifndef QT4_H
#define QT4_H

#include <QtOpenGL>
#include "datamodel.h"

#include <vector>
#include <stack>
#include "icp/icpPointToPlane.h"
#include "icp/icpPointToPoint.h"
#include "obb/OBB.h"
#include "tool/BaseModel.h"
#include "tool\fitting.h"
#include "tool\Quaternion.h"
#include "tool\PointModel.h"

#define COECOUNT 16

using namespace std;

typedef struct{
	float X;
	float Y;
	float Z;

} Normal;

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//转换字符串
enum Shape{
	NONE,
	LINE,
	PLANE,
	CIRCULAR,	//圆
	ELLIPSE,
	CYLINDER,	//圆柱体
	CONE,		//圆锥体
	SPHERE,		//球体
};

typedef enum {
	MODE_FILL,
	MODE_RAND,
	MODE_POINTS,
	MODE_WIREFRAME,
} ViewMode;
typedef struct{
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	float centreX;
	float centreY;
	float centreZ;
} BoundBox;

class MoudelWidget : public QOpenGLWidget
{
public:
	MoudelWidget(QWidget* parent = 0);
	double m_rotateRectangle;
	double m_rotateTriangle;
	//datamodel ply;
	GLint iWinWidth;			//窗口尺寸
	GLint iWinHeight;
	ViewMode viewMode;
	BoundBox boundBox;
	//void changeply(QString p);
	void setFill();
	void setPoint();
	//void setRand();
	void setOpen();
	void setClose();
	void initWidget();
protected:
	void initializeGL();
	void paintGL();
	void paintEvent(QPaintEvent *);
	void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *event);
private:
	int mouseX, mouseY;			//当前鼠标位置
	bool isMouseDown;			//鼠标是否按下  
	float rotX, rotY;			//初始化旋转角度
	float zoom;					//视图焦距
	float translateX;			//位移X
	float translateY;			//位移Y
	float scale;				//缩放比例	
	bool isOpen;
	vector<float> cr;
	vector<float> cg;
	vector<float> cb;
	vector<double>offset;
	
	double variance;

	vector<vertex> vecVertexColor8;//顶点的颜色，分8种
	vector<Point3D> triNormals;//模型面的法向量
	vector<vertex> vecVertexRand;//随机分配后的顶点和颜色
	vector<vertex> vecVertexPerturbation;//扰动后的顶点和颜色
	void PickMouse(int x, int y, GLdouble rw = 20, GLdouble rh = 20);
	void ProcessPicks(GLint tPicks, GLuint *pickBuffer);
	void DrawModelImmediate(GLenum mode = GL_RENDER);
	void DrawModelPoints(GLenum mode = GL_RENDER);
	//void DrawModel(GLenum mode = GL_RENDER);
	void DrawModelRand(GLenum mode = GL_RENDER);
	void DrawModelPerturbation(GLenum mode = GL_RENDER);
	void ComputeNormal(double v1[3], double v2[3], double v3[3], Point3D &normal);
	//void SetBoundingBox();
	void SetVertexRandom();
	void SetVertexPerturbation();
	void DrawBoundingBox();
	void DrawGuideLine();

	//void SetArea8();
	void clear();

	//*s***************************QQQ
	void DrawModelFill(GLenum mode);
public:
	double mean;
	void GetNormal(BaseModel model);
	void runICP();
	void transformPoints(double angle, int direct, double* P,
		int num_p, double tx = 0, double ty = 0, double tz = 0);
	int denoising(double* points, int &p_num, const double indist);
	//double minBoundingBox(double *points, int num);
	void readPointsICP();

	void drawPointsICP(BaseModel &M, BaseModel &T, GLenum mode = GL_RENDER);

	bool isWireframe;//是否显示网格线
	bool isICP;		//是否使用T和M绘制配准点云
	bool isLockView;
	BaseModel M_model;			//用于获取ply文件数据
	BaseModel T_model;			//点云数据

	//M,T用于存储点云顶点，配准由T至M
	double *icpM;//蓝色点云
	double *icpT;
	double *T_color;
	int num_m = 0;
	int num_t = 0;

	double obb[8][3];//包围盒

	vector<int> selectedPoints;
	double fittingCoe[COECOUNT];

	Shape fittingShape;
	void clearSelected();
	bool fitting(double *coe);
	bool pointCloudFitting(vector<Point3D> &vertexs, vector<int> &idxFit, double *coe, Shape shape = Shape::PLANE);

	void createCylinderPoints();
	void createEllipsePoints();
	void createConePoints();
	//*e***************************QQQ
};

#endif // QT4_H
