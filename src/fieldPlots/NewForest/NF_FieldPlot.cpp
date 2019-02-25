#include "NF_FieldPlot.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

//-----------------------------------------------------------------------------
NF_FieldPlot::NF_FieldPlot(const std::string &i_file):m_name(i_file)
{
   std::ifstream mystream(i_file.c_str());
   if(!mystream)
   {
      std::cerr << "FieldPlot: File \"" << i_file << "\" not found.\n";
   }
   std::istream_iterator< std::string > it(mystream);
   std::istream_iterator< std::string > sentinel;

   std::vector < std::string > words(it,sentinel);

   m_limits.resize(4);
   m_limits[0] = atof(words[1].c_str());
   m_limits[1] = atof(words[2].c_str());
   m_limits[2] = atof(words[3].c_str());
   m_limits[3] = atof(words[4].c_str());

   if(m_limits[0]<m_limits[1] || m_limits[2]<m_limits[3])
   {
      std::cout << "WARNING: Limits of " << i_file << " are wrong\n";
   }

   std::cout << "Field Plot Limits : " <<  m_limits[0] << " " << m_limits[1] << " " << m_limits[2] << " " << m_limits[3] << "\n";

   m_trees.resize(atoi(words[6].c_str()));
   for(unsigned int i=0; i<m_trees.size(); ++i)
   {
      m_trees[i] = new treeInfo;
   }

   if(words.size()!= 7+(m_trees.size()+1)*5)
   {
      std::cerr << "ERROR: File format wrong or No of trees is wrong\n";
      return;
   }


   for(unsigned int i=0; i<m_trees.size();++i)
   {
      m_trees[i]->xGPS   = (words[12+i*5  ]!="<unknown>") ? atof(words[12+i*5  ].c_str()): 0.0f ;
      m_trees[i]->yGPS   = (words[12+i*5+1]!="<unknown>") ? atof(words[12+i*5+1].c_str()): 0.0f ;
      m_trees[i]->type   = (words[12+i*5+2]!="<unknown>") ?      words[12+i*5+2]: "" ;
      m_trees[i]->Age    = (words[12+i*5+3]!="<unknown>") ? atof(words[12+i*5+3].c_str()): 0.0f ;
      m_trees[i]->Height = (words[12+i*5+4]!="<unknown>") ? atof(words[12+i*5+4].c_str()): 0.0f ;
      std::transform(m_trees[i]->type.begin(), m_trees[i]->type.end(), m_trees[i]->type.begin(), toupper);
   }

}

//-----------------------------------------------------------------------------
const std::vector<double> &NF_FieldPlot::getLimits()const
{
   return m_limits;
}

//-----------------------------------------------------------------------------
NF_FieldPlot::NF_FieldPlot()
{
}


//-----------------------------------------------------------------------------
void NF_FieldPlot::addTreeToPlot(
        float i_xGPS,
        float i_yGPS,
        std::string i_type,
        float i_age,
        float i_height
        )
{
    std::cout << " GPS : " << i_xGPS << " " << i_yGPS << "\n";
    m_trees.push_back(new treeInfo);
    m_trees[m_trees.size()-1]->xGPS   = i_xGPS;
    m_trees[m_trees.size()-1]->yGPS   = i_yGPS;
    m_trees[m_trees.size()-1]->type   = i_type;
    m_trees[m_trees.size()-1]->Age    = i_age;
    m_trees[m_trees.size()-1]->Height = i_height;
}

//{maxNorthY, minNorthY, maxEastX, minEastX}
//-----------------------------------------------------------------------------
void NF_FieldPlot::exportAsJson()const
{
   std::string i_filename = m_name + ".json";
   std::ofstream myfile (i_filename);
   if (myfile.is_open())
   {
      myfile << "{\n \"type\": \"FeatureCollection\",\n   \"features\": [\n";
//      for(unsigned int i=0; i<m_trees.size(); ++i)
      {
         myfile << "     { \"type\": \"Feature\",\n\"bbox\": [" << m_limits[3] << "," << m_limits[1] << ", " << m_limits[2] << ", " << m_limits[0];
         myfile << "],\n\"geometry\": {\"type\": \"Point\", \"coordinates\": [";
         myfile  << std::setprecision(4) << m_trees[0]->xGPS << "," << m_trees[0]->yGPS;
         myfile << "]},\n\"properties\": {\" file: \" : \"" << m_name <<"  \"}\n}";

//         if(i!=m_trees.size()-1)
//         {
//            myfile<<",";
//         }
         myfile << "\n";
      }
      myfile << " ]\n}\n";
      myfile.close();
      std::cout << i_filename << " exported\n";
   }
   else
   {
      std::cout << "Unable to open file : " << i_filename << "\n";
   }
}

//-----------------------------------------------------------------------------
const NF_FieldPlot::treeInfo *NF_FieldPlot::getTreeInfo(unsigned int i_i)
{
   return m_trees[i_i];
}


//-----------------------------------------------------------------------------
unsigned int NF_FieldPlot::getNoOfTrees() const
{
   return m_trees.size();
}

//-----------------------------------------------------------------------------
NF_FieldPlot::~NF_FieldPlot()
{
   for (unsigned int i=0; i<m_trees.size(); ++i)
   {
      delete m_trees[i];
   }
}
