#include "QuadTreeNode.h"

//-----------------------------------------------------------------------------
QuadTreeNode::QuadTreeNode(
        Pulse **i_pulses,
        unsigned int i_noOfPulses,
        // Limits are sorted as [maxNorthY, minNorthY, maxEastX, minEastX]
        const std::vector<double> i_limits,
        unsigned int i_stopLimit
        ):
    m_pulses(i_pulses),
    m_noOfPulses(i_noOfPulses)
{
   // it is important for the y axis to always be sorted
   if(i_noOfPulses>i_stopLimit)
   {
      // children are always split in the following order, where the horizontal
      // axis is the x-ax
      //   ---------
      //   | 1 | 2 |    firt group (1,2)
      //   ---------
      //   | 3 | 4 |    second group (3,4)
      //   ---------

      // yThres is the threshold that separates separates (1,2) from (3,4)
      double yThres((i_limits[0]+i_limits[1])/2.0);
      // xThres is the threshold that separates (1,3) from (2,4)
      double xThres((i_limits[2])+i_limits[3]/2.0);
      // the number of points that belong to the group (1,2)
      unsigned int lengthOf1stGroup;
      // the number of points that belong to the group (3,4)
      unsigned int lengthOf2stGroup;

      unsigned int min = 0;
      unsigned int max = m_noOfPulses;

      while(min!=max)
      {
         lengthOf1stGroup = ceil((min+max)/2);

         if(m_pulses[lengthOf1stGroup-1]->getOrigin().m_y>yThres)
         {
             // thres inside the 2nd half
             max = lengthOf1stGroup;
         }
         else
         {
            // thres inside the 1st half
            min = lengthOf1stGroup;
         }
      }

      lengthOf1stGroup = min;
      lengthOf2stGroup = m_noOfPulses-lengthOf1stGroup;





      // the array that all the elements of the group (1,2) are saved
      std::vector<Pulse *> pulsesOf1stGroup;
      // the array that all the elements of the group (3,4) are saved
      std::vector<Pulse *> pulsesOf2ndGroup;


      // then split the node into another four node
      // if a node is empty then no need to allocate memory for that node.
      m_children.resize(4,0);



      // at first separate the first part



      // then separate the second part

   }
   else
   {
      // the node is a leaf
   }
}

//-----------------------------------------------------------------------------
bool QuadTreeNode::isleaf()const
{
    // if the node is a leaf then it has no children
    return (m_children.size()==0);
}

//-----------------------------------------------------------------------------
QuadTreeNode::~QuadTreeNode()
{
   for(unsigned int i=0; m_children.size(); ++i)
   {
      if(m_children[i]!=0)
      {
          delete m_children[i];
      }
   }
}
