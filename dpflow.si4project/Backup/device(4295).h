#ifndef  DEVICE_H
#define  DEVICE_H

#define  MD_3_6V   0
#define  MD_4_5V   1
#define  MD_DEVICE_BATT          MD_3_6V
 
#define  MD_AIR_LEAK             0     //4.5V 
#define  MD_LORA                 1     //3.6V
#define  MD_NORMAL               2     //3.6V  switch unit
#define  MD_NBIOT                3     //3.6V  switch unit
#define  MD_PRODUCT_NAME         MD_NORMAL  //

#define  MD_DIS5                5
#define  MD_DIS4                4
#define  MD_DIS_DIG              MD_DIS5

//#define   MD_MODBUS            //modbus rs485 use irc interface,no sleep
//#define   MD_NO_LCD
//#define   MD_TEMP		        //temp display line 2
//#define   MD_IGNORE_ALL_ERR




#define   MD_ADC_MAX_POS               6
#define   MD_SET_REF_3030C_ON       P1_bit.no6=1
#define   MD_SET_REF_3030C_OFF      P1_bit.no6=0

#define   MD_SET_BAT_CTL_ON         P4_bit.no4=1
#define   MD_SET_BAT_CTL_OFF        P4_bit.no4=0



#define MD_MEMORY_TEST_START_ADDR                      0x0000   //
#define MD_CALIBRATION_PARAM_START_ADDR                0x0030   //64bytes
#define MD_RES_CALIBRATION_PARAM_START_ADDR            0x0070   //64bytes
#define MD_ALARM_PARAM_START_ADDR                      0x00b0   //32
#define MD_REPORT_PARAM_START_ADDR                     0x00d0   //16
#define MD_ACCESS_PARAM_START_ADDR                     0x00e0   //16
#define MD_DEVICE_ADDR_START_ADDR                      0x00f0   //16

#define MD_TIME_SEG_DATA_PARAM_START_ADDR              0x0100
#define MD_MODBUS_PARAM_START_ADDR                     0x0110
#define MD_DEVICE_COE_START_ADDR                       0x0120

#define MD_DELTAP_CALIBRATION_PARAM_START_ADDR         0x0140


#define MD_TIME_AIR_LEAK_PARAM_START_ADDR              0x0180

#define MD_SYSTEM_TIME_START_ADDR                      0x01c0
#define MD_DEVICE_SENSOR_START_ADDR                    0x01d0

#define MD_LORA_PARAM_START_ADDR                       0x01e0


  
#define MD_TIME_SEG_DATA_START_ADDR                    0x0200
#define MD_TIME_SEG_DATA_END_ADDR                      0x0400 




  
  typedef struct
  {
      long x[4];
      long y[4];
      long t[4];//Temperature value for temperature compensation
      char dot;
      unsigned char unit;//0Mpa ,1Kpa,0x81 Kpa m
      unsigned char is_calibrated; 
      unsigned char cs;
  }
  calibration_param_t;

  typedef struct
  {
      long x[4];
      long y[4];
      long t[4];//Temperature value for temperature compensation
      char dot;
      unsigned char unit;
      unsigned char is_calibrated; 
      unsigned char cs;
  }
  res_calibration_param_t;

  typedef struct
   {
       long x[4];
       long y[4];
       long t[4];//Temperature value for temperature compensation
       char dot;
       unsigned char unit;
       unsigned char is_calibrated; 
       unsigned char cs;
   }
   deltaP_calibration_param_t;
   
typedef enum  
{
	EM_CAL_PRESS=0,
	EM_CAL_RES,
	EM_CAL_DELTAP
	//....TODO.....
	
}cal_type_t;
  
  typedef struct
  {
      unsigned char min;
      unsigned char hour;
      unsigned char hour_Interval;
      int disFactor;
      unsigned char cs;
  }
  report_param_t;
 

  typedef struct
  {
      unsigned char ip[4];
      unsigned int  port;
      unsigned char cs;
  }
  access_param_t;
 

  typedef struct 
  {
      long press_high_upper;
      long press_high_lower;
      long press_low_upper;
      long press_low_lower;
      int  unit;

      int temp_high;
      int temp_low;
      unsigned char cs;
  }
  alarm_param_t;
  
  typedef struct
  {
        unsigned char year;
        unsigned char month;
        unsigned char day;
        unsigned char hour;
        unsigned char min;
        unsigned char sec;
        unsigned char cs;
  }system_time_t;

  
  typedef struct
  {
        long total;
        long flow;
        int  store_index;//Stored offset address,When the memory is broken, add 1 automatically
        char cs;
        
  }dpf_data_t;

  typedef struct
  {
        long k[4];
        long A1;
        long A2;
        long p;      // kg/m^3 current density
        long p20;    //kg/m^3 density 20��
        long PA;     //Working condition atmospheric pressure Mpa

        long _4_20ma_lower_limit;
        long _4_20ma_upper_limit;
        char _4_20ma_dir;
        
        char bT;//0 other,1 Saturated steam, 2 superheated steam
        char tE;
        char pE;
        char Fu;//flow unit 0 kg/h,1 t/h,2 m3/h
        char Fd;//flow dot
        char Td;//total dot ,unit is the same flow 
        char cs;
        
  }dpf_param_t;
   
  typedef struct
  {
        long unit;//0:Mpa 1:Kpa, 2:psi ,3 bar
        long Constant_pressure_value;
        long buck_delta_value;
        long buck_percent;
        long work_mode;//0 Constant_pressure_value,1 buck_delta_value 2 buck_percent
        long count_direction;//0,up count,1,down count 
        long delay_timer;
        long timer;
        unsigned char cs;
  }air_leak_param_t;  
                        
  typedef struct
  {
         union 
        {
        	unsigned char All;
        	struct
        	{
                 unsigned char test_finished      :1;
                 unsigned char count_en		    :1;
                 unsigned char isWarn		        :1;
            }_bit;
        }sw;
        long  ref_press;
        long  press_set_format_valve;
        int   delay_timer;
        int   timer;
   }air_leak_t;



 typedef struct 
 {
     unsigned char addr[7];
     unsigned char cs;
 }
 device_addr_t;

 typedef struct 
 {
     unsigned char ser_num[8];
     unsigned char cs;
 }
 device_sensor_t;

typedef struct
{
    int  store_addr;
    char lastSampleTime[7];
    unsigned char cs;
}
TimeSegData_t;



typedef struct _DEVICE_COMPONENTS
{
	char *desc;
	struct _DEVICE_COMPONENTS  *const this;
	int   do_init;//Whether to initialize,1:init 0,no init
	unsigned char (*const init)(struct _DEVICE_COMPONENTS *);
	unsigned long count;            //Called counter
	
    union 
    {
    	unsigned long All;
    	struct{
                unsigned char e2prom_driver_err		            :1;
                unsigned char cs123x_driver_err		            :1;
                unsigned char cs1237_driver_err                              :1;
                unsigned char res3_err		                        :1;
                unsigned char temp_adc_stb	                    :1;
                unsigned char isPLowRealseTriggered             :1;
                unsigned char isPLowLessTriggered               :1;
                unsigned char isPHighRealseTriggered            :1;
                unsigned char isPHighOverTriggered              :1;
                unsigned char over_range	                    :1;
                unsigned char deltaP_over_range	            :1;
                unsigned char adc_busy	                        :1;
                unsigned char adc_stb	                        :1;
                unsigned char deltaP_adc_stb	                :1;
                unsigned char batt_status                       :1;
                unsigned char data_mode                         :1;

    	}_bit;
    }sw;
    int PHihgOverTimer;
    int PHihgRealseTimer;
    int PLowLessTimer;
    int PLowRealseTimer;
	
	long  ad1_convert_result[MD_ADC_MAX_POS];
	unsigned int  ad1_pos;
	long  ad2_convert_result[MD_ADC_MAX_POS];
	unsigned int  ad2_pos;
	long          ad1_ad2_average_result;//(ad1-ad2)/ad1_pos
	
	long  temp_p_convert_result[MD_ADC_MAX_POS];
	unsigned int  temp_p_pos;
	long  temp_n_convert_result[MD_ADC_MAX_POS];
	unsigned int  temp_n_pos;
	long          temp_p_temp_n_average_result;//(temp_p_convert_result-temp_n_convert_result)/temp_p_pos

    long  deltaP_p_convert_result[MD_ADC_MAX_POS];
    int   deltaP_p_pos;
    long  deltaP_n_convert_result[MD_ADC_MAX_POS];
    int   deltaP_n_pos;
    long  deltaP_p_deltaP_n_average_result;
	
	long  ad3_convert_result[MD_ADC_MAX_POS];
	unsigned int  ad3_pos;
	long          ad3_average_result;
	
	

	long ntc_valve;
	long ( * const calc_ad3_average)(struct _DEVICE_COMPONENTS  *const);
	long ( * const calc_ntc_valve)(struct _DEVICE_COMPONENTS  *const);//point to function calc_ntc( device_comps_t *const this) whith param adc_result[][] 
	int  ( * const calc_temp)  (struct _DEVICE_COMPONENTS     *const);//point to function calc_temp( sensor_comp_t *const this)with param ntc_valve
    long pt_valve;
    long ( * const calc_temp_p_temp_n_average)(struct _DEVICE_COMPONENTS  *const);
	long ( * const calc_pt_valve)(struct _DEVICE_COMPONENTS  *const);//
	int  ( * const calc_pt_temp)  (struct _DEVICE_COMPONENTS     *const);//

	int  current_temp;
	int  current_temp_n_1;
	int  current_temp_n_2;

    long ( *const calc_deltaP_p_deltaP_n_average)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
	long ( *const calc_current_deltaP)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    long  current_deltaP;  //Yn
    long  max_deltaP;
	long  current_deltaP_n_1;//Yn-1
	long  current_deltaP_n_2;//Yn-2


	
	long ( *const calc_ad1_ad2_average)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
	long ( *const calc_current_press)(struct _DEVICE_COMPONENTS *const ); //point to calc_signal_period(sensor_comp_t *const this)
    long  current_press;  //Yn
    long  max_press;
	long  current_press_n_1;//Yn-1
	long  current_press_n_2;//Yn-2

    struct 
    {
        int press;
        int temp;
        long press_clr_value;
        unsigned char cs;
    }coe;
    int (*read_coe)(void *,int );
    int (*save_coe)(void const *,int);
    int (*clr_press)(void);


    unsigned char batt;//batt voltage
    
	calibration_param_t calibration_param;
	calibration_param_t calibration_param_bak;
	cal_type_t cal_type;//0 cal press, 1,cal temp
	int (*read_calibration_param)(void *,int );
    int (*save_calibration_param)(void const *,int);

    res_calibration_param_t  res_calibration_param;
    res_calibration_param_t  res_calibration_param_bak;
    int (*read_res_calibration_param)(void *,int );
    int (*save_res_calibration_param)(void const *,int);

    deltaP_calibration_param_t  deltaP_calibration_param;
    deltaP_calibration_param_t  deltaP_calibration_param_bak;
    int (*read_deltaP_calibration_param)(void *,int );
    int (*save_deltaP_calibration_param)(void const *,int);
    

    dpf_param_t dpf_param;
    long current_flow;
    long current_total;
	
    report_param_t report_param;
    int (*read_report_param)(void *,int);
	int (*save_report_param)(void const *,int);
    
    access_param_t access_param;
    int (*read_access_param)(void *,int);
	int (*save_access_param)(void const *,int);
	
    alarm_param_t alarm_param;
	int (*read_alarm_param)(void *,int);
	int (*save_alarm_param)(void const *,int);

    device_addr_t device_addr;
    int (*read_device_addr)(void *,int);
	int (*save_device_addr)(void const *,int);

	device_sensor_t device_sensor;
    int (*read_device_sensor)(void *,int);
	int (*save_device_sensor)(void const *,int);
	

	TimeSegData_t TimeSegData;
	int (*read_time_seg_data_param)(void * , int);
	int (*save_time_seg_data_param)(void const * , int);

	system_time_t system_time;
	int (*read_system_time)(void * , int);
	int (*save_system_time)(void const * , int);

	air_leak_param_t  air_leak_param;
	int (*read_air_leak_param)(void * , int);
	int (*save_air_leak_param)(void const * , int);

	air_leak_t air_leak;
	
    int report_interval_timer;
	int _1s_timr_acc;
	

	void (*read_all_param)(struct _DEVICE_COMPONENTS  *const);
    
	void  ( *const output_debug_info)(struct _DEVICE_COMPONENTS   *const);//point to sensor_comps_output_debug_info(device_comps_t const *const this)
	char debug_info[32];
	void  ( *const task_handle)(void);//point to device_comps_task_handle
}device_comps_t; 

extern device_comps_t device_comps;
void _1s_task_handle(void);



#endif
