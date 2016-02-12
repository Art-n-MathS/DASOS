#include "FirstPatch.h"

//-----------------------------------------------------------------------------
FirstPatch::FirstPatch(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}


//-----------------------------------------------------------------------------
void FirstPatch::createMap()
{
    for(unsigned int x=0; x<m_noOfPixelsX; ++x)
    {
       for(unsigned int y=0; y<m_noOfPixelsY; ++y)
       {
          unsigned int z1 = 0 , z2 = 0;
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
             while(isInside(x,y,z1) && z1<m_noOfPixelsZ)
             {
                 z2++;
                 z1++;
             }
             m_mapValues[getIndex(x,y)] = float(z2)*m_object->getVoxelLen();
          }
          else
          {
             m_mapValues[getIndex(x,y)] = -0.0f;
          }
          z2=0;
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
          }
          if(z2==m_noOfPixelsZ-z1 && m_noOfPixelsZ-z2-z1<3)
          {
             m_mapValues[getIndex(x,y)]=-0.0f;
          }
       }
    }
}



//-----------------------------------------------------------------------------
FirstPatch::~FirstPatch()
{}
