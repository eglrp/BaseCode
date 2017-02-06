#include "MainFrame.h"
#include <QtGui>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextCursor>
#include <QDateTime>
#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
MainFrame::MainFrame(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);


	//char pointsPathM[32] = "points\\model.ply";
	char pointsPathT[32] = "points\\global_1w.txt";		//无噪声10,000点
	//char pointsPathT[32] = "points\\global_2wn.txt";	//有噪声20,000点
	//char pointsPathT[32] = "points\\global_5w.txt";
	//char pointsPathT[32] = "points\\global_10w.txt"; 
	//char pointsPathT[32] = "points\\Cloud201606200538.txt";

	char mm[32] = "D:\\model.ply";

	//ui.widget->T_model.readTxtPointCloudFile(pointsPathT);

	//ui.widget->M_model.readPlyFile(mm);
	//ui.widget->GetNormal(ui.widget->M_model);
	//ui.widget->viewMode = ViewMode::MODE_FILL;
	//ui.widget->readPointsICP();
}

MainFrame::~MainFrame()
{
}

//点云
void MainFrame::on_action_2_triggered()
{
	(ui.widget)->setPoint();
	ui.action_3->setChecked(false);
	ui.action_4->setChecked(false);
}
void MainFrame::on_action_3_triggered(){
	(ui.widget)->setFill();
	ui.action_2->setChecked(false);
	ui.action_4->setChecked(false);
	ui.actionIsWireframe->setChecked(false);
}
void MainFrame::on_action_4_triggered(){
	//(ui.widget)->setRand();
	//ui.action_3->setChecked(false);
	//ui.action_2->setChecked(false);
}
void MainFrame::on_action_6_triggered(){
	(ui.widget)->setOpen();
}
void MainFrame::on_action_7_triggered(){
	(ui.widget)->setClose();
}

//********QQQ
void MainFrame::onIsWireframe()
{
	ui.widget->isWireframe = !ui.widget->isWireframe;
}
//执行ICP算法进行点云配准
void MainFrame::onRunICP()
{
	ui.widget->runICP();
}
//点云去噪
void MainFrame::onDenoising()
{
	int num = ui.widget->denoising(ui.widget->icpT, ui.widget->num_t, 0.0005);//对场景点云T去噪，阈值0.0005
	ui.widget->T_model.clear();
	for (int i = 0; i < ui.widget->num_t; i++)
	{
		ui.widget->T_model.vertexs.push_back(Point3D(ui.widget->icpT[i * 3 + 0], ui.widget->icpT[i * 3 + 1], ui.widget->icpT[i * 3 + 2]));
	}

	QMessageBox::about(this, T_CHAR("去噪"), T_CHAR("点云去噪完成。消除 %1 个点").arg(num));
}
//锁定视图
void MainFrame::onLockView()
{
	ui.widget->isLockView = ui.actionLockView->isChecked();//锁定视图
	if (ui.actionLockView->isChecked() == true)
	{
		ui.actionLockView->setIcon(QIcon("resources\\lock_view.png"));
	}
	else
	{
		ui.actionLockView->setIcon(QIcon("resources\\unlock_view.png"));
	}
}
//点云拟合
void MainFrame::onPointCloudFitting()
{
	double coe[COECOUNT] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	if (ui.widget->fitting(coe) == true)
	{
		QString fun;

		switch (ui.widget->fittingShape)
		{
		case NONE:
			break;
		case LINE:
			fun = QString(T_CHAR("拟合直线方程：(x - %1) / %4 = (y - %2) / %5 = (z - %3) / %6")).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]).arg(coe[4]).arg(coe[5]);
			break;
		case PLANE:
			fun = QString(T_CHAR("拟合平面方程：%1x + %2y + %3z + %4 = 0")).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]);
			break;
		case CIRCULAR:
			fun = QString(T_CHAR("拟合圆方程：(x - %1)^2 + (y - %2)^2 + (z - %3)^2 = %4^2   |   %5x + %6y + %7z + %8 = 0")).arg(coe[0]).arg(coe[1]).arg(coe[2])
				.arg(coe[3]).arg(coe[4]).arg(coe[5]).arg(coe[6]).arg(coe[7]);
			break;
		case ELLIPSE:
			fun = QString(T_CHAR("拟合椭圆方程：焦点坐标:(%1, %2, %3),(%4, %5, %6)，椭圆长半轴：%7, 短半轴：%8 | 椭圆所在平面：%9x + %10y + %11z + %12 = 0"))
				.arg(coe[10]).arg(coe[11]).arg(coe[12]).arg(coe[13]).arg(coe[14]).arg(coe[15]).arg(coe[6]).arg(coe[7]).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]);
				
			break;
		case CYLINDER:
			fun = QString(T_CHAR("拟合圆柱方程：中轴线：(x - %1) / %4 = (y - %2) / %5 = (z - %3) / %6   半径：%7 ")).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]).arg(coe[4]).arg(coe[5]).arg(coe[6]);
			break;
		case CONE:
			fun = QString(T_CHAR("拟合圆锥方程：顶点：(%1, %2, %3)   圆锥内一点：(%4, %5, %6)   母线与中心轴夹角：%7")).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]).arg(coe[4]).arg(coe[5]).arg(coe[6]);
			break;
		case SPHERE:
			fun = QString(T_CHAR("拟合球面方程：(x - %1)^2 + (y - %2)^2 + (z - %3)^2 = %4^2")).arg(coe[0]).arg(coe[1]).arg(coe[2]).arg(coe[3]);
			break;
		default:
			break;
		}

		ui.statusBar->showMessage(fun);

		for (int i = 0; i < COECOUNT; i++)
		{
			ui.widget->fittingCoe[i] = coe[i];
		}

		ui.widget->update();
	}
	else
	{
		QString fun;
		fun = QString(T_CHAR("不存在方程或选择的点集过多！"));
		ui.statusBar->showMessage(fun);
		for (int i = 0; i < COECOUNT; i++)
		{
			ui.widget->fittingCoe[i] = 0.0;
		}
	}

}
//清空选择
void MainFrame::onClearSelected()
{
	ui.widget->clearSelected();

	for (int i = 0; i < COECOUNT; i++)
	{
		ui.widget->fittingCoe[i] = 0.0;
	}
}
void MainFrame::onShapeLine()
{
	if (ui.actionLine->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::LINE;
		ui.actionPlane->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCylinder->setChecked(false);
		ui.actionCone->setChecked(false);
	}
}
void MainFrame::onShapePlane()
{
	if (ui.actionPlane->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::PLANE;
		ui.actionLine->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCylinder->setChecked(false);
		ui.actionCone->setChecked(false);
	}
}
void MainFrame::onShapeCircular()
{
	if (ui.actionCircular->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::CIRCULAR;
		ui.actionLine->setChecked(false);
		ui.actionPlane->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCylinder->setChecked(false);
		ui.actionCone->setChecked(false);
	}
}

void MainFrame::onShapeSphere()
{
	if (ui.actionSphere->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::SPHERE;
		ui.actionLine->setChecked(false);
		ui.actionPlane->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCylinder->setChecked(false);
		ui.actionCone->setChecked(false);
	}
}

void MainFrame::onShapeEllipse()//
{
	if (ui.actionEllipse->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::ELLIPSE;
		ui.actionLine->setChecked(false);
		ui.actionPlane->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionCone->setChecked(false);
		ui.actionCylinder->setChecked(false);
	}
}

void MainFrame::onShapeCylinder()//
{
	if (ui.actionCylinder->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::CYLINDER;
		ui.actionLine->setChecked(false);
		ui.actionPlane->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCone->setChecked(false);
	}
}

void MainFrame::onShapeCone()//
{
	if (ui.actionCone->isChecked() == true)
	{
		ui.widget->fittingShape = Shape::CONE;
		ui.actionLine->setChecked(false);
		ui.actionPlane->setChecked(false);
		ui.actionCircular->setChecked(false);
		ui.actionSphere->setChecked(false);
		ui.actionEllipse->setChecked(false);
		ui.actionCylinder->setChecked(false);
	}
}

//打开场景点云文件
void MainFrame::onOpenFileT()
{
	QString filenameT = QFileDialog::getOpenFileName(this, T_CHAR("选择点云文件"), "\\",
		T_CHAR("点云(*.txt)"));
	if (filenameT.length() == 0)
	{
		return;
	}
	else
	{
		if (NULL != ui.widget->icpT)
			delete[] ui.widget->icpT;
		ui.widget->T_model.clear();

		ui.widget->T_model.readTxtPointCloudFile(filenameT.toStdString().c_str());
		//ui.widget->GetNormal(ui.widget->M_model);
		ui.widget->readPointsICP();
	}
}

//打开模型点云文件
void MainFrame::onOpenFileM()
{
	QString filenameM = QFileDialog::getOpenFileName(this, T_CHAR("选择模型文件"), "\\",
		T_CHAR("PLY(*.ply)"));
	if (filenameM.length() == 0)
	{
		return;
	}
	else
	{
		if (NULL != ui.widget->icpM)
			delete[] ui.widget->icpM;
		ui.widget->M_model.clear();

		ui.widget->M_model.readPlyFile(filenameM.toStdString().c_str());
		ui.widget->GetNormal(ui.widget->M_model);
		ui.widget->readPointsICP();
	}
}

void MainFrame::onClosePoints()
{
	if (NULL != ui.widget->icpT)
		delete[] ui.widget->icpT;
	ui.widget->T_model.clear();
	ui.widget->update();

	ui.widget->icpT = NULL;
	ui.widget->mean = 0.0;
}
void MainFrame::onCloseModel()
{
	if (NULL != ui.widget->icpM)
		delete[] ui.widget->icpM;
	ui.widget->M_model.clear();
	ui.widget->update();

	ui.widget->icpM = NULL;
}

void MainFrame::onCloseAll()
{
	if (NULL != ui.widget->icpT)
		delete[] ui.widget->icpT;
	ui.widget->T_model.clear();
	ui.widget->update();

	if (NULL != ui.widget->icpM)
		delete[] ui.widget->icpM;
	ui.widget->M_model.clear();
	ui.widget->update();

	ui.widget->icpM = NULL;
	ui.widget->icpT = NULL;

	ui.widget->mean = 0.0;
}

void MainFrame::onExit()
{
	if (QMessageBox::question(this, T_CHAR("退出"), T_CHAR("是否退出程序？"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Ok)
		QApplication::quit();
}

void MainFrame::onCreateCylinderPoints()
{
	ui.widget->createCylinderPoints();
}

void MainFrame::onCreateEllipsePoints()
{
	ui.widget->createEllipsePoints();
}

void MainFrame::onCreateConePoints()
{
	ui.widget->createConePoints();
}