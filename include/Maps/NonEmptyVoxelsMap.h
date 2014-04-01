#ifndef THICKNESSMAP_H
#define THICKNESSMAP_H

#include "Map.h"

class NonEmptyVoxelsMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    NonEmptyVoxelsMap(const std::string i_name, Object *i_obj);
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
