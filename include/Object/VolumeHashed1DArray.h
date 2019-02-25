#ifndef VOLUMEHASHED1DARRAY_H
#define VOLUMEHASHED1DARRAY_H

#include "Volume.h"
#include <unordered_map>

class VolumeHashed1DArray :public Volume
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    //--------------------------------------------------------------------------
    VolumeHashed1DArray(
            double i_voxelLength,
            const std::vector<double> &i_userLimits,
            const std::string &i_type
            );
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    VolumeHashed1DArray(const std::string &i_filename);
    /// @brief gets the function value of the object given a point
    /// @param[in] i_point the given point
    //-------------------------------------------------------------------------
    double functionValue(
            const gmtl::Vec3f &i_point
            );
    //-------------------------------------------------------------------------
    /// @brief method that returns true when the given point is inside a
    /// voxel whose intensity is higher than the isolevel
    //-------------------------------------------------------------------------
    bool isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z);
    //-------------------------------------------------------------------------
    /// @brief exports all the information of the 3D volume into a file
    /// it is used to speed up testing, especially using valgrind
    /// @param[in] i_compression true if enabled and false otherwise
    /// @param[in] i_filename the name of file to be exported
    //-------------------------------------------------------------------------
    void exportToFile(std::string i_filename, bool i_compression);

    //-------------------------------------------------------------------------
    /// @brief method that normalises the values of the voxels
    /// makes everything belong to the limits [-1,1]
    //-------------------------------------------------------------------------
    void normalise();
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~VolumeHashed1DArray();

 private:

    //-------------------------------------------------------------------------
    /// @brief this function was generated for testing. It compares 2 objects
    /// and prints the number of corresponding voxels that do not have the
    /// same intensity value
    /// @notes commented, to work m_intensities and m_noOfReturnsPerVoxel
    /// need to be added to the base class
    //-------------------------------------------------------------------------
    void compare(Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief
    //-------------------------------------------------------------------------
    void readObjectFromFile(const std::string &i_filename, bool evaluation);
    //-------------------------------------------------------------------------
    /// @brief method that adds the itensity of a point to the voxel area
    /// @param[in] i_point the position of the point
    /// @param[in] i_itensity the itensity of that point
    /// @note please note that after calling normalise you should not add any
    /// more itensities to the voxel area
    //-------------------------------------------------------------------------
    void addItensityTypeVol(const gmtl::Vec3f &i_point,
            double i_intensity
            );
    //-------------------------------------------------------------------------
    /// @brief method that returns the value of a pixel given its indices
    /// @param[in] i_x
    /// @param[in] i_y
    /// @param[in] i_z
    //-------------------------------------------------------------------------
    double getIntensityTypeVol(
            unsigned int i_x,
            unsigned int i_y,
            unsigned int i_z
            );

    std::unordered_map<unsigned long int, double> m_intensities;

    std::unordered_map<unsigned long int, unsigned int> m_noOfSamplesPerVoxel;

};

#endif // VOLUMEHASHED1DARRAY_H
