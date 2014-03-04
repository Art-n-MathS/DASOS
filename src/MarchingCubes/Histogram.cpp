#include "Histogram.h"
#include <iostream>
#include <math.h>
#include <QImage>

#include <fstream>

//-----------------------------------------------------------------------------
Histogram::Histogram(
        const std::vector<double> i_values,
        unsigned int i_numOfintervals
        ):m_numOfIntervals(i_numOfintervals)
{
   if(i_values.size()==0)
   {
      std::cout << "Histogram: i_values has length 0. Not acceptable!\n";
      return;
   }
   m_max = i_values[0];
   m_min = i_values[0];
   // find max and min value of the values
   for(unsigned int i=1; i<i_values.size(); ++i)
   {
      if(m_max < i_values[i])
      {
         m_max = i_values[i];
      }
      else if (m_min > i_values[i])
      {
         m_min = i_values[i];
      }
   }
   // compute the length of the intervals:
   m_step = (m_max-m_min+0.00000001)/i_numOfintervals;
   // resize the array where we save the number of values that belongs to each
   // interval and set the initial values to 0
   m_numOfPoints.resize(i_numOfintervals,0);

   for(unsigned int i=0; i<i_values.size(); ++i)
   {
      unsigned int index=getIntervalIndex(i_values[i]);
      if(index<i_numOfintervals)
      {
         m_numOfPoints[index]++;
      }
   }

   std::ofstream myfile;
   myfile.open("data.txt");
   for(unsigned int i=0; i< i_values.size(); ++i)
   {
       if(i_values[i]>30)
       {
           myfile << i_values[i] << " ";
       }
   }
   myfile.close();
}

//-----------------------------------------------------------------------------
void Histogram::drawHistogram(const std::string &i_filename) const
{
   unsigned int height = 0;
   for(unsigned int i=2; i < m_numOfPoints.size(); ++i)
   {
      if(height < m_numOfPoints[i])
      {
         height = m_numOfPoints[i];
      }
   }
   std::cout << height << "\n";
   QImage *hist = new QImage(m_numOfIntervals, height,QImage::Format_RGB16);
   hist->fill(qRgb(255,255,255));

   for(unsigned int i=1; i<m_numOfPoints.size(); ++i)
   {
       for(unsigned int y = height-m_numOfPoints[i];y<height;++y)
       {
           hist->setPixel(i,y,qRgb(0,0,0));
       }
   }

   hist->save(i_filename.c_str());
   delete hist;
}

//-----------------------------------------------------------------------------
unsigned int Histogram::getIntervalIndex(double i_value) const
{
   return floor((i_value-m_min)/m_step);
}


//-----------------------------------------------------------------------------
Histogram::Histogram(
        const Histogram &i_histogram
        ):
    m_numOfPoints(i_histogram.m_numOfPoints),
    m_min(i_histogram.m_min),
    m_max(i_histogram.m_max),
    m_numOfIntervals(i_histogram.m_numOfIntervals),
    m_step(i_histogram.m_step)
{
}


//-----------------------------------------------------------------------------
Histogram::~Histogram()
{}
