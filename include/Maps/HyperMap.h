#ifndef HYPERMAP_H
#define HYPERMAP_H

#include "Map.h"

class HyperMap : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
    //-------------------------------------------------------------------------
    HyperMap(const std::string i_name, Object *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~HyperMap();

private:
    //-------------------------------------------------------------------------
    /// @brief
    //-------------------------------------------------------------------------
    void createMap();

};

#endif // HYPERMAP_H
