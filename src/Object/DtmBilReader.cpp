#include "DtmBilReader.h"
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <limits>
#include <map>
#include <fstream>
#include <math.h>
#include <iomanip>

//-----------------------------------------------------------------------------
DtmBilReader::DtmBilReader(
        const std::string &i_dtm,
        Volume *i_obj
        ):
    m_dtm(i_dtm),
    m_isValid(true),
    m_nRows(0),
    m_nColumns(0),
    m_nBytes(8),
    m_xLen(1.0f),
    m_yLen(1.0f),
    m_xMin(0.0f),
    m_yMin(0.0f),
    m_yMax(0.0f),
    m_xMax(0.0f)
{
   std::ifstream dtm;
   dtm.open(i_dtm,std::ios::binary);
   if(!dtm.is_open())
   {
      m_isValid=false;
      return;
   }
   readHeader(i_dtm);

   // initialise volume parameters
   gmtl::Vec3f minVol = i_obj->getMinLimits();
   gmtl::Vec3f maxVol = i_obj->getMaxLimits();
   // => read nX columns data each time

   if(minVol[0]<m_xMin)
   {
      std::cout << "WARNING: Volume lies outside the limits of the bil file\m";
      minVol[0]=m_xMin;
   }
   if(m_yMax<maxVol[1])
   {
      std::cout << "WARNING: Volume lies outside the limits of the bil file\m";
      maxVol[1] = m_yMax;
   }
   unsigned int offsetX = floor(float(minVol[0]-m_xMin)/m_xLen);
   unsigned int offsetY = floor(float(m_yMax-maxVol[1])/m_yLen);
   minVol[0]=float(offsetX)*m_xLen+m_xMin;
   maxVol[1]=m_yMax - float(offsetY)*m_yLen;

   // noOf voxels at dtm rate
   unsigned int nVolX = ceil((maxVol[0]-minVol[0])/m_xLen);
   unsigned int nVolY = ceil((maxVol[1]-minVol[1])/m_yLen);

   std::cout << nVolX << " " << nVolY << "\n";
   // fill preloaded heights

   m_heights.resize(nVolX*nVolY);
   for(unsigned int rV=0; rV<nVolY;++rV)
   {
      unsigned int rB = rV + offsetY;
      unsigned int bilFileIndex = (rB*m_nColumns+offsetX)*sizeof(float);
      if(bilFileIndex>m_size)
      {
         continue;
      }
      dtm.seekg(bilFileIndex);
      dtm.read((char *) &m_heights[rV*nVolX],nVolX*sizeof(float));
   }
   dtm.close();

   // forget about bil files max min and limits, because area of
   // interest has been preloaded
   m_xMin = minVol[0]; m_xMax = m_xMin + m_xLen * nVolX;
   m_yMax = maxVol[1]; m_yMin = m_yMax - m_yLen * nVolY;
   m_nColumns = nVolX;
   m_nRows = nVolY;
}


//-----------------------------------------------------------------------------
void DtmBilReader::readHeader(const std::string &i_dtm)
{
   std::string header(i_dtm);
   header.replace(header.end()-3,header.end(),"hdr");
   std::ifstream hdrFile;
   hdrFile.open(header);
   if(!hdrFile)
   {
      std::cout <<"ERROR: Header FILE DO NOT EXIST\n";
      m_isValid = false;
      return;
   }
   std::istream_iterator< std::string > it(hdrFile);
   std::istream_iterator< std::string > sentinel;
   std::vector < std::string > words(it,sentinel);

   for(auto &l : words) for (auto & c: l) c = std::toupper(c);

   std::map<std::string, int> tags = {
       {"BYTEORDER",1},
       {"NROWS",2},
       {"NCOLS",3},
       {"NBANDS",4},
       {"NBITS",5},
       {"PIXELTYPE",6},
       {"ULXMAP",7},
       {"ULYMAP",8},
       {"XDIM",9},
       {"YDIM",10},
       {"NODATA",11}
   };

   float xMin = 0.0f ;
   unsigned short int nBands(1);
   for(unsigned int i=0; i<words.size(); ++i)
   {
      switch (tags[words[i]])
      {
      case 1:  // BYTEORDER
         ++i;
         if(( isBigEndian() && words[i]=="M") ||
            (!isBigEndian() && words[i]=="I") )
         {}
         else
         {
            std::cout << "Program do only supports little endian CPUs!\n";
            m_isValid=false;
         }
         break;
      case 2:  // NROWS
         m_nRows = atoi(words[++i].c_str());
         break;
      case 3:  // NCOLS
         m_nColumns = atoi(words[++i].c_str());
         break;
      case 4:  // NBANDS
         nBands = atoi(words[++i].c_str());
         if(nBands!=1)
         {
            std::cout << "WARNING: only first layer used. The rest are ignored";
         }
         break;
      case 5:  // NBITS
         m_nBytes = atoi(words[++i].c_str())/8;
         break;
      case 6:  // PIXELTYPE
         m_pixelType = words[++i];
         for(auto &c : m_pixelType) c = std::toupper(c);
         if(m_pixelType!="FLOAT")
         {
            m_isValid = false;
            std::cout<< "WARNING: BIL reader currently only"
                     << " supports floats\n";
         }
         break;
      case 7:  // ULXMAP
         xMin = atof(words[++i].c_str());
         std::cout << xMin << " xMin\n";
         break;
      case 8:  // ULYMAP
         m_yMax = atof(words[++i].c_str());
         std::cout << m_yMax << " xMax\n";
         break;
      case 9:  // XDIM
         m_xLen = atof(words[++i].c_str());
         break;
      case 10: // YDIM
         m_yLen = atof(words[++i].c_str());
         break;
      case 11: // NODATA
         m_noData = atof(words[++i].c_str());
         break;
      default:
         // unknown or not important tag
         break;
      }
   }
   if(m_xLen<m_yLen-0.0000001 || m_xLen>m_yLen+0.00000001)
   {
      std::cerr << "WARNING: xdim and ydim should be the same size\n";
      m_yLen = m_xLen;
   }
   words.clear();
   hdrFile.close();

   // moving max and min to the corner of the bounding box
   // and calculating the min possible values
   m_xMin = xMin -(m_xLen/2.0f);
   m_yMax +=(m_yLen/2.0f);
   m_xMax = m_yMax-m_nRows*m_yLen + m_yLen/2.0f;
   std::ifstream in;
   in.open(m_dtm.c_str(),std::ios::in | std::ios::binary);
   if(!in.is_open())
   {
      std::cerr << "ERROR: BIL file noT found \n";
      return ;
   }
   in.seekg (0, std::ios::end);
   m_size = in.tellg();
   if(m_size!=m_nBytes*m_nColumns*m_nRows)
   {
      std::cout << "ERROR: size of bil file do not aggree with bytes*cols*rows\n";
      m_isValid = false;
   }
   in.close();
}

//-----------------------------------------------------------------------------
// this function was taken from the following website
// http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
//-----------------------------------------------------------------------------
bool DtmBilReader::isBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

unsigned int count (0),count2(0);

//-----------------------------------------------------------------------------
float DtmBilReader::getHeightOf(const float i_x, const float i_y)const
{
   if(!m_isValid)
   {
      return -0.0f;
   }
   // get index
   unsigned int x = floor((i_x-m_xMin)/m_xLen);
   unsigned int y = m_nRows - floor((i_y-m_yMin)/m_xLen) - 1;
   if(y*m_nColumns+x >= m_heights.size())
   {
      return -0.0f;
   }
   else
   {
       count++;
   }
   return m_heights[y*m_nColumns+x];
}

//-----------------------------------------------------------------------------
DtmBilReader::~DtmBilReader()
{
}

