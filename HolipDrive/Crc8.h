/** @file */  /* DoxyGen file declaration */
/** @addtogroup MOC_Communication
 *  @{
 */
//---------------------------------------------------------------------------
//  $Header$
//
//  Company    : Danfoss Drives A/S
//
//  Project    : P618 HVAC Basic (AOC)
//
//  Filename   : Crc8.c
//
//  Programmer : Han Feng, PE/BJ
//
//  Function   :
//
//  Entries    :    foo()
//
//  Requires   :    #include "AOCMOCInterface.h"
//                  
//
//                      ***  Confidential property of Danfoss Drives A/S ***
//                               Copyright(c) DD-MC, Danfoss Drives A/S, 2012
//---------------------------------------------------------------------------
#ifndef CRC8_H 
#define CRC8_H 

//-------------------- include files ----------------------------------------
//-------------------- public definitions -----------------------------------
//-------------------- public data ------------------------------------------
//-------------------- public functions -------------------------------------
UNSIGNED8 Crc8_byCalCrc8(UNSIGNED8 *pbyData, UNSIGNED32 dwDataLen, UNSIGNED8 byCrc8);
#endif    /* define CRC8_H */ 
/** @} *//* End of group */
//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------
