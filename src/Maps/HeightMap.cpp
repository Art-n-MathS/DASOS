#include "HeightMap.h"

//-----------------------------------------------------------------------------
HeightMap::HeightMap(
        const std::string i_name,
        Object *i_obj
        ):
    Map(i_name,i_obj)
{
}


/// TO DO: This is a prober thickness map and not a heigh map
//-----------------------------------------------------------------------------
void HeightMap::createMap()
{
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         int z1 = 0 , z2 = 0;
         for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
         {
            if(!isInside(x,y,z))
            {
               z1++;
            }
            else
            {
               break;
            }
         }
         if((unsigned int) z1!= m_noOfPixelsZ)
         {
            for(int z=m_noOfPixelsZ-1; z>=0; --z)
            {
                if(!isInside(x,y,z))
                {
                   z2++;
                }
                else
                {
                   break;
                }
            }
            m_mapValues[getIndex(x,y)] = float(m_noOfPixelsZ - z1 - z2);
         }
         else
         {
            m_mapValues[getIndex(x,y)] = 0;
         }
      }
   }
}



//-----------------------------------------------------------------------------
HeightMap::~HeightMap()
{}
