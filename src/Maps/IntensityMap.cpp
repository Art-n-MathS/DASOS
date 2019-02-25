#include "IntensityMap.h"


//-----------------------------------------------------------------------------
IntensityMap::IntensityMap(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void IntensityMap::createMap()
{
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         int z1 = 0 ;
         for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
         {
            if(isInside(x,y,z))
            {
               m_mapValues[getIndex(x,y)] +=m_object->getIntensity(x,y,z);
               z1++;
            }
         }
         if(z1>0)
         {
            m_mapValues[getIndex(x,y)] /= float(z1);
         }
      }
   }
}




//-----------------------------------------------------------------------------
IntensityMap::~IntensityMap()
{}
