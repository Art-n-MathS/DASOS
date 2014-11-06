#include <QApplication>
#include "MainWindow.h"
#include <iostream>
#include <memory>
#include "Object.h"
#include "Las1_3_handler.h"
#include "GLData.h"
#include "MCwithIntegralImages.h"
#include "MapsManager.h"
 #include <stack>
 #include <exception>
 #include <chrono>
 #include <thread>
 #include <boost/format.hpp>

int main(int argc, char **argv)
{
//  QApplication a(argc, argv);
//  QGLFormat glFormat = QGLFormat::defaultFormat();
//  glFormat.setSampleBuffers(true);
//  glFormat.setSamples(4);
//  QGLFormat::setDefaultFormat(glFormat);
//  MainWindow w;
//  w.show();


  // PARAMETERS
  std::string lasFileName("");
  std::string igmFileName("");
  std::string bilFileName("");
  std::string objFileName("");
  double voxelLength = 3.2f;
  double noiseLevel = 25.0f;
  double isolevel = -99.9f;
  // TYPES:
  // 0. Full-waveform
  // 1. All_Discrete
  // 2. Discrete_n_Waveform
  // 3. Discrete (associated with waveform only
  std::string type("full-waveform");

  std::vector<short unsigned int > bands(3);
  bands[0] = 140;
  bands[1] = 78;
  bands[2] = 23;
  std::vector<MapsManager::mapInfo *> mInfo;
  // ["NON-EMPTY VOXELS"] = 1;
  // ["DENSITY"]   = 2;
  // ["THICKNESS"]    = 3;
  // ["HYPERSPECTRAL"] = 4;
  // ["HYPERSPECTRAL MEAN"] = 5;
  MapsManager m;
  unsigned int mapsIndex=0;
  bool intergalVolume = true;

  // PARSING
  // pair arguments to numbers to ease search
  std::map<std::string, int> tags;

  // igm file name of hyperspectral imagery - string
  tags["-igm"] = 1;
  // bil file name of hyperspectral imagery - string
  tags["-bil"] = 2;
  // export polygon mesh to an obj file - string
  tags["-obj"] = 3;
  // the bands of the hyperspectral imagery that will be used as RGB values - int int int
  tags["-rgb"] = 4;
  // the voxel length - double
  tags["-vl" ] = 5;
  // the noise level - double
  tags["-nl" ] = 6;
  // isolevel
  tags ["-iso"] = 7;
  // type of the object, what to import in the polygon mesh
  // - 0. full waveform
  // - 1. All Discrete
  // - 2. Discrete and Waveform
  // - 3. Discrete (only associated with the waveform
  tags ["-otype"] = 8;
  // generate a map - 1.type of map - 2. possible more parameters - 3. name of file to be exported
  //  1. types of maps:
  // ["NON-EMPTY VOXELS"] = 1;
  // ["DENSITY"]   = 2;
  // ["THICKNESS"]    = 3;
  // ["HYPERSPECTRAL"] = 4; - follwed by band number
  // ["HYPERSPECTRAL MEAN"] = 5;
  // 2. more parameters
  // -thres <integer from [1-255] to threshold the map
  tags ["-map"] = 9;
  // optimisation algorithm - on/off
  tags ["-opt"] = 10;
  // load default setting for testing
  tags ["-default"] = 11;
  // print instructions on how to use the software
  tags ["--help"] = 12;
  // LAS file name - string
  tags["-las"] = 13;



   try
   {
     int argvIndex = 1;
     while(argvIndex<argc)
     {
        std::cout << argv[argvIndex] << " " << tags[argv[argvIndex]] << "\n";
        switch (tags[argv[argvIndex]])
        {
        case 1: // -igm <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              igmFileName = argv[argvIndex];
           }
           break;
        }
        case 2: // -bil <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              bilFileName = argv[argvIndex];
           }
           break;
        }
        case 3: // -obj <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              objFileName = argv[argvIndex];
           }
           break;
        }
        case 4: // -rgb <band1> <band2> <band3>
        {
           argvIndex+=3;
           if (argvIndex<argc)
           {
              bands[0] = atoi(argv[argvIndex-2]);
              bands[1] = atoi(argv[argvIndex-1]);
              bands[2] = atoi(argv[argvIndex  ]);
           }
           break;
        }
        case 5: // -vl <voxel length>
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              voxelLength = atof(argv[argvIndex]);
           }
           break;
        }
        case 6: // -nl <noise level>
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              noiseLevel = atof(argv[argvIndex]);
           }
           break;
        }
        case 7: // -iso <isolevel>
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              isolevel = atof(argv[argvIndex]);
           }
           break;
        }
        case 8: // -otype <input data type>
        {
            argvIndex++;
            if (argvIndex<argc)
            {
               type = argv[argvIndex];
            }
            break;
        }
        case 9: // -map <type> ?<band> <output_name> ?{ -thres <threshold> }
        {
           argvIndex+=2;
           if(argvIndex>=argc)
           {
              std::cout << "WARNING: map does not have enough parameters\n";
              break;
           }

           mInfo.push_back(new MapsManager::mapInfo);
           mInfo[mapsIndex]->type = argv[argvIndex-1];
           std::string s(argv[argvIndex-1]);
           std::transform(s.begin(), s.end(), s.begin(), toupper);
           if(s=="HYPERSPECTRAL")
           {
              argvIndex++;
              if(argvIndex<argc)
              {
                 mInfo[mapsIndex]->band = atoi(argv[argvIndex-1]);
              }
              else
              {
                  std::cout << "WARNING: Band haven't been defined. Set to 140 as default";
                  mInfo[mapsIndex]->band = 140;
              }
           }
           mInfo[mapsIndex]->name = argv[argvIndex];
           // mInfo[mapsIndex]->obj , defined later
           mInfo[mapsIndex]->bilFileName = bilFileName;
           mInfo[mapsIndex]->IGMfileName = igmFileName;
           mInfo[mapsIndex]->thres = 0;
           if(argvIndex+1<argc && std::string(argv[argvIndex])=="-thres")
           {
              argvIndex++;
              mInfo[mapsIndex]->thres = atoi(argv[argvIndex]);
           }

           mInfo[mapsIndex]->samp = 0;
           mapsIndex++;
           break;
        }
        case 10: // -opt <on/off>;
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              intergalVolume = (!(std::string(argv[argvIndex])=="off"));
           }
           break;
        }
        case 11: // -default
        {
           lasFileName = "/local1/data/scratch/mmi/2010_098_FW/classified_manually/LDR-FW10_01-201009822.LAS";
           igmFileName = "/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.ReprojectedOSNG/e098221b_osgn.igm";
           bilFileName = "/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/01.masked_data/e098221b_masked.bil";
           bands[0] = 140;
           bands[1] = 78;
           bands[2] = 30;
           objFileName = "hello";
           mInfo.push_back(new MapsManager::mapInfo);
           mInfo[mapsIndex]->type = "THICKNESS";
           mInfo[mapsIndex]->name = "map.png";
           // mInfo[mapsIndex]->obj , defined later
           mInfo[mapsIndex]->band = 140;
           mInfo[mapsIndex]->bilFileName = bilFileName;
           mInfo[mapsIndex]->IGMfileName = igmFileName;
           mInfo[mapsIndex]->thres = 0;
           mInfo[mapsIndex]->samp = 0;
           mapsIndex++;
           mInfo.push_back(new MapsManager::mapInfo);
           mInfo[mapsIndex]->type = "HYPERSPECTRAL MEAN";
           mInfo[mapsIndex]->name = "maphyper.png";
           // mInfo[mapsIndex]->obj , defined later
           mInfo[mapsIndex]->band = 140;
           mInfo[mapsIndex]->bilFileName = bilFileName;
           mInfo[mapsIndex]->IGMfileName = igmFileName;
           mInfo[mapsIndex]->thres = 0;
           mInfo[mapsIndex]->samp = 0;
           mapsIndex++;
           break;
        }
        case 12: // "--help"
        {
           std::cout<< "Las1.3Vis User Quide:\n"
           << "---------------------\n\n"
           << "Object related options:\n";
  //             std::cout.width(20);
           std::cout << boost::format("%-20s %-50s\n") % "-las <lasFileName>" % "name of LAS File to be loaded";
  //             std::cout << boost::format("%-20s %-50s\n") %
  //             std::cout << boost::format()
           break;
        }
        case 13: // -las
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              lasFileName = argv[argvIndex];
           }
           break;
        }
        default:
        {
           std::cout << "WARNING: Unkown tag: " << argv[argvIndex] << "\n";
           break;
        }
        }
        argvIndex++;
     }
   }
   catch (char const* e)
   {
      std::cout << e  << std::endl;
      std::cout << "Type Las1.3Vis --help and follow the instructions\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(20000));
      return EXIT_FAILURE;
   }



  //INTERPRETATION OF DATA
  if(lasFileName=="")
  {
     std::cout << "LAS file haven't been loaded\n";
     return EXIT_FAILURE;
  }
  Las1_3_handler lala(lasFileName);
  std::vector<double> userLimits(lala.getBoundaries());
  Object *obj = lala.readFileAndGetObject(voxelLength, userLimits, noiseLevel,type);
  obj->setIsolevel(isolevel);

  if(objFileName!="")
  {
     GLData *glData = NULL;
     if(intergalVolume)
     {
        MCwithIntegralImages mc(obj,ceil((userLimits[2]-userLimits[3])/voxelLength));
        glData= mc.createPolygonisedObject();
     }
     else
     {
        MarchingCubes mc(obj,ceil((userLimits[2]-userLimits[3])/voxelLength));
        glData= mc.createPolygonisedObject();
     }
     if(igmFileName!="")
     {
        glData->createUVsIGM(igmFileName);
     }

     glData->exportToObj(objFileName);
     if(bilFileName!="")
     {
        glData->exportHyperToImage(bilFileName,objFileName,bands);
     }
     if (glData!=NULL)
     {
        delete glData;
     }
  }

  std::cout << "mapsIndex = " << mapsIndex << "\n";
  for(unsigned int i=0; i<mapsIndex; ++i)
  {
     mInfo[i]->obj = obj;
     m.createMap(mInfo[i]);
     delete mInfo[i];
  }

  delete obj;
  std::cout << "   ***   EXIT   ***\n";
  return EXIT_SUCCESS;
//  return a.exec();
}

