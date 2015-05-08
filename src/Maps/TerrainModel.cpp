#include "TerrainModel.h"


//-----------------------------------------------------------------------------
TerrainModel::TerrainModel(
        const std::string &i_name,
        Object *i_obj
        ): Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void TerrainModel::createMap()
{
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
          unsigned int z1 = 0;
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
          if(z1==m_noOfPixelsZ)
          {
             m_mapValues[getIndex(x,y)] = 0.0f;
          }
          else
          {
             m_mapValues[getIndex(x,y)] = float(z1);
          }
      }
   }
}
