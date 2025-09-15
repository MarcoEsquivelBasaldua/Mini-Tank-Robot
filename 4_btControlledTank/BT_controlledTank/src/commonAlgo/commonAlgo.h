/******************************************************************************
*						  commonAlgo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Library with common algorithms for the other algorithms
******************************************************************************/
#ifndef COMMONALGO_h
#define COMMONALGO_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  MAX(x,y)          ( ((x)>(y)) ? (x) : (y) )  /* Max function macro */
#define  MIN(x,y)          ( ((x)<(y)) ? (x) : (y) )  /* Min function macro */

#define THREE_QUARTERS     (0.75f)

/*************************************************/

uint8 u_linBoundInterpol(uint8 const u_input  ,
                         uint8 const u_minIn  , uint8 const u_maxIn, 
                         uint8 const u_minOut , uint8 const u_maxOut);

uint8 u_abs_16to8(sint16 const inVal);

float32 f_abs_floatTofloat(float32 const inVal);

sint8 s_getSign(sint8 const s_value);

#endif
