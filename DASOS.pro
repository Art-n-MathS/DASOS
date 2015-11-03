
OBJECTS_DIR=obj/
TARGET=DASOS

CONFIG += console

CONFIG -= app_bundle

SOURCES += \
    src/main.cpp \
    src/bilLib/bil.cpp \
    src/bilLib/binaryreader.cpp \
    src/bilLib/binfile.cpp \
    src/bilLib/bsq.cpp \
    src/bilLib/commonfunctions.cpp \
    src/bilLib/logger.cpp \
    src/LASHandler/DiscreteData.cpp \
    src/LASHandler/Las1_3_handler.cpp \
    src/Object/IntegralVolumes.cpp \
    src/Object/Object.cpp \
    src/Object/GLData.cpp \
    src/Object/Grid.cpp \
    src/MarchingCubes/HashTable.cpp \
    src/MarchingCubes/MarchingCubes.cpp \
    src/MarchingCubes/MCwithIntegralImages.cpp \
    src/Maps/DensityMap.cpp \
    src/Maps/HyperMap.cpp \
    src/Maps/HyperMean.cpp \
    src/Maps/HyperStandardDeviation.cpp \
    src/Maps/Map.cpp \
    src/Maps/MapsManager.cpp \
    src/Maps/NonEmptyVoxelsMap.cpp \
    src/Maps/ThicknessMap.cpp \
    src/Maps/FirstPatch.cpp \
    src/Maps/LastPatch.cpp \
    src/Maps/NDVI.cpp \
    src/Maps/TerrainModel.cpp \
    src/Maps/SpectralSignature.cpp \
    src/Maps/SignatureDifferneceMap.cpp \
    src/Maps/AverageHeightDifference.cpp \
    src/Maps/HeightMap.cpp \
    src/bilLib/multifile.cpp \
    src/Maps/IntensityMap.cpp \
    src/Maps/IntensityMax.cpp


INCLUDEPATH+=./include/bilLib \
             ./include/LASHandler \
             ./include/Object \
             ./include/MarchingCubes \
             ./include/Maps


INCLUDEPATH += "/users/rsg/mmi/gmtl-0.6.1/build/include/gmtl-0.6.1/"

#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui


QMAKE_CXXFLAGS += -std=c++11

#CONFIG += c++11

DEPENDPATH+=include



HEADERS += \
    include/bilLib/bil.h \
    include/bilLib/binaryreader.h \
    include/bilLib/binfile.h \
    include/bilLib/bsq.h \
    include/bilLib/commonfunctions.h \
    include/bilLib/logger.h \
    include/LASHandler/DiscreteData.h \
    include/LASHandler/Las1_3_handler.h \
    include/LASHandler/Types.h \
    include/Object/IntegralVolumes.h \
    include/Object/Object.h \
    include/Object/GLData.h \
    include/Object/Grid.h \
    include/MarchingCubes/HashTable.h \
    include/MarchingCubes/MarchingCubes.h \
    include/MarchingCubes/MCTables.h \
    include/MarchingCubes/MCwithIntegralImages.h \
    include/Maps/DensityMap.h \
    include/Maps/HyperMap.h \
    include/Maps/HyperMean.h \
    include/Maps/HyperStandardDeviation.h \
    include/Maps/Map.h \
    include/Maps/MapsManager.h \
    include/Maps/NonEmptyVoxelsMap.h \
    include/Maps/ThicknessMap.h \
    include/Maps/FirstPatch.h \
    include/Maps/LastPatch.h \
    include/Maps/NDVI.h \
    include/Maps/TerrainModel.h \
    include/Maps/SpectralSignature.h \
    include/Maps/SignatureDifferneceMap.h \
    include/Maps/AverageHeightDifference.h \
    include/Maps/HeightMap.h \
    include/bilLib/multifile.h \
    include/Maps/IntensityMap.h \
    include/Maps/IntensityMax.h
