#include "TreeCrownsTemplates.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

//-----------------------------------------------------------------------------
// Templates File Format
//----------------------//
// NoOfTemplates: <No>
// Dimentions: <ie n m k, means that the teplace is an n*m*k template >
// Intensities:
// <intensitity (1,1,1) of template 0> <intensity (2,1,1) of template 0> ... <intensity (n,1,1) of teplate 0>
// <intensitity (1,1,1) of template 1> <intensity (2,1,1) of template 1> ... <intensity (n,1,1) of teplate 1>
// ...
// <intensitity (1,m-1,k-1) of template (No-1)> <intensity (2,m-1,k-1) of template (No-1)> ... <intensity (n,m,k) of teplate (No-1)>
//-----------------------------------------------------------------------------
TreeCrownsTemplates::TreeCrownsTemplates(const std::string i_file)
{
   std::ifstream mystream(i_file.c_str());
   if(!mystream)
   {
      std::cerr << "Error: File \"" << i_file << "\" not found.\n";
   }
   std::istream_iterator< std::string > it(mystream);
   std::istream_iterator< std::string > sentinel;

   std::vector < std::string > words(it,sentinel);

   if(words.size()<8)
   {
      std::cerr << "Error: File too short. Arguments missing \n";
      return;
   }
   unsigned int NoOfTemplates = atoi(words[1].c_str());
   m_dX = atoi(words[3].c_str());
   m_dY = atoi(words[4].c_str());
   m_dZ = atoi(words[5].c_str());
   std::cout << words.size() << " " << (NoOfTemplates*m_dX*m_dY*m_dZ+7) <<"\n";
   if(words.size()!=NoOfTemplates*m_dX*m_dY*m_dZ+7)
   {
      std::cerr<< "Error: File is too short or No of Templates do not match\n";
      return;
   }

   unsigned int wordsIndex = 7;
   unsigned int templateLength = m_dX*m_dY*m_dZ;
   m_templates.resize(NoOfTemplates);
   for(unsigned int i=0; i<NoOfTemplates; ++i)
   {
      m_templates[i] = new float [templateLength];
      for(unsigned int j=0; j<templateLength; ++j)
      {
         m_templates[i][j]=atof(words[wordsIndex].c_str());
         wordsIndex++;
      }
   }

}

//-----------------------------------------------------------------------------
float TreeCrownsTemplates::getValue(
        unsigned short i_index,
        unsigned short i_x,
        unsigned short i_y,
        unsigned short i_z
        )
{
   if(i_index<m_templates.size() && i_x<m_dX && i_y<m_dY && i_z<m_dZ)
   {
      return m_templates[i_index][getIndex(i_x,i_y,i_z)];
   }
   else
   {
      std::cerr << "Error: requesting template value out of boundaries\n";
      return 0.0f;
   }
}

//-----------------------------------------------------------------------------
unsigned short int TreeCrownsTemplates::getIndex(
        unsigned short i_x,
        unsigned short i_y,
        unsigned short i_z
        )
{
   return i_x+i_y*m_dX+i_z*m_dX*m_dY;
}

//-----------------------------------------------------------------------------
unsigned short int TreeCrownsTemplates::getNoOfTemplates()
{
   return m_templates.size();
}

//-----------------------------------------------------------------------------
unsigned short int TreeCrownsTemplates::getNoOfVoxelsX()
{
   return m_dX;
}

//-----------------------------------------------------------------------------
unsigned short int TreeCrownsTemplates::getNoOfVoxelsY()
{
   return m_dY;
}

//-----------------------------------------------------------------------------
unsigned short int TreeCrownsTemplates::getNoOfVoxelsZ()
{
   return m_dZ;
}

//-----------------------------------------------------------------------------
TreeCrownsTemplates::~TreeCrownsTemplates()
{
   for(unsigned int i=0;i<m_templates.size();++i)
   {
      if(m_templates[i]!=NULL)
      {
         delete [] m_templates[i];
      }
   }
}
