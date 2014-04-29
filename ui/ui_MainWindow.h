/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue Apr 29 09:41:40 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_8;
    QPushButton *m_pbClose;
    QFrame *frame_3;
    QGridLayout *gridLayout_10;
    QLabel *m_lbObjectType;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QFrame *frame;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QSpinBox *m_sbNoOfVoxelsInX;
    QPushButton *m_pbCalculate;
    QPushButton *m_pbExport;
    QLabel *label_7;
    QCheckBox *m_cbIntegralVolume;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *m_sbNoiseLevel;
    QLabel *label_5;
    QDoubleSpinBox *m_minNorthY;
    QLabel *label_3;
    QLabel *label_2;
    QDoubleSpinBox *m_limit;
    QLabel *label;
    QDoubleSpinBox *m_dsVoxelLength;
    QDoubleSpinBox *m_maxNorthY;
    QPushButton *m_pbLoadLAS;
    QLabel *label_6;
    QPlainTextEdit *m_tHist;
    QPushButton *m_pbCreate;
    QDoubleSpinBox *m_minEastX;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_4;
    QDoubleSpinBox *m_maxEastX;
    QPushButton *m_pbSaveHist;
    QLabel *label_13;
    QComboBox *m_cbDataType;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_7;
    QSpinBox *m_sbBand1;
    QLabel *label_10;
    QPushButton *m_pbLoadHyper;
    QLabel *label_11;
    QLabel *label_12;
    QSpinBox *m_sbBand3;
    QSpinBox *m_sbBand2;
    QPushButton *m_updateBands;
    QCheckBox *m_cbHyperTexture;
    QSpacerItem *verticalSpacer;
    QCheckBox *m_cbUseLevel1Data;
    QPushButton *m_pbLoadIGM;
    QWidget *tab_3;
    QGridLayout *gridLayout_9;
    QComboBox *m_cbMaps;
    QSpacerItem *verticalSpacer_3;
    QPushButton *m_pbCreateMap;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_16;
    QLabel *label_33;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QLabel *label_34;
    QSpinBox *m_sbSampling;
    QSpinBox *m_sbThreshold;
    QSpinBox *m_sbBand;
    QLabel *label_31;
    QLabel *label_32;
    QWidget *tab_4;
    QGridLayout *gridLayout_11;
    QLabel *label_21;
    QLabel *label_16;
    QFrame *frame_7;
    QLabel *m_lbCamPositionX;
    QLabel *label_19;
    QLabel *label_17;
    QLabel *label_23;
    QLabel *label_24;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_20;
    QLabel *m_lbCamPositionZ;
    QLabel *label_26;
    QLabel *label_18;
    QSpinBox *m_sbRotDeg;
    QLabel *label_28;
    QLabel *m_lbCamPanRight;
    QLabel *m_lbCamPositionY;
    QLabel *label_14;
    QLabel *label_30;
    QLabel *label_15;
    QLabel *label_22;
    QLabel *label_27;
    QDoubleSpinBox *m_sbMoveStep;
    QLabel *label_29;
    QFrame *frame_5;
    QGridLayout *gridLayout_13;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_12;
    QLabel *label_25;
    QLabel *m_lbCamPanLeft;
    QLabel *m_lbCamTiltDown;
    QLabel *m_lbCamTiltUp;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 891);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QString::fromUtf8("s_mainWindowGridLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_8 = new QGridLayout(groupBox);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        m_pbClose = new QPushButton(groupBox);
        m_pbClose->setObjectName(QString::fromUtf8("m_pbClose"));

        gridLayout_8->addWidget(m_pbClose, 2, 4, 1, 1);

        frame_3 = new QFrame(groupBox);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_10 = new QGridLayout(frame_3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        m_lbObjectType = new QLabel(frame_3);
        m_lbObjectType->setObjectName(QString::fromUtf8("m_lbObjectType"));
        m_lbObjectType->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(m_lbObjectType, 1, 0, 1, 1);

        tabWidget = new QTabWidget(frame_3);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        frame = new QFrame(tab);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        m_sbNoOfVoxelsInX = new QSpinBox(groupBox_2);
        m_sbNoOfVoxelsInX->setObjectName(QString::fromUtf8("m_sbNoOfVoxelsInX"));
        m_sbNoOfVoxelsInX->setMinimum(2);
        m_sbNoOfVoxelsInX->setMaximum(9999999);
        m_sbNoOfVoxelsInX->setValue(20);

        gridLayout_4->addWidget(m_sbNoOfVoxelsInX, 0, 1, 1, 1);

        m_pbCalculate = new QPushButton(groupBox_2);
        m_pbCalculate->setObjectName(QString::fromUtf8("m_pbCalculate"));

        gridLayout_4->addWidget(m_pbCalculate, 3, 0, 1, 2);

        m_pbExport = new QPushButton(groupBox_2);
        m_pbExport->setObjectName(QString::fromUtf8("m_pbExport"));

        gridLayout_4->addWidget(m_pbExport, 4, 0, 1, 2);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 0, 0, 1, 1);

        m_cbIntegralVolume = new QCheckBox(groupBox_2);
        m_cbIntegralVolume->setObjectName(QString::fromUtf8("m_cbIntegralVolume"));

        gridLayout_4->addWidget(m_cbIntegralVolume, 1, 0, 1, 2);


        gridLayout->addWidget(groupBox_2, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame, 1, 0, 1, 1);

        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_3 = new QGroupBox(frame_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        m_sbNoiseLevel = new QDoubleSpinBox(groupBox_3);
        m_sbNoiseLevel->setObjectName(QString::fromUtf8("m_sbNoiseLevel"));
        m_sbNoiseLevel->setMaximum(200);
        m_sbNoiseLevel->setValue(25);

        gridLayout_5->addWidget(m_sbNoiseLevel, 8, 2, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_5->addWidget(label_5, 3, 1, 1, 1);

        m_minNorthY = new QDoubleSpinBox(groupBox_3);
        m_minNorthY->setObjectName(QString::fromUtf8("m_minNorthY"));

        gridLayout_5->addWidget(m_minNorthY, 4, 2, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_5->addWidget(label_3, 4, 0, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_5->addWidget(label_2, 2, 0, 1, 3);

        m_limit = new QDoubleSpinBox(groupBox_3);
        m_limit->setObjectName(QString::fromUtf8("m_limit"));
        m_limit->setDecimals(2);
        m_limit->setMinimum(-100);
        m_limit->setMaximum(100);
        m_limit->setSingleStep(1);
        m_limit->setValue(-99);

        gridLayout_5->addWidget(m_limit, 10, 2, 1, 1);

        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_5->addWidget(label, 7, 0, 1, 2);

        m_dsVoxelLength = new QDoubleSpinBox(groupBox_3);
        m_dsVoxelLength->setObjectName(QString::fromUtf8("m_dsVoxelLength"));
        m_dsVoxelLength->setMinimum(0.2);
        m_dsVoxelLength->setSingleStep(0.5);
        m_dsVoxelLength->setValue(11.2);

        gridLayout_5->addWidget(m_dsVoxelLength, 7, 2, 1, 1);

        m_maxNorthY = new QDoubleSpinBox(groupBox_3);
        m_maxNorthY->setObjectName(QString::fromUtf8("m_maxNorthY"));

        gridLayout_5->addWidget(m_maxNorthY, 4, 1, 1, 1);

        m_pbLoadLAS = new QPushButton(groupBox_3);
        m_pbLoadLAS->setObjectName(QString::fromUtf8("m_pbLoadLAS"));

        gridLayout_5->addWidget(m_pbLoadLAS, 0, 0, 1, 3);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_5->addWidget(label_6, 3, 2, 1, 1);

        m_tHist = new QPlainTextEdit(groupBox_3);
        m_tHist->setObjectName(QString::fromUtf8("m_tHist"));
        m_tHist->setMaximumSize(QSize(167, 30));

        gridLayout_5->addWidget(m_tHist, 11, 1, 1, 2);

        m_pbCreate = new QPushButton(groupBox_3);
        m_pbCreate->setObjectName(QString::fromUtf8("m_pbCreate"));

        gridLayout_5->addWidget(m_pbCreate, 9, 0, 1, 3);

        m_minEastX = new QDoubleSpinBox(groupBox_3);
        m_minEastX->setObjectName(QString::fromUtf8("m_minEastX"));

        gridLayout_5->addWidget(m_minEastX, 5, 2, 1, 1);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_5->addWidget(label_9, 8, 0, 1, 2);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_5->addWidget(label_8, 10, 0, 1, 2);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 5, 0, 1, 1);

        m_maxEastX = new QDoubleSpinBox(groupBox_3);
        m_maxEastX->setObjectName(QString::fromUtf8("m_maxEastX"));

        gridLayout_5->addWidget(m_maxEastX, 5, 1, 1, 1);

        m_pbSaveHist = new QPushButton(groupBox_3);
        m_pbSaveHist->setObjectName(QString::fromUtf8("m_pbSaveHist"));

        gridLayout_5->addWidget(m_pbSaveHist, 11, 0, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_5->addWidget(label_13, 1, 0, 1, 1);

        m_cbDataType = new QComboBox(groupBox_3);
        m_cbDataType->setObjectName(QString::fromUtf8("m_cbDataType"));

        gridLayout_5->addWidget(m_cbDataType, 1, 1, 1, 2);

        m_pbLoadLAS->raise();
        label_2->raise();
        label_6->raise();
        label_5->raise();
        label_3->raise();
        label_4->raise();
        m_maxNorthY->raise();
        m_maxEastX->raise();
        m_minNorthY->raise();
        m_minEastX->raise();
        m_pbCreate->raise();
        label_8->raise();
        m_limit->raise();
        m_sbNoiseLevel->raise();
        label_9->raise();
        m_tHist->raise();
        m_pbSaveHist->raise();
        label->raise();
        m_dsVoxelLength->raise();
        label_13->raise();
        m_cbDataType->raise();

        gridLayout_3->addWidget(groupBox_3, 0, 0, 1, 3);


        gridLayout_6->addWidget(frame_2, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_2, 3, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_7 = new QGridLayout(groupBox_4);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        m_sbBand1 = new QSpinBox(groupBox_4);
        m_sbBand1->setObjectName(QString::fromUtf8("m_sbBand1"));
        m_sbBand1->setMaximum(252);
        m_sbBand1->setValue(145);

        gridLayout_7->addWidget(m_sbBand1, 4, 1, 1, 1);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_7->addWidget(label_10, 4, 0, 1, 1);

        m_pbLoadHyper = new QPushButton(groupBox_4);
        m_pbLoadHyper->setObjectName(QString::fromUtf8("m_pbLoadHyper"));

        gridLayout_7->addWidget(m_pbLoadHyper, 1, 0, 1, 2);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_7->addWidget(label_11, 5, 0, 1, 1);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_7->addWidget(label_12, 6, 0, 1, 1);

        m_sbBand3 = new QSpinBox(groupBox_4);
        m_sbBand3->setObjectName(QString::fromUtf8("m_sbBand3"));
        m_sbBand3->setMaximum(252);
        m_sbBand3->setValue(38);

        gridLayout_7->addWidget(m_sbBand3, 6, 1, 1, 1);

        m_sbBand2 = new QSpinBox(groupBox_4);
        m_sbBand2->setObjectName(QString::fromUtf8("m_sbBand2"));
        m_sbBand2->setMaximum(252);
        m_sbBand2->setSingleStep(1);
        m_sbBand2->setValue(75);

        gridLayout_7->addWidget(m_sbBand2, 5, 1, 1, 1);

        m_updateBands = new QPushButton(groupBox_4);
        m_updateBands->setObjectName(QString::fromUtf8("m_updateBands"));

        gridLayout_7->addWidget(m_updateBands, 7, 0, 1, 2);

        m_cbHyperTexture = new QCheckBox(groupBox_4);
        m_cbHyperTexture->setObjectName(QString::fromUtf8("m_cbHyperTexture"));

        gridLayout_7->addWidget(m_cbHyperTexture, 8, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 9, 0, 1, 2);

        m_cbUseLevel1Data = new QCheckBox(groupBox_4);
        m_cbUseLevel1Data->setObjectName(QString::fromUtf8("m_cbUseLevel1Data"));

        gridLayout_7->addWidget(m_cbUseLevel1Data, 2, 0, 1, 2);

        m_pbLoadIGM = new QPushButton(groupBox_4);
        m_pbLoadIGM->setObjectName(QString::fromUtf8("m_pbLoadIGM"));

        gridLayout_7->addWidget(m_pbLoadIGM, 3, 0, 1, 2);


        gridLayout_2->addWidget(groupBox_4, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_9 = new QGridLayout(tab_3);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        m_cbMaps = new QComboBox(tab_3);
        m_cbMaps->setObjectName(QString::fromUtf8("m_cbMaps"));

        gridLayout_9->addWidget(m_cbMaps, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_3, 4, 0, 1, 2);

        m_pbCreateMap = new QPushButton(tab_3);
        m_pbCreateMap->setObjectName(QString::fromUtf8("m_pbCreateMap"));

        gridLayout_9->addWidget(m_pbCreateMap, 5, 0, 1, 2);

        groupBox_8 = new QGroupBox(tab_3);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_16 = new QGridLayout(groupBox_8);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        label_33 = new QLabel(groupBox_8);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_16->addWidget(label_33, 1, 0, 1, 1);

        radioButton_3 = new QRadioButton(groupBox_8);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        gridLayout_16->addWidget(radioButton_3, 0, 0, 1, 1);

        radioButton_4 = new QRadioButton(groupBox_8);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        gridLayout_16->addWidget(radioButton_4, 0, 1, 1, 1);

        label_34 = new QLabel(groupBox_8);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_16->addWidget(label_34, 3, 0, 1, 1);

        m_sbSampling = new QSpinBox(groupBox_8);
        m_sbSampling->setObjectName(QString::fromUtf8("m_sbSampling"));
        m_sbSampling->setMaximum(8);

        gridLayout_16->addWidget(m_sbSampling, 3, 1, 1, 1);

        m_sbThreshold = new QSpinBox(groupBox_8);
        m_sbThreshold->setObjectName(QString::fromUtf8("m_sbThreshold"));
        m_sbThreshold->setMaximum(252);
        m_sbThreshold->setValue(150);

        gridLayout_16->addWidget(m_sbThreshold, 1, 1, 1, 1);


        gridLayout_9->addWidget(groupBox_8, 2, 0, 1, 2);

        m_sbBand = new QSpinBox(tab_3);
        m_sbBand->setObjectName(QString::fromUtf8("m_sbBand"));
        m_sbBand->setMaximum(252);
        m_sbBand->setValue(145);

        gridLayout_9->addWidget(m_sbBand, 1, 1, 1, 1);

        label_31 = new QLabel(tab_3);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_9->addWidget(label_31, 0, 0, 1, 1);

        label_32 = new QLabel(tab_3);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_9->addWidget(label_32, 1, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_11 = new QGridLayout(tab_4);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_21 = new QLabel(tab_4);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_11->addWidget(label_21, 5, 2, 1, 1);

        label_16 = new QLabel(tab_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_11->addWidget(label_16, 3, 2, 1, 1);

        frame_7 = new QFrame(tab_4);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);

        gridLayout_11->addWidget(frame_7, 9, 1, 1, 1);

        m_lbCamPositionX = new QLabel(tab_4);
        m_lbCamPositionX->setObjectName(QString::fromUtf8("m_lbCamPositionX"));
        m_lbCamPositionX->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamPositionX, 4, 1, 1, 1);

        label_19 = new QLabel(tab_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_11->addWidget(label_19, 5, 1, 1, 1);

        label_17 = new QLabel(tab_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_11->addWidget(label_17, 3, 3, 1, 1);

        label_23 = new QLabel(tab_4);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_11->addWidget(label_23, 7, 1, 1, 1);

        label_24 = new QLabel(tab_4);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_11->addWidget(label_24, 7, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_4, 15, 0, 1, 4);

        label_20 = new QLabel(tab_4);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_11->addWidget(label_20, 6, 0, 1, 1);

        m_lbCamPositionZ = new QLabel(tab_4);
        m_lbCamPositionZ->setObjectName(QString::fromUtf8("m_lbCamPositionZ"));
        m_lbCamPositionZ->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamPositionZ, 4, 3, 1, 1);

        label_26 = new QLabel(tab_4);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_11->addWidget(label_26, 12, 0, 1, 2);

        label_18 = new QLabel(tab_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_11->addWidget(label_18, 11, 0, 1, 2);

        m_sbRotDeg = new QSpinBox(tab_4);
        m_sbRotDeg->setObjectName(QString::fromUtf8("m_sbRotDeg"));
        m_sbRotDeg->setMinimum(1);
        m_sbRotDeg->setMaximum(60);
        m_sbRotDeg->setSingleStep(3);
        m_sbRotDeg->setValue(5);

        gridLayout_11->addWidget(m_sbRotDeg, 12, 2, 1, 2);

        label_28 = new QLabel(tab_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_11->addWidget(label_28, 2, 0, 1, 1);

        m_lbCamPanRight = new QLabel(tab_4);
        m_lbCamPanRight->setObjectName(QString::fromUtf8("m_lbCamPanRight"));
        m_lbCamPanRight->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamPanRight, 8, 1, 1, 1);

        m_lbCamPositionY = new QLabel(tab_4);
        m_lbCamPositionY->setObjectName(QString::fromUtf8("m_lbCamPositionY"));
        m_lbCamPositionY->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamPositionY, 4, 2, 1, 1);

        label_14 = new QLabel(tab_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_11->addWidget(label_14, 4, 0, 1, 1);

        label_30 = new QLabel(tab_4);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_11->addWidget(label_30, 13, 0, 1, 1);

        label_15 = new QLabel(tab_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_11->addWidget(label_15, 3, 1, 1, 1);

        label_22 = new QLabel(tab_4);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_11->addWidget(label_22, 8, 0, 1, 1);

        label_27 = new QLabel(tab_4);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_11->addWidget(label_27, 10, 0, 1, 1);

        m_sbMoveStep = new QDoubleSpinBox(tab_4);
        m_sbMoveStep->setObjectName(QString::fromUtf8("m_sbMoveStep"));
        m_sbMoveStep->setMinimum(0.01);
        m_sbMoveStep->setSingleStep(0.5);
        m_sbMoveStep->setValue(0.05);

        gridLayout_11->addWidget(m_sbMoveStep, 11, 2, 1, 2);

        label_29 = new QLabel(tab_4);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_11->addWidget(label_29, 0, 0, 1, 1);

        frame_5 = new QFrame(tab_4);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_13 = new QGridLayout(frame_5);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        groupBox_5 = new QGroupBox(frame_5);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_12 = new QGridLayout(groupBox_5);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label_25 = new QLabel(groupBox_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setFrameShape(QFrame::NoFrame);
        label_25->setLineWidth(1);

        gridLayout_12->addWidget(label_25, 0, 0, 1, 1);


        gridLayout_13->addWidget(groupBox_5, 0, 0, 1, 1);


        gridLayout_11->addWidget(frame_5, 1, 0, 1, 4);

        m_lbCamPanLeft = new QLabel(tab_4);
        m_lbCamPanLeft->setObjectName(QString::fromUtf8("m_lbCamPanLeft"));
        m_lbCamPanLeft->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamPanLeft, 8, 2, 1, 1);

        m_lbCamTiltDown = new QLabel(tab_4);
        m_lbCamTiltDown->setObjectName(QString::fromUtf8("m_lbCamTiltDown"));
        m_lbCamTiltDown->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamTiltDown, 6, 2, 1, 1);

        m_lbCamTiltUp = new QLabel(tab_4);
        m_lbCamTiltUp->setObjectName(QString::fromUtf8("m_lbCamTiltUp"));
        m_lbCamTiltUp->setFrameShape(QFrame::StyledPanel);

        gridLayout_11->addWidget(m_lbCamTiltUp, 6, 1, 1, 1);

        pushButton = new QPushButton(tab_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_11->addWidget(pushButton, 14, 0, 1, 4);

        tabWidget->addTab(tab_4, QString());

        gridLayout_10->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_8->addWidget(frame_3, 0, 4, 1, 1);


        s_mainWindowGridLayout->addWidget(groupBox, 0, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 29));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "LAS 1.3 Viewer", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        m_pbClose->setText(QApplication::translate("MainWindow", "Close App", 0, QApplication::UnicodeUTF8));
        m_lbObjectType->setText(QApplication::translate("MainWindow", "Object Status: Object has not been created yet.", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Marching Cubes", 0, QApplication::UnicodeUTF8));
        m_pbCalculate->setText(QApplication::translate("MainWindow", "Polygonise Object", 0, QApplication::UnicodeUTF8));
        m_pbExport->setText(QApplication::translate("MainWindow", "Export Polygon to .obj", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "No of Voxels in X axis", 0, QApplication::UnicodeUTF8));
        m_cbIntegralVolume->setText(QApplication::translate("MainWindow", "Use Integral Images", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "File and Object Specifications", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Max", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "North", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Limits of the area of your interest:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Length of Voxel", 0, QApplication::UnicodeUTF8));
        m_pbLoadLAS->setText(QApplication::translate("MainWindow", "Load LAS1.3 file", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Min", 0, QApplication::UnicodeUTF8));
        m_pbCreate->setText(QApplication::translate("MainWindow", "Create Implicit Object", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Noise Level", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Object Isolevel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "East", 0, QApplication::UnicodeUTF8));
        m_pbSaveHist->setText(QApplication::translate("MainWindow", "Save Hist", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Use", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "LiDAR", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "GroupBox", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Red Band", 0, QApplication::UnicodeUTF8));
        m_pbLoadHyper->setText(QApplication::translate("MainWindow", "Load Hyperspectral images", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Green Band", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Blue Band", 0, QApplication::UnicodeUTF8));
        m_updateBands->setText(QApplication::translate("MainWindow", "Update Bands", 0, QApplication::UnicodeUTF8));
        m_cbHyperTexture->setText(QApplication::translate("MainWindow", "Enable Hyperspectral texturing", 0, QApplication::UnicodeUTF8));
        m_cbUseLevel1Data->setText(QApplication::translate("MainWindow", "Use Level1 data", 0, QApplication::UnicodeUTF8));
        m_pbLoadIGM->setText(QApplication::translate("MainWindow", "Load IGM file", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Hyperspectral", 0, QApplication::UnicodeUTF8));
        m_pbCreateMap->setText(QApplication::translate("MainWindow", "Create", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MainWindow", "Threshold", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("MainWindow", "Threshold", 0, QApplication::UnicodeUTF8));
        radioButton_4->setText(QApplication::translate("MainWindow", "Sampling", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MainWindow", "Sampling Rate 2^x", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MainWindow", "Type Of Map", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MainWindow", "Band", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Maps", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "Down", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Y", 0, QApplication::UnicodeUTF8));
        m_lbCamPositionX->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MainWindow", "Up", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Z", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "Right", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Left", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Tilt", 0, QApplication::UnicodeUTF8));
        m_lbCamPositionZ->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "Rotation in degrees", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "Movement Length (m)", 0, QApplication::UnicodeUTF8));
        label_28->setText(QString());
        m_lbCamPanRight->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        m_lbCamPositionY->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Position", 0, QApplication::UnicodeUTF8));
        label_30->setText(QString());
        label_15->setText(QApplication::translate("MainWindow", "X", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Pan", 0, QApplication::UnicodeUTF8));
        label_27->setText(QString());
        label_29->setText(QString());
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Camera Manual", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "F: move forward\n"
"B: move backwards\n"
"U: tilt up\n"
"D: tilt down\n"
"L: pan left\n"
"R: pan right\n"
"O: Refresh to original position", 0, QApplication::UnicodeUTF8));
        m_lbCamPanLeft->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        m_lbCamTiltDown->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        m_lbCamTiltUp->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Reset Camera", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Camera", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
