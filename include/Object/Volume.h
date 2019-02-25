#ifndef VOLUME_H
#define VOLUME_H

#include <vector>
#include <gmtl/Vec.h>
#include <string>
#include <limits.h>

class Volume
{


public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_userLimits : [0:MaxNorthY, 1:MinNorthY , 2:MaxEastX,
   ///                            3:MinEastX , 4:MaxHeightZ, 5:MinHeightZ]
   //--------------------------------------------------------------------------
   Volume(double i_voxelLength,
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

   virtual bool isInside(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z
           ) = 0;

   //-------------------------------------------------------------------------
   /// @brief method that returns the coordinates of a given voxel
   /// @param[in] i_x
   /// @param[in] i_y
   /// @returns the geospatial coordinates of (i_x,i_y)
   //-------------------------------------------------------------------------
   gmtl::Vec2f getCoordinatesMiddle(unsigned int i_x,unsigned int i_y)const;
   //-------------------------------------------------------------------------
   /// @brief method that returns true when the max/sum of a branch is empty
   /// @note this is only used in IntegralOctree and Series of Hashed Octrees
   /// @param[in] i_key
   /// @param[in] i_level the level of the octree
   //-------------------------------------------------------------------------
   virtual double getMaxValue(
           unsigned int i_key,
           unsigned int i_level
           );
   //-------------------------------------------------------------------------
   /// @brief gets the function value of the object given a point
   /// @param[in] i_point the given point
   //-------------------------------------------------------------------------
   virtual double functionValue(
           const gmtl::Vec3f &i_point
           )=0;
   //-------------------------------------------------------------------------
   /// @brief method that returns the value of a pixel given its indices
   /// @param[in] i_x
   /// @param[in] i_y
   /// @param[in] i_z
   //-------------------------------------------------------------------------
   double getIntensity(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z
           );

   //-------------------------------------------------------------------------
   /// @brief method that adds the itensity of a point to the voxel area
   /// @param[in] i_point the position of the point
   /// @param[in] i_itensity the itensity of that point
   /// @note please note that after calling normalise you should not add any
   /// more itensities to the voxel area
   //-------------------------------------------------------------------------
   void addItensity(const gmtl::Vec3f &i_point,
           double i_intensity
           );

   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices of a the voxel that
   ///  contains the given point
   /// @param[in] i_x the x-coordinates of the point
   /// @param[in] i_y the y-coordinates of the point
   //-------------------------------------------------------------------------
   gmtl::Vec2i getIndices(double i_x, double i_y)const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the depth of the octree
   /// @note this method is only used at Series of Hashed Octrees
   //-------------------------------------------------------------------------
   virtual unsigned int getNoOfLevels()const;

   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices of a the voxel that
   ///  contains the given point
   /// @param[in] i_x the x-coordinates of the point
   /// @param[in] i_y the y-coordinates of the point
   /// @param[in] i_z the z-coordinates of the point
   //-------------------------------------------------------------------------
   gmtl::Vec3i getIndices(double i_x, double i_y, double i_z)const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices of a voxel given its index
   /// @param[in] i_index the index/key of the corresponding voxel
   //-------------------------------------------------------------------------
   gmtl::Vec3i getIndices(
           unsigned long int index
           )const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the x y z indices
   /// @note this function must only be called from VolumeSeriesOHAshedOctree
   /// @param[in] i_index the index/key of the corresponding voxel
   /// @param[in] i_level the level of the hashed octree
   //-------------------------------------------------------------------------
   virtual gmtl::Vec3i getIndices(
           unsigned long i_index,
           unsigned int i_level
           ) const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min Limits of the object
   //-------------------------------------------------------------------------
   const gmtl::Vec3f &getMinLimits()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max Limits of the object
   //-------------------------------------------------------------------------
   const gmtl::Vec3f &getMaxLimits()const;
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
   double getIsolevel(){return m_isolevel;}
   //-------------------------------------------------------------------------
   unsigned int getNoVoxelsX()const{return m_noOfVoxelsX;}
   unsigned int getNoVoxelsY()const{return m_noOfVoxelsY;}
   unsigned int getNoVoxelsZ()const{return m_noOfVoxelsZ;}
   //-------------------------------------------------------------------------
   /// @brief method that sets the volume
   /// @param[in] i_lenX the number of voxels in the x axis
   /// @param[in] i_lenY the number of voxels in the y axis
   /// @param[in] i_lenZ the number of voxels in the z axis
   //-------------------------------------------------------------------------
   void setVolumeSize(
           unsigned long int i_lenX,
           unsigned long int i_lenY,
           unsigned long int i_lenZ
           );
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
   /// @brief methdod that returns the length of the voxel
   //-------------------------------------------------------------------------
   double getVoxelLen()const{return m_lengthOfVoxel;}
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
   virtual unsigned int getIndex(
           const gmtl::Vec3f &i_point
           ) const;
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
           double i_intensity
           ) = 0;
   //-------------------------------------------------------------------------
   /// @brief method that returns the value of a pixel given its indices
   /// @param[in] i_x
   /// @param[in] i_y
   /// @param[in] i_z
   //-------------------------------------------------------------------------
   virtual double getIntensityTypeVol(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z
           )=0;

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
   double m_lengthOfVoxel;
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
   unsigned long int m_noOfVoxelsX;
   //-------------------------------------------------------------------------
   /// @brief the number of voxels in the y axis
   //-------------------------------------------------------------------------
   unsigned long int m_noOfVoxelsY;
   //-------------------------------------------------------------------------
   /// @brief the number of voxels in the z axis
   //-------------------------------------------------------------------------
   unsigned long int m_noOfVoxelsZ;
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
