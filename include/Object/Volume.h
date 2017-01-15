#ifndef VOLUME_H
#define VOLUME_H

#include <vector>
#include <gmtl/Vec.h>
#include <string>

class Volume
{


public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   Volume(float i_voxelLength,
           const std::vector<double> &i_userLimits,
          const std::string &i_type

          );

   //-------------------------------------------------------------------------
   /// @brief testing constructor - used to read data exported from a file
   /// to speed up testing
   //-------------------------------------------------------------------------
   Volume(const std::string &i_filename);

   //-------------------------------------------------------------------------
   /// @brief testing constructor - used to read data exported from a file
   /// to speed up testing
   //-------------------------------------------------------------------------
   Volume(const std::string &i_filename,bool evaluation);

   virtual bool isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z) = 0;

   //-------------------------------------------------------------------------
   /// @brief gets the function value of the object given a point
   /// @param[in] i_point the given point
   //-------------------------------------------------------------------------
   virtual float functionValue(
           const gmtl::Vec3f &i_point
           )=0;
   //-------------------------------------------------------------------------
   /// @brief method that returns the value of a pixel given its indices
   /// @param[in] i_x
   /// @param[in] i_y
   /// @param[in] i_z
   //-------------------------------------------------------------------------
   virtual float getIntensity(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z
           )=0;
   //-------------------------------------------------------------------------
   /// @brief method that returns the coordinates of a given voxel
   /// @param[in] i_x
   /// @param[in] i_y
   /// @returns the geospatial coordinates of (i_x,i_y)
   //-------------------------------------------------------------------------
   gmtl::Vec2f getCoordinatesMiddle(unsigned int i_x,unsigned int i_y)const;
   //-------------------------------------------------------------------------
   /// @brief method that adds the itensity of a point to the voxel area
   /// @param[in] i_point the position of the point
   /// @param[in] i_itensity the itensity of that point
   /// @note please note that after calling normalise you should not add any
   /// more itensities to the voxel area
   //-------------------------------------------------------------------------
   void addItensity(const gmtl::Vec3f &i_point,
           float i_intensity
           );

   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices of a the voxel that
   ///  contains the given point
   /// @param[in] i_x the x-coordinates of the point
   /// @param[in] i_y the y-coordinates of the point
   //-------------------------------------------------------------------------
   gmtl::Vec2i getIndices(float i_x, float i_y)const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices of a the voxel that
   ///  contains the given point
   /// @param[in] i_x the x-coordinates of the point
   /// @param[in] i_y the y-coordinates of the point
   /// @param[in] i_z the z-coordinates of the point
   //-------------------------------------------------------------------------
   gmtl::Vec3i getIndices(float i_x, float i_y, float i_z)const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min Limits of the object
   //-------------------------------------------------------------------------
   const gmtl::Vec3f getMinLimits()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max Limits of the object
   //-------------------------------------------------------------------------
   const gmtl::Vec3f getMaxLimits()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the isolevel of the object
   //-------------------------------------------------------------------------
   double getIsolevel()const;
   //-------------------------------------------------------------------------
   /// @brief method that sets the noise level of the radiation return
   /// @param[in] i_noiseLevel the new noise level
   //-------------------------------------------------------------------------
   void setNoiseLevel(double i_noiseLevel);
   //-------------------------------------------------------------------------
   /// @brief method that returns the intensities of the object
   /// @return the intensities of the voxels.
   //-------------------------------------------------------------------------
   /// @brief method that sets the threshold of the object
   /// @param[in] i_thres the new threashold that defines the surface
   //-------------------------------------------------------------------------
   void setIsolevel(double i_thres);
   //-------------------------------------------------------------------------
   /// @brief method that normalises the values of the voxels
   /// makes everything belong to the limits [-1,1]
   //-------------------------------------------------------------------------
   virtual void normalise()=0;
   //-------------------------------------------------------------------------
   /// @brief mehtod that returns the isolevel of the object
   //-------------------------------------------------------------------------
   float getIsolevel(){return m_isolevel;}
   //-------------------------------------------------------------------------
   unsigned int getNoVoxelsX()const{return m_noOfVoxelsX;}
   unsigned int getNoVoxelsY()const{return m_noOfVoxelsY;}
   unsigned int getNoVoxelsZ()const{return m_noOfVoxelsZ;}



   //-------------------------------------------------------------------------
   /// @brief this function was generated for testing. It compares 2 objects
   /// and prints the number of corresponding voxels that do not have the
   /// same intensity value
   //-------------------------------------------------------------------------
   void compareVolume(Volume *i_obj);
   //-------------------------------------------------------------------------
   /// @brief exports all the information of the 3D volume into a file
   /// it is used to speed up testing, especially using valgrind
   /// @param[in] i_compression true if enabled and false otherwise
   /// @param[in] i_filename the name of file to be exported
   //-------------------------------------------------------------------------
   virtual void exportToFile(std::string i_filename, bool i_compression);
   //-------------------------------------------------------------------------
   /// @brief method that returns the sum of teh given area
   /// @param[in] i_posX the min X position of the area of our interest
   /// @param[in] i_posY the min Y position of the area of our interest
   /// @param[in] i_posZ the min Z position of the area of our interest
   /// @param[in] i_lenX the length of the area in the x-axis
   /// @param[in] i_lenY the length of the area in the y-axis
   /// @param[in] i_lenZ the length of the area in the z-axis
   //-------------------------------------------------------------------------
   virtual double getSumOfArea(
           unsigned int i_posX,
           unsigned int i_posY,
           unsigned int i_posZ,
           unsigned int i_lenX,
           unsigned int i_lenY,
           unsigned int i_lenZ
           )const;
   //-------------------------------------------------------------------------
   /// @brief methdod that returns the length of the voxel
   //-------------------------------------------------------------------------
   float getVoxelLen()const{return m_lengthOfVoxel;}
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   virtual ~Volume();


protected:

   //-------------------------------------------------------------------------
   /// @brief method that returns the index of a voxel given its position in
   /// the space
   /// @param[in] i_x the x position of the voxel
   /// @param[in] i_y the y position of the voxel
   /// @param[in] i_z the z position of the voxel
   //-------------------------------------------------------------------------
   unsigned int getIndex(
           const gmtl::Vec3f &i_point
           ) const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the centre of a voxel given its x y z index
   /// @param[in] i_x the position of the voxel of our interest in the x axis
   /// @param[in] i_y the position of the voxel of our interest in the y axis
   /// @param[in] i_z the position of the voxel of our interest in the z axis
   //-------------------------------------------------------------------------
   gmtl::Vec3f getCentreOfVoxel(
           int i_x,
           int i_y,
           int i_z
           )const;
   //-------------------------------------------------------------------------
   /// @brief
   //-------------------------------------------------------------------------
   virtual void readObjectFromFile(
           const std::string &i_filename,
           bool evaluation
           )=0;
   //-------------------------------------------------------------------------
   /// @brief method that adds the itensity of a point to the voxel area
   /// @param[in] i_point the position of the point
   /// @param[in] i_itensity the itensity of that point
   /// @note please note that after calling normalise you should not add any
   /// more itensities to the voxel area
   //-------------------------------------------------------------------------
   virtual void addItensityTypeVol(const gmtl::Vec3f &i_point,
           float i_intensity
           ) = 0;

   //-------------------------------------------------------------------------
   /// @brief indicates whether the volume is only read for evaluation. this
   /// option contains minimal elements and cannot derive information from
   /// it.
   /// @note may lead to segmentation fault if used other than evaluation
   //-------------------------------------------------------------------------
   bool m_evaluation;
   //-------------------------------------------------------------------------
   /// @brief the type of the volume
   //-------------------------------------------------------------------------
   std::string m_type;
   //-------------------------------------------------------------------------
   /// @brief the length of voxel (voxels are cubes)
   //-------------------------------------------------------------------------
   float m_lengthOfVoxel;
   //-------------------------------------------------------------------------
   /// @brief the min limits of the object
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_lowerLimits;
   //-------------------------------------------------------------------------
   /// @brief the max limits of the object
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_higherLimits;
   //-------------------------------------------------------------------------
   /// @brief the isolevel of the object
   //------------------------------------------------------------------------
   double m_isolevel;
   //-------------------------------------------------------------------------
   /// @brief the number of voxels in the x axis
   //-------------------------------------------------------------------------
   unsigned int m_noOfVoxelsX;
   //-------------------------------------------------------------------------
   /// @brief the number of voxels in the y axis
   //-------------------------------------------------------------------------
   unsigned int m_noOfVoxelsY;
   //-------------------------------------------------------------------------
   /// @brief the number of voxels in the z axis
   //-------------------------------------------------------------------------
   unsigned int m_noOfVoxelsZ;
   //-------------------------------------------------------------------------
   /// @brief the distance between the limits
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_dis;
   //-------------------------------------------------------------------------
   /// @brief the noise level
   //-------------------------------------------------------------------------
   double m_noiseLevel;



};

#endif // VOLUME_H
