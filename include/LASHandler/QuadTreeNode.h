#ifndef QUADTREENODE_H
#define QUADTREENODE_H

//-----------------------------------------------------------------------------
/// @file QuadTreeNode.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 02/10/13
/// @class QuadTreeNode
/// @brief this class is a node of the QuadTree
/// This is a data structure that makes searching easier
/// @note the Pulses are not saved into the QuadTree, memory is managed by
/// the Pulse Manager
//-----------------------------------------------------------------------------

#include "Pulse.h"

class QuadTreeNode
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_pulses the pulses that belonged to this node
   /// @param[in] i_noOfPulses the number of pulses that belonged to this node
   /// @param[in] i_stopLimit is a number that if a Node has less pulses than
   /// this number then splitting stops. That number should be greater or equal
   /// to 1. It is not adviced though to give a very small number
   /// @note please note that i_pulses must be sorted with respect to the y
   /// position of the origin point
   //--------------------------------------------------------------------------
   QuadTreeNode(
           Pulse **i_pulses,
           unsigned int i_noOfPulses,
           const std::vector<double> i_limits,
           unsigned int i_stopLimit
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns true if the node is leaf and false otherwise
   //--------------------------------------------------------------------------
   bool isleaf()const;
   //--------------------------------------------------------------------------
   /// @brief default destructor
   /// the constructor does not delete the pulses, because the pulses are
   /// handled by the PulseManager
   //--------------------------------------------------------------------------
   ~QuadTreeNode();

private:
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   /// @brief method that spilt the data into two with respect to the x axis
   /// @param[in] i_
   //--------------------------------------------------------------------------
   void split(Pulse **i_pulses, unsigned int length);
   //--------------------------------------------------------------------------
   /// @brief the limits that the area is lie in.
   /// [maxNorthY, minNorthY, maxEastX, minEastX]
   //--------------------------------------------------------------------------
   std::vector<double> limits;
   //--------------------------------------------------------------------------
   /// @brief the childern of the node.
   /// children are always split in the following order, where the horizontal
   /// axis is the x-ax
   ///   ---------
   ///   | 1 | 2 |
   ///   ---------
   ///   | 3 | 4 |
   ///   ---------
   //--------------------------------------------------------------------------
   std::vector<QuadTreeNode *> m_children;
   //--------------------------------------------------------------------------
   /// @brief a pointer that points to the first Pulse of the QuadTree
   //--------------------------------------------------------------------------
   Pulse **m_pulses;
   //--------------------------------------------------------------------------
   /// @brief the number of pulses that this node has
   //--------------------------------------------------------------------------
   unsigned int m_noOfPulses;
};

#endif // QUADTREENODE_H
