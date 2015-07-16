#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "Map.h"

class HeightMap : public Map
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
   //-------------------------------------------------------------------------
   HeightMap(const std::string i_name, Object *i_obj);
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~HeightMap();

private:
   //-------------------------------------------------------------------------
   /// @brief method that creates a desnsity map
   //-------------------------------------------------------------------------
   void createMap();



};

#endif // HEIGHTMAP_H
