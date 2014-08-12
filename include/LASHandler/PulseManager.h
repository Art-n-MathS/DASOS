#ifndef PULSEMANAGER_H
#define PULSEMANAGER_H

#include "Pulse.h"
#include "Types.h"
#include "Object.h"

#include <vector>
#include <gmtl/Vec.h>

class PulseManager
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_wv_info
   //-------------------------------------------------------------------------
   PulseManager(
           const Types::Public_Header_Block &i_publicHeader,
           const Types::WF_packet_Descriptor &i_wv_info
           );
   //-------------------------------------------------------------------------
   /// @brief copy constructor
   /// @param[in] i_pulseManager object to be copied
   //-------------------------------------------------------------------------
   PulseManager(const PulseManager &i_pulseManager);
   //-------------------------------------------------------------------------
   /// @brief method that adds a point to the manager
   /// @param[in] i_point
   //-------------------------------------------------------------------------
   void addPoint(
           const Types::Data_Point_Record_Format_4 &i_point,
           const char *wave_data
           );
   //-------------------------------------------------------------------------
   /// @brief method that returns the max x coordinates as specified in
   /// the header file
   /// @returns the max x coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxX()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min x coordinates as specified in
   /// the header file
   /// @returns the min x coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinX()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max y coordinates as specified in
   /// the header file
   /// @returns the max y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxY()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min y coordinates as specified in
   /// the header file
   /// @returns the min y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinY()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max y coordinates as specified in
   /// the header file
   /// @returns the max y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxZ()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min z coordinates as specified in
   /// the header file
   /// @returns the min z coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinZ()const;
   //-------------------------------------------------------------------------
   /// @brief method that sets the kernel that is used to smooth the waves
   /// @param[in] i_kernel the new kernel
   //-------------------------------------------------------------------------
   void setKernel(const std::vector<double> &i_kernel);
   //-------------------------------------------------------------------------
   /// @brief method that voxelised the given area and returns an implicit
   /// object. Please note that the object is dynamically allocated
   /// this class is not resposible for freeing that memory, so by calling
   /// this function, you are resposible for deleting that object
   /// @param[in] i_x the number of voxels in the x dimension - the rest are
   /// calculated depending on the higher and lower x y z of the points
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   /// @return the implicit object which is a made by voxels
   //-------------------------------------------------------------------------
   Object *fillObject(
           Object *obj,
           const std::vector<double> &user_limits
           )const;
   //-------------------------------------------------------------------------
   /// @brief method that sets the noise level
   /// @param[in] i_noiseLevel the new noise level
   //-------------------------------------------------------------------------
   void setNoiseLevel(double i_noiseLevel);
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~PulseManager();

private:
   //-------------------------------------------------------------------------
   /// @brief method that sorts the pulses with respect to the y position of
   /// their origins
   //-------------------------------------------------------------------------
   void sortPulseWithRespectToY();

   //-------------------------------------------------------------------------
   /// @brief public header block
   //-------------------------------------------------------------------------
   Types::Public_Header_Block m_public_header;
   //-------------------------------------------------------------------------
   /// @brief waveform packet descriptor
   //-------------------------------------------------------------------------
   Types::WF_packet_Descriptor m_wfInfo;
   //-------------------------------------------------------------------------
   /// @brief all the pulses are saved into a vector memory can be cleared
   //-------------------------------------------------------------------------
   std::vector<Pulse *> m_pulses;
   //-------------------------------------------------------------------------
   /// @brief the noise Level
   //-------------------------------------------------------------------------
   double m_noiseLevel;





};

#endif // PULSEMANAGER_H
