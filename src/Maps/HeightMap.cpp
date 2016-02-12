#include "HeightMap.h"


//-----------------------------------------------------------------------------
HeightMap::HeightMap(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void HeightMap::createMap()
{
    for(unsigned int x=0; x<m_noOfPixelsX; ++x)
    {
       for(unsigned int y=0; y<m_noOfPixelsY; ++y)
       {
          unsigned int z1 = 0;
          for(int z=m_noOfPixelsZ-1; z>=0; --z)
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
          if(z1==m_noOfPixelsZ)
          {
             m_mapValues[getIndex(x,y)] =-0.0f;
          }
          else
          {
             m_mapValues[getIndex(x,y)] = m_object->getMaxLimits()[2]-float(z1)*m_object->getVoxelLen();
          }
       }
    }
}


//-----------------------------------------------------------------------------
HeightMap::~HeightMap()
{}
