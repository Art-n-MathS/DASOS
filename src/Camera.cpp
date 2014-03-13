/* This class has been modified from:
  Copyright (C) 2009 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include "Camera.h"
#include <ngl/Util.h>
#include <ngl/NGLassert.h>
#include <ngl/VertexArrayObject.h>
#include <math.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file Camera.cpp
/// @brief implementation files for Camera class
//----------------------------------------------------------------------------------------------------------------------

// a lot of this stuff is from the HILL book Computer Graphics with OpenGL 2nd Ed Prentice Hall
// a very good book
const static ngl::Real CAMERANEARLIMIT=0.00001f;

//----------------------------------------------------------------------------------------------------------------------

Camera::Camera()
{
  // Set some  default values
   m_l=-3;
  m_r=3;
  m_t=3;
  m_b=-3;
  m_nr=-1;
  m_fr=-5;
  m_zNear=0.0001f;
  m_zFar=350.0f;
  m_aspect=720.0f/576.0f;
  m_fov=45.0f;
  m_projectionMode=PERSPECTIVE;
  m_width=720;
  m_height=576;
  m_eye.set(1.0f,1.0f,1.0f);
  m_look.set(0.0f,0.0f,1.0f,0.0f);
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setOrthoParams(
                            const ngl::Real _l,
                            const ngl::Real _r,
                            const ngl::Real _t,
                            const ngl::Real _b,
                            const ngl::Real _n,
                            const ngl::Real _f
                           )
{
    m_l=_l;
    m_r=_r;
    m_t=_t;
    m_b=_b;
    m_nr=-_n;
    m_fr=-_f;
}

//-----------------------------------------------------------------------------
void Camera::moveForward(const float i_step)
{
   ngl::Vec4 vStep = i_step * m_look;
   m_eye+=vStep;
   setViewMatrix();
}

//-----------------------------------------------------------------------------
void Camera::tiltCamera(const float i_degrees)
{
   float th = radians(i_degrees);
   ngl::Vec4 cross = m_up.cross(m_look);
   float u=cross.m_x, v=cross.m_y, w=cross.m_z, sq=u*u+v*v+w*w,
           x=m_look.m_x, y=m_look.m_y, z = m_look.m_z;

   m_look.m_x=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*x*cos(th)+sqrt(sq)
               *(-w*y+v*z)*sin(th))/sq;
   m_look.m_y=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*y*cos(th)+sqrt(sq)
               *( w*x-u*z)*sin(th))/sq;
   m_look.m_z=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*z*cos(th)+sqrt(sq)
               *(-v*x+u*y)*sin(th))/sq;

   u=cross.m_x; v=cross.m_y; w=cross.m_z; sq=u*u+v*v+w*w;
   x=m_up.m_x; y=m_up.m_y; z = m_up.m_z;

   m_up.m_x=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*x*cos(th)+sqrt(sq)
               *(-w*y+v*z)*sin(th))/sq;
   m_up.m_y=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*y*cos(th)+sqrt(sq)
               *( w*x-u*z)*sin(th))/sq;
   m_up.m_z=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*z*cos(th)+sqrt(sq)
               *(-v*x+u*y)*sin(th))/sq;
   m_n=m_eye-m_look;
   m_u=m_up.cross(m_n);
   m_v=m_n.cross(m_u);
   m_u.normalize();
   m_v.normalize();
   m_n.normalize();
   setViewMatrix();
}

//-----------------------------------------------------------------------------
void Camera::panCamera(const float i_degrees)
{
   float th = radians(i_degrees);
   float u=m_up.m_x, v=m_up.m_y, w=m_up.m_z, sq=u*u+v*v+w*w,
           x=m_look.m_x, y=m_look.m_y, z = m_look.m_z;

   m_look.m_x=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*x*cos(th)+sqrt(sq)
               *(-w*y+v*z)*sin(th))/sq;
   m_look.m_y=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*y*cos(th)+sqrt(sq)
               *( w*x-u*z)*sin(th))/sq;
   m_look.m_z=(u*(u*x+v*y+w*z)*(1-cos(th))+sq*z*cos(th)+sqrt(sq)
               *(-v*x+u*y)*sin(th))/sq;
   m_n=m_eye-m_look;
   m_u=m_up.cross(m_n);
   m_v=m_n.cross(m_u);
   m_u.normalize();
   m_v.normalize();
   m_n.normalize();
   setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera :: setDefaultCamera()
{
  // make default camera
  m_l=-3.0;
  m_r=3.0;
  m_t=3.0;
  m_b=-3.0;
  m_nr=-1.0;
  m_fr=-5.0;
  m_eye=1.0f;
  m_look=ngl::Vec4(0.0f,0.0f,1.0f,0.0f);
  m_up.set(0,1,0);
  m_fov=45.0;
  m_zNear=0.0001f;
  m_zFar=350.0f;
  m_aspect=720.0f/576.0f;
  m_fov=45.0f;
  m_projectionMode=PERSPECTIVE;

  setShape(m_fov, m_aspect, m_zNear, m_zFar,PERSPECTIVE); // good default values here
  set(ngl::Vec3(5.0, 5.0, 5.0),ngl::Vec3( 0.0, 0.0, 0.0),ngl::Vec3(0, 1, 0));
}

//----------------------------------------------------------------------------------------------------------------------
void Camera :: set(
                   const ngl::Vec3 &_eye,
                   const ngl::Vec3 &_look,
                   const ngl::Vec3 &_up
                  )
{
    // make U, V, N vectors
    m_eye=_eye;
    m_look=_look;
    m_up=_up;
    m_n=m_eye-m_look;
    m_u=m_up.cross(m_n);
    m_v=m_n.cross(m_u);
    m_u.normalize();
    m_v.normalize();
    m_n.normalize();
    setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
Camera::Camera(const ngl::Vec3 &_eye,
               const ngl::Vec3 &_look,
               const ngl::Vec3 &_up,
               const CAMERAPROJECTION _proj
              )
{
    m_projectionMode=_proj;
    setDefaultCamera();
    set(_eye,_look,_up);
}


//----------------------------------------------------------------------------------------------------------------------
void Camera::ChangeProjectionMode(
                                  const CAMERAPROJECTION _proj
                                 )
{
    m_projectionMode=_proj;
    setProjectionMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setViewMatrix()
{
    // grab a pointer to the matrix so we can index is quickly
    ngl::Real *M=(ngl::Real *)&m_viewMatrix.m_m;
    M[0] =  m_u.m_x;         M[1] =  m_v.m_x;        M[2] =  m_n.m_x;        M[3] = 0.0;
    M[4] =  m_u.m_y;         M[5] =  m_v.m_y;        M[6] =  m_n.m_y;        M[7] = 0.0;
    M[8]  = m_u.m_z;         M[9] =  m_v.m_z;        M[10]=  m_n.m_z;        M[11] =0.0;
    M[12] = -m_eye.dot(m_u); M[13]= -m_eye.dot(m_v); M[14]= -m_eye.dot(m_n); M[15] =1.0;

    calculateFrustum();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setOrthoProjection()
{
    m_projectionMatrix.identity();

    m_projectionMatrix.m_openGL[0]=2.0f /(m_r-m_l);
    m_projectionMatrix.m_openGL[5]=2.0f/(m_t-m_b);
    m_projectionMatrix.m_openGL[10]=2.0f / (m_fr-m_nr) ;//(m_zFar-m_zNear));


    m_projectionMatrix.m_openGL[12]=-((m_r+m_l)/(m_r-m_l));
    m_projectionMatrix.m_openGL[13]=-((m_t+m_b)/(m_t-m_b));
    m_projectionMatrix.m_openGL[14]=-((m_fr+m_nr) /(m_fr-m_nr)) ; // (f-n) ;//(m_zFar+m_zNear)/(m_zFar-m_zNear);
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::set2PointProjPoints(
                                 const ngl::Vec4 &_p1,
                                 const ngl::Vec4 &_p2)
{
    m_projPoints[0]=_p1;
    m_projPoints[1]=_p2;
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::set3PointProjPoints(
                                 const ngl::Vec3 &_p1,
                                 const ngl::Vec3 &_p2,
                                 const ngl::Vec3 &_p3
                                )
{
    m_projPoints[0]=_p1;
    m_projPoints[1]=_p2;
    m_projPoints[2]=_p3;
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::setTwoPointProjection()
{
    // Work in Progress not correct as yet
    // caluculate the vectors for the 2Point proj
    ngl::Vec4 v1=m_eye-m_projPoints[0];
    ngl::Vec4 v2=m_eye-m_projPoints[1];

    double angle =v1.angleBetween(v2);
    float c=sin(radians(angle));
    float s=cos(radians(angle));
    m_projectionMatrix.identity();
    std::cout <<m_eye<<v1<<v2<<" angle "<<angle<<std::endl;

    m_projectionMatrix.m_m[0][3]=s;
    m_projectionMatrix.m_m[2][3]=c;
    // these are unused remove in next inc
    //ngl::Real xmin, xmax,
    ngl::Real ymin, ymax;
    ymax = m_zNear * tan( m_fov * M_PI / 360.0f );
    ymin = -ymax;

// this is legacy I think check and remove asap
//	xmin = ymin * m_aspect;
//	xmax = ymax * m_aspect;


    m_projectionMatrix.m_m[3][0]=(-2.0f*m_zFar*m_zNear)/(m_zFar-m_zNear);

    m_projectionMatrix.m_m[3][2]=-1.0f;

}

//----------------------------------------------------------------------------------------------------------------------
ngl::Real Camera::radians(const ngl::Real i_deg)
{
   return (i_deg/180.0f) * 3.14159265358979323846 ;
}


//----------------------------------------------------------------------------------------------------------------------
void Camera::setThreePointProjection(){;}


//----------------------------------------------------------------------------------------------------------------------
void Camera::setPerspProjection()
{
  // note 1/tan == cotangent
  float f= 1.0/tan(radians(m_fov)/2.0);
  m_projectionMatrix.identity();

  m_projectionMatrix.m_m[0][0]=f/m_aspect;
  m_projectionMatrix.m_m[1][1]=f;

  m_projectionMatrix.m_m[2][2]=(m_zFar+m_zNear)/(m_zNear-m_zFar);
  m_projectionMatrix.m_m[3][2]=(2*m_zFar*m_zNear)/(m_zNear-m_zFar);

  m_projectionMatrix.m_m[2][3]=-1;
  m_projectionMatrix.m_m[3][3]=1.0;

}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setProjectionMatrix()
{
    m_projectionMatrix.null();
    switch (m_projectionMode)
    {
        case ORTHOGRAPHIC : { setOrthoProjection();      break; }
        case PERSPECTIVE  : { setPerspProjection();      break; }
        case TWOPOINT     : { setTwoPointProjection();   break; }
        case THREEPOINT   : { setThreePointProjection(); break; }
    }
}
//----------------------------------------------------------------------------------------------------------------------

void Camera::setShape(ngl::Real _viewAngle,
                       ngl::Real _aspect,
                       ngl::Real _near,
                       ngl::Real _far,
                       CAMERAPROJECTION _proj
                      )

{ // load projection matrix and camera values
    m_projectionMode=_proj;
    if(_viewAngle >180.0)
    {
        _viewAngle=180.0;
    }
    NGL_ASSERT(_far>_near);
    NGL_ASSERT(_near>CAMERANEARLIMIT);
    if(_near<CAMERANEARLIMIT)
    {
        _near=CAMERANEARLIMIT;
    }
    m_fov = _viewAngle; // viewangle in degrees - must be < 180
    m_aspect = _aspect;
    m_zNear = _near;
    m_zFar = _far;
    setProjectionMatrix();
    //calculateFrustum();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setAspect(
                        ngl::Real _asp
                      )
{
    m_aspect = _asp;
    setShape(m_fov,m_aspect,m_zNear,m_zFar,m_projectionMode);
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setViewAngle(
                                                     ngl::Real _angle
                                                 )
{
    m_fov=_angle;
    setShape(_angle,m_aspect,m_zNear,m_zFar,m_projectionMode);
}


//----------------------------------------------------------------------------------------------------------------------
void Camera::slide(
                   ngl::Real _du,
                   ngl::Real _dv,
                   ngl::Real _dn
                  )
{
    // slide eye by amount du * u + dv * v + dn * n;
    m_eye.m_x += _du * m_u.m_x + _dv * m_v.m_x + _dn * m_n.m_x;
    m_eye.m_y += _du * m_u.m_y + _dv * m_v.m_y + _dn * m_n.m_y;
    m_eye.m_z += _du * m_u.m_z + _dv * m_v.m_z + _dn * m_n.m_z;
    setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::move(
                  ngl::Real _dx,
                  ngl::Real _dy,
                  ngl::Real _dz
                 )
{
// simply add the translation to the current eye point
    m_eye.m_x += _dx;
    m_eye.m_y += _dy;
    m_eye.m_z += _dz;
    setViewMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::moveBoth(
                      ngl::Real _dx,
                      ngl::Real _dy,
                      ngl::Real _dz
                     )
{
    m_eye.m_x +=_dx;
    m_eye.m_y +=_dy;
    m_eye.m_z +=_dz;
    m_look.m_x+=_dx;
    m_look.m_y+=_dy;
    m_look.m_z+=_dz;
    m_n=m_eye-m_look;
    m_u=m_up.cross(m_n);
    m_v.set(m_n.cross(m_u));
    // normalize vectors
    m_u.normalize();
    m_v.normalize();
    m_n.normalize();
    // pass to OpenGL
    setViewMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::rotAxes(
                      ngl::Vec4& io_a,
                      ngl::Vec4& io_b,
                      const ngl::Real _angle
                     )
{
// rotate orthogonal vectors a (like x axis) and b(like y axis) through angle degrees
    // convert to radians
    ngl::Real ang = radians(_angle);
    // pre-calc cos and sine
    ngl::Real c = cos(ang);
    ngl::Real s = sin(ang);
    // tmp for io_a vector
    ngl::Vec4 t( c * io_a.m_x + s * io_b.m_x,  c * io_a.m_y + s * io_b.m_y,  c * io_a.m_z + s * io_b.m_z);
    // now set to new rot value
    io_b.set(-s * io_a.m_x + c * io_b.m_x, -s * io_a.m_y + c * io_b.m_y, -s * io_a.m_z + c * io_b.m_z);
    // put tmp into _a'
    io_a.set(t.m_x, t.m_y, t.m_z);
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::roll(ngl::Real _angle
                                 )
{
  rotAxes(m_u, m_v, -_angle);
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::pitch(ngl::Real _angle
                                     )
{
  rotAxes(m_n, m_v, _angle);
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::yaw(ngl::Real _angle
                                )
{
  rotAxes(m_u, m_n, _angle);
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::moveEye(
                     ngl::Real _dx,
                     ngl::Real _dy,
                     ngl::Real _dz
                    )
{
    m_eye.m_x+=_dx;
    m_eye.m_y+=_dy;
    m_eye.m_z+=_dz;
    m_n=m_eye-m_look;
    m_u.set(m_up.cross(m_n));
    m_v.set(m_n.cross(m_u));
    // normalize the vectors
    m_u.normalize();
    m_v.normalize();
    m_n.normalize();
    // pass to OpenGL
    setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::moveLook(
                      ngl::Real _dx,
                      ngl::Real _dy,
                      ngl::Real _dz
                     )
{
    m_look.m_x+=_dx;
    m_look.m_y+=_dy;
    m_look.m_z+=_dz;
    m_n=m_eye-m_look;
    m_u.set(m_up.cross(m_n));
    m_v.set(m_n.cross(m_u));
    // normalise vectors
    m_u.normalize();
    m_v.normalize();
    m_n.normalize();
    // pass to OpenGL
    setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::update()
{
  //setProjectionMatrix();
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::normalisedYaw(
                            ngl::Real _angle
                          )
{
  // build a rotation matrix around the y axis
  ngl::Mat4 mat;
  mat.identity();
  mat.rotateY(_angle);
  //multiply all three local coord vectors by the matrix
  m_u = m_u * mat;
  m_v = m_v * mat;
  m_n = m_n * mat;
  // reset the modelview matrix
  setViewMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::normalisedRoll(
                            ngl::Real _angle
                           )
{
  // build a rotation matrix around the y axis
  ngl::Mat4 mat;
  mat.identity();
  mat.rotateZ(_angle);
  //multiply all three local coord vectors by the matrix
  m_u = m_u * mat;
  m_v = m_v * mat;
  m_n = m_n * mat;
  // reset the modelview matrix
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::normalisedPitch(
                              ngl::Real _angle
                             )
{
  // build a rotation matrix around the y axis
  ngl::Mat4 mat;
  mat.identity();
  mat.rotateX(_angle);
  //multiply all three local coord vectors by the matrix
  m_u = m_u * mat;
  m_v = m_v * mat;
  m_n = m_n * mat;
  // reset the modelview matrix
  setViewMatrix();
}

//----------------------------------------------------------------------------------------------------------------------
NGL_DLLEXPORT std::ostream& operator<<(
                                                                             std::ostream& _output,
                                                                             const Camera &_c
                                                                            )
{
    return _output<<"m_eye"<<_c.m_eye<<" m_look"<<_c.m_look<<" Up"<<_c.m_up;
}


//----------------------------------------------------------------------------------------------------------------------
void Camera::writeRib(
                      ngl::RibExport &_rib
                     ) const
{
    if(_rib.IsOpen()!=0)
    {
        ngl::Real *M=(ngl::Real *)&m_viewMatrix.m_m;
        _rib.writeTabs();
        _rib.GetStream() <<"# Camera transform from GraphicsLib Camera\n"  ;
        _rib.GetStream() <<"# now we need to flip the Z axis\n";
        _rib.GetStream() <<"Scale 1 1 -1 \n";

        _rib.GetStream() <<"ConcatTransform [ ";
        for (int i=0; i<16; i++)
        {
            _rib.GetStream() <<M[i]<<" ";
        }
        _rib.GetStream() <<"]\n";
        _rib.GetStream() <<"# now we Set the clipping \n";
        _rib.GetStream() <<"Clipping "<<m_zNear<<" "<<m_zFar<<"\n";
        _rib.GetStream() <<"Projection \"perspective\" \"fov\" ["<<m_fov<<"]\n";
        _rib.GetStream() <<"#End of Camera from GraphicsLib\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// Code modified from http://www.lighthouse3d.com/opengl/viewfrustum/index.php?intro
///
void Camera::calculateFrustum()
{

    float tang = (float)tan(radians(m_fov) * 0.5) ;
    float nh = m_zNear * tang;
    float nw = nh * m_aspect;
    float fh = m_zFar  * tang;
    float fw = fh * m_aspect;

    ngl::Vec3 nc = (m_eye - m_n * m_zNear).toVec3();
    ngl::Vec3 fc = (m_eye - m_n * m_zFar).toVec3();

    m_ntl = nc + m_v.toVec3() * nh - m_u.toVec3() * nw;
    m_ntr = nc + m_v.toVec3() * nh + m_u.toVec3() * nw;
    m_nbl = nc - m_v.toVec3() * nh - m_u.toVec3() * nw;
    m_nbr = nc - m_v.toVec3() * nh + m_u.toVec3() * nw;

    m_ftl = fc + m_v.toVec3() * fh - m_u.toVec3() * fw;
    m_ftr = fc + m_v.toVec3() * fh + m_u.toVec3() * fw;
    m_fbl = fc - m_v.toVec3() * fh - m_u.toVec3() * fw;
    m_fbr = fc - m_v.toVec3() * fh + m_u.toVec3() * fw;
    m_planes[TOP].setPoints(m_ntr,m_ntl,m_ftl);
    m_planes[BOTTOM].setPoints(m_nbl,m_nbr,m_fbr);
    m_planes[LEFT].setPoints(m_ntl,m_nbl,m_fbl);
    m_planes[RIGHT].setPoints(m_nbr,m_ntr,m_fbr);
    m_planes[NEARP].setPoints(m_ntl,m_ntr,m_nbr);
    m_planes[FARP].setPoints(m_ftr,m_ftl,m_fbl);
}

void Camera::drawFrustum()
{
  std::vector<ngl::Vec3>points;

  // draw the sides as lines
  points.push_back(m_ntl);
  points.push_back(m_ftl);

  points.push_back(m_ntr);
  points.push_back(m_ftr);

  points.push_back(m_nbl);
  points.push_back(m_fbl);

  points.push_back(m_nbr);
  points.push_back(m_fbr);
  // near plane lines
  points.push_back(m_ntr);
  points.push_back(m_ntl);
  points.push_back(m_nbr);
  points.push_back(m_nbl);
  points.push_back(m_ntr);
  points.push_back(m_nbr);
  points.push_back(m_ntl);
  points.push_back(m_nbl);
  // far plane lines
  points.push_back(m_ftr);
  points.push_back(m_ftl);
  points.push_back(m_fbr);
  points.push_back(m_fbl);
  points.push_back(m_ftr);
  points.push_back(m_fbr);
  points.push_back(m_ftl);
  points.push_back(m_fbl);


  // now we create a VAO to store the data
  ngl::VertexArrayObject *vao=ngl::VertexArrayObject::createVOA(GL_LINES);
  // bind it so we can set values
  vao->bind();
  // set the vertex data (4 for x,y,z)
  vao->setData(points.size()*sizeof(ngl::Vec3),points[0].m_x);
  // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
  vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);
  // say how many indecis to be rendered
  vao->setNumIndices(points.size());
  vao->draw();
  // now unbind
  vao->unbind();
  vao->removeVOA();

}


Camera::CAMERAINTERCEPT Camera::isPointInFrustum(
                                            const ngl::Vec3 &_p
                                         ) const
{
    Camera::CAMERAINTERCEPT result = INSIDE;
    for(int i=0; i < 6; ++i)
    {

        if (m_planes[i].distance(_p) < 0)
        {
            return OUTSIDE;
        }
    }

    return result;
}


Camera::CAMERAINTERCEPT Camera::isSphereInFrustum(
                                            ngl::Vec3 &_p,
                                            float _radius
                                            ) const
{

    Camera::CAMERAINTERCEPT result = INSIDE;
    float distance;

    for(int i=0; i< 6; ++i)
    {
        distance = m_planes[i].distance(_p);
        if (distance < -_radius)
        {
            return OUTSIDE;
        }
        else if (distance < _radius)
        {
            result =  INTERSECT;
        }
    }
    return result;
}


/// end citation http://www.lighthouse3d.com/opengl/viewfrustum/index.php?intro

void Camera::writeXML(
                        QXmlStreamWriter* _stream,
                        const std::string _tag
                      ) const

{
    _stream->writeStartElement(_tag.c_str());

        _stream->writeStartElement("fov");
            _stream->writeCharacters(QString::number(m_fov));
        _stream->writeEndElement();
        _stream->writeStartElement("aspect");
            _stream->writeCharacters(QString::number(m_aspect));
        _stream->writeEndElement();
        _stream->writeStartElement("near");
            _stream->writeCharacters(QString::number(m_zNear));
        _stream->writeEndElement();
        _stream->writeStartElement("far");
            _stream->writeCharacters(QString::number(m_zFar));
        _stream->writeEndElement();

        m_eye.writeXML(_stream,"eye");
        m_look.writeXML(_stream,"look");
        m_up.writeXML(_stream,"up");

    _stream->writeEndElement();
}


