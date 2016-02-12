#ifndef HEIGHT_H
#define HEIGHT_H

//-------------------------------------------------------------------------
/// @file HeightMap.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 13/01/14
/// @class HeightMap
/// @brief this class aims to link the GUI with the libraries.
//-------------------------------------------------------------------------

#include "Map.h"

class ThicknessMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    ThicknessMap(const std::string i_name, Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~ThicknessMap();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates thickness Map
    //-------------------------------------------------------------------------
    void createMap();
};

#endif // HEIGHT_H
