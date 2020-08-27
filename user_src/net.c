#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
#include "r_cg_rtc.h"
#include "device.h"
#include "net.h"
#include "hum.h"
#include "protocol.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"



#define GSMPWRCTL_pin	P4.2//GSM power ctrl
#define GSMON_pin		P4.3//GSM Open Close ctrl
#define GSMPWRCTL_pin_M	PM4.2
#define GSMON_pin_M		PM4.3



#define   RESET_FROM_M72D_RX_SP        netComps.recv_RxSt=&netComps.recv_base_pt[0];netMisc.recv_Idx = 0;memset(&netMisc.recv_buf[0],0,sizeof(netMisc.recv_buf))
#define   ON_NB_IOT_ACK_OR_READ_OK     netMisc.AT_Waiting=netMisc.NoAckTimes=0;netMisc.Flag_St++; netComps.recv_RxSt=&netComps.recv_base_pt[0]
#define   ON_NB_IOT_TURN_OF_STEP_OK    netMisc.AT_Waiting=netMisc.NoAckTimes=0;netMisc.TurnOFF_St++;  netComps.recv_RxSt=&netComps.recv_base_pt[0]
#define   ON_NB_IOT_NO_ACK  	       netMisc.AT_Waiting=netMisc.NoAckTimes=0;netMisc.Flag_St=29; netMisc.TurnOFF_St=0; netComps.op_window_time=0;memset(&netMisc.recv_buf[0],0,sizeof(netMisc.recv_buf))
#define   ON_NB_IOT_REACHTIME          ON_NB_IOT_NO_ACK 

#define   ON_NB_IOT_PARAM_ERR          netMisc.AT_Waiting = netMisc.NoAckTimes =0;netMisc._Flag_St=netMisc.Flag_St;netMisc.Flag_St=27;netMisc.NoAckTimes=0;netComps.recv_RxSt=&netComps.recv_base_pt[0]
#define   ON_NB_IOT_SET_OPTION         netMisc.AT_Waiting = netMisc.NoAckTimes =0;netMisc._Flag_St=netMisc.Flag_St;netMisc.Flag_St=40;netMisc.NoAckTimes=0;netComps.recv_RxSt=&netComps.recv_base_pt[0]
#define   NB_IOT_MOUDLE_TURN_ON        netMisc.AT_Waiting = netMisc.NoAckTimes =0;netMisc.Flag_St = 0; netMisc.Reset_St=0;//turn on moudle //
#define   NB_IOT_MOUDLE_TURN_ON_FAST   netMisc.AT_Waiting = netMisc.NoAckTimes =0;netMisc.Flag_St = 15;netMisc.Reset_St=0;//turn on moudle //
#define   NB_IOT_MOUDLE_TURN_OFF       M72D_TurnOFF();
#define   NB_IOT_MOUDLE_REBOOT_BY_CMD  netMisc.AT_Waiting = netMisc.NoAckTimes =0;netMisc.Flag_St = 0;netMisc.Reset_St=2;////Reload Act-----
#define   md_turn_off()                ON_NB_IOT_NO_ACK 
#define   MD_RETYR_SEND_TIMES                           2
#define   MD_WAIT_RECV_TIME                             30//30*0.5=15s
#define   MD_WAIT_ACK_TIME                              20

#define   MD_SEARCH_NET_TIME                            90 // 90-180s
#define   MD_MODULE_OP_MAX_TIME                         180
#define   MD_MODULE_UPDATE_MAX_TIME                     (unsigned int)20*60

#define  enable_net_receive()       R_UART2_Start()
#define  disable_net_receive()      R_UART2_Stop()

typedef unsigned char BYTE;


static struct 
{ 
	unsigned short NoAckTimes;
	unsigned char  AT_Waiting;	//AT?üá?·￠?í±ê??//
	unsigned char  Flag_St;
	unsigned char _Flag_St;
	unsigned int   WaitRecTimes;
	unsigned char  ReTrySendTimes;
	unsigned char  Reset_St ;
	unsigned char  TurnOFF_St;
	
	unsigned char ErrCode;
	unsigned char M72D_err;
    unsigned char TX_ATCombuffer[64]; 
	unsigned char send_buf[612];
	unsigned char recv_buf[768];
	unsigned int  recv_Idx;
	unsigned char recv_msg_buf[256];//send to protocol layer
	
	
	unsigned char SocketNum;
	unsigned char IpNum[32];
	unsigned char NconfigErrType;
	unsigned int  NoRegestNetCount;
	
	
	MD_STATUS  (*write)(uint8_t * const tx_buf, uint16_t tx_num);
	//unsigned char * (*Check_MC55Ack)(unsigned char *FindStr);  
	
}
netMisc=
{
	0,
	0,
	0,
	0,
	MD_WAIT_RECV_TIME,
	MD_RETYR_SEND_TIMES,
	0,
	0,
	
	3,
	0,
	{0},
	{0},
	{0},
	 0,
	{0},
	
	'0',
	"",
	0,
	0,
		
	R_UART2_Send
};

  
   
   
    
   
   


static void DealAnalyticCode(int err)//err=0;错误，err=1,recv ok ，err=3,超时 else off
{
	
if(err==0)//err=0;′í?ó
	{
		netMisc.AT_Waiting=0;
		netMisc.NoAckTimes=0;
		netMisc.Flag_St=21+1;   //jmp SendData
		netComps.recv_RxSt=&netComps.recv_base_pt[0]; 
		netMisc.WaitRecTimes+=10;//10*0.5s=5s ,has rec cmd ,so wait rec time delay 5s
	}

	else if(err==1)//err=1,recv data ok
	{
		netMisc.AT_Waiting=0;
        netMisc.NoAckTimes=0;
        netMisc.Flag_St=21+1;   //jmp SendData
        netComps.recv_RxSt=&netComps.recv_base_pt[0]; 
        netMisc.WaitRecTimes+=10;//10*0.5s=5s ,has rec cmd ,so wait rec time delay 5s
		
	}
	else if(err==3)//,err=3,no data
	{
		netMisc.AT_Waiting=0;
		netMisc.NoAckTimes=0;
		netMisc.Flag_St=21+1;   //jmp SendData
		netComps.recv_RxSt=&netComps.recv_base_pt[0]; 
		netMisc.WaitRecTimes+=10;//10*0.5s=5s ,has rec cmd ,so wait rec time delay 5s
	}
	else
	{
		netComps.St._bit.noResponse=1;
	}

}
	
void HexSrtToHex(unsigned char *des,unsigned char const *src,unsigned int Length)
{
	unsigned int  Count=0;
	unsigned int  i=0;
	unsigned char Temp_H=0;
	unsigned char Temp_L=0;

	while(Count<Length)
	{
		if( src[i]<=0x39)
		{
			//src[i]-=0x30;
			Temp_H=src[i]-0x30;
		}
		else 
		{
			//src[i]-=0x37;
			Temp_H=src[i]-0x37;
		}

		if( src[i+1]<=0x39)	
		{	
			//src[i+1]-=0x30;
			Temp_L=src[i+1]-0x30;
			
		}
		else 
		{
			//src[i+1]-=0x37;
			Temp_L=src[i+1]-0x37;
		}
		//des[Count++]=((src[i]<<4) |src[i+1]);
		des[Count++]=((Temp_H<<4) |Temp_L);
		i+=2;
	}

}
//void Copy_Data_From_SComRxBuf_To_SComRxTxBuf_And_To_Hex(unsigned char *des,unsigned char const*src,unsigned int Length)
//{
//	HexSrtToHex(des,src,Length);
//}
static void HandleNoAck()
{
	if(netMisc.NoAckTimes<3)
	{
		netMisc.AT_Waiting =0;
		netMisc.NoAckTimes++;
		
	}
	else 
	{
		if(!netComps.St._bit.offing)
		{
			netComps.St._bit.noResponse=1;
		}
	} 
}

static void TX_ATData(unsigned char  *ptr,unsigned int WaitingTime)
{
	//PM3.5=0;
	netComps.AckTmr = WaitingTime;	//WaitingTime*100mS//
	netMisc.AT_Waiting = 1;		//发送完一次AT指令后,防止重复发送//
	netMisc.write(ptr,strlen(ptr));
}

static void TX_ATCommand(unsigned char  *ptr,unsigned short WaitingTime)
{
	unsigned char TxLen = 0;
	//PM3.5=0;
	while(*ptr!='\0'){
		netMisc.TX_ATCombuffer[TxLen] = *ptr++;
		TxLen++;
	}
	
	netComps.AckTmr = WaitingTime;	//WaitingTime*100mS//
	netMisc.AT_Waiting = 1;		//发送完一次AT指令后,防止重复发送//
	netMisc.write(&netMisc.TX_ATCombuffer[0], TxLen);
}

static unsigned char * Check_MC55Ack(unsigned char *FindStr)  
{
    void *rslt;
    BYTE FindstrLen;
    BYTE *StartPt,*EndPt;
    
    rslt = 0;
    FindstrLen =(BYTE) strlen((char *)FindStr);
    StartPt = &netComps.recv_base_pt[0];
    EndPt = netComps.recv_RxSt;
    while(EndPt+1-StartPt>=FindstrLen)
    {
	if(!memcmp(FindStr,StartPt,FindstrLen))
	 {
		 rslt = StartPt+FindstrLen;
		 break;
	 } 
	 StartPt++;
    }
    return(rslt);
}



void GetRssi()
{
	unsigned char RSSI[2]={'0','0'};
	unsigned char i=0;
	unsigned char *pt_rssi;
	if(!netMisc.AT_Waiting)
	{
		RESET_FROM_M72D_RX_SP;
		TX_ATCommand("\r\nAT+CSQ\r\n",3);
	} 
	else if(!netComps.AckTmr)
	{
		HandleNoAck(); 
	}
	else if(Check_MC55Ack("OK\r\n"))
	{
	      if(pt_rssi=Check_MC55Ack("+CSQ:"))
	      {
    	        for(i=0;i<sizeof(RSSI);i++)
    			{
    				if(*pt_rssi!=',')
    				{
    				   RSSI[i]=*pt_rssi++;
    				}
    				else
    				{
    					break;
    				}
    			}
    			if(i>1)
    			{
    				netComps.net_info.Csq = (((RSSI[0]-0x30)&0x0F)*10)+(RSSI[1]-0x30);
    			}
    			else
    			{
    				netComps.net_info.Csq = (RSSI[0]-0x30);
    			}
    			pt_rssi++;//jmp ,
    			RSSI[0]='0';
    			RSSI[1]='0';
    			for(i=0;i<sizeof(RSSI);i++)
    			{
    				if(*pt_rssi!=',')
    				{
    				   RSSI[i]=*pt_rssi++;
    				}
    				else
    				{
    					break;	
    				}
    			}
    			
    			if(i>1)
    			{
    				netComps.net_info.CsQ = (((RSSI[0]-0x30)&0x0F)*10 )+(RSSI[1]-0x30);
    			}
    			else
    			{
    				netComps.net_info.CsQ = (RSSI[0]-0x30);
    			}
		}
	        ON_NB_IOT_ACK_OR_READ_OK; 
	}  
}
static void GetNuestats()
{
	unsigned char i=0;
	unsigned char *pt_rssi;
	unsigned char  buf[12]="";
	if(!netMisc.AT_Waiting)
	{
		RESET_FROM_M72D_RX_SP;
		//TX_ATCommand("\r\nAT+COPS?\r\n",60); 
		TX_ATCommand("\r\nAT+NUESTATS\r\n",3); 
	}
	else if(!netComps.AckTmr)
	{
		HandleNoAck();   	
	} 
	else if(Check_MC55Ack("OK\r\n"))
	{
		 if(pt_rssi=Check_MC55Ack("ECL:"))
		 {
			netComps.net_info.ecl=*pt_rssi -0x30;
		 }
		
		 if(pt_rssi=Check_MC55Ack("SNR:"))
		 {
			memset(buf,0,sizeof(buf));
		 	for(i=0;i<sizeof(buf);i++)
			{
				if(*pt_rssi!='\r')
				{
				   buf[i]=*pt_rssi++;
				}
				else
				{
					break;
				}
			}
			netComps.net_info.sina=atol(buf)/10<-128?-128:atol(buf)/10;
		 }
		 
		 if(pt_rssi=Check_MC55Ack("RSRQ:"))
		 {
			memset(buf,0,sizeof(buf));
	        for(i=0;i<sizeof(buf);i++)
		    {
				if(*pt_rssi!='\r')
				{
				    buf[i]=*pt_rssi++;
				}
				else
				{
					break;
				}
	        } 
	        netComps.net_info.rsrq=atol(buf)/10<-128?-128:atol(buf)/10;
			
		 }
		 if(pt_rssi=Check_MC55Ack("Signal power:"))
		 {
			memset(buf,0,sizeof(buf));
		 	for(i=0;i<sizeof(buf);i++)
			{
				if(*pt_rssi!='\r')
				{
				   buf[i]=*pt_rssi++;
				}
				else
				{
					break;
				}
			} 
			netComps.net_info.rsrp=atol(buf)/10<atol(buf)/10<-128?-128:atol(buf)/10;
		 }
		 
		if(pt_rssi=Check_MC55Ack("PCI:"))
		{
            memset(buf,0,sizeof(buf));
            for(i=0;i<sizeof(buf);i++)
            {
                if(*pt_rssi!='\r')
                {
                  buf[i]=*pt_rssi++;
                }
                else
                {
                   break;
                }
            } 
            netComps.net_info.pci=atol(buf);
        }
		 if(pt_rssi=Check_MC55Ack("Cell ID:"))
		 {
			memset(buf,0,sizeof(buf));
			//int k=5;
		 	//memset(netComps.net_info.cellId,0,sizeof(netComps.net_info.cellId));
			for(i=0;i<sizeof(buf);i++)
			{
				if(*pt_rssi!='\r')
				{
                    buf[i]=*pt_rssi++;
                }
				else
				{
					break;
				}
			} 
			netComps.net_info.cellId=atol(buf);
			/*
			while(i!=0)
			{
				if(i>1)
				{
					netComps.net_info.cellId[k]= buf[i-1]+(buf[i-2]<<4);
					k--;
					i-=2;
				}
				else if(i>0)
				{
					netComps.net_info.cellId[k--]= buf[i-1];
					i--;
				}
			}
			NOP();
			*/
			
		 }
		 if(pt_rssi=Check_MC55Ack("EARFCN:"))
		 {
			memset(buf,0,sizeof(buf));
		 	for(i=0;i<sizeof(buf);i++)
			{
				if(*pt_rssi!='\r')
				{
				   buf[i]=*pt_rssi++;
				}
				else
				{
					break;
				}
			} 
			netComps.net_info.earFcn=atol(buf);
		 }
		 ON_NB_IOT_ACK_OR_READ_OK;
	}
}
static void DelayMs(unsigned short TimeMs){
	unsigned short j;
	unsigned char i;
	for(j=0;j<TimeMs*10;j++){		
		//i=7;	//10us at 8Mhz
		i=97;	//100us at 8Mhz
		while(i>0) i--;
	}
}


static void SetIpAndPort()
{
	if(1)
	{  
		memset(netMisc.IpNum,0,sizeof(netMisc.IpNum));
		sprintf(netMisc.IpNum+strlen(netMisc.IpNum),"%d.",device_comps.access_param.ip[0]);
		sprintf(netMisc.IpNum+strlen(netMisc.IpNum),"%d.",device_comps.access_param.ip[1]);
		sprintf(netMisc.IpNum+strlen(netMisc.IpNum),"%d.",device_comps.access_param.ip[2]);
		sprintf(netMisc.IpNum+strlen(netMisc.IpNum),"%d,",device_comps.access_param.ip[3]);
		sprintf(netMisc.IpNum+strlen(netMisc.IpNum),"%u",device_comps.access_param.port);
	}
}


 void DataAdd30H(unsigned char *i, unsigned char const *j,unsigned int Len)
{
	while(Len--)
	{
		if(((*j&0xF0)>>4)<10)
		{
			*i++=((*j&0xF0)>>4) +0x30;
		}
		else 
		{
			*i++=((*j&0xF0)>>4) +0x37;
		}
		if((*j&0x0F)<10)
		{
			*i++=(*j&0x0F)+0x30;
		}
		else 
		{
			*i++=(*j&0x0F) +0x37;
		}
		j++;
	}
}



unsigned char PacketCoapMsg(unsigned char const *buf,int len)
{
	unsigned char ret=0;
        unsigned int  i;
	unsigned char Var[8]="";
	memset(netMisc.send_buf,0,sizeof(netMisc.send_buf));
 	sprintf(netMisc.send_buf,"%s","\r\nAT+NMGS=");
  	i=strlen(netMisc.send_buf);
	sprintf(Var,"%d",len);
	memcpy(&netMisc.send_buf[i],Var,strlen(Var));
	i+=strlen(Var);
	netMisc.send_buf[i++]=',';
	DataAdd30H(&netMisc.send_buf[i],buf,len);
	i+=len*2;
	memcpy(&netMisc.send_buf[i++],"\r\n",2);//ADD \r\n 
	ret=1;
	return ret;
}              

static void GetHexMsg(unsigned char *des,unsigned char const*src,unsigned int Length)
{
	HexSrtToHex(des,src,Length);
}

static unsigned char GetMsg(unsigned char const *buf,int len)
{
	unsigned char err=0;
	unsigned int Len= 0;
	memset(netMisc.recv_msg_buf,0,sizeof(netMisc.recv_msg_buf));	
	GetHexMsg(netMisc.recv_msg_buf,buf,len);
	netComps.msg;//=netMisc.recv_msg_buf;
	netComps.msgLen=len;
	netComps.St._bit.recvData=1;
	err=1;
    return err;////err=0;错误，err=1,ok;
}              















	
//--------------------?′??M72D--------------------------//

static void M72D_HardwareInit(void)
{
   GSMPWRCTL_pin = 0;	//GSMμ??′1üàí:1?a??μ??′01?±?μ??′//
   DelayMs(5);
   GSMON_pin = 1;   //GSM PowerKey/
   GSMON_pin_M=0;
   DelayMs(1);
}

static void Reset_M72D(void)
{
	switch(netMisc.Reset_St)
	{
		case 0:
			M72D_HardwareInit();	
			netComps.AckTmr=1;      
			netComps.recv_RxSt=&netComps.recv_base_pt[0];
			netMisc.Reset_St++;
			break;
		case 1:
			if(!netComps.AckTmr)
			{
			       GSMON_pin = 0;
			       RESET_FROM_M72D_RX_SP;
			       netComps.AckTmr=5;  //wait 4s NEUL//
			       netMisc.Flag_St++;
			       netMisc.Reset_St=0;
			       netMisc.NoAckTimes =0;
			}
			break;
		case 2:
			
			RESET_FROM_M72D_RX_SP;
		        TX_ATCommand("\r\nAT+NRB\r\n",5);
		        GSMON_pin = 0;   
		        netMisc.Flag_St++;
		        netMisc.Reset_St=0;
		        netMisc.NoAckTimes =0;
			break; 
	}
}
//==================================================//
unsigned int bSentRA=0;
void M72D_TurnOFF(void)
{   
     char msg[32]="";
     switch(netMisc.TurnOFF_St)
     { 
      case 0: 
    	   if(!netMisc.AT_Waiting)
    	   {
        	   	if(netMisc.SocketNum!='\0')
        		{
        		   	sprintf(msg,"%s","\r\nAT+NSOCL=");
        			sprintf(msg+strlen(msg),"%c",netMisc.SocketNum);
        			sprintf(msg+strlen(msg),"%s","\r\n");
        			RESET_FROM_M72D_RX_SP;
        			TX_ATCommand(msg,3);
        			bSentRA=0;
        		}
        		else
        		{
        			netComps.AckTmr = 1;	//WaitingTime*100mS//
        			netMisc.AT_Waiting = 1;
        		}
    	   }
    	   else if(!netComps.AckTmr)
    	   { 
    	    	ON_NB_IOT_TURN_OF_STEP_OK;
    		    netComps.AckTmr = 1;
           }
    	   else if(Check_MC55Ack("OK\r\n"))
    	   {
        		RESET_FROM_M72D_RX_SP;
        	    if(!bSentRA)
        		{
        		   	TX_ATCommand("\r\nAT+QLWULDATAEX=1,EE,0x0001\r\n",2);
        			bSentRA=1;
        		}
    	   
    	   }
           break;
        case 1: //2S		
           if(!netComps.AckTmr) 
           {
        		disable_net_receive();
        		netMisc.AT_Waiting = 0;
        		netMisc.TurnOFF_St=0;
        		netMisc.Flag_St=29;
        		//memset(&netComps.net_info,0,sizeof(netComps.net_info));
        		netComps.net_info.Csq=99;
        		netComps.net_info.CsQ=99;
        		GSMON_pin = 0;
        		GSMPWRCTL_pin =1;//0
        		NOP();
        		netComps.St._bit.running=0;
        		netComps.St._bit.offing=0;
        		*protocolComps.event_index_pt=-1;
        		enter_normal_mode();
          }	
           break;
        default: break;
     }
}

 

static unsigned char GMTtoLocalTime(unsigned char *tbuf)//yy mm dd hh mm ss '+' zz
{
	unsigned int buf[8];
	unsigned int i=0;
	unsigned char  monthlengths[]={31,28,31,30,31,30,31,31,30,31,30,31};
   #define  ADJFACTOR  1
   	for(i=0;i<8;i++)
	{
		if(i==0)
		{
			buf[i]=2000+tbuf[i];
			if(((buf[i]%4==0) && (buf[i]%100!=0)) || (buf[i]%400==0))
			{
				 monthlengths[2-ADJFACTOR]=29;
			}
		}
		buf[i]=tbuf[i];
	}
	buf[7]=(unsigned int)buf[7]*15/60;//15min
	if(buf[6]=='+')
	{
		buf[3]+=buf[7];//timeZone
		if(buf[3]<24)
		{
			
		}
		else
		{
			buf[3]%=24;//h
			buf[2]++;//dd
			if(buf[2]>monthlengths[buf[1]-ADJFACTOR])
			{
				buf[2]%=monthlengths[buf[1]-ADJFACTOR];
			        buf[1]++;//mm
				if(buf[1]>12)
				{
					buf[1]%=12;
					buf[0]++;//yy++
				}
			}
		}
	}
	else if(tbuf[6]=='-')
	{
		if(buf[3]>=buf[7])
		{
			buf[3]-=buf[7];
		}
		else
		{
			buf[3]=buf[3]+24-buf[7];//h
		        buf[2]--;//dd
			if(!buf[2])
			{
				buf[1]--;//mm
			        buf[2]=monthlengths[buf[1]-ADJFACTOR];
			        
				if(!buf[1])
				{
				    buf[0]--;//yy--
				    buf[1]=1;
				}
			}
		}
	}
	for(i=0;i<6;i++)
	{
		tbuf[i]=((buf[i]%100/10)<<4)+(buf[i]%10);
	}
	return 1;
}

//--GSM模块服务程序，进行模块基本参数设置，波特率、短信模式、短信存储位置、电话本模式、电话本存储位置、短信文本模式、模块信号强度--//
static void Srv_GSM(void)
{     
	unsigned char *pt_rssi;
        unsigned char i;
        unsigned char msg[64]="";
	
	switch(netMisc.Flag_St)
	{
		case 0:
			Reset_M72D();		
			break;
			
		case 1: 
			if(Check_MC55Ack("OK\r\n"))
			{
				 ON_NB_IOT_ACK_OR_READ_OK;
			}		 
			else if(!netComps.AckTmr)
			{
				 ON_NB_IOT_ACK_OR_READ_OK;
			}
			break; 
		case 2:	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
			   // #if(KIND_OF_NET_PROTOCOL==UDP)
			   //     TX_ATCommand("\r\nAT+QREGSWT=2\r\n",20);
			   // #elif(KIND_OF_NET_PROTOCOL==COAP)
			        TX_ATCommand("\r\nAT+QREGSWT=1\r\n",3);
			       //TX_ATCommand("\r\nAT+CMEE=1\r\n",20);
			   // #endif
			} 
			else if(!netComps.AckTmr)
			{
				  ON_NB_IOT_ACK_OR_READ_OK;
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				 ON_NB_IOT_ACK_OR_READ_OK;
			}	
			break;
			
		case 3:	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
			        TX_ATCommand("\r\nAT+CGMR\r\n",3);//Ver
			}  
			else if(!netComps.AckTmr)
			{
				HandleNoAck(); 
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				 if(pt_rssi=Check_MC55Ack(","))
				 {
					// if(pt_rssi=Check_MC55Ack("SECURITY,"))
					 if(pt_rssi=Check_MC55Ack(","))
					 {
						 memset(netComps.net_info.firmVer,0,sizeof(netComps.net_info.firmVer));
					 	 for(i=0;i<sizeof(netComps.net_info.firmVer);i++)
						 {
							if(*pt_rssi!='\r')
							{
						 	  netComps.net_info.firmVer[i]=*pt_rssi;
							  pt_rssi++;
							}
							else
							{
							   break; 	
							}
						  }
					 }
					 
					 if(pt_rssi=Check_MC55Ack("RADIO,"))
					 {
						 memset(netComps.net_info.firmhdVer,0,sizeof(netComps.net_info.firmhdVer));
						 for(i=0;i<sizeof(netComps.net_info.firmhdVer);i++)
						 {
							if(*pt_rssi!='\r')
							{
						 	  netComps.net_info.firmhdVer[i]=*pt_rssi;
							  pt_rssi++;
							}
							else
							{
							   break; 	
							}
						  }
					 }
				 }
				 else
				 {
					 if(pt_rssi=Check_MC55Ack("\r\n"))
					 {
						 memset(netComps.net_info.firmVer,0,sizeof(netComps.net_info.firmVer));
					         for(i=0;i<sizeof(netComps.net_info.firmVer);i++)
						 {
							if(*pt_rssi!='\r')
							{
						 	  netComps.net_info.firmVer[i]=*pt_rssi;
							  pt_rssi++;
							}
							else
							{
							   break; 	
							}
						  }
					 }
				 }
				 ON_NB_IOT_ACK_OR_READ_OK;
			}	
			break;

			
		case 4:	
			if(!netMisc.AT_Waiting) 
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NCDP?\r\n",3);
			}
			else if(!netComps.AckTmr)
			{
				if(Check_MC55Ack("ERROR"))
				{
					ON_NB_IOT_PARAM_ERR; 
				}
				else 
				{
					HandleNoAck(); 
				}
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				sprintf(msg+strlen(msg),"%s",netMisc.IpNum);
				if(Check_MC55Ack(msg))
				{
					ON_NB_IOT_ACK_OR_READ_OK;
				}
				else
				{
					ON_NB_IOT_PARAM_ERR; 
					//ON_NB_IOT_ACK_OR_READ_OK;
				}
			}
			break;
		case 5: 	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NBAND?\r\n",3); 
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck();  	
			} 
			else if(Check_MC55Ack("OK\r\n"))
			{
			    if(Check_MC55Ack("+NBAND:5"))
			    {
			    	ON_NB_IOT_ACK_OR_READ_OK;
			    }
			    else
			    {
				 ON_NB_IOT_PARAM_ERR;   
			    }
			}
			break;
			
		case 6: 	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NCONFIG?\r\n",3); 
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck();   	
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				if(Check_MC55Ack("+NCONFIG:AUTOCONNECT,FALSE"))
				{
					ON_NB_IOT_PARAM_ERR;
					netMisc.NconfigErrType=0;
				}
				else if(Check_MC55Ack("+NCONFIG:CR_0354_0338_SCRAMBLING,FALSE"))
				{
					ON_NB_IOT_PARAM_ERR;
					netMisc.NconfigErrType=1;
				}
				else if(Check_MC55Ack("+NCONFIG:CR_0859_SI_AVOID,FALSE"))
				{
					ON_NB_IOT_PARAM_ERR;
					netMisc.NconfigErrType=2;
				}
				else
				{
					 ON_NB_IOT_ACK_OR_READ_OK;
					 
					 netMisc.M72D_err=0;
				}
			}
			break;
			
		case 7: 
			if(!netMisc.AT_Waiting) 
			{
				netComps.AckTmr =2;	//wait cfun=1 0.75s
				netMisc.AT_Waiting = 1;
			}
			else if(!netComps.AckTmr)
			{
 				ON_NB_IOT_ACK_OR_READ_OK;
			}
			break;
			
		case 8: 
	        if(!netMisc.AT_Waiting) 
			{
			    RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+CFUN?\r\n",3);
			}
			else if(!netComps.AckTmr)
			{
			        HandleNoAck();
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
			       if(Check_MC55Ack("+CFUN:1"))
			       {
				       ON_NB_IOT_ACK_OR_READ_OK;
				       netComps.disCode=EM_DIS_SEARCH_NET;
				       mode_comps[hum_comps.current_mode].displayTimer=0;
			       }
			       else
			       {
				     netMisc.M72D_err++;
				     if(netMisc.M72D_err>5)
				     {
					   HandleNoAck();    
				     }
				     else
				     {
					    netMisc.Flag_St=7;
				     	netMisc.AT_Waiting=0;    
				     }
			       }
			}  
			break;
		
		case 9: 
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
			        TX_ATCommand("\r\nAT+CGSN=1\r\n",3);//IMEI
			} 
			else if(!netComps.AckTmr)
			{
				HandleNoAck();  
			}
			else if(pt_rssi=Check_MC55Ack("OK\r\n"))
			{
				if(pt_rssi=Check_MC55Ack("+CGSN:"))
				 {
					//unsigned char buf[30]={0}; 
					//int k=7;
					memset(netComps.net_info.imei,0,sizeof(netComps.net_info.imei));
			        for(i=0;i<sizeof(netComps.net_info.imei);i++)
				    {
						if(*pt_rssi!='\r')
						{
						    netComps.net_info.imei[i]=*pt_rssi++;
						}
						else
						{
							break;
						}
				    } 
				/*	while(i!=0)
					{
						if(i>1)
						{
							netComps.net_info.IMEI[k]= buf[i-1]+(buf[i-2]<<4);
							k--;
							i-=2;
						}
						else if(i>0)
						{
							netComps.net_info.IMEI[k--]= buf[i-1];
							i--;
						}
					}
					NOP();
				*/
				 }
				 ON_NB_IOT_ACK_OR_READ_OK;
			}	
			
			break;
		case 10: 	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
			        TX_ATCommand("\r\nAT+CIMI\r\n",3);//
			} 
			else if(!netComps.AckTmr)
			{
				HandleNoAck();  
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				 if(pt_rssi=Check_MC55Ack("\r\n"))
				 {
					 memset(netComps.net_info.imsi,0,sizeof(netComps.net_info.imsi));
				     for(i=0;i<sizeof(netComps.net_info.imsi);i++)
					 {
						if(*pt_rssi!='\r')
						{
					 	  netComps.net_info.imsi[i]=*pt_rssi++;
						}
						else
						{
						   break; 	
						}
					  }
				 }
				 ON_NB_IOT_ACK_OR_READ_OK;
			}	
			break;
			
		case 11:  	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NCCID\r\n",3); 
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck();  	
			} 
			else if(Check_MC55Ack("OK\r\n"))
			{
				if(pt_rssi=Check_MC55Ack("+NCCID:"))
				 {
					 memset(netComps.net_info.iccid,0,sizeof(netComps.net_info.iccid));
				     for(i=0;i<sizeof(netComps.net_info.iccid);i++)
					 {
    						if(*pt_rssi!='\r')
    						{
    					 	  netComps.net_info.iccid[i]=*pt_rssi++;
    						  
    						}
    						else
    						{
    						   break; 	
    						}
					  }
				 }
				 ON_NB_IOT_ACK_OR_READ_OK;
			}
			break;
		
		case 12: 
			if(!netMisc.AT_Waiting) 
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NSMI=0\r\n",3);
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck();  
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				ON_NB_IOT_ACK_OR_READ_OK;
			}  
			break;
			
	        case 13: 
			if(!netMisc.AT_Waiting) 
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NNMI=0\r\n",3);
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck(); 
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				ON_NB_IOT_ACK_OR_READ_OK;
			}  
			break;		

			
		case 14:  	
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
			        TX_ATCommand("\r\nAT+CPSMS?\r\n",3);//
			} 
			else if(!netComps.AckTmr)
			{
				 ON_NB_IOT_ACK_OR_READ_OK;
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
			    if(Check_MC55Ack("+CPSMS:1,,,01000011,00000001"))//3gpp psm 30h idle 1s
			    {
			    	ON_NB_IOT_ACK_OR_READ_OK;
			    }
			    else
			    {
				 ON_NB_IOT_PARAM_ERR;   
			    }
			}
			break;
		case 15: 
//			if(!netMisc.AT_Waiting)
//			{
//				RESET_FROM_M72D_RX_SP;
//				TX_ATCommand("\r\nAT+CGATT?\r\n",3); 
//			}
//			else if(!netComps.AckTmr)
//			{
//				HandleNoAck();   	
//			} 
//			else if(Check_MC55Ack("OK\r\n"))
//			{
//				if(Check_MC55Ack("+CGATT:1"))
//				{
					ON_NB_IOT_ACK_OR_READ_OK;
					netMisc.NoRegestNetCount=0;
					
//				}
//                              else
//				{	
//					  netMisc.AT_Waiting=0;
//					  netMisc.NoAckTimes=0; 
//					  netComps.recv_RxSt=&netComps.recv_base_pt[0];
//				}
//				
//			}
			break;	
               
		case 16:
			GetRssi();
			break; 
			
		case 17:  
			GetNuestats();
			//ON_NB_IOT_ACK_OR_READ_OK;
			break;	
		case 18:  
			if(!netMisc.AT_Waiting)
			{
				//if(ClrEarfcnEn==1)
				{
					//ON_NB_IOT_PARAM_ERR;  
					//ClrEarfcnEn=0;
				}
				//else
				{
					RESET_FROM_M72D_RX_SP;
					TX_ATCommand("\r\nAT+CEREG?\r\n",3); 
					netMisc.M72D_err=0;
					
				}
			}
			else if(!netComps.AckTmr)
			{
				if(!netMisc.M72D_err)
				{
					HandleNoAck(); 
				}
				else
				{
					if(netComps.net_info.Csq==0x99)
					{
						netMisc.NoRegestNetCount++;
						if(netMisc.NoRegestNetCount>=MD_SEARCH_NET_TIME)
						{
							ON_NB_IOT_PARAM_ERR;  	
						}
						else
						{
							netMisc.Flag_St=16; 
							netMisc.AT_Waiting =0;
							netMisc.NoAckTimes=0;
						}	
					}
					else
					{
						//netMisc.NoRegestNetCount=0;
						netMisc.Flag_St=16; 
						netMisc.AT_Waiting =0;
						netMisc.NoAckTimes=0;
					}
				}
			} 
			else if(Check_MC55Ack("OK\r\n"))
			{
				if(Check_MC55Ack("+CEREG:0,1"))//home 
				{
					ON_NB_IOT_ACK_OR_READ_OK;
					netComps.disCode=EM_DIS_REGISTER_NET;
				}
				else if(Check_MC55Ack("+CEREG:0,5"))//Roaming
				{
					ON_NB_IOT_ACK_OR_READ_OK;
					netComps.disCode=EM_DIS_REGISTER_NET;
				}
				else if(Check_MC55Ack("+CEREG:0,2"))//search......
				{
					
//					netMisc.Flag_St=16; 
//				        netMisc.AT_Waiting =0;
//					netMisc.NoAckTimes=0;

					netMisc.M72D_err=1;
					RESET_FROM_M72D_RX_SP;
					netComps.AckTmr=2;
					
				}
				else
				{
					netMisc.M72D_err=1;
					RESET_FROM_M72D_RX_SP;
					netComps.AckTmr=1;
					//ON_NB_IOT_PARAM_ERR;  	
				}
			}
			break;
		
		case 19:////DELAY 0.5-2s
		        if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				netMisc.AT_Waiting=1;
				//if(MouldeStartMode=='L')
				{
					netComps.AckTmr=2;
				}
				//else
				{
				//	netComps.AckTmr=1; 
				}
			}
			else if(!netComps.AckTmr)
			{
			       //ON_NB_IOT_ACK_OR_READ_OK;
			       //if(ProofTimeEn==1)
				{
					ON_NB_IOT_SET_OPTION; 
					//ProofTimeEn=0;
				}
				
			}
			else if(pt_rssi=Check_MC55Ack("+QLWEVTIND:3"))
			{
				RESET_FROM_M72D_RX_SP;
				netComps.AckTmr=1; 
			}
			break;	
		case 20: //CREAT SOCKET
			if(!netMisc.AT_Waiting)
			{
				
				RESET_FROM_M72D_RX_SP;
				TX_ATCommand("\r\nAT+NSOCR=DGRAM,17,32820,1\r\n",3);
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck(); 
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				if(pt_rssi=Check_MC55Ack("\r\n"))
	     			{
			             netMisc.SocketNum=*pt_rssi;
				}
				
				
				netMisc.ReTrySendTimes=MD_RETYR_SEND_TIMES ;
				netComps.op_window_time = 60;
				ON_NB_IOT_ACK_OR_READ_OK;
				
			}
			break;
				
		case 21:////////////REPORT
		         GetNuestats();
			     break;
			
		case 22:
			if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				netMisc.AT_Waiting=1;
				netComps.AckTmr=1; //100ms
			}
			else if(!netComps.AckTmr)
			{
				
//				ServerIdNum=SeekServerNum();//
//				if(ServerIdNum>0)
//				{       
//				        UdpCurrentFn=AFnx[ServerIdNum-1];
//					if(InitMeterServerData(ServerIdNum,netMisc.ReTrySendTimes, UdpCurrentFn))/////////////
//					{
//						ON_NB_IOT_ACK_OR_READ_OK;
//					}
//					netMisc.WaitRecTimes=MD_WAIT_ACK_TIME;
//				}
//				else
//				{
//					////////////Trap area
//					netMisc.Flag_St=24;//////jmp wait rece.....
//					netMisc.AT_Waiting=0;
//					netMisc.NoAckTimes=0;  
//					netMisc.WaitRecTimes=MD_WAIT_RECV_TIME;
//					RECEING;
//				}

				
				
				if(protocolComps.sw._bit.DataRdy)
				{
					//InitMeterServerData(ServerIdNum,netMisc.ReTrySendTimes, UdpCurrentFn);
					PacketCoapMsg(protocolComps.msg,protocolComps.msgLen);
					ON_NB_IOT_ACK_OR_READ_OK;
					netMisc.WaitRecTimes=MD_WAIT_ACK_TIME;
				}
				else
				{
					ON_NB_IOT_ACK_OR_READ_OK;
					netMisc.Flag_St++;//jump recv_data
				}
				
				
			}
			break;
		case 23://SendData
		        if(!netMisc.AT_Waiting)
			{
				RESET_FROM_M72D_RX_SP;
				TX_ATData(&netMisc.send_buf[0],5);
				 netComps.disCode=EM_DIS_SEND;
			} 
			else if(!netComps.AckTmr)
			{
				HandleNoAck(); 
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
                protocolComps.sw._bit.DataRdy=0;
                netMisc.Flag_St=24;//////jmp wait rece.....
				netMisc.AT_Waiting=0;
				netMisc.NoAckTimes=0;  
				//netMisc.WaitRecTimes=MD_WAIT_RECV_TIME;
				
			}
//			else if(Check_MC55Ack("ERROR"))
//			{
////				netMisc.Flag_St=24;//////jmp wait rece.....
////				netMisc.AT_Waiting=0;
////				netMisc.NoAckTimes=0;  
//				HandleNoAck();
//				
//			}
			break; 
			
		case 24://///////////////////Wait Rec
		  
		        if(!netMisc.AT_Waiting)
    			{ 
    				RESET_FROM_M72D_RX_SP;
    				netMisc.AT_Waiting=1;
    			 	netComps.AckTmr = 1;//////////////////0.5s
    			}
    			else if(!netComps.AckTmr)
    			{
                  ON_NB_IOT_ACK_OR_READ_OK;
    			}
	
		        break;
		case 25:
	    	if(!netMisc.AT_Waiting)
			{
                if(!netComps.St._bit.recvData)
                {
				    RESET_FROM_M72D_RX_SP;
				    TX_ATCommand("\r\nAT+NMGR\r\n",3);
				}
				else
				{
                    netMisc.Flag_St=24;//////jmp wait rece.....
                    netMisc.AT_Waiting=0;
                    netMisc.NoAckTimes=0; 
				}
			}
			else if(!netComps.AckTmr)
			{
				HandleNoAck(); 
			}
			else if(Check_MC55Ack("OK\r\n"))
			{
				unsigned int Length=0;
				memset(msg,0,sizeof(msg));
				sprintf(msg,"%s",",");
				if(pt_rssi=Check_MC55Ack(msg))
				{
					memset(msg,0,sizeof(msg));
					sprintf(msg,"%s","\r\n");
				        if(pt_rssi=Check_MC55Ack(msg))
					{
						memset(msg,0,sizeof(msg));
						for(i=0;i<64;i++)
						{
							if(*pt_rssi!=',')
							{
								msg[i]=*pt_rssi;
								pt_rssi++;
							}
							else
							{
							        pt_rssi++;//jmp ,
								break; 	
							}
						}
						if(i<64)
						{
							Length=atoi(msg);
							if((Length==0)||(Length>512)||(Length!=((unsigned long)strchr(pt_rssi,'\r')-(unsigned long)pt_rssi)/2))
							{
								netMisc.ErrCode=0;//mozu bug
							}
							else
							{
								//netMisc.ErrCode=AnalyticData(Length,pt_rssi,UdpCurrentFn);
								 netMisc.ErrCode=GetMsg(pt_rssi,Length);//(Length,pt_rssi);
								 netComps.disCode=EM_DIS_RECV; 
							}
						}
						else
						{
							netMisc.ErrCode=0;//mozu bug
						}
					}
					else
					{
						netMisc.ErrCode=0;//mozu bug
					}
					DealAnalyticCode(netMisc.ErrCode);
				}
				else
				{
					netMisc.ErrCode=3;//err=0;err，err=1,recv data,err=3,no data
				}
				DealAnalyticCode(netMisc.ErrCode);
				
				
			}
		  
			break;
		case 26:                                      
			  NB_IOT_MOUDLE_TURN_OFF;
	                  break;
			
		case 27:
	#define   MIN_MODE_GSMST         25	
			if(netMisc._Flag_St>MIN_MODE_GSMST)
			{
				ON_NB_IOT_ACK_OR_READ_OK;
			}
			else
			{
			  	if(!netMisc.AT_Waiting)
				{
					RESET_FROM_M72D_RX_SP;
					TX_ATCommand("\r\nAT+CFUN=0\r\n",3);
				} 
				else if(!netComps.AckTmr)
				{
					HandleNoAck();
				}
				else if(Check_MC55Ack("OK\r\n"))
				{
					 ON_NB_IOT_ACK_OR_READ_OK;
					
				}
			}
		       break;
			   
	          case 28:
       	         
		  	   if(!netMisc.AT_Waiting) 
			   {
					netMisc.M72D_err=0;
					RESET_FROM_M72D_RX_SP;
					if((netMisc._Flag_St==4)||(netMisc._Flag_St==25))
					{
					   sprintf(msg,"%s","\r\nAT+NCDP=");
					   sprintf(msg+strlen(msg),"%s",netMisc.IpNum);
					   sprintf(msg+strlen(msg),"%s","\r\n");
					   TX_ATCommand(msg,3); 
					}
					else if(netMisc._Flag_St==5)
					{
					   TX_ATCommand("\r\nAT+NBAND=5\r\n",3);  
					}
					else if(netMisc._Flag_St==6)
					{
					       if( netMisc.NconfigErrType==0)
					       {
							TX_ATCommand("\r\nAT+NCONFIG=AUTOCONNECT,TRUE\r\n",3); 
					       }
					       else if( netMisc.NconfigErrType==1)
					       {
						       TX_ATCommand("\r\nAT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE\r\n",3); 
					       }
					       else if( netMisc.NconfigErrType==2)
					       {
						         TX_ATCommand("\r\nAT+NCONFIG=CR_0859_SI_AVOID,TRUE\r\n",3); 
					       }
					}
					else if(netMisc._Flag_St==14)
					{
					   TX_ATCommand("\r\nAT+CPSMS=1,,,01000011,00000001\r\n",3);  
					}
				        else if(netMisc._Flag_St==18)
					{
						TX_ATCommand("\r\nAT+NCSEARFCN\r\n",3);
					}
					else
					{
					       TX_ATCommand("\r\nAT+CSQ\r\n",3);
					}
			   }
			   else if(!netComps.AckTmr)
			   {
					if(!netMisc.M72D_err)
					{
						HandleNoAck();
					}
					else
					{
						if(netMisc._Flag_St>MIN_MODE_GSMST)
						{
							NB_IOT_MOUDLE_TURN_ON;//Reboot Load
						}
						else
						{
							NB_IOT_MOUDLE_REBOOT_BY_CMD;
						}
					}
		       }
		       else if(Check_MC55Ack("OK\r\n"))
			   {
			       RESET_FROM_M72D_RX_SP;
			       if(netMisc._Flag_St>MIN_MODE_GSMST)
			       {
				     GSMPWRCTL_pin = 1;
				     DelayMs(1);
				     GSMON_pin = 1;
				     DelayMs(1); 
			       }
			       netComps.AckTmr=4;
			       netMisc.M72D_err=1;
			   } 
	           else if(Check_MC55Ack("ERROR"))
			   {
			       RESET_FROM_M72D_RX_SP;
			       if(netMisc._Flag_St>MIN_MODE_GSMST)
			       {
				     GSMPWRCTL_pin = 1;
				     DelayMs(1);
				     GSMON_pin = 1;
				     DelayMs(1); 
			       }
			       netComps.AckTmr=4;
			       netMisc.M72D_err=1;
			   } 
	                 break;
		#undef MIN_MODE_GSMST
	          case 29:
		         NB_IOT_MOUDLE_TURN_OFF;//GPRS模块关机//
			break;  
			
		
		  
		  case 40:
		        if(netMisc._Flag_St==19)
    			{
    			    if(!netMisc.AT_Waiting) 
    				{
    					RESET_FROM_M72D_RX_SP;
    					TX_ATCommand("\r\nAT+CCLK?\r\n",3);
    				}
    				else if(!netComps.AckTmr)
    				{
    					netMisc.AT_Waiting=netMisc.NoAckTimes=0;
    					netComps.recv_RxSt=&netComps.recv_base_pt[0];	  
    					netMisc.Flag_St=netMisc._Flag_St+1;  
    				}
    				else if(Check_MC55Ack("OK\r\n"))
    				{
    					 
    					 if(pt_rssi=Check_MC55Ack("+CCLK:"))
    					 {
    						unsigned char Tbuf[8]=""; 
    					        unsigned char i=0;
    						unsigned char TimeZoneEn=0;
    						for(i=0;i<6;i++)
    						{
    							Tbuf[i]=((*(pt_rssi+3*i)-0x30)*10)+(*(pt_rssi+3*i+1)-0x30);
    						}
    						Tbuf[6]=*(pt_rssi+3*i-1);//+-
    						Tbuf[7]=((*(pt_rssi+3*i)-0x30)*10)+(*(pt_rssi+3*i+1)-0x30);
    						
    					      	if(GMTtoLocalTime(Tbuf))
    						{
    							if((Tbuf[0]>0x16)&&(Tbuf[1]<0x13)&&(Tbuf[1]>0x00)&&(Tbuf[2]<0x32)&&
    							 (Tbuf[2]>0x00)&&(Tbuf[3]<0x24)&&(Tbuf[4]<0x60)&&(Tbuf[5]<0x60))
    							{
    								rtc_valve.year=Tbuf[0];
    								rtc_valve.month=Tbuf[1];
    								rtc_valve.day=Tbuf[2];
    								rtc_valve.hour=Tbuf[3];
    								rtc_valve.min=Tbuf[4];
    								rtc_valve.sec=Tbuf[5];
							        R_RTC_Set_CounterValue(rtc_valve);	//设置时间//
    							}
    						}
    					}
    					netMisc.AT_Waiting=netMisc.NoAckTimes=0;
    					netComps.recv_RxSt=&netComps.recv_base_pt[0];	  
    					netMisc.Flag_St=netMisc._Flag_St+1;
    				}  
    			}
			
			else if (netMisc._Flag_St==24)//fireWare update...
			{
			        if(!netMisc.AT_Waiting) 
				{
					RESET_FROM_M72D_RX_SP;
					//DIS_UPF_ING; 
					netComps.op_window_time= MD_MODULE_UPDATE_MAX_TIME;
					netComps.AckTmr = MD_MODULE_UPDATE_MAX_TIME;	//WaitingTime*100mS//
					netMisc.AT_Waiting = 1;		
				}	
				else if(!netComps.AckTmr)
				{
					//ReturnDataIrq=1;//R
				}
				else if(Check_MC55Ack("FIRMWARE UPDATE OVER\r\n"))
				{
					if(Check_MC55Ack("FIRMWARE DOWNLOAD FAILED\r\n"))
					{
						//DIS_UPF_ER; 
						netComps.AckTmr =4;
						RESET_FROM_M72D_RX_SP;
					}
					else if(Check_MC55Ack("FIRMWARE UPDATE FAILED\r\n"))
					{
						//DIS_UPF_ER; 
						netComps.AckTmr =4;
						RESET_FROM_M72D_RX_SP;
					}
					else if(Check_MC55Ack("FIRMWARE UPDATE SUCCESS\r\n"))
					{
						//DIS_UPF; 
						netComps.AckTmr =4;
						RESET_FROM_M72D_RX_SP;
					}
				}  
			}
			break;
		
		  default : 
			 NB_IOT_MOUDLE_TURN_OFF;
	                 break;
		  
	}
}
	
void net_task_handle(void)
{
	if(netComps.St._bit.on)
	{
		netComps.St._bit.on=0;
		netComps.St._bit.running=1;
		
		//if(GSMPWRCTL_pin)
		{
			NB_IOT_MOUDLE_TURN_ON;//turn modle
		}
//		else
		{
		//	NB_IOT_MOUDLE_TURN_ON_FAST;
		}
		
		enable_net_receive();
		SetIpAndPort();
		netMisc.SocketNum='\0';
		//netComps.St._bit.disCode=0;
		netComps.op_window_time = MD_MODULE_OP_MAX_TIME;	//5·??ó//
		netComps.St._bit.windowTimeOut=0;
		netComps.St._bit.noResponse=0;
	}
	
	if(netComps.St._bit.off)
	{
		netComps.St._bit.off=0;
		netComps.St._bit.offing=1;
		md_turn_off();
	}
	
	if(netComps.St._bit.running)
	{
		if(!netComps.op_window_time)
		{
			if(!netComps.St._bit.offing)
			{
				netComps.St._bit.windowTimeOut=1;
			}
		}	
		Srv_GSM();
	}
	
}


netComps_t netComps=
{
	{0},//st
	EM_DIS_ACT,
	&netMisc.recv_msg_buf,//unsigned char *const msg;//Passed to the protocol layer point
	0,//unsigned int   msglen;
	netMisc.recv_buf,//igned char *const recv_base_pt;//Passed to uart recv  point
   &netMisc.recv_Idx,//
	
	0,//unsigned char *recv_RxSt;
	30,//unsigned short AckTmr;//netComps.AckTmr
	MD_MODULE_OP_MAX_TIME,
	&netMisc.Flag_St,//unsigned char const *run_st;
	
	{0},//netinfo
	net_task_handle,//void (*const task_handle)(void);
};

