/*
 *  file:absolute_encoder.c
 *
 *  Created on: 2016年4月4日
 *
 *  Description: AVAGO--AEAT-84AD-LBSFO
 *
 *  Author: jiang qing wang
 */

#include "F2837xD_device.h"        // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"    // F2837xD Examples Include File
#include "Common_Definitions.h"     // Common Definitions Headerfile Include File
#include "absolute_encoder.h"   // absolute_encoder Definitions


unsigned int num_turns = 0;                   // 圈数
unsigned int last_turns = 0;                   // 圈数
unsigned int num_teeth = 0;                   // 齿数
unsigned int sel[7] = {0,0,0,0,0,0,0};        // 进位位，sel0来自于旋变对前半圈和后半圈的判断
unsigned int turn[7] = {0,0,0,0,0,0,0};       // 7个编码轮的圈数
unsigned int MTDAT0[7] = {0,0,0,0,0,0,0};     // MTDAT0
unsigned int MTDAT1[7] = {0,0,0,0,0,0,0};     // MTDAT1
unsigned int MTDAT2[7] = {0,0,0,0,0,0,0};     // MTDAT2
unsigned int D1[7] = {0,0,0,0,0,0,0};         // D1-Gray
unsigned int D2[7] = {0,0,0,0,0,0,0};         // D2-Gray
unsigned int V2[7] = {0,0,0,0,0,0,0};         // V2-Gray


//===========================================================================
// delayus.
//===========================================================================
void delayus ( unsigned long num )
{
	static unsigned long k = 0;
	for(k = num; k > 0; k--)
	{
		asm("NOP");
	}
}
//===========================================================================
// read_absolute_encoder.
//===========================================================================
void read_absolute_encoder(void)
{
	switch( num_teeth)
	{
	      case 0:
	    	  SET_MTMUX0_1;
	    	  SET_MTMUX1_0;
	    	  SET_MTMUX2_0;
	    	  break;

	      case 1:
	    	  MTDAT0[0] = DATA_MTDAT0;
	    	  MTDAT1[0] = DATA_MTDAT1;
	    	  MTDAT2[0] = DATA_MTDAT2;
	    	  SET_MTMUX0_0;
	    	  SET_MTMUX1_1;
	    	  SET_MTMUX2_0;
	    	  break;

	      case 2:
	    	  MTDAT0[1] = DATA_MTDAT0;
	    	  MTDAT1[1] = DATA_MTDAT1;
	    	  MTDAT2[1] = DATA_MTDAT2;
	    	  SET_MTMUX0_1;
	    	  SET_MTMUX1_1;
	    	  SET_MTMUX2_0;
	    	  break;

	      case 3:
	    	  MTDAT0[2] = DATA_MTDAT0;
	    	  MTDAT1[2] = DATA_MTDAT1;
	    	  MTDAT2[2] = DATA_MTDAT2;
	    	  SET_MTMUX0_0;
	    	  SET_MTMUX1_0;
	    	  SET_MTMUX2_1;
	    	  break;

	      case 4:
	    	  MTDAT0[3] = DATA_MTDAT0;
	    	  MTDAT1[3] = DATA_MTDAT1;
	    	  MTDAT2[3] = DATA_MTDAT2;
	    	  SET_MTMUX0_1;
	    	  SET_MTMUX1_0;
	    	  SET_MTMUX2_1;
	    	  break;

	      case 5:
	    	  MTDAT0[4] = DATA_MTDAT0;
	    	  MTDAT1[4] = DATA_MTDAT1;
	    	  MTDAT2[4] = DATA_MTDAT2;
	    	  SET_MTMUX0_0;
	    	  SET_MTMUX1_1;
	    	  SET_MTMUX2_1;
	    	  break;

	      case 6:
	    	  MTDAT0[5] = DATA_MTDAT0;
	    	  MTDAT1[5] = DATA_MTDAT1;
	    	  MTDAT2[5] = DATA_MTDAT2;
	    	  SET_MTMUX0_1;
	    	  SET_MTMUX1_1;
	    	  SET_MTMUX2_1;
	    	  break;

	      case 7:
	    	  MTDAT0[6] = DATA_MTDAT0;
	    	  MTDAT1[6] = DATA_MTDAT1;
	    	  MTDAT2[6] = DATA_MTDAT2;
	    	  break;

		  default:
			  break;
	}

/*
//1
	  SET_MTMUX0_1;
	  SET_MTMUX1_0;
	  SET_MTMUX2_0;

	  delayus(740);
	  MTDAT0[0] = DATA_MTDAT0;
	  MTDAT1[0] = DATA_MTDAT1;
	  MTDAT2[0] = DATA_MTDAT2;
//2
	  SET_MTMUX0_0;
	  SET_MTMUX1_1;
	  SET_MTMUX2_0;

	  delayus(740);
	  MTDAT0[1] = DATA_MTDAT0;
	  MTDAT1[1] = DATA_MTDAT1;
	  MTDAT2[1] = DATA_MTDAT2;
//3
	  SET_MTMUX0_1;
	  SET_MTMUX1_1;
	  SET_MTMUX2_0;

	  delayus(740);
	  MTDAT0[2] = DATA_MTDAT0;
	  MTDAT1[2] = DATA_MTDAT1;
	  MTDAT2[2] = DATA_MTDAT2;
//4
	  SET_MTMUX0_0;
	  SET_MTMUX1_0;
	  SET_MTMUX2_1;

	  delayus(740);
	  MTDAT0[3] = DATA_MTDAT0;
	  MTDAT1[3] = DATA_MTDAT1;
	  MTDAT2[3] = DATA_MTDAT2;
//5
	  SET_MTMUX0_1;
	  SET_MTMUX1_0;
	  SET_MTMUX2_1;

	  delayus(740);
	  MTDAT0[4] = DATA_MTDAT0;
	  MTDAT1[4] = DATA_MTDAT1;
	  MTDAT2[4] = DATA_MTDAT2;
//6
	  SET_MTMUX0_0;
	  SET_MTMUX1_1;
	  SET_MTMUX2_1;

	  delayus(740);
	  MTDAT0[5] = DATA_MTDAT0;
	  MTDAT1[5] = DATA_MTDAT1;
	  MTDAT2[5] = DATA_MTDAT2;
//7
	  SET_MTMUX0_1;
	  SET_MTMUX1_1;
	  SET_MTMUX2_1;

	  delayus(740);
	  MTDAT0[6] = DATA_MTDAT0;
	  MTDAT1[6] = DATA_MTDAT1;
	  MTDAT2[6] = DATA_MTDAT2;
*/
}

//===========================================================================
// get_real_data_absolute_encoder.
//===========================================================================
void get_real_data_absolute_encoder(void)
{
    unsigned int n = 0;
    for (n = 0; n <= 6; n++)
    {

//1
	  if( (sel[n] == 0)   &&   (((MTDAT0[n] == 0) && (MTDAT1[n] == 0) && (MTDAT2[n] == 0)) || ((MTDAT0[n] == 1) && (MTDAT1[n] == 0) && (MTDAT2[n] == 0))) )
	  {
			    D1[n] = 0;
			    D2[n] = 0;
			    V2[n] = 0;
			    sel[n+1] = 0;
	  }
	  if( (sel[n] == 1)   &&   (((MTDAT0[n] == 1) && (MTDAT1[n] == 0) && (MTDAT2[n] == 0)) || ((MTDAT0[n] == 1) && (MTDAT1[n] == 1) && (MTDAT2[n] == 0))) )
	  {
	  			D1[n] = 1;
	  			D2[n] = 0;
	  			V2[n] = 0;
	  			sel[n+1] = 0;
	  }
//2
	  if( (sel[n] == 0)   &&   (((MTDAT0[n] == 1) && (MTDAT1[n] == 1) && (MTDAT2[n] == 0)) || ((MTDAT0[n] == 0) && (MTDAT1[n] == 1) && (MTDAT2[n] == 0))) )
	  {
	  			D1[n] = 1;
	  			D2[n] = 1;
	  			V2[n] = 0;
	  			sel[n+1] = 0;
	  }
	  if( (sel[n] == 1)   &&   (((MTDAT0[n] == 0) && (MTDAT1[n] == 1) && (MTDAT2[n] == 0)) || ((MTDAT0[n] == 0) && (MTDAT1[n] == 1) && (MTDAT2[n] == 1))) )
	  {
	  	  		D1[n] = 0;
	  	  		D2[n] = 1;
	  	  		V2[n] = 0;
	  	  		sel[n+1] = 0;
	  }
//3
	  if( (sel[n] == 0)   &&   (((MTDAT0[n] == 0) && (MTDAT1[n] == 1) && (MTDAT2[n] == 1)) || ((MTDAT0[n] == 1) && (MTDAT1[n] == 1) && (MTDAT2[n] == 1))) )
	  {
	  			D1[n] = 0;
	  			D2[n] = 1;
	  			V2[n] = 1;
	  			sel[n+1] = 1;
	  }
	  if( (sel[n] == 1)   &&   (((MTDAT0[n] == 1) && (MTDAT1[n] == 1) && (MTDAT2[n] == 1)) || ((MTDAT0[n] == 1) && (MTDAT1[n] == 0) && (MTDAT2[n] == 1))) )
	  {
	  		  	D1[n] = 1;
	  		  	D2[n] = 1;
	  		  	V2[n] = 1;
	  		  	sel[n+1] = 1;
	  }
//4
	  if( (sel[n] == 0)   &&   (((MTDAT0[n] == 1) && (MTDAT1[n] == 0) && (MTDAT2[n] == 1)) || ((MTDAT0[n] == 0) && (MTDAT1[n] == 0) && (MTDAT2[n] == 1))) )
	  {
	  			D1[n] = 1;
	  			D2[n] = 0;
	  			V2[n] = 1;
	  			sel[n+1] = 1;
	  }
	  if( (sel[n] == 1)   &&   (((MTDAT0[n] == 0) && (MTDAT1[n] == 0) && (MTDAT2[n] == 1)) || ((MTDAT0[n] == 0) && (MTDAT1[n] == 0) && (MTDAT2[n] == 0))) )
	  {
	  			D1[n] = 0;
	  			D2[n] = 0;
	  			V2[n] = 1;
	  			sel[n+1] = 1;
	  }


    }

}




//===========================================================================
// analyze_absolute_encoder.
//===========================================================================

void analyze_absolute_encoder(void)
{
	unsigned int m = 0;
	for (m = 0; m <= 6; m++)
	{
	      if( ((D1[m] == 0) && (D2[m] == 0) && (V2[m] == 0) || (D1[m] == 1) && (D2[m] == 0) && (V2[m] == 0)) )
	      {
	    	         turn[m] = 0;
	      }
	      if( ((D1[m] == 1) && (D2[m] == 1) && (V2[m] == 0) || (D1[m] == 0) && (D2[m] == 1) && (V2[m] == 0)) )
	      {
	    	         turn[m] = 1;
	      }
	      if( ((D1[m] == 0) && (D2[m] == 1) && (V2[m] == 1) || (D1[m] == 1) && (D2[m] == 1) && (V2[m] == 1)) )
	      {
	    	         turn[m] = 2;
	      }
	      if( ((D1[m] == 1) && (D2[m] == 0) && (V2[m] == 1) || (D1[m] == 0) && (D2[m] == 0) && (V2[m] == 1)) )
	      {
	    	         turn[m] = 3;
	      }

	}

	num_turns = turn[0] + turn[1] * 4 + turn[2] * 16 + turn[3] * 64 + turn[4] * 256 +turn[5] * 1024 + turn[6] * 4096;
}


//===========================================================================
// End of file.
//===========================================================================
