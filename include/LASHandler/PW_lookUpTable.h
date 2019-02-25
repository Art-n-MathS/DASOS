#ifndef PW_LOOKUPTABLE_H
#define PW_LOOKUPTABLE_H

#include <vector>

class PW_lookUpTable
{
public:
   #pragma pack(push)
   #pragma pack(1)
   typedef struct LookUpTable                             //_ bytes
   {
      unsigned int size;                                  //   4 bytes
      unsigned int reserved;                              //   4 bytes
      unsigned int number_entries;                        //   4 bytes
      unsigned short int unit_of_measurement;             //   2 bytes
      unsigned char data_type;                            //   1 byte
      unsigned char options;                              //   1 byte
      unsigned int compression;                           //   4 bytes
      //--
//      char description[64];                              //  64 bytes
   }LookUpTable;
   #pragma pack(pop)


   #pragma pack(push)
   #pragma pack(1)
   typedef struct TableRecord                             //_ bytes
   {
      unsigned int size;                                  //   4 bytes
      unsigned int reserved;                              //   4 bytes
      unsigned int number_tables;                         //   4 bytes

      //--
      //      char description[64;                        //  64 bytes
   }TableRecord;
   #pragma pack(pop)

   //-------------------------------------------------------------------------
   /// @brief default contrtuctor
   //-------------------------------------------------------------------------
   PW_lookUpTable(
           char *skip_record,
           unsigned int record_length,
           unsigned short i_id
           );
   //-------------------------------------------------------------------------
   /// @brief print all the records
   //-------------------------------------------------------------------------
   void print()const;
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~PW_lookUpTable();


private:
   //-------------------------------------------------------------------------
   /// @brief Table record
   //-------------------------------------------------------------------------
   TableRecord m_tableRecord;
   //-------------------------------------------------------------------------
   /// @brief look up tables
   //-------------------------------------------------------------------------
   std::vector<LookUpTable *> m_lookUpTables;
   //-------------------------------------------------------------------------
   /// @brief descriptor id, which is a unique number
   //-------------------------------------------------------------------------
   unsigned short int m_id;

};

#endif // PW_LOOKUPTABLE_H
