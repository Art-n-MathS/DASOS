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

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

int main (int argc, char const* argv[])
{ 

//    std::string filename = "lolo.txt";
//      std::ifstream mystream(filename.c_str());
//      if(!mystream)
//      {
//         std::cerr << "File \"" << filename << "\" not found.\n";
//      }
//      std::istream_iterator< std::string > it(mystream);
//      std::istream_iterator< std::string > sentinel;

//      std::vector < std::string > words(it,sentinel);

//    for(unsigned int i=0; i< words.size();++i)
//    {
//        if(words[i]!="-100")
//         std::cout << words[i] << "\n";
//    }


   // PARAMETERS
   std::string lasFileName("");
   std::string igmFileName("");
   std::string bilFileName("");
   std::string objFileName("");
   std::string fodisFileName("");
   double voxelLength = 1.8f;
   double noiseLevel = 25.0f;
   double isolevel = -100.0f;
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
   // ["HEIGHT"] = 12;
   // ["AVERAGE_HEIGHT_DIFFERENCE"] = 13;
   // INTENSITY

   MapsManager m;
   unsigned int mapsIndex=0;
   bool intergalVolume = true;

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
   tags ["-map"] = 9; /// -map  <type>  ?<band> ?{ -thres <threshold> }  <output_name>  ?{ -signature <type> <signature_directory> }
   // optimisation algorithm - on/off
   tags ["-opt"] = 10; /// -opt <on/off>;
   // load default setting for testing with hyperspectral
   tags ["-default"] = 11;
   // print instructions on how to use the software
   tags ["--help"] = 12;
   // LAS file name - string
   tags["-las"] = 13; /// -las <lasFileName>
   // load fodis file
   tags ["-fodis"] = 14;



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
                  break;
              }
           }

           mInfo[mapsIndex]->thres = 0;
           if(argvIndex+2<argc && std::string(argv[argvIndex])=="-thres")
           {
              argvIndex+=2;
              mInfo[mapsIndex]->thres = atoi(argv[argvIndex-1]);
           }

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
        case 11: // -default
        {
            // 21
            lasFileName = "data/LDR-FW-FW10_01-201009821.LAS";
            igmFileName = "data/e098211b_osgn.igm";
            bilFileName = "data/e098211b_masked.bil";
            fodisFileName = "data/e098211b_FODIS.bil";

            bands[0] = 140;
            bands[1] = 78;
            bands[2] = 23;


            objFileName = "LDR-FW-FW10_01-201009821";


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "HYPERSPECTRAL";
            mInfo[mapsIndex]->name = "21.HYPERSPECTRAL.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "HYPERSPECTRAL_MEAN";
            mInfo[mapsIndex]->name = "21.HYPERSPECTRAL_MEAN.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "THICKNESS";
            mInfo[mapsIndex]->name = "21.THICKNESS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "NON-EMPTY_VOXELS";
            mInfo[mapsIndex]->name = "16.NON-EMPTY_VOXELS.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "DENSITY";
            mInfo[mapsIndex]->name = "22.DENSITY.png";
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
            mInfo[mapsIndex]->name = "16.NDVI.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "FIRST_PATCH";
            mInfo[mapsIndex]->name = "16.FIRST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;


            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LOWEST_RETURN";
            mInfo[mapsIndex]->name = "22.LOWEST_RETURN.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

//            mInfo.push_back(new MapsManager::mapInfo);
//            mInfo[mapsIndex]->type = "SPECTRAL_SIGNATURE";
//            mInfo[mapsIndex]->name = "21.SPECTRAL_SIGNATURE.png";
//            mInfo[mapsIndex]->band = 140;
//            mInfo[mapsIndex]->bilFileName = bilFileName;
//            mInfo[mapsIndex]->IGMfileName = igmFileName;
//            mInfo[mapsIndex]->fodisFileName =fodisFileName;
//            mInfo[mapsIndex]->spectralSignatureType = "ASTER";
//            mInfo[mapsIndex]->thres = 0;
//            mInfo[mapsIndex]->samp = 0;
//            mInfo[mapsIndex]->spectralSignature = "/users/rsg/mmi/Documents/VisLAS1.3-No3/spectral_signatures/jhu.becknic.vegetation.trees.conifers.solid.conifer.spectrum.txt";
//            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "LAST_PATCH";
            mInfo[mapsIndex]->name = "16.LAST_PATCH.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "HEIGHT";
            mInfo[mapsIndex]->name = "21.HEIGHT.png";
            mInfo[mapsIndex]->band = 140;
            mInfo[mapsIndex]->bilFileName = bilFileName;
            mInfo[mapsIndex]->IGMfileName = igmFileName;
            mInfo[mapsIndex]->fodisFileName =fodisFileName;
            mInfo[mapsIndex]->thres = 0;
            mInfo[mapsIndex]->samp = 0;
            mapsIndex++;

            mInfo.push_back(new MapsManager::mapInfo);
            mInfo[mapsIndex]->type = "AVERAGE_HEIGHT_DIFFERENCE";
            mInfo[mapsIndex]->name = "22.AVERAGE_HEIGHT_DIFFERENCE.png";
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
           std::cout<< "DASOS User Quide:\n"
           << "---------------------\n\n";

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
           std::sprintf(buff,"%-30s %s", "-map <type> <outputName>", "The available types are: \"AVERAGE_HEIGHT_DIFFERENCE\", \"NON-EMPTY_VOXELS\", \"DENSITY\", \"THICKNESS\", \"FIRST_PATCH\", \"LAST_PATCH\", \"HYPERSPECTRAL_MEAN\", \"NDVI\", \"LOWEST_RETURN\" , \"INTENSITY\" \n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map HYPERSPECTRAL <band> <outputName>", "The hyperspectral map needs an extra parameter defining which band will be outputed\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map SPECTRAL_SIGNATURE <outputName> -signature <type> <signature_directory> ", "The spectral signature map gives the square spectral difference between the signature and each pixel. The type is either \"ASTER\" or \"USGS!\".\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-map <type> -thres <threshold> <outputName>", " A threshold is optional and can be added to any type of maps. Always added before the <outputname>\n\n");
           std::cout << buff;
           std::sprintf(buff,"%-30s %s", "-default", "It reads one of the sample FW LAS files and produces a 3D polygon representation and all the related maps\n\n");
           std::cout << buff;
           std::cout << "This project is funded by the Centre for Digital Entertainment and Plymouth Marine Laboratory.\n"
                     << "The code is released under the "

                     << "The data were collected by the NERC Airborne Research and Survey\n"
                     << "Facility (ARSF). Copyright is held by the UK Natural Environment\n"
                     << "Research Council (NERC). The data are free for non-commercial use,\n"
                     << "NERC-ARSF must be acknowledged in any publications, software or other\n"
                     << "media that make use of these data\n";
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
        case 14: // -fodis <filename>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              fodisFileName = argv[argvIndex];
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
      std::cout << "LAS file haven't been specified\n";
      return EXIT_FAILURE;
   }
   Las1_3_handler lala(lasFileName);
   std::vector<double> userLimits(lala.getBoundaries());


   Object *obj = new Object(voxelLength,userLimits);
   obj->setNoiseLevel(noiseLevel);
   lala.readFileAndGetObject(obj,type);
   obj->setIsolevel(isolevel);

   if(objFileName!="")
   {
      GLData *glData = new GLData;
      if(intergalVolume)
      {
         MCwithIntegralImages mc(obj,ceil((userLimits[2]-userLimits[3])/voxelLength));
         mc.createPolygonisedObject(glData);
      }
      else
      {
         MarchingCubes mc(obj,ceil((userLimits[2]-userLimits[3])/voxelLength));
         mc.createPolygonisedObject(glData);
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
       std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
       std::cout << mInfo[i]->type << "\n";
       mInfo[i]->obj = obj;
       m.createMap(mInfo[i]);
      delete mInfo[i];
   }

   delete obj;

   std::cout << "   ***   EXIT   ***\n";
   return 0;
}


