#ifndef NDVI_H
#define NDVI_H

#include "Map.h"

class NDVI: public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    NDVI(const std::string &i_name,
            Object *i_obj,
            const std::string &i_bilFileName,
            const std::string &i_IGMfileName
            , const std::string &i_fodisFileName);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~NDVI();

private:
    //-------------------------------------------------------------------------
    /// @brief method that creates thickness Map
    //-------------------------------------------------------------------------
    void createMap();
    //-------------------------------------------------------------------------
    /// @brief the name of the IGM file associated with the hypersctral data
    //-------------------------------------------------------------------------
    std::string m_IGMfileName;
    //-------------------------------------------------------------------------
    /// @brief the name of the bil file of the hyperspectral data
    //-------------------------------------------------------------------------
    std::string m_bilFileName;
    //-------------------------------------------------------------------------
    /// @brief the name of the bil file of the hyperspectral data
    //-------------------------------------------------------------------------
    std::string m_fodisFileName;

};

#endif // NDVI_H
