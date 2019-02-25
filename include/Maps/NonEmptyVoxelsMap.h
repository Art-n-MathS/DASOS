#ifndef THICKNESSMAP_H
#define THICKNESSMAP_H

#include "Map.h"
#include "Volume.h"

class NonEmptyVoxelsMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    NonEmptyVoxelsMap(const std::string i_name, Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~NonEmptyVoxelsMap();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates thickness Map
    //-------------------------------------------------------------------------
    void createMap();

};

#endif // THICKNESSMAP_H
