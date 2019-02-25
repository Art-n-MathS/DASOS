#ifndef CreateTreeTemplates_H
#define CreateTreeTemplates_H

//-----------------------------------------------------------------------------
/// @file GLData.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 16/7/15
/// @class CreateTreeTemplates
/// @brief This class takes as input an exported volumetric object and an image
/// that has red dots at tree crowns. The x y dimensions of the volume and the
/// imported images should be the same
//-----------------------------------------------------------------------------

#include "Volume.h"

class CreateTreeTemplates
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// creates an empty class, so that the templates will be added later one
    /// by one.
    /// @param[in] i_volume: the name of the
    //-------------------------------------------------------------------------
    CreateTreeTemplates(
            const std::string &i_volume
            );
    //-------------------------------------------------------------------------
    /// @brief contstructor that creates an empty templates manager
    /// @param[in] i_obj: a volumetric object
    //-------------------------------------------------------------------------
    CreateTreeTemplates(Volume *i_obj);
    //-------------------------------------------------------------------------
    /// @brief constructor
    /// @param[in] i_volume : the name of the file that contains an exported
    /// volume
    /// @param[in] i_image : the image that shows with red the position of
    /// the tree crowns
    //-------------------------------------------------------------------------
    CreateTreeTemplates(
            const std::string &i_volume,
            const std::string &i_image
           );
    //-------------------------------------------------------------------------
    /// @brief method that adds positive or negative samples by importing an
    /// images, which shows the positions of the potential trees in red colour
    /// @param[in] i_image : the image that shows the red positions of the
    /// tree crowns
    /// @param[in] i_pos : true for positive trees and false for negative
    //-------------------------------------------------------------------------
    void addSamples(
            const std::string &i_image,
            bool i_pos = true
            );
//    //-------------------------------------------------------------------------
//    /// @brief method that adds samples from Int_PlotsManager
//    /// @param[in] i_plots the plots
//    /// @param[in] i_alive true for adding alive trees to the samples and
//    /// false if only dead trees are requested
//    //-------------------------------------------------------------------------
//    void addSamples(const Int_PlotsManager *i_plots, bool i_alive);
//    //-------------------------------------------------------------------------
//    /// @brief method that adds samples from Int_PlotsManager
//    /// @param[in] i_plots the plots
//    /// @param[in] i_offset how much data inside the plot will be discarded due
//    /// to be very close to the edge of the fieldplot
//    //-------------------------------------------------------------------------
//    void addAllPixelsAsSamples(
//            const Int_PlotsManager *i_plots,
//            const float i_offset
//            );
    //-------------------------------------------------------------------------
    /// @brief method that adds all samples. Positive are indicated by red and
    /// the rest are added as negative samples
    /// @note this functions finds the limits where the red dots lies and uses
    /// them as the boundaries of the field plot area (assuming that there are
    /// enough voxels in each side to create the templates)
    /// it also creates a header file with name i_image+".header" indicating
    /// the start and ending testing area of the image
    //-------------------------------------------------------------------------
    void addAllSamples(const std::string &i_image);
    //-------------------------------------------------------------------------
    /// @brief method that adds a template to the list
    /// @param[in] i_x the x-coordinates of the point
    /// @param[in] i_y the y-coordinates of the point
    //-------------------------------------------------------------------------
    void addTemplate(const float i_x, const float i_y, bool i_pos = true);
    //-------------------------------------------------------------------------
    /// @brief create templates
    /// @param[in] i_dimensionX how many voxels the template has in the x-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionY how many voxels the template has in the y-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionZ how many voxels the template has in the z-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_template the name of the file that all the templates are
    /// exported
    //-------------------------------------------------------------------------
    void exportTemplates(
            unsigned short i_dimensionX,
            unsigned short i_dimensionY,
            unsigned short i_dimensionZ,
            const std::string &i_template
            );
    //-------------------------------------------------------------------------
    /// @brief create training and testing files using the libSVM format
    /// @param[in] i_dimensionX how many voxels the template has in the x-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionY how many voxels the template has in the y-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionZ how many voxels the template has in the z-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_template the name of the file that all the templates are
    /// exported
    //-------------------------------------------------------------------------
    void exportTemplatesForLibSVM(
            unsigned short i_dimensionX,
            unsigned short i_dimensionY,
            unsigned short i_dimensionZ,
            const std::string &i_template
            );
    //-------------------------------------------------------------------------
    /// @brief create training and testing files using the libSVM format
    /// @param[in] i_dimensionX how many voxels the template has in the x-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionY how many voxels the template has in the y-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_dimensionZ how many voxels the template has in the z-axis
    /// It nees to be an odd number and the tree crown will me the middle pixel
    /// @param[in] i_template the name of the file that all the templates are
    /// exported
    //-------------------------------------------------------------------------
    void exportTemplatesCSV(
            unsigned short i_dimensionX,
            unsigned short i_dimensionY,
            unsigned short i_dimensionZ,
            const std::string &i_template,
            const bool i_AOI
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~CreateTreeTemplates();
    //-------------------------------------------------------------------------

private:
    //-------------------------------------------------------------------------
    /// @brief the volumetric object
    //-------------------------------------------------------------------------
    Volume *m_obj;
    //-------------------------------------------------------------------------
    /// @brief tree crowns, a list of tree crowns as x,y positions
    //-------------------------------------------------------------------------
    std::vector< std::pair<unsigned int,unsigned int> > m_trees;

    std::vector< std::pair<double,double> > m_GPStrees;

    std::string m_currentPlot;
    //-------------------------------------------------------------------------
    /// @brief is dead, is tree, is positive (what I am looking for)
    //-------------------------------------------------------------------------
    std::vector<bool> m_isDead;
    //-------------------------------------------------------------------------
    /// @brief non tree crowns, a list of faulse tree crowns as x,y positions
    //-------------------------------------------------------------------------
    std::vector< std::pair<unsigned int,unsigned int> > m_nonTrees;
    //-------------------------------------------------------------------------
    /// @brief indicated whether to free the object or not, depending whether
    /// it was created in this class or imported
    //-------------------------------------------------------------------------
    bool m_freeObject;



};

#endif // CreateTreeTemplates_H
