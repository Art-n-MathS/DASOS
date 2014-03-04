#include "GLWindow.h"
#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "MarchingCubes.h"
#include <sstream>

/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;
//----------------------------------------------------------------------------------------------------------------------
// in this ctor we need to call the CreateCoreGLContext class, this is mainly for the MacOS Lion version as
// we need to init the OpenGL 3.2 sub-system which is different than other platforms
//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
                   QWidget *_parent
                  )
                    : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent ),
                      m_object(0),m_vao(0),m_zoom(1.0),m_material(ngl::GOLD), m_shaderMode(0), m_currentTextureName(0)
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
  glClearColor(0.4f, 0.4f, 0.4f, 1.4f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);

  ngl::NGLInit *Init = ngl::NGLInit::instance();
  Init->initGlew();

  #ifdef WIN32
    glewInit(); // need a local glew init as well as lib one for windows
  #endif
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->createShaderProgram("Phong");
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  shader->bindAttribute("Phong",0,"inVert");
  shader->bindAttribute("Phong",1,"inUV");
  shader->bindAttribute("Phong",2,"inNormal");

  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();

  m_material.loadToShader("material");
  ngl::Vec3 from(0,1,m_zoom);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
  m_cam->setShape(45,(float)720.0/576.0,0.05,350,ngl::PERSPECTIVE);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  m_light->setTransform(iv);
  m_light->loadToShader("light");


  // creating texture shader for hyperspectral images
  shader->createShaderProgram("TextureShader");
  shader->attachShader("SimpleVertex",ngl::VERTEX);
  shader->attachShader("SimpleFragment",ngl::FRAGMENT);
  shader->loadShaderSource("SimpleVertex","shaders/Vertex.vs");
  shader->loadShaderSource("SimpleFragment","shaders/Fragment.fs");
  shader->compileShader("SimpleVertex");
  shader->compileShader("SimpleFragment");
  shader->attachShaderToProgram("TextureShader","SimpleVertex");
  shader->attachShaderToProgram("TextureShader","SimpleFragment");
  shader->bindAttribute("TextureShader",0,"inVert");
  shader->bindAttribute("TextureShader",1,"inUV");
  shader->bindAttribute("TextureShader",2,"inNormal");
  shader->linkProgramObject("TextureShader");
  (*shader)["TextureShader"]->use();

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::loadHyperspectral(const std::string i_filename, const std::vector<unsigned short int> &i_bands)
{
    if(m_currentTextureName!=0)
    {
       glDeleteTextures(1, &m_currentTextureName);
    }
    m_shaderMode = 1;
    m_texture.loadImage(i_filename,i_bands);
    m_currentTextureName = m_texture.setTextureGL();
    glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  // set the viewport for openGL
  glViewport(0,0,_w,_h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350,ngl::PERSPECTIVE);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::loadMatricesToShader(
                                     ngl::TransformStack &_tx
                                   )
{

   ngl::ShaderLib *shader=ngl::ShaderLib::instance();

   if(!m_shaderMode)
   {
      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      M=_tx.getCurrAndGlobal().getMatrix();
      MV=  _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
      MVP= M*m_cam->getVPMatrix();
      normalMatrix=MV;
      normalMatrix.inverse();
      shader->setShaderParamFromMat4("MV",MV);
      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
      shader->setShaderParamFromMat4("M",M);
   }
   else
   {
       ngl::Mat4 MVP;
       MVP=m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getVPMatrix();
       shader->setShaderParamFromMat4("MVP",MVP);
   }
}


//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  if(m_shaderMode)
  {
     (*shader)["TextureShader"]->use();
  }
  else
  {
     (*shader)["Phong"]->use();
  }

  // Rotation based on the mouse position for our global transform
  ngl::Transformation trans;
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  ngl::Mat4 final;
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = m_modelPos.m_z;
  final.m_m[3][3] = 1.0;
  final=final*rotY;
  final=final*rotX;
  ngl::Mat4 scale;
  scale.m_m[0][0] = 0.001;
  scale.m_m[1][1] = 0.001;
  scale.m_m[2][2] = 0.001;
  scale.m_m[3][3] = 1.0;


  final = final * scale;
  // add the translations


  // set this in the TX stack
  trans.setMatrix(final);

  m_transformStack.setGlobal(trans);

  ngl::Vec4 camPosition(m_cam->getEye());
  camPosition.m_z = 1/m_zoom/2;
  m_cam->setEye(camPosition);

  loadMatricesToShader(m_transformStack);
  draw();
}

struct vertData
{
   GLfloat u;
   GLfloat v;
   GLfloat nx;
   GLfloat ny;
   GLfloat nz;
   GLfloat x;
   GLfloat y;
   GLfloat z;
};

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::buildVAO(const GLData *i_glData)
{
    if (m_vao!=0)
    {
        m_vao->unbind();
        m_vao->removeVOA();
    }
    const std::vector <GLfloat> &vertices = i_glData->m_vertices;
    const std::vector <GLuint> &indices = i_glData->m_indices;
    const std::vector <GLfloat> &normals = i_glData->m_normals;
    const std::vector <GLfloat> &uvs = i_glData->m_UVs;
    std::cout<< "num of vertices = " << vertices.size() << "\n";
    std::cout<< "num of indices = " << indices.size() << "\n";


    // move the object into the middle of the GLWindow.
    ngl::Vec3 translation = i_glData->m_maxLimits + i_glData->m_minLimits;
    m_modelPos = translation/(-2);

    m_vao = ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
    m_vao->bind();
    m_vao->setIndexedData(sizeof(GLfloat)*vertices.size(),vertices[0],indices.size(),&indices[0],GL_UNSIGNED_INT);
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->setIndexedData(normals.size()*sizeof(GLfloat),normals[0],indices.size(),&indices[0],GL_UNSIGNED_INT);
    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);
    m_vao->setIndexedData(sizeof(GLfloat*)*uvs.size(),uvs[0],indices.size(), &indices[0],GL_UNSIGNED_INT);
    m_vao->setVertexAttributePointer(1,2,GL_FLOAT,0,0);
    m_vao->setNumIndices(indices.size());
    m_vao->unbind();

    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::draw()
{
   if(m_vao!=0)
   {
      m_vao->bind();
      m_vao->draw();
      m_vao->unbind();
   }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
    // note the method buttons() is the button state when event was called
    // this is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
      int diffx=_event->x()-m_origX;
      int diffy=_event->y()-m_origY;
      m_spinXFace += (float) 0.5f * diffy;
      m_spinYFace += (float) 0.5f * diffx;
      m_origX = _event->x();
      m_origY = _event->y();
      updateGL();

    }
      // right mouse translate code
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
          int diffX = (int)(_event->x() - m_origXPos);
          int diffY = (int)(_event->y() - m_origYPos);
          m_origXPos=_event->x();
      m_origYPos=_event->y();
          m_modelPos.m_x += INCREMENT * diffX;
          m_modelPos.m_y -= INCREMENT * diffY;
          updateGL();

      }
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
    // right mouse translate mode
    else if(_event->button() == Qt::RightButton)
    {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
        m_translate=true;
    }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
    // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

    // check the diff of the wheel position (0 means no change)
    if(_event->delta() > 0)
    {
        m_zoom+=ZOOM;
    }
    else if(_event->delta() <0 )
    {
        m_zoom-=ZOOM;
    }
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
    if (m_vao!=0)
    {
        m_vao->unbind();
        m_vao->removeVOA();
    }
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  Init->NGLQuit();
  if(m_object!=0)
  {
      delete m_object;
  }
  if(m_currentTextureName!=0)
  {
     glDeleteTextures(1, &m_currentTextureName);
  }
}
