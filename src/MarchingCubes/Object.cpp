#include "Object.h"
#include <limits>
#include <assert.h>
#include <QImage>

//-----------------------------------------------------------------------------
Object::Object(unsigned int i_x, const std::vector<double> &i_userLimits):
    m_lowerLimits(ngl::Vec3(i_userLimits[3],i_userLimits[1],i_userLimits[5])),
    m_higherLimits(ngl::Vec3(i_userLimits[2]+0.0001,
                             i_userLimits[0]+0.0001,
                             i_userLimits[4]+0.0001)),
    m_isolevel(-99.9999999),
    m_noiseLevel(0.0),
    m_isIntegralVolume(false),
    m_integralVolume(0)
{
    m_noOfVoxelsX = i_x;
    m_dis.m_x= (m_higherLimits.m_x - m_lowerLimits.m_x);
    m_dis.m_y = (m_higherLimits.m_y - m_lowerLimits.m_y);
    m_dis.m_z = (m_higherLimits.m_z - m_lowerLimits.m_z);
    m_noOfVoxelsY = ceil(((double)i_x)*m_dis.m_y/m_dis.m_x);
    m_noOfVoxelsZ = ceil(((double)i_x)*m_dis.m_z/m_dis.m_x);
    m_intensities.resize(m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ);
    m_noOfReturnsPerVoxel.resize(m_intensities.size());
    std::fill(m_noOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0);
    m_weightOfReturnsPerVoxel.resize(m_intensities.size());
    for(float &n : m_weightOfReturnsPerVoxel)
    {
       n = 0.0f;
    }
//    std::fill(m_weightOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0.0f);

    m_lengthOfVoxel = m_dis.m_x/m_noOfVoxelsX;

    m_higherLimits.m_y = m_lowerLimits.m_y + m_noOfVoxelsY * m_lengthOfVoxel;
    m_higherLimits.m_z = m_lowerLimits.m_z + m_noOfVoxelsZ * m_lengthOfVoxel;
    m_dis.m_y = (m_higherLimits.m_y - m_lowerLimits.m_y);
    m_dis.m_z = (m_higherLimits.m_z - m_lowerLimits.m_z);


    std::cout << "Legth of Voxels = " << m_lengthOfVoxel << "\n";
    std::cout << "Object constructor : no of voxels = " << m_noOfVoxelsX << " " << m_noOfVoxelsY <<  " " << m_noOfVoxelsZ << "\n";
    std::cout << "Max Limits " << m_higherLimits.m_x << " " << m_higherLimits.m_y << " " << m_higherLimits.m_z << "\n";
    std::cout << "Min Limits " << m_lowerLimits.m_x << " " << m_lowerLimits.m_y << " " << m_lowerLimits.m_z << "\n";
}

//-----------------------------------------------------------------------------
double Object::functionValue(const ngl::Vec3 &i_point)
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
}


//-----------------------------------------------------------------------------
ngl::Vec3 Object::getCentreOfVoxel(
        int i_x,
        int i_y,
        int i_z
        ) const
{
    return ngl::Vec3(m_lowerLimits.m_x+((float)i_x-0.5f)*m_lengthOfVoxel,
                     m_lowerLimits.m_y+((float)i_y-0.5f)*m_lengthOfVoxel,
                     m_lowerLimits.m_z+((float)i_z-0.5f)*m_lengthOfVoxel);
}

//-----------------------------------------------------------------------------
void Object::addItensity(const ngl::Vec3 &i_point, float i_intensity)
{
   if(i_intensity>m_noiseLevel)
   {
      int index_x, index_y, index_z;
      int v_x, v_y, v_z;
      float sumOfDistances;
      std::vector<ngl::Vec3> centresOfnearVoxels;
      centresOfnearVoxels.resize(8);
      std::vector<float> disFromPoint;
      disFromPoint.resize(8);
//      index_x = floor((i_point.m_x-m_lowerLimits.m_x-m_lengthOfVoxel/2.0f)/
//                      (m_dis.m_x+m_lengthOfVoxel)*(m_noOfVoxelsX+1.0f));
//      index_y = floor((i_point.m_y-m_lowerLimits.m_y-m_lengthOfVoxel/2.0f)/
//                      (m_dis.m_y+m_lengthOfVoxel)*(m_noOfVoxelsY+1));
//      index_z = floor((i_point.m_z-m_lowerLimits.m_z-m_lengthOfVoxel/2.0f)/
//                      (m_dis.m_z+m_lengthOfVoxel)*(m_noOfVoxelsZ+1));
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
//      ngl::Vec3 temp = centresOfnearVoxels[0]-i_point;
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
//         if(centresOfnearVoxels[i].m_x > m_lowerLimits.m_x  &&
//            centresOfnearVoxels[i].m_x < m_higherLimits.m_x &&
//            centresOfnearVoxels[i].m_y > m_lowerLimits.m_y  &&
//            centresOfnearVoxels[i].m_y < m_higherLimits.m_y &&
//            centresOfnearVoxels[i].m_z > m_lowerLimits.m_z  &&
//            centresOfnearVoxels[i].m_z < m_higherLimits.m_z)
//         {
//            unsigned int index = this->getIndex(centresOfnearVoxels[i]);
//            m_intensities[index]+= (i_intensity * disFromPoint[i]/sumOfDistances);
//            m_weightOfReturnsPerVoxel[index]+=(disFromPoint[i]/sumOfDistances);
//         }
//      }

      // test if the corresponding point is inside the voxel area limits
      if(i_point.m_x>m_lowerLimits.m_x && i_point.m_x<m_higherLimits.m_x &&
         i_point.m_y>m_lowerLimits.m_y && i_point.m_y<m_higherLimits.m_y &&
         i_point.m_z>m_lowerLimits.m_z && i_point.m_z<m_higherLimits.m_z)
      {
         unsigned int index = this->getIndex(i_point);
         m_intensities[index]+=(i_intensity);
         m_noOfReturnsPerVoxel[index]++;
      }
   }
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
        const ngl::Vec3 &i_point
        )const
{
   unsigned int index_x = floor((i_point.m_x-m_lowerLimits.m_x)
                              /m_dis.m_x*m_noOfVoxelsX);
   unsigned int index_y = floor((i_point.m_y-m_lowerLimits.m_y)
                              /m_dis.m_y*m_noOfVoxelsY);
   unsigned int index_z = floor((i_point.m_z-m_lowerLimits.m_z)
                              /m_dis.m_z*m_noOfVoxelsZ);

   return index_x + index_y*m_noOfVoxelsX + index_z*m_noOfVoxelsX*m_noOfVoxelsY;
}

//-----------------------------------------------------------------------------
const ngl::Vec3 Object::getMaxLimits()const
{
    return m_higherLimits;
    ngl::Vec3 p2((double)m_noOfVoxelsX-1,
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
const ngl::Vec3 Object::getMinLimits()const
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
