#ifndef  NET_H
#define  NET_H


typedef enum 
{
      EM_DIS_ACT=0,
      EM_DIS_OFF,
      EM_DIS_SEARCH_NET,
      EM_DIS_REGISTER_NET,
      EM_DIS_SEND, 
      EM_DIS_RECV 
}disCode_t;


typedef struct 
{   
    union 
    {
    	unsigned char All;
    	struct
    	{
    		uint8_t offing          :1;
    		uint8_t windowTimeOut	:1;
    		uint8_t noResponse	    :1;
    		uint8_t off		        :1;
    		uint8_t res1 		    :1;
    	    uint8_t on		        :1;	
    		uint8_t recvData	    :1;	
    		uint8_t running		    :1;
    		
    	}_bit;
    } St;
  
    disCode_t disCode;//0 act;1,off;2 search net;3 register net yet;4 send; 5 recv;
	unsigned char * const msg;//Passed to the protocol layer point
	unsigned int    msgLen;
	unsigned char *const recv_base_pt;//Passed to uart recv  point
	unsigned int  *recv_Idx_pt;
	
	unsigned char *recv_RxSt;
	unsigned short AckTmr;//BC35Srv_comps.AckTmr
	unsigned short op_window_time; 
	unsigned char const * const run_st;//point to bc35 run step

	struct 
    {
        unsigned char imei[20];
    	unsigned char imsi[20];
    	unsigned char iccid[20];
    	unsigned char firmVer[25];
    	unsigned char firmhdVer[25];
    	unsigned long earFcn;
    	unsigned long cellId;
    	unsigned int  pci;
    	char          rsrp;
    	char          rsrq;
    	unsigned char ecl;
    	char          sina;
    	unsigned char Csq;
    	unsigned char CsQ;
	
    }net_info;

    void (*const task_handle)(void);
    
}netComps_t;
extern netComps_t netComps;

void DataAdd30H(unsigned char *i, unsigned char const *j,unsigned int Len);



#endif

