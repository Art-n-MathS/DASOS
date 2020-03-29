:: go to directory
cd C:\DASOS_userGuide\DASOS_win\DASOS

pause

:: help 
DASOS --help
:: extract sample amplitudes for deciding noise level
DASOS -las ..\SampleDATA\DATASET_1\LDR-FW-FW10_01-201009821.LAS -exportPulses 12 ..\LAS21pulsesSamples.csv
 
DASOS -las ..\SampleDATA\DATASET_2\Australia_2.las -exportPulses 12 ..\Australia_2_pulsesSamples.csv
 
:: export sample metric and visualise it in QGIS
DASOS -las ..\SampleDATA\DATASET_2\Australia_2.las -nl 6 -vl 2 -map height ..\Australia_2_vl2_height

DASOS -las ..\SampleDATA\DATASET_2\Australia_3.las -nl 6 -vl 2 -map height ..\Australia_3_vl2_height

:: import:: extract sample amplitudes for deciding noise level
DASOS -las ..\SampleDATA\DATASET_1\LDR-FW-FW10_01-201009821.LAS -exportPulses 12 ..\LAS21pulsesSamples.csv

DASOS -las ..\SampleDATA\DATASET_2\Australia_2.las -exportPulses 12 ..\Australia_2_pulsesSamples.csv

:: export sample metric and visualise it in QGIS
 two files and export a volumetric file
DASOS -las ..\SampleDATA\DATASET_2\Australia_3.las ..\SampleDATA\DATASET_2\Australia_2.las -nl 6 -vl 2 -iso 4 -userLimits 6199990 6199639 762405 761951 -exportVolume c ..\Australia2-3.vol

:: import volume and export metrics
DASOS -volume ..\Australia2-3.vol -map height ..\Australia2-3 -map all_fw ..\Australia2-3

:: introducing dtm
DASOS -las ..\SampleDATA\DATASET_2\Australia_3.las ..\SampleDATA\DATASET_2\Australia_2.las -dtm ..\SampleDATA\DATASET_2\Australia_2_dtm.bil -nl 6 -vl 2 -iso 4 -userLimits 6199990 6199639 762405 761951 -exportVolume c ..\Australia2-3_dtm.vol -map height ..\Australia2-3_vl2_dtm_height

:: use the volume again to create something interesting
DASOS -volume ..\Australia2-3_dtm.vol -map AVERAGE_HEIGHT_DIFFERENCE ..\Australia2-3_dtm_AVG_height_diff

:: pulsewaves data
DASOS -pw ..\SampleDATA\DATASET_2\Australia_1.pls -exportPulses 15 ..\PLS_amplitudeSamples.csv 

DASOS -pw ..\SampleDATA\DATASET_2\Australia_1.pls -nl 5 -dtm ..\SampleDATA\DATASET_2\Australia_1_DTM_1m.bil 5 -vl 3 -map thickness ..\PLS_vl3_thickness -exportVolume ..\Australia_1_vl3_dtm.vol

:: Exporting Polygon representation but there is a bug and a bunch of cone may be created instead (an exception was thrown last time)
DASOS -las ..\SampleDATA\DATASET_1\LDR-FW-FW10_01-201009821.LAS -nl 20 -vl 1.7 -obj ..\LAS21.obj -exportVolume c ..\LAS21_vl1.7.vol

:: Hyperspectral Imagery 
:: Limitation: don't use ..\ because the output polygon won't be able to match the texture with the object. It is suggested to not include directory as well and then the file will be autotimatically saved into the same directory as the executable file of DASOS. 
DASOS -volume ..\LAS21_vl1.7.vol -bil ..\SampleDATA\DATASET_1\e098211b_masked.bil -igm  ..\SampleDATA\DATASET_1\e098211b_osgn.igm -fodis ..\SampleDATA\DATASET_1\e098211b_FODIS.bil -rgb 240 78 23 -obj LAS21_coloured.obj

:: Hyperspectral Metrics
DASOS -volume ..\LAS21_vl1.7.vol -bil ..\SampleDATA\DATASET_1\e098211b_masked.bil -igm  ..\SampleDATA\DATASET_1\e098211b_osgn.igm -fodis ..\SampleDATA\DATASET_1\e098211b_FODIS.bil -map hyperspectral 140 ..\LAS21_band140 -map height ..\LAS21_height -map NDVI ..\LAS21_ndvi 

:: All commands together



:: Exporting 3D priors
DASOS -vols ..\SampleDATA\DATASET_3  -icsv ..\SampleDATA\DATASET_3\testFieldplot.csv -eparameters processed -column isDead -class dead -ttype square 3 3 5 -ocsv templatesProcessedCuboid

DASOS -vols ..\SampleDATA\DATASET_3  -icsv ..\SampleDATA\DATASET_3\testFieldplot.csv -eparameters raw -column isDead -class ALL -ttype cylinder 5 3 -ocsv templatesALLCylinder

pause
