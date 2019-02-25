#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "Volume.h"
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
   MarchingCubes(Volume *i_obj, unsigned int i_x);
   //--------------------------------------------------------------------------
   /// @brief method that polygonises the object and returns its vertices,
   /// indices, normals etc
   /// @note memory is dynamic allocated and by calling the function you are
   /// responsible for freeing the memory afterwards
   //--------------------------------------------------------------------------
   void createPolygonisedObject(GLData *i_gldata);
   //--------------------------------------------------------------------------
   /// @param[in] default destructor
   //--------------------------------------------------------------------------
   virtual ~MarchingCubes();



protected:
   //--------------------------------------------------------------------------
   /// @brief method that polygonises the cube with min coordinates x,y,z
   /// @param[in] i_x the min x coordinate of the cube to be polygonised
   /// @param[in] i_y the min y coordinate of the cube to be polygonised
   /// @param[in] i_z the min z coordinate of the cube to be polygonised
   /// @param[in] i_isolevel the boundary of the object
   //--------------------------------------------------------------------------
   virtual void polygoniseXYZ(
           const unsigned int i_x,
           const unsigned int i_y,
           const unsigned int i_z,
           const double i_isolevel,
           GLData *i_glData
           );
   //--------------------------------------------------------------------------
   /// @brief method that calculates the position where a surfaces cuts an edge
   /// between two vertices using linear interpolation
   /// @param[in] isolevel
   /// @param[in] p1 the first point
   /// @param[in] p2 the seconde point
   /// @param[in] valp1 the value returned for point 1
   /// @param[in] valp2 the value returned for point 2
   //--------------------------------------------------------------------------
   gmtl::Vec3f VertexInterp(
           double isolevel,
           const gmtl::Vec3f &p1,
           const gmtl::Vec3f &p2,
           const double valp1,
           const double valp2
           );
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void Polygonise(const std::vector<gmtl::Vec3f> &points,
           double isolevel,
           GLData *i_glData
           , std::vector<double> &values);
   //--------------------------------------------------------------------------
   /// @brief method that returns the arithmetic position of a voxel in the
   /// space given its indices
   //--------------------------------------------------------------------------
   gmtl::Vec3f getXYZ(unsigned int i_x, unsigned int i_y, unsigned int i_z);
   //--------------------------------------------------------------------------
   /// @brief method that polygonises the object and save its vertices into the
   /// gl data
   /// @param[in&out] i_glData the normals, vertices, indices,uvs of the object
   //--------------------------------------------------------------------------
   virtual void computeVertices(GLData *glData);
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void computeNormals(GLData *i_glData);
   //---------------------------------------------------------------------------
   /// @brief method that generates the m_classUVs, which are used to project
   /// classification results and paint validation data
   //--------------------------------------------------------------------------
   void generateClassUVs(GLData *i_glData);
   //--------------------------------------------------------------------------
   /// @brief method that returns the index of a given vertex. If the index
   /// does not exist in glData then it is first added and then the index is
   /// returned.
   /// @param[in] i_glData where vertices and indices are saved
   /// @param[in] i_vertex the vertex of our interest
   //--------------------------------------------------------------------------
   unsigned int getIndex(GLData *i_glData, const gmtl::Vec3f &i_vertex);


   //--------------------------------------------------------------------------
   /// @brief object to be polygonised
   //--------------------------------------------------------------------------
   Volume *m_obj;
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
   gmtl::Vec3f m_maxLimits;
   //--------------------------------------------------------------------------
   /// @brief the min values of the object
   //--------------------------------------------------------------------------
   gmtl::Vec3f m_minLimits;

   HashTable m_hashTable;

};

#endif // MARCHINGCUBES_H
