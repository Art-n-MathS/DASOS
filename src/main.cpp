#include <iostream>
#include "binfile.h"
#include "Object.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <MCwithIntegralImages.h>
#include <math.h>
#include "MapsManager.h"
#include "Las1_3_handler.h"

#include <exception>
#include <chrono>
#include <thread>


int main (int argc, char const* argv[])
{
   // PARAMETERS
   std::string lasFileName("");
   std::string igmFileName("");
   std::string bilFileName("");
   std::string objFileName("");
   std::string fodisFileName("");
   double voxelLength = 1.8f;
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
   // ["LOWEST_RETURN"] = 6;
   // ["SPECTRAL_SIGNATURE"] = 7;
   // ["FIRST_PATCH"] = 9;
   // ["NDVI"] = 10;
   // ["LAST_PATCH"] = 11;

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
   // ["LOWEST_RETURN"] = 6;
   // 2. more parameters
   // -thres <integer from [1-255] to threshold the map
   tags ["-map"] = 9;
   // optimisation algorithm - on/off
   tags ["-opt"] = 10;
   // load default setting for testing
   tags ["-defaulth"] = 11;
   // print instructions on how to use the software
   tags ["--help"] = 12;
   // LAS file name - string
   tags["-las"] = 13;
   // load default setting for testing
   tags ["-default"] = 14;



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
               type = argv[argvIndex];
            }
            break;
        }
        case 9: // -map  <type>  ?<band> ?{ -thres <threshold> }  <output_name>  ?{ -signature <type> <signature_directory> }
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
           for (auto & c: s) c = std::toupper(c);
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
           mInfo[mapsIndex]->thres = 0;
           if(argvIndex+2<argc && std::string(argv[argvIndex])=="-thres")
           {
              argvIndex+=2;
              mInfo[mapsIndex]->thres = atoi(argv[argvIndex-1]);
           }

           mInfo[mapsIndex]->name = argv[argvIndex];

           if(argvIndex+3<argc && std::string(argv[argvIndex])=="-signature")
           {
              argvIndex+=3;
              mInfo[mapsIndex]->spectralSignatureType = argv[argvIndex-2]; // "ASTER" or "notASTER"
              mInfo[mapsIndex]->spectralSignature = argv[argvIndex-1];
           }

           // mInfo[mapsIndex]->obj , defined later
           mInfo[mapsIndex]->bilFileName = bilFileName;
           mInfo[mapsIndex]->IGMfileName = igmFileName;

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
        case 11: // -defaulth
        {
            lasFileName = "/local1/data/scratch/mmi/2010_098_NewForest/classified_fw_laser/LDR-FW-FW10_01-201009822.LAS";
            voxelLength = 1.5f;
            noiseLevel = 25.0f;
            isolevel = -99.9f;

            igmFileName = "/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/03.ReprojectedOSNG/e098221b_osgn.igm";
            bilFileName = "/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/01.masked_data/e098221b_masked.bil";
            fodisFileName = "/local1/data/scratch/mmi/2010_098_NewForest/FW10_01-098-hyperspectral-20120713/processed/Fodis/e098221b_FODIS.bil";
            bands[0] = 140;
            bands[1] = 78;
            bands[2] = 23;

            objFileName = "object";


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[0]->type = "HYPERSPECTRAL";
            mInfo[0]->name = "HYPERSPECTRAL.png";
            mInfo[0]->band = 140;
            mInfo[0]->bilFileName = bilFileName;
            mInfo[0]->IGMfileName = igmFileName;
            mInfo[0]->fodisFileName =fodisFileName;
            mInfo[0]->thres = 0;
            mInfo[0]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[0]->type = "HYPERSPECTRAL_MEAN";
            mInfo[0]->name = "HYPERSPECTRAL_MEAN.png";
            mInfo[0]->band = 140;
            mInfo[0]->bilFileName = bilFileName;
            mInfo[0]->IGMfileName = igmFileName;
            mInfo[0]->fodisFileName =fodisFileName;
            mInfo[0]->thres = 0;
            mInfo[0]->samp = 0;
            mapsIndex++;


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "THICKNESS";
            mInfo[mapsIndex]->name = "THICKNESS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "NON-EMPTY_VOXELS";
            mInfo[mapsIndex]->name = "NON-EMPTY_VOXELS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "DENSITY";
            mInfo[mapsIndex]->name = "DENSITY.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            bands[0] = 140;
            bands[1] = 78;
            bands[2] = 30;
            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "NDVI";
            mInfo[mapsIndex]->name = "NDVI.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "FIRST_PATCH";
            mInfo[mapsIndex]->name = "FIRST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LOWEST_RETURN";
            mInfo[mapsIndex]->name = "LOWEST_RETURN.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "SPECTRAL_SIGNATURE";
            mInfo[mapsIndex]->name = "mapSpectralFodisNoAster.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->spectralSignatureType = "ASTER";
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mInfo[mapsIndex]->spectralSignature = "/users/rsg/mmi/Documents/VisLAS1.3-No3/spectral_signatures/jhu.becknic.vegetation.trees.conifers.solid.conifer.spectrum.txt";
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LAST_PATCH";
            mInfo[mapsIndex]->name = "LAST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;
            break;
        }
        case 12: // "--help"
        {
           std::cout<< "Las1.3Vis User Quide:\n"
           << "---------------------\n\n"
           << "Object related options:\n"
           << "---------------------\n";

           char buff[2000];
           std::sprintf(buff,"%-30s %s", "-las <lasFileName>", "The name/directory of LAS file to be loaded and it's compulsory\n\n");
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
//           std::sprintf(buff,"%-30s %s", "-otype <dataType>", "The type of data to be inserted. The options are: \"full-waveform\", \"all_discrete\", \"discrete_n_waveforms\", \"discrete\" and they are not case sensitive. The default value is \"full-waveform\"\n\n");
//           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-opt <on/off>", "Enables/Disables the optimisation that uses Integral Images to discard empty chunks of voxels during polygonisation. By default the optimisation is enabled.\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map <type> <outputName>", "The available types are: \"NON-EMPTY_VOXELS\", \"DENSITY\", \"THICKNESS\", \"FIRST_PATCH\", \"LAST_PATCH\", \"HYPERSPECTRAL_MEAN\", \"NDVI\", \"LOWEST_RETURN\" \n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map HYPERSPECTRAL <band> <outputName>", "The hyperspectral map needs an extra parameter defining which band will be outputed\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map SPECTRAL_SIGNATURE <outputName> -signature <type> <signature_directory> ", "The spectral signature map gives the square spectral difference between the signature and each pixel. The type is either \"ASTER\" or \"USGS!\".\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map <type> -thres <threshold> <outputName>", " A threshold is optional and can be added to any type of maps\n\n");
           std::cout << buff;
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
        case 14: // -default
        {
            lasFileName = "full_waveform_tutorial_data.LAS";
            voxelLength = 1.5f;
            noiseLevel = 25.0f;
            isolevel = -99.9f;

            bands[0] = 140;
            bands[1] = 78;
            bands[2] = 23;

            objFileName = "object";

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "THICKNESS";
            mInfo[mapsIndex]->name = "THICKNESS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "NON-EMPTY_VOXELS";
            mInfo[mapsIndex]->name = "NON-EMPTY_VOXELS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "DENSITY";
            mInfo[mapsIndex]->name = "DENSITY.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;



            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "FIRST_PATCH";
            mInfo[mapsIndex]->name = "FIRST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LOWEST_RETURN";
            mInfo[mapsIndex]->name = "LOWEST_RETURN.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LAST_PATCH";
            mInfo[mapsIndex]->name = "LAST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;
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
      std::cout << "LAS file haven't been specified\n";
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

      glData->exportToObj(objFileName,igmFileName,bilFileName,bands);
      if (glData!=NULL)
      {
         delete glData;
      }
    }

    std::cout << "mapsIndex = " << mapsIndex << "\n";
    for(unsigned int i=0; i<mapsIndex; ++i)
    {
       std::cout << mInfo[i]->type << "\n";
       mInfo[i]->obj = obj;
       m.createMap(mInfo[i]);
      delete mInfo[i];
   }

   delete obj;
   std::cout << "   ***   EXIT   ***\n";
   return 0;
}


