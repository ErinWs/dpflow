
#include "r_cg_macrodriver.h"
#include "protocol.h"
#include "r_cg_rtc.h"
#include "device.h"
#include "string.h"
#include "math.h"
#include "net.h"
#include "irc.h"
#include "hum.h"
#include "24cxx.h"
#include "stdio.h"
#include "stdlib.h"


//#define static   
#define                     MD_SET_REPORT_TIME_MSGID              40
#define                     MD_GET_REPORT_TIME_MSGID              41
#define                     MD_SET_PALARM_THRESHOLD_MSGID         42
#define                     MD_GET_PALARM_THRESHOLD_MSGID         43  
#define                     MD_SET_TALARM_THRESHOLD_MSGID         44
#define                     MD_GET_TALARM_THRESHOLD_MSGID         45
#define                     MD_GET_DEVICE_INFO_MSGID              46
#define                     MD_SET_ACCESS_ADDR_MSGID              47 
#define                     MD_GET_ACCESS_ADDR_MSGID              48
#define                     MD_SET_DEVICE_OFFLINE_MSGID           49



static struct 
{ 
	unsigned char buf[312];
	unsigned char step;
	int event_index;
	int reRryTimes;
	 
	
}
protocolMisc=
{
	"",
	0 ,
	-1,
	3
};


//////return  lenght of Rcvbuf
static unsigned short crc_16(unsigned char const*puchMsg, unsigned char *Rcvbuf ,unsigned int usDataLen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x1021;
	unsigned short wChar = 0;
	int i=0;

	while (usDataLen--) 	
	{
	wChar = *(puchMsg++);
	wCRCin ^= (wChar << 8);
	for(i = 0;i < 8;i++)
	{
	  if(wCRCin & 0x8000)
	    wCRCin = (wCRCin << 1) ^ wCPoly;
	  else
	    wCRCin = wCRCin << 1;
	}
	}
	Rcvbuf[0] = (wCRCin & 0xff00)>>8;//??????
	Rcvbuf[1] = (wCRCin& 0x00ff);  //ж╠ик????
	return 2;
}

//return Return crc check result 0 ok 1,err
//static unsigned short check_crc_16(unsigned char const *Array, unsigned int crc_16,unsigned int Len)
//{
       
static unsigned short check_crc_16(unsigned char const*puchMsg, unsigned int crc_16,unsigned int usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x1021;
  unsigned short wChar = 0;
  int i=0;
  
  while (usDataLen--) 	
  {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for(i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  return (wCRCin!=crc_16) ;
}


static unsigned char AddAddr(unsigned char *buf)
{
	unsigned char i=7;
    DataAdd30H(buf,&device_comps.device_addr.addr[0],i);
    return i=i*2;
}
static unsigned char  AddTp(unsigned char *buf)
{
	unsigned char i=7;
	unsigned char temp[7];
	temp[0]=0x20;
	temp[1]=rtc_valve.year;
	temp[2]=rtc_valve.month;
	temp[3]=rtc_valve.day;
	temp[4]=rtc_valve.hour;
    temp[5]=rtc_valve.min;
	temp[6]=rtc_valve.sec;
    DataAdd30H(buf,temp,i);
    return  i=i*2;
}

static unsigned int AddTimeDataSeg(unsigned char *buf)
{
	unsigned int Addr=0;
	unsigned int StartAddr=0;
	unsigned int Count=0;
	unsigned int m=0;
	long temp;
	Addr=device_comps.TimeSegData.store_addr;
	if((Addr-MD_TIME_SEG_DATA_START_ADDR)/4>=48)
	{
		_24cxx_comps.read(Addr-(unsigned int)48*4,buf,(unsigned int)48*4);
	}
	else
	{
		StartAddr=MD_TIME_SEG_DATA_END_ADDR-((48-(Addr-MD_TIME_SEG_DATA_START_ADDR)/4)*4);
		Count=(48-(Addr-MD_TIME_SEG_DATA_START_ADDR)/4)*4;
		_24cxx_comps.read(StartAddr,buf,Count);
		_24cxx_comps.read(MD_TIME_SEG_DATA_START_ADDR,buf+Count,(Addr-MD_TIME_SEG_DATA_START_ADDR)/4*4);
		
	}
	for(m=0;m<48;m++)
	{
        temp=formatData4fixDot(  *((long *)&buf[m*4]) );
        buf[m*4+0]=temp>>24;
        buf[m*4+1]=temp>>16;
        buf[m*4+2]=temp>>8;
        buf[m*4+3]=temp;
	}
	return (unsigned int)48*4;
}
unsigned int AddLastSamplTps(unsigned char *buf)
{
	unsigned char i=7;
    DataAdd30H(buf,&device_comps.TimeSegData.lastSampleTime[0],i);           
	return i=i*2;
}

static unsigned char VerifyMeterId(unsigned char const *buf, unsigned char const *meterId)
{
    unsigned char temp[14];
    DataAdd30H(temp, meterId,7);
    return memcmp(buf,temp,14);
}

static int AddTriggerReportCode(unsigned char *const buf,int event)
{
	int i=0;
	static unsigned char const event_table[][4]=//Triggering event report table E.2
	{
		{0x80,0x01,0x00,0x06},//low voltage
		{0x80,0x01,0x00,0x07},
		{0x80,0x01,0x00,0x09},
		{0x80,0x01,0x00,0x10},
		{0x80,0x01,0x00,0x11},
		{0x80,0x01,0x00,0x12},
		{0x80,0x01,0x00,0x13},
		{0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x01}
	};
	buf[i++]=event_table[event][0];//Trigger report event code 1
	buf[i++]=event_table[event][1];//Trigger report event code 2
	buf[i++]=event_table[event][2];//Trigger report event code 3
	buf[i++]=event_table[event][3];//Trigger report event code 4
	return i;
}

static int Encapsulate_dataPush_package(unsigned char *const buf,int event)//6.4.3
{
    int i=0;
    unsigned long temp=0;
    buf[i++]=0;//mesID
    i+=AddAddr(&buf[i]);//meter id

    temp=formatData4fixDot(device_comps.calibration_param.y[3]);
    buf[i++]=temp>>24;
    buf[i++]=temp>>16;
    buf[i++]=temp>>8;
    buf[i++]=temp;
                                             //current p
    temp=formatData4fixDot(device_comps.current_press);                                         
    buf[i++]=temp>>24;
    buf[i++]=temp>>16;
    buf[i++]=temp>>8;
    buf[i++]=temp;
    
    buf[i++]=(device_comps.calibration_param.unit&0x0f);//param unit

    buf[i++]=device_comps.current_temp>>8;
    buf[i++]=device_comps.current_temp;

    buf[i++]=netComps.net_info.sina;
    buf[i++]=netComps.net_info.rsrp;
    buf[i++]=netComps.net_info.Csq;

    buf[i++]=netComps.net_info.pci>>8;
    buf[i++]=netComps.net_info.pci;

    buf[i++]=netComps.net_info.earFcn>>24;
    buf[i++]=netComps.net_info.earFcn>>16;
    buf[i++]=netComps.net_info.earFcn>>8;
    buf[i++]=netComps.net_info.earFcn;

    buf[i++]=netComps.net_info.ecl;
    buf[i++]=device_comps.sw._bit.batt_status;
    buf[i++]=device_comps.batt;
    buf[i++]=device_comps.sw._bit.data_mode;
    i+=AddTp(&buf[i]);

    buf[i++]=netComps.net_info.cellId>>24;
    buf[i++]=netComps.net_info.cellId>>16;
    buf[i++]=netComps.net_info.cellId>>8;
    buf[i++]=netComps.net_info.cellId;
    buf[i++]=event;

    i+=AddTimeDataSeg(&buf[i]);
    i+=AddLastSamplTps(&buf[i]);
    
    return i;
}



static int Analysis_downstream_package(unsigned char *const buf,unsigned int len,unsigned char *address_field)
{
        *address_field=buf[0];
        if((buf[0]==0xaa)&&(buf[1]==0xaa)&&(buf[2]==0x00)&&(buf[3]==0x00))
        {
            return 0;
        }
        else if(buf[0]>=40&&buf[0]<=49)
        {
            return 0;
        }
        else 
    	{
    		return 1;
    	}
}

static int  AddMethodName(unsigned char name,unsigned char *buf)
 {      
	unsigned char i=0;
	switch(name)
	{
        case MD_SET_REPORT_TIME_MSGID: 
            buf[i++]=strlen("setReportTime");//
            memcpy(&buf[i],"setReportTime",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_GET_REPORT_TIME_MSGID: 
            buf[i++]=strlen("getReportTime");//
            memcpy(&buf[i],"getReportTime",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_SET_PALARM_THRESHOLD_MSGID:
            buf[i++]=strlen("setPAlarmThreshold");//
            memcpy(&buf[i],"setPAlarmThreshold",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_GET_PALARM_THRESHOLD_MSGID: 
            buf[i++]=strlen("getPAlarmThreshold");//
            memcpy(&buf[i],"getPAlarmThreshold",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_SET_TALARM_THRESHOLD_MSGID:
            buf[i++]=strlen("getTAlarmThreshold");//
            memcpy(&buf[i],"getTAlarmThreshold",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_GET_TALARM_THRESHOLD_MSGID: 
            buf[i++]=strlen("getTAlarmThreshold");//
            memcpy(&buf[i],"getTAlarmThreshold",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_GET_DEVICE_INFO_MSGID:
            buf[i++]=strlen("getDeviceInfo");//
            memcpy(&buf[i],"getDeviceInfo",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_SET_ACCESS_ADDR_MSGID:
         buf[i++]=strlen("setAccessAddr");//
            memcpy(&buf[i],"setAccessAddr",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_GET_ACCESS_ADDR_MSGID: 
           buf[i++]=strlen("getAccessAddr");//
            memcpy(&buf[i],"getAccessAddr",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
        case MD_SET_DEVICE_OFFLINE_MSGID:  
		    buf[i++]=strlen("setDeviceOffline");//
            memcpy(&buf[i],"setDeviceOffline",buf[i-1]);//
            i+=buf[i-1]; 
            return i;
		default:
		        buf[i++]=0;//len
			return i;
	}
	return i;
 }
static void dealAckData(unsigned char address_fiel,unsigned int midId)
{
   
    unsigned int  i=0;
	memset(protocolMisc.buf,0,sizeof(protocolMisc.buf));
	protocolMisc.buf[i++]=address_fiel+0x20;//The same msgID
	i+=AddMethodName(address_fiel,&protocolMisc.buf[i]);
	protocolMisc.buf[i++]=0;//errCode
	switch(address_fiel)
	{
        case  MD_SET_ACCESS_ADDR_MSGID://
        case  MD_SET_REPORT_TIME_MSGID :   
        case  MD_SET_DEVICE_OFFLINE_MSGID:
        case  MD_SET_PALARM_THRESHOLD_MSGID:
        case  MD_SET_TALARM_THRESHOLD_MSGID:
              break;
        case  MD_GET_REPORT_TIME_MSGID:
            protocolMisc.buf[i++]=device_comps.report_param.min;
            protocolMisc.buf[i++]=device_comps.report_param.hour;
            protocolMisc.buf[i++]=device_comps.report_param.hour_Interval;
            protocolMisc.buf[i++]=device_comps.report_param.disFactor>>8;
            protocolMisc.buf[i++]=device_comps.report_param.disFactor;
            break;

        case  MD_GET_PALARM_THRESHOLD_MSGID:
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_upper>>24;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_upper>>16;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_upper>>8;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_upper;

            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_lower>>24;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_lower>>16;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_lower>>8;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_high_lower;

            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_upper>>24;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_upper>>16;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_upper>>8;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_upper;

            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_lower>>24;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_lower>>16;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_lower>>8;
            protocolMisc.buf[i++]=device_comps.alarm_param.press_low_lower;

            protocolMisc.buf[i++]=device_comps.alarm_param.unit;
            break;
        case  MD_GET_TALARM_THRESHOLD_MSGID:
            protocolMisc.buf[i++]=device_comps.alarm_param.temp_high>>8;
			protocolMisc.buf[i++]=device_comps.alarm_param.temp_high;
			protocolMisc.buf[i++]=device_comps.alarm_param.temp_low>>8;
			protocolMisc.buf[i++]=device_comps.alarm_param.temp_low;
            break;

        case  MD_GET_DEVICE_INFO_MSGID:
            i+=AddAddr(&protocolMisc.buf[i]);
            protocolMisc.buf[i++]=MD_FL_VER;//FL_VER hdware ver
            protocolMisc.buf[i++]=MD_FL_VER ;//FL_VER swware ver

            protocolMisc.buf[i++]=strlen(netComps.net_info.firmhdVer)>sizeof(netComps.net_info.firmhdVer)?sizeof(netComps.net_info.firmhdVer):strlen(netComps.net_info.firmhdVer);//moudle swver len
            memcpy(&protocolMisc.buf[i],netComps.net_info.firmhdVer,protocolMisc.buf[i-1]);//r
            i+=protocolMisc.buf[i-1];

            protocolMisc.buf[i++]=strlen(netComps.net_info.firmVer)>sizeof(netComps.net_info.firmVer)?sizeof(netComps.net_info.firmVer):strlen(netComps.net_info.firmVer);//moudle swver len
            memcpy(&protocolMisc.buf[i],netComps.net_info.firmVer,protocolMisc.buf[i-1]);//
            i+=protocolMisc.buf[i-1];

            protocolMisc.buf[i++]=strlen(netComps.net_info.imei)>sizeof(netComps.net_info.imei)?sizeof(netComps.net_info.imei):strlen(netComps.net_info.imei);//moudle swver len
            memcpy(&protocolMisc.buf[i],netComps.net_info.imei,protocolMisc.buf[i-1]);//
            i+=protocolMisc.buf[i-1];

            protocolMisc.buf[i++]=strlen(netComps.net_info.iccid)>sizeof(netComps.net_info.iccid)?sizeof(netComps.net_info.iccid):strlen(netComps.net_info.iccid);//moudle swver len
            memcpy(&protocolMisc.buf[i],netComps.net_info.iccid,protocolMisc.buf[i-1]);//
            i+=protocolMisc.buf[i-1];

            protocolMisc.buf[i++]=strlen(netComps.net_info.imsi)>sizeof(netComps.net_info.imsi)?sizeof(netComps.net_info.imsi):strlen(netComps.net_info.imsi);//moudle swver len
            memcpy(&protocolMisc.buf[i],netComps.net_info.imsi,protocolMisc.buf[i-1]);//
            i+=protocolMisc.buf[i-1];
            break;
        case  MD_GET_ACCESS_ADDR_MSGID:
            {
                char IpNum[25]="";
                
                sprintf(IpNum+strlen(IpNum),"%d.",device_comps.access_param.ip[0]);
                sprintf(IpNum+strlen(IpNum),"%d.",device_comps.access_param.ip[1]);
                sprintf(IpNum+strlen(IpNum),"%d.",device_comps.access_param.ip[2]);
                sprintf(IpNum+strlen(IpNum),"%d,",device_comps.access_param.ip[3]);
                sprintf(IpNum+strlen(IpNum),"%u",device_comps.access_param.port);
                protocolMisc.buf[i++]=strlen(IpNum)>sizeof(IpNum)?sizeof(IpNum):strlen(IpNum);//
                memcpy(&protocolMisc.buf[i],IpNum,protocolMisc.buf[i-1]);//New IP
                i+=protocolMisc.buf[i-1];
            }
            
            break;
       default:
		          break;
	}
    protocolMisc.buf[i++]=midId>>8;//The same MIDidH
	protocolMisc.buf[i++]=midId;//The same MIDidL
	
	protocolComps.msg;//=protocolMisc.buf;
	protocolComps.msgLen=i;
	protocolComps.sw._bit.DataRdy=1;
}

static int  DealDownCmd(unsigned char const *buf,unsigned int Len)
{
        unsigned int   i=15;
        unsigned int   err=1;
       
        switch(buf[0])
	    {
	            case  MD_SET_ACCESS_ADDR_MSGID://
    			  	if(!VerifyMeterId(&buf[1],&device_comps.device_addr.addr[0]))
    				{
    					char *ptsz;
    					unsigned int port=0;
    					device_comps.access_param.ip[0]=strtol(&buf[i],&ptsz,10);
    					device_comps.access_param.ip[1]= strtol(ptsz+1,&ptsz,10);
    					device_comps.access_param.ip[2]= strtol(ptsz+1,&ptsz,10);
    					device_comps.access_param.ip[3]= strtol(ptsz+1,&ptsz,10);
    					port= strtol(ptsz+1,(char **)0,10);//////////////////////bug
    					device_comps.access_param.port=port;
    					device_comps.access_param.cs=Check_Sum_5A(&device_comps.access_param, &device_comps.access_param.cs-(unsigned char *)&device_comps.access_param);
    					device_comps.save_access_param(&device_comps.access_param,sizeof(device_comps.access_param));
    				}
    				else 
    				{
    					err=0;
    				}
				break;
				
		        case  MD_SET_REPORT_TIME_MSGID :   
			        if(!VerifyMeterId(&buf[1],&device_comps.device_addr.addr[0]))
				    {
                        unsigned int tmpdat=0;
                        device_comps.report_param.min=buf[i++];//min
                        device_comps.report_param.hour=buf[i++];//hour
                        device_comps.report_param.hour_Interval=buf[i++];//report intertime
                        tmpdat=buf[i++];
                        tmpdat=(tmpdat<<8)+buf[i++];
                        if((tmpdat>59)&&(tmpdat<601))
                        {
                            device_comps.report_param.disFactor=tmpdat;
                        }
                       
                        device_comps.report_interval_timer=0;
                        device_comps.report_param.cs=Check_Sum_5A(&device_comps.report_param, &device_comps.report_param.cs-(unsigned char*)&device_comps.report_param);
    				    device_comps.save_report_param(&device_comps.report_param,sizeof(device_comps.report_param));
				    }
				else 
				{
					err=0;
				}
				break;
			case  MD_SET_DEVICE_OFFLINE_MSGID:
			      if(!buf[i])//0: has no cache cmd  ,
			      {
				     protocolMisc.step++;
				     protocolComps.sw._bit.cmd_shutDown=1;
				     protocolComps.sw._bit.dataPushYet=1;
				     
			      }
			      else
			      {
				       // CurrentReportHasCacheCmd=1;   
			      }
			      break;

            case  MD_SET_PALARM_THRESHOLD_MSGID:
                    if(!VerifyMeterId(&buf[1],&device_comps.device_addr.addr[0]))
				    {
                        //memcpy(&device_comps.alarm_param.press_high_upper,&buf[i],4*4);
    				   // device_comps.alarm_param.unit=buf[i+=4*4];
    				    //i++;
    				    device_comps.alarm_param.press_high_upper=((unsigned long)buf[i   ]<<24)+((unsigned long)buf[i+ 1]<<16)+((unsigned long)buf[i+2 ]<<8)+buf[i+3];
                        device_comps.alarm_param.press_high_lower=((unsigned long)buf[i+ 4]<<24)+((unsigned long)buf[i+ 5]<<16)+((unsigned long)buf[i+6 ]<<8)+buf[i+7];
                        device_comps.alarm_param.press_low_upper =((unsigned long)buf[i+ 8]<<24)+((unsigned long)buf[i+ 9]<<16)+((unsigned long)buf[i+10]<<8)+buf[i+11];
                        device_comps.alarm_param.press_low_lower =((unsigned long)buf[i+12]<<24)+((unsigned long)buf[i+13]<<16)+((unsigned long)buf[i+14]<<8)+buf[i+15];
                        device_comps.alarm_param.unit=buf[i+=16];
                        i++;
                        device_comps.alarm_param.cs=Check_Sum_5A(&device_comps.alarm_param, &device_comps.alarm_param.cs-(unsigned char)&device_comps.alarm_param);
    				    device_comps.save_alarm_param(&device_comps.alarm_param,sizeof(device_comps.alarm_param));
				    }
    				else 
    				{
    					err=0;
    				}
    				break;
                  
    				
            case  MD_SET_TALARM_THRESHOLD_MSGID:
                    if(!VerifyMeterId(&buf[1],&device_comps.device_addr.addr[0]))
                    {
                     //memcpy(&alarm_param_cpy.temp_high,&buf[13+4*4+1],2*2);
                        device_comps.alarm_param.temp_high =((unsigned int)buf[i]<<8)+buf[i+1];
                        device_comps.alarm_param.temp_low =((unsigned int)buf[i+2]<<8)+buf[i+3];
                        i+=4;
                        device_comps.alarm_param.cs=Check_Sum_5A(&device_comps.alarm_param, &device_comps.alarm_param.cs-(unsigned char *)&device_comps.alarm_param);
                        device_comps.save_alarm_param(&device_comps.alarm_param,sizeof(device_comps.alarm_param));
                    }
                    else 
                    {
                        err=0;
                    }
                    break;
                    
            case  MD_GET_REPORT_TIME_MSGID:
            case  MD_GET_PALARM_THRESHOLD_MSGID:
            case  MD_GET_TALARM_THRESHOLD_MSGID:
            case  MD_GET_DEVICE_INFO_MSGID:
            case  MD_GET_ACCESS_ADDR_MSGID:
                   break;
		default:
		           err=0;
	}
	
   
	if(err)
	{
        unsigned char address_fiel=buf[0];
        unsigned int  midId=((unsigned int)buf[Len-2]<<8)+buf[Len-1];
		dealAckData(buf[0],midId);
	}
	return err;


}

static void Srv_Protol(int event)
{
	if(protocolMisc.step==0)//Encapsulate the registration package and send a message to esam encryption
	{
		if(netComps.disCode>2)
		{
            if(!protocolComps.sw._bit.DataRdy)
            {
                memset(protocolMisc.buf,0,sizeof(protocolMisc.buf));
    			protocolComps.msgLen=Encapsulate_dataPush_package(protocolMisc.buf,event);
    			protocolComps.msg;//=protocolMisc.buf;	
    		    protocolComps.sw._bit.DataRdy=1; //send a message to bc35 send
    		    protocolComps.timer=10;
                protocolMisc.step++;
            }
         }
	}
	else if(protocolMisc.step==1)//
	{
		if((netComps.St._bit.recvData)&&(!protocolComps.sw._bit.DataRdy))//////////////////
		{
			unsigned char address_field;
			netComps.St._bit.recvData=0;//TODO clr buf
			memset(protocolMisc.buf,0,sizeof(protocolMisc.buf));
			memcpy(protocolMisc.buf,netComps.msg,netComps.msgLen);
			protocolComps.msgLen=netComps.msgLen;
			if(!Analysis_downstream_package(protocolMisc.buf,protocolComps.msgLen,&address_field))//Remove the link layer protocol (68, len crc etc)
			{
                if(address_field>=40&&address_field<=49)
                {
                    DealDownCmd(protocolMisc.buf,protocolComps.msgLen);
                    protocolComps.timer=10;
                }
                else if(address_field==0xaa)
                {
                     protocolComps.sw._bit.dataPushYet=1;
                }
                else
                {

                }
            }
			else
			{
				//TODO ....
			}
		}
		else
		{
			if(!protocolComps.timer)
			{
                if(!protocolComps.sw._bit.dataPushYet)
                {
    				if(protocolMisc.reRryTimes>0)
    				{
    					protocolMisc.reRryTimes--;
    					if(protocolMisc.reRryTimes>0)
    					{
    						protocolMisc.step=0;
    					}
    					else
    					{
    						protocolComps.sw._bit.recvTimeOut=1;
    						protocolMisc.step++;
    					}
    				}
				}
				else
				{
                    protocolComps.sw._bit.recvTimeOut=1;
                    protocolMisc.step++;
				}
			}
		}
	}
	else if(protocolMisc.step==2)
	{
        //if()
	}
}




static void protocolComps_task_handle()
{
	if(protocolMisc.event_index<0)
	{
		int i=0;
		for(i=0;i<sizeof(protocolComps.triggerIrq.All)*8;i++)
		{
			if((protocolComps.triggerIrq.All>>i)&1)
			{
				protocolMisc.event_index=i;
				protocolMisc.step=0;
				protocolMisc.reRryTimes=2;
				netComps.St._bit.recvData=0;
				protocolComps.sw._bit.DataRdy=0;
				protocolComps.triggerIrq.All&=~((unsigned int)1<<protocolMisc.event_index);
				protocolComps.sw._bit.dataPushYet=0; //
				enter_report_mode();
				netComps.disCode=EM_DIS_ACT;
				break;
			}
		}
	}
	else if(protocolMisc.event_index>-1)
	{
		if(!netComps.St._bit.running)
		{
			netComps.St._bit.on=1;
		}
		else
		{
			Srv_Protol(protocolMisc.event_index);
		}
	}
	if(netComps.St._bit.noResponse||netComps.St._bit.windowTimeOut||protocolComps.sw._bit.cmd_shutDown
		||protocolComps.sw._bit.recvTimeOut)
	
	{
	    int bit_off_flag=0;
		if(protocolComps.sw._bit.cmd_shutDown||protocolComps.sw._bit.dataPushYet)//normal shut down
		{
			if(!(protocolComps.triggerIrq.All & 0xFF))
			{
				bit_off_flag=1;
	        }
	        else
	        {
                bit_off_flag=0;
                netComps.op_window_time=60;
                protocolMisc.event_index=-1;
            }
        }
		else
		{
            bit_off_flag=1;
        }
        if(bit_off_flag)
        {
            if(!netComps.St._bit.offing)
            {
                 netComps.St._bit.off=1;
                 netComps.disCode=EM_DIS_OFF;
            }
           
        }
        netComps.St._bit.noResponse=0;
        netComps.St._bit.windowTimeOut=0;
        protocolComps.sw._bit.cmd_shutDown=0;
        protocolComps.sw._bit.recvTimeOut=0;
     }
     
}


protocolComps_t protocolComps=
{
    0,
    0,
    protocolMisc.buf,
    0,
    30,
    &protocolMisc.event_index,//unsigned int * const event_index_pt;
    protocolComps_task_handle
};

