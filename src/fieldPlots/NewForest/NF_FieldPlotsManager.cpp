#include "NF_FieldPlotsManager.h"
#include "CreateTreeTemplates.h"
#include <iostream>
#include <algorithm>
#include <vector>

//-----------------------------------------------------------------------------
NF_FieldPlotsManager::NF_FieldPlotsManager()
{
}

//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::readAllFieldPlots()
{
   for(unsigned int i=1; i<=21; ++i)
   {
      if(i==12)
      {
          std::cout << "warning: the total station faulty for plot 12.png\n";
      }
      std::string filename = "FieldPlots/" + std::to_string(i) + ".txt";
      addPlot(filename);
   }
}

//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::createTemplates(const std::string &i_volumeFile,
        const std::string &i_templateFileName,
        unsigned int short i_x,
        unsigned int short i_y,
        unsigned int short i_z,
        bool svm
        ) const
{
   CreateTreeTemplates myTemplates(i_volumeFile);
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      for(unsigned int j=0; j< m_plots[i]->m_trees.size(); ++j)
      {
         if((m_plots[i]->m_trees[j]->xGPS>=-0.000001 && m_plots[i]->m_trees[j]->xGPS<=0.000001) ||
            (m_plots[i]->m_trees[j]->yGPS>=-0.000001 && m_plots[i]->m_trees[j]->yGPS<=0.000001) )
         {}else
         {
           myTemplates.addTemplate(m_plots[i]->m_trees[j]->xGPS,m_plots[i]->m_trees[j]->yGPS,true);
         }
      }

   }
   if(svm)
   {
       std::cout << "Creating Templates for SVM\n";
       myTemplates.exportTemplatesForLibSVM(i_x,i_y,i_z,i_templateFileName);
   }
   else
   {
      std::cout << "Creating Templates\n";
      myTemplates.exportTemplates(i_x, i_y, i_z, i_templateFileName);
   }
   std::cout << "TEMPLATES EXPORTED!!! :D :D :D \n";
}

//-----------------------------------------------------------------------------
std::vector<double> NF_FieldPlotsManager::getLimits()const
{
   if(m_plots.size()<1)
   {
      std::cout << "Warning: the fieldplot manager contains no fieldplots\n";
      return (std::vector<double> (4,0.0f));
   }
   const std::vector<double> &plotLimit1 = m_plots[0]->getLimits();
   std::vector<double> fieldPlotLimits(4,0.0f);
   fieldPlotLimits[0] = plotLimit1[0];
   fieldPlotLimits[1] = plotLimit1[1];
   fieldPlotLimits[2] = plotLimit1[2];
   fieldPlotLimits[3] = plotLimit1[3];
   for(unsigned int i=1; i<m_plots.size(); ++i)
   {
      const std::vector<double> &plotLimits = m_plots[i]->getLimits();
      fieldPlotLimits[0] = fieldPlotLimits[0]> plotLimits[0]?fieldPlotLimits[0]:plotLimits[0];
      fieldPlotLimits[1] = fieldPlotLimits[1]< plotLimits[1]?fieldPlotLimits[1]:plotLimits[1];
      fieldPlotLimits[2] = fieldPlotLimits[2]> plotLimits[2]?fieldPlotLimits[2]:plotLimits[2];
      fieldPlotLimits[3] = fieldPlotLimits[3]< plotLimits[3]?fieldPlotLimits[3]:plotLimits[3];
   }
   return fieldPlotLimits;
}


//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::exportAsJson()const
{
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      m_plots[i]->exportAsJson();
   }
}

//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::readFieldPlots(
        const std::vector<std::string> &i_fieldplots
        )
{
   for(unsigned int i=0; i<i_fieldplots.size(); ++i)
   {
      addPlot(i_fieldplots[i]);
   }
}

//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::printStatistics()const
{
   unsigned int noOfTrees = 0;
   float totalSize = 0.0f;
   std::vector<std::string> types;
   std::vector<unsigned int> typesCount;
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
//       if(i!=11)
       {
          totalSize+= ((m_plots[i]->m_limits[0]-m_plots[i]->m_limits[1]) *
                       (m_plots[i]->m_limits[2]-m_plots[i]->m_limits[3]));
          noOfTrees+= m_plots[i]->getNoOfTrees();
          for(unsigned int j=0; j<m_plots[i]->m_trees.size(); ++j)
          {
             std::string typeName((*m_plots[i]).m_trees[j]->type);
              std::transform(typeName.begin(), typeName.end(), typeName.begin(), toupper);

             unsigned int t=0;
             for(; t<types.size(); ++t)
             {
                if(typeName==types[t])
                {
                   typesCount[t]++;
                   break;
                }
             }
             if(t==types.size())
             {
                types.push_back(typeName);
                typesCount.push_back(1);
             }
          }
       }
   }
   std::cout << "FIELD PLOTS INFORMATION \n"
             << "------------------------\n"
             << "Area: " << totalSize << " squared units\n"
             << "No of Trees: " << noOfTrees << "\n";

   for(unsigned int i=0; i<types.size(); ++i)
   {
      std::cout << types[i] << " : " << typesCount[i] << "\n";
   }
}

//-----------------------------------------------------------------------------
void NF_FieldPlotsManager::addPlot(const std::string &i_file)
{
   if( i_file!="")
   {
      m_plots.push_back(new NF_FieldPlot(i_file));
   }
   else
   {
      std::cout << "Warning: FieldPlotsManager:"
                <<" Trying to add a non existing plot\n";
   }
}

//-----------------------------------------------------------------------------
NF_FieldPlotsManager::~NF_FieldPlotsManager()
{
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      if(m_plots[i]!=NULL)
      {
         delete m_plots[i];
      }
   }
}
