#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include <ngl/TransformStack.h>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/Texture.h>
#include "GLWindow2.h"
#include <ngl/Material.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

//----------------------------------------------------------------------------------------------------------------------
GLWindow2::GLWindow2(
                   QWidget *_parent
                  )   :
    QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent ),
    m_vaoSphere(0), m_light(0), m_glData(NULL)

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
    m_origX=0;
    m_origY=0;

}

GLWindow2::~GLWindow2()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VBO's and Shaders\n";
  Init->NGLQuit();
    // now we have finished remove the VOA
    m_vaoSphere->removeVOA();
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow2::initializeGL()
{
    ngl::NGLInit *Init = ngl::NGLInit::instance();
    #ifdef WIN32
        glewInit();
    #endif
    Init->initGlew();

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
  ngl::Vec3 from(0,1,12);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

    m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.001,150,ngl::PERSPECTIVE);
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    // load a frag and vert shaders
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
    // build our VertexArrayObject
//    buildVAOSphere();
    // load and set a texture
    ngl::Texture t("textures/earth.png");

    ngl::Material material(ngl::GOLD);
    material.loadToShader("material");

    ngl::Mat4 iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
    m_light->setTransform(iv);
    m_light->loadToShader("light");


//	glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow2::resizeGL(
                        int _w,
                        int _h
                       )
{

  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,0.001,350,ngl::PERSPECTIVE);
}

// a simple structure to hold our vertex data
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


void GLWindow2::buildVAOSphere(GLData *i_glData)
{

    if (m_vaoSphere!=0)
    {
        m_vaoSphere->unbind();
        m_vaoSphere->removeVOA();
    }

    m_vaoSphere= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
    m_vaoSphere->bind();

    m_glData = i_glData;

    ngl::Vec3 offset;
    if(m_glData!=NULL)
    {
       if(m_glData->m_vertices.size()>=3)
       {
          offset.m_x = (m_glData->m_vertices[0]+m_glData->m_vertices[m_glData->m_vertices.size()-3]) / 2;
          offset.m_y = (m_glData->m_vertices[1]+m_glData->m_vertices[m_glData->m_vertices.size()-2]) / 2;
          offset.m_z = (m_glData->m_vertices[2]+m_glData->m_vertices[m_glData->m_vertices.size()-1]) / 2;
       }
    }

   std::vector<GLfloat> tvert;
   tvert.resize(m_glData->m_vertices.size());
   for(unsigned int i=0; i<tvert.size(); i+=3)
   {
      tvert[i  ] = m_glData->m_vertices[i  ] - offset.m_x;
      tvert[i+1] = m_glData->m_vertices[i+1] - offset.m_y;
      tvert[i+2] = m_glData->m_vertices[i+2] - offset.m_z;
   }


     m_vaoSphere->setIndexedData(tvert.size()*sizeof(GLfloat),tvert[0],sizeof(GLuint)*i_glData->m_indices.size()/3,&i_glData->m_indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);

     m_vaoSphere->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

     m_vaoSphere->setIndexedData(i_glData->m_normals.size()*sizeof(GLfloat),i_glData->m_normals[0],sizeof(GLuint)*i_glData->m_indices.size()/3,&i_glData->m_indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
     m_vaoSphere->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

     m_vaoSphere->setNumIndices(sizeof(GLuint)*i_glData->m_indices.size()/3);

    m_vaoSphere->unbind();
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow2::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 // build our transform stack
  ngl::Transformation trans;
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  ngl::Mat4 final=rotY*rotX;
  // add the translations
  ngl::Vec3 offset;
  if(m_glData!=NULL)
  {
     if(m_glData->m_vertices.size()>=3)
     {
        offset.m_x = (m_glData->m_vertices[0]+m_glData->m_vertices[m_glData->m_vertices.size()-3]) / 2;
        offset.m_y = (m_glData->m_vertices[1]+m_glData->m_vertices[m_glData->m_vertices.size()-2]) / 2;
        offset.m_z = (m_glData->m_vertices[2]+m_glData->m_vertices[m_glData->m_vertices.size()-1]) / 2;
     }
  }
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = m_modelPos.m_z;
  ngl::Mat4 scale;
  scale.m_m[0][0] = 0.005;
  scale.m_m[1][1] = 0.005;
  scale.m_m[2][2] = 0.005;
  scale.m_m[3][3] = 1.0;


  final = final * scale;
  // set this in the TX stack
  trans.setMatrix(final);
  m_transformStack.setGlobal(trans);


  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();


  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transformStack.getCurrAndGlobal().getMatrix();
  MV=  m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);

  if(m_vaoSphere!=0)
  {
    // now we bind back our vertex array object and draw
    m_vaoSphere->bind();
    m_vaoSphere->draw();
    // now we are done so unbind
    m_vaoSphere->unbind();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow2::mouseMoveEvent (
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
void GLWindow2::mousePressEvent (
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
void GLWindow2::mouseReleaseEvent (
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
void GLWindow2::wheelEvent(QWheelEvent *_event)
{
    if (m_vaoSphere!=0)
    {
        m_vaoSphere->unbind();
        m_vaoSphere->removeVOA();
    }
    if(m_light!=0)
    {
       delete m_light;
    }
  // check the diff of the wheel position (0 means no change)
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=ZOOM;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=ZOOM;
  }
  updateGL();
}
