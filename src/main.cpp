#include <iostream>
#include "binfile.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "MapsManager.h"
#include "Las1_3_handler.h"
#include "MarchingCubes.h"
#include <exception>
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>


#include "VolumeFactory.h"

#include "PW_handler.h"
#include <unordered_map>

#include "binfile.h"
#include  "commonfunctions.h"
#include "DtmBilReader.h"


int main (int argc, char const* argv[])
{
   // PARAMETERS
   std::vector<std::string> lasFiles;
   std::string igmFileName("");
   std::string bilFileName("");
   std::string objFileName("");
   std::string fodisFileName("");
   std::string fieldplotFileName("");
   std::string templateOutputFileName("");
   std::string templateType("");
   gmtl::Vec3i templateSize;
   std::string templatesImagePlot("");
   std::string exportVolumeFileName("");
   bool volumeCompression = false;
   std::string volumeFileName("");
   std::string dtmFileName("");
   std::vector<std::string> pwFiles;
   std::string volumeType("1D_ARRAY");
   double voxelLength = 4.2f;
   double noiseLevel = 25.0f;
   double isolevel = 0.1f;
   std::string mapsAll("");
   // TYPES:
   // 0. Full-waveform
   // 1. All_Discrete
   // 2. Discrete_n_Waveform
   // 3. Discrete (associated with waveform only
   std::string fileType("full-waveform");
//   type = "AGC";

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
   // ["LOWEST_RETURN"] = 6;
   // ["SPECTRAL_SIGNATURE"] = 7;
   // ["FIRST_PATCH"] = 9;
   // ["NDVI"] = 10;
   // ["LAST_PATCH"] = 11;
   // ["HEIGHT"] = 12;
   // ["AVERAGE_HEIGHT_DIFFERENCE"] = 13;

   MapsManager m;
   unsigned int mapsIndex=0;
   bool intergalVolume = false;

   std::vector<double> userLimits(6, 0.0f);

   // PARSING
   // pair arguments to numbers to ease search
   std::map<std::string, int> tags;

   // igm file name of hyperspectral imagery - string
   tags["-igm"] = 1; /// -igm <filename>
   // bil file name of hyperspectral imagery - string
   tags["-bil"] = 2; // <filename>
   // export polygon mesh to an obj file - string
   tags["-obj"] = 3; /// -obj <filename>
   // the bands of the hyperspectral imagery that will be used as RGB values - int int int
   tags["-rgb"] = 4; /// -rgb <band1> <band2> <band3>
   // the voxel length - double
   tags["-vl" ] = 5; /// -vl <voxel length>
   // the noise level - double
   tags["-nl" ] = 6; /// -nl <noise level>
   // isolevel
   tags ["-iso"] = 7;
   // type of the object, what to import in the polygon mesh
   // - 0. full waveform
   // - 1. All Discrete
   // - 2. Discrete and Waveform
   // - 3. Discrete (only associated with the waveform)
   // - 4. AGC value
   tags ["-otype"] = 8;
   // generate a map - 1.type of map - 2. possible more parameters - 3. name of file to be exported
   //  1. types of maps:
   // ["NON-EMPTY VOXELS"] = 1;
   // ["DENSITY"]   = 2;
   // ["THICKNESS"]    = 3;
   // ["HYPERSPECTRAL"] = 4; - follwed by band number
   // ["HYPERSPECTRAL MEAN"] = 5;
   // ["LOWEST_RETURN"] = 6;
   // 2. more parameters
   // -thres <integer from [1-255] to threshold the map
   tags ["-map"] = 9; /// -map  <type>  ?<band> ?{ -thres <threshold> }  <output_name>  ?{ -signature <type> <signature_directory> }
   // optimisation algorithm - on/off
   tags ["-opt"] = 10; /// -opt <on/off>;

   // print instructions on how to use the software
   tags ["--help"] = 12;
   // LAS file name - string
   tags["-las"] = 13; /// -las <lasFileName>
   // load fodis file
   tags ["-fodis"] = 14;
   // fieldplot
   tags ["-fieldplot"] = 15;
   // export volume? on or off - by default its off
   // 'c' indicate compression and its optional. Not every data structure has this functionality
   tags ["-exportVolume"] = 16; /// -export_volume <'c'> <exportedVolumeFileName>;
   // imports a volume instead of a las file
   tags ["-volume"] = 17; /// -volume <volumeFileName>
   // imports a pulsewave file
   tags ["-pw"] = 18; /// -pw <pwFileName>
   // define the limits of the area of interest [0:MaxNorthY, 1:MinNorthY, 2:MaxEastX, 3:MinEastX, 4:MaxHeightZ, 5:MinHeightZ]
   tags ["-userLimits"] = 19; /// -userLimits <MaxNorthY> <MinNorthY> <MaxEastX> <MinEastX>
   // creates templates , type = svm, rd or nn
   tags ["-templates"] = 20; /// -templates <type> <sizeX> <sizeY> <sizeZ> <input_fieldPlot_image> <output_templatesName>
   // choose type of structure, types = 1D_ARRAY HASHED_1D_ARRAY OCTREE  INTEGRAL_VOLUMES INTEGRAL_TREE  HASHED_OCTREE SERIES_OF_HASHED_OCTREES
   tags["-stype"] = 21; /// -stype <structure_type>
   // used to make surface flat
   tags["-dtm"]= 22; /// -dtm <dtm_fileName>



   try
   {
     int argvIndex = 1;
     while(argvIndex<argc)
     {
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
               fileType = argv[argvIndex];
            }
            break;
        }
        case 9: // -map  <type>  ?<band> ?{ -thres <threshold> } ?<fieldplot_name> <output_name>  ?{ -signature <type> <signature_directory> }
        {
           argvIndex+=2;
           if(argvIndex>=argc)
           {
              std::cout << "WARNING: map does not have enough parameters\n";
              break;
           }


           // add all metrics to mInfo later, once all parameters are defined

           std::string s(argv[argvIndex-1]);
           for (auto & c: s) c = std::toupper(c);
           if(s=="ALL_FW")
           {
              mapsAll= std::string(argv[argvIndex]) + "_";
              break;
           }

           mInfo.push_back(new MapsManager::mapInfo);
           mInfo[mapsIndex]->type = s;
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
                  break;
              }
           }



           mInfo[mapsIndex]->thres = 0;
           if(argvIndex+2<argc && std::string(argv[argvIndex])=="-thres")
           {
              argvIndex+=2;
              mInfo[mapsIndex]->thres = atoi(argv[argvIndex-1]);
           }

           if(s=="FIELDPLOT" && argvIndex+1<argc)
           {
              mInfo[mapsIndex]->fieldplot = std::string(argv[argvIndex]);
              argvIndex++;
           }

           // <output_name>
           mInfo[mapsIndex]->name = argv[argvIndex];

           if(s=="SPECTRAL_SIGNATURE")
           {
              if(argvIndex+3<argc && std::string(argv[argvIndex+1])=="-signature")
              {
                 argvIndex+=3;
                 mInfo[mapsIndex]->spectralSignatureType = argv[argvIndex-2]; // "ASTER" or "notASTER"
                 mInfo[mapsIndex]->spectralSignature = argv[argvIndex-1];
                 break;
              }
           }
           // mInfo[mapsIndex]->obj , defined later
           mInfo[mapsIndex]->bilFileName = bilFileName;
           mInfo[mapsIndex]->IGMfileName = igmFileName;
           mInfo[mapsIndex]->fodisFileName =fodisFileName;
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
        case 12: // "--help"
        {
           std::cout<< "DASOS User Quide:\n"
           << "---------------------\n\n";

           char buff[2000];
           std::sprintf(buff,"%-30s %s", "-las <lasFileName>", "The name/directory of LAS file to be loaded and it's compulsory (expection apply when a volume or a pulsewave file is loaded)\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-pw <pulsewavesFileName>", "loads an a pulsewave file instead of a LAS or an exported volume\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-igm <igmFileName>", "The name/directory of the igm file that defines the geolocaiton of the hyperspectral pixels.\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-bil <bilFileName>", "The name/directory of the bil file that contains the hyperspectral cube.\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-obj <objFileName>", "The name of the .obj file where the polygon representation of the LiDAR file will be exported to. It's optional and when not defined the data are not polygonised. Further it exports a texture when the hyperspectral data are also defined\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-rgb <band1> <band2> <band3>", "Defines the 3 bands of the hyperpectral images that will be used for texturing the polygon mesh. If not defined the default values 140, 78 and 23 are used.\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-vl <voxelLength>", "The length of the voxels in meters. Default value is 3.2m\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-nl <noiseLevel>", "The threshold that separates noise from the actual data in the waveforms.Default value is 25\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-iso <isolevel>", "The isolevel defines boundaries of the implicit object. The voxel values lies inside the range [-100,100] and everything greater than the isolevel is considered to be inside the object. Default value is -99.9\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-otype <dataType>", "The type of data to be inserted. The options are: \"full-waveform\", \"all_discrete\", \"discrete_n_waveforms\", \"discrete\" and they are not case sensitive. The default value is \"full-waveform\"\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map <type> <outputName>", "The available types are: \"NON-EMPTY_VOXELS\", \"DENSITY\", \"THICKNESS\", \"FIRST_PATCH\", \"AVERAGE_HEIGHT_DIFFERENCE\", \"LAST_PATCH\", \"HYPERSPECTRAL_MEAN\", \"NDVI\", \"LOWEST_RETURN\" , \"FIELDPLOT\", \"ALL_FW\". The ALL_FW option generates one metrics for each available full-waveform LiDAR related metrics and their names are the given outputName+metricsType+.png \n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map HYPERSPECTRAL <band> <outputName>", "The hyperspectral map needs an extra parameter defining which band will be outputed\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map SPECTRAL_SIGNATURE <outputName> -signature <type> <signature_directory> ", "The spectral signature map gives the square spectral difference between the signature and each pixel. The type is either \"ASTER\" or \"USGS!\".\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map <type> -thres <threshold> <outputName>", " A threshold is optional and can be added to any type of maps. Always added before the <outputname>\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-default", "It reads one of the sample FW LAS files and produces a 3D polygon representation and all the related maps\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-userLimits <MaxNorthY> <MinNorthY> <MaxEastX> <MinEastX>", "User define Limits of the area of interest\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-exportVolume <volumeFileName>", "exports the volume into a txt file to speed up future interpolation of the data\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-volume <volumeFileName>", "loads an exported volume instead of reading a LAS or pulsewave file\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-templates <type> <sizeX> <sizeY> <sizeZ> <input_fieldPlot_image> <output_templatesName>", "takes as input an image that indicates the positions of the tree (or features of our interest) with red colour (255,0,0) and exports a list of templates accoridng to the given type (svm, nn or rd) and template size\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-stype <structure_type>", "defines the type of structure that the volume will be saved into. The options are: , \"1D_ARRAY\", \"HASHED_1D_ARRAY\", \"OCTREE\"  , \"INTEGRAL_VOLUMES\" , \"INTEGRAL_TREE\" , \"HASHED_OCTREE\" , \"SERIES_OF_HASHED_OCTREES\" \n\n");
           std::cout << buff;
           // -templates <type> <sizeX> <sizeY> <sizeZ> <input_fieldPlot_image> <output_templatesName>
           std::sprintf(buff,"%-30s %s", "-dtm <dtmFileName>", "used a dtm file to make the surface flat. dtm file must be a .bil file\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-userLimits <maxNorthY> <minNorthY> <maxEastX> <minEastX>", "User define Limits of the area of interest\n\n\n\n");
           std::cout << buff;

           // choose type of structure, types = 1D_ARRAY OCTREE  INTEGRAL_VOLUMES IV_TREE HASHED_OCTREE SERIES_OF_HASHED_OCTREES
           tags["-stype"] = 21; /// -stype <structure_type>

           std::cout << "This project is funded by the Centre for Digital Entertainment and Plymouth Marine Laboratory.\n"
                     << "The code is released under the GPL_v1.3 licence and "
                     << "the following paper should be cited in any publication: \n"
                     << "Miltiadou, M., et al. \"ALIGNMENT OF HYPERSPECTRAL IMAGERY AND FULL-WAVEFORM LIDAR DATA FOR VISUALISATION AND CLASSIFICATION PURPOSES.\" International Archives of the Photogrammetry, Remote Sensing & Spatial Information Sciences (2015).\n";

//                     << "The sample data were collected by the NERC Airborne Research and Survey\n"
//                     << "Facility (ARSF). Copyright is held by the UK Natural Environment\n"
//                     << "Research Council (NERC). The data are free for non-commercial use,\n"
//                     << "NERC-ARSF must be acknowledged in any publications, software or other\n"
//                     << "media that make use of these data\n";
           return 0;
           break;
        }
        case 13: // -las
        {        
           argvIndex++;
           while (argvIndex<argc)
           {
              std::cout << argv[argvIndex] << "\n";
              if(argv[argvIndex][0]!='-')
              {
                 lasFiles.push_back(std::string(argv[argvIndex]));
                 argvIndex++;
              }
              else
              {
                 argvIndex--;
                 break;
              }
           }
           break;
        }
        case 14: // -fodis <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              fodisFileName = argv[argvIndex];
           }
           break;
        }
        case 15: // -fieldplot <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              fieldplotFileName = argv[argvIndex];
           }
           break;
        }
        case 16: // -export_volume <'c'> <exportedVolumeFileName>;
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              if(std::string(argv[argvIndex])!="c")
              {
                 exportVolumeFileName = argv[argvIndex];
              }
              else // compression is enable if available
              {
                  argvIndex++;
                  volumeCompression=true;
                  if(argvIndex<argc)
                  {
                     exportVolumeFileName=argv[argvIndex];
                  }
              }
           }
           break;
        }
        case 17: // -volume <volumeFileName>
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              volumeFileName = argv[argvIndex];
           }
           break;
        }
        case 18: // -pw <pulsewaveFileName>
        {
           argvIndex++;
           while (argvIndex<argc)
           {
              std::cout << argv[argvIndex] << "\n";
              if(argv[argvIndex][0]!='-')
              {
                 pwFiles.push_back(std::string(argv[argvIndex]));
                 argvIndex++;
              }
              else
              {
                 argvIndex--;
                 break;
              }
           }
           break;
        }
        case 19: // -userLimits <MaxNorthY> <MinNorthY> <MaxEastX> <MinEastX>
        {
           argvIndex+=4;
           if (argvIndex<argc)
           {
              userLimits[3] = atof(argv[argvIndex  ]);
              userLimits[2] = atof(argv[argvIndex-1]);
              userLimits[1] = atof(argv[argvIndex-2]);
              userLimits[0] = atof(argv[argvIndex-3]);
              std::cout << "user Limits: " << userLimits[0] << " " << userLimits[1] << " " << userLimits[2] << " " << userLimits[3] << "\n";
           }
           break;
        }
        case 20: // -templates <type> <sizeX> <sizeY> <sizeZ> <input_fieldPlot_image> <output_templatesName>
        {
           argvIndex+=6;
           if(argvIndex<argc)
           {
              templateOutputFileName = argv[argvIndex  ] ;
              templatesImagePlot      = argv[argvIndex-1] ;
              templateSize[2]   = atof(argv[argvIndex-2]);
              templateSize[1]   = atof(argv[argvIndex-3]);
              templateSize[0]   = atof(argv[argvIndex-4]);
              templateType      =      argv[argvIndex-5] ;
           }
           break;
        }
        case 21:// -stype <structure_type>
        {
            argvIndex++;
            if (argvIndex<argc)
            {
               volumeType = argv[argvIndex];
            }

            break;
        }
        case 22: // -dtm <dtm_filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              std::string dtmName(argv[argvIndex]);
              if(dtmName.substr(dtmName.find_last_of(".") + 1) == "bil")
              {
                 dtmFileName = dtmName;
              }
              else
              {
                std::cerr<<"WARNING: DTM file ignore. Not supported format.\n";
              }
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
   // if mapsAll then add all the fw maps to mapInfo
   if(mapsAll!="")
   {
      MapsManager *m;
      m = new MapsManager;
      const std::vector<std::string> &metricsNames = m->getNamesOfFWMetrics();
      unsigned int noOfMetrics=metricsNames.size();
      mInfo.resize(mapsIndex+noOfMetrics);
      for(unsigned int i=mapsIndex; i<mInfo.size();++i)
      {
          mInfo[mapsIndex+i] = new MapsManager::mapInfo;
          mInfo[mapsIndex+i]->type = metricsNames[i-mapsIndex];
          mInfo[mapsIndex+i]->name = mapsAll + metricsNames[i-mapsIndex] + ".png";
          mInfo[mapsIndex+i]->band = 140;
          mInfo[mapsIndex+i]->thres = 0;
          mInfo[mapsIndex+i]->samp = 0;
      }
      mapsIndex+=noOfMetrics;
      delete m;
   }
   Volume *vol = NULL;

   //--------------------------------------------------------------------------------------
   // read filename
   //--------------------------------------------------------------------------------------
   if(lasFiles.size()==0 && volumeFileName=="" && pwFiles.size()==0)
   {
      std::cout << "LAS, pulsewave or volume file haven't been specified\n";
      return EXIT_FAILURE;
   }
   if(lasFiles.size()!=0)
   {
      Las1_3_handler lala(lasFiles[0]);
      if(userLimits[0]<0.001 && userLimits[0]>-0.0001)
      {
         //then user haven't defined limits
         std::cout << "WARNING: Limits haven't been set, so entire file will be loaded\n";
         userLimits = lala.getBoundaries();
      }
      else
      {
         std::vector<double> temp_userLimits(lala.getBoundaries());
         userLimits[4] = temp_userLimits[4];
         userLimits[5] = temp_userLimits[5];
      }
      std::cout << "userLimits "<< userLimits[0] << " " << userLimits[1]<<" "
                << userLimits[2]<<" " << userLimits[0]<<"\n";

      vol = VolumeFactory::produceVolume(voxelLength,userLimits,volumeType);
      vol->setNoiseLevel(noiseLevel);
      lala.readFileAndGetObject(vol,fileType,dtmFileName);
      for(unsigned int nextFile=1; nextFile<lasFiles.size(); ++nextFile)
      {
         Las1_3_handler nextLAShandler(lasFiles[nextFile]);
         nextLAShandler.readFileAndGetObject(vol,fileType,dtmFileName);
      }
      vol->normalise();
   }
   else if (volumeFileName!="")
   {
      vol = VolumeFactory::produceVolume(volumeFileName,volumeType);
      gmtl::Vec3f maxLimits = vol->getMaxLimits();
      gmtl::Vec3f minLimits = vol->getMinLimits();
      userLimits[0] = maxLimits[1];
      userLimits[1] = minLimits[1];
      userLimits[2] = maxLimits[0];
      userLimits[3] = minLimits[0];
      userLimits[4] = maxLimits[2];
      userLimits[5] = maxLimits[2];

   }
   else if (pwFiles.size()!=0)
   {
      PW_handler pw(pwFiles[0]);
      if(userLimits[0]<0.001 && userLimits[0]>-0.0001)
      {
         //then user haven't defined limits
         userLimits = pw.getBoundaries();
         std::cout << "WARNING: Limits haven't been set, so entire file will be loaded\n";
         std::cout << "userLimits " << userLimits[0] << " " << userLimits[1] << " " << userLimits[2] << " " << userLimits[3] <<" " << userLimits[4] << " " << userLimits[5] << "\n";
      }
      else
      {
         std::vector<double> temp_userLimits(pw.getBoundaries());
         userLimits[4] = temp_userLimits[4];
         userLimits[5] = temp_userLimits[5];
      }
      vol = VolumeFactory::produceVolume(voxelLength,userLimits,volumeType);
      pw.readFileAndGetObject(vol,"full-waveform",dtmFileName);
      for(unsigned int nextFile=1; nextFile<pwFiles.size(); ++nextFile)
      {
         PW_handler nextPWhandler(pwFiles[nextFile]);
         nextPWhandler.readFileAndGetObject(vol,"full-waveform",dtmFileName);
      }

      vol->normalise();
      std::cout << "Pulse WAVES read!\n";
   }

   vol->setIsolevel(isolevel);
   vol->setNoiseLevel(noiseLevel);

   if(exportVolumeFileName!="")
   {
     vol->exportToFile(exportVolumeFileName,volumeCompression);
   }


   //--------------------------------------------------------------------------------------
   // Polygonise volume
   //--------------------------------------------------------------------------------------
   if(objFileName!="")
   {
      GLData *glData = new GLData;
      MarchingCubes *mc = VolumeFactory::getMarchingCubes(
                  volumeType,intergalVolume,vol,ceil((userLimits[2]-userLimits[3])/voxelLength)
              );
      mc->createPolygonisedObject(glData);

      if(igmFileName!="")
      {
         glData->createUVsIGM(igmFileName);
         std::cout << "UVs created\n";
      }

      glData->exportToObj(objFileName,igmFileName,bilFileName,bands);
      if (glData!=NULL)
      {
         delete glData;
      }
      if (mc!=NULL)
      {
         delete mc;
      }
    }

   //--------------------------------------------------------------------------------------
   // generate maps
   //--------------------------------------------------------------------------------------

   for(unsigned int i=0; i<mapsIndex; ++i)
   {
      mInfo[i]->obj = vol;
      m.createMap(mInfo[i]);
     delete mInfo[i];
   }


   if(vol!=NULL)
   {
      delete vol;
   }


   std::cout << "   ***   EXIT   ***\n";
   return 0;
}
