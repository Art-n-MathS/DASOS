#ifndef FIRSTPATCH_H
#define FIRSTPATCH_H
#include "Map.h"
//-------------------------------------------------------------------------
/// @file FirstPatch.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 16/03/15
/// @class FirstPatch
/// @brief
//-------------------------------------------------------------------------


class FirstPatch: public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    FirstPatch(
            const std::string i_name,
            Volume *i_obj
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~FirstPatch();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates the Map
    //-------------------------------------------------------------------------
    void createMap();
};

#endif // FIRSTPATCH_H
