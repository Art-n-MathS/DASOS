#include "Int_PlotsManager.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include "Volume.h"
#include "VolumeFactory.h"
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <numeric>

//-----------------------------------------------------------------------------
Int_PlotsManager::Int_PlotsManager(
        const std::string i_csvFile
        )
    : csvImportingFieldplots(i_csvFile)
    , csvExportingTemplates("")
    , volsDir("")
    , fcolumn("")
    , fclass("")
    , roffset(-0.5)
    , ttype(0)
    , tx(3) ,ty(3), tz(9)
    , th(9),tr(2)
    , mheight(1)
    , fpCenterX_label("EASTING"),fpCentreY_label("NORTHING")
    , fpTreeX_label("X"),fpTreeY_label("Y")
    , fpRadius("RADIUS")
    , eparameters(0)
    , m_currentPlot(0)
{
  readFieldplots();
}

//-----------------------------------------------------------------------------
bool Int_PlotsManager::checkValidity() const
{
   if (csvExportingTemplates=="")
       std::cout << "ERROR: csvExportingTemplates was not defined\n";
   if (csvImportingFieldplots=="")
       std::cout << "ERROR: csvImportingFieldplots was not defined\n";
   if (volsDir=="")
       std::cout << "ERROR: volsDir was not defined\n";
   if (fcolumn=="")
       std::cout << "ERROR: fcolumn was not defined\n";
   if (fclass=="")
       std::cout << "ERROR: fclass was not defined\n";
   if (fpCenterX_label=="")
       std::cout << "ERROR: fpCenterX_label was not defined\n";
   if (fpCentreY_label=="")
       std::cout << "ERROR: fpCentreY_label was not defined\n";
   if (fpTreeX_label=="")
       std::cout << "ERROR: fpTreeX_label was not defined\n";
   if (fpTreeY_label=="")
       std::cout << "ERROR: fpTreeY_label was not defined\n";
   if (fpRadius=="")
       std::cout << "ERROR: fpRadius was not defined\n";
   if ((eparameters<0 || eparameters>3))
   {
      std::cout <<  eparameters  << " parameters\n";
      std::cout << "ERROR: eparameters only takes the values raw and processed \n";
   }
   if ((tx<=0 || ty<=0 || tz<=0))
       std::cout << "ERROR : tx, ty and tz should be positive integer numbers\n";
   if (tr<=0 || th<=0)
       std::cout << "ERRORL tr and th should be positive numbers";

   return ( (csvExportingTemplates!="") &&
            (csvImportingFieldplots!="") &&
            (volsDir!="") &&
            (fcolumn!="") &&
            (fclass!="") &&
            (ttype<2) &&
            (tx>0 && ty>0 && tz>0)  &&
            (tr>0 && th>0 ) &&
            /*mheight - checked later*/
            (fpCenterX_label!="" && fpCentreY_label!="")  &&
            (fpTreeX_label!="" && fpTreeY_label!="")  &&
            (fpRadius!="") &&
            (eparameters>=0 && eparameters<3)
           );
}

//-----------------------------------------------------------------------------
Int_PlotsManager::Int_PlotsManager()
    : csvImportingFieldplots("")
    , csvExportingTemplates("")
    , volsDir("")
    , fcolumn("")
    , fclass("")
    , roffset(-0.5)
    , ttype(0)
    , tx(3) ,ty(3), tz(9)
    , th(9),tr(2)
    , mheight(1)
    , fpCenterX_label("EASTING"),fpCentreY_label("NORTHING")
    , fpTreeX_label("X"),fpTreeY_label("Y")
    , fpRadius("RADIUS")
    , eparameters(0)
    , m_currentPlot(0)
{

}

//-----------------------------------------------------------------------------
const Int_Plot * Int_PlotsManager::getPlotOfAOI(
        double i_minX,
        double i_minY,
        double i_lenX,
        double i_lenY) const
{
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      if(m_plots[i]->isCentreOfPlotInsideVol(i_minX,i_minY,i_lenX,i_lenY))
      {
         return m_plots[i];
      }
   }
   std::cerr << "WARNING:: No plot found in the Area of Interest."
             << "Returning empty pointer\n";
   return NULL;
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::readFieldplots()
{
    if(m_plots.size()!=0)
    {
       std::cout<<"WARNING: Fieldplots Already Loaded. Dublicates may occur\n";
    }
    std::ifstream infile( csvImportingFieldplots );
    std::vector<std::string> labels;
    std::string s;
    if (!std::getline( infile, s )) return;

    std::istringstream ss( s );
    std::vector <std::string> record;
    while (ss)
    {
      std::string subS;
      if (!std::getline( ss, subS, ',' )) break;
      labels.push_back(subS);
    }
    for(auto &l : labels) for (auto & c: l) c = std::toupper(c);
    while (infile)
    {
       if (!std::getline( infile, s )) break;
       record.clear();
       ss = std::istringstream(s);
       while(ss)
       {
          std::string s;
          if (!std::getline( ss, s, ',' )) break;
          record.push_back(s);
       }
       addTree(labels,record);
    }
    if (!infile.eof())
    {
       std::cerr << "Error: CSV file!\n";
    }
}


//-----------------------------------------------------------------------------
void Int_PlotsManager::addTree(
        const std::vector<std::string> &i_labels,
        const std::vector<std::string> &i_treeInfo
        )
{
    assert(i_labels.size()==i_treeInfo.size());
    std::map<std::string, int> c_csvLabels=
    {
        {fpTreeX_label,4},
        {fpTreeY_label,5},
        {fpRadius,6},
        {fpCentreY_label,7},
        {fpCenterX_label,8},
        {fcolumn,10}
    };

   if(i_labels.size()!=i_treeInfo.size())
   {
      std::cout << "WARNING: Potential empty line, therefore "
                << "line is ignored\n";
      return;
   }
   Int_Plot::CSVtreeInfo *nextTree = new Int_Plot::CSVtreeInfo;
   // information for identifying fl
   double eastingX = 0.0f;
   double radius = 0.0f;
   double northingY = 0.0f;

   // read info for the tree
   for(unsigned int i=0; i<i_labels.size(); ++i)
   {
      std::string label=i_labels[i];
      std::string value=i_treeInfo[i];

      switch(c_csvLabels[label])
      {
      case 4: // X position of tree
          nextTree->xGPS = std::atof(value.c_str());
          break;
      case 5: // Y position of tree
          nextTree->yGPS = std::atof(value.c_str());
          break;
      case 6: // plot radius
          radius = std::atof(value.c_str());
          break;
      case 7: // northing x
          northingY = std::atof(value.c_str());
          break;
      case 8: // easting y
          eastingX = atof(value.c_str());
          break;
      case 10: // does it belong to the class of interest
          for (auto & c: value) c = std::toupper(c);
          if(value==fclass)
          {
              nextTree->isfclass = true;
          }
          nextTree->fclass=value;
          break;
          // else tree is alive, the default value
      default:
          assert(!(label==fpCenterX_label || label==fpCentreY_label ||
                   label==fpRadius || label==fpTreeX_label ||
                   label==fcolumn  || label==fpTreeY_label));
          // information not important for this project
          break;
      }
   }

   // find the plot the tree belongs to
   // create a plot if plot doesn't exist
   // save tree to the corresponding plot
   if(m_plots.size()==0)
   {
      m_plots.push_back(new Int_Plot(radius,northingY,eastingX));
      m_plots[m_currentPlot]->addTree(nextTree);
      return;
   }

   if(m_plots[m_currentPlot]->isThisPlot(northingY,eastingX,radius))
   {
      m_plots[m_currentPlot]->addTree(nextTree);
      return;
   }
   else // find the plot that the tree lies inside or create a new plot
   {
      m_currentPlot=0;
      for(; m_currentPlot<m_plots.size(); ++m_currentPlot)
      {
         if(m_plots[m_currentPlot]->isThisPlot(northingY,eastingX,radius))
         {
            break;
         }
      }
      if(m_currentPlot==m_plots.size())
      {
         Int_Plot *plot = new Int_Plot(radius,northingY,eastingX);
         if(plot!=NULL)
         {
            m_plots.push_back(plot);
         }
         else
         {
            std::cerr<<"WARNING: Failed to allocated memory. Fieldplot ignored\n";
         }
      }
   }
   m_plots[m_currentPlot]->addTree(nextTree);
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::printParameters()const
{
  std::cout << "inputCSV: "  << csvImportingFieldplots << "\n";
  std::cout << "outCSV: " << csvExportingTemplates << "\n";
  std::cout << "vols Directory: " << volsDir << "\n";
  std::cout << "fcolumn: " << fcolumn << "\n";
  std::cout << "fclass-roffset: " << fclass << " - " << roffset << "\n";
  std::cout << "ttype: " << ttype << "\n";
  std::cout << "tx ty tz : " << tx << " " << ty << " " << tz << "\n";
  std::cout << "tr rh: " << tr << " " << th << "\n";
  std::cout << "mheight: " << mheight << "\n";
  std::cout << "fieldplot centre names for x y: " << fpCenterX_label << " "
            << fpCentreY_label << "\n";
  std::cout << "tree locations names for x y: " << fpTreeX_label << " "
            << fpTreeY_label << "\n";
  std::cout << "radius of the fieldplot name: " << fpRadius << "\n";
  std::cout << "eparameters: " << eparameters <<"\n";


 }

//-----------------------------------------------------------------------------
void Int_PlotsManager::printPlots()const
{
   std::cout << "NoOfPlots : " << m_plots.size() <<"n";
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      std::cout << "\n+++++++++++++++++++++  " << i <<"  +++++++++++++++++++\n\n";
      m_plots[i]->print();
   }
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::interprateData(
        const std::string &volumeType,
        double isolevel,
        double offset
        )
{
   if(!checkValidity())
   {
     std::cerr << "ERROR: Some template/fieldplot related parameters has been loaded but not "
                  "all the required ones, or some of them contain incorrect values. \n";
     return;
   }

   // read fieldplots
   readFieldplots();
//   printPlots();

   // read volumes and generate templates
   readVols_n_ExportPriors(volumeType,isolevel);

}


//-----------------------------------------------------------------------------
bool Int_PlotsManager::isThisColumnIncluded(
        int i_x,
        int i_y,
        const Volume *i_obj,
        const std::vector<Int_Plot *> &i_plots
        )
{
   gmtl::Vec2f xyCoordinates = i_obj->getCoordinatesMiddle(i_x,i_y);
   bool isValid_n_insidePlot(false);
   double vl = i_obj->getVoxelLen();
   for(unsigned int i=0; i<i_plots.size(); ++i)
   {
      if(fclass=="ALL" ||
         (i_plots[i]->isPointInsidePlot(xyCoordinates[0],xyCoordinates[1],
                                        ((ttype)?tr:std::max(tx,ty))) &&
          i_plots[i]->isColumnInAOI(xyCoordinates[0],xyCoordinates[1],vl,
                                   fclass,roffset)))
      {
         isValid_n_insidePlot=true;
         break;
      }
   }
   return (isValid_n_insidePlot &&
           // check if they are close to boundaries
           ((ttype==1 &&  i_x-tr>=0 && i_x+tr<int(i_obj->getNoVoxelsX()) &&
             i_y>=tr && i_y+tr<int(i_obj->getNoVoxelsY())) ||

            (ttype==0 && i_x-floor(double(tx)/2.0)>=0.0
             && i_x+floor(double(tx)/2.0)<int(i_obj->getNoVoxelsX()) &&
             i_y>=ty && i_y+ty<int(i_obj->getNoVoxelsY())))
           );
}

//-----------------------------------------------------------------------------
bool Int_PlotsManager::isInsideCylinder(
        double xCol,
        double yCol
        )
{
   double c(tr);
   double disfromCentre=sqrt((xCol-c)*(xCol-c)+(yCol-c)*(yCol-c));
//   std::cout << disfromCentre << " - " << tr << "\n";
   return disfromCentre<=double(tr);
}


//-----------------------------------------------------------------------------
gmtl::Vec3f Int_PlotsManager::getMeanMedianStd(
        const std::vector<double> &i_vector
        )
{
   std::vector<double> vector(i_vector);
   std::sort(vector.begin(),vector.end());
   double sumDis = std::accumulate(vector.begin(),vector.end(),0.0);
   double meanDis = sumDis/double(vector.size());
   std::vector<double>disDiff(vector.size());
   std::transform(vector.begin(),vector.end(),disDiff.begin(),
                  std::bind2nd(std::minus<double>(),meanDis));
   double stdDis = std::inner_product(disDiff.begin(),disDiff.end(),
                                      disDiff.begin(),0.0);
   stdDis = std::sqrt(stdDis/vector.size());
   return gmtl::Vec3f(meanDis,vector[floor(double(vector.size())/2.0)],stdDis);
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::printLabelsIn(std::ofstream &i_file)
{
    i_file << ",Height_Middle_Column,Height_Mean,Height_Median,Height_Std,"
              "Sum_Int_Diff_X,Sum_Int_Diff_Y,Sum_Int_Diff_Z,Max_Int,Min_Int,"
              "Ave_Int,Median_Int,Per_Int_Above_Iso,Dis_Mean,Dis_Median,"
              "Dis_Std,Top_Patch_Len_Middle_Col,Top_Patch_Len_Mean,"
              "Top_Patch_Len_Median,Top_Patch_Len_Std,Mirror_Diff_X_Mean,"
              "Mirror_Diff_X_Median,Mirror_Diff_X_Std,Mirror_Diff_Y_Mean,"
              "Mirror_Diff_Y_Median,Mirror_Diff_Y_Std,Mirror_Diff_Z_Mean,"
              "Mirror_Diff_Z_Median,Mirror_Diff_Z_Std";
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::printProcessedValues(
        std::ofstream &i_file,
        const std::vector<double> &i_values,
        double i_vl,
        double i_height,
        double i_isolevel
        // 11 & 12. Mean and Standard Diviation distance from centre to
        // non-empty voxels (a)
        )
{
   // initialisation
   int maxX(ttype==0?tx:tr*2+1),
       maxY(ttype==0?ty:tr*2+1),
       maxZ(ttype==0?tz:th),
       countAll(0),zCountTop(0),zCountFP(0);
   double sumIntX1(0.0),sumIntZ1(0.0),sumIntY1(0.0),sumIntX2(0.0),
          sumIntZ2(0.0),sumIntY2(0.0),currentValue(0.0),min(0.0),max(0.0),
          sumAve(0.0);
   int midEndX(floor(maxX/2)),midStartX(ceil(maxX/2)),midEndY(floor(maxY/2)),
       midStartY(ceil(maxY/2)),midEndZ(floor(maxZ/2)),midStartZ(ceil(maxZ/2)),
       countNonEmpty(0);
   unsigned int stillCountingZ(0);

   if(i_values.size()>0) {min=i_values[0];max=i_values[0];}

   std::vector<double> valuesSortedMinToMax(i_values),disFromCentre,heights,
           firstPathLens,mirrorDiffX,mirrorDiffY,mirrorDiffZ;

   // Median Intensity  if size is an even number then the +1 is chosen
   std::sort(valuesSortedMinToMax.begin(),valuesSortedMinToMax.end());
   gmtl::Vec3f centralVoxel;
   centralVoxel[0] = floor(double(maxX)/2.0);
   centralVoxel[1] = floor(double(maxY)/2.0);
   centralVoxel[2] = floor(double(maxZ)/2.0);

   // interpretation
   for(int dX=0; dX<maxX; ++dX)
   {
      for(int dY=0; dY<maxY; ++dY)
      {
         std::cout << dX << " " << dY << "  : ";
         if(ttype==0 || (ttype==1 && isInsideCylinder(dX,dY)))
         {
            zCountTop=0; stillCountingZ=0; zCountFP=0;
            for(int dZ(maxZ-1); dZ>=0 ; --dZ) // from top to bottom
            {
               countAll++;
               currentValue = i_values[dX+dY*maxX+dZ*maxX*maxY];


               // 2-4. Average Height (a/c)
               if(stillCountingZ==0)
               {
                  ++zCountTop;
                  if(currentValue>=i_isolevel) stillCountingZ=1;
               }
               // 16-19. Mid Mean Median Std of the len of 1st Patch from Top(a/c)
               if(stillCountingZ==1)
               {
                   ++zCountFP;
                   if(currentValue<i_isolevel) stillCountingZ=2;
               }
               // 5. Mirror Sum Difference in the x-axis: difference of summed Int
               if(dX<midEndX) sumIntX1+=currentValue;
               else if (dX>midStartX) sumIntX2+=currentValue;
               // 6. Mirror Sum Difference in the y-axis: difference of summed Int
               if(dY<midEndY) sumIntY1+=currentValue;
               else if (dY>midStartY) sumIntY2+=currentValue;
               // 7. Mirror Sum Difference in the z-axis: difference of summed Int
               if(dZ<midEndZ) sumIntZ1+=currentValue;
               else if (dZ>midStartZ)sumIntZ2+=currentValue;
               // 8 & 9. Max and Min intensities
               if(currentValue>max) max=currentValue;
               else if(currentValue<min) min=currentValue;
               // 10. Average Intensity
               sumAve+=currentValue;

               if(currentValue>=i_isolevel)
               {
                   // 12. Percentage of voxels with intensity above isolevel
                   countNonEmpty++;
                   // 13-14. Median, Mean and Standard Deviation distance from
                   // centre to non-empty voxels (a/b)
                   double a(centralVoxel[0]-double(dX)),
                          b(centralVoxel[1]-double(dY)),
                          c(centralVoxel[2]-double(dZ));
                   disFromCentre.push_back(sqrt(a*a+b*b+c*c));
               }

               // 21-22. Mirror Difference in the x-axis (a/b)
               if(dX<midEndX)
               {
                  mirrorDiffX.push_back(std::abs(currentValue-
                                  i_values[(maxX-1-dX)+dY*maxX+dZ*maxX*maxY]));
               }
               // 23-25. Mirror Difference in the y-axis (a/b)
               if(dY<midEndY)
               {
                  mirrorDiffY.push_back(std::abs(currentValue-
                                  i_values[dX+(maxY-1-dY)*maxX+dZ*maxX*maxY]));
               }
               // 26-28. Mirror Difference in the z-axis (a/b)
               if(dZ<midEndZ)
               {
                  mirrorDiffZ.push_back(std::abs(currentValue-
                                  i_values[dX+dY*maxX+(maxZ-1-dZ)*maxX*maxY]));
               }
            }
            // 2-4. Height Mean, Median, std (b/c)
            heights.push_back(i_height-i_vl*(maxZ-zCountTop));
            // 16-19. Mid Mean Median Std of the len of 1st Patch from Top(b/c)
            firstPathLens.push_back(zCountFP*i_vl);
         }
      }
   }


   // 13-14. Median, Mean and Standard Diviation distance from
   // centre tonon-empty voxels (b/b)
   gmtl::Vec3f MeanMedianStdDis = getMeanMedianStd(disFromCentre);
   // 2-4. Height Mean, Median, std (c/c)
   gmtl::Vec3f MeanMedianStdHei = getMeanMedianStd(heights);
   // 16-19. Mid Mean Median Std of the len of 1st Patch from Top(c/c)
   gmtl::Vec3f MeanMeadianStdFPLen = getMeanMedianStd(firstPathLens);
   // 20-22. Mirror Difference in the x-axis (b/b)
   gmtl::Vec3f MeanMeadianStdMirrorDiffX = getMeanMedianStd(mirrorDiffX);
   // 23-25. Mirror Difference in the y-axis (b/b)
   gmtl::Vec3f MeanMeadianStdMirrorDiffY = getMeanMedianStd(mirrorDiffY);
   // 26-28. Mirror Difference in the z-axis (b/b)
   gmtl::Vec3f MeanMeadianStdMirrorDiffZ = getMeanMedianStd(mirrorDiffZ);


   // save results to file
   i_file << i_height-i_vl*(mheight);             // 1 . Height_Middle_Column
   i_file << "," << MeanMedianStdHei[0];          // 2 . Height_Mean
   i_file << "," << MeanMedianStdHei[1];          // 3 . Height_Median
   i_file << "," << MeanMedianStdHei[2];          // 4 . Height_std
   i_file << "," << std::abs(sumIntX1-sumIntX2);  // 5 . Sum_Int_Diff_X
   i_file << "," << std::abs(sumIntY1-sumIntY2);  // 6 . Sum_Int_Diff_Y
   i_file << "," << std::abs(sumIntZ1-sumIntZ2);  // 7 . Sum_Int_Diff_Z
   i_file << "," << max;                          // 8 . Max_Int
   i_file << "," << min;                          // 9 . Min_Int
   i_file << "," << sumAve/countAll;              // 10. Ave_Int
   i_file << "," << valuesSortedMinToMax          // 11. Median_Int
             [floor(double(valuesSortedMinToMax.size())/2.0)];
   i_file << "," << double(countNonEmpty)/        // 12. Per_Int_Above_Iso
             double(countAll)*100.0;
   i_file << "," << MeanMedianStdDis[0];          // 13. Dis_Mean
   i_file << "," << MeanMedianStdDis[1];          // 14. Dis_Median
   i_file << "," << MeanMedianStdDis[2];          // 15. Dis_Std

   i_file << "," <<firstPathLens[                 // 16. Top_Patch_Len_Middle_Col
             int(centralVoxel[0])+int(centralVoxel[1])*maxX+
             int(centralVoxel[2])*maxX*maxY];
   i_file << "," << MeanMeadianStdFPLen[0];       // 17. Top_Patch_Len_Mean
   i_file << "," << MeanMeadianStdFPLen[1];       // 18. Top_Patch_Len_Median
   i_file << "," << MeanMeadianStdFPLen[2];       // 19. Top_Patch_Len_Std
   i_file << "," << MeanMeadianStdMirrorDiffX[0]; // 20. Mirror_Diff_X_Mean
   i_file << "," << MeanMeadianStdMirrorDiffX[1]; // 21. Mirror_Diff_X_Median
   i_file << "," << MeanMeadianStdMirrorDiffX[2]; // 22. Mirror_Diff_X_Std
   i_file << "," << MeanMeadianStdMirrorDiffY[0]; // 23. Mirror_Diff_Y_Mean
   i_file << "," << MeanMeadianStdMirrorDiffY[1]; // 24. Mirror_Diff_Y_Median
   i_file << "," << MeanMeadianStdMirrorDiffY[2]; // 25. Mirror_Diff_Y_Std
   i_file << "," << MeanMeadianStdMirrorDiffZ[0]; // 26. Mirror_Diff_Z_Mean
   i_file << "," << MeanMeadianStdMirrorDiffZ[1]; // 27. Mirror_Diff_Z_Median
   i_file << "," << MeanMeadianStdMirrorDiffZ[2]; // 28. Mirror_Diff_Z_Std

   // Mirror Difference in the y-axis : using intensities as booleans

   // Mirror Difference in the y-axis

   // Mirror Difference in the z-axis

   // Average Height Difference standard deviation


}

//-----------------------------------------------------------------------------
void Int_PlotsManager::exportPriors(
        Volume *i_obj,
        const std::vector<Int_Plot *> &i_plots,
        const std::__cxx11::string &i_VolFileName
        )
{
   if((!(tx%2 | ty%2)) && ttype==0 /*square*/)
   {
      std::cout<<"Warning: Centre of template not in the centre. "
               <<"Please choose an odd number to move centre in the middle.\n";
   }
   gmtl::Vec2f xyCoordinates;
   std::ofstream fileWithTemplates;
   const std::string csvFileName(csvExportingTemplates+"_" +i_VolFileName.substr
                            (i_VolFileName.find_last_of("/\\")+1)+".csv");

   fileWithTemplates.open(csvFileName);
   if(!fileWithTemplates.is_open())
   {
      std::cerr << "ERROR: failed to generate csv file at given"
                   " directory\n";
      return;
   }
   fileWithTemplates << "Index, centroid_x, centroid_y";
   unsigned int maxX(ttype==0?tx:tr*2+1);
   unsigned int maxY(ttype==0?ty:tr*2+1);
   unsigned int maxZ(ttype==0?tz:th);
   if(eparameters==0)
   {
      for(int dX=0; dX<maxX; ++dX)
      {
         for(int dY=0; dY<maxY; ++dY)
         {
            if(ttype==0 || (ttype==1 && isInsideCylinder(dX,dY)))
            {
               for(int dZ=0; dZ<maxZ ; ++dZ)
               {
                  fileWithTemplates << ", V" << dX << "_" << dY << "_" << dZ  ;
               }
            }
         }
      }
   }
   else  if(eparameters==1 )
   {
      printLabelsIn(fileWithTemplates);
   }
   fileWithTemplates <<"\n";
   fileWithTemplates << std::setprecision(3) << std::fixed;
   std::vector<double> currentPrior(maxX*maxY*maxZ,0.0);

   unsigned int x(0),y(0),z(0),
                xVolLen(i_obj->getNoVoxelsX()),
                yVolLen(i_obj->getNoVoxelsY());
   unsigned int countIncludedCols(0);
   double topHeight(0.0);
   for(unsigned int xVol=0; xVol<xVolLen; ++xVol)
   {
      for(unsigned int yVol=0; yVol<yVolLen; ++yVol)
      {
         if(isThisColumnIncluded(xVol,yVol,i_obj,i_plots))
         {
             xyCoordinates = i_obj->getCoordinatesMiddle(xVol,yVol);
             int zVol=i_obj->getNoVoxelsZ()-1;
             for(; zVol>0; --zVol)

             {
                if(i_obj->isInside(xVol,yVol,zVol))
                {
                   break;
                }
             }
             if(zVol<tz)
             {
                std::cerr << "WARNING: Potential empty column inside the"
                          << " voxelised space, prior ignored.\n";
                continue;
             }
             topHeight = i_obj->getMaxLimits()[2]-
                   (i_obj->getNoVoxelsZ()-zVol-mheight-1)*i_obj->getVoxelLen();
             if(ttype==0) // square
             {
                x = xVol-floor(double(tx/2.0));
                y = yVol-floor(double(ty/2.0));
                z = zVol-tz+1+mheight;
             }
             else //ttype == 1 - cylinder
             {
                x = xVol-tr;
                y = yVol-tr;
                z = zVol-th+1+mheight;
             }


             fileWithTemplates << countIncludedCols << ","  ;
             countIncludedCols++;
             std::fill(currentPrior.begin(),currentPrior.end(),0.0);
             fileWithTemplates << xyCoordinates[0] << "," << xyCoordinates[1] <<",";
             for(unsigned int dX=0; dX<maxX ; ++dX)
             {
                for(unsigned int dY=0; dY<maxY ; ++dY)
                {
                   if(ttype==0 || (ttype==1 && isInsideCylinder(dX,dY)))
                   {
                      for(unsigned int dZ=0; dZ<maxZ; ++dZ)
                      {
                         if(eparameters>0)
                         {
                            currentPrior[dX+dY*maxX+dZ*maxX*maxY]=i_obj->getIntensity(x+dX,y+dY,z+dZ);
                         }
                         else
                         {
                            fileWithTemplates << i_obj->getIntensity(x+dX,y+dY,z+dZ);
                            if((ttype==0 && (dX!=tx-1 || dY!=ty-1 || dZ!=tz-1)) ||
                               (ttype==1 && (dX!=tr*2 || dY!=tr*2 || dZ!=th-1)))
                            {
                                fileWithTemplates << ",";
                            }
                         }
                      }
                   }
                }
             }
             if(eparameters>0)
             {
                printProcessedValues(fileWithTemplates,currentPrior,
                                     i_obj->getVoxelLen(),topHeight,
                                     i_obj->getIsolevel());
             }

             fileWithTemplates << "\n";
         }
         // else
         {
            // column outside;
         }
      }
   }
   fileWithTemplates.close();
}

//-----------------------------------------------------------------------------
void Int_PlotsManager::readVols_n_ExportPriors(
        const std::string &volumeType,
        double isolevel
        )
{
    Volume *vol = NULL;
    std::vector<double> userLimits(6);
       // read all vols for each one create a template with dead trees or all cols
       std::string extension(".vol");
       DIR *dir;
       struct dirent *ent;
       std::vector<Int_Plot *>currentPlots;
       if ((dir = opendir (volsDir.c_str())) != NULL)
       {
         while((ent = readdir (dir)) != NULL)
         {
           std::string current(ent->d_name);
           if(current.size()>extension.size())
           {
              std::string ext = current.substr(current.length()
                                               -extension.length());
              if(ext==extension)
              {
                  current = volsDir + "/" + current;
                  vol = VolumeFactory::produceVolume(current,volumeType);
                  gmtl::Vec3f maxLimits = vol->getMaxLimits();
                  gmtl::Vec3f minLimits = vol->getMinLimits();
                  vol->setIsolevel(isolevel);
                  currentPlots.clear();
                  for(unsigned int p=0; p<m_plots.size(); ++p)
                  {
                     if(m_plots[p]->isCentreOfPlotInsideVol(minLimits[0],
                                 minLimits[1],maxLimits[0]-minLimits[0],
                                             maxLimits[1]-minLimits[1]))
                     {
                        currentPlots.push_back(m_plots[p]);
                     }
                  }
                  if(currentPlots.size()==0)
                  {
                     std::cout << "WARNING: No fieldplot passing through volume "
                               << current << "\n";
                     continue;
                  }

                  exportPriors(vol,currentPlots,current);

                  if(vol!=NULL)
                  {
                     delete vol;
                     vol = NULL;
                  }
              }
           }
         }
       }
       else
       {
          std::cerr << "ERROR: Directory of volumes was not found\n";
          std::exit(EXIT_FAILURE);
       }
}

//-----------------------------------------------------------------------------
Int_PlotsManager::~Int_PlotsManager()
{
   for(unsigned int i=0; i<m_plots.size(); ++i)
   {
      if(m_plots[i]!=NULL)
      {
         delete m_plots[i];
      }
   }
}
