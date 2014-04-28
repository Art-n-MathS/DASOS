#include <QApplication>
#include "MainWindow.h"

typedef struct
{
   unsigned short int mins[3];
   unsigned short int lens[3];
   unsigned short int divisibles;
   unsigned short int nextSide;
}Cube;

int main(int argc, char **argv)
{

  QApplication a(argc, argv);

  MainWindow w;
  std::cout << "~Main Window created\n";
  // show it
  w.show();

//  Cube cube1 = {{1,2,3},{2,3,4},1,2};

//  std::cout << "---------------------------------\n";
//  std::cout << cube1.mins[0] << " " << cube1.mins[1] << " " << cube1.mins[2] << "\n";
//  std::cout << cube1.lens[0] << " " << cube1.lens[1] << " " << cube1.lens[2] << "\n";
//  std::cout << cube1.divisibles << "\n";
//  std::cout << cube1.nextSide << "\n";

//  Cube cube2 = cube1;

////  memcpy(&cube2,&cube2,sizeof(Cube));

//  std::cout << "---------------------------------\n";
//  std::cout << cube2.mins[0] << " " << cube2.mins[1] << " " << cube2.mins[2] << "\n";
//  std::cout << cube2.lens[0] << " " << cube2.lens[1] << " " << cube2.lens[2] << "\n";
//  std::cout << cube2.divisibles << "\n";
//  std::cout << cube2.nextSide << "\n";

  std::cout << "   ***   EXIT   ***\n";
  return a.exec();
}

//#include <QApplication>
//#include "Object.h"
//#include "MCwithIntegralImages.h"
//#include "Las1_3_handler.h"
//#include "PulseManager.h"
//#include "DiscreteData.h"
//#include "Manager.h"
//#include "GLData.h"
//#include "GLWindow.h"

//int main(int argc, char **argv)
//{
////   QApplication a(argc, argv);

//   Las1_3_handler lala("/local/scratch/mmi/2010_098_FW/classified_manually/LDR-FW10_01-201009822.LAS");
//   PulseManager *m_pulseManager = NULL;
//   DiscreteData *m_discreteData = NULL;
//   Object *m_obj = NULL;
//   GLData *m_glData = NULL;
//   float voxelLength = 7.2f;
//   float isolevel = 85.0f;
//   unsigned short int noOfVoxelsInX;
//   bool integral = true;
//   lala.readFileAndGetPulseManager(&m_pulseManager, &m_discreteData);
//   m_pulseManager->setNoiseLevel(25);
//   std::vector<double> m_user_limits(6);

//   m_user_limits[0] = m_pulseManager->getMaxY();
//   m_user_limits[1] = m_pulseManager->getMinY();
//   m_user_limits[2] = m_pulseManager->getMaxX();
//   m_user_limits[3] = m_pulseManager->getMinX();
//   m_user_limits[4] = m_pulseManager->getMaxZ();
//   m_user_limits[5] = m_pulseManager->getMinZ();

//   m_obj = Manager::createObject(
//               ceil((m_user_limits[2]-m_user_limits[3])/voxelLength),
//               m_user_limits,m_pulseManager,isolevel);
//   noOfVoxelsInX = m_obj->getNoVoxelsX();
//   m_glData = Manager::getPolygonisedObject(
//               m_obj,noOfVoxelsInX,integral);

//   if(m_obj!=NULL)
//   {
//      delete m_obj;
//   }
//   if(m_glData!=NULL)
//   {
//      delete m_glData;
//   }
//   if(m_pulseManager!=NULL)
//   {
//      delete m_pulseManager;
//   }
//   if(m_discreteData!=NULL)
//   {
//      delete m_discreteData;
//   }
////   std::cout << "No Of Vertices : " << m_glData->m_vertices.size() << "\n";
//   std::cout << "LALALLALLAL\n";
//   return EXIT_SUCCESS;
////   return a.exec();
//}
