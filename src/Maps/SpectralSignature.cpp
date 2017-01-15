#include "SpectralSignature.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <math.h>

//-----------------------------------------------------------------------------
SpectralSignature::SpectralSignature(
        const std::string &i_file,
        const std::string i_type
        )
{
   std::ifstream bvhStream(i_file.c_str());
   if (!bvhStream)
   {
      std::cout << "File \"" << i_file << "\" not found.\n";
      exit(1);
   }
   std::istream_iterator<std::string> bvhIt(bvhStream);
   std::istream_iterator<std::string> sentinel;

   std::vector<std::string> words(bvhIt,sentinel);

   std::string type= "ASTER";

   if(words.size()<1)
   {
      std::cerr << "Error: File located do not contain any signature info\n";
      return;
   }

   if(type == i_type)
   {
      unsigned int i=0;
      for (auto & c: words[i]) c = std::toupper(c);
      while(i<words.size() && (words[i].compare(0,5,"VALUE")!=0))
      {
         i++;
         for (auto & c: words[i]) c = std::toupper(c);
      }
      if(words[++i]==":")
      {
         ++i;
      }

      std::string firstXValue = words[i];
      while(++i<words.size() && (words[i].compare(0,firstXValue.length(),firstXValue)!=0))
      {}


      m_reflectances.reserve((words.size()-i)/2+1);
      m_wavelengths.reserve((words.size()-i)/2+1);
      for(unsigned int k=i; k<words.size(); k+=2)
      {
         m_wavelengths.push_back(atof(words[k].c_str()));
         m_reflectances.push_back(atof(words[k+1].c_str()));
      }
      if(m_reflectances.size()==0)
      {
         std::cerr << "Error: reflectances haven't been saved\n";
         return;
      }
      // reverse if necessary to be in ascending order
      if(m_wavelengths[0]>m_wavelengths[m_wavelengths.size()-1])
      {
         std::reverse(m_wavelengths.begin(),m_wavelengths.end());
         std::reverse(m_reflectances.begin(),m_reflectances.end());
      }
      for(unsigned int k=0; k<m_reflectances.size(); ++k)
      {
         // convert wavelengths to nanometer to match the scanned data
         m_wavelengths[k]*=100.0f;
         m_reflectances[k]/=100.0f;
      }
   }
   else
   {
      unsigned int i=0;
      for (auto & c: words[i]) c = std::toupper(c);
      while(i<words.size() && (words[i].compare(0,4,"COPY")!=0))
      {
         i++;
         for (auto & c: words[i]) c = std::toupper(c);
      }
      i+=5;
      m_reflectances.reserve((words.size()-i)/3+1);
      m_wavelengths.reserve((words.size()-i)/3+1);
      for(unsigned int k=i; k<words.size(); k+=3)
      {
         m_wavelengths.push_back(atof(words[k].c_str()));
         m_reflectances.push_back(atof(words[k+1].c_str()));
      }
      if(m_reflectances.size()==0)
      {
         std::cerr << "Error: reflectances haven't been saved\n";
         return;
      }
      // reverse if necessary to be in ascending order
      if(m_wavelengths[0]>m_wavelengths[m_wavelengths.size()-1])
      {
         std::reverse(m_wavelengths.begin(),m_wavelengths.end());
         std::reverse(m_reflectances.begin(),m_reflectances.end());
      }
      for(unsigned int k=0; k<m_reflectances.size(); ++k)
      {
         // convert wavelengths to nanometer to match the scanned data
         m_wavelengths[k]*=1000.0f;
         m_reflectances[k]*=100.0f;
         std::cout << m_wavelengths[k] << " " << m_reflectances[k] << "\n";
      }
   }


   std::cout << " " << m_reflectances.size() << "\n";

//   // scale to be from 0 to 100
//   float max = m_reflectances[0], min =m_reflectances[0];
//   for(unsigned int k=1; k<m_reflectances.size(); ++k)
//   {
//      if(max<m_reflectances[k])
//      {
//         max = m_reflectances[k];
//      }
//      else if (min>m_reflectances[k])
//      {
//          min = m_reflectances[k];
//      }
//   }


}

//-----------------------------------------------------------------------------
float SpectralSignature::percentageError(
        const std::vector<float> &i_wavelength,
        const std::vector<float> &i_reflectance
        )
{
   unsigned int w = 0;
   float sum = 0;
   for(unsigned int i=0; i<i_reflectance.size();++i)
   {
      while(m_wavelengths[++w]<i_wavelength[i])
      {}
      float A = fabs(m_reflectances[w  ]-i_reflectance[i]);
      float B = fabs(m_reflectances[w-1]-i_reflectance[i]);
      if(A>B)
      {
         sum+=(A*A);
         w--;
      }
      else
      {
         sum+=(B*B);
      }
   }
   return sum/i_reflectance.size();
}

//-----------------------------------------------------------------------------
SpectralSignature::~SpectralSignature()
{}


