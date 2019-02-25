#ifndef NF_FIELDPLOT_H
#define NF_FIELDPLOT_H

#include <string>
#include <vector>

class NF_FieldPlot
{
   friend class NF_FieldPlotsManager;
   friend class NF_FieldPlotManagerByType;

public:
   struct treeInfo
   {
      float xGPS;
      float yGPS;
      std::string type;
      float Age;
      float Height;
   };

   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @parm[in] i_file the file that contains the information about the fild
   /// plot
   // File format:
   // -------------
   // PlotLocation: <maxNorthY> <minNorthY> <maxEastX> <minEastX>
   // NoOfTrees: <noOfTrees>
   // X      Y      Species Age    Height
   // <xGPS> <yGPS> <type>  <age> <height>
   // ----
   // <xGPS> <yGPS> <type>  <age> <height>
   //--------------------------------------------------------------------------
   NF_FieldPlot(const std::string &i_file);
   //--------------------------------------------------------------------------
   /// @brief constructor that creates an empty field plot - used by
   /// FieldPlotManagerByType to add species of the same tree type only
   /// @note this method do not sets the limits because multiple trees from
   /// different fieldplots can be added at once, this can cause problems
   /// during evaluation
   //--------------------------------------------------------------------------
   NF_FieldPlot();
   //--------------------------------------------------------------------------
   /// @brief method that adds a tree to the field plot
   /// @param[in] i_tree tree to be added
   //--------------------------------------------------------------------------
   void addTreeToPlot(
           float i_xGPS,
           float i_yGPS,
           std::string i_type,
           float i_age,
           float i_height
           );
   //--------------------------------------------------------------------------
   /// @brief method the returns the limits of the area of the fieldplot
   //--------------------------------------------------------------------------
   const std::vector<double> &getLimits()const;
   //--------------------------------------------------------------------------
   /// @brief method that returns the number of the trees in the area
   //--------------------------------------------------------------------------
   unsigned int getNoOfTrees()const;
   //--------------------------------------------------------------------------
   /// @brief method that exports the tree positions into .json file for QGIS
   /// @param[in] i_filename the name of the file to be exported
   //--------------------------------------------------------------------------
   void exportAsJson() const;
   //--------------------------------------------------------------------------
   /// @brief method that returns the ith tree
   /// @param[in] i_i the index of the tree to be return
   /// @returns the information of the trees requested
   //--------------------------------------------------------------------------
   const treeInfo *getTreeInfo(unsigned int i_i);
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~NF_FieldPlot();

private:
   //--------------------------------------------------------------------------
   /// @brief the trees
   //--------------------------------------------------------------------------
   std::vector<treeInfo *> m_trees;
   //--------------------------------------------------------------------------
   /// @brief limits {maxNorthY, minNorthY, maxEastX, minEastX}
   //--------------------------------------------------------------------------
   std::vector<double> m_limits;
   //--------------------------------------------------------------------------
   /// @brief field plot filename
   //--------------------------------------------------------------------------
   std::string m_name;



};

#endif // NF_FIELDPLOT_H
