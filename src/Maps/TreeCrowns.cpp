#include "TreeCrowns.h"
#include <math.h>

//-----------------------------------------------------------------------------
TreeCrowns::TreeCrowns(
        const std::string i_name,
        Volume *i_obj,
        const std::string i_templatesFileName
        ):
    Map(i_name,i_obj),
    m_positiveTemplates(i_templatesFileName)
{
    std::cout << "Templates saved\n";
}


//-----------------------------------------------------------------------------
void TreeCrowns::createMap()
{
   // template dimensions of the tree positive templates
   unsigned short int dX_pos = m_positiveTemplates.getNoOfVoxelsX();
   unsigned short int dY_pos = m_positiveTemplates.getNoOfVoxelsY();
   unsigned short int dZ_pos = m_positiveTemplates.getNoOfVoxelsZ();
   unsigned short int noOfTemplates = m_positiveTemplates.getNoOfTemplates();

   std::fill(m_mapValues.begin(),m_mapValues.end(),-0.0f);
   float maxValue = -0.00001f;
   float lengthOfTemplate = dX_pos*dY_pos*dZ_pos;
   for(unsigned int x=dX_pos; x<m_noOfPixelsX-dX_pos; ++x)
   {
      for(unsigned int y=dY_pos; y<m_noOfPixelsY-dY_pos; ++y)
      {
//         unsigned int closestTemplate(0);
         float smallerSquaredDiff(3400000000.0f);
         int z=m_noOfPixelsZ-1;
         for(;z>=0; --z)
         {
            if(isInside(x,y,z))
            {
               break;
            }
         }
         if((z-floor(double(dZ_pos)/2))<=0)
         {
            m_mapValues[getIndex(x,y)] = -0.00001f;
         }
         else
         {
            for(unsigned int t=0; t<noOfTemplates;++t)
            {
               float squaredDiff = 0.0f;
               unsigned int xv = x-floor(dX_pos/2);
               unsigned int yv = y-floor(dY_pos/2);
               unsigned int zv = z-floor(dZ_pos/2);
               for(unsigned int xt=0; xt<dX_pos; ++xt)
               {
                  for(unsigned int yt=0; yt<dY_pos; ++yt)
                  {
                     for(unsigned int zt=0;zt<dZ_pos; ++zt) //check if z+ exist
                     {
                        float diff = m_positiveTemplates.getValue(t,xt,yt,zt) -
                                     m_object->getIntensity(xv+xt,yv+yt,zv+zt);
                        squaredDiff += (((diff)*(diff))/lengthOfTemplate);
                     }
                  }
               }
               if(squaredDiff<smallerSquaredDiff)
               {
                  smallerSquaredDiff = squaredDiff;
//                  closestTemplate = t;
               }
            }
            if(smallerSquaredDiff<3399999999.0f)
            {
               m_mapValues[getIndex(x,y)]=smallerSquaredDiff;
               if(maxValue<smallerSquaredDiff)
               {
                  maxValue = smallerSquaredDiff;
               }
            }
            else
            {
               m_mapValues[getIndex(x,y)]=-0.00001f;
            }
         }
      }
   }

   std::cout << "Max Value = " << maxValue << "\n";
   for(unsigned int x=dX_pos; x<m_noOfPixelsX-dX_pos; ++x)
   {
      for(unsigned int y=dY_pos; y<m_noOfPixelsY-dY_pos; ++y)
      {
         unsigned int index = getIndex(x,y);
         if(m_mapValues[index]>-0.00000001f)
         {
            m_mapValues[index] = maxValue-m_mapValues[index];
         }
      }
   }
}


//-----------------------------------------------------------------------------
TreeCrowns::~TreeCrowns()
{}
