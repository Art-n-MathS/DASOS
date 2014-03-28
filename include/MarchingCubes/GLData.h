#ifndef GLDATA_H
#define GLDATA_H

//-----------------------------------------------------------------------------
/// @file GLData.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 14/10/13
/// @class GLData
/// @brief in this class all the vertices, normals, indices and uvs are
/// saved in an order that can be easily rendered with openGL.
//-----------------------------------------------------------------------------

#include <ngl/Types.h>
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <vector>

class GLData
{
   friend class MarchingCubes;

public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   //-------------------------------------------------------------------------
   GLData();
   //-------------------------------------------------------------------------
   /// @brief default copy constructor
   /// @param[in] i_glData data to be copied
   //------------------------------------------------------------------------
   GLData(const GLData &i_glData);
   //------------------------------------------------------------------------
   /// @brief method that sets the data
   //------------------------------------------------------------------------
   void setGLData(const GLData &i_glData);
   //------------------------------------------------------------------------
   /// @brief method that exports data to as an .obj file
   /// @param[in] i_name the name of the file that will be saved
   //------------------------------------------------------------------------
   void exportToObj(std::string _name)const;
   //------------------------------------------------------------------------
   /// @brief method that adds vertex and gets its index in return
   /// @param[in] i_vertex vertex to be added
   /// @return the index of the vertex
   //------------------------------------------------------------------------
   unsigned int addVertex(const ngl::Vec3 &i_vertex);
   //------------------------------------------------------------------------
   /// @brief method that returns a vertex given its index
   /// @param[in] i_index the index of the vertex
   /// @return the vertex of our interest
   //------------------------------------------------------------------------
   ngl::Vec3 getVertex(unsigned int i_index);
   //------------------------------------------------------------------------
   /// @brief method that creates UVs for the model the name of the bil file
   /// @note please note that this method is for raster data
   /// @param[in] i_bilFilename the name of the bil file
   //------------------------------------------------------------------------
   void createUVsBIL(const std::string &i_bilFilename);
   //------------------------------------------------------------------------
   /// @brief method that creates UVs for the model given the igm file from
   /// the original hyperspectral data
   /// @param[in] i_igmFile the name of the igm file
   //------------------------------------------------------------------------
   void createUVsIGM(const std::string &i_igmFile);
   //------------------------------------------------------------------------
   /// @brief default destructor
   //------------------------------------------------------------------------
   ~GLData();


//private:
   //-------------------------------------------------------------------------
   /// @brief the vertices of an object
   //-------------------------------------------------------------------------
   std::vector<GLfloat> m_vertices;
   //-------------------------------------------------------------------------
   /// @brief the indices of an object
   //-------------------------------------------------------------------------
   std::vector<GLuint> m_indices;
   //-------------------------------------------------------------------------
   /// @brief the normals of an object
   //-------------------------------------------------------------------------
   std::vector<GLfloat> m_normals;
   //-------------------------------------------------------------------------
   /// @brief the texture coordinates (UVs) of an object
   //-------------------------------------------------------------------------
   std::vector<GLfloat> m_UVs;
   //-------------------------------------------------------------------------
   /// @brief lower limits
   //-------------------------------------------------------------------------
   ngl::Vec3 m_minLimits;
   //-------------------------------------------------------------------------
   /// @brief max Limits
   //-------------------------------------------------------------------------
   ngl::Vec3 m_maxLimits;


   //-------------------------------------------------------------------------
   /// @brief the lower limits of the hyperspectral data
   //-------------------------------------------------------------------------
   ngl::Vec2 m_hyperMinLimits;
   //-------------------------------------------------------------------------
   /// @brief the higher limits of the hyperspectral data
   //-------------------------------------------------------------------------
   ngl::Vec2 m_hyperMaxLimits;
};

#endif // GLDATA_H
