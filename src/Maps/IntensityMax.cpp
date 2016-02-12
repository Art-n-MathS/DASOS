#include "IntensityMax.h"



//-----------------------------------------------------------------------------
IntensityMax::IntensityMax(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void IntensityMax::createMap()
{
    for(unsigned int x=0; x<m_noOfPixelsX; ++x)
    {
       for(unsigned int y=0; y<m_noOfPixelsY; ++y)
       {
          float max =0.0f;
          for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
          {
             if(isInside(x,y,z))
             {
                float intensity = m_object->getIntensity(x,y,z);
                if( intensity >max)
                {
                   max = intensity;
                }
             }
          }
          m_mapValues[getIndex(x,y)]= max;
       }
    }
}




//-----------------------------------------------------------------------------
IntensityMax::~IntensityMax()
{}
