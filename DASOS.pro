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
    src/LASHandler/Las1_3_handler.cpp \
    src/Object/GLData.cpp \
    src/Object/Grid.cpp \
    src/MarchingCubes/HashTable.cpp \
    src/MarchingCubes/MarchingCubes.cpp \
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
    src/Maps/TreeCrowns.cpp \
    src/Maps/TreeCrownsTemplates.cpp \
    src/Maps/TreeCrownsWith2Templates.cpp \
    src/Maps/IntensityMap.cpp \
    src/Maps/IntensityMax.cpp \
    src/Maps/FieldPlotOnHeightMap.cpp \
    src/fieldPlots/NewForest/TreeDetectionEvaluation.cpp \
    src/fieldPlots/NewForest/CreateTreeTemplates.cpp \
    src/fieldPlots/NewForest/TemplatesManager.cpp \
    src/fieldPlots/NewForest/NF_FieldPlot.cpp \
    src/fieldPlots/NewForest/NF_FieldPlotManagerByType.cpp \
    src/fieldPlots/NewForest/NF_FieldPlotsManager.cpp \
    src/LASHandler/PW_handler.cpp \
    src/Object/Volume.cpp \
    src/Object/VolumeFactory.cpp \
    src/LASHandler/PW_descriptor.cpp \
    src/LASHandler/PW_scannerVLR.cpp \
    src/LASHandler/PW_lookUpTable.cpp \
    src/Object/VolumeHashed1DArray.cpp \
    src/fieldPlots/Interpine/Int_Plot.cpp \
    src/fieldPlots/Interpine/Int_PlotsManager.cpp \
    src/Object/DtmBilReader.cpp \
    src/Object/PairDoubleInt.cpp


INCLUDEPATH+=./include/bilLib \
             ./include/LASHandler \
             ./include/Object \
             ./include/Object/Octree \
             ./include/MarchingCubes \
             ./include/Maps\
             ./include/svm \
             ./include/fieldPlots/NewForest \
             ./include/fieldPlots/Interpine \
             ./include/fieldPlots


INCLUDEPATH += "/users/rsg/mmi/gmtl-0.6.1/build/include/gmtl-0.6.1/"
#INCLUDEPATH += "C:\Users\milto\Adobe Flash Builder 4.6\Documents\EngD\DASOS_testing2-master\gmtl-0.6.1"

INCLUDEPATH += "C:\Users\Milto\Documents\gmtl-master"

INCLUDEPATH += "/home/milto/gmtl-master"
#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui

#QMAKE_LFLAGS += -static -static-libgcc -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc

QMAKE_CXXFLAGS += -std=c++11



DEPENDPATH+=include



HEADERS += \
    include/bilLib/bil.h \
    include/bilLib/binaryreader.h \
    include/bilLib/binfile.h \
    include/bilLib/bsq.h \
    include/bilLib/commonfunctions.h \
    include/bilLib/logger.h \
    include/LASHandler/Las1_3_handler.h \
    include/Object/GLData.h \
    include/Object/Grid.h \
    include/MarchingCubes/HashTable.h \
    include/MarchingCubes/MarchingCubes.h \
    include/MarchingCubes/MCTables.h \
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
    include/Maps/TreeCrowns.h \
    include/Maps/TreeCrownsTemplates.h \
    include/Maps/TreeCrownsWith2Templates.h \
    include/Maps/IntensityMap.h \
    include/Maps/IntensityMax.h \
    include/Maps/FieldPlotOnHeightMap.h \
    include/fieldPlots/NewForest/TreeDetectionEvaluation.h \
    include/fieldPlots/NewForest/CreateTreeTemplates.h \
    include/LASHandler/PWTypes.h \
    include/LASHandler/PW_handler.h \
    include/LASHandler/LAS1_3Types.h \
    include/Object/Volume.h \
    include/Object/VolumeFactory.h \
    include/LASHandler/PW_descriptor.h \
    include/LASHandler/PW_scannerVLR.h \
    include/LASHandler/PW_lookUpTable.h \
    include/Object/VolumeHashed1DArray.h \
    include/fieldPlots/NewForest/NF_FieldPlot.h \
    include/fieldPlots/NewForest/NF_FieldPlotManagerByType.h \
    include/fieldPlots/NewForest/NF_FieldPlotsManager.h \
    include/fieldPlots/NewForest/TemplatesManager.h \
    include/fieldPlots/Interpine/Int_Plot.h \
    include/fieldPlots/Interpine/Int_PlotsManager.h \
    include/Object/DtmBilReader.h \
    include/Object/PairDoubleInt.h

