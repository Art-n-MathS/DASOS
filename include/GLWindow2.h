#ifndef __GL_WINDOW2_H__
#define __GL_WINDOW2_H__


#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include "ngl/TransformStack.h"
#include <ngl/VertexArrayObject.h>
#include <QKeyEvent>
#include <ngl/Material.h>
#include <ngl/Light.h>
#include "GLData.h"

class GLWindow2 : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :
  /// @brief Constructor for GLWindow
  /// @param [in] parent the parent window to create the GL context in
  GLWindow2(
           QWidget *_parent
          );

  void buildVAOSphere(const GLData *i_glData);

  ~GLWindow2();
private :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the x rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinXFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the y rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinYFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the Right mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_translate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origY;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origXPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origYPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  /// @brief Our Camera
  ngl::Camera *m_cam;
  /// @brief our transform stack for drawing the teapots
  ngl::TransformStack m_transformStack;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a simple light use to illuminate the screen
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Light *m_light;



protected:

  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  /// @note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called resizeGL )
  void resizeGL(
                const int _w,
                const int _h
               );
  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  /// be re-drawn
  void paintGL();

private :

        ngl::VertexArrayObject *m_vaoSphere;
        ngl::Material m_material;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (
                          QMouseEvent * _event
                        );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent (
                            QMouseEvent *_event
                         );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent (
                            QMouseEvent *_event
                            );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(
                      QWheelEvent *_event
                   );


};

#endif
