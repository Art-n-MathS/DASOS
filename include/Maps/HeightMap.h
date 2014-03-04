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

class HeightMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    HeightMap(const std::string i_name, Object *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~HeightMap();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates thickness Map
    //-------------------------------------------------------------------------
    void createMap();
};

#endif // HEIGHT_H
