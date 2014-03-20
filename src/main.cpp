#include <QApplication>
#include "MainWindow.h"

//#include <gtkmm.h>
//#include <Quadtree.h>

#include "Las1_3_handler.h"
#include "PulseManager.h"
#include "Object.h"
#include "MarchingCubes.h"
#include "HashTable.h"

//#include <binfile.h>
#include <iomanip>      // std::setprecision

#include <unordered_map>
#include <sstream>

#include <bilLib/binfile.h>

std::string Convert (float number){
    std::stringstream buff;
    buff.precision(6);
     buff<<number;
    return buff.str();
}


void getData (unsigned int **mydata)
{
    *mydata = new unsigned int[5];
    (*mydata)[0] = 1;
    (*mydata)[1] = 2;
    (*mydata)[2] = 3;
    (*mydata)[3] = 4;
    (*mydata)[4] = 5;
}



int main(int argc, char **argv)
{

  QApplication a(argc, argv);

  unsigned int *mydata;
  getData(&mydata);

  MainWindow w;
  std::cout << "~Main Window created\n";
  // show it
  w.show();



//  double* data=0;
//  try
//  {
//     //Open file
//     bilLib::BinFile file("/local/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.Reprojected/e098221b.bil");

////     bilLib::BinFile file("/users/rsg/mmi/Documents/Playing with bil files/BilLibrary/e127b013b_n_sct-0.03.igm.hdr");



//     // ----------------------------------------------------------------------------------
//     // example code on how to use the BilLibrary
//     // ----------------------------------------------------------------------------------
//     //Get dimensions of the file
//     unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
//     unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
//     unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

//     //Print out file dimensions
//     std::cout<<"Dimensions: "<<nsamps<<" "<<nlines<<" "<<nbands<<std::endl;

//     //Create data array for one line
//     data=new double[nsamps*nbands];
//     //Read in first line (line 0) of file into data array
//     file.Readline((char*)data,0);
//     //print out first 3 X values
//     std::cout<<data[0]<<" "<<data[1]<<" "<<data[2]<<std::endl;
//     //print out first 3 Y values
//     std::cout<<data[1*nsamps+0]<<" "<<data[1*nsamps+1]<<" "<<data[1*nsamps+2]<<std::endl;
//     //print out first 3 Z values
//     std::cout<<data[2*nsamps+0]<<" "<<data[2*nsamps+1]<<" "<<data[2*nsamps+2]<<std::endl;
//     //Close the file
//     file.Close();
//    //tidy up
//  }
//  catch(bilLib::BinaryReader::BRexception e)
//  {
//     std::cout<<e.what()<<std::endl;
//     std::cout<<e.info<<std::endl;
//  }

//  if(data!=0)
//  {
//     delete []data;
//  }



  // ----------------------------------------------------------------------------------
  // example on how to use the code without the GUI and Qt creator
  // ----------------------------------------------------------------------------------
//  Las1_3_handler lala("/local/scratch/mmi/2010_098_FW/LDR-FW10_01-201009822.LAS");
//  PulseManager *m_pulseManager = lala.readFileAndGetPulseManager();
//  m_pulseManager->createQuadtree(10000);

//  std::vector<double> m_user_limits;
//  m_user_limits.resize(6);
//  // load LAS file into the pulse manager
//  m_user_limits[0] = m_pulseManager->getMaxY();
//  m_user_limits[1] = m_pulseManager->getMinY();
//  m_user_limits[2] = m_pulseManager->getMaxX();
//  m_user_limits[3] = m_pulseManager->getMinX();
//  m_user_limits[4] = m_pulseManager->getMaxZ();
//  m_user_limits[5] = m_pulseManager->getMinZ();

//  Object *lasObj = m_pulseManager->createObject(20,m_user_limits);
//  lasObj->setThreshold(-99.99f);
//  m_pulseManager->setNoiseLevel(25);


//  Object *sphere = new Sphere(ngl::Vec3(0.0f,0.0f,0.0f),0.5);
//  MarchingCubes mc(sphere);
//  mc.computeGLData(40,40,40);

//  ObjGenerator objG(mc.getGLData());
//  objG.exportObjectAsObj("hello2.obj");

//  delete sphere;
//  delete m_pulseManager;
//  delete lasObj;
  std::cout << "   ***   EXIT   ***\n";

//  return 0;

  return a.exec();
}
