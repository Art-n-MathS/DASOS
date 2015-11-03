#include "LastPatch.h"


//-----------------------------------------------------------------------------
LastPatch::LastPatch(
        const std::string i_name,
        Object *i_obj
        ):
    Map(i_name,i_obj)
{
}

//-----------------------------------------------------------------------------
void LastPatch::createMap()
{
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         int zz = 0, z;
         for(z=m_noOfPixelsZ-1; z>=0; --z)
         {
             if(isInside(x,y,z))
             {
                break;
             }
         }
         if(z<=0)
         {
            m_mapValues[getIndex(x,y)] = -0.0f;
         }
         else
         {
            for(; z>=0; z--)
            {
               if (isInside(x,y,z))
               {
                  zz++;
               }
               else
               {
                  break;
               }
            }
            m_mapValues[getIndex(x,y)] = zz;
         }
      }
   }
}

//-----------------------------------------------------------------------------
LastPatch::~LastPatch()
{}
