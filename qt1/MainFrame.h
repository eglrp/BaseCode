#ifndef QT1_H
#define QT1_H

#include <QtWidgets/QMainWindow>
#include "ui_qt1.h"
#include "datamodel.h"
#include "MoudelWidget.h"

class MainFrame : public QMainWindow
{
	Q_OBJECT

public:
	datamodel ply;
	MainFrame(QWidget *parent = 0);
	~MainFrame();

	private slots:
	void on_action_2_triggered();
	void on_action_3_triggered();
	void on_action_4_triggered();
	void on_action_6_triggered();
	void on_action_7_triggered();

	void onRunICP();
	void onDenoising();
	void onIsWireframe();
	void onLockView();
	void onPointCloudFitting();
	void onClearSelected();
	void onOpenFileM();
	void onOpenFileT();
	void onClosePoints();
	void onCloseModel();
	void onCloseAll();
	void onExit();

	void onShapeLine();
	void onShapePlane();
	void onShapeCircular();
	void onShapeSphere();
	void onShapeEllipse();
	void onShapeCylinder();
	void onShapeCone();

	void onCreateCylinderPoints();
	void onCreateEllipsePoints();
	void onCreateConePoints();

private:
	Ui::qt1Class ui;
};

#endif // QT1_H
