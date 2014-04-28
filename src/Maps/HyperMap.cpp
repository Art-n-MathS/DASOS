#include "HyperMap.h"
#include "Grid.h"
#include <bilLib/binfile.h>

//-----------------------------------------------------------------------------
HyperMap::HyperMap(
        const std::string &i_name,
        Object *i_obj,
        const unsigned short int i_band,
        const std::string &i_bilFileName,
        const std::string &i_IGMfileName
        ):
    Map(i_name,i_obj),
    m_IGMfileName(i_IGMfileName),
    m_hyperData(NULL)
{
   // read the band of our interest and put the data into an array
   try
   {
      bilLib::BinFile file(i_bilFileName);
      unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
      unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
      unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

      if(i_band>nbands)
      {
          std::cout<<"The selected Band does not exist!\n";
          return;
      }

      m_hyperData = new unsigned short int[nlines*nsamps];
      file.Readband((char *)m_hyperData,i_band);

      unsigned int k= 0;
      while(m_hyperData[k]==0 && k<nsamps*nlines)
      {
         k++;
      }
      unsigned int short min = m_hyperData[k];
      unsigned int short max = m_hyperData[k];
      for(unsigned int i=k; i<nsamps*nlines; ++i)
      {
         if(min>m_hyperData[i] && m_hyperData[i]!=0)
         {
            min = m_hyperData[i];
         }
         else if (max<m_hyperData[i])
         {
            max = m_hyperData[i];
         }
      }
      for(unsigned int i=0; i<nsamps*nlines; ++i)
      {
         if(m_hyperData[i]!=0)
         {
             m_hyperData[i] = double(double(m_hyperData[i]-min)/(max-min))*255;
         }
      }
      file.Close();
   }
   catch(bilLib::BinaryReader::BRexception e)
   {
      std::cout<<e.what()<<std::endl;
      std::cout<<e.info<<std::endl;
   }
}


//-----------------------------------------------------------------------------
void HyperMap::createMap()
{
   Grid *grid = new Grid(m_IGMfileName,m_object->m_lengthOfVoxel,
                         m_object->getMinLimits());
   const ngl::Vec3 &mins = m_object->getMinLimits();
   const float vl(m_object->m_lengthOfVoxel);

   const unsigned int noX = m_object->getNoVoxelsX();
   const unsigned int noY = m_object->getNoVoxelsY();
   unsigned int *pixPos = NULL;
   for(unsigned int x=0; x<noX; ++x)
   {
      for(unsigned int y=0; y<noY; ++y)
      {
         const ngl::Vec2 point(mins.m_x+vl/2+ vl*x,mins.m_y+vl/2+vl*y) ;
         const unsigned int noOfPix = grid->pixIndicesOfSquare(point,&pixPos);

         for(unsigned int i=0; i<noOfPix; ++i)
         {
            m_mapValues[x+y*m_noOfPixelsX] += m_hyperData[pixPos[i]];
         }
         m_mapValues[x+y*m_noOfPixelsX]/=noOfPix;
         delete []pixPos;
      }
   }
   delete grid;
}

//-----------------------------------------------------------------------------
HyperMap::~HyperMap()
{

}
