#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include <ngl/TransformStack.h>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/Texture.h>
#include "GLWindow1.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1;

//----------------------------------------------------------------------------------------------------------------------
GLWindow1::GLWindow1(
                   QWidget *_parent
                  )   : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent ),
    m_vao(NULL),m_object(0),m_zoom(1.0),m_material(ngl::GOLD), m_shaderMode(0), m_currentTextureName(0),m_vaoSphere(NULL)

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

GLWindow1::~GLWindow1()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VBO's and Shaders\n";
  Init->NGLQuit();
    // now we have finished remove the VOA
  if (m_vaoSphere!=NULL)
  {
    m_vaoSphere->removeVOA();
  }
    if (m_vao!=NULL)
    {
        m_vao->unbind();
        m_vao->removeVOA();
    }
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  if(m_object!=0)
  {
      delete m_object;
  }
  if(m_currentTextureName!=0)
  {
     glDeleteTextures(1, &m_currentTextureName);
  }
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow1::initializeGL()
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
    ngl::Vec3 from(0,1,2);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);


    m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.001,150,ngl::PERSPECTIVE);

    // now to load the shader and set the values
    // grab an instance of shader manager
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
    // build our VertexArrayObject
//    buildVAOSphere(new GLData);
    // load and set a texture
    ngl::Texture t("textures/earth.png");


    t.setTextureGL();
//	glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow1::buildVAO(/*const*/ GLData *i_glData)
{
    if (m_vao!=0)
    {
        m_vao->unbind();
        m_vao->removeVOA();
    }

//    i_glData->m_vertices.push_back(438272);
//    i_glData->m_vertices.push_back(105049);
//    i_glData->m_vertices.push_back(393.1);

//    i_glData->m_vertices.push_back(437701);
//    i_glData->m_vertices.push_back(103358);
//    i_glData->m_vertices.push_back(-54.767);

//    i_glData->m_vertices.push_back(438272);
//    i_glData->m_vertices.push_back(105049);
//    i_glData->m_vertices.push_back(-54.767);

//    i_glData->m_indices.push_back((i_glData->m_vertices.size()/3)-3);
//    i_glData->m_indices.push_back((i_glData->m_vertices.size()/3)-2);
//    i_glData->m_indices.push_back((i_glData->m_vertices.size()/3)-1);

//    const std::vector <GLfloat> &vertices = i_glData->m_vertices;
//    const std::vector <GLuint> &indices = i_glData->m_indices;
//    const std::vector <GLfloat> &normals = i_glData->m_normals;
//    const std::vector <GLfloat> &uvs = i_glData->m_UVs;
//    std::cout<< "num of vertices = " << vertices.size() << "\n";
//    std::cout<< "num of indices = " << indices.size() << "\n";




//    std::cout << i_glData->m_maxLimits.m_x << " " << i_glData->m_maxLimits.m_y << " " << i_glData->m_maxLimits.m_z << "    ****\n";
//    std::cout << i_glData->m_minLimits.m_x << " " << i_glData->m_minLimits.m_y << " " << i_glData->m_minLimits.m_z << "    ****\n";


//    // move the object into the middle of the GLWindow.
////    ngl::Vec3 translation = (i_glData->m_maxLimits + i_glData->m_minLimits)/2;
////    std::cout << translation.m_x << " " << translation.m_y << " " << translation.m_z << "    ++++\n";
////    m_cam->setEye(ngl::Vec4(translation.m_x, translation.m_y, translation.m_z , 1.0f));
////    m_modelPos = translation/(-2);

//    m_vao = ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
//    m_vao->bind();
//    m_vao->setIndexedData(sizeof(GLfloat)*vertices.size(),vertices[0],indices.size(),&indices[0],GL_UNSIGNED_INT);
//    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
//    m_vao->setIndexedData(normals.size()*sizeof(GLfloat),normals[0],indices.size(),&indices[0],GL_UNSIGNED_INT);
//    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);
//    m_vao->setIndexedData(sizeof(GLfloat*)*uvs.size(),uvs[0],indices.size(), &indices[0],GL_UNSIGNED_INT);
//    m_vao->setVertexAttributePointer(1,2,GL_FLOAT,0,0);
//    m_vao->setNumIndices(indices.size());
//    m_vao->unbind();



    // create a vao as a series of GL_TRIANGLES
    m_vao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
      m_vao->bind();


    const static GLubyte indices[]=  {
                                        0,1,5,0,4,5, // back
                                        3,2,6,7,6,3, // front
                                        0,1,2,3,2,0, // top
                                        4,5,6,7,6,4, // bottom
                                        0,3,4,4,7,3,
                                        1,5,2,2,6,5
                                     };

     GLfloat vertices[] = {-1,1,-1,
                           1,1,-1,
                           1,1,1,
                           -1,1,1,
                           -1,-1,-1,
                           1,-1,-1,
                           1,-1,1,
                           -1,-1,1
                          };

     GLfloat colours[]={
                          1,0,0,
                          0,1,0,
                          0,0,1,
                          1,1,1,
                          0,0,1,
                          0,1,0,
                          1,0,0,
                          1,1,1
                        };
       // in this case we are going to set our data as the vertices above

     m_vao->setIndexedData(24*sizeof(GLfloat),vertices[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
     // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

     m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

     m_vao->setIndexedData(24*sizeof(GLfloat),colours[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
     // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

     m_vao->setVertexAttributePointer(1,3,GL_FLOAT,0,0);



     m_vao->setNumIndices(sizeof(indices));



   // now unbind
      m_vao->unbind();

      std::cout << "* Object Build\n";
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow1::loadHyperspectral(const std::string i_filename, const std::vector<unsigned short int> &i_bands)
{
    if(m_currentTextureName!=0)
    {
       glDeleteTextures(1, &m_currentTextureName);
    }

    m_texture.loadImage(i_filename,i_bands);
    m_currentTextureName = m_texture.setTextureGL();
    glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow1::resizeGL(
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

void GLWindow1::buildVAOSphere(GLData *data)
{
    if(m_vaoSphere!=NULL)
    {
       m_vaoSphere->unbind();
       m_vaoSphere->removeVOA();
    }

    //  Sphere code based on a function Written by Paul Bourke.
    //  http://astronomy.swin.edu.au/~pbourke/opengl/sphere/
    // first we grab an instance of our VOA class as a TRIANGLE_STRIP
  m_vaoSphere= ngl::VertexArrayObject::createVOA(GL_TRIANGLE_STRIP);
    // next we bind it so it's active for setting data
    m_vaoSphere->bind();
    // the next part of the code calculates the P,N,UV of the sphere for tri_strips
    int buffSize;

    float theta1 = 0.0;
    float theta2 = 0.0;
    float theta3 = 0.0;


    float radius=1.0;
  float precision=40;
    // a std::vector to store our verts, remember vector packs contiguously so we can use it
  buffSize = (precision/2) * ((precision+1)*2);

  std::vector <vertData> data1(buffSize);
    // calculate how big our buffer is
    // Disallow a negative number for radius.
    if( radius < 0 )
    {
    radius = -radius;
    }
    // Disallow a negative number for _precision.
    if( precision < 4 )
    {
    precision = 4;
    }
    // now fill in a vertData structure and add to the data list for our sphere
    vertData d;
  unsigned int index=0;
    for( int i = 0; i < precision/2; ++i )
    {
        theta1 = i * ngl::TWO_PI / precision - ngl::PI2;
        theta2 = (i + 1) * ngl::TWO_PI / precision - ngl::PI2;

        for( int j = 0; j <= precision; ++j )
        {
            theta3 = j * ngl::TWO_PI / precision;

            d.nx = cosf(theta2) * cosf(theta3);
            d.ny = sinf(theta2);
            d.nz = cosf(theta2) * sinf(theta3);
            d.x = radius * d.nx;
            d.y = radius * d.ny;
            d.z = radius * d.nz;

            d.u  = (j/(float)precision);
            d.v  = 2*(i+1)/(float)precision;

      data1[index++]=d;

            d.nx = cosf(theta1) * cosf(theta3);
            d.ny = sinf(theta1);
            d.nz = cosf(theta1) * sinf(theta3);
            d.x = radius * d.nx;
            d.y = radius * d.ny;
            d.z = radius * d.nz;

            d.u  = (j/(float)precision);
            d.v  = 2*i/(float)precision;
      data1[index++]=d;
        } // end inner loop
    }// end outer loop

//    for(unsigned int i=0; i< data.size(); ++i)
//    {
//        std::cout << data[i].u << " " << data[i].v << "\n";
//    }


    // now we have our data add it to the VAO, we need to tell the VAO the following
    // how much (in bytes) data we are copying
    // a pointer to the first element of data (in this case the address of the first element of the
    // std::vector
    m_vaoSphere->setData(buffSize*sizeof(vertData),data1[0].u);
    // in this case we have packed our data in interleaved format as follows
    // u,v,nx,ny,nz,x,y,z
    // If you look at the shader we have the following attributes being used
    // attribute vec3 inVert; attribute 0
    // attribute vec2 inUV; attribute 1
    // attribute vec3 inNormal; attribure 2
    // so we need to set the vertexAttributePointer so the correct size and type as follows
    // vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
    // sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
    m_vaoSphere->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
    // uv same as above but starts at 0 and is attrib 1 and only u,v so 2
    m_vaoSphere->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
    // normal same as vertex only starts at position 2 (u,v)-> nx
    m_vaoSphere->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);
    // now we have set the vertex attributes we tell the VAO class how many indices to draw when
    // glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
    // specify less (in steps of 3))
  m_vaoSphere->setNumIndices(buffSize);
    // finally we have finished for now so time to unbind the VAO
    m_vaoSphere->unbind();
//    m_vaoSphere= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
//    m_vaoSphere->bind();
//    // move the object into the middle of the GLWindow.
//    ngl::Vec3 translation = data->m_maxLimits + data->m_minLimits;
//    m_modelPos = translation/(-2);

//    m_vaoSphere->setIndexedData(24*sizeof(GLfloat),data->m_vertices[0],sizeof(data->m_indices),&data->m_indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
//    m_vaoSphere->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

//    m_vaoSphere->setIndexedData(24*sizeof(GLfloat),data->m_UVs[0],sizeof(data->m_indices),&data->m_indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
//    m_vaoSphere->setVertexAttributePointer(1,2,GL_FLOAT,0,0);

//    m_vaoSphere->setIndexedData(24*sizeof(GLfloat),data->m_normals[0],sizeof(data->m_indices),&data->m_indices[0],GL_UNSIGNED_INT,GL_STATIC_DRAW);
//    m_vaoSphere->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

//    m_vaoSphere->setNumIndices(sizeof(data->m_indices));

//    m_vaoSphere->unbind();
//    updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow1::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    // Rotation based on the mouse position for our global transform
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
    final.m_m[3][0] = m_modelPos.m_x;
    final.m_m[3][1] = m_modelPos.m_y;
    final.m_m[3][2] = m_modelPos.m_z;
    // set this in the TX stack
    trans.setMatrix(final);
    m_transformStack.setGlobal(trans);

//  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

//  if(!m_shaderMode && m_currentTextureName==0)
//  {
//     (*shader)["Phong"]->use();
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
//  }
//  else
//  {
//      (*shader)["TextureShader"]->use();
//      ngl::Mat4 MVP;
//      MVP=m_transformStack.getCurrAndGlobal().getMatrix()*m_cam->getVPMatrix();
//      shader->setShaderParamFromMat4("MVP",MVP);
//  }

  if (m_vaoSphere!=NULL)
  {
      m_vaoSphere->bind();
    m_vaoSphere->draw();
    m_vaoSphere->unbind();
  }

  std::cout << "* Object painted\n";
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow1::mouseMoveEvent (
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
void GLWindow1::mousePressEvent (
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
void GLWindow1::mouseReleaseEvent (
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
void GLWindow1::wheelEvent(QWheelEvent *_event)
{

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
