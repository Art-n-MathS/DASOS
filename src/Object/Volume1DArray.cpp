#include "Volume1DArray.h"

#include <iostream>
#include <math.h>
#include <limits>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <string>
#include <exception>      // std::exception


//-----------------------------------------------------------------------------
Volume1DArray::Volume1DArray(
        float i_voxelLength,
        const std::vector<double> &i_user_limits,
        const std::string &i_type
        ) : Volume(i_voxelLength,i_user_limits,i_type)
{
    m_intensities.resize(m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ,0.0f);
    std::fill(m_intensities.begin(),m_intensities.end(),0.0f);
    m_noOfReturnsPerVoxel.resize(m_intensities.size(),0);
    std::fill(m_noOfReturnsPerVoxel.begin(),m_noOfReturnsPerVoxel.end(),0);
    std::cout << m_intensities.size() << " the size \n";
}


//-----------------------------------------------------------------------------
Volume1DArray::Volume1DArray(const std::string &i_filename):Volume(i_filename)
{
   readObjectFromFile(i_filename,m_evaluation);
}

//-----------------------------------------------------------------------------
Volume1DArray::Volume1DArray(
        const std::string &i_filename,
        bool evaluation
        ) : Volume(i_filename,evaluation)
{

}

//-----------------------------------------------------------------------------
void Volume1DArray::compare(Volume */*i_obj*/)
{
//   unsigned int diff = 0;
//   if(i_obj->m_intensities.size() != m_intensities.size())
//   {
//      return;
//   }
//   for(unsigned int i=0; i<m_intensities.size(); ++i)
//   {
//      if(i_obj->m_intensities[i]<m_intensities[i]-0.001f || i_obj->m_intensities[i]>m_intensities[i]+0.001f)
//      {
//         diff++;
//      }
//   }
//   std::cout << "Difference = " << diff << " / " << m_intensities.size() << "\n";
}


//-----------------------------------------------------------------------------
float Volume1DArray::functionValue(const gmtl::Vec3f &i_point)
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

//-------------------------------------------------------------------------
float Volume1DArray::getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   return m_intensities[i_x+i_y*m_noOfVoxelsX+
           i_z*m_noOfVoxelsX*m_noOfVoxelsY];
}

//-----------------------------------------------------------------------------
bool Volume1DArray::isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   return m_intensities[i_x+i_y*m_noOfVoxelsX+
           i_z*m_noOfVoxelsX*m_noOfVoxelsY]>m_isolevel;
}


//-----------------------------------------------------------------------------
void Volume1DArray::addItensityTypeVol(const gmtl::Vec3f &i_point, float i_intensity)
{
   if(m_noOfReturnsPerVoxel.size()==0)
   {
      std::cout << "IntegralVolume has been normalised. So no more intensities"
                << " can be added\n";
      return;
   }
   unsigned int index = this->getIndex(i_point);
   m_noOfReturnsPerVoxel[index]++;
   float noOfRe = m_noOfReturnsPerVoxel[index];
   m_intensities[index]=(noOfRe-1)/noOfRe*m_intensities[index] + float(i_intensity)/noOfRe;
}


//-----------------------------------------------------------------------------
void Volume1DArray::normalise()
{
   m_noOfReturnsPerVoxel.clear();
}
//-----------------------------------------------------------------------------
void Volume1DArray::exportToFile(
        std::string i_filename,
        bool i_compression
        )
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
    // Compression on/off
    // - All the intensities
    std::ofstream myfile;
    myfile << std::fixed;
    myfile << std::setprecision(8);

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

    if(i_compression)
    {
       myfile << "Compression true\n";
       for(unsigned int i=0; i<m_intensities.size();++i)
       {
          if(m_intensities[i]<0.000001 && m_intensities[i]>-0.000001)
          {
             unsigned int noOfZeros(0);
             while (i+noOfZeros<m_intensities.size() &&
                    m_intensities[i+noOfZeros]<0.000001 &&
                    m_intensities[i+noOfZeros]>-0.000001)
             {
                ++noOfZeros;
             }
             i+=noOfZeros;
             myfile << "0x" << noOfZeros << " ";
          }
          else
          {
             myfile << m_intensities[i] << " ";
          }
       }
    }
    else
    {
       myfile << "Compression false\n";
       for(unsigned int i=0; i<m_intensities.size(); ++i)
       {
          myfile << m_intensities[i] << " ";
       }
    }
    myfile << "\n\n";
    myfile.close();
}

//-----------------------------------------------------------------------------
void Volume1DArray::readObjectFromFile(
        const std::string &i_filename,
        bool evaluation
        )
{
    try{

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

    // LowerLimits x y z
    // HigherLimits x y z
    // Isolevel Iso
    // NoOfVoxels x y z
    // Distance dx dy dz
    // NoiseLevel l
    // NumberOfIntensities Num23
    // Compression on/off25
    // - All the intensities
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

    bool isFileCompressed = (words[25] == "true");

    std::cout << " PAssiging this test";
    unsigned int inum =(unsigned int) atoi(words[23].c_str());
    if(!evaluation)
    {
       m_intensities.resize(inum);
       std::fill(m_intensities.begin(),m_intensities.end(),0.0f);
       if(isFileCompressed)
       {
          unsigned int wordsIndex = 26;
          std::cout << "File compressed\n";
          for(unsigned int i=0; i<m_intensities.size();++i)
          {
             if(words[wordsIndex].substr(0,1)=="0")
             {
                //compressed value
                unsigned int len = atoi(words[wordsIndex].substr(2,words[wordsIndex].length()-2).c_str());
                if(wordsIndex+len>m_intensities.size())
                {
                   std::cout << "ERROR: reach end of file but not all intensities has been read\n";
                   return;
                }
                wordsIndex++;
                i+=len;
             }
             else
             {
                if(wordsIndex<words.size())
                {
                   m_intensities[i] = atof(words[wordsIndex].c_str());
                   ++wordsIndex;
                }
                else
                {
                   std::cout << "ERROR: reach end of file but not all intensities has been read\n";
                   return;
                }
             }
          }
       }
       else
       {
           std::cout << "26 + " << words[23] << " = " << words.size() << "\n";
           if(words.size()!=(unsigned int)(26+atoi(words[23].c_str())))
           {
               std::cerr << "ERROR: File \"" << i_filename
                         << "\" is not written in the correct format.\n";
               return;
           }
          std::cout << "File is not compressed\n";
          for(unsigned int i = 0; i < inum; ++i)
          {
             m_intensities[i] = atof(words[26+i].c_str());
          }
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
    catch (std::exception& e)
    {
       std::cerr << "exception caught: " << e.what() << '\n';
    }

}



//-----------------------------------------------------------------------------
Volume1DArray::~Volume1DArray()
{

}

