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
        Volume */*i_obj*/
        ):
    m_dtm(i_dtm),
    m_isValid(true),
    m_nRows(0),
    m_nCols(0),
    m_nBits(32),
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
      std::cout << "DTM File not found\n";
      m_isValid=false;
      return;
   }
   readHeader(i_dtm);

   std::cout << "HEADER READ!!\n";

   // fill preloaded heights
   m_heights.resize(m_nRows*m_nCols);
   dtm.read(reinterpret_cast<char*>(&m_heights[0]), m_nCols*m_nRows*sizeof(float));
   dtm.close();

   std::cout << "(+++++++++++) m_heights.size() "<< m_heights.size() << " m_nRows "
             << m_nRows << " - m_nColumns " << m_nCols << "\n";
   std::cout << "Limits of DTM min(x,y) max(x,y): " << m_xMin    << " " << m_yMax
             << " " << m_xMin    << " " << m_yMax    << "\n";
   std::cout << "(+++++++++++) m_nRows " << m_nRows << " - m_nColumns " << m_nCols
             << "\n";
}


//-----------------------------------------------------------------------------
void DtmBilReader::readHeader(const std::string &i_dtm)
{
   std::cout << "READING HEADER :::::::::\n\n";
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
   for(unsigned long long int i=0; i<words.size(); ++i)
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
         std::cout << "+++++++++++ m_nRows " << m_nRows << "\n";
         break;
      case 3:  // NCOLS
         m_nCols = atoi(words[++i].c_str());
         std::cout << "+++++++++++ m_nColumns " << m_nCols << "\n";
         break;
      case 4:  // NBANDS
         nBands = atoi(words[++i].c_str());
         if(nBands!=1)
         {
            std::cout << "WARNING: only first layer used. The rest are ignored";
         }
         break;
      case 5:  // NBITS
         m_nBits = atoi(words[++i].c_str())/8;
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
         std::cout << xMin << " xMin +++++++++++++++++++++++++++++++++++++++\n";
         break;
      case 8:  // ULYMAP
         m_yMax = atof(words[++i].c_str());
         std::cout << m_yMax << " xMax +++++++++++++++++++++++++++++++++++++\n";
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
   m_yMin = m_yMax-m_nRows*m_yLen;
   m_xMax = m_xMin+m_nCols*m_xLen;
   std::ifstream in;
   in.open(m_dtm.c_str(),std::ios::in | std::ios::binary);
   if(!in.is_open())
   {
      std::cerr << "ERROR: BIL file noT found \n";
      return ;
   }
   in.seekg (0, std::ios::end);
   m_size = in.tellg();
   std::cout << m_size << " " << m_nBits*m_nCols*m_nRows << "\n";
   if(m_size!=m_nBits*m_nCols*m_nRows)
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


//-----------------------------------------------------------------------------
float DtmBilReader::getHeightOf(const float i_x, const float i_y)const
{


   assert(m_heights.size()==m_nRows*m_nCols);
   // get index
   long long int x = floor((i_x-m_xMin)/m_xLen);
   long long int y = m_nRows - floor((i_y-m_yMin)/m_yLen)-1;
   if(!(x<0 || y<0 || x>=m_nRows || y>=m_nCols))
   {

      assert(y*m_nCols+x < m_heights.size());
      {
          if ((m_heights[y*m_nCols+x] > -1000000 ||
               m_heights[y*m_nCols+x] < 1000000 ))
          {
             return 0.0f;
          }
          else
          {
//              std::cout << "**************************\n";
//              std::cout << "A\n";
//              std::cout << "geoX, geoY input = "<< i_x << " " << i_y << "\n";
//              std::cout << "DTM ymax ymin xmax xmin: " << m_yMax << " " << m_yMin << " "<< m_xMax << " " << m_xMin  <<"\n";
//              std::cout << "DTM nRows nCols: " << m_nRows << " " << m_nCols <<"\n";
//              std::cout << " m_xLen m_yLen " << m_xLen << " " << m_yLen << "\n";
//              std::cout << "Indexes x y: " << x << " " << y << "\n";
              return 0.0f;
          }

      }
   }
//   std::cout << "**************************\n";
//   std::cout << "B\n";
//   std::cout << "geoX, geoY input = "<< i_x << " " << i_y << "\n";
//   std::cout << "DTM ymax ymin xmax xmin: " << m_yMax << " " << m_yMin << " "<< m_xMax << " " << m_xMin  <<"\n";
//   std::cout << "DTM nRows nCols: " << m_nRows << " " << m_nCols <<"\n";
//   std::cout << " m_xLen m_yLen " << m_xLen << " " << m_yLen << "\n";
//   std::cout << "Indexes x y: " << x << " " << y << "\n";

   return 0;
}

//-----------------------------------------------------------------------------
DtmBilReader::~DtmBilReader()
{
}

