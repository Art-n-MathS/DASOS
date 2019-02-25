DASOS released Date: 20/01/2017

This project is funded by the Centre for Digital Entertainement and Plymouth 
Marine Laboratory and the source code is released under the GNU GENERAL PUBLIC
 LICENSE, Version 3. 

For any publications using DASOS, please reference the following paper:
Miltiadou M., Warren M. A., Grant M., Brown M., 2015, Alignment of 
Hyperspectral Imagery and full-waveform LiDAR data for visualisation and 
classification purposes, ISPRS Archives 36th International Symposium of
Remote Sensing of the enviroment
Available at: http://meetingorganizer.copernicus.org/ISRSE36/ISRSE36-158-2.pdf

For testing the system, please download the following sample here: 
https://rsg.pml.ac.uk/shared_files/arsf/DASOS/
"These sample data were collected by the NERC Airborne Research and Survey
Facility (ARSF). Copyright is held by the UK Natural Environment
Research Council (NERC). The data are free for non-commercial use,
NERC-ARSF must be acknowledged in any publications, software or other
media that make use of these data."


Installation Dependancies:
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

-las <lasFileName>             The name/directory of LAS file. The program only
                               supports LAS1.3 with waveform packet format 4.

-pw <pulsewavesFileName>       loads an a pulsewave file instead of a LAS or an
                               exported volume

-volume <volumeFileName>       loads an exported volume instead of reading a 
                               LAS or pulsewave file

-vols <dir                     loads all the exported volume that are inside 
                               the given directory "dir". This option must and 
                               only be used for generating 3D priors

-vl <voxelLength>              The length of the voxels in meters. Default 
                               value is 2.5m

-nl <noiseLevel>               The threshold that separates noise from the 
                               actual data in the waveforms.Default value is 25
                               Please note that the intensity of each wave 
                               sample haven't been transformed to GHZ yet. 
                               According to the LAS file specifications there 
                               is a way to do, but that will be included in 
                               future releases of DASOS.

-iso <isolevel>                The isolevel defines boundaries of the implicit 
                               object. By default it's zero. Please note that 
                               noise level and isosurface level are related.

-userLimits <maxNorthY> <minNorthY> <maxEastX> <minEastX> 
                               User define Limits of the area of interest

-dtm <dtlFileNameIn_.bil_format> subtract the pre-calculated dtm from each 
                               waveform sample before importing it to the 
                               volume. Please note that the format should be
                               .bil and it currently only supports float 
                               pointing numbers saved into the .bil with 
                               potential limitations. 

-csv <fieldplots.csv>          The input csv file that lists all the trees from
                               a number of field-plot. This is a compulsory 
                               input for generating 3D priors;

-igm <igmFileName>             The name/directory of the .igm file that defines
                               the geolocaiton of the hyperspectral pixels.

-bil <bilFileName>             The name/directory of the .bil file that 
                               contains the hyperspectral cube.

-fodis <fodisFileName>         The name/directory of the .bil file for 
                               hyperspectral imagery 

-obj <objFileName>             The name of the .obj file where the polygon 
                               representation of the LiDAR file will be 
                               exported to. A texture is exported when 
                               hyperspectral images are loaded. Please note, 
                               that a bug was currently detected into the 
                               algorithm and the polygon may look like a bunch
                               of triangle instead. 

-rgb <band1> <band2> <band3>   Defines the 3 bands of the hyperpectral images 
                               that will be used for texturing the polygon mesh
                               If not defined the default values 140, 78 and 23 
                               are used. Only used if a bil and igm file are 
                               loaded.

-map <type> <outputName>      The available types are: 
                              - NON-EMPTY_VOXELS 
                              - DENSITY 
                              - THICKNESS
                              - FIRST_PATCH
                              - AVERAGE_HEIGHT_DIFFERENCE
                              - LAST_PATCH
                              - HYPERSPECTRAL_MEAN
                              - NDVI
                              - LOWEST_RETURN
                              - FIELDPLOT
                              - ALL_FW
                              All the maps are exported into .asc format and 
                              can be loaded int QGIS and other software 
                              packages. The ALL_FW option generates one metrics
                              for each available full-waveform LiDAR related 
                              metrics and their names are:
                              outputName+metricsType+.asc

-map HYPERSPECTRAL <band> <outputName> The hyperspectral map needs an extra 
                              parameter defining which band will be outputed

-exportVolume c <volumeFileName> exports the volume into an ASCII file to speed
                              up future interpolation of the data. 'c' refers 
                              to compressed and its an implicit functionality. 

-column <label>               the label of the column that defines the class of
                              each entry (e.g. <label>= isDead)

-class <className or ALL>     the name of the class (e.g. dead or alive) of 
                              interest or ALL. If a class is chosen, then only 
                              the columns that contain a tree of that class are
                              taken into consideration; a prior is exported for 
                              each tree that belongs to this class only. The 
                              ALL option is the area of interest and generates
                              a template for each column that lies inside the 
                              voxelised space.

-ttype square <x> <y> <z>     generates a squared prior of size x, y, z voxels.
                              The systems finds the first non empty voxel 
                              starting from the top of the column. By default
                              it moves one voxel upwards and sets that to be 
                              the top of the prior. It is highly recommended to
                              use odd numbers, otherwise the centre of the 
                              prior will be wrongly set and unpredicted output 
                              values may occur.

-ttype cylinder <h> <r>       generates a cylindrical template with height h 
                              and diameter (2 × r + 1) voxels and height h. The
                              systems finds the first non empty voxel starting
                              from the top of the column. By default it moves 
                              one voxel upwards and sets that to be the top of
                              the prior.

-mheight <n>                  moves the template into the y-axis n voxels 
                              upwards instead of one which is the default. The
                              value n must be a positive number.

-eparameters <raw or processed> the ‘raw’ option saves all the intensity values
                              of the template and the or processed> ‘processed’ 
                              option saves parameters derived from the raw 
                              intensities.                        
                              


Examples Commands:
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj Polygon -vl 3
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj ColouredPolygon -vl 3.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness -map NDVI ndvi
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness -map NDVI ndvi -obj happy -exportVolume twiceHappy.vol

Full userguide available at: https://github.com/Art-n-MathS/DASOS/DASOS_userGuide_v2.pdf

Please note that this is a research software generated for supporting my thesis
Therefore it may be file formats dependant and it may contain bugs.
Identified bugs potential improvements list can be found here:
https://docs.google.com/spreadsheets/d/10yE5p463cLA_GtKkyiaWEzScW7N9cVxbPs5y0muXuZY/edit?usp=sharing

For bringing potential issues in discussion please use our group:
https://groups.google.com/forum/#!forum/dasos---the-native-full-waveform-fw-lidar-software

For updates and news please follow twitter @MiltoMiltiadou



