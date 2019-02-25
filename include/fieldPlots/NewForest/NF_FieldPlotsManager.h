#ifndef NF_FIELDPLOTSMANAGER_H
#define NF_FIELDPLOTSMANAGER_H

#include "NF_FieldPlot.h"
#include <vector>

class NF_FieldPlotsManager
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   NF_FieldPlotsManager();
   //--------------------------------------------------------------------------
   /// @brief method that reads the given field plots
   /// @param[in] i_filedplots names of field plots to be read
   //--------------------------------------------------------------------------
   virtual void readFieldPlots(const std::vector<std::string> &i_fieldplots);

   //--------------------------------------------------------------------------
   /// @brief method that creates tree templates
   /// @param[in] i_volumeFile is the file name of the volume which will be
   /// used for generating the templates
   /// @param[in] i_templateFileName the name of the files that the templates
   /// will be saved into
   /// @param[in] i_x the length of the templates in the x axis
   /// @param[in] i_y the length of the templates in the y axis
   /// @param[in] i_z the length of the templates in the z axis
   //--------------------------------------------------------------------------
   virtual void createTemplates(const std::string &i_volumeFile,
           const std::string &i_templateFileName,
           unsigned short i_x,
           unsigned short i_y,
           unsigned short i_z,
           bool svm = 0
           )const;
   //--------------------------------------------------------------------------
   /// @brief method that returns the limits of the given plots
   /// @note only valid for FieldPlotManager, FieldPlotManagerpy type do not
   /// save information about fieldplot limits
   //--------------------------------------------------------------------------
   std::vector<double> getLimits()const;
   //--------------------------------------------------------------------------
   /// @brief method that exports all the positions of the trees into .json
   /// files that can be read into QGIS
   //--------------------------------------------------------------------------
   void exportAsJson()const;
   //--------------------------------------------------------------------------
   /// @brief method that calculates and prints the statistics about the field
   /// plots
   //--------------------------------------------------------------------------
   void printStatistics()const;
   //--------------------------------------------------------------------------
   /// @brief method that reads all the field plots by hardcoding them
   /// all saved in folder 'FieldPlots' and have names 01.txt - 21.txt
   //--------------------------------------------------------------------------
   void readAllFieldPlots();
   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   virtual ~NF_FieldPlotsManager();

private:
   //--------------------------------------------------------------------------
   /// @brief method that adds fieldplots to the manager
   /// @param[in] i_file field plot to be added to the manager
   //--------------------------------------------------------------------------
   void addPlot(const std::string &i_file);


protected:
   //--------------------------------------------------------------------------
   /// @brief a vector containg all the plots
   //--------------------------------------------------------------------------
   std::vector<NF_FieldPlot *> m_plots;

};

#endif // NF_FIELDPLOTSMANAGER_H
