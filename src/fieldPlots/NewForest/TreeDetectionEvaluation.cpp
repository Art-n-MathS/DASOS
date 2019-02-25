//#include "TreeDetectionEvaluation.h"
//#include <QImage>
//#include <QString>
//#include <QColor>
//#include <iostream>
//#include <gmtl/Vec.h>
//#include <Volume1DArray.h>


////-----------------------------------------------------------------------------
//TreeDetectionEvaluation::TreeDetectionEvaluation(
//        const std::string &i_obj,
//        std::string i_resutlsImg,
//        const std::string &i_fielplot
//        ):
//    m_obj(new Volume1DArray(i_obj,true)),
//    m_fieldplot(i_fielplot)
//{
//    QString fileName(i_resutlsImg.c_str());

//    m_image = QImage(fileName);
//}

////-----------------------------------------------------------------------------
//void TreeDetectionEvaluation::printResults()
//{
//   // {maxNorthY, minNorthY, maxEastX, minEastX}
//   const std::vector<double> &limits = m_fieldplot.getLimits();
//   if(limits.size()!=4)
//   {
//      std::cout << "WARNING: missing information about fieldplot limits\n";
//      return;
//   }
//   // the boundaries for the evaluation in voxel space
//   gmtl::Vec2i maxIndices = m_obj->getIndices(limits[2],limits[0]);
//   gmtl::Vec2i minIndices = m_obj->getIndices(limits[3],limits[1]);

//   std::vector< std::pair<int,  int> > classifiedTrees;
//   std::vector< std::pair<int,  int> > actualTrees;

//   std::cout << "max and min of fieldplot : " << maxIndices[0] << " " << maxIndices[1] << " " << minIndices[0] << " " << minIndices[1] << "\n";
//   std::cout << "image : " << m_image.width() << " " << m_image.height();
//   std::cout << "object : " << m_obj->getNoVoxelsX() << " " << m_obj->getNoVoxelsY() << "\n";

//   std::cout << "Classsified trees are: \n";
//   for(int x= minIndices[0]; x<=maxIndices[0]; ++x)
//   {
//      for(int y=minIndices[1]; y<=maxIndices[1]; ++y)
//      {
//         QColor colour(m_image.pixel(QPoint(x,y)));
//         qreal r,g,b;
//         colour.getRgbF(&r,&g,&b);
//         if(colour.red()>0.99f && colour.green()<0.001f && colour.blue()<0.0001f)
//         {
//            // pixel classified as tree, so add to the classifiedTrees
//            classifiedTrees.push_back(std::make_pair(x,y));
//         }
//      }
//   }
//   std::cout << "\nactual trees are: \n";
//   unsigned int noOfTreeInFieldPlot = m_fieldplot.getNoOfTrees();
//   for(unsigned int i=0; i<noOfTreeInFieldPlot; ++i)
//   {
//      const NF_FieldPlot::treeInfo *tree = m_fieldplot.getTreeInfo(i);
//      gmtl::Vec2i indices = m_obj->getIndices(tree->xGPS,tree->yGPS);
//      actualTrees.push_back(std::make_pair(indices[0],indices[1]));
//   }

//   unsigned int falsePos = 0;
//   unsigned int falseNeg = 0;
//   unsigned int truePos = 0;
//   unsigned int trueNeg = (maxIndices[0]-minIndices[0])*(maxIndices[1]-minIndices[1])-actualTrees.size();

//   for(unsigned int i=0; i<actualTrees.size(); ++i)
//   {
//      bool IsTruePos = false;
//      for(unsigned int j=0; j<classifiedTrees.size(); ++j)
//      {
//        if(actualTrees[i].first == classifiedTrees[j].first &&
//           actualTrees[i].second == classifiedTrees[j].second)
//        {
//           truePos++;
//           IsTruePos = true;
//        }
//      }
//      if(!IsTruePos && actualTrees[i].first>= minIndices[0] && actualTrees[i].first <=maxIndices[0] &&
//              actualTrees[i].second>= minIndices[1] && actualTrees[i].second <=maxIndices[1])
//      {
//         falseNeg++;
//      }
//   }

//   falsePos = classifiedTrees.size()-truePos;
//   trueNeg-=falsePos;

//   std::cout  << "True Positive  : " << truePos  << "\n";
//   std::cout  << "False Positive : " << falsePos << "\n";
//   std::cout  << "False Negative : " << falseNeg << "\n";
//   std::cout  << "True Negative  : " << trueNeg  << "\n";

//   std::cout << " No of Classified vs actual : " << classifiedTrees.size() << " " << actualTrees.size() <<"\n";

//}

////-----------------------------------------------------------------------------
//TreeDetectionEvaluation::~TreeDetectionEvaluation()
//{
//   if(m_obj!=NULL)
//   {
//      delete m_obj;
//   }
//}
