This project is funded by the Centre for Digital Entertainement and Plymouth Marine Laboratory and the source code is released under the GNU GENERAL PUBLIC LICENSE, Version 3. 

For any publications using DASOS, please reference the following paper:
Miltiadou M., Warren M. A., Grant M., Brown M., 2015, Alignment of Hyperspectral Imagery and full-waveform LiDAR data for visualisation and classification purposes, 36th International Symposium of Remote Sensing of the enviroment
Available at: http://meetingorganizer.copernicus.org/ISRSE36/ISRSE36-158-2.pdf

For testing the system, please download and copy the sample data inside a folder named 'data'.
"These data were collected by the NERC Airborne Research and Survey
Facility (ARSF). Copyright is held by the UK Natural Environment
Research Council (NERC). The data are free for non-commercial use,
NERC-ARSF must be acknowledged in any publications, software or other
media that make use of these data."
Available at: https://rsg.pml.ac.uk/shared_files/arsf/DASOS/

Dependancies:
 qmake-qt4
 gmtl library - please make sure that .pro file points to the correct directory
 -std=c++11


Installation guide:
$: qmake-qt4
$: make


Instructions on how to use the software:
$: ./DASOS --help

DASOS User Quide:
---------------------

-las <lasFileName>             The name/directory of LAS file to be loaded and it's compulsory (expection apply when a volume or a pulsewave file is loaded). The program only supports LAS1.3 with waveform packet format 4 while waveforms can either be internally saved into the las file or expetrnal saved into a .wdp file. 

-pw <pulsewavesFileName>       loads an a pulsewave file instead of a LAS or an exported volume

-igm <igmFileName>             The name/directory of the .igm file that defines the geolocaiton of the hyperspectral pixels.

-bil <bilFileName>             The name/directory of the .bil file that contains the hyperspectral cube.

-obj <objFileName>             The name of the .obj file where the polygon representation of the LiDAR file will be exported to. It's optional and when not defined the data are not polygonised. Further it exports a texture when the hyperspectral data are also defined if hyperspectral images are also loaded. Please note, that a bug was currently detected into the algorithm, so it's possible to get some unexpected results with that command.

-rgb <band1> <band2> <band3>   Defines the 3 bands of the hyperpectral images that will be used for texturing the polygon mesh. If not defined the default values 140, 78 and 23 are used.

-vl <voxelLength>              The length of the voxels in meters. Default value is 3.2m

-nl <noiseLevel>               The threshold that separates noise from the actual data in the waveforms.Default value is 25. Please note that the intensity of each wavesample haven't been transformed to GHZ yet. According to the LAS file specifications there is a way to do, but that will be included in future releases of DASOS.

-iso <isolevel>                The isolevel defines boundaries of the implicit object. 

-map <type> <outputName>      The available types are: \"NON-EMPTY_VOXELS\", \"DENSITY\", \"THICKNESS\", \"FIRST_PATCH\", \"AVERAGE_HEIGHT_DIFFERENCE\", \"LAST_PATCH\", \"HYPERSPECTRAL_MEAN\", \"NDVI\", \"LOWEST_RETURN\" , \"FIELDPLOT\", \"ALL_FW\". All the maps are exported into .asc format and can be loaded int QGIS and other software packages. The ALL_FW option generates one metrics for each available full-waveform LiDAR related metrics and their names are the given outputName+metricsType+.asc

-map HYPERSPECTRAL <band> <outputName> The hyperspectral map needs an extra parameter defining which band will be outputed

-map <type> -thres <threshold> <outputName>  A threshold is optional and can be added to any type of maps. Always added before the <outputname>

-userLimits <MaxNorthY> <MinNorthY> <MaxEastX> <MinEastX> User define Limits of the area of interest.

-exportVolume <volumeFileName> exports the volume into a txt file to speed up future interpolation of the data.

-volume <volumeFileName>       loads an exported volume instead of reading a LAS or pulsewave file

-userLimits <maxNorthY> <minNorthY> <maxEastX> <minEastX> User define Limits of the area of interest

-dtm <dtlFileNameIn_.bil_format> subtract the pre-calculated dtm from each waveform sample before importing it to the volume. Please note that the format should be .bil and it currently only supports float pointing numbers saved into the .bil and probably specific files. 

Examples Commands:
$:  ./DASOS -default
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj Polygon -vl 3
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj ColouredPolygon -vl 3.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness.png -map NDVI ndvi.png
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness.png -map NDVI ndvi.png -obj happy

Please note that this is a research software generated for supporting my thesis. Therefore it's possible that many file formats depend on my specific input data and that bugs may also exists. Identified bugs potential improvements list can be found here:
https://docs.google.com/spreadsheets/d/10yE5p463cLA_GtKkyiaWEzScW7N9cVxbPs5y0muXuZY/edit?usp=sharing


