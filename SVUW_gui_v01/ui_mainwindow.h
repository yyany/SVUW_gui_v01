/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "AnimatedSplitter.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Hide;
    QAction *action_Show;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label_StatusOnWindow;
    AnimatedSplitter *splitter_2;
    AnimatedSplitter *splitter;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_VisualSection;
    QGraphicsView *graphicsView01;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_InfoSection;
    QTabWidget *tabWidget_ProgParam;
    QWidget *tab_StateInfo;
    QGridLayout *gridLayout_13;
    QPlainTextEdit *plainTextEdit_StateInfo;
    QWidget *tab_AlgList;
    QGridLayout *gridLayout_14;
    QPlainTextEdit *plainTextEdit_AlgList;
    QWidget *tab_ConfigFile;
    QGridLayout *gridLayout_15;
    QPlainTextEdit *plainTextEdit_ConfigFile;
    QWidget *tab_AirCraft;
    QFormLayout *formLayout;
    QGridLayout *gridLayout_16;
    QTableWidget *tableWidget_AirCraft_xyz;
    QSpacerItem *verticalSpacer_4;
    QTableWidget *tableWidget_AirCraft_rtf;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_10;
    QSpinBox *spinBox_LogDataWriteSpeed_Aircraft;
    QLabel *label_LogDataWriteSpeed_Aircraft;
    QPushButton *pushButton_WiriteLogAirCraft3DCoordinates;
    QLabel *label_7;
    QLabel *label_12;
    QCheckBox *checkBox_WiriteLogAirCraft3DCoordinates;
    QGridLayout *gridLayout_9;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_ax;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_bx;
    QLabel *label_AirCraftParamEquation_y;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_by;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_bz;
    QLabel *label_AirCraftParamEquation_z;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_az;
    QLabel *label_AirCraftParamEquation_b;
    QLabel *label_AirCraftParamEquation_a;
    QDoubleSpinBox *doubleSpinBox_AirCraftParamEquation_ay;
    QLabel *label_AirCraftParamEquation_x;
    QLabel *label_15;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_UnderWater;
    QLabel *label_10;
    QTableWidget *tableWidget_xyzDetObj;
    QTableWidget *tableWidget_DetectObjSize;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_20;
    QLabel *label_DarkNet_Reply;
    QLabel *label_DarkNet;
    QTextEdit *textEdit_DarkNet_Reply2;
    QTableWidget *tableWidget_TwoPoint;
    QLabel *label_11;
    QTableWidget *tableWidget_xyz;
    QWidget *tab_8;
    QLabel *label_13;
    QPushButton *pushButton;
    QPlainTextEdit *plainTextEdit_Cameras;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_ControlSection;
    QGridLayout *gridLayout_7;
    QLabel *label_logo_WorkMode;
    QLabel *label_logo;
    QLabel *label_logo_CamTypy_2;
    QLabel *label_logo_CamTypy;
    QSpacerItem *horizontalSpacer_2;
    QTabWidget *tabWidget_ProgSettings;
    QWidget *tab_Main;
    QGridLayout *gridLayout_8;
    QComboBox *comboBoxMain_ModeWork;
    QGridLayout *gridLayout_OpenFiles;
    QLineEdit *lineEdit_OpenFile2R;
    QLineEdit *lineEdit_OpenFile1L;
    QPushButton *pushButton_OpenFile1L;
    QPushButton *pushButton_OpenFile2R;
    QComboBox *comboBoxMain_ImgType;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_OpenCalibr;
    QCheckBox *checkBox_setWarterCalibr;
    QPushButton *pushButton_OpenCalibr;
    QLabel *label_6;
    QComboBox *comboBoxMain_SelectCam;
    QComboBox *comboBox_CamType;
    QLabel *label_2;
    QGridLayout *gridLayout_5;
    QRadioButton *radioButton_line;
    QRadioButton *radioButton_3D;
    QGridLayout *gridLayout_3;
    QCheckBox *checkBox_03;
    QLabel *label_timeMinThreadDela;
    QCheckBox *checkBox_08;
    QCheckBox *checkBox_09;
    QCheckBox *checkBox_00;
    QCheckBox *checkBox_07;
    QSpinBox *spinBox_timeMinThreadDelay_07;
    QLabel *label_timeThreadRuntime_11;
    QLabel *label_ThreadNum;
    QCheckBox *checkBox_11;
    QLineEdit *lineEdit_Alg_inThr01;
    QLabel *label_timeThreadRuntime_01;
    QLineEdit *lineEdit_Alg_inThr10;
    QLabel *label_NoteThread05;
    QLineEdit *lineEdit_Alg_inThr04;
    QLabel *label_timeThreadRuntime_09;
    QSpinBox *spinBox_timeMinThreadDelay_05;
    QSpinBox *spinBox_timeMinThreadDelay_06;
    QCheckBox *checkBox_04;
    QCheckBox *checkBox_05;
    QLabel *label_NoteThread03;
    QLineEdit *lineEdit_Alg_inThr03;
    QLabel *label_NoteThread02;
    QLabel *label_timeThreadRuntime_10;
    QLabel *label_timeThreadRuntime_02;
    QLabel *label_timeThreadRuntime_07;
    QLineEdit *lineEdit_Alg_inThr09;
    QLabel *label_NoteThread07;
    QCheckBox *checkBox_02;
    QLabel *label_NoteThread00;
    QLabel *label_timeThreadRuntime_08;
    QSpinBox *spinBox_timeMinThreadDelay_08;
    QLineEdit *lineEdit_Alg_inThr07;
    QLabel *label_timeThreadRuntime_00;
    QSpinBox *spinBox_timeMinThreadDelay_04;
    QCheckBox *checkBox_10;
    QLabel *label_20;
    QSpinBox *spinBox_timeMinThreadDelay_02;
    QLineEdit *lineEdit_Alg_inThr00;
    QSpinBox *spinBox_timeMinThreadDelay_00;
    QLabel *label_NoteThread04;
    QSpinBox *spinBox_timeMinThreadDelay_03;
    QSpinBox *spinBox_timeMinThreadDelay_09;
    QLabel *label_timeThreadRuntime_2;
    QCheckBox *checkBox_06;
    QLabel *label_timeThreadRuntime_04;
    QLabel *label_timeThreadRuntime_05;
    QLabel *label_NoteThread10;
    QLineEdit *lineEdit_Alg_inThr05;
    QLabel *label_NoteThread11;
    QLabel *label_NoteThread01;
    QLineEdit *lineEdit_Alg_inThr06;
    QSpinBox *spinBox_timeMinThreadDelay_01;
    QLabel *label_timeThreadRuntime_03;
    QLabel *label_timeThreadRuntime_06;
    QCheckBox *checkBox_01;
    QLabel *label_22;
    QLineEdit *lineEdit_Alg_inThr08;
    QLineEdit *lineEdit_Alg_inThr02;
    QLineEdit *lineEdit_Alg_inThr11;
    QLabel *label_NoteThread09;
    QLabel *label_NoteThread08;
    QLabel *label_NoteThread06;
    QSpinBox *spinBox_timeMinThreadDelay_10;
    QSpinBox *spinBox_timeMinThreadDelay_11;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label;
    QGridLayout *gridLayout_17;
    QPushButton *pushButton_RulerStart;
    QPushButton *pushButton_Start3DRuler;
    QPushButton *pushButton_DarkNet;
    QPushButton *pushButton_OutImgWindow;
    QPushButton *pushButton_ResultVideoStream;
    QWidget *tab_Add;
    QGridLayout *gridLayout_11;
    QLabel *label_5;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_WriteResultVideo;
    QCheckBox *checkBox_WriteSourceVideo;
    QPushButton *pushButton_WriteOneStereoPair;
    QPushButton *pushButton_WriteExp;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_6;
    QGridLayout *gridLayout_4;
    QSlider *horizontalSlider_Scale;
    QLabel *label_29;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_25;
    QSlider *horizontalSlider_Ty;
    QLabel *label_30;
    QSlider *horizontalSlider_Tx;
    QSlider *horizontalSlider_Dz;
    QSlider *horizontalSlider_Rx;
    QSlider *horizontalSlider_Ry;
    QSlider *horizontalSlider_Rz;
    QLabel *label_28;
    QSlider *horizontalSlider_Tz;
    QLabel *label_24;
    QLabel *label_23;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_9;
    QGridLayout *gridLayout_19;
    QLineEdit *lineEdit_SNCam02;
    QLabel *label_SNCam02;
    QLineEdit *lineEdit_SNCam01;
    QLabel *label_SNCam01;
    QLabel *label_FrameSize;
    QLineEdit *lineEdit_FrameSize;
    QWidget *tab;
    QGridLayout *gridLayout_22;
    QGridLayout *gridLayout_21;
    QLineEdit *lineEdit_VideoStreamPort;
    QSpinBox *spinBox_VideoStreamQualiy;
    QLabel *label_VideoStreamPort;
    QLabel *label_VideoStreamQualiy;
    QPushButton *pushButton_ResultVideoStream_2;
    QLabel *label_16;
    QSpacerItem *verticalSpacer_5;
    QGridLayout *gridLayout_6;
    QPushButton *pushButton_Start;
    QPushButton *pushButton_Settings;
    QPushButton *pushButton_aboutQt;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1349, 1042);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        action_Hide = new QAction(MainWindow);
        action_Hide->setObjectName(QString::fromUtf8("action_Hide"));
        action_Show = new QAction(MainWindow);
        action_Show->setObjectName(QString::fromUtf8("action_Show"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setEnabled(true);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_StatusOnWindow = new QLabel(centralwidget);
        label_StatusOnWindow->setObjectName(QString::fromUtf8("label_StatusOnWindow"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_StatusOnWindow->sizePolicy().hasHeightForWidth());
        label_StatusOnWindow->setSizePolicy(sizePolicy);
        label_StatusOnWindow->setMaximumSize(QSize(16777215, 20));
        QFont font1;
        font1.setPointSize(12);
        label_StatusOnWindow->setFont(font1);

        gridLayout->addWidget(label_StatusOnWindow, 3, 0, 1, 1, Qt::AlignRight);

        splitter_2 = new AnimatedSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy1);
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new AnimatedSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        verticalLayoutWidget_2 = new QWidget(splitter);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayout_VisualSection = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_VisualSection->setObjectName(QString::fromUtf8("verticalLayout_VisualSection"));
        verticalLayout_VisualSection->setSizeConstraint(QLayout::SetMinimumSize);
        graphicsView01 = new QGraphicsView(verticalLayoutWidget_2);
        graphicsView01->setObjectName(QString::fromUtf8("graphicsView01"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(graphicsView01->sizePolicy().hasHeightForWidth());
        graphicsView01->setSizePolicy(sizePolicy2);
        graphicsView01->setMinimumSize(QSize(192, 120));
        graphicsView01->setBaseSize(QSize(810, 600));
        graphicsView01->setMouseTracking(true);
        graphicsView01->setSceneRect(QRectF(0, 0, 0, 0));
        graphicsView01->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_VisualSection->addWidget(graphicsView01);

        splitter->addWidget(verticalLayoutWidget_2);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        gridLayout_InfoSection = new QGridLayout(layoutWidget);
        gridLayout_InfoSection->setObjectName(QString::fromUtf8("gridLayout_InfoSection"));
        gridLayout_InfoSection->setSizeConstraint(QLayout::SetMinAndMaxSize);
        gridLayout_InfoSection->setContentsMargins(0, -1, -1, -1);
        tabWidget_ProgParam = new QTabWidget(layoutWidget);
        tabWidget_ProgParam->setObjectName(QString::fromUtf8("tabWidget_ProgParam"));
        sizePolicy2.setHeightForWidth(tabWidget_ProgParam->sizePolicy().hasHeightForWidth());
        tabWidget_ProgParam->setSizePolicy(sizePolicy2);
        tabWidget_ProgParam->setMinimumSize(QSize(500, 100));
        tab_StateInfo = new QWidget();
        tab_StateInfo->setObjectName(QString::fromUtf8("tab_StateInfo"));
        gridLayout_13 = new QGridLayout(tab_StateInfo);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        plainTextEdit_StateInfo = new QPlainTextEdit(tab_StateInfo);
        plainTextEdit_StateInfo->setObjectName(QString::fromUtf8("plainTextEdit_StateInfo"));

        gridLayout_13->addWidget(plainTextEdit_StateInfo, 0, 0, 1, 1);

        tabWidget_ProgParam->addTab(tab_StateInfo, QString());
        tab_AlgList = new QWidget();
        tab_AlgList->setObjectName(QString::fromUtf8("tab_AlgList"));
        gridLayout_14 = new QGridLayout(tab_AlgList);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        plainTextEdit_AlgList = new QPlainTextEdit(tab_AlgList);
        plainTextEdit_AlgList->setObjectName(QString::fromUtf8("plainTextEdit_AlgList"));

        gridLayout_14->addWidget(plainTextEdit_AlgList, 0, 0, 1, 1);

        tabWidget_ProgParam->addTab(tab_AlgList, QString());
        tab_ConfigFile = new QWidget();
        tab_ConfigFile->setObjectName(QString::fromUtf8("tab_ConfigFile"));
        gridLayout_15 = new QGridLayout(tab_ConfigFile);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        plainTextEdit_ConfigFile = new QPlainTextEdit(tab_ConfigFile);
        plainTextEdit_ConfigFile->setObjectName(QString::fromUtf8("plainTextEdit_ConfigFile"));

        gridLayout_15->addWidget(plainTextEdit_ConfigFile, 0, 0, 1, 1);

        tabWidget_ProgParam->addTab(tab_ConfigFile, QString());
        tab_AirCraft = new QWidget();
        tab_AirCraft->setObjectName(QString::fromUtf8("tab_AirCraft"));
        formLayout = new QFormLayout(tab_AirCraft);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        tableWidget_AirCraft_xyz = new QTableWidget(tab_AirCraft);
        if (tableWidget_AirCraft_xyz->columnCount() < 4)
            tableWidget_AirCraft_xyz->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableWidget_AirCraft_xyz->rowCount() < 4)
            tableWidget_AirCraft_xyz->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_AirCraft_xyz->setItem(3, 0, __qtablewidgetitem7);
        tableWidget_AirCraft_xyz->setObjectName(QString::fromUtf8("tableWidget_AirCraft_xyz"));
        sizePolicy2.setHeightForWidth(tableWidget_AirCraft_xyz->sizePolicy().hasHeightForWidth());
        tableWidget_AirCraft_xyz->setSizePolicy(sizePolicy2);
        tableWidget_AirCraft_xyz->setMinimumSize(QSize(395, 149));
        tableWidget_AirCraft_xyz->setMaximumSize(QSize(370, 63));
        tableWidget_AirCraft_xyz->setTabKeyNavigation(true);
        tableWidget_AirCraft_xyz->setRowCount(4);
        tableWidget_AirCraft_xyz->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_AirCraft_xyz->horizontalHeader()->setMinimumSectionSize(30);
        tableWidget_AirCraft_xyz->horizontalHeader()->setDefaultSectionSize(75);
        tableWidget_AirCraft_xyz->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_AirCraft_xyz->verticalHeader()->setDefaultSectionSize(30);

        gridLayout_16->addWidget(tableWidget_AirCraft_xyz, 0, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_16->addItem(verticalSpacer_4, 2, 0, 1, 1);

        tableWidget_AirCraft_rtf = new QTableWidget(tab_AirCraft);
        if (tableWidget_AirCraft_rtf->columnCount() < 4)
            tableWidget_AirCraft_rtf->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        if (tableWidget_AirCraft_rtf->rowCount() < 4)
            tableWidget_AirCraft_rtf->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setVerticalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setVerticalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setVerticalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_AirCraft_rtf->setItem(3, 0, __qtablewidgetitem15);
        tableWidget_AirCraft_rtf->setObjectName(QString::fromUtf8("tableWidget_AirCraft_rtf"));
        sizePolicy2.setHeightForWidth(tableWidget_AirCraft_rtf->sizePolicy().hasHeightForWidth());
        tableWidget_AirCraft_rtf->setSizePolicy(sizePolicy2);
        tableWidget_AirCraft_rtf->setMinimumSize(QSize(395, 149));
        tableWidget_AirCraft_rtf->setMaximumSize(QSize(370, 63));
        tableWidget_AirCraft_rtf->setTabKeyNavigation(true);
        tableWidget_AirCraft_rtf->setRowCount(4);
        tableWidget_AirCraft_rtf->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_AirCraft_rtf->horizontalHeader()->setMinimumSectionSize(30);
        tableWidget_AirCraft_rtf->horizontalHeader()->setDefaultSectionSize(75);
        tableWidget_AirCraft_rtf->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_AirCraft_rtf->verticalHeader()->setDefaultSectionSize(30);

        gridLayout_16->addWidget(tableWidget_AirCraft_rtf, 1, 0, 1, 1);


        formLayout->setLayout(0, QFormLayout::LabelRole, gridLayout_16);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        spinBox_LogDataWriteSpeed_Aircraft = new QSpinBox(tab_AirCraft);
        spinBox_LogDataWriteSpeed_Aircraft->setObjectName(QString::fromUtf8("spinBox_LogDataWriteSpeed_Aircraft"));
        spinBox_LogDataWriteSpeed_Aircraft->setMinimum(10);
        spinBox_LogDataWriteSpeed_Aircraft->setMaximum(5000);
        spinBox_LogDataWriteSpeed_Aircraft->setSingleStep(50);
        spinBox_LogDataWriteSpeed_Aircraft->setValue(500);

        gridLayout_10->addWidget(spinBox_LogDataWriteSpeed_Aircraft, 0, 1, 1, 1);

        label_LogDataWriteSpeed_Aircraft = new QLabel(tab_AirCraft);
        label_LogDataWriteSpeed_Aircraft->setObjectName(QString::fromUtf8("label_LogDataWriteSpeed_Aircraft"));

        gridLayout_10->addWidget(label_LogDataWriteSpeed_Aircraft, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_10, 8, 0, 1, 1);

        pushButton_WiriteLogAirCraft3DCoordinates = new QPushButton(tab_AirCraft);
        pushButton_WiriteLogAirCraft3DCoordinates->setObjectName(QString::fromUtf8("pushButton_WiriteLogAirCraft3DCoordinates"));
        pushButton_WiriteLogAirCraft3DCoordinates->setMaximumSize(QSize(397, 16777215));

        gridLayout_12->addWidget(pushButton_WiriteLogAirCraft3DCoordinates, 7, 0, 1, 1);

        label_7 = new QLabel(tab_AirCraft);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font2;
        font2.setPointSize(10);
        font2.setItalic(true);
        font2.setStrikeOut(false);
        label_7->setFont(font2);
        label_7->setWordWrap(true);

        gridLayout_12->addWidget(label_7, 3, 0, 1, 1);

        label_12 = new QLabel(tab_AirCraft);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        label_12->setFont(font3);

        gridLayout_12->addWidget(label_12, 2, 0, 1, 1);

        checkBox_WiriteLogAirCraft3DCoordinates = new QCheckBox(tab_AirCraft);
        checkBox_WiriteLogAirCraft3DCoordinates->setObjectName(QString::fromUtf8("checkBox_WiriteLogAirCraft3DCoordinates"));

        gridLayout_12->addWidget(checkBox_WiriteLogAirCraft3DCoordinates, 6, 0, 1, 1);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        doubleSpinBox_AirCraftParamEquation_ax = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_ax->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_ax"));
        doubleSpinBox_AirCraftParamEquation_ax->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_ax->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_ax->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_ax->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_ax, 4, 2, 1, 1);

        doubleSpinBox_AirCraftParamEquation_bx = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_bx->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_bx"));
        doubleSpinBox_AirCraftParamEquation_bx->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_bx->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_bx->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_bx->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_bx, 4, 3, 1, 1);

        label_AirCraftParamEquation_y = new QLabel(tab_AirCraft);
        label_AirCraftParamEquation_y->setObjectName(QString::fromUtf8("label_AirCraftParamEquation_y"));

        gridLayout_9->addWidget(label_AirCraftParamEquation_y, 5, 1, 1, 1);

        doubleSpinBox_AirCraftParamEquation_by = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_by->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_by"));
        doubleSpinBox_AirCraftParamEquation_by->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_by->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_by->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_by->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_by, 5, 3, 1, 1);

        doubleSpinBox_AirCraftParamEquation_bz = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_bz->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_bz"));
        doubleSpinBox_AirCraftParamEquation_bz->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_bz->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_bz->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_bz->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_bz, 6, 3, 1, 1);

        label_AirCraftParamEquation_z = new QLabel(tab_AirCraft);
        label_AirCraftParamEquation_z->setObjectName(QString::fromUtf8("label_AirCraftParamEquation_z"));

        gridLayout_9->addWidget(label_AirCraftParamEquation_z, 6, 1, 1, 1);

        doubleSpinBox_AirCraftParamEquation_az = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_az->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_az"));
        doubleSpinBox_AirCraftParamEquation_az->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_az->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_az->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_az->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_az, 6, 2, 1, 1);

        label_AirCraftParamEquation_b = new QLabel(tab_AirCraft);
        label_AirCraftParamEquation_b->setObjectName(QString::fromUtf8("label_AirCraftParamEquation_b"));

        gridLayout_9->addWidget(label_AirCraftParamEquation_b, 3, 3, 1, 1);

        label_AirCraftParamEquation_a = new QLabel(tab_AirCraft);
        label_AirCraftParamEquation_a->setObjectName(QString::fromUtf8("label_AirCraftParamEquation_a"));

        gridLayout_9->addWidget(label_AirCraftParamEquation_a, 3, 2, 1, 1);

        doubleSpinBox_AirCraftParamEquation_ay = new QDoubleSpinBox(tab_AirCraft);
        doubleSpinBox_AirCraftParamEquation_ay->setObjectName(QString::fromUtf8("doubleSpinBox_AirCraftParamEquation_ay"));
        doubleSpinBox_AirCraftParamEquation_ay->setDecimals(6);
        doubleSpinBox_AirCraftParamEquation_ay->setMinimum(-1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_ay->setMaximum(1000.000000000000000);
        doubleSpinBox_AirCraftParamEquation_ay->setSingleStep(0.100000000000000);

        gridLayout_9->addWidget(doubleSpinBox_AirCraftParamEquation_ay, 5, 2, 1, 1);

        label_AirCraftParamEquation_x = new QLabel(tab_AirCraft);
        label_AirCraftParamEquation_x->setObjectName(QString::fromUtf8("label_AirCraftParamEquation_x"));

        gridLayout_9->addWidget(label_AirCraftParamEquation_x, 4, 1, 1, 1);

        label_15 = new QLabel(tab_AirCraft);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_9->addWidget(label_15, 2, 2, 1, 1);

        label_8 = new QLabel(tab_AirCraft);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_9->addWidget(label_8, 3, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout_9, 4, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_12->addItem(verticalSpacer_3, 11, 0, 1, 1);


        formLayout->setLayout(0, QFormLayout::FieldRole, gridLayout_12);

        tabWidget_ProgParam->addTab(tab_AirCraft, QString());
        tab_UnderWater = new QWidget();
        tab_UnderWater->setObjectName(QString::fromUtf8("tab_UnderWater"));
        label_10 = new QLabel(tab_UnderWater);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 280, 66, 27));
        QFont font4;
        font4.setPointSize(15);
        font4.setBold(true);
        font4.setItalic(true);
        label_10->setFont(font4);
        tableWidget_xyzDetObj = new QTableWidget(tab_UnderWater);
        if (tableWidget_xyzDetObj->columnCount() < 4)
            tableWidget_xyzDetObj->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_xyzDetObj->setHorizontalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_xyzDetObj->setHorizontalHeaderItem(1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_xyzDetObj->setHorizontalHeaderItem(2, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_xyzDetObj->setHorizontalHeaderItem(3, __qtablewidgetitem19);
        if (tableWidget_xyzDetObj->rowCount() < 1)
            tableWidget_xyzDetObj->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_xyzDetObj->setVerticalHeaderItem(0, __qtablewidgetitem20);
        tableWidget_xyzDetObj->setObjectName(QString::fromUtf8("tableWidget_xyzDetObj"));
        tableWidget_xyzDetObj->setGeometry(QRect(10, 320, 281, 71));
        sizePolicy1.setHeightForWidth(tableWidget_xyzDetObj->sizePolicy().hasHeightForWidth());
        tableWidget_xyzDetObj->setSizePolicy(sizePolicy1);
        tableWidget_xyzDetObj->setMinimumSize(QSize(0, 0));
        tableWidget_xyzDetObj->setMaximumSize(QSize(16777215, 16777215));
#if QT_CONFIG(whatsthis)
        tableWidget_xyzDetObj->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
        tableWidget_xyzDetObj->horizontalHeader()->setVisible(true);
        tableWidget_xyzDetObj->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_xyzDetObj->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget_xyzDetObj->horizontalHeader()->setDefaultSectionSize(65);
        tableWidget_xyzDetObj->verticalHeader()->setVisible(true);
        tableWidget_xyzDetObj->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_xyzDetObj->verticalHeader()->setDefaultSectionSize(30);
        tableWidget_DetectObjSize = new QTableWidget(tab_UnderWater);
        if (tableWidget_DetectObjSize->columnCount() < 6)
            tableWidget_DetectObjSize->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(2, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(3, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(4, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setHorizontalHeaderItem(5, __qtablewidgetitem26);
        if (tableWidget_DetectObjSize->rowCount() < 1)
            tableWidget_DetectObjSize->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableWidget_DetectObjSize->setVerticalHeaderItem(0, __qtablewidgetitem27);
        tableWidget_DetectObjSize->setObjectName(QString::fromUtf8("tableWidget_DetectObjSize"));
        tableWidget_DetectObjSize->setGeometry(QRect(320, 320, 441, 71));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tableWidget_DetectObjSize->sizePolicy().hasHeightForWidth());
        tableWidget_DetectObjSize->setSizePolicy(sizePolicy3);
        tableWidget_DetectObjSize->horizontalHeader()->setDefaultSectionSize(67);
        gridLayoutWidget_2 = new QWidget(tab_UnderWater);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 140, 741, 141));
        gridLayout_20 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        gridLayout_20->setContentsMargins(0, 0, 0, 0);
        label_DarkNet_Reply = new QLabel(gridLayoutWidget_2);
        label_DarkNet_Reply->setObjectName(QString::fromUtf8("label_DarkNet_Reply"));

        gridLayout_20->addWidget(label_DarkNet_Reply, 0, 1, 1, 1);

        label_DarkNet = new QLabel(gridLayoutWidget_2);
        label_DarkNet->setObjectName(QString::fromUtf8("label_DarkNet"));
        QFont font5;
        font5.setPointSize(14);
        font5.setBold(true);
        label_DarkNet->setFont(font5);

        gridLayout_20->addWidget(label_DarkNet, 0, 0, 1, 1);

        textEdit_DarkNet_Reply2 = new QTextEdit(gridLayoutWidget_2);
        textEdit_DarkNet_Reply2->setObjectName(QString::fromUtf8("textEdit_DarkNet_Reply2"));
        textEdit_DarkNet_Reply2->setMinimumSize(QSize(0, 100));
        textEdit_DarkNet_Reply2->setMaximumSize(QSize(400, 130));

        gridLayout_20->addWidget(textEdit_DarkNet_Reply2, 0, 2, 1, 1);

        tableWidget_TwoPoint = new QTableWidget(tab_UnderWater);
        if (tableWidget_TwoPoint->columnCount() < 4)
            tableWidget_TwoPoint->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tableWidget_TwoPoint->setHorizontalHeaderItem(0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tableWidget_TwoPoint->setHorizontalHeaderItem(1, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tableWidget_TwoPoint->setHorizontalHeaderItem(2, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        tableWidget_TwoPoint->setHorizontalHeaderItem(3, __qtablewidgetitem31);
        if (tableWidget_TwoPoint->rowCount() < 2)
            tableWidget_TwoPoint->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        tableWidget_TwoPoint->setVerticalHeaderItem(0, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        tableWidget_TwoPoint->setVerticalHeaderItem(1, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(0, 0, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(0, 1, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(0, 2, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(0, 3, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(1, 0, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(1, 1, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(1, 2, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        tableWidget_TwoPoint->setItem(1, 3, __qtablewidgetitem41);
        tableWidget_TwoPoint->setObjectName(QString::fromUtf8("tableWidget_TwoPoint"));
        tableWidget_TwoPoint->setEnabled(true);
        tableWidget_TwoPoint->setGeometry(QRect(349, 43, 400, 90));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tableWidget_TwoPoint->sizePolicy().hasHeightForWidth());
        tableWidget_TwoPoint->setSizePolicy(sizePolicy4);
        tableWidget_TwoPoint->setMinimumSize(QSize(0, 0));
        tableWidget_TwoPoint->setMaximumSize(QSize(400, 90));
        tableWidget_TwoPoint->setAutoFillBackground(false);
        tableWidget_TwoPoint->setAutoScroll(true);
        tableWidget_TwoPoint->setDragEnabled(false);
        tableWidget_TwoPoint->setAlternatingRowColors(false);
        tableWidget_TwoPoint->setShowGrid(true);
        tableWidget_TwoPoint->setSortingEnabled(false);
        tableWidget_TwoPoint->setCornerButtonEnabled(true);
        tableWidget_TwoPoint->horizontalHeader()->setVisible(true);
        tableWidget_TwoPoint->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_TwoPoint->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget_TwoPoint->horizontalHeader()->setDefaultSectionSize(71);
        tableWidget_TwoPoint->horizontalHeader()->setHighlightSections(true);
        tableWidget_TwoPoint->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_TwoPoint->horizontalHeader()->setStretchLastSection(false);
        tableWidget_TwoPoint->verticalHeader()->setVisible(true);
        tableWidget_TwoPoint->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget_TwoPoint->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_TwoPoint->verticalHeader()->setDefaultSectionSize(30);
        tableWidget_TwoPoint->verticalHeader()->setHighlightSections(true);
        tableWidget_TwoPoint->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_TwoPoint->verticalHeader()->setStretchLastSection(false);
        label_11 = new QLabel(tab_UnderWater);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 10, 71, 27));
        label_11->setFont(font4);
        tableWidget_xyz = new QTableWidget(tab_UnderWater);
        if (tableWidget_xyz->columnCount() < 4)
            tableWidget_xyz->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        tableWidget_xyz->setHorizontalHeaderItem(0, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        tableWidget_xyz->setHorizontalHeaderItem(1, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        tableWidget_xyz->setHorizontalHeaderItem(2, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        tableWidget_xyz->setHorizontalHeaderItem(3, __qtablewidgetitem45);
        if (tableWidget_xyz->rowCount() < 1)
            tableWidget_xyz->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        tableWidget_xyz->setVerticalHeaderItem(0, __qtablewidgetitem46);
        tableWidget_xyz->setObjectName(QString::fromUtf8("tableWidget_xyz"));
        tableWidget_xyz->setGeometry(QRect(10, 43, 321, 91));
        sizePolicy1.setHeightForWidth(tableWidget_xyz->sizePolicy().hasHeightForWidth());
        tableWidget_xyz->setSizePolicy(sizePolicy1);
        tableWidget_xyz->setMinimumSize(QSize(0, 0));
        tableWidget_xyz->setMaximumSize(QSize(16777215, 16777215));
#if QT_CONFIG(whatsthis)
        tableWidget_xyz->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
        tableWidget_xyz->horizontalHeader()->setVisible(true);
        tableWidget_xyz->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_xyz->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget_xyz->horizontalHeader()->setDefaultSectionSize(65);
        tableWidget_xyz->verticalHeader()->setVisible(true);
        tableWidget_xyz->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_xyz->verticalHeader()->setDefaultSectionSize(30);
        tabWidget_ProgParam->addTab(tab_UnderWater, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        label_13 = new QLabel(tab_8);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(70, 50, 111, 61));
        pushButton = new QPushButton(tab_8);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(70, 150, 151, 81));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(255, 151, 57);\n"
"  color: white; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #FF7832, stop: 1 #FF9739);\n"
"}"));
        plainTextEdit_Cameras = new QPlainTextEdit(tab_8);
        plainTextEdit_Cameras->setObjectName(QString::fromUtf8("plainTextEdit_Cameras"));
        plainTextEdit_Cameras->setGeometry(QRect(410, 130, 311, 301));
        tabWidget_ProgParam->addTab(tab_8, QString());

        gridLayout_InfoSection->addWidget(tabWidget_ProgParam, 1, 0, 1, 1);

        splitter->addWidget(layoutWidget);
        splitter_2->addWidget(splitter);
        verticalLayoutWidget = new QWidget(splitter_2);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        sizePolicy1.setHeightForWidth(verticalLayoutWidget->sizePolicy().hasHeightForWidth());
        verticalLayoutWidget->setSizePolicy(sizePolicy1);
        verticalLayout_ControlSection = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_ControlSection->setSpacing(2);
        verticalLayout_ControlSection->setObjectName(QString::fromUtf8("verticalLayout_ControlSection"));
        verticalLayout_ControlSection->setSizeConstraint(QLayout::SetMinAndMaxSize);
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_logo_WorkMode = new QLabel(verticalLayoutWidget);
        label_logo_WorkMode->setObjectName(QString::fromUtf8("label_logo_WorkMode"));
        label_logo_WorkMode->setMinimumSize(QSize(80, 80));
        label_logo_WorkMode->setMaximumSize(QSize(100, 100));
        label_logo_WorkMode->setPixmap(QPixmap(QString::fromUtf8("files/Logo/logoSTV01_100.png")));

        gridLayout_7->addWidget(label_logo_WorkMode, 0, 2, 1, 1);

        label_logo = new QLabel(verticalLayoutWidget);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setMinimumSize(QSize(80, 80));
        label_logo->setMaximumSize(QSize(100, 100));
        label_logo->setPixmap(QPixmap(QString::fromUtf8("files/Logo/logo5_100.png")));

        gridLayout_7->addWidget(label_logo, 0, 1, 1, 1);

        label_logo_CamTypy_2 = new QLabel(verticalLayoutWidget);
        label_logo_CamTypy_2->setObjectName(QString::fromUtf8("label_logo_CamTypy_2"));
        label_logo_CamTypy_2->setMinimumSize(QSize(80, 80));
        label_logo_CamTypy_2->setMaximumSize(QSize(100, 100));
        label_logo_CamTypy_2->setPixmap(QPixmap(QString::fromUtf8(":/res_ImgLogo/files/Logo/logo_IUTS.jpg")));

        gridLayout_7->addWidget(label_logo_CamTypy_2, 0, 4, 1, 1);

        label_logo_CamTypy = new QLabel(verticalLayoutWidget);
        label_logo_CamTypy->setObjectName(QString::fromUtf8("label_logo_CamTypy"));
        label_logo_CamTypy->setMinimumSize(QSize(80, 80));
        label_logo_CamTypy->setMaximumSize(QSize(100, 100));
        label_logo_CamTypy->setPixmap(QPixmap(QString::fromUtf8(":/res_ImgLogo/files/Logo/logo_IUTS.jpg")));

        gridLayout_7->addWidget(label_logo_CamTypy, 0, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_2, 0, 5, 1, 1);


        verticalLayout_ControlSection->addLayout(gridLayout_7);

        tabWidget_ProgSettings = new QTabWidget(verticalLayoutWidget);
        tabWidget_ProgSettings->setObjectName(QString::fromUtf8("tabWidget_ProgSettings"));
        tabWidget_ProgSettings->setMinimumSize(QSize(0, 0));
        tabWidget_ProgSettings->setStyleSheet(QString::fromUtf8(""));
        tab_Main = new QWidget();
        tab_Main->setObjectName(QString::fromUtf8("tab_Main"));
        tab_Main->setStyleSheet(QString::fromUtf8(""));
        gridLayout_8 = new QGridLayout(tab_Main);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        comboBoxMain_ModeWork = new QComboBox(tab_Main);
        comboBoxMain_ModeWork->addItem(QString());
        comboBoxMain_ModeWork->addItem(QString());
        comboBoxMain_ModeWork->addItem(QString());
        comboBoxMain_ModeWork->addItem(QString());
        comboBoxMain_ModeWork->setObjectName(QString::fromUtf8("comboBoxMain_ModeWork"));
        sizePolicy1.setHeightForWidth(comboBoxMain_ModeWork->sizePolicy().hasHeightForWidth());
        comboBoxMain_ModeWork->setSizePolicy(sizePolicy1);
        comboBoxMain_ModeWork->setMaximumSize(QSize(500, 25));
        QFont font6;
        font6.setPointSize(10);
        font6.setBold(false);
        comboBoxMain_ModeWork->setFont(font6);
        comboBoxMain_ModeWork->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"  background-color: rgb(250, 200, 150);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QComboBox:clicked {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #FF7832, stop: 1 #FF9739);\n"
"}"));

        gridLayout_8->addWidget(comboBoxMain_ModeWork, 2, 0, 1, 1);

        gridLayout_OpenFiles = new QGridLayout();
        gridLayout_OpenFiles->setObjectName(QString::fromUtf8("gridLayout_OpenFiles"));
        lineEdit_OpenFile2R = new QLineEdit(tab_Main);
        lineEdit_OpenFile2R->setObjectName(QString::fromUtf8("lineEdit_OpenFile2R"));
        lineEdit_OpenFile2R->setMaximumSize(QSize(350000, 25));
        QFont font7;
        font7.setPointSize(9);
        lineEdit_OpenFile2R->setFont(font7);
        lineEdit_OpenFile2R->setStyleSheet(QString::fromUtf8("  background-color: rgb(200, 150, 150);"));

        gridLayout_OpenFiles->addWidget(lineEdit_OpenFile2R, 1, 1, 1, 1);

        lineEdit_OpenFile1L = new QLineEdit(tab_Main);
        lineEdit_OpenFile1L->setObjectName(QString::fromUtf8("lineEdit_OpenFile1L"));
        lineEdit_OpenFile1L->setMaximumSize(QSize(3500000, 25));
        lineEdit_OpenFile1L->setFont(font7);
        lineEdit_OpenFile1L->setStyleSheet(QString::fromUtf8("  background-color: rgb(200, 150, 150);"));

        gridLayout_OpenFiles->addWidget(lineEdit_OpenFile1L, 0, 1, 1, 1);

        pushButton_OpenFile1L = new QPushButton(tab_Main);
        pushButton_OpenFile1L->setObjectName(QString::fromUtf8("pushButton_OpenFile1L"));
        pushButton_OpenFile1L->setMaximumSize(QSize(100, 16777215));
        pushButton_OpenFile1L->setFont(font7);
        pushButton_OpenFile1L->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(140,	210, 248);\n"
"  color: green; \n"
"   border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}"));

        gridLayout_OpenFiles->addWidget(pushButton_OpenFile1L, 0, 2, 1, 1);

        pushButton_OpenFile2R = new QPushButton(tab_Main);
        pushButton_OpenFile2R->setObjectName(QString::fromUtf8("pushButton_OpenFile2R"));
        pushButton_OpenFile2R->setMaximumSize(QSize(100, 16777215));
        pushButton_OpenFile2R->setFont(font7);
        pushButton_OpenFile2R->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgb(140,	210, 248);\n"
"  color: green; \n"
"   border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}"));

        gridLayout_OpenFiles->addWidget(pushButton_OpenFile2R, 1, 2, 1, 1);


        gridLayout_8->addLayout(gridLayout_OpenFiles, 7, 0, 1, 1);

        comboBoxMain_ImgType = new QComboBox(tab_Main);
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->addItem(QString());
        comboBoxMain_ImgType->setObjectName(QString::fromUtf8("comboBoxMain_ImgType"));
        sizePolicy1.setHeightForWidth(comboBoxMain_ImgType->sizePolicy().hasHeightForWidth());
        comboBoxMain_ImgType->setSizePolicy(sizePolicy1);
        comboBoxMain_ImgType->setMaximumSize(QSize(500, 25));
        comboBoxMain_ImgType->setFont(font6);
        comboBoxMain_ImgType->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"  background-color: rgb(170,	  225,     255);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QComboBox:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #0DBAF7, stop: 1 #0DBAF7);\n"
"}"));

        gridLayout_8->addWidget(comboBoxMain_ImgType, 11, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit_OpenCalibr = new QLineEdit(tab_Main);
        lineEdit_OpenCalibr->setObjectName(QString::fromUtf8("lineEdit_OpenCalibr"));
        lineEdit_OpenCalibr->setMaximumSize(QSize(3500000, 25));
        lineEdit_OpenCalibr->setFont(font7);
        lineEdit_OpenCalibr->setStyleSheet(QString::fromUtf8("  background-color: rgb(150, 150, 200);"));

        horizontalLayout_2->addWidget(lineEdit_OpenCalibr);

        checkBox_setWarterCalibr = new QCheckBox(tab_Main);
        checkBox_setWarterCalibr->setObjectName(QString::fromUtf8("checkBox_setWarterCalibr"));

        horizontalLayout_2->addWidget(checkBox_setWarterCalibr);

        pushButton_OpenCalibr = new QPushButton(tab_Main);
        pushButton_OpenCalibr->setObjectName(QString::fromUtf8("pushButton_OpenCalibr"));
        pushButton_OpenCalibr->setMaximumSize(QSize(100, 16777215));
        pushButton_OpenCalibr->setFont(font7);
        pushButton_OpenCalibr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(200, 150, 150);\n"
"  color: black; \n"
"   border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
"}"));

        horizontalLayout_2->addWidget(pushButton_OpenCalibr);


        gridLayout_8->addLayout(horizontalLayout_2, 9, 0, 1, 1);

        label_6 = new QLabel(tab_Main);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMaximumSize(QSize(16777215, 20));
        QFont font8;
        font8.setPointSize(10);
        font8.setBold(true);
        label_6->setFont(font8);

        gridLayout_8->addWidget(label_6, 10, 0, 1, 1);

        comboBoxMain_SelectCam = new QComboBox(tab_Main);
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->addItem(QString());
        comboBoxMain_SelectCam->setObjectName(QString::fromUtf8("comboBoxMain_SelectCam"));
        sizePolicy1.setHeightForWidth(comboBoxMain_SelectCam->sizePolicy().hasHeightForWidth());
        comboBoxMain_SelectCam->setSizePolicy(sizePolicy1);
        comboBoxMain_SelectCam->setMaximumSize(QSize(500, 25));
        comboBoxMain_SelectCam->setFont(font6);
        comboBoxMain_SelectCam->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"  background-color: rgb(210,     250,    180);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QComboBox:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #D2FE82, stop: 1 #D2FE82);\n"
"}"));

        gridLayout_8->addWidget(comboBoxMain_SelectCam, 4, 0, 1, 1);

        comboBox_CamType = new QComboBox(tab_Main);
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->addItem(QString());
        comboBox_CamType->setObjectName(QString::fromUtf8("comboBox_CamType"));
        comboBox_CamType->setMaximumSize(QSize(16777215, 25));
        comboBox_CamType->setFont(font7);
        comboBox_CamType->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"  background-color: rgb(150,     180,   150);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QComboBox:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #B5CBB5, stop: 1 #B5CBB5);\n"
"}"));

        gridLayout_8->addWidget(comboBox_CamType, 6, 0, 1, 1);

        label_2 = new QLabel(tab_Main);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMaximumSize(QSize(16777215, 20));
        label_2->setFont(font8);

        gridLayout_8->addWidget(label_2, 13, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        radioButton_line = new QRadioButton(tab_Main);
        radioButton_line->setObjectName(QString::fromUtf8("radioButton_line"));
        sizePolicy1.setHeightForWidth(radioButton_line->sizePolicy().hasHeightForWidth());
        radioButton_line->setSizePolicy(sizePolicy1);
        radioButton_line->setMaximumSize(QSize(16777215, 15));
        radioButton_line->setFont(font);
        radioButton_line->setStyleSheet(QString::fromUtf8("\n"
"  background-color: rgb(150,     180,   150);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
""));

        gridLayout_5->addWidget(radioButton_line, 2, 0, 1, 1);

        radioButton_3D = new QRadioButton(tab_Main);
        radioButton_3D->setObjectName(QString::fromUtf8("radioButton_3D"));
        sizePolicy1.setHeightForWidth(radioButton_3D->sizePolicy().hasHeightForWidth());
        radioButton_3D->setSizePolicy(sizePolicy1);
        radioButton_3D->setMaximumSize(QSize(16777215, 15));
        radioButton_3D->setFont(font);
        radioButton_3D->setStyleSheet(QString::fromUtf8("\n"
"   background-color: rgb(250, 200, 150);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"\n"
"\n"
""));
        radioButton_3D->setChecked(true);

        gridLayout_5->addWidget(radioButton_3D, 2, 1, 1, 1);


        gridLayout_8->addLayout(gridLayout_5, 12, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        checkBox_03 = new QCheckBox(tab_Main);
        checkBox_03->setObjectName(QString::fromUtf8("checkBox_03"));
        sizePolicy1.setHeightForWidth(checkBox_03->sizePolicy().hasHeightForWidth());
        checkBox_03->setSizePolicy(sizePolicy1);
        checkBox_03->setMaximumSize(QSize(16777215, 16));
        checkBox_03->setFont(font7);
        checkBox_03->setChecked(true);

        gridLayout_3->addWidget(checkBox_03, 4, 1, 1, 1);

        label_timeMinThreadDela = new QLabel(tab_Main);
        label_timeMinThreadDela->setObjectName(QString::fromUtf8("label_timeMinThreadDela"));
        QFont font9;
        font9.setPointSize(9);
        font9.setBold(true);
        label_timeMinThreadDela->setFont(font9);

        gridLayout_3->addWidget(label_timeMinThreadDela, 0, 5, 1, 1);

        checkBox_08 = new QCheckBox(tab_Main);
        checkBox_08->setObjectName(QString::fromUtf8("checkBox_08"));
        checkBox_08->setMaximumSize(QSize(16777215, 16));
        checkBox_08->setFont(font7);

        gridLayout_3->addWidget(checkBox_08, 9, 1, 1, 1);

        checkBox_09 = new QCheckBox(tab_Main);
        checkBox_09->setObjectName(QString::fromUtf8("checkBox_09"));
        checkBox_09->setMaximumSize(QSize(16777215, 16));
        checkBox_09->setFont(font7);

        gridLayout_3->addWidget(checkBox_09, 10, 1, 1, 1);

        checkBox_00 = new QCheckBox(tab_Main);
        checkBox_00->setObjectName(QString::fromUtf8("checkBox_00"));
        checkBox_00->setMaximumSize(QSize(16777215, 16));
        checkBox_00->setFont(font7);
        checkBox_00->setChecked(true);

        gridLayout_3->addWidget(checkBox_00, 1, 1, 1, 1);

        checkBox_07 = new QCheckBox(tab_Main);
        checkBox_07->setObjectName(QString::fromUtf8("checkBox_07"));
        checkBox_07->setMaximumSize(QSize(16777215, 16));
        checkBox_07->setFont(font7);

        gridLayout_3->addWidget(checkBox_07, 8, 1, 1, 1);

        spinBox_timeMinThreadDelay_07 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_07->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_07"));
        spinBox_timeMinThreadDelay_07->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_07->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_07, 8, 5, 1, 1);

        label_timeThreadRuntime_11 = new QLabel(tab_Main);
        label_timeThreadRuntime_11->setObjectName(QString::fromUtf8("label_timeThreadRuntime_11"));
        label_timeThreadRuntime_11->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_11, 14, 4, 1, 1);

        label_ThreadNum = new QLabel(tab_Main);
        label_ThreadNum->setObjectName(QString::fromUtf8("label_ThreadNum"));
        label_ThreadNum->setFont(font9);

        gridLayout_3->addWidget(label_ThreadNum, 0, 1, 1, 1, Qt::AlignHCenter);

        checkBox_11 = new QCheckBox(tab_Main);
        checkBox_11->setObjectName(QString::fromUtf8("checkBox_11"));
        checkBox_11->setMaximumSize(QSize(16777215, 16));
        checkBox_11->setFont(font7);

        gridLayout_3->addWidget(checkBox_11, 14, 1, 1, 1);

        lineEdit_Alg_inThr01 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr01->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr01"));
        lineEdit_Alg_inThr01->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr01->setFont(font7);
        lineEdit_Alg_inThr01->setAutoFillBackground(false);
        lineEdit_Alg_inThr01->setInputMethodHints(Qt::ImhNone);
        lineEdit_Alg_inThr01->setEchoMode(QLineEdit::Normal);
        lineEdit_Alg_inThr01->setClearButtonEnabled(false);

        gridLayout_3->addWidget(lineEdit_Alg_inThr01, 2, 3, 1, 1);

        label_timeThreadRuntime_01 = new QLabel(tab_Main);
        label_timeThreadRuntime_01->setObjectName(QString::fromUtf8("label_timeThreadRuntime_01"));
        label_timeThreadRuntime_01->setMaximumSize(QSize(16777215, 20));

        gridLayout_3->addWidget(label_timeThreadRuntime_01, 2, 4, 1, 1);

        lineEdit_Alg_inThr10 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr10->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr10"));
        lineEdit_Alg_inThr10->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr10->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr10, 12, 3, 1, 1);

        label_NoteThread05 = new QLabel(tab_Main);
        label_NoteThread05->setObjectName(QString::fromUtf8("label_NoteThread05"));
        label_NoteThread05->setMinimumSize(QSize(100, 0));
        QFont font10;
        font10.setPointSize(8);
        font10.setItalic(true);
        label_NoteThread05->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread05, 6, 0, 1, 1);

        lineEdit_Alg_inThr04 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr04->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr04"));
        lineEdit_Alg_inThr04->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr04->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr04, 5, 3, 1, 1);

        label_timeThreadRuntime_09 = new QLabel(tab_Main);
        label_timeThreadRuntime_09->setObjectName(QString::fromUtf8("label_timeThreadRuntime_09"));
        label_timeThreadRuntime_09->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_09->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_09, 10, 4, 1, 1);

        spinBox_timeMinThreadDelay_05 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_05->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_05"));
        spinBox_timeMinThreadDelay_05->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_05->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_05, 6, 5, 1, 1);

        spinBox_timeMinThreadDelay_06 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_06->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_06"));
        spinBox_timeMinThreadDelay_06->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_06->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_06, 7, 5, 1, 1);

        checkBox_04 = new QCheckBox(tab_Main);
        checkBox_04->setObjectName(QString::fromUtf8("checkBox_04"));
        sizePolicy1.setHeightForWidth(checkBox_04->sizePolicy().hasHeightForWidth());
        checkBox_04->setSizePolicy(sizePolicy1);
        checkBox_04->setMaximumSize(QSize(16777215, 16));
        checkBox_04->setFont(font7);
        checkBox_04->setChecked(true);

        gridLayout_3->addWidget(checkBox_04, 5, 1, 1, 1);

        checkBox_05 = new QCheckBox(tab_Main);
        checkBox_05->setObjectName(QString::fromUtf8("checkBox_05"));
        checkBox_05->setEnabled(true);
        sizePolicy1.setHeightForWidth(checkBox_05->sizePolicy().hasHeightForWidth());
        checkBox_05->setSizePolicy(sizePolicy1);
        checkBox_05->setMaximumSize(QSize(16777215, 16));
        QFont font11;
        font11.setPointSize(9);
        font11.setBold(false);
        checkBox_05->setFont(font11);
        checkBox_05->setChecked(false);

        gridLayout_3->addWidget(checkBox_05, 6, 1, 1, 1);

        label_NoteThread03 = new QLabel(tab_Main);
        label_NoteThread03->setObjectName(QString::fromUtf8("label_NoteThread03"));
        label_NoteThread03->setMinimumSize(QSize(100, 0));
        label_NoteThread03->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread03, 4, 0, 1, 1);

        lineEdit_Alg_inThr03 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr03->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr03"));
        lineEdit_Alg_inThr03->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr03->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr03, 4, 3, 1, 1);

        label_NoteThread02 = new QLabel(tab_Main);
        label_NoteThread02->setObjectName(QString::fromUtf8("label_NoteThread02"));
        label_NoteThread02->setMinimumSize(QSize(100, 0));
        label_NoteThread02->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread02, 3, 0, 1, 1);

        label_timeThreadRuntime_10 = new QLabel(tab_Main);
        label_timeThreadRuntime_10->setObjectName(QString::fromUtf8("label_timeThreadRuntime_10"));
        label_timeThreadRuntime_10->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_10, 12, 4, 1, 1);

        label_timeThreadRuntime_02 = new QLabel(tab_Main);
        label_timeThreadRuntime_02->setObjectName(QString::fromUtf8("label_timeThreadRuntime_02"));
        label_timeThreadRuntime_02->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_02->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_02, 3, 4, 1, 1);

        label_timeThreadRuntime_07 = new QLabel(tab_Main);
        label_timeThreadRuntime_07->setObjectName(QString::fromUtf8("label_timeThreadRuntime_07"));
        label_timeThreadRuntime_07->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_07->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_07, 8, 4, 1, 1);

        lineEdit_Alg_inThr09 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr09->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr09"));
        lineEdit_Alg_inThr09->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr09->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr09, 10, 3, 1, 1);

        label_NoteThread07 = new QLabel(tab_Main);
        label_NoteThread07->setObjectName(QString::fromUtf8("label_NoteThread07"));
        label_NoteThread07->setMinimumSize(QSize(100, 0));
        label_NoteThread07->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread07, 8, 0, 1, 1);

        checkBox_02 = new QCheckBox(tab_Main);
        checkBox_02->setObjectName(QString::fromUtf8("checkBox_02"));
        sizePolicy1.setHeightForWidth(checkBox_02->sizePolicy().hasHeightForWidth());
        checkBox_02->setSizePolicy(sizePolicy1);
        checkBox_02->setMaximumSize(QSize(16777215, 16));
        checkBox_02->setFont(font7);
        checkBox_02->setChecked(true);

        gridLayout_3->addWidget(checkBox_02, 3, 1, 1, 1);

        label_NoteThread00 = new QLabel(tab_Main);
        label_NoteThread00->setObjectName(QString::fromUtf8("label_NoteThread00"));
        label_NoteThread00->setMinimumSize(QSize(100, 0));
        label_NoteThread00->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread00, 1, 0, 1, 1);

        label_timeThreadRuntime_08 = new QLabel(tab_Main);
        label_timeThreadRuntime_08->setObjectName(QString::fromUtf8("label_timeThreadRuntime_08"));
        label_timeThreadRuntime_08->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_08->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_08, 9, 4, 1, 1);

        spinBox_timeMinThreadDelay_08 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_08->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_08"));
        spinBox_timeMinThreadDelay_08->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_08->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_08, 9, 5, 1, 1);

        lineEdit_Alg_inThr07 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr07->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr07"));
        lineEdit_Alg_inThr07->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr07->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr07, 8, 3, 1, 1);

        label_timeThreadRuntime_00 = new QLabel(tab_Main);
        label_timeThreadRuntime_00->setObjectName(QString::fromUtf8("label_timeThreadRuntime_00"));
        label_timeThreadRuntime_00->setMaximumSize(QSize(16777215, 20));

        gridLayout_3->addWidget(label_timeThreadRuntime_00, 1, 4, 1, 1);

        spinBox_timeMinThreadDelay_04 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_04->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_04"));
        spinBox_timeMinThreadDelay_04->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_04->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_04, 5, 5, 1, 1);

        checkBox_10 = new QCheckBox(tab_Main);
        checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));
        checkBox_10->setMaximumSize(QSize(16777215, 16));
        checkBox_10->setFont(font7);

        gridLayout_3->addWidget(checkBox_10, 12, 1, 1, 1);

        label_20 = new QLabel(tab_Main);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font9);

        gridLayout_3->addWidget(label_20, 0, 0, 1, 1);

        spinBox_timeMinThreadDelay_02 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_02->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_02"));
        spinBox_timeMinThreadDelay_02->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_02->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_02, 3, 5, 1, 1);

        lineEdit_Alg_inThr00 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr00->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr00"));
        lineEdit_Alg_inThr00->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr00->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr00, 1, 3, 1, 1);

        spinBox_timeMinThreadDelay_00 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_00->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_00"));
        spinBox_timeMinThreadDelay_00->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_00->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_00, 1, 5, 1, 1);

        label_NoteThread04 = new QLabel(tab_Main);
        label_NoteThread04->setObjectName(QString::fromUtf8("label_NoteThread04"));
        label_NoteThread04->setMinimumSize(QSize(100, 0));
        label_NoteThread04->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread04, 5, 0, 1, 1);

        spinBox_timeMinThreadDelay_03 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_03->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_03"));
        spinBox_timeMinThreadDelay_03->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_03->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_03, 4, 5, 1, 1);

        spinBox_timeMinThreadDelay_09 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_09->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_09"));
        spinBox_timeMinThreadDelay_09->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_09->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_09, 10, 5, 1, 1);

        label_timeThreadRuntime_2 = new QLabel(tab_Main);
        label_timeThreadRuntime_2->setObjectName(QString::fromUtf8("label_timeThreadRuntime_2"));
        label_timeThreadRuntime_2->setFont(font9);

        gridLayout_3->addWidget(label_timeThreadRuntime_2, 0, 4, 1, 1);

        checkBox_06 = new QCheckBox(tab_Main);
        checkBox_06->setObjectName(QString::fromUtf8("checkBox_06"));
        sizePolicy1.setHeightForWidth(checkBox_06->sizePolicy().hasHeightForWidth());
        checkBox_06->setSizePolicy(sizePolicy1);
        checkBox_06->setMaximumSize(QSize(16777215, 16));
        checkBox_06->setFont(font7);

        gridLayout_3->addWidget(checkBox_06, 7, 1, 1, 1);

        label_timeThreadRuntime_04 = new QLabel(tab_Main);
        label_timeThreadRuntime_04->setObjectName(QString::fromUtf8("label_timeThreadRuntime_04"));
        label_timeThreadRuntime_04->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_04->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_04, 5, 4, 1, 1);

        label_timeThreadRuntime_05 = new QLabel(tab_Main);
        label_timeThreadRuntime_05->setObjectName(QString::fromUtf8("label_timeThreadRuntime_05"));
        label_timeThreadRuntime_05->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_05->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_05, 6, 4, 1, 1);

        label_NoteThread10 = new QLabel(tab_Main);
        label_NoteThread10->setObjectName(QString::fromUtf8("label_NoteThread10"));
        label_NoteThread10->setMinimumSize(QSize(100, 0));
        label_NoteThread10->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread10, 12, 0, 1, 1);

        lineEdit_Alg_inThr05 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr05->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr05"));
        lineEdit_Alg_inThr05->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr05->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr05, 6, 3, 1, 1);

        label_NoteThread11 = new QLabel(tab_Main);
        label_NoteThread11->setObjectName(QString::fromUtf8("label_NoteThread11"));
        label_NoteThread11->setMinimumSize(QSize(100, 0));
        label_NoteThread11->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread11, 14, 0, 1, 1);

        label_NoteThread01 = new QLabel(tab_Main);
        label_NoteThread01->setObjectName(QString::fromUtf8("label_NoteThread01"));
        label_NoteThread01->setMinimumSize(QSize(100, 0));
        label_NoteThread01->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread01, 2, 0, 1, 1);

        lineEdit_Alg_inThr06 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr06->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr06"));
        lineEdit_Alg_inThr06->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr06->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr06, 7, 3, 1, 1);

        spinBox_timeMinThreadDelay_01 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_01->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_01"));
        spinBox_timeMinThreadDelay_01->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_01->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_01, 2, 5, 1, 1);

        label_timeThreadRuntime_03 = new QLabel(tab_Main);
        label_timeThreadRuntime_03->setObjectName(QString::fromUtf8("label_timeThreadRuntime_03"));
        label_timeThreadRuntime_03->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_03->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_03, 4, 4, 1, 1);

        label_timeThreadRuntime_06 = new QLabel(tab_Main);
        label_timeThreadRuntime_06->setObjectName(QString::fromUtf8("label_timeThreadRuntime_06"));
        label_timeThreadRuntime_06->setMaximumSize(QSize(16777215, 20));
        label_timeThreadRuntime_06->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_timeThreadRuntime_06, 7, 4, 1, 1);

        checkBox_01 = new QCheckBox(tab_Main);
        checkBox_01->setObjectName(QString::fromUtf8("checkBox_01"));
        checkBox_01->setEnabled(true);
        sizePolicy1.setHeightForWidth(checkBox_01->sizePolicy().hasHeightForWidth());
        checkBox_01->setSizePolicy(sizePolicy1);
        checkBox_01->setMaximumSize(QSize(16777215, 16));
        checkBox_01->setFont(font7);
        checkBox_01->setCheckable(true);
        checkBox_01->setChecked(true);

        gridLayout_3->addWidget(checkBox_01, 2, 1, 1, 1);

        label_22 = new QLabel(tab_Main);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setSizeIncrement(QSize(0, 0));
        label_22->setFont(font9);

        gridLayout_3->addWidget(label_22, 0, 3, 1, 1, Qt::AlignHCenter);

        lineEdit_Alg_inThr08 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr08->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr08"));
        lineEdit_Alg_inThr08->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr08->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr08, 9, 3, 1, 1);

        lineEdit_Alg_inThr02 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr02->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr02"));
        lineEdit_Alg_inThr02->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr02->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr02, 3, 3, 1, 1);

        lineEdit_Alg_inThr11 = new QLineEdit(tab_Main);
        lineEdit_Alg_inThr11->setObjectName(QString::fromUtf8("lineEdit_Alg_inThr11"));
        lineEdit_Alg_inThr11->setMaximumSize(QSize(16777215, 20));
        lineEdit_Alg_inThr11->setFont(font7);

        gridLayout_3->addWidget(lineEdit_Alg_inThr11, 14, 3, 1, 1);

        label_NoteThread09 = new QLabel(tab_Main);
        label_NoteThread09->setObjectName(QString::fromUtf8("label_NoteThread09"));
        label_NoteThread09->setMinimumSize(QSize(100, 0));
        label_NoteThread09->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread09, 10, 0, 1, 1);

        label_NoteThread08 = new QLabel(tab_Main);
        label_NoteThread08->setObjectName(QString::fromUtf8("label_NoteThread08"));
        label_NoteThread08->setMinimumSize(QSize(100, 0));
        label_NoteThread08->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread08, 9, 0, 1, 1);

        label_NoteThread06 = new QLabel(tab_Main);
        label_NoteThread06->setObjectName(QString::fromUtf8("label_NoteThread06"));
        label_NoteThread06->setMinimumSize(QSize(100, 0));
        label_NoteThread06->setFont(font10);

        gridLayout_3->addWidget(label_NoteThread06, 7, 0, 1, 1);

        spinBox_timeMinThreadDelay_10 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_10->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_10"));
        spinBox_timeMinThreadDelay_10->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_10->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_10, 12, 5, 1, 1);

        spinBox_timeMinThreadDelay_11 = new QSpinBox(tab_Main);
        spinBox_timeMinThreadDelay_11->setObjectName(QString::fromUtf8("spinBox_timeMinThreadDelay_11"));
        spinBox_timeMinThreadDelay_11->setMaximumSize(QSize(16777215, 20));
        spinBox_timeMinThreadDelay_11->setMaximum(999);

        gridLayout_3->addWidget(spinBox_timeMinThreadDelay_11, 14, 5, 1, 1);


        gridLayout_8->addLayout(gridLayout_3, 14, 0, 1, 1);

        label_3 = new QLabel(tab_Main);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMaximumSize(QSize(16777215, 20));
        label_3->setFont(font8);

        gridLayout_8->addWidget(label_3, 8, 0, 1, 1);

        label_4 = new QLabel(tab_Main);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMaximumSize(QSize(16777215, 20));
        label_4->setFont(font8);

        gridLayout_8->addWidget(label_4, 1, 0, 1, 1);

        label = new QLabel(tab_Main);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(16777215, 20));
        label->setFont(font8);

        gridLayout_8->addWidget(label, 3, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        pushButton_RulerStart = new QPushButton(tab_Main);
        pushButton_RulerStart->setObjectName(QString::fromUtf8("pushButton_RulerStart"));
        pushButton_RulerStart->setMinimumSize(QSize(60, 60));
        pushButton_RulerStart->setMaximumSize(QSize(16777215, 16777215));
        pushButton_RulerStart->setFont(font8);
        pushButton_RulerStart->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(250, 150, 190);\n"
" color:black;\n"
"  border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #BEBEBE, stop: 1 #D7D7D7);\n"
"}"));

        gridLayout_17->addWidget(pushButton_RulerStart, 1, 6, 1, 1);

        pushButton_Start3DRuler = new QPushButton(tab_Main);
        pushButton_Start3DRuler->setObjectName(QString::fromUtf8("pushButton_Start3DRuler"));
        pushButton_Start3DRuler->setMinimumSize(QSize(60, 60));
        pushButton_Start3DRuler->setFont(font8);
        pushButton_Start3DRuler->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(255, 151, 57);\n"
"  color: black; \n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #FF7832, stop: 1 #FF9739);\n"
"}"));

        gridLayout_17->addWidget(pushButton_Start3DRuler, 1, 7, 1, 1);

        pushButton_DarkNet = new QPushButton(tab_Main);
        pushButton_DarkNet->setObjectName(QString::fromUtf8("pushButton_DarkNet"));
        pushButton_DarkNet->setMinimumSize(QSize(60, 60));
        pushButton_DarkNet->setMaximumSize(QSize(16777215, 16777215));
        pushButton_DarkNet->setFont(font8);
        pushButton_DarkNet->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(140,	210, 248);\n"
" color:black;\n"
"  border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #BEBEBE, stop: 1 #D7D7D7);\n"
"}"));

        gridLayout_17->addWidget(pushButton_DarkNet, 1, 5, 1, 1);

        pushButton_OutImgWindow = new QPushButton(tab_Main);
        pushButton_OutImgWindow->setObjectName(QString::fromUtf8("pushButton_OutImgWindow"));
        pushButton_OutImgWindow->setMinimumSize(QSize(60, 60));
        pushButton_OutImgWindow->setMaximumSize(QSize(16777215, 16777215));
        pushButton_OutImgWindow->setFont(font8);
        pushButton_OutImgWindow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(255, 255, 0);\n"
"  color:black;\n"
"  border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #BEBEBE, stop: 1 #D7D7D7);\n"
"}"));

        gridLayout_17->addWidget(pushButton_OutImgWindow, 1, 3, 1, 1);

        pushButton_ResultVideoStream = new QPushButton(tab_Main);
        pushButton_ResultVideoStream->setObjectName(QString::fromUtf8("pushButton_ResultVideoStream"));
        pushButton_ResultVideoStream->setMinimumSize(QSize(60, 60));
        pushButton_ResultVideoStream->setMaximumSize(QSize(16777215, 16777215));
        pushButton_ResultVideoStream->setFont(font8);
        pushButton_ResultVideoStream->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(48, 172, 119);\n"
"  color:black;\n"
"  border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #BEBEBE, stop: 1 #D7D7D7);\n"
"}"));

        gridLayout_17->addWidget(pushButton_ResultVideoStream, 1, 4, 1, 1);


        gridLayout_8->addLayout(gridLayout_17, 16, 0, 1, 1);

        tabWidget_ProgSettings->addTab(tab_Main, QString());
        tab_Add = new QWidget();
        tab_Add->setObjectName(QString::fromUtf8("tab_Add"));
        gridLayout_11 = new QGridLayout(tab_Add);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_5 = new QLabel(tab_Add);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font8);

        gridLayout_11->addWidget(label_5, 3, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        checkBox_WriteResultVideo = new QCheckBox(tab_Add);
        checkBox_WriteResultVideo->setObjectName(QString::fromUtf8("checkBox_WriteResultVideo"));
        checkBox_WriteResultVideo->setMaximumSize(QSize(16777215, 15));
        checkBox_WriteResultVideo->setFont(font);
        checkBox_WriteResultVideo->setTabletTracking(false);

        gridLayout_2->addWidget(checkBox_WriteResultVideo, 4, 0, 1, 1);

        checkBox_WriteSourceVideo = new QCheckBox(tab_Add);
        checkBox_WriteSourceVideo->setObjectName(QString::fromUtf8("checkBox_WriteSourceVideo"));
        checkBox_WriteSourceVideo->setMaximumSize(QSize(16777215, 15));
        checkBox_WriteSourceVideo->setFont(font);

        gridLayout_2->addWidget(checkBox_WriteSourceVideo, 6, 0, 1, 1);

        pushButton_WriteOneStereoPair = new QPushButton(tab_Add);
        pushButton_WriteOneStereoPair->setObjectName(QString::fromUtf8("pushButton_WriteOneStereoPair"));
        pushButton_WriteOneStereoPair->setFont(font7);

        gridLayout_2->addWidget(pushButton_WriteOneStereoPair, 6, 2, 1, 1);

        pushButton_WriteExp = new QPushButton(tab_Add);
        pushButton_WriteExp->setObjectName(QString::fromUtf8("pushButton_WriteExp"));
        pushButton_WriteExp->setFont(font7);

        gridLayout_2->addWidget(pushButton_WriteExp, 4, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 6, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_2, 4, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_6, 6, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, -1, -1);
        horizontalSlider_Scale = new QSlider(tab_Add);
        horizontalSlider_Scale->setObjectName(QString::fromUtf8("horizontalSlider_Scale"));
        horizontalSlider_Scale->setMinimum(5);
        horizontalSlider_Scale->setMaximum(500);
        horizontalSlider_Scale->setValue(100);
        horizontalSlider_Scale->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Scale, 5, 1, 1, 1);

        label_29 = new QLabel(tab_Add);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_4->addWidget(label_29, 5, 0, 1, 1);

        label_26 = new QLabel(tab_Add);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_4->addWidget(label_26, 0, 2, 1, 1);

        label_27 = new QLabel(tab_Add);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_4->addWidget(label_27, 1, 2, 1, 1);

        label_25 = new QLabel(tab_Add);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_4->addWidget(label_25, 4, 0, 1, 1);

        horizontalSlider_Ty = new QSlider(tab_Add);
        horizontalSlider_Ty->setObjectName(QString::fromUtf8("horizontalSlider_Ty"));
        horizontalSlider_Ty->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Ty->setMinimum(-2000);
        horizontalSlider_Ty->setMaximum(2000);
        horizontalSlider_Ty->setSingleStep(10);
        horizontalSlider_Ty->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Ty, 1, 3, 1, 1);

        label_30 = new QLabel(tab_Add);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_4->addWidget(label_30, 5, 2, 1, 1);

        horizontalSlider_Tx = new QSlider(tab_Add);
        horizontalSlider_Tx->setObjectName(QString::fromUtf8("horizontalSlider_Tx"));
        horizontalSlider_Tx->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Tx->setMinimum(-2000);
        horizontalSlider_Tx->setMaximum(2000);
        horizontalSlider_Tx->setSingleStep(10);
        horizontalSlider_Tx->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Tx, 0, 3, 1, 1);

        horizontalSlider_Dz = new QSlider(tab_Add);
        horizontalSlider_Dz->setObjectName(QString::fromUtf8("horizontalSlider_Dz"));
        horizontalSlider_Dz->setMinimum(-5000);
        horizontalSlider_Dz->setMaximum(0);
        horizontalSlider_Dz->setSingleStep(10);
        horizontalSlider_Dz->setValue(-3000);
        horizontalSlider_Dz->setSliderPosition(-3000);
        horizontalSlider_Dz->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Dz, 5, 3, 1, 1);

        horizontalSlider_Rx = new QSlider(tab_Add);
        horizontalSlider_Rx->setObjectName(QString::fromUtf8("horizontalSlider_Rx"));
        horizontalSlider_Rx->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Rx->setMinimum(-90);
        horizontalSlider_Rx->setMaximum(90);
        horizontalSlider_Rx->setSingleStep(5);
        horizontalSlider_Rx->setValue(0);
        horizontalSlider_Rx->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Rx, 0, 1, 1, 1);

        horizontalSlider_Ry = new QSlider(tab_Add);
        horizontalSlider_Ry->setObjectName(QString::fromUtf8("horizontalSlider_Ry"));
        horizontalSlider_Ry->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Ry->setMinimum(-90);
        horizontalSlider_Ry->setMaximum(90);
        horizontalSlider_Ry->setSingleStep(5);
        horizontalSlider_Ry->setValue(0);
        horizontalSlider_Ry->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Ry, 1, 1, 1, 1);

        horizontalSlider_Rz = new QSlider(tab_Add);
        horizontalSlider_Rz->setObjectName(QString::fromUtf8("horizontalSlider_Rz"));
        horizontalSlider_Rz->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Rz->setTabletTracking(false);
        horizontalSlider_Rz->setMinimum(-90);
        horizontalSlider_Rz->setMaximum(90);
        horizontalSlider_Rz->setSingleStep(5);
        horizontalSlider_Rz->setValue(0);
        horizontalSlider_Rz->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Rz, 4, 1, 1, 1);

        label_28 = new QLabel(tab_Add);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_4->addWidget(label_28, 4, 2, 1, 1);

        horizontalSlider_Tz = new QSlider(tab_Add);
        horizontalSlider_Tz->setObjectName(QString::fromUtf8("horizontalSlider_Tz"));
        horizontalSlider_Tz->setMaximumSize(QSize(16777215, 20));
        horizontalSlider_Tz->setMinimum(-5000);
        horizontalSlider_Tz->setMaximum(0);
        horizontalSlider_Tz->setSingleStep(10);
        horizontalSlider_Tz->setValue(-2000);
        horizontalSlider_Tz->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_Tz, 4, 3, 1, 1);

        label_24 = new QLabel(tab_Add);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_4->addWidget(label_24, 1, 0, 1, 1);

        label_23 = new QLabel(tab_Add);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_4->addWidget(label_23, 0, 0, 1, 1);


        gridLayout_11->addLayout(gridLayout_4, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_2, 0, 0, 1, 1);

        label_9 = new QLabel(tab_Add);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QFont font12;
        font12.setPointSize(10);
        font12.setBold(true);
        font12.setItalic(false);
        label_9->setFont(font12);

        gridLayout_11->addWidget(label_9, 1, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        lineEdit_SNCam02 = new QLineEdit(tab_Add);
        lineEdit_SNCam02->setObjectName(QString::fromUtf8("lineEdit_SNCam02"));

        gridLayout_19->addWidget(lineEdit_SNCam02, 1, 1, 1, 1);

        label_SNCam02 = new QLabel(tab_Add);
        label_SNCam02->setObjectName(QString::fromUtf8("label_SNCam02"));

        gridLayout_19->addWidget(label_SNCam02, 1, 0, 1, 1);

        lineEdit_SNCam01 = new QLineEdit(tab_Add);
        lineEdit_SNCam01->setObjectName(QString::fromUtf8("lineEdit_SNCam01"));

        gridLayout_19->addWidget(lineEdit_SNCam01, 0, 1, 1, 1);

        label_SNCam01 = new QLabel(tab_Add);
        label_SNCam01->setObjectName(QString::fromUtf8("label_SNCam01"));

        gridLayout_19->addWidget(label_SNCam01, 0, 0, 1, 1);

        label_FrameSize = new QLabel(tab_Add);
        label_FrameSize->setObjectName(QString::fromUtf8("label_FrameSize"));

        gridLayout_19->addWidget(label_FrameSize, 2, 0, 1, 1);

        lineEdit_FrameSize = new QLineEdit(tab_Add);
        lineEdit_FrameSize->setObjectName(QString::fromUtf8("lineEdit_FrameSize"));

        gridLayout_19->addWidget(lineEdit_FrameSize, 2, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_19, 5, 0, 1, 1);

        tabWidget_ProgSettings->addTab(tab_Add, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_22 = new QGridLayout(tab);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        lineEdit_VideoStreamPort = new QLineEdit(tab);
        lineEdit_VideoStreamPort->setObjectName(QString::fromUtf8("lineEdit_VideoStreamPort"));

        gridLayout_21->addWidget(lineEdit_VideoStreamPort, 0, 1, 1, 1);

        spinBox_VideoStreamQualiy = new QSpinBox(tab);
        spinBox_VideoStreamQualiy->setObjectName(QString::fromUtf8("spinBox_VideoStreamQualiy"));
        spinBox_VideoStreamQualiy->setFont(font7);
        spinBox_VideoStreamQualiy->setValue(50);

        gridLayout_21->addWidget(spinBox_VideoStreamQualiy, 1, 1, 1, 1);

        label_VideoStreamPort = new QLabel(tab);
        label_VideoStreamPort->setObjectName(QString::fromUtf8("label_VideoStreamPort"));

        gridLayout_21->addWidget(label_VideoStreamPort, 0, 0, 1, 1);

        label_VideoStreamQualiy = new QLabel(tab);
        label_VideoStreamQualiy->setObjectName(QString::fromUtf8("label_VideoStreamQualiy"));

        gridLayout_21->addWidget(label_VideoStreamQualiy, 1, 0, 1, 1);

        pushButton_ResultVideoStream_2 = new QPushButton(tab);
        pushButton_ResultVideoStream_2->setObjectName(QString::fromUtf8("pushButton_ResultVideoStream_2"));
        pushButton_ResultVideoStream_2->setMaximumSize(QSize(125, 22));

        gridLayout_21->addWidget(pushButton_ResultVideoStream_2, 2, 1, 1, 1, Qt::AlignRight);


        gridLayout_22->addLayout(gridLayout_21, 1, 0, 1, 1);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        QFont font13;
        font13.setPointSize(13);
        font13.setBold(true);
        font13.setItalic(true);
        label_16->setFont(font13);

        gridLayout_22->addWidget(label_16, 0, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 573, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_22->addItem(verticalSpacer_5, 3, 0, 1, 1);

        tabWidget_ProgSettings->addTab(tab, QString());

        verticalLayout_ControlSection->addWidget(tabWidget_ProgSettings);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        pushButton_Start = new QPushButton(verticalLayoutWidget);
        pushButton_Start->setObjectName(QString::fromUtf8("pushButton_Start"));
        sizePolicy1.setHeightForWidth(pushButton_Start->sizePolicy().hasHeightForWidth());
        pushButton_Start->setSizePolicy(sizePolicy1);
        pushButton_Start->setMinimumSize(QSize(65, 65));
        pushButton_Start->setMaximumSize(QSize(16777215, 16777215));
        QFont font14;
        font14.setPointSize(12);
        font14.setBold(true);
        font14.setItalic(false);
        font14.setUnderline(false);
        font14.setStrikeOut(false);
        font14.setKerning(true);
        pushButton_Start->setFont(font14);
        pushButton_Start->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(230, 110, 50);\n"
"  color: black; \n"
"  border-radius: 5px;\n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #FF7832, stop: 1 #FF9739);\n"
"}"));

        gridLayout_6->addWidget(pushButton_Start, 0, 0, 1, 1);

        pushButton_Settings = new QPushButton(verticalLayoutWidget);
        pushButton_Settings->setObjectName(QString::fromUtf8("pushButton_Settings"));
        pushButton_Settings->setMinimumSize(QSize(65, 65));
        pushButton_Settings->setMaximumSize(QSize(95, 95));
        pushButton_Settings->setFont(font3);
        pushButton_Settings->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: rgb(216,     83,     25);\n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #FF7832, stop: 1 #FF9739);\n"
"}"));

        gridLayout_6->addWidget(pushButton_Settings, 0, 1, 1, 1);

        pushButton_aboutQt = new QPushButton(verticalLayoutWidget);
        pushButton_aboutQt->setObjectName(QString::fromUtf8("pushButton_aboutQt"));
        pushButton_aboutQt->setMinimumSize(QSize(60, 60));
        pushButton_aboutQt->setMaximumSize(QSize(95, 95));
        pushButton_aboutQt->setFont(font9);
        pushButton_aboutQt->setStyleSheet(QString::fromUtf8("color:red;\n"
"backgrond-color:rgba(236,101,80,1);"));

        gridLayout_6->addWidget(pushButton_aboutQt, 0, 2, 1, 1);


        verticalLayout_ControlSection->addLayout(gridLayout_6);

        splitter_2->addWidget(verticalLayoutWidget);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1349, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(action_Show);
        toolBar->addAction(action_Hide);

        retranslateUi(MainWindow);

        tabWidget_ProgParam->setCurrentIndex(0);
        tabWidget_ProgSettings->setCurrentIndex(0);
        comboBoxMain_ModeWork->setCurrentIndex(0);
        comboBoxMain_ImgType->setCurrentIndex(0);
        comboBoxMain_SelectCam->setCurrentIndex(0);
        comboBox_CamType->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Hide->setText(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\321\217\321\202\320\260\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        action_Hide->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+1", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Show->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        action_Show->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+2", nullptr));
#endif // QT_CONFIG(shortcut)
        label_StatusOnWindow->setText(QCoreApplication::translate("MainWindow", "Dat & Time ", nullptr));
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_StateInfo), QCoreApplication::translate("MainWindow", "State Info", nullptr));
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_AlgList), QCoreApplication::translate("MainWindow", "Alg List", nullptr));
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_ConfigFile), QCoreApplication::translate("MainWindow", "ConfigFile", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_AirCraft_xyz->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", " Aircraft ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_AirCraft_xyz->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_AirCraft_xyz->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_AirCraft_xyz->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_AirCraft_xyz->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "xyz AirCraft", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_AirCraft_xyz->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "xyz GlidePath", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_AirCraft_xyz->verticalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "xyz Deviation", nullptr));

        const bool __sortingEnabled = tableWidget_AirCraft_xyz->isSortingEnabled();
        tableWidget_AirCraft_xyz->setSortingEnabled(false);
        tableWidget_AirCraft_xyz->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_AirCraft_rtf->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", " Aircraft ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_AirCraft_rtf->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "ro", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_AirCraft_rtf->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "theta", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_AirCraft_rtf->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "phi", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_AirCraft_rtf->verticalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "AirCraft    ", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_AirCraft_rtf->verticalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "GlidePath    ", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_AirCraft_rtf->verticalHeaderItem(2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Deviation    ", nullptr));

        const bool __sortingEnabled1 = tableWidget_AirCraft_rtf->isSortingEnabled();
        tableWidget_AirCraft_rtf->setSortingEnabled(false);
        tableWidget_AirCraft_rtf->setSortingEnabled(__sortingEnabled1);

        label_LogDataWriteSpeed_Aircraft->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\320\270 \320\264\320\260\320\262\320\275\320\275\321\213\321\205, \320\274\321\201", nullptr));
        pushButton_WiriteLogAirCraft3DCoordinates->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214  \320\272\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202\321\213 \320\233\320\220 \320\262 LOG-\321\204\320\260\320\271\320\273", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\321\213 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\321\217 \320\277\321\200\321\217\320\274\320\276\320\271 \320\262 3D", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\264\320\260\320\275\320\270\320\265 \320\263\320\273\320\270\321\201\321\201\320\260\320\264\321\213 (\321\202\321\200\320\260\320\265\320\272\321\202\320\276\321\200\320\270\320\270 \320\277\320\276\321\201\320\260\320\264\320\272\320\270 \320\233\320\220)", nullptr));
        checkBox_WiriteLogAirCraft3DCoordinates->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214 LOG-\321\204\320\260\320\271\320\273\320\260 \320\272\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202 \320\233\320\220", nullptr));
        label_AirCraftParamEquation_y->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        label_AirCraftParamEquation_z->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        label_AirCraftParamEquation_b->setText(QCoreApplication::translate("MainWindow", "b (x0, y0, z0)", nullptr));
        label_AirCraftParamEquation_a->setText(QCoreApplication::translate("MainWindow", " a (m, n, p)", nullptr));
        label_AirCraftParamEquation_x->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\320\236\321\201\320\270", nullptr));
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_AirCraft), QCoreApplication::translate("MainWindow", "AirCraft", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Aurora", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_xyzDetObj->horizontalHeaderItem(0);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_xyzDetObj->horizontalHeaderItem(1);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_xyzDetObj->horizontalHeaderItem(2);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_xyzDetObj->horizontalHeaderItem(3);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
#if QT_CONFIG(statustip)
        tableWidget_xyzDetObj->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(accessibility)
        tableWidget_xyzDetObj->setAccessibleDescription(QString());
#endif // QT_CONFIG(accessibility)
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_DetectObjSize->horizontalHeaderItem(0);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "L", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_DetectObjSize->horizontalHeaderItem(1);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "W", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_DetectObjSize->horizontalHeaderItem(2);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "H", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget_DetectObjSize->horizontalHeaderItem(3);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "Length", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget_DetectObjSize->horizontalHeaderItem(4);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "Width", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget_DetectObjSize->horizontalHeaderItem(5);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "Num Points", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget_DetectObjSize->verticalHeaderItem(0);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "Obj", nullptr));
        label_DarkNet_Reply->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_DarkNet->setText(QCoreApplication::translate("MainWindow", "DarkNet", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget_TwoPoint->horizontalHeaderItem(0);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidget_TwoPoint->horizontalHeaderItem(1);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableWidget_TwoPoint->horizontalHeaderItem(2);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tableWidget_TwoPoint->horizontalHeaderItem(3);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "\321\200\320\260\321\201\321\201\321\202\320\276\321\217\320\275\320\270\320\265", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = tableWidget_TwoPoint->verticalHeaderItem(0);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "01", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = tableWidget_TwoPoint->verticalHeaderItem(1);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("MainWindow", "02", nullptr));

        const bool __sortingEnabled2 = tableWidget_TwoPoint->isSortingEnabled();
        tableWidget_TwoPoint->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem31 = tableWidget_TwoPoint->item(0, 0);
        ___qtablewidgetitem31->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = tableWidget_TwoPoint->item(0, 1);
        ___qtablewidgetitem32->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem33 = tableWidget_TwoPoint->item(0, 2);
        ___qtablewidgetitem33->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem34 = tableWidget_TwoPoint->item(0, 3);
        ___qtablewidgetitem34->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        QTableWidgetItem *___qtablewidgetitem35 = tableWidget_TwoPoint->item(1, 0);
        ___qtablewidgetitem35->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem36 = tableWidget_TwoPoint->item(1, 1);
        ___qtablewidgetitem36->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem37 = tableWidget_TwoPoint->item(1, 2);
        ___qtablewidgetitem37->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        QTableWidgetItem *___qtablewidgetitem38 = tableWidget_TwoPoint->item(1, 3);
        ___qtablewidgetitem38->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        tableWidget_TwoPoint->setSortingEnabled(__sortingEnabled2);

#if QT_CONFIG(statustip)
        tableWidget_TwoPoint->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        tableWidget_TwoPoint->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        tableWidget_TwoPoint->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
#if QT_CONFIG(accessibility)
        tableWidget_TwoPoint->setAccessibleDescription(QString());
#endif // QT_CONFIG(accessibility)
        label_11->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>\320\240\320\242\320\232</p></body></html>", nullptr));
        QTableWidgetItem *___qtablewidgetitem39 = tableWidget_xyz->horizontalHeaderItem(0);
        ___qtablewidgetitem39->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem40 = tableWidget_xyz->horizontalHeaderItem(1);
        ___qtablewidgetitem40->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        QTableWidgetItem *___qtablewidgetitem41 = tableWidget_xyz->horizontalHeaderItem(2);
        ___qtablewidgetitem41->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        QTableWidgetItem *___qtablewidgetitem42 = tableWidget_xyz->horizontalHeaderItem(3);
        ___qtablewidgetitem42->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
#if QT_CONFIG(statustip)
        tableWidget_xyz->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(accessibility)
        tableWidget_xyz->setAccessibleDescription(QString());
#endif // QT_CONFIG(accessibility)
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_UnderWater), QCoreApplication::translate("MainWindow", "UnderWater", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>\320\242\320\225\320\232\320\241\320\242 </p><p>\321\202\320\265\320\272\321\201\321\202</p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        tabWidget_ProgParam->setTabText(tabWidget_ProgParam->indexOf(tab_8), QCoreApplication::translate("MainWindow", "Page", nullptr));
        label_logo_WorkMode->setText(QString());
        label_logo->setText(QString());
        label_logo_CamTypy_2->setText(QString());
        label_logo_CamTypy->setText(QString());
        comboBoxMain_ModeWork->setItemText(0, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \320\240\320\242\320\232", nullptr));
        comboBoxMain_ModeWork->setItemText(1, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \320\232\320\260\320\273\320\270\320\261\321\200\320\276\320\262\320\272\320\270", nullptr));
        comboBoxMain_ModeWork->setItemText(2, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \"\320\220\320\262\321\200\320\276\321\200\320\260\"", nullptr));
        comboBoxMain_ModeWork->setItemText(3, QCoreApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \"\320\242\320\265\321\201\321\202 TCP\"", nullptr));

        pushButton_OpenFile1L->setText(QCoreApplication::translate("MainWindow", "Open File 01", nullptr));
        pushButton_OpenFile2R->setText(QCoreApplication::translate("MainWindow", "Open File 02", nullptr));
        comboBoxMain_ImgType->setItemText(0, QCoreApplication::translate("MainWindow", "0 >>> \320\230\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265 (\320\273\320\265\320\262\320\260\321\217 \320\272\320\260\320\274\320\265\321\200\320\260) + \320\264\320\276\320\277. \320\270\320\275\321\204\320\276\321\200\320\260\320\274\320\260\321\206\320\270\321\217", nullptr));
        comboBoxMain_ImgType->setItemText(1, QCoreApplication::translate("MainWindow", "1 \320\232\320\260\321\200\321\202\320\260 \320\264\320\270\321\201\320\277\320\260\321\200\320\260\321\202\320\275\320\276\321\201\321\202\320\270", nullptr));
        comboBoxMain_ImgType->setItemText(2, QCoreApplication::translate("MainWindow", "2 \320\237\321\200\320\270\320\274\320\270\321\202\320\270\320\262\321\213", nullptr));
        comboBoxMain_ImgType->setItemText(3, QCoreApplication::translate("MainWindow", "3 \320\236\321\200\320\270\320\263\320\270\320\275\320\260\320\273\321\214\320\275\320\276\320\265 (\320\273\320\265\320\262\320\260\321\217 \320\272\320\260\320\274\320\265\321\200\320\260)", nullptr));
        comboBoxMain_ImgType->setItemText(4, QCoreApplication::translate("MainWindow", "4 \320\236\321\200\320\270\320\263\320\270\320\275\320\260\320\273\321\214\320\275\320\276\320\265 (\320\277\321\200\320\260\320\262\320\260\321\217 \320\272\320\260\320\274\320\265\321\200\320\260)", nullptr));
        comboBoxMain_ImgType->setItemText(5, QCoreApplication::translate("MainWindow", "5 \320\241\321\202\320\265\321\200\320\265\320\276\320\277\320\260\321\200\320\260 (\320\273\320\265\320\262\320\276\320\265 + \320\277\321\200\320\260\320\262\320\276\320\265)", nullptr));
        comboBoxMain_ImgType->setItemText(6, QCoreApplication::translate("MainWindow", "6^^^  \320\264\320\273\321\217 \321\200\320\265\320\266\320\270\320\274\320\260 \320\220\320\262\321\200\320\276\321\200\320\260 (\320\237\320\276\320\270\321\201\320\272 \321\206\320\262\320\265\321\202\320\275\320\276\320\263\320\276 \320\276\320\261\321\212\320\265\320\272\321\202\320\260 \320\277\320\276 \320\272\320\276\320\275\321\202\321\203\321\200\321\203)", nullptr));
        comboBoxMain_ImgType->setItemText(7, QCoreApplication::translate("MainWindow", "7 3D \321\202\320\276\321\207\320\272\320\270 + \320\262\321\200\320\260\321\211\320\265\320\275\320\270\320\265", nullptr));
        comboBoxMain_ImgType->setItemText(8, QCoreApplication::translate("MainWindow", "8 \320\241\321\202\320\265\321\200\320\265\320\276\320\277\320\260\321\200\320\260  (\320\277\320\276\321\201\320\273\320\265 \321\200\320\265\320\272\321\202\320\270\321\204\320\270\320\272\320\260\321\206\320\270\320\270)", nullptr));
        comboBoxMain_ImgType->setItemText(9, QCoreApplication::translate("MainWindow", "9 \320\241\321\202\320\265\321\200\320\265\320\276\320\277\320\260\321\200\320\260  (\320\277\320\276\321\201\320\273\320\265 \320\277\321\200\320\265\320\264\320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270)", nullptr));
        comboBoxMain_ImgType->setItemText(10, QCoreApplication::translate("MainWindow", "10 \320\241\321\202\320\265\321\200\320\265\320\276\320\277\320\260\321\200\320\260  (\320\277\320\276\321\201\320\273\320\265 \320\277\321\200\320\265\320\264\320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270) [Gray]", nullptr));
        comboBoxMain_ImgType->setItemText(11, QCoreApplication::translate("MainWindow", "11 DarkNetDetector", nullptr));

        checkBox_setWarterCalibr->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\320\264\320\260", nullptr));
        pushButton_OpenCalibr->setText(QCoreApplication::translate("MainWindow", "   \320\222\321\213\320\261\321\200\320\260\321\202\321\214   ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217 \320\275\320\260 \320\262\321\213\321\205\320\276\320\264\320\265", nullptr));
        comboBoxMain_SelectCam->setItemText(0, QCoreApplication::translate("MainWindow", "0 \320\241\321\202\320\265\321\200\320\265\320\276 \320\272\320\260\320\274\320\265\321\200\320\260 Basler (by Serial Number)", nullptr));
        comboBoxMain_SelectCam->setItemText(1, QCoreApplication::translate("MainWindow", "1 \320\241\321\202\320\265\321\200\320\265\320\276 IP \320\272\320\260\320\274\320\265\321\200\321\213 (\321\201\321\202\320\260\321\200\321\213\320\271)", nullptr));
        comboBoxMain_SelectCam->setItemText(2, QCoreApplication::translate("MainWindow", "2 \320\241\321\202\320\265\321\200\320\265\320\276 \320\262\320\270\320\264\320\265\320\276 \321\204\320\260\320\271\320\273", nullptr));
        comboBoxMain_SelectCam->setItemText(3, QCoreApplication::translate("MainWindow", "3 \320\241\321\202\320\265\321\200\320\265\320\276 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", nullptr));
        comboBoxMain_SelectCam->setItemText(4, QCoreApplication::translate("MainWindow", "4 \320\241\321\202\320\265\321\200\320\265\320\276 \320\272\320\260\320\274\320\265\321\200\320\260 HikRobot (by Serial Number)", nullptr));
        comboBoxMain_SelectCam->setItemText(5, QCoreApplication::translate("MainWindow", "5 \320\241\321\202\320\265\321\200\320\265\320\276 \320\272\320\260\320\274\320\265\321\200\321\213 HikRobot (\320\272\320\260\320\274\320\265\321\200\320\260 01 \320\270 02 \320\277\320\260\321\200\320\260\320\273\320\273\320\265\320\273\321\214\320\275\320\276)", nullptr));
        comboBoxMain_SelectCam->setItemText(6, QCoreApplication::translate("MainWindow", "6 \320\241\321\202\320\265\321\200\320\265\320\276 IP \320\272\320\260\320\274\320\265\321\200\321\213 (\320\272\320\260\320\274\320\265\321\200\321\213  01 \320\270 02 \320\277\320\260\321\200\320\260\320\273\320\273\320\265\320\273\321\214\320\275\320\276) ", nullptr));
        comboBoxMain_SelectCam->setItemText(7, QCoreApplication::translate("MainWindow", "7 \320\241\321\202\320\265\321\200\320\265\320\276 \320\272\320\260\320\274\320\265\321\200\320\260 HikRobot (by IP address)", nullptr));
        comboBoxMain_SelectCam->setItemText(8, QCoreApplication::translate("MainWindow", "8 \320\241\321\202\320\265\321\200\320\265\320\276 \320\272\320\260\320\274\320\265\321\200\320\260 Basler (by IP address)", nullptr));

        comboBox_CamType->setItemText(0, QCoreApplication::translate("MainWindow", "0 (-1) --- \320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\273\321\214\320\275\320\260\321\217 \320\272\320\260\320\274\320\265\321\200\320\260 (\321\200\320\260\320\267\320\274\320\265\321\200 \320\272\320\260\320\264\321\200\320\260 \320\275\320\265 \320\274\320\265\320\275\321\217\320\265\321\202\321\201\321\217)", nullptr));
        comboBox_CamType->setItemText(1, QCoreApplication::translate("MainWindow", "1 (0)   --- \320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\272\320\260\320\274\320\265\321\200\321\213 \320\277\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216 \320\270\320\267 Config.xml", nullptr));
        comboBox_CamType->setItemText(2, QCoreApplication::translate("MainWindow", "2 (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 960x600", nullptr));
        comboBox_CamType->setItemText(3, QCoreApplication::translate("MainWindow", "3 (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1440x900", nullptr));
        comboBox_CamType->setItemText(4, QCoreApplication::translate("MainWindow", "4 (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1920x1200", nullptr));
        comboBox_CamType->setItemText(5, QCoreApplication::translate("MainWindow", "5 (2) +++ Basler :: acA1300-60gc :: 1280x1024  -> 960x600 ", nullptr));
        comboBox_CamType->setItemText(6, QCoreApplication::translate("MainWindow", "6 (2) +++  Basler :: acA1300-60gc :: 1280x1024  -> 1280x1024", nullptr));
        comboBox_CamType->setItemText(7, QCoreApplication::translate("MainWindow", "7 (3) ^^^ HikR :: MV-CA032-10GC :: 1280x1024  -> 960x720", nullptr));
        comboBox_CamType->setItemText(8, QCoreApplication::translate("MainWindow", "8 (4) === IP Cam :: StereoPi :: 640x480 ", nullptr));

        comboBox_CamType->setCurrentText(QCoreApplication::translate("MainWindow", "0 (-1) --- \320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\273\321\214\320\275\320\260\321\217 \320\272\320\260\320\274\320\265\321\200\320\260 (\321\200\320\260\320\267\320\274\320\265\321\200 \320\272\320\260\320\264\321\200\320\260 \320\275\320\265 \320\274\320\265\320\275\321\217\320\265\321\202\321\201\321\217)", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\277\320\276\321\202\320\276\320\272\320\260\320\274\320\270", nullptr));
        radioButton_line->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\321\200\320\265\320\275\320\270\320\265 \320\273\320\270\320\275\320\265\320\271\320\272\320\276\320\271", nullptr));
        radioButton_3D->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\276\320\261\321\212\320\265\320\272\321\202\320\260", nullptr));
        checkBox_03->setText(QCoreApplication::translate("MainWindow", "3) ", nullptr));
        label_timeMinThreadDela->setText(QCoreApplication::translate("MainWindow", "\320\233\320\270\320\274\320\270\321\202", nullptr));
        checkBox_08->setText(QCoreApplication::translate("MainWindow", "8)", nullptr));
        checkBox_09->setText(QCoreApplication::translate("MainWindow", "9)", nullptr));
        checkBox_00->setText(QCoreApplication::translate("MainWindow", "0)", nullptr));
        checkBox_07->setText(QCoreApplication::translate("MainWindow", "7)", nullptr));
        label_timeThreadRuntime_11->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_ThreadNum->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\202\320\276\320\272", nullptr));
        checkBox_11->setText(QCoreApplication::translate("MainWindow", "11)", nullptr));
        lineEdit_Alg_inThr01->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_timeThreadRuntime_01->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread05->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lineEdit_Alg_inThr04->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_timeThreadRuntime_09->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        checkBox_04->setText(QCoreApplication::translate("MainWindow", "4) ", nullptr));
        checkBox_05->setText(QCoreApplication::translate("MainWindow", "5) ", nullptr));
        label_NoteThread03->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lineEdit_Alg_inThr03->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_NoteThread02->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_10->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_02->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_07->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread07->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        checkBox_02->setText(QCoreApplication::translate("MainWindow", "2) ", nullptr));
        label_NoteThread00->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_08->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_00->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        checkBox_10->setText(QCoreApplication::translate("MainWindow", "10)", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", nullptr));
        lineEdit_Alg_inThr00->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_NoteThread04->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_2->setText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217", nullptr));
        checkBox_06->setText(QCoreApplication::translate("MainWindow", "6) ", nullptr));
        label_timeThreadRuntime_04->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_05->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread10->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lineEdit_Alg_inThr05->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_NoteThread11->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread01->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_03->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_timeThreadRuntime_06->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        checkBox_01->setText(QCoreApplication::translate("MainWindow", "1) ", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "\342\204\226 \320\220\320\273\320\263\320\276\321\200\320\270\321\202\320\274\320\260", nullptr));
        lineEdit_Alg_inThr02->setText(QCoreApplication::translate("MainWindow", "-1", nullptr));
        label_NoteThread09->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread08->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_NoteThread06->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \320\272 \321\204\320\260\320\271\320\273\321\203 \320\272\320\260\320\273\320\270\320\261\321\200\320\276\320\262\320\272\320\270", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 \321\200\320\265\320\266\320\270\320\274\320\260 \321\200\320\260\320\261\320\276\321\202\321\213", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\230\321\201\321\202\320\276\321\207\320\275\320\270\320\272 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\271", nullptr));
        pushButton_RulerStart->setText(QCoreApplication::translate("MainWindow", "Ruler", nullptr));
        pushButton_Start3DRuler->setText(QCoreApplication::translate("MainWindow", "Ruler 3D", nullptr));
        pushButton_DarkNet->setText(QCoreApplication::translate("MainWindow", "DarkNet", nullptr));
        pushButton_OutImgWindow->setText(QCoreApplication::translate("MainWindow", "Img Window", nullptr));
        pushButton_ResultVideoStream->setText(QCoreApplication::translate("MainWindow", "Video Stream", nullptr));
        tabWidget_ProgSettings->setTabText(tabWidget_ProgSettings->indexOf(tab_Main), QCoreApplication::translate("MainWindow", "Initial Settings", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214", nullptr));
        checkBox_WriteResultVideo->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\277\320\270\321\201\321\214 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\320\276\320\262 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270", nullptr));
        checkBox_WriteSourceVideo->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\277\320\270\321\201\321\214 \320\270\321\201\321\205\320\276\320\264\320\275\320\276\320\263\320\276 \321\201\321\202\320\265\321\200\320\265\320\276\320\262\320\270\320\264\320\265\320\276", nullptr));
        pushButton_WriteOneStereoPair->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214 \320\276\320\264\320\275\320\276\320\271 \321\201\321\202\320\265\321\200\320\265\320\276\320\277\320\260\321\200\321\213", nullptr));
        pushButton_WriteExp->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214 \320\255\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260 (1 \320\272\320\260\320\264\321\200)", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Scale", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Tx", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Ty", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Rz", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "Dz", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Tz", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Ry", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Rx", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\260\321\211\320\265\320\275\320\270\320\265 3D \320\276\320\261\320\273\320\260\320\272\320\260 \321\202\320\276\321\207\320\265\320\272", nullptr));
        label_SNCam02->setText(QCoreApplication::translate("MainWindow", "Serial Num Cam 02", nullptr));
        label_SNCam01->setText(QCoreApplication::translate("MainWindow", "Serial Num Cam 01", nullptr));
        label_FrameSize->setText(QCoreApplication::translate("MainWindow", "Frame Size", nullptr));
        tabWidget_ProgSettings->setTabText(tabWidget_ProgSettings->indexOf(tab_Add), QCoreApplication::translate("MainWindow", "Additional Settings", nullptr));
        label_VideoStreamPort->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\321\202", nullptr));
        label_VideoStreamQualiy->setText(QCoreApplication::translate("MainWindow", "\320\232\320\260\321\207\320\265\321\201\321\202\320\262\320\276 \321\201\321\202\321\200\320\270\320\274\320\260", nullptr));
        pushButton_ResultVideoStream_2->setText(QCoreApplication::translate("MainWindow", "Video Stream", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\222\320\270\320\264\320\265\320\276 \320\241\321\202\321\200\320\270\320\274\320\260", nullptr));
        tabWidget_ProgSettings->setTabText(tabWidget_ProgSettings->indexOf(tab), QCoreApplication::translate("MainWindow", "Video Stream", nullptr));
        pushButton_Start->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\320\272 ", nullptr));
        pushButton_Settings->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        pushButton_aboutQt->setText(QCoreApplication::translate("MainWindow", "about Qt", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
