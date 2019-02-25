#ifndef DENSITYMAP_H
#define DENSITYMAP_H

#include "Map.h"

class DensityMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
    //-------------------------------------------------------------------------
    DensityMap(const std::string i_name, Volume *i_obj);

    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~DensityMap();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates a desnsity map
    //-------------------------------------------------------------------------
    void createMap();


};

#endif // DENSITYMAP_H
