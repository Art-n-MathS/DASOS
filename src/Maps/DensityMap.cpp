#include "DensityMap.h"

//-----------------------------------------------------------------------------
DensityMap::DensityMap(
        const std::string i_name,
        Object *i_obj
        ):
    Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void DensityMap::createMap()
{
    std::vector<float> m_mapValues(m_noOfPixelsX*m_noOfPixelsY);
    unsigned int numOfvoxels=0;
    float value;

    for(unsigned int y=0; y<m_noOfPixelsY; ++y)
    {
       for(unsigned int x=0; x<m_noOfPixelsX; ++x)
       {
          m_mapValues[x+y*m_noOfPixelsX] = 0;
          numOfvoxels =0;
          for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
          {
              value = getIntensity(x,y,z);
             if(isInside(x,y,z))
             {
                numOfvoxels++;
                m_mapValues[x+y*m_noOfPixelsX]+=((value+100.0f)/100.0f);
             }
          }
          if(numOfvoxels!=0)
          {
             m_mapValues[x+y*m_noOfPixelsX]=m_mapValues[x+y*m_noOfPixelsX]/numOfvoxels/2.0f*255.0f;
             std::cout << m_mapValues[x+y*m_noOfPixelsX] << "\n";
          }
       }
    }
}

//-----------------------------------------------------------------------------
DensityMap::~DensityMap()
{}
