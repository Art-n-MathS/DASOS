#include "NonEmptyVoxelsMap.h"


//-----------------------------------------------------------------------------
NonEmptyVoxelsMap::NonEmptyVoxelsMap(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}


//-----------------------------------------------------------------------------
void NonEmptyVoxelsMap::createMap()
{
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         int h=0;
         for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
         {
            if(isInside(x,y,z))
            {
               h++;
            }
         }
         m_mapValues[getIndex(x,y)] = float(h);
      }
   }
}


//-----------------------------------------------------------------------------
NonEmptyVoxelsMap::~NonEmptyVoxelsMap()
{}

