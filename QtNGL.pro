QT+=opengl \
      gui \
      core
INCLUDEPATH+=include \
                +=ui

INCLUDEPATH+=./include \
             ./include/Objects \
             ./include/MarchingCubes \
             ./include/LASHandler \
             ./include/Maps


OBJECTS_DIR=obj/
MOC_DIR=moc/
TARGET=Las1.3Vis

CONFIG += console

CONFIG -= app_bundle
UI_HEADERS_DIR=ui
SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/GLWindow.cpp \
    src/MarchingCubes/GLData.cpp \
    src/MarchingCubes/HashTable.cpp \
    src/MarchingCubes/MarchingCubes.cpp \
    src/MarchingCubes/Object.cpp \
    src/Manager.cpp \
    src/MarchingCubes/Histogram.cpp \
    src/Maps/MapsManager.cpp \
    src/Maps/Map.cpp \
    src/Maps/DensityMap.cpp \
    src/Texture.cpp \
    src/LASHandler/DiscreteData.cpp \
    src/Camera.cpp \
    src/MarchingCubes/IntegralVolumes.cpp \
    src/MarchingCubes/MCwithIntegralImages.cpp \
    src/MarchingCubes/Grid.cpp \
    src/Maps/NonEmptyVoxelsMap.cpp \
    src/Maps/ThicknessMap.cpp \
    src/Maps/HyperMap.cpp \
    src/MarchingCubes/IntegralVolumeBox.cpp \
    src/Maps/HyperMean.cpp \
    src/Maps/HyperStandardDeviation.cpp

SOURCES += \
    src/LASHandler/Las1_3_handler.cpp

HEADERS += \
    include/MainWindow.h \
    include/GLWindow.h \
    include/LASHandler/Types.h \
    include/LASHandler/Las1_3_handler.h \
    include/Manager.h \
    include/MarchingCubes/GLData.h \
    include/MarchingCubes/HashTable.h \
    include/MarchingCubes/MarchingCubes.h \
    include/MarchingCubes/MCTables.h \
    include/MarchingCubes/Object.h \
    include/MarchingCubes/Histogram.h \
    include/Maps/MapsManager.h \
    include/Maps/Map.h \
    include/Maps/DensityMap.h \
    include/Texture.h \
    include/LASHandler/DiscreteData.h \
    include/Camera.h \
    include/MarchingCubes/IntegralVolumes.h \
    include/MarchingCubes/MCwithIntegralImages.h \
    include/MarchingCubes/Grid.h \
    include/Maps/NonEmptyVoxelsMap.h \
    include/Maps/ThicknessMap.h \
    include/Maps/HyperMap.h \
    include/MarchingCubes/IntegralVolumeBox.h \
    include/Maps/HyperMean.h \
    include/Maps/HyperStandardDeviation.h

FORMS += \
    ui/MainWindow.ui

OTHER_FILES+= shaders/Phong.fs \
              shaders/Phong.vs \
    shaders/PhongFragment.glsl \
    shaders/PhongVertex.glsl \
    shaders/Vertex.vs \
    shaders/Fragment.fs

INCLUDEPATH +=    "/usr/include/boost/" \
                  "/users/rsg/mmi/gmtl-0.6.1/build/include/gmtl-0.6.1/"


QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG



LIBS +=  -L/$(HOME)/BilLibrary/lib -lBilLibrary
LIBS +=  -L/$(HOME)/NGL/lib -lNGL


# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-g++ {
    DEFINES += LINUX
    LIBS+= -lGLEW
}

QMAKE_CXXFLAGS += -std=gnu++0x
QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_CXXFLAGS += -Wno-unknown-pragmas

CONFIG += c++11

DEPENDPATH+=include
DEPENDPATH+= $HOME/NGL/lib
DEPENDPATH+= $HOME/BilLibrary/lib

# if we are on a mac define DARWIN
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH += $$(HOME)/BilLibrary/include/

