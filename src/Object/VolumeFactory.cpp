#include "VolumeFactory.h"

#include "VolumeHashed1DArray.h"

#include <map>
#include <iostream>
#include <algorithm>


VolumeFactory::typesMap VolumeFactory::types = {
    {"HASHED_1D_ARRAY",7}
};

//-----------------------------------------------------------------------------
Volume *VolumeFactory::produceVolume(
        double i_voxelLength,
        const std::vector<double> &i_user_limits,
        const std::string &i_type
        )
{

   Volume *myVolume = nullptr;

   std::string type = i_type;
   std::transform(type.begin(), type.end(), type.begin(), toupper);

   switch (types[type]) {

   case 7: // HASHED_1D_ARRAY
      myVolume = new VolumeHashed1DArray(i_voxelLength,i_user_limits,type);
      break;

   default:
      std::cout << "ERROR: *Undifined type of Object " << type.size() << "\nProgram will terminate\n";
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
   MarchingCubes *myMC = nullptr;
   std::string type = i_type;
   std::transform(type.begin(), type.end(), type.begin(), toupper);

   if(i_optimisation)
   {

      switch (types[type]) {

      case 7: // HASHED_1D_ARRAY
         std::cout << "WARNING: Optimisation ignored, since not supported with "
                   << i_type << " Volume type\n";
         myMC = new MarchingCubes(i_obj,i_x);
         break;

      default:
         std::cout << "ERROR: *Undifined type of Object " << type << " 2\nProgram will terminate\n";
         std::exit(EXIT_FAILURE);

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
    Volume *myVolume = nullptr;

    std::string type = i_type;
    std::transform(type.begin(), type.end(), type.begin(), toupper);

    switch (types[type]) {

    case 7: // HASHED_1D_ARRAY
       myVolume = new VolumeHashed1DArray(i_filename);
       break;

    default:
       std::cout << "ERROR: *Undifined type of Object " << type << " 3\nProgram will terminate\n";
       std::exit(EXIT_FAILURE);
       break;
    }
    return myVolume;
}



