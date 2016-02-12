#ifndef VOLUMEFACTORY_H
#define VOLUMEFACTORY_H

#include "Volume.h"
#include "MarchingCubes.h"
#include <string>
#include <map>

class VolumeFactory
{
public:
   //--------------------------------------------------------------------------
   /// @brief function that returns an object with the data structure of your
   /// choice
   /// @note this function dynamically creates a new object that needs to be
   /// freed manually afterwards
   //--------------------------------------------------------------------------
   static Volume *produceVolume(
           float i_voxelLength,
           const std::vector<double> &i_user_limits,
           const std::string &i_type
           );
   //--------------------------------------------------------------------------
   /// @brief function that creates a volume from a file
   /// @note this function dynamically creates a new object that needs to be
   /// freed manually afterwards
   /// @param[in] i_filname the file that contains the volume
   /// @param[in] i_type the type of the volume the file contains
   //--------------------------------------------------------------------------
   static Volume *produceVolume(
           const std::string &i_filename,
           const std::string &i_type
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns the correct Marching Cubes version to be used
   /// depending on the type of the structure and whether optimisation is
   /// enabled
   /// @param[in] i_type the type of the volume
   /// @param[in] i_optimisation: true if optimisation is enabled and false
   /// otherwise
   /// @note this function dynamically creates a new object that needs to be
   /// freed manually afterwards
   //--------------------------------------------------------------------------
   static MarchingCubes * getMarchingCubes(
           const std::string &i_type,
           const bool i_optimisation,
           Volume *i_obj,
           unsigned int i_x
           );

   typedef std::map<std::string, int> typesMap;
   static typesMap types;




};

#endif // VOLUMEFACTORY_H
