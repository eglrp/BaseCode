/********************************************************************************
** Form generated from reading UI file 'qt1.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT1_H
#define UI_QT1_H

#include <MoudelWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qt1Class
{
public:
    QAction *action_2;
    QAction *action_3;
    QAction *action_4;
    QAction *actionOpenT;
    QAction *action_6;
    QAction *action_7;
    QAction *actionRunICP;
    QAction *actionDenoising;
    QAction *actionIsWireframe;
    QAction *actionLockView;
    QAction *actionPointCloudFitting;
    QAction *actionClearSelected;
    QAction *actionPlane;
    QAction *actionLine;
    QAction *actionCircular;
    QAction *actionOpenModel;
    QAction *actionClosePoints;
    QAction *actionCloseModel;
    QAction *actionEllipse;
    QAction *actionSphere;
    QAction *actionCylinder;
    QAction *actionCone;
    QAction *actionCreateCylinderPoints;
    QAction *actionCloseAll;
    QAction *actionExit;
    QAction *actionCreateEllipsePoints;
    QAction *actionCreateConePoints;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    MoudelWidget *widget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menuFunction;
    QMenu *menu_6;
    QMenu *menu_5;
    QMenu *menuZ;
    QToolBar *mainToolView;
    QStatusBar *statusBar;
    QToolBar *toolBarFitting;
    QToolBar *toolBarFile;
    QToolBar *toolBarICP;

    void setupUi(QMainWindow *qt1Class)
    {
        if (qt1Class->objectName().isEmpty())
            qt1Class->setObjectName(QStringLiteral("qt1Class"));
        qt1Class->resize(1004, 636);
        action_2 = new QAction(qt1Class);
        action_2->setObjectName(QStringLiteral("action_2"));
        action_2->setCheckable(true);
        action_2->setChecked(true);
        action_3 = new QAction(qt1Class);
        action_3->setObjectName(QStringLiteral("action_3"));
        action_3->setCheckable(true);
        action_3->setChecked(false);
        action_4 = new QAction(qt1Class);
        action_4->setObjectName(QStringLiteral("action_4"));
        action_4->setCheckable(true);
        actionOpenT = new QAction(qt1Class);
        actionOpenT->setObjectName(QStringLiteral("actionOpenT"));
        actionOpenT->setCheckable(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/MainFrame/resources/points.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenT->setIcon(icon);
        action_6 = new QAction(qt1Class);
        action_6->setObjectName(QStringLiteral("action_6"));
        action_6->setCheckable(true);
        action_6->setChecked(true);
        action_7 = new QAction(qt1Class);
        action_7->setObjectName(QStringLiteral("action_7"));
        action_7->setCheckable(true);
        actionRunICP = new QAction(qt1Class);
        actionRunICP->setObjectName(QStringLiteral("actionRunICP"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/MainFrame/resources/icp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRunICP->setIcon(icon1);
        actionDenoising = new QAction(qt1Class);
        actionDenoising->setObjectName(QStringLiteral("actionDenoising"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/MainFrame/resources/denoising.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDenoising->setIcon(icon2);
        actionIsWireframe = new QAction(qt1Class);
        actionIsWireframe->setObjectName(QStringLiteral("actionIsWireframe"));
        actionIsWireframe->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/MainFrame/resources/grid.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionIsWireframe->setIcon(icon3);
        actionLockView = new QAction(qt1Class);
        actionLockView->setObjectName(QStringLiteral("actionLockView"));
        actionLockView->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/MainFrame/resources/unlock_view.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLockView->setIcon(icon4);
        actionPointCloudFitting = new QAction(qt1Class);
        actionPointCloudFitting->setObjectName(QStringLiteral("actionPointCloudFitting"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/MainFrame/resources/fitting.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPointCloudFitting->setIcon(icon5);
        actionClearSelected = new QAction(qt1Class);
        actionClearSelected->setObjectName(QStringLiteral("actionClearSelected"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/MainFrame/resources/clear_color.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearSelected->setIcon(icon6);
        actionPlane = new QAction(qt1Class);
        actionPlane->setObjectName(QStringLiteral("actionPlane"));
        actionPlane->setCheckable(true);
        actionPlane->setChecked(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/MainFrame/resources/plane.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlane->setIcon(icon7);
        actionLine = new QAction(qt1Class);
        actionLine->setObjectName(QStringLiteral("actionLine"));
        actionLine->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/MainFrame/resources/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLine->setIcon(icon8);
        actionCircular = new QAction(qt1Class);
        actionCircular->setObjectName(QStringLiteral("actionCircular"));
        actionCircular->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/MainFrame/resources/circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCircular->setIcon(icon9);
        actionOpenModel = new QAction(qt1Class);
        actionOpenModel->setObjectName(QStringLiteral("actionOpenModel"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/MainFrame/resources/modell.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenModel->setIcon(icon10);
        actionClosePoints = new QAction(qt1Class);
        actionClosePoints->setObjectName(QStringLiteral("actionClosePoints"));
        actionCloseModel = new QAction(qt1Class);
        actionCloseModel->setObjectName(QStringLiteral("actionCloseModel"));
        actionEllipse = new QAction(qt1Class);
        actionEllipse->setObjectName(QStringLiteral("actionEllipse"));
        actionEllipse->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/MainFrame/resources/ellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEllipse->setIcon(icon11);
        actionSphere = new QAction(qt1Class);
        actionSphere->setObjectName(QStringLiteral("actionSphere"));
        actionSphere->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/MainFrame/resources/sphere.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSphere->setIcon(icon12);
        actionCylinder = new QAction(qt1Class);
        actionCylinder->setObjectName(QStringLiteral("actionCylinder"));
        actionCylinder->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/MainFrame/resources/cylinder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCylinder->setIcon(icon13);
        actionCone = new QAction(qt1Class);
        actionCone->setObjectName(QStringLiteral("actionCone"));
        actionCone->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/MainFrame/resources/cone.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCone->setIcon(icon14);
        actionCreateCylinderPoints = new QAction(qt1Class);
        actionCreateCylinderPoints->setObjectName(QStringLiteral("actionCreateCylinderPoints"));
        actionCloseAll = new QAction(qt1Class);
        actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
        actionExit = new QAction(qt1Class);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionCreateEllipsePoints = new QAction(qt1Class);
        actionCreateEllipsePoints->setObjectName(QStringLiteral("actionCreateEllipsePoints"));
        actionCreateConePoints = new QAction(qt1Class);
        actionCreateConePoints->setObjectName(QStringLiteral("actionCreateConePoints"));
        centralWidget = new QWidget(qt1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new MoudelWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMouseTracking(true);
        widget->setFocusPolicy(Qt::WheelFocus);

        gridLayout->addWidget(widget, 0, 1, 1, 1);

        qt1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qt1Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1004, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menu_2);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_3->setEnabled(true);
        menu_4 = new QMenu(menu_2);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menuFunction = new QMenu(menuBar);
        menuFunction->setObjectName(QStringLiteral("menuFunction"));
        menu_6 = new QMenu(menuFunction);
        menu_6->setObjectName(QStringLiteral("menu_6"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        menuZ = new QMenu(menuBar);
        menuZ->setObjectName(QStringLiteral("menuZ"));
        qt1Class->setMenuBar(menuBar);
        mainToolView = new QToolBar(qt1Class);
        mainToolView->setObjectName(QStringLiteral("mainToolView"));
        qt1Class->addToolBar(Qt::TopToolBarArea, mainToolView);
        statusBar = new QStatusBar(qt1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        qt1Class->setStatusBar(statusBar);
        toolBarFitting = new QToolBar(qt1Class);
        toolBarFitting->setObjectName(QStringLiteral("toolBarFitting"));
        qt1Class->addToolBar(Qt::TopToolBarArea, toolBarFitting);
        toolBarFile = new QToolBar(qt1Class);
        toolBarFile->setObjectName(QStringLiteral("toolBarFile"));
        qt1Class->addToolBar(Qt::TopToolBarArea, toolBarFile);
        toolBarICP = new QToolBar(qt1Class);
        toolBarICP->setObjectName(QStringLiteral("toolBarICP"));
        qt1Class->addToolBar(Qt::TopToolBarArea, toolBarICP);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menuFunction->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menuBar->addAction(menuZ->menuAction());
        menu->addAction(actionOpenT);
        menu->addAction(actionOpenModel);
        menu->addAction(actionClosePoints);
        menu->addAction(actionCloseModel);
        menu->addAction(actionCloseAll);
        menu->addAction(actionExit);
        menu_2->addAction(menu_3->menuAction());
        menu_2->addAction(menu_4->menuAction());
        menu_2->addAction(actionIsWireframe);
        menu_3->addAction(action_3);
        menu_3->addAction(action_2);
        menu_3->addAction(action_4);
        menu_4->addAction(action_6);
        menu_4->addAction(action_7);
        menuFunction->addAction(actionRunICP);
        menuFunction->addAction(actionDenoising);
        menuFunction->addSeparator();
        menuFunction->addAction(actionPointCloudFitting);
        menuFunction->addAction(menu_6->menuAction());
        menu_6->addAction(actionPlane);
        menu_6->addAction(actionLine);
        menu_6->addAction(actionCircular);
        menu_6->addAction(actionEllipse);
        menu_6->addAction(actionSphere);
        menu_6->addAction(actionCylinder);
        menu_6->addAction(actionCone);
        menu_5->addAction(actionLockView);
        menu_5->addAction(actionClearSelected);
        menuZ->addAction(actionCreateCylinderPoints);
        menuZ->addAction(actionCreateEllipsePoints);
        menuZ->addAction(actionCreateConePoints);
        mainToolView->addAction(actionLockView);
        mainToolView->addSeparator();
        mainToolView->addAction(actionClearSelected);
        toolBarFitting->addAction(actionPointCloudFitting);
        toolBarFitting->addSeparator();
        toolBarFitting->addAction(actionLine);
        toolBarFitting->addAction(actionPlane);
        toolBarFitting->addAction(actionCircular);
        toolBarFitting->addAction(actionEllipse);
        toolBarFitting->addAction(actionSphere);
        toolBarFitting->addAction(actionCylinder);
        toolBarFitting->addAction(actionCone);
        toolBarFile->addAction(actionOpenT);
        toolBarFile->addAction(actionOpenModel);
        toolBarICP->addAction(actionRunICP);
        toolBarICP->addAction(actionDenoising);

        retranslateUi(qt1Class);
        QObject::connect(actionRunICP, SIGNAL(triggered()), qt1Class, SLOT(onRunICP()));
        QObject::connect(actionDenoising, SIGNAL(triggered()), qt1Class, SLOT(onDenoising()));
        QObject::connect(actionIsWireframe, SIGNAL(triggered()), qt1Class, SLOT(onIsWireframe()));
        QObject::connect(actionLockView, SIGNAL(triggered()), qt1Class, SLOT(onLockView()));
        QObject::connect(actionPointCloudFitting, SIGNAL(triggered()), qt1Class, SLOT(onPointCloudFitting()));
        QObject::connect(actionClearSelected, SIGNAL(triggered()), qt1Class, SLOT(onClearSelected()));
        QObject::connect(actionPlane, SIGNAL(triggered()), qt1Class, SLOT(onShapePlane()));
        QObject::connect(actionLine, SIGNAL(triggered()), qt1Class, SLOT(onShapeLine()));
        QObject::connect(actionOpenModel, SIGNAL(triggered()), qt1Class, SLOT(onOpenFileM()));
        QObject::connect(actionOpenT, SIGNAL(triggered()), qt1Class, SLOT(onOpenFileT()));
        QObject::connect(actionCloseModel, SIGNAL(triggered()), qt1Class, SLOT(onCloseModel()));
        QObject::connect(actionClosePoints, SIGNAL(triggered()), qt1Class, SLOT(onClosePoints()));
        QObject::connect(actionCircular, SIGNAL(triggered()), qt1Class, SLOT(onShapeCircular()));
        QObject::connect(actionSphere, SIGNAL(triggered()), qt1Class, SLOT(onShapeSphere()));
        QObject::connect(actionCone, SIGNAL(triggered()), qt1Class, SLOT(onShapeCone()));
        QObject::connect(actionEllipse, SIGNAL(triggered()), qt1Class, SLOT(onShapeEllipse()));
        QObject::connect(actionCylinder, SIGNAL(triggered()), qt1Class, SLOT(onShapeCylinder()));
        QObject::connect(actionCreateCylinderPoints, SIGNAL(triggered()), qt1Class, SLOT(onCreateCylinderPoints()));
        QObject::connect(actionCloseAll, SIGNAL(triggered()), qt1Class, SLOT(onCloseAll()));
        QObject::connect(actionExit, SIGNAL(triggered()), qt1Class, SLOT(onExit()));
        QObject::connect(actionCreateEllipsePoints, SIGNAL(triggered()), qt1Class, SLOT(onCreateEllipsePoints()));
        QObject::connect(actionCreateConePoints, SIGNAL(triggered()), qt1Class, SLOT(onCreateConePoints()));

        QMetaObject::connectSlotsByName(qt1Class);
    } // setupUi

    void retranslateUi(QMainWindow *qt1Class)
    {
        qt1Class->setWindowTitle(QApplication::translate("qt1Class", "qt1", 0));
        action_2->setText(QApplication::translate("qt1Class", "\347\202\271\344\272\221", 0));
        action_3->setText(QApplication::translate("qt1Class", "\345\256\236\344\275\223", 0));
        action_4->setText(QApplication::translate("qt1Class", "\351\232\217\346\234\272\n"
"", 0));
        actionOpenT->setText(QApplication::translate("qt1Class", "\346\211\223\345\274\200\347\202\271\344\272\221", 0));
        actionOpenT->setShortcut(QApplication::translate("qt1Class", "Ctrl+O", 0));
        action_6->setText(QApplication::translate("qt1Class", "\345\274\200\345\220\257\n"
"", 0));
        action_7->setText(QApplication::translate("qt1Class", "\345\205\263\351\227\255\n"
"", 0));
        actionRunICP->setText(QApplication::translate("qt1Class", "\351\205\215\345\207\206", 0));
        actionDenoising->setText(QApplication::translate("qt1Class", "\345\216\273\345\231\252", 0));
        actionIsWireframe->setText(QApplication::translate("qt1Class", "\346\230\276\347\244\272\347\275\221\346\240\274", 0));
        actionLockView->setText(QApplication::translate("qt1Class", "\351\224\201\345\256\232\350\247\206\345\233\276", 0));
        actionLockView->setIconText(QApplication::translate("qt1Class", "\351\224\201\345\256\232\350\247\206\345\233\276", 0));
        actionPointCloudFitting->setText(QApplication::translate("qt1Class", "\347\202\271\344\272\221\346\213\237\345\220\210", 0));
        actionClearSelected->setText(QApplication::translate("qt1Class", "\346\270\205\351\231\244\351\200\211\345\217\226", 0));
        actionPlane->setText(QApplication::translate("qt1Class", "\345\271\263\351\235\242", 0));
        actionLine->setText(QApplication::translate("qt1Class", "\347\233\264\347\272\277", 0));
        actionCircular->setText(QApplication::translate("qt1Class", "\345\234\206", 0));
        actionOpenModel->setText(QApplication::translate("qt1Class", "\346\211\223\345\274\200\346\250\241\345\236\213", 0));
        actionClosePoints->setText(QApplication::translate("qt1Class", "\345\205\263\351\227\255\347\202\271\344\272\221", 0));
        actionCloseModel->setText(QApplication::translate("qt1Class", "\345\205\263\351\227\255\346\250\241\345\236\213", 0));
        actionEllipse->setText(QApplication::translate("qt1Class", "\346\244\255\345\234\206", 0));
        actionSphere->setText(QApplication::translate("qt1Class", "\347\220\203\344\275\223", 0));
        actionCylinder->setText(QApplication::translate("qt1Class", "\345\234\206\346\237\261\344\275\223", 0));
        actionCone->setText(QApplication::translate("qt1Class", "\345\234\206\351\224\245", 0));
        actionCreateCylinderPoints->setText(QApplication::translate("qt1Class", "\345\234\206\346\237\261\347\202\271\344\272\221", 0));
        actionCloseAll->setText(QApplication::translate("qt1Class", "\345\205\263\351\227\255\345\205\250\351\203\250", 0));
        actionExit->setText(QApplication::translate("qt1Class", "\351\200\200\345\207\272", 0));
        actionCreateEllipsePoints->setText(QApplication::translate("qt1Class", "\346\244\255\345\234\206\347\202\271\344\272\221", 0));
        actionCreateConePoints->setText(QApplication::translate("qt1Class", "\345\234\206\351\224\245\347\202\271\344\272\221", 0));
        menu->setTitle(QApplication::translate("qt1Class", "\346\226\207\344\273\266", 0));
        menu_2->setTitle(QApplication::translate("qt1Class", "\346\250\241\345\236\213", 0));
        menu_3->setTitle(QApplication::translate("qt1Class", "\346\250\241\345\274\217", 0));
        menu_4->setTitle(QApplication::translate("qt1Class", "\345\205\211\347\205\247\346\270\262\346\237\223\n"
"", 0));
        menuFunction->setTitle(QApplication::translate("qt1Class", "\347\202\271\344\272\221", 0));
        menu_6->setTitle(QApplication::translate("qt1Class", "\346\213\237\345\220\210\345\233\276\345\275\242", 0));
        menu_5->setTitle(QApplication::translate("qt1Class", "\350\247\206\345\233\276", 0));
        menuZ->setTitle(QApplication::translate("qt1Class", "\347\224\237\346\210\220", 0));
        toolBarFitting->setWindowTitle(QApplication::translate("qt1Class", "toolBar", 0));
        toolBarFile->setWindowTitle(QApplication::translate("qt1Class", "toolBar", 0));
        toolBarICP->setWindowTitle(QApplication::translate("qt1Class", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class qt1Class: public Ui_qt1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT1_H
