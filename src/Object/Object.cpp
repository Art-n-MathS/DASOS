#include "Object.h"
#include <limits>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>

//-----------------------------------------------------------------------------
Object::Object(float i_voxelLength, const std::vector<double> &i_userLimits):
    m_integralVolume(0),
    m_lengthOfVoxel(i_voxelLength),
    m_lowerLimits(gmtl::Vec3f(i_userLimits[3],i_userLimits[1],i_userLimits[5])),
    m_higherLimits(gmtl::Vec3f(i_userLimits[2]+0.0001,
                             i_userLimits[0]+0.0001,
                             i_userLimits[4]+0.0001)),
    m_isolevel(-99.9999999),
    m_noiseLevel(0.0),
    countIncludedPoints(0),
    countNotInclPoints(0)
{
    m_noOfVoxelsX = ceil((i_userLimits[2]-i_userLimits[3])/i_voxelLength);
    m_dis[0]= (m_higherLimits[0] - m_lowerLimits[0]);
    for(unsigned int i=0; i<3;++i)
    {
       m_higherLimits[i]+=(i_voxelLength+0.00001);
       m_lowerLimits[i]-=(i_voxelLength+0.00001);
    }
    m_dis[0]= (m_higherLimits[0] - m_lowerLimits[0]);
    m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
    m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);
    m_noOfVoxelsY = ceil(((double)m_noOfVoxelsX)*m_dis[1]/m_dis[0]);
    m_noOfVoxelsZ = ceil(((double)m_noOfVoxelsX)*m_dis[2]/m_dis[0]);
    m_intensities.resize(m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ);
    m_noOfReturnsPerVoxel.resize(m_intensities.size());
    std::fill(m_noOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0);


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
gmtl::Vec2i Object::getIndices(float i_x, float i_y) const
{
   gmtl::Vec2i indices;
   indices[0] = floor((i_x-m_lowerLimits[0])
                               /m_dis[0]*m_noOfVoxelsX);
   indices[1] = floor((i_y-m_lowerLimits[1])
                               /m_dis[1]*m_noOfVoxelsY);
   return indices;
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
   if(m_integralVolume!=0)
   {
      delete m_integralVolume;
   }
   m_integralVolume = new IntegralVolumes(m_noOfVoxelsX,m_noOfVoxelsY,
                                          m_noOfVoxelsZ,m_intensities);
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
      std::vector<gmtl::Vec3f> centresOfnearVoxels;
      centresOfnearVoxels.resize(8);
      std::vector<float> disFromPoint;
      disFromPoint.resize(8);
      // test if the corresponding point is inside the voxel area limits
      if(i_point[0]>m_lowerLimits[0] && i_point[0]<m_higherLimits[0] &&
         i_point[1]>m_lowerLimits[1] && i_point[1]<m_higherLimits[1] &&
         i_point[2]>m_lowerLimits[2] && i_point[2]<m_higherLimits[2])
      {
         countIncludedPoints++;
         unsigned int index = this->getIndex(i_point);
         m_intensities[index]+=(i_intensity);
         m_noOfReturnsPerVoxel[index]++;
      }
      else
      {
         countNotInclPoints++;
      }
   }
}

//-----------------------------------------------------------------------------
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
void Object::exportToFile(std::string i_filename)
{
    /// FILE STRUCTURE
    // LowerLimits x y z
    // HigherLimits x y z
    // Isolevel Iso
    // NoOfVoxels x y z
    // Distance dx dy dz
    // NoiseLevel l
    // VoxelLength vl
    // NumberOfIntensities Num
    // - All the intensities
    std::ofstream myfile;
    myfile.open(i_filename);
    myfile << "LowerLimits " << m_lowerLimits[0] << " " << m_lowerLimits[1]
           << " " << m_lowerLimits[2] << "\n";
    myfile << "HigherLimits " << m_higherLimits[0] << " " << m_higherLimits[1]
           << " " << m_higherLimits[2] << "\n";
    myfile << "Isolever " << m_isolevel << "\n";
    myfile << "NoOfVoxels " << m_noOfVoxelsX << " " << m_noOfVoxelsY << " "
           << m_noOfVoxelsZ << "\n";
    myfile << "Distance " << m_dis[0] << " " << m_dis[1] << " " << m_dis[2] << "\n";
    myfile << "NoiseLevel " << m_noiseLevel << "\n";
    myfile << "VoxelLength " << m_lengthOfVoxel << "\n";
    myfile << "NumberOfIntensities " << m_intensities.size() << "\n";

    std::cout << "nl _ iL " << m_noiseLevel << " : " << m_isolevel << "\n";

    for(unsigned int i=0; i<m_intensities.size(); ++i)
    {
       myfile << m_intensities[i] << " ";
    }
    myfile << "\n\n";
    myfile.close();
}


void Object::readObjectFromFile(const std::string &i_filename, bool evaluation)
{
    m_integralVolume = 0;
    std::cout << "Object created from file\n";
    std::ifstream mystream(i_filename.c_str());
    if(!mystream)
    {
       std::cerr << "File \"" << i_filename << "\" not found.\n";
    }
    std::istream_iterator<std::string> it(mystream);
    std::istream_iterator<std::string> sentinel;

    std::vector<std::string> words(it,sentinel);

    if(words.size()<24)
    {
       std::cerr << "ERROR: File \"" << i_filename
                 << "\" is not written in the correct format.\n";
       return;
    }
    std::cout << words[23] << "\n";
    if(words.size()!=(unsigned int)(24+atoi(words[23].c_str())))
    {
        std::cerr << "ERROR: File \"" << i_filename
                  << "\" is not written in the correct format.\n";
        return;
    }

 //    LowerLimits x1 y2 z3
 //    HigherLimits x5 y6 z7
 //    Isolevel Iso9
 //    NoOfVoxels x11 y12 z13
 //    Distance dx15 dy16 dz17
 //    NoiseLevel l19
 //    NumberOfIntensities Num21
 //    - All the intensities
    m_lowerLimits[0] = atof(words[1].c_str());
    m_lowerLimits[1] = atof(words[2].c_str());
    m_lowerLimits[2] = atof(words[3].c_str());
    m_higherLimits[0] = atof(words[5].c_str());
    m_higherLimits[1] = atof(words[6].c_str());
    m_higherLimits[2] = atof(words[7].c_str());
    m_isolevel = atof(words[9].c_str());
    m_noOfVoxelsX = atoi(words[11].c_str());
    m_noOfVoxelsY = atoi(words[12].c_str());
    m_noOfVoxelsZ = atoi(words[13].c_str());
    m_dis[0] = atof(words[15].c_str());
    m_dis[1] = atof(words[16].c_str());
    m_dis[2] = atof(words[17].c_str());
    m_noiseLevel = atof(words[19].c_str());
    m_lengthOfVoxel = atof(words[21].c_str());
    std::cout << m_lengthOfVoxel << "\n";
    unsigned int inum = (unsigned int) atoi(words[23].c_str());
    if(!evaluation)
    {
       m_intensities.resize(inum);
       for(unsigned int i = 0; i < inum; ++i)
       {
          m_intensities[i] = atof(words[24+i].c_str());
       }
    }
    else
    {
       std::cout << "WARNING: Object has been partially created, only use this"
                 << " option for evalueation, otherwise segmentation fault may"
                 << " occur! creating Object\n";
    }
 //   m_lengthOfVoxel = m_dis[0]/m_noOfVoxelsX;
}

//-----------------------------------------------------------------------------
Object::Object(const std::string &i_filename)
{
   readObjectFromFile(i_filename,false);
}

//-----------------------------------------------------------------------------
Object::Object(const std::string &i_filename, bool evaluation)
{
   readObjectFromFile(i_filename,evaluation);
}


//-----------------------------------------------------------------------------
void Object::setNoiseLevel(double i_noiseLevel)
{
    m_noiseLevel=i_noiseLevel;
}

//-----------------------------------------------------------------------------
void Object::normalise()
{
   std::cout << "======================================\n";
   std::cout << "Points Included vs Points Not included: " << countIncludedPoints << " " << countNotInclPoints << "\n";
   std::cout << "======================================\n";
   if(m_noOfReturnsPerVoxel.size()!=m_intensities.size())
   {
      std::cout << "Warning: Object Normalisation has already been called\n";
      return;
   }
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
   m_noOfReturnsPerVoxel.clear();

   if (m_intensities.size()<1)
   {
      // nothing to normalise, LASVoxels is empty
      return;
   }
   float max = m_intensities[0];
   float min = m_intensities[0];
   std::cout << "max = " << max << " " << min << "\n";
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
