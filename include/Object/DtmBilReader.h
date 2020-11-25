#ifndef DTMBILREADER_H
#define DTMBILREADER_H

//-----------------------------------------------------------------------------
/// @file DtmBilReader.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 03/02/16
/// @class DtmBilReader
/// @brief Reads a Dtm file in bil format.
//-----------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <vector>
#include <Volume.h>

class DtmBilReader
{
public:
   //--------------------------------------------------------------------------
   /// @brief constructor that preload the file for faster interpetation
   /// @param[in] i_dtm the name of the dtm file in .bil format
   /// @param[in] i_obj the volume object that will be used as reference to
   /// load the values into the correct limits
   //--------------------------------------------------------------------------
   DtmBilReader(const std::string &i_dtm,Volume *i_obj);
   //--------------------------------------------------------------------------
   /// @brief method that takes a input a point and returns the height of the
   /// this location, according to the dtm
   //--------------------------------------------------------------------------
   float getHeightOf(const float i_x, const float i_y) const;
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~DtmBilReader();

private:
   //--------------------------------------------------------------------------
   /// @brief method that checks the endianess of the computer
   //--------------------------------------------------------------------------
   bool isBigEndian();
   //--------------------------------------------------------------------------
   /// @brief method that read the header file
   //--------------------------------------------------------------------------
   void readHeader(const std::string &i_dtm);
   //--------------------------------------------------------------------------
   /// @brief method that gets the location of the height in the bil file given
   /// the x y coordinates of a point
   //--------------------------------------------------------------------------
   unsigned long long int getBilIndex(
           const float i_x,
           const float i_y
           )const;


   //--------------------------------------------------------------------------
   /// @brief the filename of the dtm
   //--------------------------------------------------------------------------
   std::string m_dtm;
   //--------------------------------------------------------------------------
   /// @brief after contructor, it checks whether the dtm bil and hdr files
   /// contain all the required information
   //--------------------------------------------------------------------------
   bool m_isValid;
   //--------------------------------------------------------------------------
   /// @brief the numbers of rows
   //--------------------------------------------------------------------------
   unsigned long long int m_nRows;
   //--------------------------------------------------------------------------
   /// @brief the number of columns
   //--------------------------------------------------------------------------
   unsigned long long int m_nColumns;
   //--------------------------------------------------------------------------
   /// @brief the number of bits per value
   //--------------------------------------------------------------------------
   unsigned long long int m_nBits;
   //--------------------------------------------------------------------------
   /// @brief the type of the pixels
   //--------------------------------------------------------------------------
   std::string m_pixelType;
   //--------------------------------------------------------------------------
   /// @brief the length of each cell in the x-axis
   //--------------------------------------------------------------------------
   float m_xLen;
   //--------------------------------------------------------------------------
   /// @brief the length of each cell in the y axis
   //--------------------------------------------------------------------------
   float m_yLen;
   //--------------------------------------------------------------------------
   /// @brief the min x value
   /// The x-axis map coordinate of the center of the upper left pixel.
   //--------------------------------------------------------------------------
   float m_xMin;
   //--------------------------------------------------------------------------
   /// @brief the min y value
   /// The y-axis map coordinate of the center of the upper left pixel.
   //--------------------------------------------------------------------------
   float m_yMin;
   //--------------------------------------------------------------------------
   /// @brief the max y value
   /// The y-axis map coordinate of the center of the upper left pixel.
   //--------------------------------------------------------------------------
   float m_yMax;
   //--------------------------------------------------------------------------
   /// @brief the min y value
   /// The y-axis map coordinate of the center of the upper left pixel.
   //--------------------------------------------------------------------------
   float m_xMax;
   //--------------------------------------------------------------------------
   /// @brief the non data value
   //--------------------------------------------------------------------------
   double m_noData;
   //--------------------------------------------------------------------------
   /// @brief preloaded values
   //--------------------------------------------------------------------------
   std::vector<float> m_heights;
   //--------------------------------------------------------------------------
   /// @brief size of bil file
   //--------------------------------------------------------------------------
   unsigned long long int m_size;



};

#endif // DTMBILREADER_H
