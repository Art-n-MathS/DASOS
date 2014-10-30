#include "Object.h"
#include <limits>
#include <assert.h>
#include <QImage>
#include <iostream>

//-----------------------------------------------------------------------------
Object::Object(unsigned int i_x, const std::vector<double> &i_userLimits):
    m_integralVolume(0),
    m_lowerLimits(gmtl::Vec3f(i_userLimits[3],i_userLimits[1],i_userLimits[5])),
    m_higherLimits(gmtl::Vec3f(i_userLimits[2]+0.0001,
                             i_userLimits[0]+0.0001,
                             i_userLimits[4]+0.0001)),
    m_isolevel(-99.9999999),
    m_noiseLevel(0.0),
    m_isIntegralVolume(false)
{
    m_noOfVoxelsX = i_x;
    m_dis[0]= (m_higherLimits[0] - m_lowerLimits[0]);
    float voxelLength = m_dis[0]/i_x;
    for(unsigned int i=0; i<3;++i)
    {
       m_higherLimits[i]+=(voxelLength+0.001);
       m_lowerLimits[i]-=(voxelLength+0.001);
    }
    m_dis[0]= (m_higherLimits[0] - m_lowerLimits[0]);
    m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
    m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);
    m_noOfVoxelsY = ceil(((double)i_x)*m_dis[1]/m_dis[0]);
    m_noOfVoxelsZ = ceil(((double)i_x)*m_dis[2]/m_dis[0]);
    m_intensities.resize(m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ);
    m_noOfReturnsPerVoxel.resize(m_intensities.size());
    std::fill(m_noOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0);
    m_weightOfReturnsPerVoxel.resize(m_intensities.size());
    for(float &n : m_weightOfReturnsPerVoxel)
    {
       n = 0.0f;
    }
//    std::fill(m_weightOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0.0f);

    m_lengthOfVoxel = m_dis[0]/m_noOfVoxelsX;

    m_higherLimits[1] = m_lowerLimits[1] + m_noOfVoxelsY * m_lengthOfVoxel;
    m_higherLimits[2] = m_lowerLimits[2] + m_noOfVoxelsZ * m_lengthOfVoxel;
    m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
    m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);


    std::cout << "Legth of Voxels = " << m_lengthOfVoxel << "\n";
    std::cout << "Object constructor : no of voxels = " << m_noOfVoxelsX << " " << m_noOfVoxelsY <<  " " << m_noOfVoxelsZ << "\n";
    std::cout << "Max Limits " << m_higherLimits[0] << " " << m_higherLimits[1] << " " << m_higherLimits[2] << "\n";
    std::cout << "Min Limits " << m_lowerLimits[0] << " " << m_lowerLimits[1] << " " << m_lowerLimits[2] << "\n";
}

//-----------------------------------------------------------------------------
double Object::functionValue(const gmtl::Vec3f &i_point)
{
   // in case the object is empty then it will always return 0
   if(m_intensities.size()==0)
   {
      return 0.0;
   }
   // shift values such that the lower point is considered to be the (0,0,0)
   // used to match the actual coordinated with the index of the corresponding
   // voxel


   unsigned int myIndexfloor = getIndex(i_point);

   if(myIndexfloor>=m_intensities.size())
   {
      std::cout<<"ERROR : Point outside limits "<<myIndexfloor
              <<" / "<<m_intensities.size()<<"\n";
   }
   double value = m_intensities[myIndexfloor];

//   assert(value==value);
//   assert(value!=std::numeric_limits<float>::infinity());
//   assert(value!=-std::numeric_limits<float>::infinity());

  return value;
}

//-----------------------------------------------------------------------------
void Object::insertIntoIntegralVolume()
{
    m_integralVolume = new IntegralVolumes(m_noOfVoxelsX,m_noOfVoxelsY,
                                           m_noOfVoxelsZ,m_intensities);
//    std::cout << m_integralVolume->getSumOfArea(0,0,0,60,200,50) << "\n";
}


//-----------------------------------------------------------------------------
gmtl::Vec3f Object::getCentreOfVoxel(
        int i_x,
        int i_y,
        int i_z
        ) const
{
    return gmtl::Vec3f(m_lowerLimits[0]+((float)i_x-0.5f)*m_lengthOfVoxel,
                     m_lowerLimits[1]+((float)i_y-0.5f)*m_lengthOfVoxel,
                     m_lowerLimits[2]+((float)i_z-0.5f)*m_lengthOfVoxel);
}

//-----------------------------------------------------------------------------
void Object::addItensity(const gmtl::Vec3f &point, float i_intensity)
{
   gmtl::Vec3f i_point(point[0],point[1],point[2]);

   if(i_intensity>m_noiseLevel)
   {
//      int index_x, index_y, index_z;
//      int v_x, v_y, v_z;
//      float sumOfDistances;
      std::vector<gmtl::Vec3f> centresOfnearVoxels;
      centresOfnearVoxels.resize(8);
      std::vector<float> disFromPoint;
      disFromPoint.resize(8);
//      index_x = floor((i_point[0]-m_lowerLimits[0]-m_lengthOfVoxel/2.0f)/
//                      (m_dis[0]+m_lengthOfVoxel)*(m_noOfVoxelsX+1.0f));
//      index_y = floor((i_point[1]-m_lowerLimits[1]-m_lengthOfVoxel/2.0f)/
//                      (m_dis[1]+m_lengthOfVoxel)*(m_noOfVoxelsY+1));
//      index_z = floor((i_point[2]-m_lowerLimits[2]-m_lengthOfVoxel/2.0f)/
//                      (m_dis[2]+m_lengthOfVoxel)*(m_noOfVoxelsZ+1));
//      v_x = index_x-1;
//      v_y = index_y-1;
//      v_z = index_z-1;

//      // calculate the centres of the voxels that are close to the point
//      centresOfnearVoxels[0] = getCentreOfVoxel(index_x,index_y,index_z);
//      centresOfnearVoxels[1] = getCentreOfVoxel(index_x,v_y    ,index_z);
//      centresOfnearVoxels[2] = getCentreOfVoxel(index_x,index_y,v_z    );
//      centresOfnearVoxels[3] = getCentreOfVoxel(index_x,v_y    ,v_z    );
//      centresOfnearVoxels[4] = getCentreOfVoxel(v_x    ,index_y,index_z);
//      centresOfnearVoxels[5] = getCentreOfVoxel(v_x    ,v_y    ,index_z);
//      centresOfnearVoxels[6] = getCentreOfVoxel(v_x    ,index_y,v_z    );
//      centresOfnearVoxels[7] = getCentreOfVoxel(v_x    ,v_y    ,v_z    );

//      // calculate the distance from the above centres to the input point
//      gmtl::Vec3f temp = centresOfnearVoxels[0]-i_point;
//      disFromPoint[0] = temp.length();
//      temp = centresOfnearVoxels[1]-i_point;
//      disFromPoint[1] = temp.length();
//      temp = centresOfnearVoxels[2]-i_point;
//      disFromPoint[2] = temp.length();
//      temp = centresOfnearVoxels[3]-i_point;
//      disFromPoint[3] = temp.length();
//      temp = centresOfnearVoxels[4]-i_point;
//      disFromPoint[4] = temp.length();
//      temp = centresOfnearVoxels[5]-i_point;
//      disFromPoint[5] = temp.length();
//      temp = centresOfnearVoxels[6]-i_point;
//      disFromPoint[6] = temp.length();
//      temp = centresOfnearVoxels[7]-i_point;
//      disFromPoint[7] = temp.length();

////      for(unsigned int i=0; i<8; ++i)
////      {
////        if(disFromPoint[i]<20.0f)
////        {
//////            std::cout << disFromPoint[i] << " " << "\n";
////           disFromPoint[i]=0.0f;
////        }
//////        std::cout << "\n";
////      }

//      sumOfDistances = disFromPoint[0]+disFromPoint[1]+disFromPoint[2]+
//              disFromPoint[3]+disFromPoint[4]+disFromPoint[5]+
//              disFromPoint[6]+disFromPoint[7];

//      // loop through all the points and add the corresponding intensity to the
//      // correct voxel
//      for(unsigned int i=0; i<8; ++i)
//      {
//         if(centresOfnearVoxels[i][0] > m_lowerLimits[0]  &&
//            centresOfnearVoxels[i][0] < m_higherLimits[0] &&
//            centresOfnearVoxels[i][1] > m_lowerLimits[1]  &&
//            centresOfnearVoxels[i][1] < m_higherLimits[1] &&
//            centresOfnearVoxels[i][2] > m_lowerLimits[2]  &&
//            centresOfnearVoxels[i][2] < m_higherLimits[2])
//         {
//            unsigned int index = this->getIndex(centresOfnearVoxels[i]);
//            m_intensities[index]+= (i_intensity * disFromPoint[i]/sumOfDistances);
//            m_weightOfReturnsPerVoxel[index]+=(disFromPoint[i]/sumOfDistances);
//         }
//      }

      // test if the corresponding point is inside the voxel area limits
      if(i_point[0]>m_lowerLimits[0] && i_point[0]<m_higherLimits[0] &&
         i_point[1]>m_lowerLimits[1] && i_point[1]<m_higherLimits[1] &&
         i_point[2]>m_lowerLimits[2] && i_point[2]<m_higherLimits[2])
      {
         unsigned int index = this->getIndex(i_point);
         m_intensities[index]+=(i_intensity);
         m_noOfReturnsPerVoxel[index]++;
      }
   }
}

////-----------------------------------------------------------------------------
void Object::compare(Object *i_obj)
{
   unsigned int diff = 0;
   if(i_obj->m_intensities.size() != m_intensities.size())
   {
      return;
   }
   for(unsigned int i=0; i<m_intensities.size(); ++i)
   {
      if(i_obj->m_intensities[i]<m_intensities[i]-0.001f || i_obj->m_intensities[i]>m_intensities[i]+0.001f)
      {
         diff++;
      }
   }
   std::cout << "Difference = " << diff << " / " << m_intensities.size() << "\n";
}

//-----------------------------------------------------------------------------
void Object::setNoiseLevel(double i_noiseLevel)
{
    m_noiseLevel=i_noiseLevel;
}

//-----------------------------------------------------------------------------
void Object::normalise()
{
   for(unsigned int i=0; i< m_intensities.size(); ++i)
   {
      if(m_noOfReturnsPerVoxel[i]!=0)
      {
         m_intensities[i]/=m_noOfReturnsPerVoxel[i];
      }
//       if(m_weightOfReturnsPerVoxel[i]>0.000001)
//       {
//           m_intensities[i]/=m_weightOfReturnsPerVoxel[i];
//       }
   }

   if (m_intensities.size()<1)
   {
      // nothing to normalise, LASVoxels is empty
      return;
   }
   float max = m_intensities[0];
   float min = m_intensities[0];
   for(unsigned int i=1;i<m_intensities.size(); ++i)
   {
      if (max<m_intensities[i])
      {
         max=m_intensities[i];
      }
      else if (min>m_intensities[i])
      {
          min = m_intensities[i];
      }
   }

   // we want to shift the median point to be equal to 0
   double median = min+max/2.0;
   // and scale the values to be in the range of [-100,100]
   double div = (max-min)/2.0/100;
   for(unsigned int i=0; i<m_intensities.size(); ++i)
   {
       m_intensities[i] -= median; // translate
       m_intensities[i] /= div;    // scale
   }

    max = m_intensities[0];
    min = m_intensities[0];
   for(unsigned int i=1;i<m_intensities.size(); ++i)
   {
      if (max<m_intensities[i])
      {
         max=m_intensities[i];
      }
      else if (min>m_intensities[i])
      {
          min = m_intensities[i];
      }
   }
}

//-------------------------------------------------------------------------
const std::vector<double> &Object::getIntensities()const
{
   return m_intensities;
}


//-------------------------------------------------------------------------
unsigned int Object::getIndex(
        const gmtl::Vec3f &i_point
        )const
{
   unsigned int index_x = floor((i_point[0]-m_lowerLimits[0])
                              /m_dis[0]*m_noOfVoxelsX);
   unsigned int index_y = floor((i_point[1]-m_lowerLimits[1])
                              /m_dis[1]*m_noOfVoxelsY);
   unsigned int index_z = floor((i_point[2]-m_lowerLimits[2])
                              /m_dis[2]*m_noOfVoxelsZ);

   return index_x + index_y*m_noOfVoxelsX + index_z*m_noOfVoxelsX*m_noOfVoxelsY;
}

//-----------------------------------------------------------------------------
const gmtl::Vec3f Object::getMaxLimits()const
{
    return m_higherLimits;
    gmtl::Vec3f p2((double)m_noOfVoxelsX-1,
                 (double)m_noOfVoxelsY-1,
                 (double)m_noOfVoxelsZ-1);
    return p2;
}

//-----------------------------------------------------------------------------
void Object::setIsolevel(double i_thres)
{
   m_isolevel = i_thres;
   std::cout << "~Isolevel = " << m_isolevel << "\n";
}

//-----------------------------------------------------------------------------
const gmtl::Vec3f Object::getMinLimits()const
{
   return m_lowerLimits;
}

//-----------------------------------------------------------------------------
double Object::getIsolevel()const
{
   return m_isolevel;
}


//-----------------------------------------------------------------------------
Object::~Object()
{
   if (m_integralVolume!=0)
   {
      delete m_integralVolume;
   }
}
