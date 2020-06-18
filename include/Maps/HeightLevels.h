#ifndef HEIGHTLEVELS_H
#define HEIGHTLEVELS_H
#include "Map.h"
//-------------------------------------------------------------------------
/// @file HeightLevels.h
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 18th June 2020
/// @class HeightLevels
/// @brief
//-------------------------------------------------------------------------

class HeightLevels: public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    HeightLevels(
            const std::string i_name,
            Volume *i_obj
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~HeightLevels();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates the Map
    //-------------------------------------------------------------------------
    void createMap();
};

#endif // HEIGHTLEVELS_H
