#ifndef OBJECT_H
#define OBJECT_H

//-----------------------------------------------------------------------------
/// @file Object.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 14/10/13
/// @class Object
/// @brief this class represents an implicit object.
//-----------------------------------------------------------------------------

#include <gmtl/Vec.h>
#include "GLData.h"
#include "IntegralVolumes.h"

class Object
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    Object(
            float i_voxelLength,
            const std::vector<double> &i_user_limits
            );
    //-------------------------------------------------------------------------
    /// @brief gets the function value of the object given a point
    /// @param[in] i_point the given point
    //-------------------------------------------------------------------------
    double functionValue(const gmtl::Vec3f &i_point);
    //-------------------------------------------------------------------------
    /// @brief method that returns the value of a pixel given its indices
    /// @param[in] i_x
    /// @param[in] i_y
    /// @param[in] i_z
    //-------------------------------------------------------------------------
    float getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
    {
       return m_intensities[i_x+i_y*m_noOfVoxelsX+
               i_z*m_noOfVoxelsX*m_noOfVoxelsY];
    }

    bool isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z)
    {
       return m_intensities[i_x+i_y*m_noOfVoxelsX+
               i_z*m_noOfVoxelsX*m_noOfVoxelsY]>m_isolevel;
    }
    //-------------------------------------------------------------------------
    /// @brief method that returns the x y z indices of a the voxel that
    ///  contains the given point
    /// @param[in] i_x the x-coordinates of the point
    /// @param[in] i_y the y-coordinates of the point
    /// @param[in] i_z the z-coordinates of the point
    //-------------------------------------------------------------------------
    gmtl::Vec2i getIndices(float i_x, float i_y)const;
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
    const std::vector<double> &getIntensities()const;
    //-------------------------------------------------------------------------
    /// @brief method that sets the threshold of the object
    /// @param[in] i_thres the new threashold that defines the surface
    //-------------------------------------------------------------------------
    void setIsolevel(double i_thres);
    //-------------------------------------------------------------------------
    /// @brief method that normalises the values of the voxels
    /// makes everything belong to the limits [-1,1]
    //-------------------------------------------------------------------------
    void normalise();
    //-------------------------------------------------------------------------
    /// @brief method that inserts the values of the object inside the integral
    /// volume
    //-------------------------------------------------------------------------
    void insertIntoIntegralVolume();
    //-------------------------------------------------------------------------
    /// @brief all the data inserted into an integral volume
    //--------------------------------------------------------------------------------------------------------------------temporarly public
    IntegralVolumes *m_integralVolume;
    //-------------------------------------------------------------------------
    /// @brief the length of voxel (voxels are cubes)
    //-------------------------------------------------------------------------
    float m_lengthOfVoxel;
    //-------------------------------------------------------------------------
    /// @brief mehtod that returns the isolevel of the object
    //-------------------------------------------------------------------------
    float getIsolevel(){return m_isolevel;}
    //-------------------------------------------------------------------------
    unsigned int getNoVoxelsX()const{return m_noOfVoxelsX;}
    unsigned int getNoVoxelsY()const{return m_noOfVoxelsY;}
    unsigned int getNoVoxelsZ()const{return m_noOfVoxelsZ;}
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~Object();


    //-------------------------------------------------------------------------
    /// @brief this function was generated for testing. It compares 2 objects
    /// and prints the number of corresponding voxels that do not have the
    /// same intensity value
    //-------------------------------------------------------------------------
    void compare(Object *i_obj);
    //-------------------------------------------------------------------------
    /// @brief exports all the information of the 3D volume into a file
    /// it is used to speed up testing, especially using valgrind
    /// @param[in] i_filename the name of file to be exported
    //-------------------------------------------------------------------------
    void exportToFile(std::string i_filename);
    //-------------------------------------------------------------------------
    /// @brief testing constructor - used to read data exported from a file
    /// to speed up testing
    //-------------------------------------------------------------------------
    Object(const std::string &i_filename);
    //-------------------------------------------------------------------------
    /// @brief testing constructor - used to read data exported from a file
    /// to speed up testing
    //-------------------------------------------------------------------------
    Object(const std::string &i_filename,bool evaluation);
    //-------------------------------------------------------------------------
    /// @brief methdod that returns the length of the voxel
    //-------------------------------------------------------------------------
    float getVoxelLen()const{return m_lengthOfVoxel;}


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
    void readObjectFromFile(const std::string &i_filename, bool evaluation);

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
    /// @brief the itensity of each voxel
    //-------------------------------------------------------------------------
    std::vector<double> m_intensities;
    //-------------------------------------------------------------------------
    /// @brief number of returns per voxel
    //-------------------------------------------------------------------------
    std::vector<unsigned short int> m_noOfReturnsPerVoxel;

//    std::vector<float> m_weightOfReturnsPerVoxel;
    //-------------------------------------------------------------------------
    /// @brief the distance between the limits
    //-------------------------------------------------------------------------
    gmtl::Vec3f m_dis;

    //-------------------------------------------------------------------------
    /// @brief the noise level
    //-------------------------------------------------------------------------
    double m_noiseLevel;

    unsigned countIncludedPoints;
    unsigned countNotInclPoints;
};

#endif // OBJECT_H
