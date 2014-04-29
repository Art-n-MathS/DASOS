#include "MarchingCubes.h"
#include "MCTables.h"

//-----------------------------------------------------------------------------
MarchingCubes::MarchingCubes(
        Object *i_obj,
        unsigned int i_x
        ):
    m_obj(i_obj),
    m_numOfCubsX(i_x),

    m_maxLimits(i_obj->getMaxLimits()),
    m_minLimits(i_obj->getMinLimits())
{
       // set the limits of the area
   ngl::Vec3 minLimits = m_obj->getMinLimits();
   ngl::Vec3 maxLimits = m_obj->getMaxLimits();

//    calculate how many voxels corresponds to the y and z axis
   ngl::Vec3 dis;
   dis.m_x = maxLimits.m_x - minLimits.m_x;
   dis.m_y = maxLimits.m_y - minLimits.m_y;
   dis.m_z = maxLimits.m_z - minLimits.m_z;
   if(m_obj->getNoVoxelsX()==i_x)
   {
      m_numOfCubsY = i_obj->getNoVoxelsY();
      m_numOfCubsZ = i_obj->getNoVoxelsZ();
   }
   else
   {
      m_numOfCubsY = ceil(((double)m_numOfCubsX)*dis.m_y/dis.m_x);
      m_numOfCubsZ = ceil(((double)m_numOfCubsX)*dis.m_z/dis.m_x);
   }
   std::cout << "Num of Cubes: " << m_numOfCubsX << " " << m_numOfCubsY << " " << m_numOfCubsZ << "\n";

}

//-----------------------------------------------------------------------------
unsigned int MarchingCubes::getIndex(
        GLData *i_glData,
        const ngl::Vec3 &i_vertex
        )
{
   for(unsigned int i=0; i<i_glData->m_vertices.size(); i+=3)
   {
       ngl::Vec3 v(i_glData->m_vertices[i  ],
                   i_glData->m_vertices[i+1],
                   i_glData->m_vertices[i+2]);
       if(v==i_vertex)
       {
           return i/3;
       }
   }
   return i_glData->addVertex(i_vertex);
}

//-----------------------------------------------------------------------------
/// Modified from:-
/// Paul Bourke (May, 1994) Polygonise a scalar field
/// Available from: paulbourke.net/geometry/polygonise [Accessed 10 Feb 2012]
//-----------------------------------------------------------------------------
// Linearly interpolate the ngl::Vec3 where an isosurface cuts
// an edge between two vertices, each with their own scalar value
//-----------------------------------------------------------------------------
ngl::Vec3 MarchingCubes::VertexInterp(
        double isolevel,
        const ngl::Vec3 &p1,
        const ngl::Vec3 &p2,
        const double valp1,
        const double valp2
        )
{
   double mu;
   ngl::Vec3 p;

   mu = (isolevel - valp1) / (valp2 - valp1);
   p.m_x = p1.m_x + mu * (p2.m_x - p1.m_x);
   p.m_y = p1.m_y + mu * (p2.m_y - p1.m_y);
   p.m_z = p1.m_z + mu * (p2.m_z - p1.m_z);

   return(p);
}
//-----------------------------------------------------------------------------
void MarchingCubes::Polygonise(
        const std::vector<ngl::Vec3> &points,
        double isolevel,
        GLData *i_glData
        )
{
   std::vector<double> values;
   unsigned int short cubeindex =0;
   unsigned int vertlist[12];

   values.resize(8);
   values[0] = m_obj->functionValue(points[0]);
   values[1] = m_obj->functionValue(points[1]);
   values[2] = m_obj->functionValue(points[2]);
   values[3] = m_obj->functionValue(points[3]);
   values[4] = m_obj->functionValue(points[4]);
   values[5] = m_obj->functionValue(points[5]);
   values[6] = m_obj->functionValue(points[6]);
   values[7] = m_obj->functionValue(points[7]);


   if (values[0] <= isolevel) cubeindex |= 1;
   if (values[1] <= isolevel) cubeindex |= 2;
   if (values[2] <= isolevel) cubeindex |= 4;
   if (values[3] <= isolevel) cubeindex |= 8;
   if (values[4] <= isolevel) cubeindex |= 16;
   if (values[5] <= isolevel) cubeindex |= 32;
   if (values[6] <= isolevel) cubeindex |= 64;
   if (values[7] <= isolevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
   if (edgeTable[cubeindex] == 0)
   {
      return;
   }
   /* Find the vertices where the surface intersects the cube */
   if (edgeTable[cubeindex] & 1)
   {
      vertlist[0] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[0],points[1],values[0],values[1]));
   }
   if (edgeTable[cubeindex] & 2)
   {
      vertlist[1] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[1],points[2],values[1],values[2]));
   }
   if (edgeTable[cubeindex] & 4)
   {
      vertlist[2] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[2],points[3],values[2],values[3]));
   }
   if (edgeTable[cubeindex] & 8)
   {
      vertlist[3] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[3],points[0],values[3],values[0]));
   }
   if (edgeTable[cubeindex] & 16)
   {
      vertlist[4] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[4],points[5],values[4],values[5]));
   }
   if (edgeTable[cubeindex] & 32)
   {
      vertlist[5] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[5],points[6],values[5],values[6]));
   }
   if (edgeTable[cubeindex] & 64)
   {
      vertlist[6] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[6],points[7],values[6],values[7]));
   }
   if (edgeTable[cubeindex] & 128)
   {
      vertlist[7] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[7],points[4],values[7],values[4]));
   }
   if (edgeTable[cubeindex] & 256)
   {
      vertlist[8] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[0],points[4],values[0],values[4]));
   }
   if (edgeTable[cubeindex] & 512)
   {
      vertlist[9] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[1],points[5],values[1],values[5]));
   }
   if (edgeTable[cubeindex] & 1024)
   {
      vertlist[10] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[2],points[6],values[2],values[6]));
   }
   if (edgeTable[cubeindex] & 2048)
   {
      vertlist[11] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[3],points[7],values[3],values[7]));
   }
   for(unsigned int i=0; triTable[cubeindex][i]!=-1;i+=3)
   {
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i  ]]);
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i+1]]);
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i+2]]);
   }
}

//-----------------------------------------------------------------------------
/// end of citation
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
ngl::Vec3 MarchingCubes::getXYZ(
        unsigned int i_x,
        unsigned int i_y,
        unsigned int i_z
        )
{
   ngl::Vec3 result;
   double dis = m_maxLimits.m_x-m_minLimits.m_x;
   dis/= (double)(m_numOfCubsX *i_x);
   result.m_x = m_minLimits.m_x + ( (m_maxLimits.m_x - m_minLimits.m_x)
                                    / (double(m_numOfCubsX)) * (double( i_x)));
   result.m_y = m_minLimits.m_y + ( (m_maxLimits.m_y - m_minLimits.m_y)
                                    / double(m_numOfCubsY) *(double (i_y)));
   result.m_z = m_minLimits.m_z + ( (m_maxLimits.m_z - m_minLimits.m_z)
                                    / double(m_numOfCubsZ) *(double (i_z)));
   return result;
}

//-----------------------------------------------------------------------------

void MarchingCubes::polygoniseXYZ(
        const unsigned int i_x,
        const unsigned int i_y,
        const unsigned int i_z,
        const double i_isolevel,
        GLData *i_glData
        )
{
    std::vector<ngl::Vec3> points;
    points.resize(8);
    points[0]= getXYZ(i_x  , i_y   ,i_z  );
    points[1]= getXYZ(i_x+1, i_y   ,i_z  );
    points[2]= getXYZ(i_x+1, i_y   ,i_z+1);
    points[3]= getXYZ(i_x  , i_y   ,i_z+1);
    points[4]= getXYZ(i_x  , i_y+1 ,i_z  );
    points[5]= getXYZ(i_x+1, i_y+1 ,i_z  );
    points[6]= getXYZ(i_x+1, i_y+1 ,i_z+1);
    points[7]= getXYZ(i_x  , i_y+1 ,i_z+1);
    Polygonise(points,i_isolevel,i_glData);
}

//-----------------------------------------------------------------------------
void MarchingCubes::computeVertices(GLData *i_glData)
{
   double isolevel = m_obj->getIsolevel();

   for(unsigned int x=0; x<m_numOfCubsX-1; ++x)
   {
      for(unsigned int y=0; y<m_numOfCubsY-1; ++y)
      {
         for (unsigned int z=0; z<m_numOfCubsZ-1; ++z)
         {
            polygoniseXYZ(x,y,z,isolevel,i_glData);
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
void MarchingCubes::computeNormals(GLData *i_glData
        )
{
  // resize the normals, so that the number of normals is equal to the number of vertices
  // each vertex should have each own normal.
  i_glData->m_normals.resize(i_glData->m_vertices.size());

  const std::vector<GLfloat>& vertices = i_glData->m_vertices;
  const std::vector<GLuint>& indices = i_glData->m_indices;
  unsigned int numOfIndices = i_glData->m_indices.size();

  // calculate normals
  ngl::Vec3 normal;
  for (unsigned int i=0;i<numOfIndices;i+=3) // size
  {
    // calculate normal of next triangle

    int index = indices[i]*3;
    const ngl::Vec3& A = ngl::Vec3(vertices[index],vertices[index+1],vertices[index+2]);
    index = indices[i+1]*3;
    const ngl::Vec3& B = ngl::Vec3(vertices[index],vertices[index+1],vertices[index+2]);
    index = indices[i+2]*3;
    const ngl::Vec3& C = ngl::Vec3(vertices[index],vertices[index+1],vertices[index+2]);
    const ngl::Vec3& AB = B-A;
    const ngl::Vec3& AC = C-A;
    normal.cross(AB,AC);

    // add the normal to all the vertices of that triangle
    if (normal.length()!=0)
    {
       i_glData->m_normals[indices[i]*3  ]-=normal.m_x;
       i_glData->m_normals[indices[i]*3+1]-=normal.m_y;
       i_glData->m_normals[indices[i]*3+2]-=normal.m_z;

       i_glData->m_normals[indices[i+1]*3  ]-=normal.m_x;
       i_glData->m_normals[indices[i+1]*3+1]-=normal.m_y;
       i_glData->m_normals[indices[i+1]*3+2]-=normal.m_z;

       i_glData->m_normals[indices[i+2]*3  ]-=normal.m_x;
       i_glData->m_normals[indices[i+2]*3+1]-=normal.m_y;
       i_glData->m_normals[indices[i+2]*3+2]-=normal.m_z;
    }
  }
  std::cout << "Normals calculated\n";
}

//-----------------------------------------------------------------------------
GLData *MarchingCubes::createPolygonisedObject()
{
   clock_t t1,t2;
   t1 =clock();

   GLData *glData = new GLData;
   glData->m_maxLimits = m_maxLimits;
   glData->m_minLimits = m_minLimits;
   m_hashTable.setGLData(glData);
   computeVertices(glData);
   computeNormals(glData);

   t2 = clock();
   float diff= ((float)t2-(float)t1) / CLOCKS_PER_SEC;
   std::cout << "Polygonisation took " << diff << " SECONDS!!!\n";

   return glData;
}


//-----------------------------------------------------------------------------
MarchingCubes::~MarchingCubes()
{
}
