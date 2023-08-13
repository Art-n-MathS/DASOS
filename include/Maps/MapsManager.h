#ifndef MAPSMANAGER_H
#define MAPSMANAGER_H

#include "Map.h"
#include <map>
#include <Volume.h>

class MapsManager
{
public:
    struct mapInfo
    {
       std::string type;
       std::string name;
       Volume *obj;
       unsigned int band;
       std::string bilFileName;
       std::string IGMfileName;
       unsigned int thres;
       unsigned int samp;
       std::string fodisFileName;
       std::string spectralSignature;
       std::string spectralSignatureType;
       std::string templatesPosFileName;
       std::string templatesNegFileName;
       std::string fieldplot;
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
    /// @brief method that returns a list with the names of all the
    /// full-waveform related maps
    //-------------------------------------------------------------------------
    const std::vector<std::string > &getNamesOfFWMetrics()const;
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~MapsManager();

    //-------------------------------------------------------------------------
    /// @brief the possible types
    //-------------------------------------------------------------------------
    std::map<std::string, int> m_types;
    //-------------------------------------------------------------------------
    /// @brief create and store waveform maps given a vol
    /// Used in Int_PlotsManager for exporting feature vectors
    /// @param[in] i_vol the volume object to be used for creating the maps
    /// @note generates all maps into the m_maps vector. If a map is equal to
    /// zero then it failed to create it. But it is importatn to keep it in the
    /// order to match with the getNamesOfFWMetrics function
    //-------------------------------------------------------------------------
    void createALLFWMAPs(Volume *i_vol);
    //-------------------------------------------------------------------------
    /// @brief method that takes as input a list of 2D coordinates from a
    /// window and returns a list with the mean and std of all the maps stored
    /// in m_maps. The vector return is double the size of m_maps, first the
    /// mean is stored and then the std of each map
    /// i_coo the list of coordinates in index form of the volume
    //-------------------------------------------------------------------------
    const std::vector<double> & getValues(
            const std::vector< std::pair<unsigned int, unsigned int> > &i_coo);
    //-------------------------------------------------------------------------
    /// @brief an array were the mean and std of the 2D metrics at plot
    /// location is stored
    //-------------------------------------------------------------------------
    std::vector<double> m_values;
    //-------------------------------------------------------------------------

private:
    //-------------------------------------------------------------------------
    /// @brief the map to be created
    //-------------------------------------------------------------------------
    Map *m_map;
    //-------------------------------------------------------------------------
    /// @brief the names of all the full-waveform metrics
    //-------------------------------------------------------------------------
    const std::vector<std::string> m_FWMetrics;
    //-------------------------------------------------------------------------
    /// @brief list of current maps - used in Int_PlotsManager for geting
    /// feature vectors
    //-------------------------------------------------------------------------
    std::vector<Map *> m_maps;



};

#endif // MAPSMANAGER_H
