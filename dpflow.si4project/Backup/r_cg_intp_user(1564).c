/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_intp_user.c
* Version      : Applilet4 for R7F0C003/004 V1.00.00.01 [01 Mar 2016]
* Device(s)    : R7F0C004M
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTP module.
* Creation Date: 2020/6/30 星期�?
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_sau.h"
#include "lora.h"
#include "cs123x.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_intc0_interrupt(vect=INTP0)
#pragma interrupt r_intc1_interrupt(vect=INTP1)
#pragma interrupt r_intc2_interrupt(vect=INTP2)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	 if(!MD_LORA_AUX_PIN) 
    {
        if(loraComps.work_st.dir==EM_RECV)
        {
            loraComps.sw._bit.runing=1;
            loraComps.op_window_time=6;
            enable_lora();
            MD_LORA_INTP_DISABLE();
        }
     //   else if(loraComps.work_st.dir==EM_SEND)
     //   {
      //      loraComps.sw._bit.runing=0;
      //      disable_lora();
      //  }
    }
    //else
    //{
        
      //  if(loraComps.work_st.dir==EM_RECV)
      //  {
     //      
     //   }
     //   else if(loraComps.work_st.dir==EM_SEND)
     //   {
      //      loraComps.op_window_time=0;
       //     enter_cad_mode();
      //  }
    //}
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_intc1_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    cs1237_comps.sw._bit.adc_updated=1;
    cs1237_comps.disale_interrup();
    cs1237_comps.task_handle();
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_intc2_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    cs123x_comps.sw._bit.adc_updated=1;
    cs123x_comps.disale_interrup();
    cs123x_comps.task_handle();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
