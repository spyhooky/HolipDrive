/** file */  /* Causes DoxyGen to make an "Include dependency graph" */
//---------------------------------------------------------------------------
//  $Header$
//
//  Company    : Holip
//
//  Project    :  
//
//  Filename   : sharebits.c
//
//  Programmer : Hu Yaoqi
//
//  Function   :
//
//  Entries    :
//
//
//
//                      ***  Confidential property of Holip ***
//                               Copyright(c) Holip, 2017
//--------------------------------------------------------------------------- 
#include <string.h>
#include "Holipheader.h"
#include "Sharebits.h"
//-------------------- pragmas ----------------------------------------------
//-------------------- include file ----------------------------------------
//-------------------- local definition ------------------------------------
//-------------------- private data -----------------------------------------
//-------------------- private typedef --------------------------------------
//-------------------- private function -------------------------------------
//-------------------- public definition ------------------------------------
//-------------------- public data ------------------------------------------
//-------------------- public function -------------------------------------

UNSIGNED32 HTONL(UNSIGNED32 n)
{
  return (((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24));
}

   
//---------------------------------------------------------------------------
//  Name     :       putleu16
//                  ======================================
//
//  Function :       big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
void putleu16(void *ptr,UNSIGNED16 off,UNSIGNED16 val)
{
   union
   {
      UNSIGNED16 s;
      char buf[sizeof(UNSIGNED16)];
   }u_s;
   u_s.s= (val);
   memcpy((UNSIGNED8*)(ptr)+off,u_s.buf,sizeof(UNSIGNED16));
}   

//---------------------------------------------------------------------------
//  Name     :       putles16
//                  ======================================
//
//  Function :       big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
void putles16(void *ptr,SIGNED16 off,SIGNED16 val)
{
   union
   {
      SIGNED16 s;
      char buf[sizeof(SIGNED16)];
   }u_s;
   u_s.s= (val);
   memcpy((UNSIGNED8*)(ptr)+off,u_s.buf,sizeof(SIGNED16));
} 


//---------------------------------------------------------------------------
//  Name     :          getleu16
//                  ======================================
//
//  Function :    big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
UNSIGNED16 getleu16(void *ptr,UNSIGNED16 off)
{
	union
	{
		UNSIGNED16 s;
		char buf[sizeof(UNSIGNED16)];
	}u_s;
	memcpy(u_s.buf,(char *)(ptr)+off,sizeof(UNSIGNED16));
   return (u_s.s);
}   


//---------------------------------------------------------------------------
//  Name     :          getles16
//                  ======================================
//
//  Function :    big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
UNSIGNED16 getles16(void *ptr,SIGNED16 off)
{
	union
	{
		SIGNED16 s;
		char buf[sizeof(SIGNED16)];
	}u_s;
	memcpy(u_s.buf,(char *)(ptr)+off,sizeof(SIGNED16));
   return (u_s.s);
}   


//---------------------------------------------------------------------------
//  Name     :       putleu32
//                  ======================================
//
//  Function :       big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
void putleu32(void *ptr,UNSIGNED32 off,UNSIGNED32 val)
{
   union
   {
      UNSIGNED32 s;
      char buf[sizeof(UNSIGNED32)];
   }u_s;
   u_s.s= (val);
   memcpy((UNSIGNED8*)(ptr)+off,u_s.buf,sizeof(UNSIGNED32));
} 

//---------------------------------------------------------------------------
//  Name     :       putles32
//                  ======================================
//
//  Function :       big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
void putles32(void *ptr,SIGNED32 off,SIGNED32 val)
{
   union
   {
      SIGNED32 s;
      char buf[sizeof(SIGNED32)];
   }u_s;
   u_s.s= (val);
   memcpy((UNSIGNED8*)(ptr)+off,u_s.buf,sizeof(SIGNED32));
} 


//---------------------------------------------------------------------------
//  Name     :          getleu32
//                  ======================================
//
//  Function :    big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
UNSIGNED32 getleu32(void *ptr,UNSIGNED32 off)
{
	union
	{
		UNSIGNED32 s;
		char buf[sizeof(UNSIGNED32)];
	}u_s;
	memcpy(u_s.buf,(char *)(ptr)+off,sizeof(UNSIGNED32));
   return (u_s.s);
} 

//---------------------------------------------------------------------------
//  Name     :          getles32
//                  ======================================
//
//  Function :    big edition
//
//  Input    : -
//
//  Output   : -
//
//---------------------------------------------------------------------------
UNSIGNED32 getles32(void *ptr,SIGNED32 off)
{
	union
	{
		SIGNED32 s;
		char buf[sizeof(SIGNED32)];
	}u_s;
	memcpy(u_s.buf,(char *)(ptr)+off,sizeof(SIGNED32));
   return (u_s.s);
} 


UNSIGNED8 * STRCHR(UNSIGNED8 *s, UNSIGNED8 c)
{
    while(*s != '\0' && *s != c)
    {
        ++s;
    }
    return *s==c ? s : NULL;
}





//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------

