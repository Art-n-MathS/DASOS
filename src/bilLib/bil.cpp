//January 18th 2011
//M Warren
//Methods for the BILReader class

#include "bil.h"
//#define BILREADERDEBUG
#ifndef BILREADERDEBUG
   #define DEBUGPRINT(X)  
#else
   #define DEBUGPRINT(X) std::cout<< X <<std::endl;
#endif

namespace bilLib
{

BILReader::BILReader(std::string fname)
{

   //Asssign filename
   this->filename=fname;
   //Want to open bil file, read in hdr file, and assign all the class variables
   this->filein=fopen(this->filename.c_str(),"rb");
   if(!this->IsOpen())
   {
      //File has failed to open
      brinfo<<"An error has occured opening the file: "<<this->filename<<std::endl;
      this->filesize=-1;
      this->numrows=-1;
      this->numsamples=-1;
      this->numbands=-1;
      this->datasize=-1;
      throw BRexception(brinfo.str()); //Throw an exception
   }
   else
   {
      //File exists and has been opened successfully
      //Get size of file
      FileSeek(filein,0,SEEK_END); //seek to end of file
      this->filesize=FileTell(filein);//get the position (in bytes)
      FileSeek(filein,0,SEEK_SET); //seek back to start of file
      //Now try and read in hdr file
      int retval=this->ReadHeader();

      //An error occured, store a general error message into the brinfo stream
      if(retval!=1)
      {
         brinfo<<"An error has occurred whilst reading from the hdr file."<<std::endl;      
         throw BRexception(brinfo.str()); //Throw an exception
      }
      std::map<std::string,std::string>::iterator it; //use an iterator to access the Header map
      //set the numsamps, number of lines etc and also perform other checks against file
      it=this->Header.find("interleave"); //Search for interleave type
      if(it==this->Header.end())//check if present in header
      {
         this->CannotFind("interleave");
      }
      if( it->second != "bil")
      {
         brinfo<<"Header file states file "<<this->filename<<" is not a bil file but a "<<it->second<<std::endl;
         throw BRexception(brinfo.str()); //Throw an exception
      }

      //get the number of lines
      it=this->Header.find("lines");
      if(it==this->Header.end()) //doesnt exist in Header map/file
         this->CannotFind("lines");
      else
         this->numrows=StringToUINT(it->second);

      it=this->Header.find("samples");
      if(it==this->Header.end()) //doesnt exist in Header map/file
         this->CannotFind("samples");
      else
         this->numsamples=StringToUINT(it->second);

      it=this->Header.find("bands");
      if(it==this->Header.end()) //doesnt exist in Header map/file
         this->CannotFind("bands");
      else
         this->numbands=StringToUINT(it->second);

      it=this->Header.find("data type");
      if(it==this->Header.end()) //doesnt exist in Header map/file
         this->CannotFind("data type");
      else
         this->datatype=StringToUINT(it->second);

      DatasizeFromDatatype(); //assign data size

      //Test filesize on disk against filesize from header file - casting to return as a uint64 type
      uint64_t fsize=static_cast<uint64_t>(this->numrows)*this->numsamples*this->numbands*this->datasize;

      if(fsize != this->filesize)
      {
         brinfo<<"Filesize from header: "<<fsize<<" does not agree with filesize from disk (for "<<this->filename<<"): "<<this->filesize<<std::endl;
         throw BRexception(brinfo.str()); //Throw an exception         
      }

      //Test for datatypes 6 or 9 - these are not capable of being read
      if((this->datatype==6)||(this->datatype==9))
      {
         brinfo<<"Data type is complex (real/imaginary). This file type is unsupported"<<std::endl;
         throw BRexception(brinfo.str()); //Throw an exception
      }
   }
}

BILReader::~BILReader()
{
   DEBUGPRINT("Entering BILReader destructor...")
   Close();
}

//Read in a lines worth of data for each band from the current file pointer position
void BILReader::Readline(char* const chdata)
{
   if(this->IsGood() == false)
   {
      DEBUGPRINT("Read line has failed...stream not good.") 
      brinfo<<"Read line function has failed because 'file stream is not good.'"<<std::endl;  
      brinfo<<"Further information for debugging: FERROR value: "<<this->Ferror()<<" FEOF: "<<this->Feof()<<std::endl;
      brinfo<<"File in question: "<<this->filename<<std::endl;
      brinfo<<strerror(errno)<<std::endl;    
      throw BRexception(brinfo.str());
   }

   //DEBUG statement
   DEBUGPRINT("Reading Line from BIL...");

   //number of bytes that should be read from the file
   int64_t nbytestoread=(this->numsamples * this->datasize * this->numbands);
   if(this->CheckCapacity(nbytestoread))
   {
      this->prevpointerloc=FileTell(this->filein);
      fread(chdata,sizeof(char),nbytestoread,filein);
   }
   else
   {
      brinfo<<"Attempted to read "<<nbytestoread<<" but file reports there are less bytes remaining to be read"<<std::endl;
      throw BRexception(brinfo.str());
   }
}

//Read in numlines lines of data for each band from startline
void BILReader::Readlines(char* const chdata, unsigned int startline, unsigned int numlines)
{
   //check error bit is not set
   if(this->IsGood() == false)
   {
      DEBUGPRINT("Readlines has failed...stream not good.") 
      brinfo<<"Readlines function has failed because 'file stream is not good.'"<<std::endl; 
      brinfo<<"Further information for debugging: FERROR value: "<<this->Ferror()<<" FEOF: "<<this->Feof()<<std::endl;     
      brinfo<<"File in question: "<<this->filename<<std::endl;    
      brinfo<<strerror(errno)<<std::endl;
      throw BRexception(brinfo.str());
   }
   
   //DEBUG statement
   DEBUGPRINT("Reading Lines from BIL...from "<<startline<<" reading "<<numlines<<" lines.");
   DEBUGPRINT("Position in file (before seeking):"<<ftello(filein));
   //number of bytes that should be read from the file
   uint64_t nbytestoread=(this->numsamples * this->datasize * this->numbands) * numlines;
   //position at which to start reading from
   uint64_t pos=(this->numsamples * this->datasize * this->numbands) * startline;

   //Move the file pointer to this position
   FileSeek(filein,pos,SEEK_SET);

   if(this->CheckCapacity(nbytestoread))
   {
      fread(chdata,sizeof(char),nbytestoread,filein);
   }
   else
   {
      brinfo<<"Attempted to read "<<nbytestoread<<" but file reports there are less bytes remaining to be read"<<std::endl;
      throw BRexception(brinfo.str());
   }
}

//Read in a certain number of bytes from the current file pointer position
void BILReader::Readbytes(char* const chdata, unsigned long int nbytes)
{
   if(this->IsGood() == false)
   {
      DEBUGPRINT("Readbytes has failed...stream not good.") 
      brinfo<<"Readbytes function has failed because 'file stream is not good.'"<<std::endl; 
      brinfo<<"Further information for debugging: FERROR value: "<<this->Ferror()<<" FEOF: "<<this->Feof()<<std::endl;     
      brinfo<<"File in question: "<<this->filename<<std::endl;    
      brinfo<<strerror(errno)<<std::endl;
      throw BRexception(brinfo.str());
   }

   //DEBUG statement
   DEBUGPRINT("Reading "<<nbytes<<" bytes from BIL...");
   if(this->CheckCapacity(nbytes))
   {
      this->prevpointerloc=FileTell(this->filein);
      fread(chdata,sizeof(char),nbytes,filein);
   }
   else
   {
      brinfo<<"Attempted to read "<<nbytes<<" but file reports there are less bytes remaining to be read"<<std::endl;   
      throw BRexception(brinfo.str());
   }
}

//Reads the specified band
int BILReader::Readband(char* const chdata, unsigned int band)
{
   //DEBUG statement
   DEBUGPRINT("Reading band "<<band<<" from BIL...");
   for(unsigned int line=0;line<this->numrows;line++)
   {
      Readbandline(&chdata[this->numsamples*line*this->datasize],band,line);
   }
   return 1;
}

//reads the given line for the given band
int BILReader::Readbandline(char* const chdata, unsigned int band, unsigned int line)
{
   if(this->IsGood() == false)
   {
      DEBUGPRINT("Readbandline has failed...stream not good.") 
      brinfo<<"Readbandline function has failed because 'file stream is not good.'"<<std::endl;   
      brinfo<<"Further information for debugging: FERROR value: "<<this->Ferror()<<" FEOF: "<<this->Feof()<<std::endl;   
      brinfo<<"File in question: "<<this->filename<<std::endl;    
      brinfo<<strerror(errno)<<std::endl;
      throw BRexception(brinfo.str());
   }

   //DEBUG statement
   DEBUGPRINT("Reading line "<<line<<" from band "<<band);
   //number of bytes that should be read from the file
   uint64_t nbytestoread=this->numsamples * this->datasize;
   //position at which to start reading from
   uint64_t pos=(this->numsamples * this->datasize * this->numbands) * line  + (this->numsamples * this->datasize * band);

   //Move the file pointer to this position
   //this->filein.seekg(pos,std::ios::beg);
   //SeekInFileForWindows(pos);
   FileSeek(filein,pos,SEEK_SET);

   if(this->CheckCapacity(nbytestoread))
      fread(chdata,sizeof(char),nbytestoread,filein);
   else
   {
      brinfo<<"Attempted to read "<<nbytestoread<<" but file reports there are less bytes remaining to be read."<<std::endl;   
      throw BRexception(brinfo.str());
   }
   return 1;
}

//Read in a cell from the file and return it as a double
double BILReader::ReadCell(const unsigned int band,const unsigned int line, const unsigned int col)
{
   if(this->IsGood() == false)
   {
      DEBUGPRINT("ReadCell has failed...stream not good.") 
      brinfo<<"ReadCell function has failed because 'file stream is not good.'"<<std::endl;   
      brinfo<<"Further information for debugging: FERROR value: "<<this->Ferror()<<" FEOF: "<<this->Feof()<<std::endl;   
      brinfo<<"File in question: "<<this->filename<<std::endl;    
      brinfo<<strerror(errno)<<std::endl;
      throw BRexception(brinfo.str());
   }   

   if((line>numrows)||(col>numsamples))
   {
      brinfo<<"ReadCell has failed...row/col out of bounds."<<std::endl;   
      throw BRexception(brinfo.str());      
   }

   //Get the number of bytes to skip to get to point to read
   uint64_t nbytestoskip=((this->numsamples * this->numbands)*line + (this->numsamples * band) + col) * this->datasize;
   //Create a char array of the size of one cell
   char* cbuffer=new char[this->datasize];
   //Seek to and read that cell of data
   //this->filein.seekg(nbytestoskip,std::ios::beg);;
   FileSeek(filein,nbytestoskip,SEEK_SET);

   fread(cbuffer,this->datasize,1,filein);
   //Need to dereference this, convert to double and return
   double val=DerefToDouble(cbuffer);
   delete[] cbuffer;
   return val;
}


void BILReader::ReadlineToDoubles(double* const ddata,unsigned int line)
{
   char* chtmp=new char[this->numsamples*this->numbands* this->datasize];
   Readlines(chtmp,line,1);

   for(unsigned int sample=0;sample<this->numsamples*this->numbands;sample++)
   {
      //The following switch statement allows the data in chtmp to be derefenced
      //to many types, allowing different data formats in the BIL to be used 
      switch(this->datatype)
      {
      case 1: //8-bit
         {
            char* cderef=(char*)(chtmp);      
            ddata[sample]=static_cast<double>(cderef[sample]);
            break;   
         }
      case 2: //16 bit signed short int
         {
            short int* sideref=(short int*)(chtmp);      
            ddata[sample]=static_cast<double>(sideref[sample]);
            break;
         }
      case 3: //32 bit signed short int
         {
            int* ideref=(int*)(chtmp);      
            ddata[sample]=static_cast<double>(ideref[sample]);
            break;
         }
      case 4: //float
         {
            float* fderef=(float*)(chtmp);
            ddata[sample]=static_cast<double>(fderef[sample]);
            break;
         }
      case 5: //double
         {
            double* dderef=(double*)(chtmp);
            ddata[sample]=static_cast<double>(dderef[sample]);
            break;
         }
      case 12: //16 bit unsigned short int
         {
            unsigned short int* usideref=(unsigned short int*)(chtmp);
            ddata[sample]=static_cast<double>(usideref[sample]);
            break;
         }
      case 13: //32 bit unsigned int
         {
            unsigned int* uideref=(unsigned int*)(chtmp);
            ddata[sample]=static_cast<double>(uideref[sample]);
            break;
         }
      default:
         throw "Unrecognised data type for BIL file. Currently supports 8-bit, signed and unsigned 16-bit and 32-bit integer, 32-bit and 64-bit float";
         break;
      }
   }   


   delete[] chtmp;
}

}
