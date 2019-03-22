/** file */  /* Causes DoxyGen to make an "Include dependency graph" */
//---------------------------------------------------------------------------
//  $Header$
//
//  Company    : Holip
//
//  Project    :  
//
//  Filename   : sharebits.h
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
   
//-------------------- pragmas ----------------------------------------------
//-------------------- include file ----------------------------------------
//-------------------- local definition ------------------------------------
//-------------------- private data -----------------------------------------
//-------------------- private typedef --------------------------------------
//-------------------- private function -------------------------------------
//-------------------- public definition ------------------------------------
//-------------------- public data ------------------------------------------
//-------------------- public function -------------------------------------
void putleu16(void *ptr,UNSIGNED16 off,UNSIGNED16 val);   
UNSIGNED16 getleu16(void *ptr,UNSIGNED16 off);   
UNSIGNED32 getleu32(void *ptr,UNSIGNED32 off);
void putleu32(void *ptr,UNSIGNED32 off,UNSIGNED32 val);
void putles16(void *ptr,SIGNED16 off,SIGNED16 val);   
UNSIGNED16 getles16(void *ptr,SIGNED16 off);   
UNSIGNED32 getles32(void *ptr,SIGNED32 off);
void putles32(void *ptr,SIGNED32 off,SIGNED32 val);
UNSIGNED8 * STRCHR(UNSIGNED8 *s, UNSIGNED8 c);

//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------

