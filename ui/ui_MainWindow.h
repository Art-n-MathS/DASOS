/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Mar 10 09:51:55 2014
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
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_8;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QFrame *frame;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QPushButton *m_pbCalculate;
    QPushButton *m_pbExport;
    QPushButton *m_pbClose;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *m_sbNoiseLevel;
    QLabel *label_5;
    QLabel *label_7;
    QDoubleSpinBox *m_minNorthY;
    QLabel *label_3;
    QLabel *label_2;
    QDoubleSpinBox *m_limit;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox;
    QDoubleSpinBox *m_maxNorthY;
    QPushButton *m_pbLoadLAS;
    QLabel *label_6;
    QPlainTextEdit *m_tHist;
    QPushButton *m_pbCreate;
    QDoubleSpinBox *m_minEastX;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_4;
    QSpinBox *m_sbNoOfVoxelsInX;
    QDoubleSpinBox *m_maxEastX;
    QPushButton *m_pbSaveHist;
    QLabel *label_13;
    QComboBox *m_cbDataType;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_7;
    QLabel *label_10;
    QPushButton *m_pbLoadHyper;
    QSpinBox *m_sbBand1;
    QLabel *label_11;
    QLabel *label_12;
    QSpinBox *m_sbBand3;
    QSpinBox *m_sbBand2;
    QPushButton *m_updateBands;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QGridLayout *gridLayout_9;
    QPushButton *m_pbCreateMap;
    QComboBox *m_cbMaps;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 886);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QString::fromUtf8("s_mainWindowGridLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_8 = new QGridLayout(groupBox);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        tabWidget = new QTabWidget(groupBox);
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
        m_pbCalculate = new QPushButton(groupBox_2);
        m_pbCalculate->setObjectName(QString::fromUtf8("m_pbCalculate"));

        gridLayout_4->addWidget(m_pbCalculate, 1, 0, 1, 2);

        m_pbExport = new QPushButton(groupBox_2);
        m_pbExport->setObjectName(QString::fromUtf8("m_pbExport"));

        gridLayout_4->addWidget(m_pbExport, 2, 0, 1, 2);


        gridLayout->addWidget(groupBox_2, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame, 1, 0, 1, 1);

        m_pbClose = new QPushButton(tab);
        m_pbClose->setObjectName(QString::fromUtf8("m_pbClose"));

        gridLayout_6->addWidget(m_pbClose, 2, 0, 1, 1);

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

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_5->addWidget(label_7, 6, 0, 1, 2);

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

        doubleSpinBox = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        gridLayout_5->addWidget(doubleSpinBox, 7, 2, 1, 1);

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

        m_sbNoOfVoxelsInX = new QSpinBox(groupBox_3);
        m_sbNoOfVoxelsInX->setObjectName(QString::fromUtf8("m_sbNoOfVoxelsInX"));
        m_sbNoOfVoxelsInX->setMinimum(2);
        m_sbNoOfVoxelsInX->setMaximum(9999999);
        m_sbNoOfVoxelsInX->setValue(20);

        gridLayout_5->addWidget(m_sbNoOfVoxelsInX, 6, 2, 1, 1);

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
        label_7->raise();
        m_sbNoOfVoxelsInX->raise();
        label_8->raise();
        m_limit->raise();
        m_sbNoiseLevel->raise();
        label_9->raise();
        m_tHist->raise();
        m_pbSaveHist->raise();
        label->raise();
        doubleSpinBox->raise();
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
        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_7->addWidget(label_10, 2, 0, 1, 1);

        m_pbLoadHyper = new QPushButton(groupBox_4);
        m_pbLoadHyper->setObjectName(QString::fromUtf8("m_pbLoadHyper"));

        gridLayout_7->addWidget(m_pbLoadHyper, 1, 0, 1, 2);

        m_sbBand1 = new QSpinBox(groupBox_4);
        m_sbBand1->setObjectName(QString::fromUtf8("m_sbBand1"));
        m_sbBand1->setMaximum(252);
        m_sbBand1->setValue(145);

        gridLayout_7->addWidget(m_sbBand1, 2, 1, 1, 1);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_7->addWidget(label_11, 3, 0, 1, 1);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_7->addWidget(label_12, 4, 0, 1, 1);

        m_sbBand3 = new QSpinBox(groupBox_4);
        m_sbBand3->setObjectName(QString::fromUtf8("m_sbBand3"));
        m_sbBand3->setMaximum(252);
        m_sbBand3->setValue(38);

        gridLayout_7->addWidget(m_sbBand3, 4, 1, 1, 1);

        m_sbBand2 = new QSpinBox(groupBox_4);
        m_sbBand2->setObjectName(QString::fromUtf8("m_sbBand2"));
        m_sbBand2->setMaximum(252);
        m_sbBand2->setSingleStep(1);
        m_sbBand2->setValue(75);

        gridLayout_7->addWidget(m_sbBand2, 3, 1, 1, 1);

        m_updateBands = new QPushButton(groupBox_4);
        m_updateBands->setObjectName(QString::fromUtf8("m_updateBands"));

        gridLayout_7->addWidget(m_updateBands, 5, 0, 1, 2);

        checkBox = new QCheckBox(groupBox_4);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_7->addWidget(checkBox, 6, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 7, 0, 1, 2);


        gridLayout_2->addWidget(groupBox_4, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_9 = new QGridLayout(tab_3);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        m_pbCreateMap = new QPushButton(tab_3);
        m_pbCreateMap->setObjectName(QString::fromUtf8("m_pbCreateMap"));

        gridLayout_9->addWidget(m_pbCreateMap, 0, 1, 1, 1);

        m_cbMaps = new QComboBox(tab_3);
        m_cbMaps->setObjectName(QString::fromUtf8("m_cbMaps"));

        gridLayout_9->addWidget(m_cbMaps, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_3, 1, 0, 1, 2);

        tabWidget->addTab(tab_3, QString());

        gridLayout_8->addWidget(tabWidget, 0, 4, 1, 1);


        s_mainWindowGridLayout->addWidget(groupBox, 0, 3, 1, 1);

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

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "LAS 1.3 Visualiser", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Marching Cubes", 0, QApplication::UnicodeUTF8));
        m_pbCalculate->setText(QApplication::translate("MainWindow", "Polygonise Object", 0, QApplication::UnicodeUTF8));
        m_pbExport->setText(QApplication::translate("MainWindow", "Export Polygon to .obj", 0, QApplication::UnicodeUTF8));
        m_pbClose->setText(QApplication::translate("MainWindow", "Close App", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Load and Specifications", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Max", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "No of Voxels in X axis", 0, QApplication::UnicodeUTF8));
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
        checkBox->setText(QApplication::translate("MainWindow", "Disable Hyperspectral texture", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Hyperspectral", 0, QApplication::UnicodeUTF8));
        m_pbCreateMap->setText(QApplication::translate("MainWindow", "Create", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Maps", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
