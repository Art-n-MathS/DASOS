//#include "NF_FieldPlotManagerByType.h"

//#include <iostream>
//#include <fstream>
//#include <iterator>
//#include <string>
//#include <vector>
//#include "CreateTreeTemplates.h"

////-----------------------------------------------------------------------------
//NF_FieldPlotManagerByType::NF_FieldPlotManagerByType()
//{

//}



////-----------------------------------------------------------------------------
//void NF_FieldPlotManagerByType::readFieldPlots(
//        const std::vector<std::string> &i_fieldplots
//        )
//{
//   for(unsigned int f=0; f<i_fieldplots.size(); ++f)
//   {
//       std::ifstream mystream(i_fieldplots[f].c_str());
//       if(!mystream)
//       {
//          std::cerr << "File \"" << i_fieldplots[f] << "\" not found.\n";
//       }
//       std::istream_iterator< std::string > it(mystream);
//       std::istream_iterator< std::string > sentinel;

//       std::vector < std::string > words(it,sentinel);



//       unsigned int NoOfTrees = atoi(words[6].c_str());
//       if(words.size()!= 7+(NoOfTrees+1)*5)
//       {
//          std::cerr << "ERROR: File format wrong or No of trees is wrong\n";
//          return;
//       }

//       for(unsigned int i=0; i<NoOfTrees;++i)
//       {
//          unsigned int index =0;
//          for(;index<m_treeTypes.size()
//              &&m_treeTypes[index]!=words[12+i*5+2];++index);
//          if(index<m_treeTypes.size())
//          {
//             m_plots[index]->addTreeToPlot(
//             ((words[12+i*5  ]!="<unknown>") ? atof(words[12+i*5  ].c_str()): 0.0f),
//             ((words[12+i*5+1]!="<unknown>") ? atof(words[12+i*5+1].c_str()): 0.0f),
//             ((words[12+i*5+2]!="<unknown>") ?      words[12+i*5+2]: ""),
//             ((words[12+i*5+3]!="<unknown>") ? atof(words[12+i*5+3].c_str()): 0.0f),
//             ((words[12+i*5+4]!="<unknown>") ? atof(words[12+i*5+4].c_str()): 0.0f));
//          }
//          else
//          {
//             m_treeTypes.push_back(words[12+i*5+2]);
//             m_plots.push_back(new NF_FieldPlot);
//             m_plots[m_plots.size()-1]->addTreeToPlot(
//             ((words[12+i*5  ]!="<unknown>") ? atof(words[12+i*5  ].c_str()): 0.0f),
//             ((words[12+i*5+1]!="<unknown>") ? atof(words[12+i*5+1].c_str()): 0.0f),
//             ((words[12+i*5+2]!="<unknown>") ?      words[12+i*5+2]: ""),
//             ((words[12+i*5+3]!="<unknown>") ? atof(words[12+i*5+3].c_str()): 0.0f),
//             ((words[12+i*5+4]!="<unknown>") ? atof(words[12+i*5+4].c_str()): 0.0f));
//          }
//       }
//   }
//}

////-----------------------------------------------------------------------------
//void NF_FieldPlotManagerByType::createTemplates(
//        const std::string & i_volumeFile,
//        const std::string & i_templateFileName,
//        unsigned short i_x,
//        unsigned short i_y,
//        unsigned short i_z,
//        bool svm
//        ) const
//{
//   for(unsigned int i=0; i<m_plots.size(); ++i)
//   {
//      std::cout << "starting to look into the " << i << "th template\n";
//      std::string templateFileName = m_treeTypes[i]+i_templateFileName;


//      std::cout << "outputname will be: " << templateFileName << "\n";

//      CreateTreeTemplates myTemplates(i_volumeFile);

//      for(unsigned int j=0; j< m_plots[i]->m_trees.size(); ++j)
//      {
//         if((m_plots[i]->m_trees[j]->xGPS>=-0.000001 && m_plots[i]->m_trees[j]->xGPS<=0.000001) ||
//            (m_plots[i]->m_trees[j]->yGPS>=-0.000001 && m_plots[i]->m_trees[j]->yGPS<=0.000001) )
//         {
//             std::cout << "Warning: GPS position not specified\n";
//         }
//         else
//         {
//           myTemplates.addTemplate(m_plots[i]->m_trees[j]->xGPS,m_plots[i]->m_trees[j]->yGPS,true);
//         }
//      }

//      if(svm)
//      {
//          myTemplates.exportTemplatesForLibSVM(i_x,i_y,i_z,templateFileName);
//      }
//      else
//      {
//         myTemplates.exportTemplates(i_x, i_y, i_z, templateFileName);
//      }
//   }
//   std::cout << "ALl templates are saved!\n";
//}


////-----------------------------------------------------------------------------
//NF_FieldPlotManagerByType::~NF_FieldPlotManagerByType()
//{}





















