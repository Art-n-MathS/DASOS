#ifndef AVERAGEHEIGHTDIFFERENCE_H
#define AVERAGEHEIGHTDIFFERENCE_H

#include "Map.h"

class AverageHeightDifference: public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name the name of the map image to be saved
    /// @param[in] i_obj the object that the map with derived from
    //-------------------------------------------------------------------------
    AverageHeightDifference(const std::string i_name, Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~AverageHeightDifference();

protected:
    //-------------------------------------------------------------------------
    /// @brief method that creates average height Difference map
    //-------------------------------------------------------------------------
    virtual void createMap();

};

#endif // AVERAGEHEIGHTDIFFERENCE_H
