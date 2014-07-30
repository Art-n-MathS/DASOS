#ifndef HYPERSTANDARDDEVIATION_H
#define HYPERSTANDARDDEVIATION_H

#include "Map.h"

class HyperStandardDeviation : public Map
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    HyperStandardDeviation(
            const std::string &i_name,
            Object *i_obj,
            const std::string &i_bilFileName,
            const std::string &i_IGMfileName
            );
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~HyperStandardDeviation();

private:
    //-------------------------------------------------------------------------
    /// @brief the name of the IGM file associated with the hypersctral data
    //-------------------------------------------------------------------------
    std::string m_IGMfileName;
    //-------------------------------------------------------------------------
    void createMap();
    //-------------------------------------------------------------------------


};

#endif // HYPERSTANDARDDEVIATION_H
