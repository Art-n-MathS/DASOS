#include "HyperStandardDeviation.h"


//-----------------------------------------------------------------------------
HyperStandardDeviation::HyperStandardDeviation(
        const std::string &i_name,
        Object *i_obj,
        const std::string &/*i_bilFileName*/,
        const std::string &i_IGMfileName
        ):
    Map(i_name,i_obj),
    m_IGMfileName(i_IGMfileName)
{

}


//-----------------------------------------------------------------------------
void HyperStandardDeviation::createMap()
{
//    try
//    {
//       bilLib::BinFile file(m_IGMfileName);
//       unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
//       unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
//       unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

//       Grid *grid = new Grid(m_IGMfileName,m_object->m_lengthOfVoxel,
//                             m_object->getMinLimits());
//       const ngl::Vec3 &mins = m_object->getMinLimits();
//       const float vl(m_object->m_lengthOfVoxel);


//       unsigned short int *m_hyperData = new unsigned short int[nlines*nsamps];
//       const unsigned int noX = m_object->getNoVoxelsX();
//       const unsigned int noY = m_object->getNoVoxelsY();
//       unsigned int *pixPos = NULL;
//       float tempValue = 0;

//       // add one band to the map each time to avoid loading all the data
//       // to memory
//       for(unsigned int b=0; b<nbands; ++b)
//       {
//          file.Readband((char *)m_hyperData,b);
//          for(unsigned int x=0; x<noX; ++x)
//          {
//             for(unsigned int y=0; y<noY; ++y)
//             {
//                const ngl::Vec2 point(mins.m_x+vl/2+ vl*x,mins.m_y+vl/2+vl*y) ;
//                const unsigned int noOfPix =
//                        grid->pixIndicesOfSquare(point,&pixPos);
//                tempValue = 0;
//                for(unsigned int i=0; i<noOfPix; ++i)
//                {
//                  tempValue += m_hyperData[pixPos[i]];
//                }
//                tempValue/=noOfPix;
//                m_mapValues[x+y*m_noOfPixelsX]+=tempValue;
//                delete []pixPos;
//             }
//          }
//       }

//       // normalise to get the actual mean value;
//       for( float &v : m_mapValues)
//       {
//          v/=nbands;
//       }

//       file.Close();
//    }
//    catch(bilLib::BinaryReader::BRexception e)
//    {
//       std::cout<<e.what()<<std::endl;
//       std::cout<<e.info<<std::endl;
//    }
}

//-----------------------------------------------------------------------------
HyperStandardDeviation::~HyperStandardDeviation()
{

}

