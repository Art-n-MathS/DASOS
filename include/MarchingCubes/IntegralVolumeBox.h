#ifndef INTEGRALVOLUMEBOX_H
#define INTEGRALVOLUMEBOX_H
#include <vector>

class IntegralVolumeBox
{
   friend class MCwithIntegralImages;
public:
   //-----------------------------------------------------------------------------
   /// @brief default constructor
   //-----------------------------------------------------------------------------
   IntegralVolumeBox(
           const unsigned int short i_lenX,
           const unsigned int short i_lenY,
           const unsigned int short i_lenZ
           );
   //-----------------------------------------------------------------------------
   /// @brief copy constructor
   //-----------------------------------------------------------------------------
   IntegralVolumeBox(const IntegralVolumeBox & i_box);

   //-----------------------------------------------------------------------------
   /// @brief default destructor
   //-----------------------------------------------------------------------------
   ~IntegralVolumeBox();

private:
   //-----------------------------------------------------------------------------
   /// @brief the minimum limits of the current box
   //-----------------------------------------------------------------------------
   std::vector<unsigned short int> m_mins;
   //-----------------------------------------------------------------------------
   /// @brief the number of boxes in the x y z axes
   //-----------------------------------------------------------------------------
   std::vector<unsigned short int> m_lens;
   //-----------------------------------------------------------------------------
   /// @brief the divisible sides of the box
   /// 7 for all sides, 5 for zx, 6 for zy, 3 for yx, 1 for x, 2 for y and 4 for z
   //-----------------------------------------------------------------------------
   unsigned short int m_divisibles;
   //-----------------------------------------------------------------------------
   /// @brief the next side to be divided
   //-----------------------------------------------------------------------------
   unsigned short int m_nextSide;


};

#endif // INTEGRALVOLUMEBOX_H
