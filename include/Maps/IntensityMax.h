#ifndef INTENSITYMAX_H
#define INTENSITYMAX_H

#include "Map.h"


//-------------------------------------------------------------------------
/// @file IntensityMap.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 14/10/15
/// @class IntensityMap
/// @brief this map finds the higher intensity for each column
//-------------------------------------------------------------------------


class IntensityMax : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
    //-------------------------------------------------------------------------
    IntensityMax(const std::string i_name, Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~IntensityMax();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates a desnsity map
    //-------------------------------------------------------------------------
    void createMap();

};

#endif // INTENSITYMAX_H
