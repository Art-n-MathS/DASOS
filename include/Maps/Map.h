#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "Volume.h"
#include <iostream>



class Map
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_name the name of the map image to be saved
   /// @param[in] i_obj the object that the map with derived from
   //--------------------------------------------------------------------------
   Map(const std::string i_name,Volume *i_obj);
   //--------------------------------------------------------------------------
   /// @brief copy constructor
   /// @param[in] i_map map to be copied
   //--------------------------------------------------------------------------
   Map(const Map &i_map);
   //--------------------------------------------------------------------------
   /// @brief method that creates and saves a map image
   //--------------------------------------------------------------------------
   void createAndSave(unsigned int i_thres, unsigned int i_sample);

   //--------------------------------------------------------------------------
   /// @brief + operator add Map+Map
   /// @param[in] &i_map the map to add
   /// @returns the Map + i_map
   //--------------------------------------------------------------------------
   const Map operator + (
           const Map &i_map
           )const;

   //--------------------------------------------------------------------------
   /// @brief = operator equal Map = i_map
   /// @param[in] &i_map the map to be copied
   //--------------------------------------------------------------------------
   Map &operator = (
           const Map &i_map
           );
   //--------------------------------------------------------------------------
   /// @brief += operator adds i_map to current map
   /// @param[in] i_map map to be added
   //--------------------------------------------------------------------------
   Map &operator+=(const Map &i_map);
   //--------------------------------------------------------------------------
   /// @brief -= operator adds i_map to current map
   /// @param[in] i_map map to be added
   //--------------------------------------------------------------------------
   Map &operator-=(const Map &i_map);

   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
//    Map operator+(const Map &i_map)const;

   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   virtual ~Map();


private:
   //--------------------------------------------------------------------------
   /// @brief method that saves the map values before normalisation into
   /// a text file
   //--------------------------------------------------------------------------
   void saveAsc();
   //--------------------------------------------------------------------------
   /// @brief method that normalises the values to be between 0 and 255
   //--------------------------------------------------------------------------
   void normalise();
   //--------------------------------------------------------------------------
   /// @brief method that thresholds the map
   /// @param[in] i_thres the threshold that separates the map into two
   /// categories
   //--------------------------------------------------------------------------
   void threshold(unsigned int i_thres);
   //--------------------------------------------------------------------------
   /// @brief method that samples the map
   /// @param[in] i_samp 2^i_samps is the number of returned samples
   //--------------------------------------------------------------------------
   void sample(unsigned int i_samp);
   //--------------------------------------------------------------------------
   /// @brief method that takes as input the values of one of the maps and
   /// produces an image. The size of the input array should be equal to
   /// m_noOfVoxelsX*m_noOfVoxelsY
   //--------------------------------------------------------------------------
   virtual void saveMapToImage();
   //--------------------------------------------------------------------------
   /// @brief method that creates a map
   //--------------------------------------------------------------------------
   virtual void createMap(){std::cout << "this should never be called\n";}


protected:
   //--------------------------------------------------------------------------
   /// @brief method that returns the intensity value of the voxel
   /// @param[in] i_x the x coordinate of the voxel
   /// @param[in] i_y the y coordinate of the voxel
   /// @param[in] i_z the z coordinate of the voxel
   //--------------------------------------------------------------------------
   float getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z);
   //--------------------------------------------------------------------------
   /// @brief method that returns true if the given voxel is inside the object
   /// and false if it is outside the object
   /// @param[in] i_x the x coordinate of the voxel
   /// @param[in] i_y the y coordinate of the voxel
   /// @param[in] i_z the z coordinate of the voxel
   //--------------------------------------------------------------------------
   bool isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z);
   //--------------------------------------------------------------------------
   /// @brief method that flips the image due to different coordinate systems
   //--------------------------------------------------------------------------
   void flip();

   //--------------------------------------------------------------------------
   /// @brief method that returns the index of the map
   /// @param[in] i_x the x position of the pixel on the image
   /// @param[in] i_x the x position of the pixel on the image
   //--------------------------------------------------------------------------
   unsigned int getIndex(unsigned int i_x, unsigned int i_y)const
   {return i_x+m_noOfPixelsX*i_y;}

   //--------------------------------------------------------------------------
   /// @brief the name of the file to be saved
   //--------------------------------------------------------------------------
   std::string m_name;
   //--------------------------------------------------------------------------
   /// @brief object of our interest, for where the maps are derived
   //--------------------------------------------------------------------------
   Volume *m_object;
   //--------------------------------------------------------------------------
   /// @brief the number of pixels in the x axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfPixelsX;
   //--------------------------------------------------------------------------
   /// @brief the number of pixels in the y axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfPixelsY;
   //--------------------------------------------------------------------------
   /// @brief the number of pixels in the z axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfPixelsZ;
   //--------------------------------------------------------------------------
   /// @brief the values of the map
   //--------------------------------------------------------------------------
   std::vector<float> m_mapValues;


};

#endif // MAP_H
