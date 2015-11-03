#ifndef INTENSITYMAP_H
#define INTENSITYMAP_H

#include "Map.h"


//-------------------------------------------------------------------------
/// @file IntensityMap.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 14/10/15
/// @class IntensityMap
/// @brief this map finds the average intensity for each column
/// please note that this works better for the discrete data
/// Try IntensityMax for full-waveform
//-------------------------------------------------------------------------


class IntensityMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
    //-------------------------------------------------------------------------
    IntensityMap(const std::string i_name, Object *i_obj);

    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
   ~IntensityMap();


private:
    //-------------------------------------------------------------------------
    /// @brief method that creates a desnsity map
    //-------------------------------------------------------------------------
    void createMap();


};

#endif // INTENSITYMAP_H
