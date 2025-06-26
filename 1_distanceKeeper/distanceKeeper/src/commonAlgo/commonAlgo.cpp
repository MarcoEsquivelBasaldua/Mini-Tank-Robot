/******************************************************************************
*						  commonAlgo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Library with common algorithms for the other algorithms
******************************************************************************/
#include "commonAlgo.h"

/**********************************************************
*  Function u_linBoundInterpol
*
*  Brief: Bounded linear interpolation is done.
*
*          u_maxOut .|              .......
*                    |             /
*                    |            /
*                    |           /
*          u_minOut .|........../
*                    |________________________________________
*                               .  .
*                         u_minIn   u_maxIn
*
*  Inputs: [uint8] u_input   : input to be mapped
*          [uint8] u_minDist : minimum allowed Input
*          [uint8] u_maxDist : maximum allowed Input
*          [uint8] u_minOut  : minimum allowed output
*          [uint8] u_maxOut  : maximum allowed output
*
*  Outputs: [uint8] mapped value
*
*  Wire Inputs: None
**********************************************************/
uint8 u_linBoundInterpol(uint8 const u_input  ,
                         uint8 const u_minIn  , uint8 const u_maxIn, 
                         uint8 const u_minOut , uint8 const u_maxOut)
{
	float f_input;
	float f_minIn;
	float f_maxIn;
	float f_minOut;
	float f_maxOut;
	float f_slope;
	float f_out;

	if(u_input <= u_minIn)
	{
		return u_minOut;
	}
	else if(u_input >= u_maxIn)
	{
		return u_maxOut;
	}
	else
	{
		/* Convert to float all variables */
		f_input   = (float)u_input;
		f_minIn = (float)u_minIn;
		f_maxIn = (float)u_maxIn;
		f_minOut  = (float)u_minOut;
		f_maxOut  = (float)u_maxOut;

		f_slope = (f_maxOut - f_minOut)/(f_maxIn - f_minIn);
		f_out = f_slope * (f_input - f_minIn) + f_minOut;
	}
	return (uint8)f_out;
}

/**********************************************************
*  Function u_abs_16to8()
*
*  Brief: Get absolute value from sint16 value and cast it into uint8
*
*  Inputs: [sint16] inVal : input value
*
*  Outputs: [uint8] outVal : output value
**********************************************************/
uint8 u_abs_16to8(sint16 const inVal)
{
	uint8 outVal;

	if (inVal >= 0)
	{
		outVal = uint8(inVal);
	}
	else
	{
		outVal = uint8(-inVal);
	}

	return outVal;
}

/**********************************************************
*  Function f_abs_floatTofloat()
*
*  Brief: Get absolute value from float32 value and also return float32
*
*  Inputs: [float32] inVal : input value
*
*  Outputs: [float32] outVal : output value
**********************************************************/
float32 f_abs_floatTofloat(float32 const inVal)
{
	float32 outVal;
	
	if (inVal >= 0)
	{
		outVal = inVal;
	}
	else
	{
		outVal = -inVal;
	}

	return outVal;
}
