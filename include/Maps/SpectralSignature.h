#ifndef SPECTRALSIGNATURE_H
#define SPECTRALSIGNATURE_H
#include <string>
#include <vector>

//-------------------------------------------------------------------------
/// @file SpectralSignature.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 21/01/15
/// @class SpectralSignature
/// @brief this class is responsible for reading specrtal signatures from
/// ASTER libary. It is useful for producing metrics with pixelwise squared
/// difference between the spectral signature and each pixel's spectrum
//-------------------------------------------------------------------------


class SpectralSignature
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_file the name/position of the spectral signature file
   //--------------------------------------------------------------------------
   SpectralSignature(const std::string &i_file, const std::string i_type);
   //--------------------------------------------------------------------------
   /// @brief method that returns the average error percentage difference
   /// it first scales the the input reflectances from 0 to
   /// @param[in] i_wavelength the wavelegths of the pixel spectrum in
   /// ascending order
   /// @param[in] i_reflectance the return reflectance per wavelegth
   //--------------------------------------------------------------------------
   float percentageError(
           const std::vector<float> &i_wavelength,
           const std::vector<float> &i_reflectance
           );
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~SpectralSignature();

private:
   //--------------------------------------------------------------------------
   /// @brief name of the class
   //--------------------------------------------------------------------------
   std::string m_className;
   //--------------------------------------------------------------------------
   /// @brief the wavelengths in nm
   //--------------------------------------------------------------------------
   std::vector<float> m_wavelengths;
   //--------------------------------------------------------------------------
   /// @brief the refectance at each corresponding wavelength in precent
   //--------------------------------------------------------------------------
   std::vector<float> m_reflectances;

};

#endif // SPECTRALSIGNATURE_H
