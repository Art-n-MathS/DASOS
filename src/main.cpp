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


#include <CreateTreeTemplates.h>
#include <TreeDetectionEvaluation.h>
#include "NF_FieldPlot.h"
#include "NF_FieldPlotsManager.h"
#include "NF_FieldPlotManagerByType.h"

#include "VolumeFactory.h"

#include "PW_handler.h"
#include <unordered_map>

#include "Int_PlotsManager.h"
#include "CreateTreeTemplates.h"

#include "binfile.h"
#include  "commonfunctions.h"
#include "DtmBilReader.h"

#include <math.h>

//-vols /home/milto/Documents/RedGum/Volumes/0.6 -icsv "/home/milto/Documents/RedGum/Redgum Project/Trees_RedGum_WGS1984_z55_noNonExistingTrees_Correct.csv" -eparameters processed_basic -column isDead -class dead -ttype square 2 3 5 -ocsv templates

// -las C:\Users\milto\Documents\TEPAK\SampleData\L004-1-M01-S1-C1_s_w_2.las -totalIn C:\Users\milto\Documents\EngD\DASOS_bugFixed2019\testResults\ints2.csv

// -las "D:\RedGum\Classified_AofI\cld_150317_25_001734_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_25_001734_d.las.csv"

// -las "D:\RedGum\Classified_AofI\cld_150317_25_015416_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_25_015416_d.las.csv"

// -las "D:\RedGum\Classified_AofI\cld_150317_25_025509_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_25_025509_d.las.csv"

// -las "D:\RedGum\Classified_AofI\cld_150317_26_073325_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_26_073325_d.las.csv"

// -las "D:\RedGum\Classified_AofI\cld_150318_08_214301_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150318_08_214301_d.las.csv"

// -las "D:\RedGum\Classified_AofI\cld_150317_26_061640_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_26_061640_d.las.csv"
// -las "D:\RedGum\Classified_AofI\cld_150317_26_064524_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_26_064524_d.las.csv"
// -las "D:\RedGum\Classified_AofI\cld_150317_26_071531_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150317_26_071531_d.las.csv"
// -las "D:\RedGum\Classified_AofI\cld_150318_09_001510_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150318_09_001510_d.las.csv"
// -las "D:\RedGum\Classified_AofI\cld_150323_28_234103_d.las" -totalIn "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\Work Packages\WP4\CSVdata\cld_150323_28_234103_d.las.csv"


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
   std::string intsfileName("");
   gmtl::Vec3i templateSize;
   std::string templatesImagePlot(""), exportVolumeFileName("");
   bool volumeCompression = false;
   std::string volumeFileName("");
   std::string dtmFileName("");
   std::vector<std::string> pwFiles;
   std::string volumeType("HASHED_1D_ARRAY");
   double voxelLength = 2.5f;
   double noiseLevel = 25.0f;
   double isolevel = 1.0f;
   std::string mapsAll("");

   Int_PlotsManager fieldplotsManager; // manager of the plots instead of the previous one
   std::string csvSamplesPulses_exportingFile("");
   std::string csvImportingFieldplots(""), csvExportingTemplates("");
   std::string volsDir("");
   bool isFieldPlotManagerValid(false);

   bool calBoundaries(false);

   unsigned int noSamples(0);

   // TYPES:
   // 0. Full-waveform
   // 1. All_Discrete
   // 2. Discrete_n_Waveform
   // 3. Discrete (associated with waveform only
   std::string fileType("Full-waveform");
//   fileType = "All_Discrete";

   std::vector<short unsigned int > bands(3);
   bands[0] = 140;
   bands[1] = 78;
   bands[2] = 23;
   std::vector<MapsManager::mapInfo *> mInfo;

   MapsManager m;
   unsigned int mapsIndex=0;
   bool optPolygonisation = false;

   std::vector<double> userLimits(6, 0.0f);

   // PARSING
   // pair arguments to numbers to ease search
   std::map<std::string, int> tags;

   /// igm file name of hyperspectral imagery - string
   tags["-igm"] = 1; // -igm <filename>
   /// bil file name of hyperspectral imagery - string
   tags["-bil"] = 2; // <filename>
   /// export polygon mesh to an obj file - string
   tags["-obj"] = 3; // -obj <filename>
   /// the bands of the hyperspectral imagery that will be used as RGB values -
   ///  int int int
   tags["-rgb"] = 4; // -rgb <band1> <band2> <band3>
   /// the voxel length - double
   tags["-vl" ] = 5; // -vl <voxel length>
   /// the noise level - double
   tags["-nl" ] = 6; // -nl <noise level>
   /// isolevel
   tags ["-iso"] = 7;
   /// type of the object, what to import in the polygon mesh
   /// - 0. full_waveform
   /// - 1. All_Discrete
   /// - 2. Discrete and Waveform
   /// - 3. Discrete (only associated with the waveform)
   /// - 4. AGC value
   tags ["-otype"] = 8;
   /// generate a map - 1.type of map -
   ///                  2. possible more parameters -
   ///                  3. name of file to be exported
   ///  1. types of maps:
   /// ["NON-EMPTY VOXELS"] = 1;
   /// ["DENSITY"]   = 2;
   /// ["THICKNESS"]    = 3;
   /// ["HYPERSPECTRAL"] = 4; - follwed by band number
   /// ["HYPERSPECTRAL MEAN"] = 5;
   /// ["LOWEST_RETURN"] = 6;
   /// 2. more parameters
   /// -thres <integer from [1-255] to threshold the map
   tags ["-map"] = 9; // -map  <type>  ?<band> ?{ -thres <threshold> }
                      //       <output_name>
                      //      ?{ -signature <type> <signature_directory> }
   /// optimisation algorithm - on/off
   tags ["-opt"] = 10; // -opt <on/off>;

   /// print instructions on how to use the software
   tags ["--help"] = 12;
   /// LAS file name - string
   tags["-las"] = 13; // -las <lasFileName>
   /// load fodis file
   tags ["-fodis"] = 14;
   /// fieldplot
   tags ["-fieldplot"] = 15;
   /// export volume? on or off - by default its off
   /// 'c' indicate compression and its optional. Not every data structure has
   /// this functionality
   tags ["-exportVolume"] = 16; // -export_volume <'c'> <exportedVolumeFileName>;
   /// imports a volume instead of a las file
   tags ["-volume"] = 17; // -volume <volumeFileName>
   /// imports a pulsewave file
   tags ["-pw"] = 18; // -pw <pwFileName>
   /// define the limits of the area of interest [0:MaxNorthY, 1:MinNorthY,
   /// 2:MaxEastX, 3:MinEastX, 4:MaxHeightZ, 5:MinHeightZ]
   tags ["-userLimits"] = 19; // -userLimits <MaxNorthY> <MinNorthY>
                              //             <MaxEastX> <MinEastX>
   /// creates templates , type = svm, rd or nn
   tags ["-templates"] = 20; // -templates <type> <sizeX> <sizeY> <sizeZ>
                             //            <input_fieldPlot_image>
                             //            <output_templatesName>
   /// choose type of structure,
   /// types =
   /// * 1D_ARRAY
   /// * HASHED_1D_ARRAY
   /// * OCTREE
   /// * INTEGRAL_VOLUMES
   /// * INTEGRAL_TREE
   /// * HASHED_OCTREE
   /// * SERIES_OF_HASHED_OCTREES
   tags["-stype"] = 21; // -stype <structure_type>
   /// used to make surface flat
   tags["-dtm"]= 22; // -dtm <dtm_fileName>
   /// exports 10 samples into a .csv file
   tags["-exportPulses"]=23;  // -exportPulses  <noOfPulses> <filename.csv>
   /// imports a .csv file with fieldplots
   tags["-icsv"]=24; // -icsv <CSVfile.csv>
   /// directory with volumes related to the fieldplots that lie inside the csv
   ///  file
   tags["-vols"]=25; // // -vols <volsDir>
   /// exports csv files with the values of the 3D priors when volsDir is
   /// loaded then this should also be a directory
   tags["-ocsv"] = 26; // -exportTemplatesCSV <csvFileName>
   /// the label of the column that defines the class of each entry
   /// (e.g. <label> = isDead)
   tags["-column"] = 27;  // -column <label>
   /// the name of the class (e.g. dead or alive) or "ALL". The "ALL" option is the
   /// area of interest and generates a template for each column that lies
   /// inside a pad area.
   /// The <offset> parameter is optional. When it is defiend by a positive number
   /// then all the columns with label <className> and everything within
   /// distance less than <offset> are not included in the generation of priors
   /// and everything else is included. It's like the option "ALL" minus the
   /// circles with centres the trees of class <className> and radius equal to
   /// <offset>.
   tags["-class"] = 28; // -class <className> ?<offset>
   /// -ttype  square <x> <y> <z> :: generates a squared template of size x,y,z
   ///                               voxels*.  By default the systems finds the
   ///                               first non empty voxel starting from the
   ///                               top of the volume and the first element
   ///                               in the y axis starts at -1 voxels.
   /// -ttype circle <h> <r> :: generates a cylindrical template with height h
   ///                          and radius r voxels*. By default the systems
   ///                          finds the first non empty voxel starting from
   ///                          the top of the volume and the first element in
   ///                          the y axis starts at -1 voxels. If r is equal
   ///                          to -1, then the radius is calculated according
   ///                          to the height of the middle point.
   tags["-ttype"] = 29;
   /// moves y to start at the -n voxels.
   tags["-mheight"] = 30; // -mheight <n>
   /// the ‘raw’ option saves all the raw intensity values of the template and
   /// the ‘processed’ option saves parameters derived from the raw intensities
   /// Here there is a list with all the derived parameters saved using the processed option
   /// Processed_basic:
   /// ----------------
   /// Height of middle column (height)
   /// Average height (h_ave)
   /// Max Intensity (int_max)
   /// Min Intensity (int_min)
   /// Average Intensity (int_ave)
   /// Mean Intensity  (int_mean)
   /// Standard Deviation (int_std)
   /// Percentage of non-empty voxels (pnev)
   /// Average Height Difference
   /// Average Length of Top Patch (altp)
   /// Average Length of Lower Patch (allp)
   ///
   /// Processed_all:
   /// --------------
   /// Height of middle column (height)
   /// Average height (h_ave)
   /// Max Intensity (int_max)
   /// Min Intensity (int_min)
   /// Average Intensity (int_ave)
   /// Average Intensity (int_ave_r1, int_ave_r2, … , int_ave_rn)***
   /// Mean Intensity  (int_mean)
   /// Mean Intensity per row  (int_mean_r1, int_mean_r2, … , int_mean_rn)***
   /// Standard Deviation (int_std)
   /// Standard Deviation per row (int_std_r1, nt_std_r2, … , int_std_rn)***
   /// Percentage of non-empty voxels (pnev)
   /// Percentage of non-empty voxels per row (pnev_r1, pnev_r2, … , pnev_rn)***
   /// Average Height Difference
   /// Average Height Difference per row (ahd_r1, ahd_r2, …, ahd_rn)***
   /// Average Length of Top Patch (altp)
   /// Average Length of Lower Patch (allp)
   ///
   /// *** r1 is the topest row saved into the template
   tags["-eparameters"] = 31; //-eparameters <raw or processed_basic
                              //              or processed_all>
   /// exports a file with the sum of itensities per pulse seperated with comma
   /// e.g. -las flightline.las intsfileName.csv
   tags["-totalIn"]=32; // -totalIn <intsfileName.csv>

   /// enable to automatically calcualate the boundaries of the volume by taking the
   /// min and max boundaries of the discrete returns recorded withing a file.
   tags["-calBoundaries"]=33; // -calBoundaries <yes or no>


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
              optPolygonisation = (!(std::string(argv[argvIndex])=="off"));
           }
           break;
        }
        case 12: // "--help"
        {
           std::fstream readmeFile;
           readmeFile.open("Readme.txt");
           if(!readmeFile.is_open())
           {
              std::cerr<<"ERROR: README.txt file doesn't exist.\n"
                       <<"Please look at previous versions of DASOS for instructions\n";
              return EXIT_FAILURE;
           }
           std::string line;
           while(std::getline(readmeFile,line))
           {
              std::cout << line << "\n";
           }
           return EXIT_SUCCESS;
           break;
        }
        case 13: // -las
        {
           std::cout << "Multiple LAS files currently not supported\n";
            argvIndex ++;
            if (argvIndex<argc)
            {
               lasFiles.push_back(std::string(argv[argvIndex]));
            }
            break;

//           argvIndex++;
//           while (argvIndex<argc)
//           {
//              count++;
//              if(argv[argvIndex][0]!='-')
//              {

//                 argvIndex++;
//              }
//              else
//              {
//                 break;
//              }
//           }
//           std::cout << argv[argvIndex] << "\n";
//           std::cout << count << " ****\n";
//           break;
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
        case 16: // -exportVolume <'c'> <exportedVolumeFileName>;
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
        case 23: // -exportPulses <noOfPulses> <filename.csv>
        {
            argvIndex++;
            if (argvIndex+1<argc)
            {
               noSamples = atoi(argv[argvIndex]);
               csvSamplesPulses_exportingFile = argv[++argvIndex];
            }
            break;
        }
        case 24: // -icsv <CSVfile.csv>
        {
            argvIndex++;
            isFieldPlotManagerValid = true;
            if (argvIndex<argc)
            {
               csvImportingFieldplots = argv[argvIndex];
               fieldplotsManager.set_csvImportingFieldplots(argv[argvIndex]);

            }
           break;
        }
        case 25: // -vols <volsDir>
        {
            argvIndex++;
            isFieldPlotManagerValid = true;
            if (argvIndex<argc)
            {
               volsDir=(argv[argvIndex]);
               fieldplotsManager.set_volsDir(argv[argvIndex]);
            }
           break;
        }
        case 26: // -ocsv <csvFileName>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              csvExportingTemplates = argv[argvIndex];
              fieldplotsManager.set_csvExportingTemplates(argv[argvIndex]);
           }
           break;
        }
        case 27: // -column <label>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              fieldplotsManager.set_fcolumn(argv[argvIndex]);
           }
           break;
        }
        case 28: // -class <className>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              fieldplotsManager.set_fclass(argv[argvIndex]);
           }
           if((argvIndex+1<argc) && argv[argvIndex+1][0]!='-')
           {
              argvIndex++;
              fieldplotsManager.set_roffset(atof(argv[argvIndex]));
           }
           break;
        }
        case 29: // -ttype  square <x> <y> <z>  or
                 // -ttype circle <h> <r>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              std::string ttypeS = argv[argvIndex];
              std::transform(ttypeS.begin(), ttypeS.end(), ttypeS.begin(), toupper);
              if(ttypeS=="SQUARE")
              {
                  fieldplotsManager.set_ttype(0);
              }
              else if (ttypeS=="CYLINDER")
              {
                  fieldplotsManager.set_ttype(1);
              }
              else
              {
                 std::cerr << "WARNING: Wrong template type. It is set to 'square'.";
                 fieldplotsManager.set_ttype(0);
              }
           }
           if(((argvIndex+2<argc)&&fieldplotsManager.get_ttype()==1)
                   || ((argvIndex+3<argc)&&fieldplotsManager.get_ttype()==0))
           {
              if(fieldplotsManager.get_ttype()==1) // cylinder
              {
                  fieldplotsManager.set_th(atoi(argv[++argvIndex]));
                  fieldplotsManager.set_tr(atoi(argv[++argvIndex]));
              }
              else // square
              {
                 fieldplotsManager.set_tx ( atoi(argv[++argvIndex]));
                 fieldplotsManager.set_ty ( atoi(argv[++argvIndex]));
                 fieldplotsManager.set_tz ( atoi(argv[++argvIndex]));
              }
              if(fieldplotsManager.checkthrxyz())
              {
                 std::cerr << "ERROR: Parameters of ttype should be greater than zero. " <<
                              "Check whether the correct number of parameters were given. " <<"\n";
                 return EXIT_FAILURE;
              }
           }
           else
           {
              std::cerr << "Missing Arguments for -ttype\n";
           }
           break;
        }
        case 30: // -mheight <n>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              fieldplotsManager.set_mheight (atoi(argv[argvIndex]));
           }
           break;
        }
        case 31: //-eparameters <raw or processed_basic or processed_all>
        {
           argvIndex++;
           isFieldPlotManagerValid = true;
           if (argvIndex<argc)
           {
              std::string parType(argv[argvIndex]);
              std::transform(parType.begin(), parType.end(), parType.begin(), toupper);
              fieldplotsManager.set_eparameters ( (parType=="RAW")?0 :
                           ((parType=="PROCESSED_BASIC" || parType=="PROCESSED")? 1:
                           ((parType=="PROCESSED_ALL") ? 2:-1)));
              if(((parType=="RAW")?0 :
                      ((parType=="PROCESSED_BASIC"|| parType=="PROCESSED")? 1:
                      ((parType=="PROCESSED_ALL") ? 2:-1))) == -1)
              {
                 std::cerr << "WARNING: " << argv[argvIndex]
                           << " is unknow type of parameters. Set to 'raw' \n";
                 fieldplotsManager.set_eparameters (0);
              }
           }
           break;
        }
        case 32: //  -totalIn <intsfileName.csv>
        {
           argvIndex ++;
           if (argvIndex<argc)
           {
              intsfileName = argv[argvIndex];
           }
           break;
        }
        case 33: //  -calBoundaries <yes or no>
        {
           argvIndex++;
           if (argvIndex<argc)
           {
              std::string parType(argv[argvIndex]);
              std::transform(parType.begin(), parType.end(), parType.begin(), toupper);
              calBoundaries=(parType=="YES");
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
       std::cout << "Type DASOS --help and follow the instructions\n";
       std::this_thread::sleep_for(std::chrono::milliseconds(20000));
       return EXIT_FAILURE;
    }

   //INTERPRETATION OF DATA
   fieldplotsManager.printParameters();
   //--------------------------------------------------------------------------------------
   // create templates from fieldplot
   //--------------------------------------------------------------------------------------
   // check validity of fieldplot manager
   if(isFieldPlotManagerValid)
   {
       fieldplotsManager.interprateData(volumeType,isolevel);
   }

   Volume *vol = NULL; 
   double Construction_total (0);



   //--------------------------------------------------------------------------------------
   // read filename
   //--------------------------------------------------------------------------------------
   if(lasFiles.size()==0 && volumeFileName=="" && pwFiles.size()==0)
   {
      if(isFieldPlotManagerValid)
      {
         std::cout << "   ***   EXIT   ***\n";
         return EXIT_SUCCESS;
      }
      std::cerr << "LAS, pulsewave or volume file haven't been specified\n";
      std::cerr << "use \"DASOS --help\" for instructions.\n";
      return EXIT_FAILURE;
   }
   if(csvSamplesPulses_exportingFile!="")
   {
      std::cout << "Exporting 10 pulses into " << csvSamplesPulses_exportingFile << "\n"
                << "Pulses exported from the first file loaded\n";
      if(lasFiles.size()!=0)
      {
         Las1_3_handler lala(lasFiles[0]);
         lala.saveSamples(csvSamplesPulses_exportingFile,noSamples);
      }
      else if (pwFiles.size()!=0)
      {
         PW_handler lala(pwFiles[0]);
         lala.saveSamples(csvSamplesPulses_exportingFile,noSamples);
      }
      else
      {
         std::cerr << "ERROR: no pulsewaves or las file loaded\n";
         return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
   }
   if(intsfileName!="")
   {
      std::cout << "Exporting sum intensity of pulses into " << csvSamplesPulses_exportingFile << "\n";
      if(lasFiles.size()!=0)
      {
         Las1_3_handler lala(lasFiles[0]);
         lala.saveSumIntensity(intsfileName);
      }
      else
      {
         std::cerr << "ERROR: no pulsewaves or las file loaded\n";
         return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
   }
   std::cout << lasFiles.size() << " ----- las file size\n";
   if(lasFiles.size()!=0)
   {
      Las1_3_handler lala(lasFiles[0]);
      lala.printPublicHeader();
      if(userLimits[0]<0.001 && userLimits[0]>-0.0001)
      {
         //then user haven't defined limits
         std::cout << "WARNING: Limits haven't been set, so entire file will be loaded\n";
         if (calBoundaries)
         {
            std::cout << "Calculating boundaries of discrete returns\n";
           userLimits = lala.calBoundaries();
//            userLimits[0]=708587264.00;
//            userLimits[1]=612764480.00;
//            userLimits[2]=607443584.00;
//            userLimits[3]=506632352.00;
//            userLimits[4]=247611392.00;
//            userLimits[5]= 75554704.00;

         }
         else
         {
             userLimits = lala.getBoundaries();
         }
      }
      else
      {
         std::vector<double> temp_userLimits(lala.getBoundaries());
         userLimits[4] = temp_userLimits[4];
         userLimits[5] = temp_userLimits[5];
      }
      std::cout<< "userLimits "<< userLimits[0] << " " << userLimits[1]<<" "
                << userLimits[2]<<" " << userLimits[3]<<" " <<userLimits[4]<<" " << userLimits[5]<<"\n";
      std::vector<double> temp_userLimits(lala.getBoundaries());
      std::cout << "Fligtline Limits "<< temp_userLimits[0] << " " << temp_userLimits[1]<<" "
                << temp_userLimits[2]<<" " << temp_userLimits[3]<<" " <<temp_userLimits[4]<<" " << temp_userLimits[5]<<"\n";


      const std::clock_t before_Vol_Construction = std::clock();
      // -las "C:\Users\milto\Documents\TEPAK\SampleData\L004-1-M01-S1-C1_s_w_2.las" -dtm "C:\Users\milto\Documents\TEPAK\SampleData\DTM_1m_AofI.bil" -map height "C:\Users\milto\Documents\TEPAK\SampleData\height.dtm2"
      //  userLimits[5]-=380.0;
      vol = VolumeFactory::produceVolume(voxelLength,userLimits,volumeType);
      vol->setNoiseLevel(noiseLevel);
      lala.readFileAndGetObject(vol,fileType,dtmFileName);
//      for(unsigned int nextFile=1; nextFile<lasFiles.size(); ++nextFile)
//      {
//         Las1_3_handler nextLAShandler(lasFiles[nextFile]);
//         nextLAShandler.readFileAndGetObject(vol,fileType,dtmFileName);
//      }
      vol->normalise();

       Construction_total = double(std::clock()-before_Vol_Construction)/ double(CLOCKS_PER_SEC); // time in milliseconds

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
       vol->setIsolevel(isolevel);
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







 const std::clock_t before_poly = std::clock();
   //--------------------------------------------------------------------------------------
   // Polygonise volume
   //--------------------------------------------------------------------------------------
   if(objFileName!="")
   {
      GLData *glData = new GLData;
      MarchingCubes *mc = VolumeFactory::getMarchingCubes(
                  volumeType,optPolygonisation,vol,ceil((userLimits[2]-userLimits[3])/voxelLength)
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

   double pol_time = double(std::clock()-before_poly) / double(CLOCKS_PER_SEC); // time in milliseconds

   std::cout << std::setprecision(5) << "Vol_Construction_time = " << Construction_total << " seconds\n";
   std::cout << std::setprecision(5) << "pol_time = " << pol_time << " seconds\n";
   std::cout << std::setprecision(5) << "total = " << (pol_time+Construction_total)<< " seconds\n";

   //--------------------------------------------------------------------------------------
   // generate maps
   //--------------------------------------------------------------------------------------
   // if mapsAll then add all the fw maps to mapInfo
   if(mapsAll!="")
   {
      MapsManager *m;
      m = new MapsManager;
      const std::vector<std::string> &metricsNames = m->getNamesOfFWMetrics();
      unsigned int noOfMetrics=metricsNames.size();
      for(unsigned int i=0; i<noOfMetrics;++i)
      {
          mInfo.push_back(new MapsManager::mapInfo);
          mInfo[mInfo.size()-1]->type = metricsNames[i];
          mInfo[mInfo.size()-1]->name = mapsAll + metricsNames[i];
          mInfo[mInfo.size()-1]->band = 140;
          mInfo[mInfo.size()-1]->thres = 0;
          mInfo[mInfo.size()-1]->samp = 0;
          ++mapsIndex;
      }
      delete m;
   }
   for(unsigned int i=0; i<mInfo.size(); ++i)
   {
      mInfo[i]->obj = vol;
      m.createMap(mInfo[i]);
      delete mInfo[i];
   }
   if(mInfo.size()!=0)
   {
      std::cout << "ALL MAPS Saved\n";
   }



   if(vol!=NULL)
   {
      delete vol;
   }
   std::cout << "   ***   EXIT   ***\n";
   return EXIT_SUCCESS;

}
