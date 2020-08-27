#ifndef PCF857X_H
#define PCF857X_H


typedef struct _PCF857X_COMPONENTS
{
	char *desc;
	struct  _PCF857X_COMPONENTS *this;
	int current_channel;
	union 
    {
    	unsigned char All;
    	struct
    	{
                unsigned char adc_updated		    :1;
                unsigned char running 	            :1;       
    	 }_bit;
    }sw;
	void (*const power_down)(void); //
	void (*const restart)(void);// 
	int (*const Init )(unsigned char dat);     
	int (*const read_adc)(long *const ad);   // 
	unsigned char (*const read_reg )(long * ad);         
	void (*const write_reg )(long *ad,unsigned char dat); 
	void (*const disale_interrup)(void);
	void (*const enable_interrupt)(void);
	void  ( *const task_handle)(void);//point to device_comps_task_handle
	
}pcf857x_comps_t;
extern pcf857x_comps_t pcf857x_comps;
#endif

