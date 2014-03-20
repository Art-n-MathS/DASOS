#include "MCwithIntegralImages.h"
#include "math.h"

//-----------------------------------------------------------------------------
MCwithIntegralImages::MCwithIntegralImages(
        Object *i_obj,
        unsigned int i_x
        ):
    MarchingCubes(i_obj,i_x)
{
}

//-----------------------------------------------------------------------------
void MCwithIntegralImages::divideVolume(
        unsigned short int i_dividableSides,
        const unsigned short int i_n,
        const std::vector<unsigned short int> &i_mins,
        const std::vector<unsigned short int> &i_lens,
        GLData *i_glData
        )
{
    // if there are not dividable sides then there is only one cube left
    if(i_dividableSides==0)
     {
       //polygonise cube
       polygoniseXYZ(i_mins[0],i_mins[1],i_mins[2],m_obj->getIsolevel(),i_glData);
       return;
    }

//    // check if the area is empty. If it is then return without doing anything.
    if(m_obj->m_integralVolume->getSumOfArea(i_mins[0],i_mins[1],i_mins[2],i_lens[0]-1,i_lens[1]-1,i_lens[2]-1)<0.0f)
    {
//        // the volume is empty => no more process is required
        return;
    } else
    {
    }

    unsigned char lala = i_dividableSides&(unsigned int)(pow(2.0,(double)i_n));

    // value will be 1 instead of 0 - need to check when program terminated and
    // update values correctly
   if(lala!=0) // then current side is dividable
   {
       std::vector<unsigned short int> nMins1(i_mins),nMins2(i_mins);
       std::vector<unsigned short int> nLens1(i_lens),nLens2(i_lens);
       nLens1[i_n] = i_lens[i_n]/2;
       nLens2[i_n] = i_lens[i_n]-nLens1[i_n];
       nMins2[i_n]+=nLens1[i_n];

       if(nLens1[i_n]==0)
           return;
       // 7 if all sides are divisible
       // 2^i_n gives me 1, 2 or 4 for the current side
       // 7-2^i_n gives me 0 for the current side and 1 for the rest
       // (unsigned int)(pow(2.0,(double)i_n)
       if(nLens1[i_n]==1)
       {
          // that side cannot be divided anymore
           divideVolume(i_dividableSides&
              ((unsigned short int)7-(unsigned short int)pow(2.0,double(i_n))),
              (i_n+1)%3,nMins1,nLens1,i_glData);

       }
       else
       {
          // the divisables status haven't change
          divideVolume(i_dividableSides,(i_n+1)%3,nMins1,nLens1,i_glData);

       }

       if(nLens2[i_n]==1)
       {
          // that side cannot be divided anymore
          divideVolume(i_dividableSides&(
               (unsigned short int)7-(unsigned short int)pow(2.0,double(i_n))),
               (i_n+1)%3,nMins2,nLens2,i_glData);
       }
       else
       {
          divideVolume(i_dividableSides,(i_n+1)%3,nMins2,nLens2,i_glData);
       }
   }
   else
   {
      // current side is not divisible but at least another one is
      // so the recursion does not stop until it finds the other side
      divideVolume(i_dividableSides,(i_n+1)%3,i_mins,i_lens,i_glData);
   }
}

//-----------------------------------------------------------------------------
void MCwithIntegralImages::computeVertices(GLData *i_glData)
{
//   m_obj->m_integralVolume->print();
   std::cout << "Marching Cubes algorithm with integral Volumes\n";
   std::vector<unsigned short int> mins(3,0);
   std::vector<unsigned short int> lens(3);
   lens[0]=m_numOfCubsX-1; lens[1]=m_numOfCubsY-1; lens[2]=m_numOfCubsZ-1;
    // 7 for 1+2+4 => all the 3 coordinates are dividable
   divideVolume(7,0,mins,lens,i_glData);

//    std::vector<ngl::Vec3> points;
//    points.resize(8);
//    for(unsigned int x=0; x<m_numOfCubsX-1; ++x)
//    {
//       for(unsigned int y=0; y<m_numOfCubsY-1; ++y)
//       {
//          for (unsigned int z=0; z<m_numOfCubsZ-1; ++z)
//          {
//             polygoniseXYZ(x,y,z,m_obj->getIsolevel(),i_glData);
//          }
//       }
//    }
}


//-----------------------------------------------------------------------------
MCwithIntegralImages::~MCwithIntegralImages()
{}
