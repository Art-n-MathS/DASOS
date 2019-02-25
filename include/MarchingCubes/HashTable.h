#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include "GLData.h"
#include <unordered_map>


class HashTable
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_glData the object that we are interested in a quick way of
   /// searching the vertices and sorting the indices
   //--------------------------------------------------------------------------
   HashTable(GLData *i_glData);

   HashTable();

   void setGLData(GLData *i_glData);
   //--------------------------------------------------------------------------
   /// @brief method that returns the index of a given vertex
   /// If the vertex does not exist then the vertex is added into the glData
   /// @param[in] _vertex is a vertex of the object
   /// @returns the index of the given vertex
   //--------------------------------------------------------------------------
   unsigned int getIndex(const gmtl::Vec3f &i_vertex);
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~HashTable();

private:
   //--------------------------------------------------------------------------
   /// @brief method that converts a double number to string
   /// @param[in] i_number number to be converted
   /// @return the string
   //--------------------------------------------------------------------------
   std::string convert (double i_number);
   //--------------------------------------------------------------------------
   /// @brief the glData used to store the vertices inside. this is only a
   /// pointer to the data, used to avoid copied the data afterwards
   //--------------------------------------------------------------------------
   GLData *m_glData;
   //--------------------------------------------------------------------------
   /// @brief the index of the next vertex to be added
   //--------------------------------------------------------------------------
   unsigned int m_currentIndex;
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   std::unordered_map<std::string,unsigned int> mymap;



};

#endif // HASHTABLE_H
