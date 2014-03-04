#ifndef DENSITYMAP_H
#define DENSITYMAP_H

#include "Map.h"

class DensityMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    DensityMap(const std::string i_name, Object *i_obj);

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
