#include "VolumeHashed1DArray.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

//-----------------------------------------------------------------------------
VolumeHashed1DArray::VolumeHashed1DArray(
        double i_voxelLength,
        const std::vector<double> &i_user_limits,
        const std::string &i_type
        ) :
    Volume(i_voxelLength,i_user_limits,i_type)
{
}


//-----------------------------------------------------------------------------
VolumeHashed1DArray::VolumeHashed1DArray(
        const std::string &i_filename
        ):Volume(i_filename)
{
   readObjectFromFile(i_filename,m_evaluation);
}

//-----------------------------------------------------------------------------
void VolumeHashed1DArray::compare(Volume */*i_obj*/)
{
   std::cout << "VolumeSeriesOfHashedOctrees::compare haven't been implemented yet \n";
}

//-----------------------------------------------------------------------------
double VolumeHashed1DArray::functionValue(const gmtl::Vec3f &i_point)
{
   unsigned long int key = getIndex(i_point);
   std::unordered_map<unsigned long int, double>::iterator got =m_intensities.find(key);
   if(got==m_intensities.end())
   {
      return 0.0f;
   }
   else
   {
      return got->second;
   }
}

//-----------------------------------------------------------------------------
double VolumeHashed1DArray::getIntensityTypeVol(
        unsigned int i_x,
        unsigned int i_y,
        unsigned int i_z
        )
{
   unsigned long int key=i_x+i_y*m_noOfVoxelsX+i_z*m_noOfVoxelsX*m_noOfVoxelsY;
   std::unordered_map<unsigned long int, double>::iterator got =m_intensities.find(key);
   if(got==m_intensities.end())
   {
      return 0.0f;
   }
   else
   {
      return got->second;
   }
}

//-----------------------------------------------------------------------------
void VolumeHashed1DArray::normalise()
{
   m_noOfSamplesPerVoxel.clear();
}

//-----------------------------------------------------------------------------
bool VolumeHashed1DArray::isInside(
        unsigned int i_x,
        unsigned int i_y,
        unsigned int i_z
        )
{
   unsigned long int key=i_x+i_y*long(m_noOfVoxelsX)+i_z*long(m_noOfVoxelsX)*long(m_noOfVoxelsY);
   std::unordered_map<unsigned long int, double>::iterator got =m_intensities.find(key);
   if(got==m_intensities.end())
   {
       return false;
   }
   else
   {
      return got->second>m_isolevel;
   }
}

//-----------------------------------------------------------------------------
void VolumeHashed1DArray::addItensityTypeVol(
        const gmtl::Vec3f &i_point,
        double i_intensity
        )
{
   // check if intensity is 0 - this happens with discrete data
   if(i_intensity<0.00001 && i_intensity>-0.000001)
   {
      // replace by 1.0 since a zero intensity will be merged with the non-empty voxels.
      i_intensity = 1.0;
   }
   if(m_noOfSamplesPerVoxel.empty() && !m_noOfSamplesPerVoxel.empty())
   {
      std::cout << "WARNING:After normalisationm no intensity can be added\n";
      return;
   }
   unsigned long int key = getIndex(i_point);


   // check if it already exist in the table:
   std::unordered_map<unsigned long int, double>::iterator got =m_intensities.find(key);
   std::unordered_map<unsigned long int,unsigned int>::iterator got2 =
                                            m_noOfSamplesPerVoxel.find(key);

   if(got==m_intensities.end())
   {
      std::pair<unsigned long int, double> pair(key,i_intensity);
      m_intensities.insert(pair);
      std::pair<unsigned long int, unsigned int> pairNoOfSamples(key,1);
      m_noOfSamplesPerVoxel.insert(pairNoOfSamples);
   }
   else
   {
      // add intensity to current one and increment number of samples
      if(got2!=m_noOfSamplesPerVoxel.end())
      {
         got2->second++;
         double noOfRe = got2->second;
         got->second=(noOfRe-1)/noOfRe*got->second + i_intensity/noOfRe;
      }
      else
      {
         std::cout << "WARNING: number of samples should existed\n";
      }
   }
}

//-----------------------------------------------------------------------------
void VolumeHashed1DArray::exportToFile(
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
    myfile << "NumberOfIntensities " << m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ << "\n";

    unsigned int nI = m_noOfVoxelsX * m_noOfVoxelsY *m_noOfVoxelsZ;

    std::cout << "Start exporting file\n";
    if(i_compression)
    {
       myfile << "Compression true\n";
       for(unsigned int i=0; i<nI; ++i)
       {
          std::unordered_map<unsigned long int, double>::const_iterator got = m_intensities.find(i);
          if(got==m_intensities.end())
          {
             unsigned int noOfZeros(0);
             while(got==m_intensities.end() && (i+noOfZeros<nI))
             {
                got = m_intensities.find(i+noOfZeros);
                ++noOfZeros;
             }
             i+=noOfZeros;
             myfile << "0x" <<noOfZeros << " ";
          }
          else
          {
             myfile << got->second << " ";
          }
       }
    }
    else
    {
       myfile << "Compression false\n";
       for(unsigned int i=0; i<nI; ++i)
       {
          std::unordered_map<unsigned long int, double>::const_iterator got = m_intensities.find(i);
          if(got==m_intensities.end())
          {
             myfile << "0 ";
          }
          else
          {
             myfile << got->second << " ";
          }
       }
    }
    myfile << "\n\n";
    myfile.close();
}

//-----------------------------------------------------------------------------
void VolumeHashed1DArray::readObjectFromFile(
        const std::string &i_filename,
        bool evaluation
        )
{
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

    unsigned int noOfIntensities  = m_noOfVoxelsX*m_noOfVoxelsY*m_noOfVoxelsZ;
    bool isFileCompressed = (words[25] == "true");
    unsigned int inum = (unsigned int) atoi(words[23].c_str());
    if(!evaluation)
    {
       unsigned int wordsIndex = 26;
       if(isFileCompressed)
       {
          for(unsigned int i=0; i<noOfIntensities;++i)
          {
             if(words[wordsIndex].substr(0,1)=="0")
             {
                //compressed value
                unsigned int len = atoi(words[wordsIndex].substr(2,words[wordsIndex].length()-2).c_str());
                if(wordsIndex+len>noOfIntensities)
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
                    std::pair<unsigned long int, double> pair(i,atof(words[wordsIndex].c_str()));
                    m_intensities.insert(pair);
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
          for(unsigned int i = 0; i < inum; ++i)
          {
             double inten = atof(words[26+i].c_str());
             if(inten>0.0000001)
             {
                std::pair<unsigned long int, double> pair(i,atof(words[26+i].c_str()));
                m_intensities.insert(pair);
             }
             else
             {
                // intensity is equal to zero, no need to be added in the hash
             }
          }
        }
    }
    else
    {
       std::cout << "WARNING: Object has been partially created, only use this"
                 << " option for evalueation, otherwise segmentation fault may"
                 << " occur! creating Object\n";
    }
}



//-----------------------------------------------------------------------------
VolumeHashed1DArray::~VolumeHashed1DArray()
{
}

