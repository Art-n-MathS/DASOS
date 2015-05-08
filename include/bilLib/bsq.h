//January 18th 2011
//M Warren

#ifndef BSQ_H
#define BSQ_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <exception>
#include "binaryreader.h"

//Class for reading in BSQ files. Inherits from the BinaryReader class
namespace bilLib
{


class BSQReader:public BinaryReader
{
public:
   BSQReader(std::string fname);
   ~BSQReader();

   void Readline(char* const chdata); //Read 1 line for all bands [equivalent number of bytes from current position]
   void Readline(char* const chdata, unsigned int line) //Read specified line for all bands
         {Readlines(chdata,line,1);} //This is essentially a special case of readlines where numlines=1
   void Readlines(char* const chdata, unsigned int startline, unsigned int numlines); //Read numlines lines of data from startline
   void Readbytes(char* const chdata, uint64_t bytes); //Read the specified number of bytes from current position
   int Readband(char* const chdata, unsigned int band); //Reads the specified band
   int Readbandline(char* const chdata, unsigned int band, unsigned int line); //reads the given line for the given band

   double ReadCell(const unsigned int band,const unsigned int line, const unsigned int col);
   void ReadlineToDoubles(double* const ddata,unsigned int line);
protected:

};

}
#endif
