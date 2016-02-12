#ifndef SIGNATUREDIFFERNECEMAP_H
#define SIGNATUREDIFFERNECEMAP_H

#include "Map.h"
#include "SpectralSignature.h"

class SignatureDifferneceMap : public Map
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    //--------------------------------------------------------------------------
    SignatureDifferneceMap(
            const std::string &i_name,
            Volume *i_obj,
            const std::string &i_bilFileName,
            const std::string &i_IGMfileName,
            const std::string &i_fodisFileName,
            const std::string &i_signatureFileName,
            const std::string &I_signatureType
            );

    //--------------------------------------------------------------------------
    /// @brief default destructor
    //--------------------------------------------------------------------------
    ~SignatureDifferneceMap();

private:
    //--------------------------------------------------------------------------
    /// @brief
    //--------------------------------------------------------------------------
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
    //-------------------------------------------------------------------------
    /// @brief a spectral signature from ASTER library
    //-------------------------------------------------------------------------
    SpectralSignature m_spectralSingature;


};

#endif // SIGNATUREDIFFERNECEMAP_H
