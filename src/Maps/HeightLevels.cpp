#include "HeightLevels.h"
#include <algorithm>


//-----------------------------------------------------------------------------
HeightLevels::HeightLevels(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}


//-----------------------------------------------------------------------------
void HeightLevels::createMap()
{
   std::string oname = m_name;
   // Loop through all the voxels and generate the metric of interest
   // the variable m_noOfPixelsX, m_noOfPixelsY and m_noOfPixelsZ gives you
   // the the number of voxels in x,y,z axis respectively
   std::vector<unsigned int> Zs;
   Zs.resize(m_noOfPixelsX*m_noOfPixelsY,0);
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
          for(int zt=m_noOfPixelsZ-1; zt>=0; --zt)
          {
            if(!isInside(x,y,zt))
            {
               Zs[x+y*m_noOfPixelsX]++;
            }
            else
            {
               break;
            }
         }
      }
   }
   unsigned int temp =*min_element(std::begin(Zs), std::end(Zs));
   unsigned int zmax (m_noOfPixelsZ-temp);
   std::cout << temp << " " <<m_noOfPixelsZ << " " << zmax << "   ===============================\n";
   for(unsigned int z=0; z<zmax; ++z)
   {

      for(unsigned int x=0; x<m_noOfPixelsX; ++x)
      {
         for(unsigned int y=0; y<m_noOfPixelsY; ++y)
         {
             unsigned int z1 = 0;
               for(int zl=z-1; zl>=0; --zl)
               {
                  if(!isInside(x,y,zl))
                  {
                     z1++;
                  }
                  else
                  {
                     break;
                  }
               }
               if(z1==z)
               {
                  m_mapValues[getIndex(x,y)] =-0.0f;
               }
               else
               {
//                  m_mapValues[getIndex(x,y)] =  m_object->getMaxLimits()[2]-float(z1-z)*m_object->getVoxelLen();

//                  m_mapValues[getIndex(x,y)] =  float(zmax)*m_object->getVoxelLen()
//                                                -float(z1  )*m_object->getVoxelLen()
//                                                -float(z   )*m_object->getVoxelLen();

                  m_mapValues[getIndex(x,y)] = (m_object->getMinLimits()[2]+float(z-z1)*m_object->getVoxelLen());
               }

         }
      }
      m_name = oname + std::to_string(z);
      this->saveAsc();
   }
}

//-----------------------------------------------------------------------------
HeightLevels::~HeightLevels()
{}
