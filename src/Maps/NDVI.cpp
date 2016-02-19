#include "NDVI.h"
#include "Grid.h"
#include "binfile.h"

//-----------------------------------------------------------------------------
NDVI::NDVI(
        const std::string &i_name,
        Volume *i_obj,
        const std::string &i_bilFileName,
        const std::string &i_IGMfileName,
        const std::string &i_fodisFileName
        ):
    Map(i_name,i_obj),
    m_IGMfileName(i_IGMfileName),
    m_bilFileName(i_bilFileName),
    m_fodisFileName(i_fodisFileName)
{
}

//-----------------------------------------------------------------------------
void NDVI::createMap()
{
    try
     {
        if(m_bilFileName=="" || m_IGMfileName=="")
        {
           std::cout<<"WARNING: hyperspectral files have not been loaded\n";
           return;
        }
        bilLib::BinFile file(m_bilFileName);
        unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
        unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
        unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));
        Grid *grid = new Grid(m_IGMfileName,m_object->getVoxelLen()*1.8,
        m_object->getMinLimits());
        const gmtl::Vec3f &mins = m_object->getMinLimits();
        const float vl(m_object->getVoxelLen());
        unsigned short int *m_hyperData = new unsigned short int[nlines*nsamps];
        const unsigned int noX = m_object->getNoVoxelsX();
        const unsigned int noY = m_object->getNoVoxelsY();
        unsigned int *pixPos = NULL;
        // add one band to the map each time to avoid loading all the data
        // to memory

        std::vector<float> NIR(m_mapValues.size());
        std::vector<float> VIS(m_mapValues.size());
        std::fill(VIS.begin(),VIS.end(),0.0f);
        std::fill(NIR.begin(),NIR.end(),0.0f);
        for(unsigned int b=4; b<132; ++b)
        {
           file.Readband((char *)m_hyperData,b);
           for(unsigned int x=0; x<noX; ++x)
           {
              for(unsigned int y=0; y<noY; ++y)
              {
                 const gmtl::Vec2f point(mins[0]+vl/2+ vl*x,mins[1]+vl/2+vl*y) ;
                 VIS[x+y*m_noOfPixelsX]+=m_hyperData[grid->getClosestPixelPosition(point[0],point[1])];
                 delete []pixPos;
              }
           }
        }
        // normalise to get the actual mean value;
        for( float &v : VIS)
        {
           v/=(nbands-132-4);
        }


        // 132-251
        for(unsigned int b=132; b<nbands; ++b)
        {
           file.Readband((char *)m_hyperData,b);
           for(unsigned int x=0; x<noX; ++x)
           {
              for(unsigned int y=0; y<noY; ++y)
              {
                 const gmtl::Vec2f point(mins[0]+vl/2+ vl*x,mins[1]+vl/2+vl*y) ;
                 NIR[x+y*m_noOfPixelsX]+=m_hyperData[grid->getClosestPixelPosition(point[0],point[1])];
                 delete []pixPos;
              }
           }
        }
        // normalise to get the actual mean value;
        for( float &v : NIR)
        {
           v/=(nbands-132);
        }

        for(unsigned int x=0; x<m_mapValues.size(); ++x)
        {
           m_mapValues[x] = (NIR[x]-VIS[x])/(NIR[x]+VIS[x]);
        }

        file.Close();
        delete grid;
        delete [] m_hyperData;
     }
     catch(bilLib::BinaryReader::BRexception e)
     {
        std::cout<<e.what()<<std::endl;
        std::cout<<e.info<<std::endl;
     }
     std::cout << "MAP IS CREATED!\n";
}

//-----------------------------------------------------------------------------
NDVI::~NDVI()
{}
