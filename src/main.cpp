//#include <QApplication>
//#include "MainWindow.h"

//typedef struct
//{
//   unsigned short int mins[3];
//   unsigned short int lens[3];
//   unsigned short int divisibles;
//   unsigned short int nextSide;
//}Cube;

//int main(int argc, char **argv)
//{
//  QApplication a(argc, argv);
//  QGLFormat glFormat = QGLFormat::defaultFormat();
//  glFormat.setSampleBuffers(true);
//  glFormat.setSamples(4);
//  QGLFormat::setDefaultFormat(glFormat);
//  MainWindow w;
//  w.show();
//  std::cout << "   ***   EXIT   ***\n";
//  return a.exec();
//}

#include <QApplication>
#include <stack>
#include "Object.h"
#include "MCwithIntegralImages.h"
#include "Las1_3_handler.h"
#include "PulseManager.h"
#include "DiscreteData.h"
#include "Manager.h"
#include "GLData.h"
#include "GLWindow.h"
#include "MapsManager.h"
#include <exception>

int main(int argc, char **argv)
{
   PulseManager *m_pulseManager = NULL;
   DiscreteData *m_discreteData = NULL;
   Object *m_obj = NULL;
   GLData *m_glData = NULL;
   float voxelLength = 7.2f;
   float isolevel = -99.0f;
   unsigned short int noOfVoxelsInX;
   bool integral = true;
   std::string lasFileName("/local1/data/scratch/mmi/2010_098_FW/classified_manually/LDR-FW10_01-201009822.LAS");
   std::string igmFileName("/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.ReprojectedOSNG/e098221b_osgn.igm");
   std::string bilFileName("/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/01.masked_data/e098221b_masked.bil");
   bool igmHyper = true;
   std::vector<short unsigned int > bands(3);
   bands[0] = 140;
   bands[1] = 78;
   bands[2] = 30;
   std::string objVsHyperTextureFileNames = "hello";
   double noiseLevel = 25.0;
   bool polygonise = true;
   bool fwData = true;
   std::stack<MapsManager::mapInfo *> mapsStack;
   MapsManager mapsManager;
   MapsManager::mapInfo *tMapInfo;


   bool defaultForQuickTesting = true;


   try
   {
       std::cout << "I hope my room was cooler!\n";
       unsigned int argvIndex = 2;
       if(argvIndex>=argc)
       {
           throw "I love Conor";
       }
       std::cout << argv[argvIndex] << "\n";
   }
   catch (char const* e)
   {
       std::cout << e  << std::endl;
       std::cout << "Type Las1.3Vis --help and follow the instructions\n";
       return EXIT_FAILURE;
   }


   Las1_3_handler lala(lasFileName);
   lala.readFileAndGetPulseManager(&m_pulseManager, &m_discreteData);
   m_pulseManager->setNoiseLevel(noiseLevel);
   std::vector<double> m_user_limits(6);

   m_user_limits[0] = m_pulseManager->getMaxY();
   m_user_limits[1] = m_pulseManager->getMinY();
   m_user_limits[2] = m_pulseManager->getMaxX();
   m_user_limits[3] = m_pulseManager->getMinX();
   m_user_limits[4] = m_pulseManager->getMaxZ();
   m_user_limits[5] = m_pulseManager->getMinZ();

   // create density volume
   if(fwData)
   {
      m_obj = Manager::createObject(
                    ceil((m_user_limits[2]-m_user_limits[3])/voxelLength),
                    m_user_limits,m_pulseManager,isolevel);
   }
   else
   {
      m_obj = Manager::createObject(
                    ceil((m_user_limits[2]-m_user_limits[3])/voxelLength),
                    m_user_limits,m_discreteData);
   }

   tMapInfo = new MapsManager::mapInfo;
   tMapInfo->name = "ThicknessOr.png";
   tMapInfo->type = "Thickness";
   tMapInfo->obj = m_obj;
   tMapInfo->band = 0;
   tMapInfo->bilFileName = "";
   tMapInfo->IGMfileName = "";
   tMapInfo->thres = 0;
   tMapInfo->samp = 0;
   mapsStack.push(tMapInfo);

//   tMapInfo = new MapsManager::mapInfo;
//   tMapInfo->name = "HyperMean.png";
//   tMapInfo->type = "HYPERSPECTRAL MEAN";
//   tMapInfo->samp = 0;
//   tMapInfo->samp = 0;
//   tMapInfo->bilFileName = bilFileName;
//   tMapInfo->IGMfileName = igmFileName;
//   tMapInfo->obj = m_obj;
//   mapsStack.push(tMapInfo);

   // polygonise
   if(polygonise)
   {
      noOfVoxelsInX = m_obj->getNoVoxelsX();
      m_glData = Manager::getPolygonisedObject(
                 m_obj,noOfVoxelsInX,integral);
      if(igmHyper)
      {
         m_glData->createUVsIGM(igmFileName);
      }
      else
      {
         m_glData->createUVsBIL(bilFileName);
      }
      // export to obj
      if(objVsHyperTextureFileNames!="")
      {
         m_glData->exportToObj(objVsHyperTextureFileNames);
         if(bilFileName!="")
         {
            m_glData->exportHyperToImage(bilFileName,objVsHyperTextureFileNames,bands);
         }
      }
      std::cout << "No Of Vertices : " << m_glData->m_vertices.size() << "\n";
   }


   // generate map
   while(!mapsStack.empty())
   {
      tMapInfo = mapsStack.top();
      mapsStack.pop();
      mapsManager.createMap(tMapInfo);
      delete tMapInfo;
   }


   // clear memory
   if(m_obj!=NULL)
   {
      delete m_obj;
   }
   if(m_glData!=NULL)
   {
      delete m_glData;
   }
   if(m_pulseManager!=NULL)
   {
      delete m_pulseManager;
   }
   if(m_discreteData!=NULL)
   {
      delete m_discreteData;
   }

   std::cout << "   ***   EXIT   ***\n";
   return EXIT_SUCCESS;
}
