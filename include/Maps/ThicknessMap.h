#ifndef THICKNESSMAP_H
#define THICKNESSMAP_H

#include "Map.h"

class ThicknessMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    ThicknessMap(const std::string i_name, Object *i_obj);
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

#endif // THICKNESSMAP_H
