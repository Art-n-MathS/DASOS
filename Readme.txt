This project is funded by the Centre for Digital Entertainement and Plymouth Marine Laboratory and the source code is released under the GNU GENERAL PUBLIC LICENSE, Version 3. For any publications or public use please reference the related paper "ALIGNMENT OF HYPERSPECTRAL IMAGERY AND FULL-WAVEFORM LIDAR DATA FOR VISUALISATION AND CLASSIFICATION PURPOSES", which was presented at the 36th International Symposium of Remote Sensing of the Enviroment 2015. 

For testing the system, please download and copy the sample data inside the 'data' folder.
These data were collected by the NERC Airborne Research and Survey
Facility (ARSF). Copyright is held by the UK Natural Environment
Research Council (NERC). The data are free for non-commercial use,
NERC-ARSF must be acknowledged in any publications, software or other
media that make use of these data.
Available at:https: //rsg.pml.ac.uk/shared_files/arsf/DASOS/

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

-las <lasFileName>             The name/directory of LAS file to be loaded and it's compulsory

-igm <igmFileName>             The name/directory of the igm file that defines the geolocaiton of the hyperspectral pixels.

-bil <bilFileName>             The name/directory of the bil file that contains the hyperspectral cube.

-fodis <fodisFileName>         The name/directory of the fodis files required for the NDVI map

-obj <objFileName>             The name of the .obj file where the polygon representation of the LiDAR file will be exported to. It's optional and when not defined the data are not polygonised. Further it exports a texture when the hyperspectral data are also defined

-rgb <band1> <band2> <band3>   Defines the 3 bands of the hyperpectral images that will be used for texturing the polygon mesh. If not defined the default values 140, 78 and 23 are used.

-vl <voxelLength>              The length of the voxels in meters. Default value is 3.2m

-nl <noiseLevel>               The threshold that separates noise from the actual data in the waveforms.Default value is 25

-iso <isolevel>                The isolevel defines boundaries of the implicit object. The voxel values lies inside the range [-100,100] and everything greater than the isolevel is considered to be inside the object. Default value is -99.9

-opt <on/off>                  Enables/Disables the optimisation that uses Integral Images to discard empty chunks of voxels during polygonisation. By default the optimisation is enabled.

-map <type> <outputName>       The available types are: "NON-EMPTY_VOXELS", "DENSITY", "THICKNESS", "FIRST_PATCH", "LAST_PATCH", "HYPERSPECTRAL_MEAN", "NDVI", "LOWEST_RETURN", "AVERAGE_HEIGHT_DIFFERENCE", "HEIGHT"

-map HYPERSPECTRAL <band> <outputName> The hyperspectral map needs an extra parameter defining which band will be outputed

-map SPECTRAL_SIGNATURE <outputName> -signature <type> <signature_directory>  The spectral signature map gives the square spectral difference between the signature and each pixel. The type is either "ASTER" or "USGS!".

-map <type> -thres <threshold> <outputName>  A threshold is optional and can be added to any type of maps. Always added before the <outputname>

-default                       It reads one of the sample FW LAS files and produces a 3D polygon representation and all the related maps

-defaulth                      It reads one of the sample FW LAS files along with the hyperspectral and produces a coloured 3D polygon representation and all the related metrics



Examples Commands:
$:  ./DASOS -default
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj Polygon -vl 3
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -obj ColouredPolygon -vl 3.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness.png -map NDVI ndvi.png
$:  ./DASOS -las data/LDR-FW-FW10_01-201009821.LAS -vl 2.5 -igm data/e098211b_osgn.igm -bil data/e098211b_masked.bil -fodis data/e098211b_FODIS.bil -map THICKNESS thickness.png -map NDVI ndvi.png -obj happy




