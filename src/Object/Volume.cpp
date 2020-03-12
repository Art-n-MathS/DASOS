#include "Volume.h"
#include <math.h>
#include <iostream>
#include <assert.h>

//-----------------------------------------------------------------------------
Volume::Volume(
        double i_voxelLength,
        const std::vector<double> &i_userLimits,
        const std::string &i_type
        ):
    m_evaluation(false),
    m_type(i_type),
    m_lengthOfVoxel(i_voxelLength),
    m_lowerLimits(gmtl::Vec3f(i_userLimits[3],i_userLimits[1],i_userLimits[5])),
    m_higherLimits(gmtl::Vec3f(i_userLimits[2]+0.0001,
                               i_userLimits[0]+0.0001,
                               i_userLimits[4]+0.0001)),
    m_isolevel(0.1),
    m_noiseLevel(0.0)
{
   for(unsigned int i=0; i<3;++i)
   {
      m_higherLimits[i]+=(i_voxelLength+0.00001);
      m_lowerLimits[i]-=(i_voxelLength+0.00001);
   }
   // adjust lower limits to keep consistency between different flightlines
   m_lowerLimits[2] = (ceil(m_lowerLimits[2]/i_voxelLength)) * i_voxelLength-i_voxelLength;
   m_higherLimits[2] = (ceil(m_higherLimits[2]/i_voxelLength)) * i_voxelLength+i_voxelLength;
   m_lowerLimits[0] -=i_voxelLength;
   m_higherLimits[0] +=i_voxelLength;
   m_lowerLimits[1] -=i_voxelLength;
   m_higherLimits[1] +=i_voxelLength;

   m_noOfVoxelsX = ceil((m_higherLimits[0]-m_lowerLimits[0])/m_lengthOfVoxel);

   m_dis[0]= (m_higherLimits[0] - m_lowerLimits[0]);
   m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
   m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);
   m_noOfVoxelsY = ceil((m_higherLimits[1]-m_lowerLimits[1])/m_lengthOfVoxel);
   m_noOfVoxelsZ = ceil((m_higherLimits[2]-m_lowerLimits[2])/m_lengthOfVoxel);



   m_higherLimits[1] = m_lowerLimits[1] + m_noOfVoxelsY * m_lengthOfVoxel;
   m_higherLimits[2] = m_lowerLimits[2] + m_noOfVoxelsZ * m_lengthOfVoxel;

   m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
   m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);


//    std::cout << "Legth of Voxels = " << m_lengthOfVoxel << "\n";
  std::cout << "Volume constructor : no of voxels = " << m_noOfVoxelsX << " " << m_noOfVoxelsY <<  " " << m_noOfVoxelsZ << "\n";
    std::cout << "Max Limits " << m_higherLimits[0] << " " << m_higherLimits[1] << " " << m_higherLimits[2] << "\n";
    std::cout << "Min Limits " << m_lowerLimits[0] << " " << m_lowerLimits[1] << " " << m_lowerLimits[2] << "\n";
    std::cout << "*Max Limits " << i_userLimits[2]+0.0001<< " " <<
            i_userLimits[0]+0.0001<< " " <<
            i_userLimits[4]+0.0001 << "\n";
    std::cout << "*Min Limits " << i_userLimits[3]<< " " <<i_userLimits[1]<< " " <<i_userLimits[5] << "\n";

}

//-----------------------------------------------------------------------------
double Volume::getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   if(i_x>=m_noOfVoxelsX || i_y>=m_noOfVoxelsY || i_z>=m_noOfVoxelsZ)
   {
      return -0.0;
   }
   return getIntensityTypeVol(i_x,i_y,i_z);
}

//-----------------------------------------------------------------------------
void Volume::setVolumeSize(
        unsigned long i_lenX,
        unsigned long i_lenY,
        unsigned long i_lenZ
        )
{
   m_noOfVoxelsX = i_lenX;
   m_noOfVoxelsY = i_lenY;
   m_noOfVoxelsZ = i_lenZ;
   m_higherLimits[0] = m_lowerLimits[0] + m_noOfVoxelsX * m_lengthOfVoxel;
   m_higherLimits[1] = m_lowerLimits[1] + m_noOfVoxelsY * m_lengthOfVoxel;
   m_higherLimits[2] = m_lowerLimits[2] + m_noOfVoxelsZ * m_lengthOfVoxel;
   m_dis[0] = (m_higherLimits[0] - m_lowerLimits[0]);
   m_dis[1] = (m_higherLimits[1] - m_lowerLimits[1]);
   m_dis[2] = (m_higherLimits[2] - m_lowerLimits[2]);
}

//-----------------------------------------------------------------------------
Volume::Volume(const std::string &i_filename, bool evaluation)
{
   readObjectFromFile(i_filename,evaluation);
}


//-----------------------------------------------------------------------------
unsigned int Volume::getNoOfLevels()const
{
   std::cerr << "ERROR: Volume::getDepthLevel() is only "
             << "used at Series of Hashed Octrees\n";
   assert(true);
   exit(EXIT_FAILURE);
}

//-----------------------------------------------------------------------------
void Volume::addItensity(const gmtl::Vec3f &i_point, double i_intensity)
{
   if(i_intensity>m_noiseLevel)
   {
//      std::cout << "Int OK - ";
      std::vector<gmtl::Vec3f> centresOfnearVoxels;
      centresOfnearVoxels.resize(8);
      std::vector<double> disFromPoint;
      disFromPoint.resize(8);
      // test if the corresponding point is inside the voxel area limits
      if(i_point[0]>m_lowerLimits[0] && i_point[0]<m_higherLimits[0] &&
         i_point[1]>m_lowerLimits[1] && i_point[1]<m_higherLimits[1] &&
         i_point[2]>m_lowerLimits[2] && i_point[2]<m_higherLimits[2])
      {
//         std::cout << " - inside limits\n";
         addItensityTypeVol(i_point,i_intensity);
      }
//      std::cout << "\n";
   }
}


//-----------------------------------------------------------------------------
void Volume::exportToFile(std::string /*i_filename*/,bool /*i_compression*/)
{
   std::cout << "WARNING: exportToFile is not defined => Volume not exported\n";
}

//-----------------------------------------------------------------------------
void Volume::compareVolume(Volume *i_obj)
{
   std::cout << "start comparing volume " << m_type << " " << i_obj->m_type << "\n" ;
   unsigned int wrong(0);
   std::cout << i_obj->m_noOfVoxelsX << " " << i_obj->m_noOfVoxelsY << " " << i_obj->getNoVoxelsZ() << "\n";
   std::cout << m_noOfVoxelsX << " " << m_noOfVoxelsY << " " << m_noOfVoxelsZ << "\n";
   unsigned int zeros(0);
   unsigned int nonzerosButnotEqual(0);
   for(unsigned int x=0; x<m_noOfVoxelsX; x++)
   {
      for(unsigned int y=0; y<m_noOfVoxelsY; y++)
      {
         for(unsigned int z=0; z<m_noOfVoxelsZ; z++)
         {
            double int1 = getIntensity(x,y,z);
            double int2 = i_obj->getIntensity(x,y,z);
//            std::cout << wrong << " " << int1 << " " << int2 << "\n";
            if(int1>int2+0.0001 || int1<int2-0.0001)
            {
               wrong++;
               std::cout << x << " " << y << " " << z << " : " << int1 << " " << int2 << "\n";
//               std::cout << wrong << " " << int1 << " " << int2 << "\n";
            }
            else
            {
               if(int1<0.01)
               {
                  zeros++;
               }
               else
               {
                  nonzerosButnotEqual++;
               }
            }
         }
      }
   }
   std::cout<< "Compare Volumes Results:\nDifferent Voxels are " << wrong
            << " of " << m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ << "\n"
            <<"Percentage = "<<double(wrong/double(m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ))
            << "\nzeros = " << zeros << " + " << nonzerosButnotEqual << "\n"
            << "\n";

}

//-----------------------------------------------------------------------------
Volume::Volume(const std::string &/*i_filename*/):m_evaluation(false)
{
}

//-----------------------------------------------------------------------------
gmtl::Vec2i Volume::getIndices(double i_x, double i_y) const
{
   gmtl::Vec2i indices;
   indices[0] = floor((i_x-m_lowerLimits[0])
                               /m_dis[0]*m_noOfVoxelsX);
   indices[1] = floor((i_y-m_lowerLimits[1])
                               /m_dis[1]*m_noOfVoxelsY);
   return indices;
}

//-----------------------------------------------------------------------------
gmtl::Vec3i Volume::getIndices(double i_x, double i_y, double i_z) const
{
   gmtl::Vec3i indices;
   indices[0] = floor((i_x-m_lowerLimits[0])
                               /m_dis[0]*m_noOfVoxelsX);
   indices[1] = floor((i_y-m_lowerLimits[1])
                               /m_dis[1]*m_noOfVoxelsY);
   indices[2] = floor((i_z-m_lowerLimits[2])
                               /m_dis[2]*m_noOfVoxelsZ);
   return indices;
}

//-----------------------------------------------------------------------------
gmtl::Vec3i Volume::getIndices(unsigned long int index) const
{
    gmtl::Vec3i indices;
    indices[2] = floor(double(index)/double(m_noOfVoxelsX*m_noOfVoxelsY)); //z
    indices[1] = floor((double(index)-indices[2]*m_noOfVoxelsX*m_noOfVoxelsY)/
            double(m_noOfVoxelsX)); //y
    indices[0] = index-indices[1]*m_noOfVoxelsX-indices[2]*m_noOfVoxelsX*m_noOfVoxelsY; //x
    // index_x + index_y*m_noOfVoxelsX + index_z*m_noOfVoxelsX*m_noOfVoxelsY;
    return indices;
}

//-----------------------------------------------------------------------------
gmtl::Vec3i Volume::getIndices(
        unsigned long /*i_index*/,
        unsigned int /*i_level*/
        ) const
{
   std::cerr << "ERROR: Volume::getIndices(unsigned long,unsigned int)"
             << "only exist at the SeriesOfHashedOctrees\n";
   assert(true);
   exit(EXIT_FAILURE);
}

//-----------------------------------------------------------------------------
double Volume::getIsolevel()const
{
   return m_isolevel;
}

//-----------------------------------------------------------------------------
const gmtl::Vec3f &Volume::getMaxLimits()const
{
    return m_higherLimits;
}

//-----------------------------------------------------------------------------
const gmtl::Vec3f &Volume::getMinLimits()const
{
   return m_lowerLimits;
}

//-----------------------------------------------------------------------------
void Volume::setNoiseLevel(double i_noiseLevel)
{
    m_noiseLevel=i_noiseLevel;
}

//-----------------------------------------------------------------------------
gmtl::Vec2f Volume::getCoordinatesMiddle(
        unsigned int i_x,
        unsigned int i_y
        ) const
{
   return gmtl::Vec2f(m_lowerLimits[0]+i_x*m_lengthOfVoxel+m_lengthOfVoxel/2.0,
                      m_lowerLimits[1]+i_y*m_lengthOfVoxel+m_lengthOfVoxel/2.0);
}

//-----------------------------------------------------------------------------
double Volume::getMaxValue(
        unsigned int /*i_key*/,
        unsigned int /*i_level*/
        )
{
   std::cerr << "ERROR: this subclass of Volume do not support this function\n";
   assert(true);
   exit(EXIT_FAILURE);
}


//-----------------------------------------------------------------------------
void Volume::setIsolevel(double i_thres)
{
   m_isolevel = i_thres;
}

//-------------------------------------------------------------------------
unsigned int Volume::getIndex(
        const gmtl::Vec3f &i_point
        )const
{
   gmtl::Vec3i indices= getIndices(i_point[0],i_point[1],i_point[2]);
   return indices[0] + indices[1]*m_noOfVoxelsX +
           indices[2]*m_noOfVoxelsX*m_noOfVoxelsY;
}

//-----------------------------------------------------------------------------
void Volume::readObjectFromFile(
        const std::string &/*i_filename*/,
        bool /*evaluation*/
        )
{
   std::cout << "Reading Volume from file haven't been defined\n"
             << "Program will terminate\n";
   std::exit(EXIT_FAILURE);
}



//-----------------------------------------------------------------------------
Volume::~Volume()
{}
