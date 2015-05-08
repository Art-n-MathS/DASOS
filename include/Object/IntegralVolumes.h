#ifndef INTEGRALVOLUMES_H
#define INTEGRALVOLUMES_H

#include <vector>

class IntegralVolumes
{
public:
   //--------------------------------------------------------------------------
   /// @brief contructor
   /// @param[in] i_x the number of values in the x-axis
   /// @param[in] i_y the number of values in the y-axis
   /// @param[in] i_z the number of values in the z-axis
   /// @param[in] i_tableValues
   //--------------------------------------------------------------------------
   IntegralVolumes(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z,
           const std::vector<double> i_values
           );
   //--------------------------------------------------------------------------
   /// @brief method that prints the values of the table
   //--------------------------------------------------------------------------
   void print()const;
   //--------------------------------------------------------------------------
   /// @brief method that returns the sum of the given area
   /// @param[in] i_posX the min X position of the area of our interest
   /// @param[in] i_posY the min Y position of the area of our interest
   /// @param[in] i_posZ the min Z position of the area of our interest
   /// @param[in] i_lenX the length of the area in the x-axis
   /// @param[in] i_lenY the length of the area in the y-axis
   /// @param[in] i_lenZ the length of the area in the z-axis
   //--------------------------------------------------------------------------
   double getSumOfArea(
           unsigned int i_posX,
           unsigned int i_posY,
           unsigned int i_posZ,
           unsigned int i_lenX,
           unsigned int i_lenY,
           unsigned int i_lenZ
           )const;
   //--------------------------------------------------------------------------
   /// @brief default destractor
   //--------------------------------------------------------------------------
   ~IntegralVolumes();

private:
   //--------------------------------------------------------------------------
   /// @brief method that returns the index of a value in m_values
   /// @param[in] i_x the x position of the value on the table
   /// @param[in] i_y the y position of the value on the table
   /// @param[in] i_z the z position of the value on the table
   //--------------------------------------------------------------------------
   unsigned int getIndex(
           unsigned int i_x,
           unsigned int i_y,
           unsigned int i_z
           ) const;

   //--------------------------------------------------------------------------
   /// @brief all the values of the sum table
   //--------------------------------------------------------------------------
   std::vector<double> m_values;
   //--------------------------------------------------------------------------
   /// @brief number of values in the x-axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfValuesX;
   //--------------------------------------------------------------------------
   /// @brief number of values in the y axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfValuesY;
   //--------------------------------------------------------------------------
   /// @brief number of values in the z axis
   //--------------------------------------------------------------------------
   unsigned int m_noOfValuesZ;
};

#endif // INTEGRALVOLUMES_H
