#include "VolumeFactory.h"

#include "Volume1DArray.h"
#include "MCwithIntegralImages.h"

#include <map>
#include <iostream>
#include <algorithm>


VolumeFactory::typesMap VolumeFactory::types = {
    {"1D_ARRAY", 1},

};

//-----------------------------------------------------------------------------
Volume *VolumeFactory::produceVolume(
        float i_voxelLength,
        const std::vector<double> &i_user_limits,
        const std::string &i_type
        )
{

   Volume *myVolume = NULL;

   std::string type = i_type;
   std::transform(type.begin(), type.end(), type.begin(), toupper);

   switch (types[type]) {
   case 1: // 1D_array
      myVolume = new Volume1DArray(i_voxelLength,i_user_limits,type);
      break;
   default:
      std::cout << "ERROR: Undifined type of Object\nProgram will terminate\n";
      std::exit(EXIT_FAILURE);
      break;
   }

   return myVolume;
}

//-----------------------------------------------------------------------------
MarchingCubes *VolumeFactory::getMarchingCubes(
        const std::string &i_type,
        const bool i_optimisation,
        Volume *i_obj,
        unsigned int i_x
        )
{
   MarchingCubes *myMC = NULL;

   std::string type = i_type;
   std::transform(type.begin(), type.end(), type.begin(), toupper);

   if(i_optimisation)
   {

      switch (types[type]) {
      case 1: // 1D_array
         std::cout << "WARNING: Optimisation ignored, since not supported with "
                   << i_type << " Volume type\n";
         myMC = new MarchingCubes(i_obj,i_x);
         break;
      default:
         myMC = new MarchingCubes(i_obj,i_x);
         break;
      }
   }
   else
   {
      myMC = new MarchingCubes(i_obj,i_x);
   }
   return myMC;
}


//-----------------------------------------------------------------------------
Volume * VolumeFactory::produceVolume(
        const std::string &i_filename,
        const std::string &i_type
        )
{
    Volume *myVolume = NULL;

    std::string type = i_type;
    std::transform(type.begin(), type.end(), type.begin(), toupper);

    switch (types[type]) {
    case 1: // 1D_array
       myVolume = new Volume1DArray(i_filename);
       break;
    default:
       myVolume = new Volume1DArray(i_filename);
       break;
    }
    return myVolume;
}



