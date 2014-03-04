#ifndef MAPSMANAGER_H
#define MAPSMANAGER_H

#include "Map.h"

class MapsManager
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    MapsManager();
    //-------------------------------------------------------------------------
    /// @brief method that creates a map of a given type
    /// @param[in] i_type the type of the map to be created
    //-------------------------------------------------------------------------
    void createMap(
            const std::string &i_type,
            const std::string &i_name,
            Object *i_obj
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~MapsManager();

    //-------------------------------------------------------------------------
    /// @brief the possible types
    //-------------------------------------------------------------------------
    std::map<std::string, int> m_types;


private:
    //-------------------------------------------------------------------------
    /// @brief the map to be created
    //-------------------------------------------------------------------------
    Map *m_map;


};

#endif // MAPSMANAGER_H
