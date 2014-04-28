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
    //--------------------------------------------------------------------------
    /// @brief default destructor
    //--------------------------------------------------------------------------
    ~MCwithIntegralImages();

private:
    typedef struct
    {
       unsigned int m_mins[3];
       unsigned int m_lens[3];
       unsigned int m_divisibles;
       unsigned int m_nextSide;
    }Cube;
    //--------------------------------------------------------------------------
    /// @brief recursive method that keep dividing every non empty sub-volumes
    /// and polygonises every non-empty cube that cannot be divided further
    /// @param[in] i_n next side to be divided, 2 for z, 1 for y and 0 for x
    /// @param[in] i_mins the min coordinates of the volume of our interest
    /// i_mins[0] for x-axis, i_mins[1] for y-axis & i_mins[2] for z-axis
    /// @param[in] i_dividableSides shows which sides are still dividable
    /// m_dividableSides&4>2=true if z side is still dividable
    /// m_dividableSides&2>1=true if y side is still dividable
    /// m_dividableSides&1  =true if x side is still dividable
    /// @param[in] i_lens the number of cubes in x,y,z directions accordingly
    //--------------------------------------------------------------------------
    void divideVolume(
            unsigned int i_dividableSides,
            const unsigned int i_n,
            const std::vector <unsigned int> &i_mins,
            const std::vector <unsigned int> &i_lens,
            GLData *i_glData
            );
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    /// @brief method that divides the volume without recursion
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------

 };

#endif // MCWITHINTEGRALIMAGES_H
