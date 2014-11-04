#include <QApplication>
#include "MainWindow.h"
#include <iostream>
#include <memory>
#include "Object.h"
#include "Las1_3_handler.h"
#include "GLData.h"
#include "MCwithIntegralImages.h"
#include "MapsManager.h"

int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  QGLFormat glFormat = QGLFormat::defaultFormat();
  glFormat.setSampleBuffers(true);
  glFormat.setSamples(4);
  QGLFormat::setDefaultFormat(glFormat);
//  MainWindow w;
//  w.show();

  std::string lasFileName("/local1/data/scratch/mmi/2010_098_FW/classified_manually/LDR-FW10_01-201009822.LAS");
  Las1_3_handler lala(lasFileName);
  std::vector<double> userLimits(lala.getBoundaries());
  double voxelLength = 3.2f;
  double noiseLevel = 25.0f;
  double isolevel = -99.9f;
  // TYPES:
  // 0. Full-waveform
  // 1. All Discrete
  // 2. Discrete and Waveforms
  // 3. Discrete (associated with waveform only
  int type = 0;
  std::string objFileName("test1");
  std::string igmFileName("/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.ReprojectedOSNG/e098221b_osgn.igm");
  std::string bilFileName("/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/01.masked_data/e098221b_masked.bil");
  std::vector<short unsigned int > bands(3);
  bands[0] = 140;
  bands[1] = 78;
  bands[2] = 23;


  Object *obj = lala.readFileAndGetObject(voxelLength, userLimits, noiseLevel,type);
  obj->setIsolevel(isolevel);

  MCwithIntegralImages mc(obj,ceil((userLimits[2]-userLimits[3])/voxelLength));
  GLData *glData = mc.createPolygonisedObject();
  glData->createUVsIGM(igmFileName);

  glData->exportToObj(objFileName);
  glData->exportHyperToImage(bilFileName,objFileName,bands);

  // ["NON-EMPTY VOXELS"] = 1;
  // ["DENSITY"]   = 2;
  // ["THICKNESS"]    = 3;
  // ["HYPERSPECTRAL"] = 4;
  // ["HYPERSPECTRAL MEAN"] = 5;
  MapsManager m;
  unsigned int mapsIndex=0;
  std::vector<MapsManager::mapInfo *> mInfo;
  mInfo.push_back(new MapsManager::mapInfo);
  mInfo[mapsIndex]->type = "THICKNESS";
  mInfo[mapsIndex]->name = "map.png";
  mInfo[mapsIndex]->obj = obj;
  mInfo[mapsIndex]->band = 140;
  mInfo[mapsIndex]->bilFileName = bilFileName;
  mInfo[mapsIndex]->IGMfileName = igmFileName;
  mInfo[mapsIndex]->thres = 0;
  mInfo[mapsIndex]->samp = 0;
  mapsIndex++;

  mInfo.push_back(new MapsManager::mapInfo);
  mInfo[mapsIndex]->type = "HYPERSPECTRAL MEAN";
  mInfo[mapsIndex]->name = "maphyper.png";
  mInfo[mapsIndex]->obj = obj;
  mInfo[mapsIndex]->band = 140;
  mInfo[mapsIndex]->bilFileName = bilFileName;
  mInfo[mapsIndex]->IGMfileName = igmFileName;
  mInfo[mapsIndex]->thres = 0;
  mInfo[mapsIndex]->samp = 0;
  mapsIndex++;

  for(unsigned int i=0; i<mapsIndex; ++i)
  {
     m.createMap(mInfo[i]);
     delete mInfo[i];
  }


  delete obj;
  delete glData;
  std::cout << "   ***   EXIT   ***\n";
  return a.exec();
}

