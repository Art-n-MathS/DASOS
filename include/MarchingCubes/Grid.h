#ifndef GRID_H
#define GRID_H

//-------------------------------------------------------------------------
/// @file Grid.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 24/03/14
/// @class Grid
/// @brief this class is a Grid that is used to align the hyperspectral
/// data with the object. Each pixel is inserted into the square that
/// it lies in. Once the grid is constructed,the user can query it and find
/// the closer pixel to a given point.
//-------------------------------------------------------------------------

#include <ngl/Vec2.h>
#include <string>
#include <unordered_map>


class Grid
{

   typedef std::unordered_multimap<int, int> stringmap;
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_IGMfileName the name of the IGM file
   /// @param[in] i_samplingRate average of expected pixels per square
   //--------------------------------------------------------------------------
   Grid(
           const std::string &i_IGMfileName,
           const unsigned short int i_samplingRate
           );
   //--------------------------------------------------------------------------
   /// @brief method that takes as input a point and returns the position of
   /// its closest pixel
   /// @param[in] i_x the x position of the vertex of our interest
   /// @param[in] i_y the y position of the vertex of our interest
   /// @returns the position of its closest pixel on the image
   //--------------------------------------------------------------------------
   ngl::Vec2 getPixelPositionScaled0_1(
           const float i_x,
           const float i_y
           );
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~Grid();

private:
   //--------------------------------------------------------------------------
   /// @brief method that given a long returns you the pixel position of the
   /// corresponding number
   /// @param[in] i_a = nsamps*y1 + x1, where (x1,y1) the position of the pixel
   /// @returns the position (x1,y1) of the pixel
   //--------------------------------------------------------------------------
   ngl::Vec2 getPixelPosFromKey(const unsigned int i_a);
   //--------------------------------------------------------------------------
   /// @brief method that takes the x,y position of a square and returns the
   /// key of the corresponding container in m_multiMap
   /// @param[in] i_x the x position of the square
   /// @param[in] i_y the y position of the square
   /// @returns the key of the corresponding container in m_multiMap
   //--------------------------------------------------------------------------
   unsigned int getKeyOfSquare(
           const unsigned int i_x,
           const unsigned int i_y
           );
   //--------------------------------------------------------------------------
   /// @brief method that takes the position of the pixel and returns its key
   /// @param[in] i_x the x position of the pixel
   /// @param[in] i_y the y position of the pixel
   //--------------------------------------------------------------------------
   unsigned int getKeyOfPixel(
           const unsigned int i_x,
           const unsigned int i_y
           );

   //--------------------------------------------------------------------------
   /// @brief the number of squares in the x-axis
   //--------------------------------------------------------------------------
   unsigned int m_nX;
   //--------------------------------------------------------------------------
   /// @brief the number of squares in the y-axis
   //--------------------------------------------------------------------------
   unsigned int m_nY;
   //--------------------------------------------------------------------------
   /// @brief the number of samples in the x-axis
   //--------------------------------------------------------------------------
   unsigned int m_nsamps;
   //--------------------------------------------------------------------------
   /// @brief the number of lines int the y-axis
   //--------------------------------------------------------------------------
   unsigned int m_nlines;
   //--------------------------------------------------------------------------
   /// @brief the min values of the geo positions of the pixels
   //--------------------------------------------------------------------------
   ngl::Vec2 m_min;
   //--------------------------------------------------------------------------
   /// @brief the max values of the geo positions of the pixels
   //--------------------------------------------------------------------------
   ngl::Vec2 m_max;

   //--------------------------------------------------------------------------
   stringmap m_map;
   //--------------------------------------------------------------------------
   /// @brief the x positions of the IGM file
   //--------------------------------------------------------------------------
   double *m_Xs;
   //--------------------------------------------------------------------------
   /// @brief the y positions of the IGM file
   //--------------------------------------------------------------------------
   double *m_Ys;

};

#endif // GRID_H
