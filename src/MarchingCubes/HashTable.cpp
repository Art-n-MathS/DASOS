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
std::string HashTable::convert (float number)
{
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
unsigned int HashTable::getIndex(const gmtl::Vec3f &i_vertex)
{
   std::string input = convert(i_vertex[0]) + " " + convert(i_vertex[1])
           + " " + convert(i_vertex[2]);

   std::unordered_map<std::string,unsigned int>::const_iterator got =
           mymap.find(input);

   if(got == mymap.end())
   {
      std::pair<std::string, unsigned int> pair(input,m_currentIndex);
      m_currentIndex++;
      mymap.insert(pair);
      m_glData->addVertex(gmtl::Vec3f(i_vertex[0],i_vertex[1],i_vertex[2]));
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
