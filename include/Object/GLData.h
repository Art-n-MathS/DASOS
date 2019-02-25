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

#include <gmtl/Vec.h>
#include <vector>
#include <string>

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
   void exportToObj(std::string _name,
           const std::string i_igmFileName,
                    const std::string i_bilFileName,
           const std::vector<unsigned short> i_bands
           );
   //------------------------------------------------------------------------
   /// @brief method that adds vertex and gets its index in return
   /// @param[in] i_vertex vertex to be added
   /// @return the index of the vertex
   //------------------------------------------------------------------------
   unsigned int addVertex(const gmtl::Vec3f &i_vertex);
   //------------------------------------------------------------------------
   /// @brief method that returns a vertex given its index
   /// @param[in] i_index the index of the vertex
   /// @return the vertex of our interest
   //------------------------------------------------------------------------
   gmtl::Vec3f getVertex(unsigned int i_index);
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

   //---------------------------------------------------------------------------
   /// @brief method that exports the given 3 bands into an image that can be
   /// used as a texture that matches the UVs of the .obj file
   /// @param[in] i_fName the name of the bil file
   /// @param[in] i_name the name of the image to be saved
   /// @param[in] i_bands the bands of our interest
   //---------------------------------------------------------------------------
   void exportHyperToImage(const std::string &i_fName,
                      std::string i_name,
                      const std::vector<unsigned short int> &i_bands);


private:
   //-------------------------------------------------------------------------
   /// @brief the vertices of an object
   //-------------------------------------------------------------------------
   std::vector<double> m_vertices;
   //-------------------------------------------------------------------------
   /// @brief the indices of an object
   //-------------------------------------------------------------------------
   std::vector<int> m_indices;
   //-------------------------------------------------------------------------
   /// @brief the normals of an object
   //-------------------------------------------------------------------------
   std::vector<double> m_normals;
   //-------------------------------------------------------------------------
   /// @brief the texture coordinates (UVs) of an object
   //-------------------------------------------------------------------------
   std::vector<double> m_UVs;
   //-------------------------------------------------------------------------
   /// @brief the texture coordinates used for painting validation data and
   /// for showing the classification results
   //-------------------------------------------------------------------------
   std::vector<double> m_classUVs;
   //-------------------------------------------------------------------------
   /// @brief lower limits
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_minLimits;
   //-------------------------------------------------------------------------
   /// @brief max Limits
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_maxLimits;






};

#endif // GLDATA_H
