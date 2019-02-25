#include "Int_Plot.h"
#include <cstddef>
#include <iostream>
#include <math.h>

//-----------------------------------------------------------------------------
Int_Plot::Int_Plot(
        double i_radius,
        double i_northing,
        double i_easting
        ):
    m_radius(i_radius),
    m_northingY(i_northing),
    m_eastingX(i_easting)
{
    if (i_radius<0.00001)
    {
       std::cerr << "ERROR: The radius of fieldplot haven't been defined\n";
    }
}

//-----------------------------------------------------------------------------
bool Int_Plot::isThisPlot(
        double i_y,
        double i_x,
        double i_radius
        ) const
{
   return
     (i_y<m_northingY  +0.00001 && i_y>m_northingY  -0.000001 &&
      i_x<m_eastingX   +0.00001 && i_x>m_eastingX   -0.000001 &&
      i_radius<m_radius+0.00001 && i_radius>m_radius-0.000001);
}

//-----------------------------------------------------------------------------
bool Int_Plot::isCentreOfPlotInsideVol(
        double i_minX,
        double i_minY,
        double i_lenX,
        double i_lenY
        ) const
{
   return ( m_eastingX >i_minX && (m_eastingX -i_minX)<i_lenX &&
            m_northingY>i_minY && (m_northingY-i_minY)<i_lenY);
}



//-----------------------------------------------------------------------------
bool Int_Plot::isPointInsidePlot(
        const double i_x,
        const double i_y,
        const double i_offset
        )const
{
   double radiusMinusOffset = m_radius-i_offset;
   double disFromCentre = sqrt((i_x-m_eastingX )*(i_x-m_eastingX )+
                              (i_y-m_northingY)*(i_y-m_northingY));
   return disFromCentre<radiusMinusOffset;
}

//-----------------------------------------------------------------------------
bool Int_Plot::isColumnInAOI(
        double i_x,
        double i_y,
        double i_vl,
        const std::string &i_fclass,
        double &i_roffset
        )
{
    for(unsigned int i=0; i<m_trees.size(); ++i)
    {
       if(m_trees[i]->fclass==i_fclass)
       {
          if(i_roffset<0.0)
          {
             // check if this column if yes return true else ignore
             if(i_x-i_vl/2 <  m_trees[i]->xGPS &&
                i_x+i_vl/2 >= m_trees[i]->xGPS &&
                i_y-i_vl/2 <  m_trees[i]->yGPS &&
                i_y+i_vl/2 >= m_trees[i]->yGPS)
             {
                 return true;
             }
             else
             {
                // the tree of interest is not that specific tree
             }
          }
          else
          {
             // check whether it is within radius
             // if yes return false else ignore
             double disFromTreeCentre (sqrt((i_x-m_trees[i]->xGPS)*
                                            (i_x-m_trees[i]->xGPS)+
                                            (i_y-m_trees[i]->yGPS)*
                                            (i_y-m_trees[i]->yGPS)));
             if(disFromTreeCentre<i_roffset)
             {
                return false;
             }
          }
       }
    }

    if(i_roffset<0.0)
    {
       return false;
    }
    else
    {
       return true;
    }
}

//-----------------------------------------------------------------------------
void Int_Plot::addTree(CSVtreeInfo *i_tree)
{
   m_trees.push_back(i_tree);
}
//-----------------------------------------------------------------------------
void Int_Plot::print()const
{
   std::cout<< "NorthingY-EastingX: "<<m_northingY<<" "<<m_eastingX<<"\n";
   std::cout<< "Radius: " << m_radius << "\n";
   std::cout<< "No of Trees: " << m_trees.size() << "\n";
   for(unsigned int i=0; i<m_trees.size();++i)
   {
      if(m_trees[i]!=NULL)
      {
         std::cout << "----------------------------------------\n";
         std::cout<< "  ***  Tree i " << i << "  ***\n";

         std::cout<< "x GPS: " << m_trees[i]->xGPS   << "\n";
         std::cout<< "y GPS: " << m_trees[i]->yGPS   << "\n";
         std::cout<< "class: " << m_trees[i]->fclass << "\n";
         std::cout<< "it belongs to class of intersts: "
                  << m_trees[i]->isfclass << "\n";
      }
      else
      {
         //memory for that tree was not allocated correctly
      }
   }

}

//-----------------------------------------------------------------------------
Int_Plot::~Int_Plot()
{
   for(unsigned int i=0; i<m_trees.size(); ++i)
   {
      if(m_trees[i]!=NULL)
      {
         delete m_trees[i];
      }
   }
}
