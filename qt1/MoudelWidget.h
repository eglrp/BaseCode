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

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//ת���ַ���
enum Shape{
	NONE,
	LINE,
	PLANE,
	CIRCULAR,	//Բ
	ELLIPSE,
	CYLINDER,	//Բ����
	CONE,		//Բ׶��
	SPHERE,		//����
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
	GLint iWinWidth;			//���ڳߴ�
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
	int mouseX, mouseY;			//��ǰ���λ��
	bool isMouseDown;			//����Ƿ���  
	float rotX, rotY;			//��ʼ����ת�Ƕ�
	float zoom;					//��ͼ����
	float translateX;			//λ��X
	float translateY;			//λ��Y
	float scale;				//���ű���	
	bool isOpen;
	vector<float> cr;
	vector<float> cg;
	vector<float> cb;
	vector<double>offset;
	
	double variance;

	vector<vertex> vecVertexColor8;//�������ɫ����8��
	vector<Point3D> triNormals;//ģ����ķ�����
	vector<vertex> vecVertexRand;//��������Ķ������ɫ
	vector<vertex> vecVertexPerturbation;//�Ŷ���Ķ������ɫ
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

	bool isWireframe;//�Ƿ���ʾ������
	bool isICP;		//�Ƿ�ʹ��T��M������׼����
	bool isLockView;
	BaseModel M_model;			//���ڻ�ȡply�ļ�����
	BaseModel T_model;			//��������

	//M,T���ڴ洢���ƶ��㣬��׼��T��M
	double *icpM;//��ɫ����
	double *icpT;
	double *T_color;
	int num_m = 0;
	int num_t = 0;

	double obb[8][3];//��Χ��

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
