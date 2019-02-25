#ifndef TERRAINMODEL_H
#define TERRAINMODEL_H

#include "Map.h"

class TerrainModel: public Map
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    //--------------------------------------------------------------------------

    TerrainModel(const std::string &i_name,
                 Volume *i_obj);

private:
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void createMap();
};

#endif // TERRAINMODEL_H
