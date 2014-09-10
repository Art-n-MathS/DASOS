#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-------------------------------------------------------------------------
/// @file MainWindow.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 30/09/13
/// @class MainWindow
/// @brief this class aims to link the GUI with the libraries.
//-------------------------------------------------------------------------


#include <QMainWindow>
#include <ngl/Vec4.h>
#include "GLWindow.h"
#include "GLData.h"
#include "Object.h"
#include "PulseManager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------
    /// @brief defualt constructor
    //-------------------------------------------------------------------------
    explicit MainWindow(QWidget *parent = 0);
    //-------------------------------------------------------------------------
    /// @brief method that handles key press events
    //-------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent * i_event);
    //-------------------------------------------------------------------------
    /// @brief defaulst destructor
    //-------------------------------------------------------------------------
    ~MainWindow();


public slots:
    //-------------------------------------------------------------------------
    /// @brief method that load a LAS file into m_pulseManager
    //-------------------------------------------------------------------------
    void loadLASfile();
    //-------------------------------------------------------------------------
    /// @brief method that creates an implicit object from the LAS data
    //-------------------------------------------------------------------------
    void createObject();
    //-------------------------------------------------------------------------
    /// @brief method that voxelises a given area, polygonise the object using
    /// the Marching cubes algorithm and visualise it in the GL Window.
    //-------------------------------------------------------------------------
    void polygonise();
    //-------------------------------------------------------------------------
    /// @brief method that exports the object generated from voxelise()
    //-------------------------------------------------------------------------
    void exportOBJ();
    //-------------------------------------------------------------------------
    /// @brief method that sets the threshold of the surface of the object
    /// @param[in] i_threshold the threshold
    //-------------------------------------------------------------------------
    void setThreshold(double i_threshold);
    //-------------------------------------------------------------------------
    /// @brief method that close the application
    //-------------------------------------------------------------------------
    void close();
    //-------------------------------------------------------------------------
    /// @brief method that creates the histogram of the frequency intensities
    /// in the voxels
    //-------------------------------------------------------------------------
    void createHist();
    //-------------------------------------------------------------------------
    /// @brief method that load the bil file for the hyperspctral data
    //-------------------------------------------------------------------------
    void loadHyperspectraldata();
    //-------------------------------------------------------------------------
    /// @brief method that updates the 3 bands used and updates the texture
    //-------------------------------------------------------------------------
    void updateHyperspectral();
    //-------------------------------------------------------------------------
    /// @brief method that creates a map and saves it as an image
    //-------------------------------------------------------------------------
    void createMap();
    //-------------------------------------------------------------------------
    /// @brief method that changes the type of data that will be used
    //-------------------------------------------------------------------------
    void changeDataType(int i_type);
    //-------------------------------------------------------------------------
    /// @brief method that changes the shader type either texture from hyper-
    /// spectral data or gold
    //-------------------------------------------------------------------------
    void changeShaderType(bool i_type);
    //-------------------------------------------------------------------------
    /// @brief method that reveals or hides the load IGM File button
    //-------------------------------------------------------------------------
    void hideOrRevealIGMButton(bool i_reveal);
    //-------------------------------------------------------------------------
    /// @brief method that loads an IGM file
    //-------------------------------------------------------------------------
    void loadIGM();



private:
    //-------------------------------------------------------------------------
    /// @brief this method updates the values of m_user_limits
    /// it takes the values shown on the GUI
    //-------------------------------------------------------------------------
    void updateLimits();

    //-------------------------------------------------------------------------
    /// @brief the name/location of the hyperspectraldata
    //-------------------------------------------------------------------------
    std::string m_bilFilename;
    //-------------------------------------------------------------------------
    /// @brief the name/location of the IGM file
    //-------------------------------------------------------------------------
    std::string m_IGMFilename;
    //-------------------------------------------------------------------------
    /// @brief the GUI form
    //-------------------------------------------------------------------------
    Ui::MainWindow *m_ui;
    //-------------------------------------------------------------------------
    /// @brief the openGL widget
    //-------------------------------------------------------------------------
    GLWindow *m_gl;
    //-------------------------------------------------------------------------
    /// @brief the pulse manager where all the info of the LAS file are saved
    //-------------------------------------------------------------------------
    PulseManager *m_pulseManager;
    //-------------------------------------------------------------------------
    /// @brief those limits define the area that the user would like to process
    /// @note before you use them you should always update them by calling the
    /// updateLimits() function.
    /// the limits are ordered as follow:
    /// [MaxNorthY, MinNorthY, MaxEastX, MinEastX, MaxHeightZ, MaxHeightZ]
    //-------------------------------------------------------------------------
    std::vector<double> m_user_limits;
    //-------------------------------------------------------------------------
    /// @brief an implicit object, created from the las data
    //-------------------------------------------------------------------------
    Object *m_obj;
    //-------------------------------------------------------------------------
    /// @brief the vertices, normals and indices that specify an object
    /// the object generated from the voxelise() function
    /// it is equal to 0 if it does not include any object
    //-------------------------------------------------------------------------
    GLData *m_glData;
    //-------------------------------------------------------------------------
    /// @brief what time of data will be used
    /// 0/false for fullwaveform data and 1/true for discrete data
    //-------------------------------------------------------------------------
    int m_type;


};

#endif // MAINWINDOW_H
