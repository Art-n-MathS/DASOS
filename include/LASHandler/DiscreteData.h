#ifndef DISCRETEDATA_H
#define DISCRETEDATA_H

#include <vector>
#include "Object.h"

class DiscreteData
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_numOfRecords the number of points records
    //-------------------------------------------------------------------------
    DiscreteData(unsigned int i_numOfRecords);
    //-------------------------------------------------------------------------
    /// @brief method that adds a point to the descrete data
    /// @param[in] i_intensity the intensity of the point
    /// @param[in] i_point the position of the point
    //-------------------------------------------------------------------------
    void addPoint(unsigned short i_intensity, gmtl::Vec3f i_point);
    //-------------------------------------------------------------------------
    /// @brief method that voxelised the given area and returns an implicit
    /// object.
    /// @note the memory for the object i_obj should have already dynamically
    /// allocated, this class is not resposible for neithere creating or
    /// freeing that memory
    /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
    /// @param[in] i_obj object to be filled with the returns
    /// @return the implicit object which is a made by voxels
    //-------------------------------------------------------------------------
    Object *fillObject(
            Object *i_obj,
            const std::vector<double> &user_limits
            )const;
    //--------------------------------------------------------------------------
    /// @brief method that returns true if the pulse is inside the user limits
    /// @param[in] i_limits the limits of area of our interest
    /// @returns boolean whether the pulse is insise the given limits or not
    //--------------------------------------------------------------------------
    bool isInsideLimits(
            const std::vector<double> &i_user_limits,
            const gmtl::Vec3f &i_point
            )const;
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~DiscreteData();


private:
    //-------------------------------------------------------------------------
    /// @brief the position of all the points
    //-------------------------------------------------------------------------
    std::vector<gmtl::Vec3f> m_positions;
    //-------------------------------------------------------------------------
    /// @brief the corresponding intensities of the points
    //-------------------------------------------------------------------------
    std::vector<unsigned short> m_intensities;
    //-------------------------------------------------------------------------
    /// @brief the index of the next point to be added
    //-------------------------------------------------------------------------
    unsigned int m_next;

};

#endif // DISCRETEDATA_H
