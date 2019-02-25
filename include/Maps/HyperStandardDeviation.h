//#include "HyperStandardDeviation.h"
//#include "Grid.h"
//#include "binfile.h"
//#include "math.h"

////-----------------------------------------------------------------------------
///*HyperStandardDeviation*/::HyperStandardDeviation(
//        const std::string &i_name,
//        Object *i_obj,
//        const std::string &i_bilFileName,
//        const std::string &i_IGMfileName,
//        const std::string &i_fodisFileName
//        ):
//    Map(i_name,i_obj),
//    m_IGMfileName(i_IGMfileName),
//    m_bilFileName(i_bilFileName),
//    m_fodisFileName(i_fodisFileName)
//{

//}


////-----------------------------------------------------------------------------
//void HyperStandardDeviation::createMap()
//{
//    try
//    {
//       if(m_bilFileName=="" || m_IGMfileName=="")
//       {
//          std::cout<<"WARNING: hyperspectral files have not been loaded\n";
//          return;
//       }
//       std::cout << "m_bilFileName = " << m_bilFileName << "\n";
//       bilLib::BinFile file(m_bilFileName);
//       unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
//       unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
//       unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

//       Grid *grid = new Grid(m_IGMfileName,m_object->m_lengthOfVoxel*1.8,
//                             m_object->getMinLimits());
//       const gmtl::Vec3f &mins = m_object->getMinLimits();
//       const float vl(m_object->m_lengthOfVoxel);


//       unsigned short int *m_hyperData = new unsigned short int[nlines*nsamps];
//       const unsigned int noX = m_object->getNoVoxelsX();
//       const unsigned int noY = m_object->getNoVoxelsY();
//       unsigned int *pixPos = NULL;
//       float tempValue = 0;

//       std::vector<float> means(m_mapValues.size());
//       // add one band to the map each time to avoid loading all the data
//       // to memory
//       for(unsigned int b=1; b<nbands; ++b)
//       {
//          file.Readband((char *)m_hyperData,b);
//          for(unsigned int x=0; x<noX; ++x)
//          {
//             for(unsigned int y=0; y<noY; ++y)
//             {
//                const gmtl::Vec2f point(mins[0]+vl/2+ vl*x,mins[1]+vl/2+vl*y) ;
//                const unsigned int noOfPix =
//                        grid->pixIndicesOfSquare(point,&pixPos);
//                tempValue = 0;
//                for(unsigned int i=0; i<noOfPix; ++i)
//                {
//                  tempValue += m_hyperData[pixPos[i]];
//                }
//                tempValue/=noOfPix;
//                means[x+y*m_noOfPixelsX]+=tempValue;
//                delete []pixPos;
//             }
//          }
//       }

//       // normalise to get the actual mean value;
//       for( float &v : means)
//       {
//          v/=(nbands-1);
//       }

//       for(unsigned int b=1; b<nbands; ++b)
//       {
//          file.Readband((char *)m_hyperData,b);
//          for(unsigned int x=0; x<noX; ++x)
//          {
//             for(unsigned int y=0; y<noY; ++y)
//             {
//                const gmtl::Vec2f point(mins[0]+vl/2+ vl*x,mins[1]+vl/2+vl*y) ;
//                const unsigned int noOfPix =
//                        grid->pixIndicesOfSquare(point,&pixPos);
//                tempValue = 0;
//                for(unsigned int i=0; i<noOfPix; ++i)
//                {
//                  tempValue += m_hyperData[pixPos[i]];
//                }
//                tempValue/=noOfPix;
//                m_mapValues[x+y*m_noOfPixelsX]+=((means[x+y*m_noOfPixelsX]-tempValue)*(means[x+y*m_noOfPixelsX]-tempValue));
//                delete []pixPos;
//             }
//          }
//       }
//       // normalise to get the actual mean value;
//       for( float &v : m_mapValues)
//       {
//       v=sqrt(v);
//       }


//       file.Close();
//       delete grid;
//       delete [] m_hyperData;
//    }
//    catch(bilLib::BinaryReader::BRexception e)
//    {
//       std::cout<<e.what()<<std::endl;
//       std::cout<<e.info<<std::endl;
//    }
//}

////-----------------------------------------------------------------------------
//HyperStandardDeviation::~HyperStandardDeviation()
//{

//}

