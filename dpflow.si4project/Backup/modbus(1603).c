#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_sau.h"
#include "device.h"
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_sau.h"
#include "24cxx.h"
#include "device.h"
#include "hum.h"
#include "irc.h"
#include "lora.h"
#include "modbus.h"
#include "string.h"
#include "stdio.h"



static struct 
{ 
	unsigned char  send_buf[128];
	unsigned char  recv_buf[64];
	unsigned char  rec_pos;
	modebs_param_t param;
}
modbusMisc=
{
	{0},
	{0},
	 0,
	 
    {0}
};

static void * const modbus_data_map[]=
{
    &modbusMisc.param.addr,
    &modbusMisc.param.baud,
    &device_comps.calibration_param.dot,
    &device_comps.calibration_param.unit,
    &device_comps.current_press,
    &device_comps.calibration_param.y[3],
    &device_comps.current_temp,
    &device_comps.batt,
    &device_comps.coe.press,
    &device_comps.coe.temp,//9

    &device_comps.deltaP_calibration_param.dot,
    &device_comps.current_deltaP,
    &device_comps.deltaP_calibration_param.y[3],
    &device_comps.coe.deltaP,
    &device_comps.dpf_param.Fd,
    &device_comps.dpf_param.Fu,
    &device_comps.current_flow,
    &device_comps.current_flow,
    &device_comps.coe.flow,
    &device_comps.dpf_param.Td,
    &device_comps.dpf_data.total,
    &device_comps.dpf_data.total
    
};




static int read_modbus_param(void const *buf,int len )
{
     return _24cxx_comps.read(MD_MODBUS_PARAM_START_ADDR,buf,len);
}

static int save_modbus_param(void const *buf,int len )
{
     return _24cxx_comps.write(MD_MODBUS_PARAM_START_ADDR,buf,len);
}


static unsigned int generateCRC(unsigned char *buffer, unsigned int messageLength)
{
	unsigned int crc = 0xFFFF;
	unsigned int crcHigh = 0;
	unsigned int crcLow = 0;
	int i, j = 0;

	for (i = 0;i < messageLength;i++)
	{
		crc ^= buffer[i];
		for (j = 8; j != 0; j--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	
	return crc;
}




static unsigned char Pro_modbus(unsigned char Cmd,unsigned char *buf,int len)
{
	int i=0;
	unsigned int crc;
	unsigned char k=0;
   	int tmp;
	
	unsigned int addr=((unsigned int)buf[2]<<8)+buf[3];
	if(Cmd==0x03)//Read
	{
		unsigned int num=((unsigned int)buf[4]<<8)+buf[5];
		int k;
		if(addr==0x40)//ser_num
		{
            modbusMisc.send_buf[i++]=modbusMisc.param.addr;
            modbusMisc.send_buf[i++]=Cmd;
            modbusMisc.send_buf[i++]=num*2;//length
            memcpy(&modbusMisc.send_buf[i],&device_comps.device_sensor.ser_num[0],sizeof(device_comps.device_sensor.ser_num));
            i+=sizeof(device_comps.device_sensor.ser_num);
            crc=generateCRC(modbusMisc.send_buf, i);
            modbusMisc.send_buf[i++]=crc;
            modbusMisc.send_buf[i++]=crc>>8;
	        modbusComps.write(modbusMisc.send_buf,i);
            return 8;
		}
		else
		{
    		if((addr>0x15)||(num+addr>0x15+1))
    		{
    		    return 1;
    		}
    		else
    		{
                modbusMisc.send_buf[i++]=modbusMisc.param.addr;
                modbusMisc.send_buf[i++]=Cmd;
                modbusMisc.send_buf[i++]=num*2;//length
                for(k=0;k<num;k++)
                {
                    if(addr==0||addr==1||addr==3||addr==7)//addr, baud,measure_unit batt
                    {   
                        modbusMisc.send_buf[i++] =0;
                        modbusMisc.send_buf[i++] =*((unsigned char *)modbus_data_map[addr++]);
                    }
                    else if(addr==2)//dot
                    {
                        modbusMisc.send_buf[i++] =0;
                        modbusMisc.send_buf[i++] =(*((unsigned char *)modbus_data_map[addr++]))-2;
                    }
                    else if(addr==4||addr==5)//press full
                    {
                        modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr])/100>>8;
                        modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr++])/100;
                    }
                    else if(addr==6||addr==8||addr==9)//6 8 9 temp press_coi temp_coi
                    {
                        modbusMisc.send_buf[i++] =*((int*)modbus_data_map[addr])>>8;
                        modbusMisc.send_buf[i++] =*((int*)modbus_data_map[addr++]);
                    }

                    ///////////////////dfp use
                    else if(addr==0x0a)
                    {
                        modbusMisc.send_buf[i++] =0;
                        modbusMisc.send_buf[i++] =(*((unsigned char *)modbus_data_map[addr++]))-2;
                    } 
                    else if(addr==0x0e||addr==0x0f||addr==0x13)
                    {
                        modbusMisc.send_buf[i++] =0;
                        modbusMisc.send_buf[i++] =*((unsigned char *)modbus_data_map[addr++]);
                    }
                    else if(addr==0x0b||addr==0x0c)
                    {
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr])/100>>8;
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr++])/100;
                    }
                     else if(addr==0x0d||addr==0x12)
                    {
                         modbusMisc.send_buf[i++] =*((int*)modbus_data_map[addr])>>8;
                         modbusMisc.send_buf[i++] =*((int*)modbus_data_map[addr++]);
                    }
                    else if(addr==0x10)
                    {
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr])>>24;
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr++])>>16;
                    }
                    else if(addr==0x11)
                    {
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr])>>8;
                         modbusMisc.send_buf[i++] =*((long *)modbus_data_map[addr++]);
                    }
                    else if(addr==0x14)
                    {
                        long total=*((long *)modbus_data_map[addr++])*lpwr(device_comps.dpf_param.Td)+(long)(device_comps.dpf_total_dec_part*lpwr(device_comps.dpf_param.Td));
                        modbusMisc.send_buf[i++] =total>>24;
                        modbusMisc.send_buf[i++] =total>>16;

                    }
                    else if(addr==0x15)
                    {
                        long total=*((long *)modbus_data_map[addr++])*lpwr(device_comps.dpf_param.Td)+(long)(device_comps.dpf_total_dec_part*lpwr(device_comps.dpf_param.Td));
                        modbusMisc.send_buf[i++] =total>>8;
                        modbusMisc.send_buf[i++] =total;

                    }
                    else
                    {
                        return 1;
                    }
                }
                crc=generateCRC(modbusMisc.send_buf, i);
                modbusMisc.send_buf[i++]=crc;
                modbusMisc.send_buf[i++]=crc>>8;
                modbusComps.write(modbusMisc.send_buf,i);
    		    return 8;
             }
         }
    }
	else if(Cmd==6)//Write
	{
                if(addr==0)
                {
                    modbusMisc.send_buf[i++]=buf[5];
                } 
                else
                {
                    modbusMisc.send_buf[i++]=modbusMisc.param.addr;
                }
				modbusMisc.send_buf[i++]=Cmd;
				modbusMisc.send_buf[i++]=buf[2];
				modbusMisc.send_buf[i++]=buf[3];
				modbusMisc.send_buf[i++]=buf[4];
				modbusMisc.send_buf[i++]=buf[5];
	    switch(addr)
	    {
          case 0:                //addr
				modbusMisc.param.addr=buf[5];
                modbusMisc.param.cs=Check_Sum_5A(&modbusMisc.param, &modbusMisc.param.cs-(unsigned char *)&modbusMisc.param);
    			modbusComps.save_param(&modbusMisc.param,sizeof(modbusMisc.param));
    			break;
                
		  case 1:                //baud
		        tmp=buf[5];
		        if(tmp>0&&tmp<5)
		        {
    				modbusMisc.param.baud=tmp;
    				
                    modbusMisc.param.cs=Check_Sum_5A(&modbusMisc.param, &modbusMisc.param.cs-(unsigned char *)&modbusMisc.param);
        			modbusComps.save_param(&modbusMisc.param,sizeof(modbusMisc.param));
        			modbusComps.sw._bit.baud_modified=1;
        		}
				break;		
		   case 8:                //press_coe
		        tmp=((unsigned int)buf[4]<<8)+buf[5];
		        if(tmp>=7000&&tmp<=13000)
		        {
                	device_comps.coe.press=tmp;
                	device_comps.coe.cs=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe);
        			device_comps.save_coe(&device_comps.coe,sizeof(device_comps.coe));
        		}
                break;
				
		   case 9:                //temp_coe
		        tmp=((unsigned int)buf[4]<<8)+buf[5];
		        if(tmp>=7000&&tmp<=13000)
		        {
                     device_comps.coe.temp=tmp;
                     device_comps.coe.cs=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe);
        			 device_comps.save_coe(&device_comps.coe,sizeof(device_comps.coe));
    			}
                 break;

                 
          case 0x0d:                //deltaP_coe
		        tmp=((unsigned int)buf[4]<<8)+buf[5];
		        if(tmp>=7000&&tmp<=13000)
		        {
                	device_comps.coe.deltaP=tmp;
                	device_comps.coe.cs=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe);
        			device_comps.save_coe(&device_comps.coe,sizeof(device_comps.coe));
        		}
                break;
				
		   case 0x12:                //flow_coe
		        tmp=((unsigned int)buf[4]<<8)+buf[5];
		        if(tmp>=7000&&tmp<=13000)
		        {
                     device_comps.coe.flow=tmp;
                     device_comps.coe.cs=Check_Sum_5A(&device_comps.coe, &device_comps.coe.cs-(unsigned char *)&device_comps.coe);
        			 device_comps.save_coe(&device_comps.coe,sizeof(device_comps.coe));
    			}
                 break;
		   default:       
					return 1;	
		}
		crc=generateCRC(modbusMisc.send_buf, i);
		modbusMisc.send_buf[i++]=crc;
		modbusMisc.send_buf[i++]=crc>>8;
		modbusComps.write(modbusMisc.send_buf,i);
		return 8;
		
	}
	else if(Cmd==0x10)//write ser_num
	{
        memcpy(device_comps.device_sensor.ser_num,&buf[7],8);
    	device_comps.device_sensor.cs=Check_Sum_5A(&device_comps.device_sensor.ser_num, &device_comps.device_sensor.cs-(unsigned char *)&device_comps.device_sensor.ser_num);
    	device_comps.save_device_sensor(&device_comps.device_sensor.ser_num,sizeof(device_comps.device_sensor));
        modbusMisc.send_buf[i++]=modbusMisc.param.addr;
        modbusMisc.send_buf[i++]=Cmd;
        modbusMisc.send_buf[i++]=buf[2];
        modbusMisc.send_buf[i++]=buf[3];
        modbusMisc.send_buf[i++]=buf[4];
        modbusMisc.send_buf[i++]=buf[5];
        crc=generateCRC(modbusMisc.send_buf, i);
		modbusMisc.send_buf[i++]=crc;
		modbusMisc.send_buf[i++]=crc>>8;
		modbusComps.write(modbusMisc.send_buf,i);
		return len;
	}
	else 
	{
		return 1;
	}
	return 1;
	
}
static unsigned char Check_modbus_Com(unsigned char *Rec_Data,unsigned char Rec_Pos)
{
    int len;
	if(modbusComps.sw._bit.busy)
	{
	    return 0;
	}
	if(Rec_Pos<2)
	{
		return 0;
	}
	if(Rec_Data[0]!=modbusMisc.param.addr&&Rec_Data[0]!=0)
	{
		return 1;
	}
	if(Rec_Data[1]!=0x03&&Rec_Data[1]!=0x06&&Rec_Data[1]!=0x10)
	{
		return 1;
	}
	if(Rec_Pos<8)
	{
		return 0;
	}
	if(Rec_Data[1]==0x10)
	{
        if(Rec_Data[6]!=8)
        {
            return 1;
        }
        if(Rec_Pos<8+9) 
        {
            return 0;
        }
        len=17;
	}
	else 
	{
	  len=8;
	}
	if(((unsigned int)Rec_Data[len-1]<<8)+Rec_Data[len-2]!=generateCRC(Rec_Data,len-2))
	{
		return 1;
	}
	return Pro_modbus(Rec_Data[1],Rec_Data,len);
}


static void Deal_modbus(void)
{
	unsigned char err=0;
	do
	{
	    err=Check_modbus_Com(modbusMisc.recv_buf,modbusMisc.rec_pos); 
		if(err>0)
		{
		    DI();
			memcpy(modbusMisc.recv_buf,modbusMisc.recv_buf+err,modbusMisc.rec_pos-err);
			modbusMisc.rec_pos-=err;
			EI();
		}
	}
    while (err>0);
	
}

static void  modbusComps_task_handle(void)
{
    static int init_once=1;
    if(init_once)
    {
        modbusComps.read_param(&modbusMisc.param,sizeof(modbusMisc.param));
        if(modbusMisc.param.cs!=Check_Sum_5A(&modbusMisc.param, &modbusMisc.param.cs-(unsigned char *)&modbusMisc.param))
        {
            modbusMisc.param.baud=3;
        }
        if(modbusMisc.param.baud==0||modbusMisc.param.baud>4)
        {
            modbusMisc.param.baud=3;
        }
       #ifndef MD_MODBUS
        modbusComps.sw._bit.runing=0;
       #else
	     modbusComps.sw._bit.runing=1;
         modbusComps.modify_baud(modbusMisc.param.baud);
         enable_modbus_receive();
	    #endif  
        init_once=0;
    }
   if(modbusComps.sw._bit.runing)
   {
        Deal_modbus();
   }
}

static void write_modbus(unsigned char * const buf,unsigned int len)
{
    if(len>0)
    {
        enable_modbus_send();
        disable_modbus_receive();  
        modbusComps.sw._bit.busy=1;
        R_UART0_Send(buf,len);
    }
}

modbusComps_t modbusComps=
{
    modbusMisc.recv_buf,
    &modbusMisc.rec_pos,
    30,
    &modbusMisc.param,
    {0},

    read_modbus_param,
    save_modbus_param,
    modbus_modify_baud,
    write_modbus,
    modbusComps_task_handle
};




