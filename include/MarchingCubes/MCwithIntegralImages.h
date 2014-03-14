#ifndef MCWITHINTEGRALIMAGES_H
#define MCWITHINTEGRALIMAGES_H

#include "MarchingCubes.h"

class MCwithIntegralImages : public MarchingCubes
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_obj object to be polygonised
    /// @param[in] i_x the number of cubes in the x-axis
    //--------------------------------------------------------------------------
    MCwithIntegralImages(Object *i_obj, unsigned int i_x);

    //--------------------------------------------------------------------------
    /// @brief method that polygonises the object and save its vertices into the
    /// gl data
    /// @param[in&out] i_glData the normals, vertices, indices,uvs of the object
    //--------------------------------------------------------------------------
    void computeVertices(GLData *i_glData);

 };

#endif // MCWITHINTEGRALIMAGES_H
