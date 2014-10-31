#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Las1_3_handler.h"
#include "Manager.h"
#include "MapsManager.h"


#include <time.h>
#include <QFileDialog>

//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_lasFileName(""),
    m_bilFileName(""),
    m_IGMFilename("/local/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.ReprojectedOSNG/e098061b_osgn.igm"),
    m_ui(new Ui::MainWindow),
    m_gl(0),
    m_pulseManager(0),
    m_obj(0),
    m_glData(0),
    m_type(0)
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
   connect(m_ui->m_cbDataType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDataType(int)));
   connect(m_ui->m_cbHyperTexture,SIGNAL(clicked(bool)),this,SLOT(changeShaderType(bool)));
   connect(m_ui->m_pbLoadIGM,SIGNAL(clicked()),this,SLOT(loadIGM()));
   connect(m_ui->m_cbUseLevel1Data,SIGNAL(clicked(bool)),this,SLOT(hideOrRevealIGMButton(bool)));

//		connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
//		/// set the combo box index change signal
//		connect(m_ui->m_objectSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObjectMode(int)));

   QStringList maps;
   maps << "Non-Empty Voxels" << "Density" << "Thickness"  << "Hyperspectral" << "Hyperspectral Mean";
   m_ui->m_cbMaps->insertItems(0,maps);
   QStringList types;
   types << /*0*/ "Full-wavefrom"
         << /*1*/ "All Discrete"
         << /*2*/ "Discrete & Waveforms"
         << /*3*/ "Discrete";
   m_ui->m_cbDataType->insertItems(0,types);
   m_ui->m_pbLoadIGM->hide();
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
      m_bilFileName = file.toStdString();
      if(m_glData!=0 && !m_ui->m_cbUseLevel1Data->isChecked())
      {
         m_glData->createUVsBIL(m_bilFileName);
      }
      else if(m_glData!=0 && m_ui->m_cbUseLevel1Data->isChecked())
      {
         // create UVs using the igm file provided
         m_glData->createUVsIGM(m_IGMFilename);
      }
      updateHyperspectral();
   }
   else
   {
      // file does not exist
   }
}

//-----------------------------------------------------------------------------
void MainWindow::loadIGM()
{
   QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",tr("Files (*.*)"));
   if(!file.isEmpty())
   {
      m_IGMFilename = file.toStdString();
      std::cout << "IGM file name: " << m_IGMFilename << "\n";
   }
   else
   {
      // file does not exist
   }
}

//-----------------------------------------------------------------------------
void MainWindow::changeDataType(int i_type)
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
    m_gl->loadHyperspectral(m_bilFileName,bands);
    if(m_glData!=NULL)
    {
        m_gl->buildVAO(m_glData);

    }
    else
    {
       // object have not been created yet
    }
}

//-----------------------------------------------------------------------------
void MainWindow::hideOrRevealIGMButton(bool i_reveal)
{
   if(i_reveal)
   {
      m_ui->m_pbLoadIGM->show();
   }
   else
   {
      m_ui->m_pbLoadIGM->hide();
   }
}

//-----------------------------------------------------------------------------
void MainWindow::changeShaderType(bool i_type)
{
   m_gl->changeShaderMode(i_type);
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
//    QString file("/local/scratch/mmi/2010_098_NewForest/classified_fw_laser/LDR-FW-FW10_01-201009806.LAS");
//    QString file("/local1/data/scratch/mmi/2010_098_FW/classified_manually/LDR-FW10_01-201009822.LAS");
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "",tr("Files (*.*)"));

   if(!file.isEmpty())
   {
      m_lasFileName = file.toStdString();
      Las1_3_handler lala(file.toStdString());
      std::vector<float> temp(lala.getBoundaries());
      m_user_limits[4] = temp[4];
      m_user_limits[5] = temp[5];
      // update the limits North and East on GUI
      m_ui->m_maxNorthY->setMaximum(temp[0]);
      m_ui->m_maxNorthY->setMinimum(temp[1]);
      m_ui->m_maxNorthY->setValue(temp[0]);

      m_ui->m_minNorthY->setMinimum(temp[0]);
      m_ui->m_minNorthY->setMaximum(temp[1]);

      m_ui->m_maxEastX->setMaximum(temp[2]);
      m_ui->m_maxEastX->setMinimum(temp[3]);
      m_ui->m_maxEastX->setValue(temp[2]);

      m_ui->m_minEastX->setMinimum(temp[2]);
      m_ui->m_minEastX->setMaximum(temp[3]);
      updateLimits();
   }
}

//-----------------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *i_event)
{
    switch (i_event->key())
    {
    case Qt::Key_F :
    case Qt::Key_B :
        m_gl->processKeyPress(i_event,m_ui->m_sbMoveStep->value());
        break;
    case Qt::Key_R :
    case Qt::Key_L :
    case Qt::Key_U :
    case Qt::Key_D :
        m_gl->processKeyPress(i_event,m_ui->m_sbRotDeg->value());
        break;
    default : break;
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

   if(m_obj!=0)
   {
      delete m_obj;
   }

   if(m_lasFileName=="")
   {
       std::cout << "Please Load a LAS file first!\n";
       return;
   }

   Las1_3_handler lala(m_lasFileName);
   updateLimits();
   double voxelLength = m_ui->m_dsVoxelLength->value();

   std::cout << "user limits = " << m_user_limits[0] << " " << m_user_limits[1]
             << " " << m_user_limits[2] << " " << m_user_limits[3] << "\n";
   m_obj = lala.readFileAndGetObject(m_ui->m_dsVoxelLength->value(),m_user_limits,m_ui->m_sbNoiseLevel->value(),0);

   m_ui->m_sbNoOfVoxelsInX->setValue(ceil((m_user_limits[2]
                                          -m_user_limits[3])/voxelLength));
   std::string labelStr("Object Status: Object created from " +
                  m_ui->m_cbDataType->currentText().toStdString() + " data");
     m_ui->m_lbObjectType->setText(labelStr.c_str());

   std::cout << "Object 2 Generated!\n";
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
      MapsManager::mapInfo *infoOfMap = new MapsManager::mapInfo;
      infoOfMap->type = mapType.toStdString();
      infoOfMap->name = file.toStdString();
      infoOfMap->obj = m_obj;
      infoOfMap->band = m_ui->m_sbBand->value();
      infoOfMap->bilFileName = m_bilFileName;
      infoOfMap->IGMfileName = m_IGMFilename;
      infoOfMap->thres = m_ui->m_sbThreshold->value();
      infoOfMap->samp = m_ui->m_sbSampling->value();
      m.createMap(infoOfMap);
      delete infoOfMap;
   }
}

//-----------------------------------------------------------------------------
void MainWindow::polygonise()
{
   if(m_obj!=0)
   {
   // if an object already exists then it should be deleted before another one
   // is created
      if(m_glData!=0)
      {
          delete m_glData;
      }
      m_glData = Manager::getPolygonisedObject(
                  m_obj,m_ui->m_sbNoOfVoxelsInX->value(),
                  m_ui->m_cbIntegralVolume->isChecked());
      if(m_bilFileName!="" && m_IGMFilename=="")
      {
         m_glData->createUVsBIL(m_bilFileName);
      }
      else if (m_bilFileName!="" && m_IGMFilename!="")
      {
         m_glData->createUVsIGM(m_IGMFilename);
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
      if(m_bilFileName!="")
      {
          std::vector<short unsigned int > bands(3);
          bands[0] = m_ui->m_sbBand1->value();
          bands[1] = m_ui->m_sbBand2->value();
          bands[2] = m_ui->m_sbBand3->value();
         //save texture as well
         m_glData->exportHyperToImage(m_bilFileName,file.toStdString(),bands);
         std::cout << "Texture saved as well\n";
      }
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
