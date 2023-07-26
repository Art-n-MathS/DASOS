#ifndef INT_PLOTSMANAGER_H
#define INT_PLOTSMANAGER_H

//-----------------------------------------------------------------------------
/// @file Int_PlotsManager.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 28/01/16
/// @class Int_PlotsManager
/// @brief this class manages the field plots saved into an csv file
//-----------------------------------------------------------------------------


#include "Int_Plot.h"
#include <vector>
#include <string>
#include <map>
#include "Volume.h"
#include "MapsManager.h"

//-----------------------------------------------------------------------------
/// @file Int_PlotsManager.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 28/01/16
/// @class Int_Plot
/// @brief the manager of all the field plot
//-----------------------------------------------------------------------------

class Int_PlotsManager
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    //--------------------------------------------------------------------------
    Int_PlotsManager();
   //--------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_csvFile
   //--------------------------------------------------------------------------
   Int_PlotsManager(const std::string i_csvFile);
   //--------------------------------------------------------------------------
   /// @brief method that interpretates all the data, read the fieldplots and
   /// generates the templates according to the specified parameters
   //--------------------------------------------------------------------------
   void interprateData(
           const std::string &volumeType,
           double isolevel,
           double offset=0.0);

   //--------------------------------------------------------------------------
   /// @brief method that prints all the information of the csv file
   /// in groups of trees/plots
   //--------------------------------------------------------------------------
   void printPlots()const;
   //--------------------------------------------------------------------------
   /// @brief method that prints imported parameters
   //--------------------------------------------------------------------------
   void printParameters()const;
   //--------------------------------------------------------------------------
   /// @brief method that sets csvImportingFieldplots
   //--------------------------------------------------------------------------
   void set_csvImportingFieldplots(const char *i_c)
   {csvImportingFieldplots=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets csvExportingTemplates
   //--------------------------------------------------------------------------
   void set_csvExportingTemplates(const char *i_c){csvExportingTemplates=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets   volsDir
   //--------------------------------------------------------------------------
   void set_volsDir(const char *i_c){volsDir=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets a las file if volsDir not defined
   //--------------------------------------------------------------------------
   void set_las(Volume *i_vol){m_vol=i_vol;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fcolumn
   //--------------------------------------------------------------------------
   void set_fcolumn(const char *i_c) {fcolumn=i_c;
                    for (auto & c: fcolumn) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fclass
   //--------------------------------------------------------------------------
   void set_fclass(const char *i_c){fclass=i_c;
                    for (auto & c: fclass) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fpCenterX_label
   //--------------------------------------------------------------------------
   void set_fpCenterX_label(const char *i_c){fpCenterX_label=i_c;
                    for (auto & c: fpCenterX_label) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fpCentreY_label
   //--------------------------------------------------------------------------
   void set_fpCentreY_label(const char *i_c){fpCentreY_label=i_c;
                    for (auto & c: fpCentreY_label) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fpTreeX_label
   //--------------------------------------------------------------------------
   void set_fpTreeX_label(const char *i_c){fpTreeX_label=i_c;
                    for (auto & c: fpTreeX_label) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fpTreeY_label
   //--------------------------------------------------------------------------
   void set_fpTreeY_label(const char *i_c){fpTreeY_label=i_c;
                    for (auto & c: fpTreeY_label) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    fpRadius
   //--------------------------------------------------------------------------
   void set_fpRadius(const char *i_c){fpRadius=i_c;
                    for (auto & c: fpRadius) c = std::toupper(c);}
   //--------------------------------------------------------------------------
   /// @brief method that sets    m_currentPlot
   //--------------------------------------------------------------------------
   void set_m_currentPlot(unsigned int short i_c){m_currentPlot=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    mheight
   //--------------------------------------------------------------------------
   void set_mheight(int i_c){mheight=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    th
   //--------------------------------------------------------------------------
   void set_th(int i_c){th=i_c;tz=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    tr
   //--------------------------------------------------------------------------
   void set_tr(int i_c){tr=i_c;tx=i_c;ty=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    tx
   //--------------------------------------------------------------------------
   void set_tx(int i_c){tx=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    ty
   //--------------------------------------------------------------------------
   void set_ty(int i_c){ty=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    tz
   //--------------------------------------------------------------------------
   void set_tz(int i_c){tz=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    ttype
   //--------------------------------------------------------------------------
   void set_ttype(int i_c){ttype=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that sets    eparameters
   //--------------------------------------------------------------------------
   void set_eparameters(int i_c){eparameters=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that returns ttype
   //--------------------------------------------------------------------------
   int get_ttype()const{return ttype;}
   //--------------------------------------------------------------------------
   /// @brief method that sets roffset
   //--------------------------------------------------------------------------
   void set_roffset(double i_c){roffset=i_c;}
   //--------------------------------------------------------------------------
   /// @brief method that checks whether the th tr tx tx tz are set correctly
   //--------------------------------------------------------------------------
   bool checkthrxyz()const{return (th<=0||tr<=0||tx<=0||ty<=0||tz<=0);}

   //--------------------------------------------------------------------------
   /// @brief method that loops through all the plots and returns the one that
   /// lines inside the Area of Interest
   /// @param[in] i_minX the min easting of the AOI
   /// @param[in] i_minY the min northing of the AOI
   /// @param[in] i_lenX the x easting length of AOI
   /// @param[in] i_lenY the y northing length of AOI
   //--------------------------------------------------------------------------
   const Int_Plot *getPlotOfAOI(
           double i_minX,
           double i_minY,
           double i_lenX,
           double i_lenY
           )const;

   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~Int_PlotsManager();

private:
   //--------------------------------------------------------------------------
   /// @brief print labels of processed intensities
   /// @param[in] i_file the name of the out file where the values are saved
   //--------------------------------------------------------------------------
   void printLabelsIn(std::ofstream &i_file);
   //--------------------------------------------------------------------------
   /// @brief method that prints the processed values of the given template
   /// @param[in] i_file the name of the out file where the values are saved
   /// @param[in] i_values the of the current prior
   /// @param[in] i_vl the voxel lenght of the volume
   /// @param[in] i_height the max height of the prior
   //--------------------------------------------------------------------------
   void printProcessedValues(std::ofstream &i_file,
           double *i_values, unsigned int lenValues,
           double i_vl,
           double i_height
           , double i_isolevel);
   //--------------------------------------------------------------------------
   /// @brief method that takes as input a volume and exports the priors
   //--------------------------------------------------------------------------
   void givenVolExportPriors(
           Volume *Vol,
           double isolevel,
           const std::string &current
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns whether a column is inside the cylinder or
   /// not
   /// @param[in] i_xCol the x-coordinate of the column
   /// @param[in] i_yCol the y-coordinate of the column
   //--------------------------------------------------------------------------
   bool isInsideCylinder(double i_xCol,
           double i_yCol);
   //--------------------------------------------------------------------------
   /// @brief method that adds a tree to one of our plots
   /// creates a new plot if the tree belongs to a plot that haven't created
   /// yet
   /// @param[in] i_labels the first line of csv file that defines what info
   /// about the tree are included inside the file
   /// @param[in] i_treeInfo the line of the csv file that contains the
   /// information about the tree
   //--------------------------------------------------------------------------
   void addTree(
           const std::vector<std::string> &i_labels,
           const std::vector<std::string> &i_treeInfo
           );
   //--------------------------------------------------------------------------
   /// @brief check whether fieldplot is correct or not
   /// @return true for valid fieldplot manager and false for non valid
   //--------------------------------------------------------------------------
   bool checkValidity()const;
   //--------------------------------------------------------------------------
   /// @brief method that reads all fieldplots
   //--------------------------------------------------------------------------
   void readFieldplots();
   //--------------------------------------------------------------------------
   /// @brief method that reads all volumes and generated the priors
   //--------------------------------------------------------------------------
   void readVols_n_ExportPriors(
           const std::string &volumeType,
           double isolevel
           );
   //--------------------------------------------------------------------------
   /// @brief method that exports priors to .csv
   /// @param[in] i_obj the volumetric FW LiDAR
   /// @param[in] i_plots the fieldplots that lie within i_obj
   //--------------------------------------------------------------------------
   void exportPriors(
           Volume *i_obj,
           const std::vector<Int_Plot *> &i_plots,
           const std::string &i_VolFileName
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns whether a prior should be generated for the
   /// given a column (i_x,iy)
   /// @param[in] i_x the x-cooridnate of the column
   /// @param[in] i_y the y-coordinate of the column
   /// @param[in] i_obj the voxelised FW LiDAR of the area of interest
   /// @param[in] i_plots all the plots that lie inside the voxelised FW LiDAR
   //--------------------------------------------------------------------------
   bool isThisColumnIncluded(
           int i_x,
           int i_y,
           const Volume *i_obj,
           const std::vector<Int_Plot *> &i_plots
           );
   //--------------------------------------------------------------------------
   /// @brief method that returns the mean, median and std of a given vector
   /// @param[in] i_vector the vector with the values of interest
   //--------------------------------------------------------------------------
   gmtl::Vec3f getMeanMedianStd(const std::vector<double> &i_vector);
   //--------------------------------------------------------------------------
   /// @brief the directory of the csv file that contains all the fieldplots
   //--------------------------------------------------------------------------   
   std::string csvImportingFieldplots;
   //--------------------------------------------------------------------------
   /// @brief the directory and name of the templates/signatures to be exported
   //--------------------------------------------------------------------------   
   std::string csvExportingTemplates;
   //--------------------------------------------------------------------------
   /// @brief the directory that contails all the volumes of the fieldplots
   //--------------------------------------------------------------------------   
   std::string volsDir;
   //--------------------------------------------------------------------------
   /// @brief the label of the column that gives the classes of the fieldplots
   //--------------------------------------------------------------------------
   std::string fcolumn;
   //--------------------------------------------------------------------------
   /// @brief the name of the class of interest
   //--------------------------------------------------------------------------
   std::string fclass;
   //--------------------------------------------------------------------------
   /// @brief when the option for generating priors for all columns is selected
   /// then there is the option to remove a radius of pixels within distance
   /// "roffset" from the centre of the trees with chosen label fclass
   /// if roffset is set to zero and fclass is "ALL" then all pixels within
   /// fieldplot area is used
   //--------------------------------------------------------------------------
   double roffset;
   //--------------------------------------------------------------------------
   /// @brief the type of the template to be exported (0 square and 1 cylinder)
   //--------------------------------------------------------------------------
   unsigned int ttype; // 0 square and 1 cylinder
   //--------------------------------------------------------------------------
   /// @brief the x y z length of the template if it's square
   //--------------------------------------------------------------------------
   int tx,ty,tz;
   //--------------------------------------------------------------------------
   /// @brief the height and radius of the template if its a cylinder
   //--------------------------------------------------------------------------
   int th,tr;
   //--------------------------------------------------------------------------
   /// @brief moves the template into the y-axis minus n voxels upwards insted
   /// of minus 1 that it is the default value. This must aleays be a positive
   /// number.
   //--------------------------------------------------------------------------
   int mheight;
   //--------------------------------------------------------------------------
   /// @brief the label names of the fieldplot centre that define x and y
   /// respectively
   //--------------------------------------------------------------------------
   std::string fpCenterX_label,fpCentreY_label;
   //--------------------------------------------------------------------------
   /// @brief the label names that define the tree locations
   //--------------------------------------------------------------------------
   std::string fpTreeX_label,fpTreeY_label;
   //--------------------------------------------------------------------------
   /// @brief the label name that define the radius of the fieldplot
   //--------------------------------------------------------------------------
   std::string fpRadius;
   //--------------------------------------------------------------------------
   /// @brief 
   //--------------------------------------------------------------------------
   int eparameters; // 0 for raw, 1 for processed_basic,
                                // 2 for processed_all, -1 for wrong
   //--------------------------------------------------------------------------
   /// @brief all the plots that are saved inside the csv file
   //--------------------------------------------------------------------------
   std::vector<Int_Plot *> m_plots;
   //--------------------------------------------------------------------------
   /// @brief temporary storing the location of pixels of a plot and use them
   /// to extract the mean and standard deviation of the 2D metrics
   //--------------------------------------------------------------------------
   std::vector< std::pair<unsigned int, unsigned int> >m_tmpPixelsOfWindow;
   //--------------------------------------------------------------------------
   /// @brief current plot
   /// used to speed up calculations since usually trees of the same plot are
   /// saved after each other
   //--------------------------------------------------------------------------
   unsigned int short m_currentPlot;
   //--------------------------------------------------------------------------
   /// @brief manages all maps for exporting metrics related to all 2D maps
   //--------------------------------------------------------------------------
   MapsManager m_mapsManager;
   //--------------------------------------------------------------------------
   /// @brief if volsDir not defined then a las file can be imported instead
   //--------------------------------------------------------------------------
   Volume *m_vol;


};

#endif // INT_PLOTSMANAGER_H
