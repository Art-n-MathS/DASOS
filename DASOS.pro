
OBJECTS_DIR=obj/
TARGET=DASOS

CONFIG += console

#CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/MarchingCubes/MarchingCubes.cpp \
    src/MarchingCubes/HashTable.cpp \
    src/Object/VolumeFactory.cpp \
    src/Object/Grid.cpp \
    src/Object/DtmBilReader.cpp \
    src/Object/VolumeHashed1DArray.cpp \
    src/Object/Volume.cpp \
    src/Object/GLData.cpp \
    src/Maps/MapsManager.cpp \
    src/Maps/HyperMap.cpp \
    src/Maps/Map.cpp \
    src/Maps/AverageHeightDifference.cpp \
    src/Maps/IntensityMap.cpp \
    src/Maps/DensityMap.cpp \
    src/Maps/FirstPatch.cpp \
    src/Maps/HeightMap.cpp \
    src/Maps/HyperMean.cpp \
    src/Maps/HyperStandardDeviation.cpp \
    src/Maps/IntensityMax.cpp \
    src/Maps/LastPatch.cpp \
    src/Maps/NDVI.cpp \
    src/Maps/NonEmptyVoxelsMap.cpp \
    src/Maps/SignatureDifferneceMap.cpp \
    src/Maps/SpectralSignature.cpp \
    src/Maps/TerrainModel.cpp \
    src/Maps/ThicknessMap.cpp \
    src/Maps/TreeCrowns.cpp \
    src/Maps/TreeCrownsTemplates.cpp \
    src/Maps/TreeCrownsWith2Templates.cpp \
    src/bilLib/bil.cpp \
    src/bilLib/binaryreader.cpp \
    src/bilLib/binfile.cpp \
    src/bilLib/bsq.cpp \
    src/bilLib/commonfunctions.cpp \
    src/bilLib/logger.cpp \
    src/bilLib/multifile.cpp \
    src/LASHandler/Las1_3_handler.cpp \
    src/LASHandler/PW_handler.cpp \
    src/LASHandler/PW_descriptor.cpp \
    src/LASHandler/PW_lookUpTable.cpp \
    src/LASHandler/PW_scannerVLR.cpp


INCLUDEPATH+=./include/bilLib \
             ./include/LASHandler \
             ./include/Object \
             ./include/MarchingCubes \
             ./include/Maps\
             ./include/svm


INCLUDEPATH += "/users/rsg/mmi/gmtl-0.6.1/build/include/gmtl-0.6.1/"
INCLUDEPATH += "C:\Users\milto\Adobe Flash Builder 4.6\Documents\EngD\DASOS_testing2-master\gmtl-0.6.1"

#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui


QMAKE_CXXFLAGS += -std=c++11



DEPENDPATH+=include



HEADERS += \
    include/MarchingCubes/HashTable.h \
    include/MarchingCubes/MarchingCubes.h \
    include/MarchingCubes/MCTables.h \
    include/Object/VolumeHashed1DArray.h \
    include/Object/DtmBilReader.h \
    include/Object/Volume.h \
    include/Object/Volume1DArray.h \
    include/Object/Grid.h \
    include/Object/GLData.h \
    include/Object/VolumeFactory.h \
    include/Maps/Map.h \
    include/Maps/MapsManager.h \
    include/Maps/AverageHeightDifference.h \
    include/Maps/DensityMap.h \
    include/Maps/FirstPatch.h \
    include/Maps/HeightMap.h \
    include/Maps/HyperMap.h \
    include/Maps/HyperMean.h \
    include/Maps/HyperStandardDeviation.h \
    include/Maps/IntensityMap.h \
    include/Maps/IntensityMax.h \
    include/Maps/LastPatch.h \
    include/Maps/NDVI.h \
    include/Maps/NonEmptyVoxelsMap.h \
    include/Maps/SignatureDifferneceMap.h \
    include/Maps/SpectralSignature.h \
    include/Maps/TerrainModel.h \
    include/Maps/ThicknessMap.h \
    include/Maps/TreeCrowns.h \
    include/Maps/TreeCrownsTemplates.h \
    include/Maps/TreeCrownsWith2Templates.h \
    include/bilLib/bil.h \
    include/bilLib/binaryreader.h \
    include/bilLib/binfile.h \
    include/bilLib/bsq.h \
    include/bilLib/commonfunctions.h \
    include/bilLib/logger.h \
    include/bilLib/multifile.h \
    include/LASHandler/Las1_3_handler.h \
    include/LASHandler/PW_descriptor.h \
    include/LASHandler/PW_handler.h \
    include/LASHandler/LAS1_3Types.h \
    include/LASHandler/PW_scannerVLR.h \
    include/LASHandler/PWTypes.h \
    include/LASHandler/PW_lookUpTable.h
