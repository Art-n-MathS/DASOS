#include "MCwithIntegralImages.h"
#include "math.h"
#include <stack>
#include <iostream>

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
        unsigned int i_dividableSides,
        const unsigned int i_n,
        const std::vector<unsigned int> &i_mins,
        const std::vector<unsigned int> &i_lens,
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

    // check if the area is empty. If it is then return without doing anything.
    if(m_obj->m_integralVolume->getSumOfArea(i_mins[0],i_mins[1],i_mins[2],i_lens[0]-1,i_lens[1]-1,i_lens[2]-1)<0.0f)
    {
        // the volume is empty => no more process is required
        return;
    } else
    {
    }
//    std::cout << ": " << i_mins[0] << " " << i_mins[1] << " " << i_mins[2] << " "
//                                 << i_lens[0] << " " << i_lens[1] << " " << i_lens[2] << " -> ";
    std::cout << m_obj->m_integralVolume->getSumOfArea(i_mins[0],i_mins[1],i_mins[2],i_lens[0]-1,i_lens[1]-1,i_lens[2]-1) << " *** \n";
    return;
    unsigned char lala = i_dividableSides&(unsigned int)(pow(2.0,(double)i_n));

    // value will be 1 instead of 0 - need to check when program terminated and
    // update values correctly
   if(lala!=0) // then current side is dividable
   {
       std::vector<unsigned int> nMins1(i_mins),nMins2(i_mins);
       std::vector<unsigned int> nLens1(i_lens),nLens2(i_lens);
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
              ((unsigned int)7-(unsigned int)pow(2.0,double(i_n))),
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
               (unsigned int)7-(unsigned int)pow(2.0,double(i_n))),
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
   Cube currentCube = {{0,0,0},{m_numOfCubsX-2,m_numOfCubsY-2,m_numOfCubsZ-2},7,0};
//   if(m_obj->m_integralVolume->getSumOfArea(currentCube.m_mins[0],
//           currentCube.m_mins[1],currentCube.m_mins[2],currentCube.m_lens[1]-1,
//           currentCube.m_lens[1]-1,currentCube.m_lens[2]-1)<0.000001f)
//   {
//      // the volume is empty => no more process is required
//      return;
//   }
   double integralIsolevel = (m_obj->getIsolevel()+100.0)/2.0;
   std::stack<Cube> cubes;
   cubes.push(currentCube);
   while(cubes.size()!=0)
   {
      currentCube = cubes.top();
      cubes.pop();
      // check if only one cube is left and polygonise if it is
      if(currentCube.m_divisibles==0)
      {
         polygoniseXYZ( currentCube.m_mins[0], currentCube.m_mins[1],
                        currentCube.m_mins[2],m_obj->getIsolevel(),i_glData);
         continue;
      }
      // check if area is empty and discard if it is
      if(currentCube.m_mins[2]==0 || currentCube.m_mins[1]==0 || currentCube.m_mins[0]==0 ||m_obj->m_integralVolume->getSumOfArea(currentCube.m_mins[0]-1,currentCube.m_mins[1]-1,currentCube.m_mins[2]-1,currentCube.m_lens[0]+1,currentCube.m_lens[1]+1,currentCube.m_lens[2]+1)>integralIsolevel)
      {
      }
      else
      {
        // the volume is empty => no more process is required
         continue;
      }

      if(int(currentCube.m_divisibles&(unsigned int)(pow(2.0,(double)currentCube.m_nextSide)))==0)
      {
         // current side is not divisible, but at least one is,
         // so push current cube to stack after moving to the next side
         currentCube.m_nextSide = (currentCube.m_nextSide+1)%3;
         cubes.push(currentCube);
         continue;
      }
      // else current side is divisible, so divide it and push the cubes to the
      // stack if they are not empty.
      Cube cube1(currentCube);
      Cube cube2(currentCube);
      cube1.m_lens[currentCube.m_nextSide] = currentCube.m_lens[currentCube.m_nextSide]/2;
      cube2.m_lens[currentCube.m_nextSide] = currentCube.m_lens[currentCube.m_nextSide] -
                                         cube1.m_lens[currentCube.m_nextSide];
      cube2.m_mins[currentCube.m_nextSide]+= cube1.m_lens[currentCube.m_nextSide];
      cube1.m_nextSide = (currentCube.m_nextSide+1)%3;
      cube2.m_nextSide = cube1.m_nextSide;
      if(cube1.m_lens[currentCube.m_nextSide]==1)
      {
         cube1.m_divisibles = currentCube.m_divisibles&((unsigned int)7-
                       (unsigned int)pow(2.0,double(currentCube.m_nextSide)));
      }
      if(cube2.m_lens[currentCube.m_nextSide]==1)
      {
         cube2.m_divisibles = currentCube.m_divisibles&((unsigned int)7-
                       (unsigned int)pow(2.0,double(currentCube.m_nextSide)));
      }

      cubes.push(cube1);
      cubes.push(cube2);
   }
}


//-----------------------------------------------------------------------------
MCwithIntegralImages::~MCwithIntegralImages()
{}
