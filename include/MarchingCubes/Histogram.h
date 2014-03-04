#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <string>

class Histogram
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   Histogram(
           const std::vector<double> i_values,
           unsigned int i_numOfintervals
           );
   //--------------------------------------------------------------------------
   /// @brief default copy constructor
   /// @param[in] i_histogram histogram to be copied
   //--------------------------------------------------------------------------
   Histogram(const Histogram &i_histogram);
   //--------------------------------------------------------------------------
   /// @brief method that saves the image
   /// @param[in] i_filename the name of the image
   //--------------------------------------------------------------------------
   void drawHistogram(const std::string &i_filename)const;
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~Histogram();


private:
   //--------------------------------------------------------------------------
   /// @brief method that returns the index of the interval that the given
   /// value belongs to
   /// @param[in] i_value the value of our interest
   //--------------------------------------------------------------------------
   unsigned int getIntervalIndex(double i_value)const;
   //--------------------------------------------------------------------------
   /// @brief the number of values that lie inside each interval
   //--------------------------------------------------------------------------
   std::vector<unsigned int> m_numOfPoints;
   //--------------------------------------------------------------------------
   /// @brief the min value
   //--------------------------------------------------------------------------
   double m_min;
   //--------------------------------------------------------------------------
   /// @brief the max value
   //--------------------------------------------------------------------------
   double m_max;
   //--------------------------------------------------------------------------
   /// @brief number of intervals
   //--------------------------------------------------------------------------
   unsigned int m_numOfIntervals;
   //--------------------------------------------------------------------------
   /// @brief the length of each interval
   //--------------------------------------------------------------------------
   double m_step;

};

#endif // HISTOGRAM_H
