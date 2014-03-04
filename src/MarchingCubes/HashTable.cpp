#include "HashTable.h"
#include <sstream>

//-----------------------------------------------------------------------------
HashTable::HashTable(
        GLData *i_glData
        ):
    m_glData(i_glData),
    m_currentIndex(0)
{
}

HashTable::HashTable():m_currentIndex(0)
{}

//-----------------------------------------------------------------------------
std::string HashTable::convert (float number){
    std::stringstream buff;
    buff.precision(6);
     buff<<number;
    return buff.str();
}

//-----------------------------------------------------------------------------
void HashTable::setGLData(GLData *i_glData)
{
   m_glData = i_glData;
}

//-----------------------------------------------------------------------------
unsigned int HashTable::getIndex(
        const ngl::Vec3 &i_vertex
        )
{
   std::string input = convert(i_vertex.m_x) + " " + convert(i_vertex.m_y)
           + " " + convert(i_vertex.m_z);

   std::unordered_map<std::string,unsigned int>::const_iterator got =
           mymap.find(input);

   if(got == mymap.end())
   {
      std::pair<std::string, unsigned int> pair(input,m_currentIndex);
      m_currentIndex++;
      mymap.insert(pair);
      m_glData->addVertex(i_vertex);
      return m_currentIndex-1;
   }
   else
   {
      return got->second;
   }
}


//-----------------------------------------------------------------------------
HashTable::~HashTable()
{}
