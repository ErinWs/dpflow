
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_adc.h"
#include "r_cg_sau.h"
#include "hum.h"
#include "device.h"
#include "string.h"
#include "pcf857x.h"

static int pcf857x_init(pcf857x_comps_t *this)
{
     
}



static int pcf857x_read(pcf857x_comps_t *this)
{
    unsigned char w_wait,SlaveAddr;	
	SlaveAddr = 0x40;
	for(w_wait=0; w_wait<64; w_wait++)
	{
		I2CStart();
		if(MD_I2C_STATUS_TRUE == I2CByteTX(SlaveAddr))
		{
		    break;
		}
		I2CStop();
		I2CStop();
		DelayMs(1);
	}	
	if(w_wait >= 64) 
	{
	    MD_I2C_RETURN_FALSE;
	}
	this->in_data= I2CByteRX();
	MD_I2C_RETURN_TRUE;
}


static int pcf857x_write(pcf857x_comps_t *this)
{
    unsigned char w_wait,SlaveAddr;	
	SlaveAddr = 0x40;
	for(w_wait=0; w_wait<64; w_wait++)
	{
		I2CStart();
		if(MD_I2C_STATUS_TRUE == I2CByteTX(SlaveAddr))
		{
		    break;
		}
		I2CStop();
		I2CStop();
		DelayMs(1);
	}	
	if(w_wait >= 64) 
	{
	    MD_I2C_RETURN_FALSE;
	}
	if(MD_I2C_STATUS_FALSE == I2CByteTX(this->out_data)) 
	{
	    MD_I2C_RETURN_FALSE; 
	}
	MD_I2C_RETURN_TRUE;
}


pcf857x_comps_t pcf857x_comps=
{
   	 "",
	&pcf857x_comps,
    0,
    0,
    pcf857x_init,
    pcf857x_read,
    pcf857x_write
  
};


