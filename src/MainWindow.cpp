#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Las1_3_handler.h"
#include "Manager.h"
#include "MapsManager.h"

#include <bilLib/binfile.h>
#include <bilLib/commonfunctions.h>
#include <time.h>
#include <QFileDialog>


//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_gl(0),
    m_pulseManager(0),
    m_discreteData(0),
    m_obj(0),
    m_glData(0),
    m_type(false)
{
   m_ui->setupUi(this);
   m_gl=new  GLWindow(this);
   m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,1,3);
   connect(m_ui->m_pbLoadLAS,SIGNAL(clicked()),this,SLOT(loadLASfile()));
   connect(m_ui->m_pbCalculate,SIGNAL(clicked()),this,SLOT(polygonise()));
   connect(m_ui->m_pbExport,SIGNAL(clicked()),this,SLOT(exportOBJ()));
   connect(m_ui->m_pbClose,SIGNAL(clicked()),this,SLOT(close()));
   connect(m_ui->m_pbCreate,SIGNAL(clicked()),this,SLOT(createObject()));
   connect(m_ui->m_limit,SIGNAL(valueChanged(double)),this,SLOT(setThreshold(double)));
   connect(m_ui->m_pbSaveHist,SIGNAL(clicked()),this,SLOT(createHist()));
   connect(m_ui->m_pbLoadHyper,SIGNAL(clicked()),this,SLOT(loadHyperspectraldata()));
   connect(m_ui->m_updateBands,SIGNAL(clicked()),this,SLOT(updateHyperspectral()));
   connect(m_ui->m_pbCreateMap,SIGNAL(clicked()),this,SLOT(createMap()));
   connect(m_ui->m_cbDataType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeType(int)));

//		connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
//		/// set the combo box index change signal
//		connect(m_ui->m_objectSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObjectMode(int)));

   QStringList maps;
   maps << "Thickness" << "Density" << "Height";
   m_ui->m_cbMaps->insertItems(0,maps);
   QStringList types;
   types << "Full-wavefrom" << "Discrete";
   m_ui->m_cbDataType->insertItems(0,types);

   m_user_limits.resize(6);
   loadLASfile();
}


//-----------------------------------------------------------------------------
void MainWindow::loadHyperspectraldata()
{
   QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",tr("Files (*.*)"));
   if(!file.isEmpty())
   {
      m_bilFilename = file.toStdString();
      updateHyperspectral();
   }
   else
   {
      // file does not exist
   }
}

//-----------------------------------------------------------------------------
void MainWindow::changeType(int i_type)
{
    m_type = i_type;
}

//-----------------------------------------------------------------------------
void MainWindow::updateHyperspectral()
{
    std::vector<short unsigned int > bands(3);
    bands[0] = m_ui->m_sbBand1->value();
    bands[1] = m_ui->m_sbBand2->value();
    bands[2] = m_ui->m_sbBand3->value();
    m_gl->loadHyperspectral(m_bilFilename,bands);

    bilLib::BinFile file(m_bilFilename);
    std::string map_info = file.FromHeader("map info");

    unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
    unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));

    ngl::Vec2 min(atof(bilLib::GetItemFromString(map_info,3,',').c_str()),
                   atof(bilLib::GetItemFromString(map_info,4,',').c_str())-
                  nlines*atof(bilLib::GetItemFromString(map_info,6,',').c_str()));                  ;
    ngl::Vec2 max(min.m_x+nsamps*atof(bilLib::GetItemFromString(map_info,5,',').c_str()),
                  atof(bilLib::GetItemFromString(map_info,4,',').c_str()));

    m_glData->createUVs(min,max);
    m_gl->buildVAO(m_glData);
}

//-----------------------------------------------------------------------------
void MainWindow::createHist()
{
   QString file = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                "",tr("Files (*.png)"));
   if(!file.isEmpty())
   {
      Manager::createHistogram(m_obj,file.toStdString());
   }
}

//-----------------------------------------------------------------------------
void MainWindow::loadLASfile()
{
   QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "",tr("Files (*.*)"));
   if(!file.isEmpty())
   {
      clock_t t1,t2;
      t1 =clock();
//      Las1_3_handler lala("/local/scratch/mmi/2010_098_NewForest/classified_fw_laser/LDR-FW10_01-201009822.LAS");
      Las1_3_handler lala(file.toStdString());
   //   std::cout << "~"<<file.toStdString() <<"\n";
      if (m_pulseManager!=0)
      {
         delete m_pulseManager;
         m_pulseManager=0;
      }
      if(m_discreteData!=0)
      {
         delete m_discreteData;
         m_discreteData=0;
      }
      // load LAS file into the pulse manager
      lala.readFileAndGetPulseManager(&m_pulseManager, &m_discreteData);
      t2 = clock();
      float diff= ((float)t2-(float)t1) / CLOCKS_PER_SEC;
      std::cout << "Reading LAS1_3 file took " << diff << " SECONDS!!!\n";
      t1 =clock();
//      m_pulseManager->createQuadtree(10000);
      t2 =clock();
      diff= ((float)t2-(float)t1) / CLOCKS_PER_SEC;
      std::cout << "Creating Quadtree took " << diff << " SECONDS. \n";

      // update the limits North and East on GUI
      m_ui->m_maxNorthY->setMaximum(m_pulseManager->getMaxY());
      m_ui->m_maxNorthY->setMinimum(m_pulseManager->getMinY());
      m_ui->m_maxNorthY->setValue(m_pulseManager->getMaxY());

      m_ui->m_minNorthY->setMinimum(m_pulseManager->getMinY());
      m_ui->m_minNorthY->setMaximum(m_pulseManager->getMaxY());

      m_ui->m_maxEastX->setMaximum(m_pulseManager->getMaxX());
      m_ui->m_maxEastX->setMinimum(m_pulseManager->getMinX());
      m_ui->m_maxEastX->setValue(m_pulseManager->getMaxX());

      m_ui->m_minEastX->setMinimum(m_pulseManager->getMinX());
      m_ui->m_minEastX->setMaximum(m_pulseManager->getMaxX());

      m_user_limits[4] = m_pulseManager->getMaxZ();
      m_user_limits[5] = m_pulseManager->getMinZ();
      updateLimits();
   }
//   else
   {
      // no file has been loaded
   }
}

//-----------------------------------------------------------------------------
void MainWindow::close()
{
    std::cout << "Application is closing!\n";
    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
void MainWindow::createObject()
{
   if(m_pulseManager==0)
   {
      std::cout << "Please load a LAS1.3 file first\n";
      return;
   }
   m_pulseManager->setNoiseLevel(m_ui->m_sbNoiseLevel->value());
   std::cout << "Start creating Object\n";


   updateLimits();
   std::cout << "user limits = " << m_user_limits[0] << " " << m_user_limits[1]
             << " " << m_user_limits[2] << " " << m_user_limits[3] << "\n";
   if(m_obj!=0)
   {
       delete m_obj;
   }
   if(m_type)
   {
      std::cout << "discrete\n";
      m_obj = Manager::createObject(m_ui->m_sbNoOfVoxelsInX->value(),
                                    m_user_limits,m_discreteData);
   }
   else
   {
      std::cout << "full waveform\n";
      m_obj = Manager::createObject(m_ui->m_sbNoOfVoxelsInX->value(),
                   m_user_limits,m_pulseManager,
                   m_ui->m_sbNoiseLevel->value());
   }
   std::string labelStr("Object Status: Object created from " + m_ui->m_cbDataType->currentText().toStdString() + " data");
   std::cout << "Object Satus: Object created from " + m_ui->m_cbDataType->currentText().toStdString() + " data\n" ;
   std::cout << labelStr << "\n";
   m_ui->m_lbObjectType->setText(labelStr.c_str());
   std::cout << "Object created!\n";
}

//-----------------------------------------------------------------------------
void MainWindow::createMap()
{
   if(m_obj==0)
   {
      std::cout << "Please create object first\n";
      return;
   }
   QString file = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                "",tr("Files (*.png)"));
   if(!file.isEmpty())
   {
      m_obj->setIsolevel(m_ui->m_limit->value());

      MapsManager m;
      QString mapType = m_ui->m_cbMaps->currentText();
      std::cout << "++++: " << mapType.toStdString() << "\n";
      m.createMap(mapType.toStdString(),file.toStdString(),m_obj);
   }
}

//-----------------------------------------------------------------------------
void MainWindow::polygonise()
{
   if(m_pulseManager==0)
   {
      std::cout << "File have not been loaded yet\n";
      return;
   }

   if(m_obj!=0)
   {
   // if an object already exists then it should be deleted before another one
   // is created
      if(m_glData!=0)
      {
          delete m_glData;
      }

      m_glData = Manager::getPolygonisedObject(
                  m_obj,m_ui->m_sbNoOfVoxelsInX->value());
      if(m_bilFilename!="")
      {
         bilLib::BinFile file(m_bilFilename);
         std::string map_info = file.FromHeader("map info");

         unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
         unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));

         ngl::Vec2 min(atof(bilLib::GetItemFromString(map_info,3,',').c_str()),
                        atof(bilLib::GetItemFromString(map_info,4,',').c_str()));
         ngl::Vec2 max(min.m_x+nsamps*atof(bilLib::GetItemFromString(map_info,5,',').c_str()),
                       min.m_y+nlines*atof(bilLib::GetItemFromString(map_info,6,',').c_str()));
         m_glData->createUVs(min,max);
      }
      m_gl->buildVAO(m_glData);
      std::cout << "Object has been polygonised!\n";
   }
   else
   {
      std::cout << "Please create object first\n";
   }
}

//-----------------------------------------------------------------------------
void MainWindow::exportOBJ()
{
   if(m_glData==0)
   {
      // data have not been created, nothing to export
      std::cout << "Please press \"Polygonise\" fisrt\n";
   }
   else
   {
      QString file = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                       "",tr("Files (*.*)"));
      m_glData->exportToObj(file.toStdString());
   }
}

//-----------------------------------------------------------------------------
void MainWindow::setThreshold(double i_threshold)
{
   if (m_obj!=0)
   {
      m_obj->setIsolevel(i_threshold);
   }
   else
   {
      // object has not been created yet. Threshold'll be set once it's created
   }
}

//-----------------------------------------------------------------------------
void MainWindow::updateLimits()
{
   m_user_limits[0] = m_ui->m_maxNorthY->value();
   m_user_limits[1] = m_ui->m_minNorthY->value();
   m_user_limits[2] = m_ui->m_maxEastX->value();
   m_user_limits[3] = m_ui->m_minEastX->value();
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
   delete m_ui;
   if (m_pulseManager!=0)
   {
      delete m_pulseManager;
   }
   if(m_glData!=0)
   {
      delete m_glData;
   }
   if(m_obj!=0)
   {
      delete m_obj;
   }
   if(m_gl!=0)
   {
       delete m_gl;
   }
}
