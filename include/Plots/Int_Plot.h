#ifndef INT_PLOT_H
#define INT_PLOT_H

//-----------------------------------------------------------------------------
/// @file Int_Plot.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 28/01/16
/// @class Int_Plot
/// @brief a single field plot
//-----------------------------------------------------------------------------


#include <vector>
#include <string>

class Int_Plot
{
    friend class CreateTreeTemplates;
public:
   struct CSVtreeInfo
   {
      double xGPS;
      double yGPS;
      bool isfclass;
      std::string fclass;

      CSVtreeInfo():
          xGPS(0.0f),
          yGPS(0.0f),
          isfclass(false),
          fclass(""){}
   };

   //--------------------------------------------------------------------------


   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   Int_Plot(double i_radius, double i_northing, double i_easting);
   //--------------------------------------------------------------------------
   /// @brief method that adds a tree to the plot
   //--------------------------------------------------------------------------
   void addTree(CSVtreeInfo * i_tree);
   //--------------------------------------------------------------------------
   /// @brief print information about this plot
   //--------------------------------------------------------------------------
   void print()const;
   //--------------------------------------------------------------------------
   /// @brief method that returns whether column in class of interest
   /// (i.e. close to a dead tree or whether all columns are included)
   //--------------------------------------------------------------------------
   bool isColumnInAOI(
           double i_x,
           double i_y,
           double i_vl,
           const std::string &i_fclass,
           double &i_roffset
           );
   //--------------------------------------------------------------------------
   /// @brief method that checks if the current plot is the same as this
   /// plot
   /// @param[in] i_y the northing of the centre
   /// @param[in] i_x the easting of the centre
   /// @param[in] i_radius the radius of the plot
   /// @return true if the current plot is the same with this one and false
   /// otherwise
   //--------------------------------------------------------------------------
   bool isThisPlot(double i_y, double i_x, double i_radius)const;
   //--------------------------------------------------------------------------
   /// @brief checks whether this plot is inside an area
   /// @param[in] i_minX
   /// @param[in] i_minY
   /// @param[in] i_lenX
   /// @param[in] i_lenY
   /// @return true if plot inside the given boundaries and false otherwise
   //--------------------------------------------------------------------------
   bool isCentreOfPlotInsideVol(
           const double i_minX,
           const double i_minY,
           const double i_lenX,
           const double i_lenY
           )const;
   //--------------------------------------------------------------------------
   /// @brief method that finds whether a given point lies inside or outside
   /// the boundaries of the plot
   /// @param[in] i_x
   /// @param[in] i_y
   /// @param[in] i_offset a line that defines the width of the boundaries.
   /// this offset is subtracted from the radius of the plot in order to avoid
   /// including trees that may or may not exist
   /// @returns true of point (i_x,i_y) lies inside this plot
   //--------------------------------------------------------------------------
   bool isPointInsidePlot(
           const double i_x,
           const double i_y,
           const double i_offset
           )const;
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~Int_Plot();

private:
   //-------------------------------------------------------------------------
   /// @brief diameter of the plot
   //-------------------------------------------------------------------------
   double m_radius;
   //-------------------------------------------------------------------------
   /// @brief centre of the plot in y-axis / northing
   //-------------------------------------------------------------------------
   double m_northingY;
   //-------------------------------------------------------------------------
   /// @brief centre of the plot in x-axis / easting
   //-------------------------------------------------------------------------
   double m_eastingX;
   //-------------------------------------------------------------------------
   /// @brief number of trees inside the plot
   //-------------------------------------------------------------------------
   std::vector<CSVtreeInfo *> m_trees;
};

#endif // INT_PLOT_H
