#ifndef BINFILE_H
#define BINFILE_H

#include "binaryreader.h"
#include "bil.h"
#include "bsq.h"
#include "multifile.h"

//-------------------------------------------------------------------------
// Class to use for BIL/BSQ reader - make an instance of this one to use
// for either a BIL or BSQ file
//-------------------------------------------------------------------------
namespace bilLib
{

class BinFile
{
public:
   BinFile(){};//default constructor for inheritence
   BinFile(std::string fname);
   virtual ~BinFile();

   virtual void Readline(char* const chdata){br->Readline(chdata);}
   virtual void Readline(char* const chdata, unsigned int line){br->Readline(chdata,line);}
   virtual void Readlines(char* const chdata, unsigned int startline, unsigned int numlines){br->Readlines(chdata,startline,numlines);}
   virtual void Readbytes(char* const chdata, unsigned long int bytes){br->Readbytes(chdata,bytes);}
   virtual int Readband(char* const chdata, unsigned int band){return br->Readband(chdata,band);}
   virtual int Readbandline(char* const chdata, unsigned int band, unsigned int line){return br->Readbandline(chdata,band,line);}

   virtual double ReadCell(const unsigned int band,const unsigned int line, const unsigned int col){return br->ReadCell(band,line,col);}
   virtual void ReadlineToDoubles(double* const ddata,unsigned int line){return br->ReadlineToDoubles(ddata,line);}

   virtual std::string FromHeader(std::string key,std::string THROW="false"){return br->FromHeader(key,THROW);}
   virtual std::string FromHeader(std::string key,int itemnum,std::string THROW="false"){return br->FromHeader(key,itemnum,THROW);}

   virtual unsigned int GetDataSize() const {return br->GetDataSize();}
   virtual unsigned int GetDataType() const {return br->GetDataType();}
   virtual void Close(){br->Close();br=NULL;}
   virtual std::string HeaderDump(bool ret){return br->HeaderDump(ret);}
   virtual std::string TidyForHeader(std::string totidy,bool wrapinbraces=false){return br->TidyForHeader(totidy,wrapinbraces);}
   virtual uint64_t GetFileSize() const {return br->GetFileSize();}
   virtual std::map<std::string, std::string, cmpstr> CopyHeader()const{return br->CopyHeader();}

   virtual std::map<std::string, std::string, cmpstr> CopyHeaderExcluding();

   virtual std::string GetHeaderFilename(){return br->GetHeaderFilename();}
   virtual std::string MissingHeaderItemError()const{return br->MissingHeaderItemError();}

   virtual std::string GetFileName()const{return br->GetFileName();}

   virtual uint64_t NumLines()const{return br->NumLines();}
   virtual uint64_t NumSamples()const{return br->NumSamples();}
   virtual uint64_t NumBands()const{return br->NumBands();}

protected:
   BinaryReader* br;
};

}
#endif
