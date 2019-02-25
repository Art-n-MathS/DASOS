//#include "FieldPlotOnHeightMap.h"


//#include <QImage>
//#include <gmtl/Vec.h>


////-----------------------------------------------------------------------------
//FieldPlotOnHeightMap::FieldPlotOnHeightMap(
//        const std::string i_name,
//        Volume *i_obj,
//        const std::string &i_fieldPlot
//        ):
//    Map(i_name,i_obj),
//    m_fieldPlot(i_fieldPlot)
//{
//}

////-----------------------------------------------------------------------------
//void FieldPlotOnHeightMap::saveMapToImage()
//{
////   std::cout << " ===================================================\n";
//   if (m_mapValues.size()!=m_noOfPixelsX*m_noOfPixelsY)
//   {
//      std::cout << "Length of i_mapValues is wrong! Image map not saved\n";
//      return;
//   }
//   QImage *image =new QImage(m_noOfPixelsX, m_noOfPixelsY,QImage::Format_RGB16);
//   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
//   {
//      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
//      {
//         int value = (m_mapValues[getIndex(x,y)]);
//         image->setPixel(x,m_noOfPixelsY-1-y,qRgb(value,value,value));
//      }
//   }
//   unsigned int noOfTree = m_fieldPlot.getNoOfTrees();

//   for(unsigned int i=0; i<noOfTree; ++i)
//   {
//      const NF_FieldPlot::treeInfo *tree = m_fieldPlot.getTreeInfo(i);
//      const gmtl::Vec2i pixel = m_object->getIndices(tree->xGPS,tree->yGPS);
//      image->setPixel(pixel[0],m_noOfPixelsY-1-pixel[1],qRgb(255,0,0));
////      std::cout << pixel[0] << " " << pixel[1] << "\n";
//   }

//   image->save(m_name.c_str());
//   delete image;
//}


//// that's copy and paste from the HeightMap.cpp (15th Oct 2015)
////-----------------------------------------------------------------------------
//void FieldPlotOnHeightMap::createMap()
//{
//    for(unsigned int x=0; x<m_noOfPixelsX; ++x)
//    {
//       for(unsigned int y=0; y<m_noOfPixelsY; ++y)
//       {
//          unsigned int z1 = 0;
//          for(int z=m_noOfPixelsZ-1; z>=0; --z)
//          {
//             if(!isInside(x,y,z))
//             {
//                z1++;
//             }
//             else
//             {
//                break;
//             }
//          }
//          if(z1==m_noOfPixelsZ)
//          {
//             m_mapValues[getIndex(x,y)] =-0.0f;
//          }
//          else
//          {
//              m_mapValues[getIndex(x,y)] = m_object->getMaxLimits()[2]-float(z1)*m_object->getVoxelLen();
//          }
//       }
//    }
//}

////-----------------------------------------------------------------------------
//FieldPlotOnHeightMap::~FieldPlotOnHeightMap()
//{
//}
