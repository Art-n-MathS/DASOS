#ifndef MAPSMANAGER_H
#define MAPSMANAGER_H

#include "Map.h"
#include <map>

class MapsManager
{
public:
    struct mapInfo
    {
       std::string type;
       std::string name;
       Object *obj;
       unsigned int band;
       std::string bilFileName;
       std::string IGMfileName;
       unsigned int thres;
       unsigned int samp;
       std::string fodisFileName;
       std::string spectralSignature;
       std::string spectralSignatureType;
    };

    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    MapsManager();
    //-------------------------------------------------------------------------
    /// @brief method that creates a map of a given type
    /// @param[in] i_type the type of the map to be created
    //-------------------------------------------------------------------------
    void createMap(mapInfo *m_infoOfMap);
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
