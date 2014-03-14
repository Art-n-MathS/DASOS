#include "IntegralVolumes.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>

//-----------------------------------------------------------------------------
IntegralVolumes::IntegralVolumes(
        unsigned int i_x,
        unsigned int i_y,
        unsigned int i_z,
        const std::vector<double> &i_values
        ):
    m_values(i_values),
    m_noOfValuesX(i_x),
    m_noOfValuesY(i_y),
    m_noOfValuesZ(i_z)
{
   if(m_values.size()!= i_x*i_y*i_z)
   {
      std::cout << "ERROR: the len of the i_values is not equal to i_x*i_y*i_z\n";
      exit(EXIT_FAILURE);
   }
   // create summed table
   // first compute the first horizontal line
   for(unsigned int x=1; x < m_noOfValuesX;++x)
   {
      // no need to call getIndex because the y position is always 0
      m_values[x] += m_values[x-1];
   }
   // then compute the first vertical line
   for(unsigned int y=1; y<m_noOfValuesY; ++y)
   {
      m_values[getIndex(0,y,0)] += m_values[getIndex(0,y-1,0)];
   }
   // then the first depth line
   for(unsigned int z=1; z<m_noOfValuesZ; ++z)
   {
      m_values[getIndex(0,0,z)]+=m_values[getIndex(0,0,z-1)];
   }
   std::cout << "\n---------------------------------------\n";

   // then fill the 3 side planes for x=0, z=0 and y=0
   for(unsigned int x=1; x < m_noOfValuesX; ++x)
   {
      for (unsigned int y=1; y < m_noOfValuesY; ++y)
      {
          m_values[getIndex(x,y,0)] += m_values[getIndex(x-1,y,0)]
                 + m_values[getIndex(x,y-1,0)] - m_values[getIndex(x-1,y-1,0)];
      }
   }

   for(unsigned int z=1; z < m_noOfValuesZ; ++z)
   {
      for (unsigned int y=1; y < m_noOfValuesY; ++y)
      {
          m_values[getIndex(0,y,z)] += m_values[getIndex(0,y,z-1)]
                 + m_values[getIndex(0,y-1,z)] - m_values[getIndex(0,y-1,z-1)];
      }
   }
   for(unsigned int x=1; x < m_noOfValuesX; ++x)
   {
      for (unsigned int z=1; z < m_noOfValuesZ; ++z)
      {
          m_values[getIndex(x,0,z)] += m_values[getIndex(x-1,0,z)]
                 + m_values[getIndex(x,0,z-1)] - m_values[getIndex(x-1,0,z-1)];
      }
   }

   for(unsigned int x=1; x<m_noOfValuesX; ++x)
   {
      for(unsigned int y=1; y<m_noOfValuesY; ++y)
      {
         for(unsigned int z=1; z<m_noOfValuesZ;++z)
         {
             m_values[getIndex(x,y,z)]+=m_values[getIndex(x-1,y,z)]
                     + m_values[getIndex(x,y-1,z)]+m_values[getIndex(x,y,z-1)]
                     - m_values[getIndex(x,y-1,z-1)]
                     - m_values[getIndex(x-1,y,z-1)]
                     - m_values[getIndex(x-1,y-1,z)]
                     + m_values[getIndex(x-1,y-1,z-1)];
         }
      }
   }
}

//-----------------------------------------------------------------------------
void IntegralVolumes::print()const
{
   std::cout << "Table size = " << m_noOfValuesX << " x "
             << m_noOfValuesY << " x " << m_noOfValuesZ << "\n";
   for(unsigned int i=0; i < m_values.size(); ++i)
   {
      if(i%m_noOfValuesX==0)
      {
         std::cout << "\n";
      }
      if(i%(m_noOfValuesX*m_noOfValuesY)==0)
      {
         std::cout << "\n";
      }
      std::cout << " " << std::setw(6) << std::setprecision(4) <<  m_values[i];
   }
   std::cout << "\n\n";
}

//-----------------------------------------------------------------------------
double IntegralVolumes::getSumOfArea(
        unsigned short x,
        unsigned short y,
        unsigned short z,
        unsigned short lenX,
        unsigned short lenY,
        unsigned short lenZ
        ) const
{


   // if the box of our interest is in the lower corner
   if(x==0 && y==0 && z==0)
   {
      return m_values[getIndex(lenX,lenY,lenZ)];
   }

   //
   if(x==0 && y==0)
   {
      return m_values[getIndex(lenX,lenY,z+lenZ)]
              -m_values[getIndex(lenX,lenY,z-1)];
   }
   if(y==0 && z==0)
   {
      return m_values[getIndex(x+lenX,lenY,lenZ)]
              -m_values[getIndex(x-1,lenY,lenZ)];
   }
   if(x==0 && z==0)
   {
      return m_values[getIndex(lenX,y+lenY,lenZ)]
              -m_values[getIndex(lenX,y-1,lenZ)];
   }



   // if the box lies on the lower x plane
   if(x==0)
   {
      return  m_values[getIndex(lenX,y+lenY,z+lenZ)]
             -m_values[getIndex(lenX,y-1   ,z+lenZ)]
             -m_values[getIndex(lenX,y+lenY,z-1   )]
             +m_values[getIndex(lenX,y-1   ,z-1   )];
   }
   if(y==0)
   {
      return  m_values[getIndex(x+lenX,lenY,z+lenZ)]
             -m_values[getIndex(x-1   ,lenY,z+lenZ)]
             -m_values[getIndex(x+lenX,lenY,z-1   )]
             +m_values[getIndex(x-1   ,lenY,z-1   )];
   }
   if(z==0)
   {
      return  m_values[getIndex(x+lenX,y+lenY,lenZ)]
             -m_values[getIndex(x+lenX,y-1   ,lenZ)]
             -m_values[getIndex(x-1   ,y+lenY,lenZ)]
             +m_values[getIndex(x-1   ,y-1   ,lenZ)];
   }

   // in x,y,z are not zero
   return m_values[getIndex(x+lenX,y+lenY,z+lenZ)]
        - m_values[getIndex(x-1   ,y+lenY,z+lenZ)]
        - m_values[getIndex(x+lenX,y-1   ,z+lenZ)]
        - m_values[getIndex(x+lenX,y+lenY,z-1   )]
        + m_values[getIndex(x-1   ,y-1   ,z+lenZ)]
        + m_values[getIndex(x-1   ,y+lenY,z-1   )]
        + m_values[getIndex(x+lenX,y-1   ,z-1   )]
        - m_values[getIndex(x-1   ,y-1   ,z-1   )];
}

//-----------------------------------------------------------------------------
unsigned short int IntegralVolumes::getIndex(
        unsigned short i_x,
        unsigned short i_y,
        unsigned short i_z
        ) const
{
    return i_x+i_y*m_noOfValuesX+i_z*m_noOfValuesX*m_noOfValuesY;
}


//-----------------------------------------------------------------------------
IntegralVolumes::~IntegralVolumes()
{}
