#include "AverageHeightDifference.h"
#include <math.h>

//-----------------------------------------------------------------------------
AverageHeightDifference::AverageHeightDifference(
        const std::string i_name,
        Object *i_obj
        ):
    Map(i_name,i_obj)
{

}


//-----------------------------------------------------------------------------
void AverageHeightDifference::createMap()
{
   std::cout << "start generating Average Height Difference map\n";
   std::vector<float> heightValues(m_noOfPixelsX*m_noOfPixelsY);
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         unsigned int z1 = 0;
         for(int z=m_noOfPixelsZ-1; z>=0; --z)
         {
            if(!isInside(x,y,z))
            {
               z1++;
            }
            else
            {
               break;
            }
         }
         if(z1==m_noOfPixelsZ)
         {
            heightValues[getIndex(x,y)] =-0.0f;
         }
         else
         {
            heightValues[getIndex(x,y)] = float(m_noOfPixelsZ)-float(z1);
         }
      }
   }

   std::cout <<"hello world\n";
   for(unsigned int x=1; x<m_noOfPixelsX-1; ++x)
   {
      for(unsigned int y=1; y<m_noOfPixelsY-1; ++y)
      {
         if(heightValues[getIndex(x,y)]>0.0001f)
         {
            m_mapValues[getIndex(x,y)]= heightValues[getIndex(x-1,y-1)] +
                 heightValues[getIndex(x,y-1)]+heightValues[getIndex(x+1,y-1)]+
                 heightValues[getIndex(x-1,y)]+heightValues[getIndex(x+1,y)]+
                 heightValues[getIndex(x-1,y+1)]+heightValues[getIndex(x,y+1)]+
                 heightValues[getIndex(x+1,y+1)];
            float div = (heightValues[getIndex(x-1,y-1)]>0.0001f) +
                     (heightValues[getIndex(x  ,y-1)]>0.0001f) +
                     (heightValues[getIndex(x+1,y-1)]>0.0001f) +
                     (heightValues[getIndex(x-1,y  )]>0.0001f) +
                     (heightValues[getIndex(x+1,y  )]>0.0001f) +
                     (heightValues[getIndex(x-1,y+1)]>0.0001f) +
                     (heightValues[getIndex(x  ,y+1)]>0.0001f) +
                     (heightValues[getIndex(x+1,y+1)]>0.0001f);
            m_mapValues[getIndex(x,y)]=heightValues[getIndex(x,y)]-
                     (m_mapValues[getIndex(x,y)])/div;
         }
         else
         {
            m_mapValues[getIndex(x,y)] = -0.0f;
         }
      }
   }

   std::cout <<"hello world\n";
   // left up corner
   if(heightValues[0]>0.0001f)
   {
       m_mapValues[0]= heightValues[getIndex(1,0)] +
                       heightValues[getIndex(0,1)] +
                       heightValues[getIndex(1,1)];
       float div = (heightValues[getIndex(1,0)]>0.0001f) +
                   (heightValues[getIndex(0,1)]>0.0001f) +
                   (heightValues[getIndex(1,1)]>0.0001f);

       m_mapValues[0]=fabs(heightValues[0]-m_mapValues[0]/div);
   }
   else
   {
      m_mapValues[0] = -0.0f;
   }
   // up right corner
   if(heightValues[getIndex(m_noOfPixelsX-1,0)]>0.0001f)
   {
       m_mapValues[0]= heightValues[getIndex(m_noOfPixelsX-2,0)] +
                       heightValues[getIndex(m_noOfPixelsX-2,1)] +
                       heightValues[getIndex(m_noOfPixelsX-1,1)];
       float div = (heightValues[getIndex(m_noOfPixelsX-2,0)]>0.0001f) +
                   (heightValues[getIndex(m_noOfPixelsX-2,1)]>0.0001f) +
                   (heightValues[getIndex(m_noOfPixelsX-1,1)]>0.0001f);
       m_mapValues[0]=fabs(heightValues[0]-m_mapValues[0]/div);
   }
   // down left corner
   if(heightValues[getIndex(0,m_noOfPixelsY-1)]>0.0001f)
   {
       m_mapValues[0]= heightValues[getIndex(0,m_noOfPixelsY-2)] +
                       heightValues[getIndex(1,m_noOfPixelsY-2)] +
                       heightValues[getIndex(1,m_noOfPixelsY-1)];
       float div = (heightValues[getIndex(0,m_noOfPixelsY-2)]>0.0001f) +
                   (heightValues[getIndex(1,m_noOfPixelsY-2)]>0.0001f) +
                   (heightValues[getIndex(1,m_noOfPixelsY-1)]>0.0001f);
       m_mapValues[0]=fabs(heightValues[0]-m_mapValues[0]/div);
   }
   // down right corner /// TO BE CORRECTED
   if(heightValues[getIndex(0,m_noOfPixelsY-1)]>0.0001f)
   {
       m_mapValues[0]= heightValues[getIndex(0,m_noOfPixelsY-2)] +
                       heightValues[getIndex(1,m_noOfPixelsY-2)] +
                       heightValues[getIndex(1,m_noOfPixelsY-1)];
       float div = (heightValues[getIndex(0,m_noOfPixelsY-2)]>0.0001f) +
                   (heightValues[getIndex(1,m_noOfPixelsY-2)]>0.0001f) +
                   (heightValues[getIndex(1,m_noOfPixelsY-1)]>0.0001f);
       m_mapValues[0]=fabs(heightValues[0]-m_mapValues[0]/div);
   }
   /// TO DO: WRITE ABOUT THE REST OF THE EDGES




   std::cout << "finished generating Average Height Difference map\n";

}

//-----------------------------------------------------------------------------
AverageHeightDifference::~AverageHeightDifference()
{

}
