#include "TreeCrownsWith2Templates.h"
#include <math.h>

//-----------------------------------------------------------------------------
TreeCrownsWith2Templates::TreeCrownsWith2Templates(const std::string i_name,
        Volume *i_obj,
        const std::string i_PositiveTemplatesFileName,
        const std::string i_NegativeTemplatesFileName
      ):
    Map(i_name,i_obj),
    m_positiveTemplates(i_PositiveTemplatesFileName),
    m_negativeTemplates(i_NegativeTemplatesFileName)
{
    std::cout << "Templates saved\n";
}



//-----------------------------------------------------------------------------
void TreeCrownsWith2Templates::createMap()
{
    // template dimensions of the tree positive templates
    unsigned short int dX_pos = m_positiveTemplates.getNoOfVoxelsX();
    unsigned short int dY_pos = m_positiveTemplates.getNoOfVoxelsY();
    unsigned short int dZ_pos = m_positiveTemplates.getNoOfVoxelsZ();
    unsigned short int noOfPosTemplates = m_positiveTemplates.getNoOfTemplates();

    // template dimensions of the tree positive templates
    unsigned short int dX_neg = m_negativeTemplates.getNoOfVoxelsX();
    unsigned short int dY_neg = m_negativeTemplates.getNoOfVoxelsY();
    unsigned short int dZ_neg = m_negativeTemplates.getNoOfVoxelsZ();
    unsigned short int noOfNegTemplates = m_negativeTemplates.getNoOfTemplates();

    std::vector<float> negValues(m_mapValues.size());
    std::fill(m_mapValues.begin(),m_mapValues.end(),-0.0f);
    std::fill(negValues.begin(),negValues.end(),-0.0f);


    // comparison with positive templates
    float lengthOfTemplate = dX_pos*dY_pos*dZ_pos;
    for(unsigned int x=dX_pos; x<m_noOfPixelsX-dX_pos; ++x)
    {
       for(unsigned int y=dY_pos; y<m_noOfPixelsY-dY_pos; ++y)
       {
 //         unsigned int closestTemplate(0);
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
             for(unsigned int t=0; t<noOfPosTemplates;++t)
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
                m_mapValues[getIndex(x,y)]+=squaredDiff;
             }
             m_mapValues[getIndex(x,y)]/=noOfPosTemplates;
          }
       }
    }

    //comparison with negative templates
    lengthOfTemplate = dX_neg*dY_neg*dZ_neg;
    for(unsigned int x=dX_neg; x<m_noOfPixelsX-dX_neg; ++x)
    {
       for(unsigned int y=dY_neg; y<m_noOfPixelsY-dY_neg; ++y)
       {
 //         unsigned int closestTemplate(0);
          int z=m_noOfPixelsZ-1;
          for(;z>=0; --z)
          {
             if(isInside(x,y,z))
             {
                break;
             }
          }
          if((z-floor(double(dZ_neg)/2))<=0)
          {
             negValues[getIndex(x,y)] = -0.00001f;
          }
          else
          {
             for(unsigned int t=0; t<noOfNegTemplates;++t)
             {
                float squaredDiff = 0.0f;
                unsigned int xv = x-floor(dX_neg/2);
                unsigned int yv = y-floor(dY_neg/2);
                unsigned int zv = z-floor(dZ_neg/2);
                for(unsigned int xt=0; xt<dX_neg; ++xt)
                {
                   for(unsigned int yt=0; yt<dY_neg; ++yt)
                   {
                      for(unsigned int zt=0;zt<dZ_neg; ++zt) //check if z+ exist
                      {
                         float diff = m_negativeTemplates.getValue(t,xt,yt,zt) -
                                      m_object->getIntensity(xv+xt,yv+yt,zv+zt);
                         squaredDiff += (((diff)*(diff))/lengthOfTemplate);
                      }
                   }
                }
                negValues[getIndex(x,y)]+=squaredDiff;
             }
             negValues[getIndex(x,y)]/=noOfNegTemplates;
          }
       }
    }


   // Merge returns from both templates
   float maxValue = -0.00001f;
   for(unsigned int x=dX_pos; x<m_noOfPixelsX-dX_pos; ++x)
   {
      for(unsigned int y=dY_pos; y<m_noOfPixelsY-dY_pos; ++y)
      {
         unsigned int index = getIndex(x,y);
         m_mapValues[index]=m_mapValues[index]/
                 (negValues[index]+m_mapValues[index]);
         if(maxValue<m_mapValues[index])
         {
            maxValue = m_mapValues[index];
         }
      }
   }

   // reverve values max <-> min

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
TreeCrownsWith2Templates::~TreeCrownsWith2Templates()
{}
