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
    /// @param[in] i_band the band of our interest
    /// @param[in] i_bilFileName the name of the bil file that contains the
    /// hyperspectral images
    //-------------------------------------------------------------------------
    HyperMap(
            const std::string &i_name,
            Object *i_obj,
            const unsigned short int i_band,
            const std::string &i_bilFileName, const std::string &i_IGMfileName
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~HyperMap();

protected:
    //-------------------------------------------------------------------------
    /// @brief
    //-------------------------------------------------------------------------
    virtual void createMap();
    //-------------------------------------------------------------------------
    /// @brief the name of the IGM file associated with the hypersctral data
    //-------------------------------------------------------------------------
    std::string m_IGMfileName;
    //-------------------------------------------------------------------------
    /// @brief the hyperspectral data of the band of our interest
    /// @note the time of the data is assume to be 12 which corresponds to a
    /// 16-bit unsigned integear
    //-------------------------------------------------------------------------
    unsigned short int *m_hyperData;

};

#endif // HYPERMAP_H
