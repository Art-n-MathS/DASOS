#ifndef TREECROWNSTEMPLATES_H
#define TREECROWNSTEMPLATES_H

//-------------------------------------------------------------------------
/// @file TreeCrownsTemplates.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 17/07/15
/// @class TreeCrownsTemplates
/// @brief It contains all the templates for a given set ie. tree crown
//-------------------------------------------------------------------------


#include <vector>
#include <string>

class TreeCrownsTemplates
{
public:
   //-----------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_file: the name/directory of the file that contains the
   /// information about the templates.
   //-----------------------------------------------------------------------------
   TreeCrownsTemplates(const std::string i_file);
   //-----------------------------------------------------------------------------
   /// @brief method that returns the intensity of a voxel given
   /// @param[in] i_index the index of the template
   /// @param[in] i_x the x position of the voxel
   /// @param[in] i_y the y position of the voxel
   /// @param[in] i_z the z position of the voxel
   //-----------------------------------------------------------------------------
   float getValue(
           unsigned short int i_index,
           unsigned short int i_x,
           unsigned short int i_y,
           unsigned short int i_z
           );
   //-----------------------------------------------------------------------------
   /// @brief method that returns the number of templates
   /// @return the number of templates
   //-----------------------------------------------------------------------------
   unsigned short int getNoOfTemplates();
   //-----------------------------------------------------------------------------
   /// @brief method that returns the number of voxels in the x-axis
   /// @return the number of voxels in the x-axis per template
   //-----------------------------------------------------------------------------
   unsigned short int getNoOfVoxelsX();
   //-----------------------------------------------------------------------------
   /// @brief method that returns the number of voxels in the y-axis
   /// @return the number of voxels in the y-axis per template
   //-----------------------------------------------------------------------------
   unsigned short int getNoOfVoxelsY();
   //-----------------------------------------------------------------------------
   /// @brief method that returns the number of voxels in the z-axis
   /// @return the number of voxels in the z-axis per template
   //-----------------------------------------------------------------------------
   unsigned short int getNoOfVoxelsZ();
   //-----------------------------------------------------------------------------
   /// @brief default destructor
   //-----------------------------------------------------------------------------
   ~TreeCrownsTemplates();

private:
   //-----------------------------------------------------------------------------
   /// @brief method that returns the index of the voxel in a template
   /// @param[in] i_x the x position of the voxel
   /// @param[in] i_y the y position of the voxel
   /// @param[in] i_z the z position of the voxel
   //-----------------------------------------------------------------------------
   unsigned short int getIndex(
           unsigned short int i_x,
           unsigned short int i_y,
           unsigned short int i_z
           );

   //-----------------------------------------------------------------------------
   /// @brief the number of voxels in the x-axis
   //-----------------------------------------------------------------------------
   unsigned short int m_dX;
   //-----------------------------------------------------------------------------
   /// @brief the number of voxels in the x-axis
   //-----------------------------------------------------------------------------
   unsigned short int m_dY;
   //-----------------------------------------------------------------------------
   /// @brief the number of voxels in the x-axis
   //-----------------------------------------------------------------------------
   unsigned short int m_dZ;
   //-----------------------------------------------------------------------------
   /// @brief the templates
   //-----------------------------------------------------------------------------
   std::vector<float *> m_templates;

};

#endif // TREECROWNSTEMPLATES_H
