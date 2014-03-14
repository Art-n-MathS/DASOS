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

#include <ngl/Vec3.h>
#include "GLData.h"
#include "IntegralVolumes.h"

class Object
{
    friend class Map;
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    Object(
            unsigned int i_x,
            const std::vector<double> &i_user_limits
            );
    //-------------------------------------------------------------------------
    /// @brief gets the function value of the object given a point
    /// @param[in] i_point the given point
    //-------------------------------------------------------------------------
    double functionValue(const ngl::Vec3 &i_point);
    //-------------------------------------------------------------------------
    /// @brief method that adds the itensity of a point to the voxel area
    /// @param[in] i_point the position of the point
    /// @param[in] i_itensity the itensity of that point
    /// @note please note that after calling normalise you should not add any
    /// more itensities to the voxel area
    //-------------------------------------------------------------------------
    void addItensity(
            const ngl::Vec3 &i_point,
            float i_intensity
            );
    //-------------------------------------------------------------------------
    /// @brief method that returns the min Limits of the object
    //-------------------------------------------------------------------------
    const ngl::Vec3 getMinLimits()const;
    //-------------------------------------------------------------------------
    /// @brief method that returns the max Limits of the object
    //-------------------------------------------------------------------------
    const ngl::Vec3 getMaxLimits()const;
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
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~Object();


protected:
    //-------------------------------------------------------------------------
    /// @brief method that returns the index of a voxel given its position in
    /// the space
    /// @param[in] i_x the x position of the voxel
    /// @param[in] i_y the y position of the voxel
    /// @param[in] i_z the z position of the voxel
    //-------------------------------------------------------------------------
    unsigned int getIndex(
            const ngl::Vec3 &i_point
            ) const;
    //-------------------------------------------------------------------------
    /// @brief method that returns the centre of a voxel given its x y z index
    /// @param[in] i_x the position of the voxel of our interest in the x axis
    /// @param[in] i_y the position of the voxel of our interest in the y axis
    /// @param[in] i_z the position of the voxel of our interest in the z axis
    //-------------------------------------------------------------------------
    ngl::Vec3 getCentreOfVoxel(
            int i_x,
            int i_y,
            int i_z
            )const;

    //-------------------------------------------------------------------------
    /// @brief the min limits of the object
    //-------------------------------------------------------------------------
    ngl::Vec3 m_lowerLimits;
    //-------------------------------------------------------------------------
    /// @brief the max limits of the object
    //-------------------------------------------------------------------------
    ngl::Vec3 m_higherLimits;
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

    std::vector<float> m_weightOfReturnsPerVoxel;
    //-------------------------------------------------------------------------
    /// @brief the distance between the limits
    //-------------------------------------------------------------------------
    ngl::Vec3 m_dis;
    //-------------------------------------------------------------------------
    /// @brief the length of voxel (voxels are cubes)
    //-------------------------------------------------------------------------
    float m_lengthOfVoxel;
    //-------------------------------------------------------------------------
    /// @brief the noise level
    //-------------------------------------------------------------------------
    double m_noiseLevel;
    //-------------------------------------------------------------------------
    /// @brief indicates if the data's been inserted into the integral volume
    //-------------------------------------------------------------------------
    bool m_isIntegralVolume;
    //-------------------------------------------------------------------------
    /// @brief all the data inserted into an integral volume
    //-------------------------------------------------------------------------
    IntegralVolumes *m_integralVolume;

};

#endif // OBJECT_H
