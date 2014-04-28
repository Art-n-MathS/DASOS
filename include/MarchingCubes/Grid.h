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

#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <string>
#include <unordered_map>


class Grid
{
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
   /// @brief constructor
   /// @param[in] i_IGMfileName the name of the IGM file
   /// @param[in] i_vl the length of the square
   /// @param[in] i_LiDARmins the min values of the lidar voxels, used to align
   /// the data
   //--------------------------------------------------------------------------
   Grid(
           const std::string &i_IGMfileName,
           const float i_vl,
           const ngl::Vec3 i_LiDARmins
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
           ) const;
   //--------------------------------------------------------------------------
   /// @brief given a point, this method finds the square that point lies in
   /// and then returns all the pixel positions of the pixels that lie inside
   /// that square
   /// @param[in] i_point the point of our interest
   /// @param[io] i_pixPos the pixels positions of all the points of the square
   /// that i_point lies in
   /// @returns the number of points that lie inside that square
   /// @note please delete [] i_pixPos after calling this function to avoid
   /// memory leaks
   //--------------------------------------------------------------------------
   unsigned short int pixIndicesOfSquare(
           const ngl::Vec2 &i_point,
           unsigned int **i_pixPos
           )const;
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~Grid();

private:
   //--------------------------------------------------------------------------
   /// @brief method that read the X & Y positions of the pixels from the IGM
   /// file and calcuated the Maximum and minimum locations
   /// @param[in] i_IGMfileName the name of the igm file
   //--------------------------------------------------------------------------
   void readIGMandFindMinsMaxs(const std::string &i_IGMfileName);
   //--------------------------------------------------------------------------
   /// @brief method that loops through all the geographical positions of the
   /// pixels and imports them inside the grid
   //--------------------------------------------------------------------------
   void fillGrid();
   //--------------------------------------------------------------------------
   /// @brief method that given a long returns you the pixel position of the
   /// corresponding number
   /// @param[in] i_a = nsamps*y1 + x1, where (x1,y1) the position of the pixel
   /// @returns the position (x1,y1) of the pixel
   //--------------------------------------------------------------------------
   ngl::Vec2 getPixelPosFromKey(const unsigned int i_a) const;
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
           ) const;
   //--------------------------------------------------------------------------
   /// @brief method that takes the position of the pixel and returns its key
   /// @param[in] i_x the x position of the pixel
   /// @param[in] i_y the y position of the pixel
   //--------------------------------------------------------------------------
   unsigned int getKeyOfPixel(
           const unsigned int i_x,
           const unsigned int i_y
           )const;

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
   std::unordered_multimap<int, int> m_map;
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
