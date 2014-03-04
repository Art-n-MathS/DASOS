#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "Object.h"
#include "GLData.h"
#include "HashTable.h"

class MarchingCubes
{

public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_obj object to be polygonised
   /// @param[in] i_x the number of cubes in the x-axis
   //--------------------------------------------------------------------------
   MarchingCubes(Object *i_obj, unsigned int i_x);
   //--------------------------------------------------------------------------
   /// @brief method that polygonises the object and returns its vertices,
   /// indices, normals etc
   /// @note memory is dynamic allocated and by calling the function you are
   /// responsible for freeing the memory afterwards
   //--------------------------------------------------------------------------
   GLData *createPolygonisedObject();
   //--------------------------------------------------------------------------
   /// @param[in] default destructor
   //--------------------------------------------------------------------------
   ~MarchingCubes();



private:
   //--------------------------------------------------------------------------
   /// @brief method that calculates the position where a surfaces cuts an edge
   /// between two vertices using linear interpolation
   /// @param[in] isolevel
   /// @param[in] p1 the first point
   /// @param[in] p2 the seconde point
   /// @param[in] valp1 the value returned for point 1
   /// @param[in] valp2 the value returned for point 2
   //--------------------------------------------------------------------------
   ngl::Vec3 VertexInterp(
           double isolevel,
           const ngl::Vec3 &p1,
           const ngl::Vec3 &p2,
           const double valp1,
           const double valp2
           );
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void Polygonise(
           const std::vector<ngl::Vec3> &points,
           double isolevel,
           GLData *i_glData
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns the arithmetic position of a voxel in the
   /// space given its indices
   //--------------------------------------------------------------------------
   ngl::Vec3 getXYZ(unsigned int i_x, unsigned int i_y, unsigned int i_z);
   //--------------------------------------------------------------------------
   /// @brief method that polygonises the object and save its vertices into the
   /// gl data
   /// @param[in&out] i_glData the normals, vertices, indices,uvs of the object
   //--------------------------------------------------------------------------
   void computeVertices(GLData *glData);
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void computeNormals(GLData *i_glData);
   //--------------------------------------------------------------------------
   /// @brief method that returns the index of a given vertex. If the index
   /// does not exist in glData then it is first added and then the index is
   /// returned.
   /// @param[in] i_glData where vertices and indices are saved
   /// @param[in] i_vertex the vertex of our interest
   //--------------------------------------------------------------------------
   unsigned int getIndex(GLData *i_glData, const ngl::Vec3 &i_vertex);
   //--------------------------------------------------------------------------
   /// @brief object to be polygonised
   //--------------------------------------------------------------------------
   Object *m_obj;
   //--------------------------------------------------------------------------
   /// @brief number of cubes in x-axis
   //--------------------------------------------------------------------------
   unsigned int m_numOfCubsX;
   //--------------------------------------------------------------------------
   /// @brief number of cubes in y-axis
   //--------------------------------------------------------------------------
   unsigned int m_numOfCubsY;
   //--------------------------------------------------------------------------
   /// @brief number of cubes in z-axis
   //--------------------------------------------------------------------------
   unsigned int m_numOfCubsZ;
   //--------------------------------------------------------------------------
   /// @brief the min and max values define the bounding box of the object
   /// @brief the max values of the object
   //--------------------------------------------------------------------------
   ngl::Vec3 m_maxLimits;
   //--------------------------------------------------------------------------
   /// @brief the min values of the object
   //--------------------------------------------------------------------------
   ngl::Vec3 m_minLimits;

   HashTable m_hashTable;

};

#endif // MARCHINGCUBES_H
