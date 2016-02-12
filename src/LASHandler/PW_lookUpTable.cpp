#include "PW_lookUpTable.h"
#include <stdio.h>
#include <string.h>
#include <iostream>


//-----------------------------------------------------------------------------
PW_lookUpTable::PW_lookUpTable(
        char *skip_record,
        unsigned int record_length,
        unsigned short i_id
        ):m_id(i_id)
{
   unsigned int offsetToStartOfRecord =0;
   char *tempSkipRecord = skip_record;
   unsigned int size;
   memcpy((void *)&size,(void *)skip_record,sizeof(unsigned int));
   if(size+offsetToStartOfRecord>record_length)
   {
      std::cout << "Warning: size of scanner VLR bigger than allocated\n";
   }
   memcpy((void *)&m_tableRecord,(void *)skip_record,sizeof(TableRecord));
   offsetToStartOfRecord+=m_tableRecord.size;
   m_lookUpTables.resize(m_tableRecord.number_tables);
   for(unsigned int i=0; i<m_lookUpTables.size(); ++i)
   {
      tempSkipRecord=skip_record+offsetToStartOfRecord;
      memcpy((void *)&size,(void *)tempSkipRecord,sizeof(unsigned int));
      if(size+offsetToStartOfRecord>record_length)
      {
         std::cout << "Warning: size of scanner VLR bigger than allocated\n";
      }
      m_lookUpTables[i] = new LookUpTable;
      memcpy((void *)m_lookUpTables[i],(void *)tempSkipRecord,sizeof(LookUpTable));
      offsetToStartOfRecord+=m_lookUpTables[i]->size;
   }
}

//-----------------------------------------------------------------------------
void PW_lookUpTable::print()const
{
   std::cout << "Look up table with " << m_tableRecord.number_tables << " tables\n";
   for(unsigned int i=0; i< m_lookUpTables.size(); ++i)
   {
      std::cout << "size = " << (int)m_lookUpTables[i]->size << "\n";
      std::cout << "reserved = " <<(int) m_lookUpTables[i]->reserved << "\n";
      std::cout << "number_entries = " << (int)m_lookUpTables[i]->number_entries << "\n";
      std::cout << "unit_of_measurement = "<<(int)m_lookUpTables[i]->unit_of_measurement<<"\n";
      std::cout << "data_type = " << (int)m_lookUpTables[i]->data_type << "\n";
      std::cout << "options = " << (int)m_lookUpTables[i]->options << "\n";
      std::cout << "compression = " << (int)m_lookUpTables[i]->compression << "\n";
   }
}

//-----------------------------------------------------------------------------
PW_lookUpTable::~PW_lookUpTable()
{
   for(unsigned int i=0; i<m_lookUpTables.size(); ++i)
   {
      delete m_lookUpTables[i];
   }
}
