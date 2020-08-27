#ifndef HUM_H

//#define   MD_09_PIN_LCD_SEG06   SEG6
#define   MD_10_PIN_LCD_SEG04   SEG4
#define   MD_11_PIN_LCD_SEG05   SEG5
#define   MD_12_PIN_LCD_SEG08   SEG8
#define   MD_13_PIN_LCD_SEG09   SEG9
#define   MD_14_PIN_LCD_SEG10   SEG10
#define   MD_15_PIN_LCD_SEG11   SEG11
#define   MD_16_PIN_LCD_SEG12   SEG12
#define   MD_17_PIN_LCD_SEG13   SEG13
#define   MD_18_PIN_LCD_SEG14   SEG14
#define   MD_19_PIN_LCD_SEG15   SEG15
#define   MD_20_PIN_LCD_SEG16   SEG16
#define   MD_21_PIN_LCD_SEG17   SEG17
#define   MD_22_PIN_LCD_SEG18   SEG18
#define   MD_23_PIN_LCD_SEG19   SEG19
#define   MD_24_PIN_LCD_SEG20   SEG20
#define   MD_25_PIN_LCD_SEG21   SEG21
#define   MD_26_PIN_LCD_SEG22   SEG22
#define   MD_27_PIN_LCD_SEG23   SEG23
#define   MD_28_PIN_LCD_SEG24   SEG24
#define   MD_29_PIN_LCD_SEG26   SEG26
#define   MD_30_PIN_LCD_SEG27   SEG27
#define   MD_31_PIN_LCD_SEG28   SEG28
#define   MD_32_PIN_LCD_SEG30   SEG30
#define   MD_33_PIN_LCD_SEG31   SEG31
#define   MD_34_PIN_LCD_SEG32   SEG32
#define   MD_35_PIN_LCD_SEG33   SEG33
#define   MD_36_PIN_LCD_SEG34   SEG34
#define   MD_37_PIN_LCD_SEG35   SEG35
#define   MD_38_PIN_LCD_SEG36   SEG36
#define   MD_39_PIN_LCD_SEG37   SEG37
#define   MD_40_PIN_LCD_SEG38   SEG38
#define   MD_41_PIN_LCD_SEG39   SEG39
#define   MD_42_PIN_LCD_SEG44   SEG44
#define   MD_43_PIN_LCD_SEG45   SEG45
#define   MD_44_PIN_LCD_SEG48   SEG48
#define   MD_45_PIN_LCD_SEG49   SEG49
#define   MD_46_PIN_LCD_SEG50   SEG50


#define   MD_DIS_T1_TOTAL               MD_46_PIN_LCD_SEG50|=0x01
#define   MD_DIS_T2_TON                 MD_19_PIN_LCD_SEG15|=0x10
#define   MD_DIS_T3_M_3                 MD_19_PIN_LCD_SEG15|=0x20
#define   MD_DIS_T4_L                   MD_19_PIN_LCD_SEG15|=0x40
#define   MD_DIS_T5_ML                  MD_18_PIN_LCD_SEG14|=0x80
#define   MD_DIS_T6_KG                  MD_19_PIN_LCD_SEG15|=0x80
#define   MD_DIS_T7_MPA                 MD_19_PIN_LCD_SEG15|=0x02
#define   MD_DIS_T8_KPA                 MD_19_PIN_LCD_SEG15|=0x01
#define   MD_DIS_T9_DELTAP              MD_26_PIN_LCD_SEG22|=0x80
#define   MD_DIS_T10_KPA                MD_26_PIN_LCD_SEG22|=0x40
#define   MD_DIS_T11_MPA                MD_26_PIN_LCD_SEG22|=0x20
#define   MD_DIS_T12_PRESS              MD_40_PIN_LCD_SEG38|=0x20
#define   MD_DIS_T13_FLOW               MD_40_PIN_LCD_SEG38|=0x10
#define   MD_DIS_T14_COMMUNICATION      MD_40_PIN_LCD_SEG38|=0x08
#define   MD_DIS_T15_MAX                MD_40_PIN_LCD_SEG38|=0x04
#define   MD_DIS_T16_P                  MD_40_PIN_LCD_SEG38|=0x02
#define   MD_DIS_T17_DELTAP             MD_40_PIN_LCD_SEG38|=0x01
#define   MD_DIS_T18_CELSIUS            MD_26_PIN_LCD_SEG22|=0x10

#define   MD_DIS_S1_M_3_H               MD_33_PIN_LCD_SEG31|=0x01
#define   MD_DIS_S2_NM_3_H              MD_33_PIN_LCD_SEG31|=0x02
#define   MD_DIS_S3_L_H                 MD_33_PIN_LCD_SEG31|=0x04
#define   MD_DIS_S4_L_M                 MD_33_PIN_LCD_SEG31|=0x08
#define   MD_DIS_S5_NL_H                MD_33_PIN_LCD_SEG31|=0x10
#define   MD_DIS_S6_KG_H                MD_33_PIN_LCD_SEG31|=0x20
#define   MD_DIS_S7_T_H                 MD_34_PIN_LCD_SEG32|=0x80
#define   MD_DIS_S8_TEMP                MD_33_PIN_LCD_SEG31|=0x40
#define   MD_DIS_S9_SM_3_H              MD_33_PIN_LCD_SEG31|=0x80

//#define   MD_DIS_P1_DOT                 MD_21_PIN_LCD_SEG23|=0x08
#define   MD_DIS_P2_DOT                 MD_10_PIN_LCD_SEG04|=0x80
#define   MD_DIS_P3_DOT                 MD_11_PIN_LCD_SEG05|=0x80
#define   MD_DIS_P4_DOT                 MD_12_PIN_LCD_SEG08|=0x80
#define   MD_DIS_P5_DOT                 MD_13_PIN_LCD_SEG09|=0x80
#define   MD_DIS_P6_DOT                 MD_14_PIN_LCD_SEG10|=0x80
#define   MD_DIS_P7_DOT                 MD_15_PIN_LCD_SEG11|=0x80
#define   MD_DIS_P8_DOT                 MD_16_PIN_LCD_SEG12|=0x80
#define   MD_DIS_P9_DOT                 MD_17_PIN_LCD_SEG13|=0x80
#define   MD_DIS_P12_DOT                MD_21_PIN_LCD_SEG17|=0x01
#define   MD_DIS_P13_DOT                MD_22_PIN_LCD_SEG18|=0x01
#define   MD_DIS_P14_DOT                MD_23_PIN_LCD_SEG19|=0x01
#define   MD_DIS_P15_DOT                MD_24_PIN_LCD_SEG20|=0x01
#define   MD_DIS_P16_DOT                MD_25_PIN_LCD_SEG21|=0x01
#define   MD_DIS_P18_DOT                MD_45_PIN_LCD_SEG49|=0x01
#define   MD_DIS_P19_DOT                MD_44_PIN_LCD_SEG48|=0x01
#define   MD_DIS_P20_DOT                MD_43_PIN_LCD_SEG45|=0x01
#define   MD_DIS_P21_DOT                MD_42_PIN_LCD_SEG44|=0x01
#define   MD_DIS_P22_DOT                MD_41_PIN_LCD_SEG39|=0x01
#define   MD_DIS_P23_DOT                MD_39_PIN_LCD_SEG37|=0x80
#define   MD_DIS_P24_DOT                MD_38_PIN_LCD_SEG36|=0x80
#define   MD_DIS_P25_DOT                MD_37_PIN_LCD_SEG35|=0x80
#define   MD_DIS_P26_DOT                MD_36_PIN_LCD_SEG34|=0x80
#define   MD_DIS_P27_DOT                MD_35_PIN_LCD_SEG33|=0x80
#define   MD_DIS_P29_DOT                MD_32_PIN_LCD_SEG30|=0x80
#define   MD_DIS_P30_DOT                MD_31_PIN_LCD_SEG28|=0x80
#define   MD_DIS_P31_DOT                MD_30_PIN_LCD_SEG27|=0x80
#define   MD_DIS_P32_DOT                MD_29_PIN_LCD_SEG26|=0x80

#define   MD_DIS_COL1                   MD_19_PIN_LCD_SEG15|=0x04
#define   MD_DIS_COL2                   MD_19_PIN_LCD_SEG15|=0x08

#define   MD_DIS_D0_BAT                 MD_26_PIN_LCD_SEG22|=0x01
#define   MD_DIS_D1_BAT                 MD_26_PIN_LCD_SEG22|=0x04
#define   MD_DIS_D2_BAT                 MD_26_PIN_LCD_SEG22|=0x02
#define   MD_DIS_D3_BAT                 MD_26_PIN_LCD_SEG22|=0x08

#define   MD_DIS_N1_CSQ                 MD_27_PIN_LCD_SEG23|=0x01
#define   MD_DIS_N2_CSQ                 MD_27_PIN_LCD_SEG23|=0x02
#define   MD_DIS_N3_CSQ                 MD_27_PIN_LCD_SEG23|=0x04
#define   MD_DIS_N4_CSQ                 MD_27_PIN_LCD_SEG23|=0x08


#define   MD_HIDE_T1_TOTAL               MD_46_PIN_LCD_SEG50&=~0x01
#define   MD_HIDE_T2_TON                 MD_19_PIN_LCD_SEG15&=~0x10
#define   MD_HIDE_T3_M_3                 MD_19_PIN_LCD_SEG15&=~0x20
#define   MD_HIDE_T4_L                   MD_19_PIN_LCD_SEG15&=~0x40
#define   MD_HIDE_T5_ML                  MD_18_PIN_LCD_SEG14&=~0x80
#define   MD_HIDE_T6_KG                  MD_19_PIN_LCD_SEG15&=~0x80
#define   MD_HIDE_T7_MPA                 MD_19_PIN_LCD_SEG15&=~0x02
#define   MD_HIDE_T8_KPA                 MD_19_PIN_LCD_SEG15&=~0x01
#define   MD_HIDE_T9_DELTAP              MD_26_PIN_LCD_SEG22&=~0x80
#define   MD_HIDE_T10_KPA                MD_26_PIN_LCD_SEG22&=~0x40
#define   MD_HIDE_T11_MPA                MD_26_PIN_LCD_SEG22&=~0x20
#define   MD_HIDE_T12_PRESS              MD_40_PIN_LCD_SEG38&=~0x20
#define   MD_HIDE_T13_FLOW               MD_40_PIN_LCD_SEG38&=~0x10
#define   MD_HIDE_T14_COMMUNICATION      MD_40_PIN_LCD_SEG38&=~0x08
#define   MD_HIDE_T15_MAX                MD_40_PIN_LCD_SEG38&=~0x04
#define   MD_HIDE_T16_P                  MD_40_PIN_LCD_SEG38&=~0x02
#define   MD_HIDE_T17_DELTAP             MD_40_PIN_LCD_SEG38&=~0x01
#define   MD_HIDE_T18_CELSIUS            MD_26_PIN_LCD_SEG22&=~0x10

#define   MD_HIDE_S1_M_3_H               MD_33_PIN_LCD_SEG31&=~0x01
#define   MD_HIDE_S2_NM_3_H              MD_33_PIN_LCD_SEG31&=~0x02
#define   MD_HIDE_S3_L_H                 MD_33_PIN_LCD_SEG31&=~0x04
#define   MD_HIDE_S4_L_M                 MD_33_PIN_LCD_SEG31&=~0x08
#define   MD_HIDE_S5_NL_H                MD_33_PIN_LCD_SEG31&=~0x10
#define   MD_HIDE_S6_KG_H                MD_33_PIN_LCD_SEG31&=~0x20
#define   MD_HIDE_S7_T_H                 MD_34_PIN_LCD_SEG32&=~0x80
#define   MD_HIDE_S8_TEMP                MD_33_PIN_LCD_SEG31&=~0x40
#define   MD_HIDE_S9_SM_3_H              MD_33_PIN_LCD_SEG31&=~0x80

//#define   MD_HIDE_P1_DOT                 MD_21_PIN_LCD_SEG23&=~0x08
#define   MD_HIDE_P2_DOT                 MD_10_PIN_LCD_SEG04&=~0x80
#define   MD_HIDE_P3_DOT                 MD_11_PIN_LCD_SEG05&=~0x80
#define   MD_HIDE_P4_DOT                 MD_12_PIN_LCD_SEG08&=~0x80
#define   MD_HIDE_P5_DOT                 MD_13_PIN_LCD_SEG09&=~0x80
#define   MD_HIDE_P6_DOT                 MD_14_PIN_LCD_SEG10&=~0x80
#define   MD_HIDE_P7_DOT                 MD_15_PIN_LCD_SEG11&=~0x80
#define   MD_HIDE_P8_DOT                 MD_16_PIN_LCD_SEG12&=~0x80
#define   MD_HIDE_P9_DOT                 MD_17_PIN_LCD_SEG13&=~0x80
#define   MD_HIDE_P12_DOT                MD_21_PIN_LCD_SEG17&=~0x01
#define   MD_HIDE_P13_DOT                MD_22_PIN_LCD_SEG18&=~0x01
#define   MD_HIDE_P14_DOT                MD_23_PIN_LCD_SEG19&=~0x01
#define   MD_HIDE_P15_DOT                MD_24_PIN_LCD_SEG20&=~0x01
#define   MD_HIDE_P16_DOT                MD_25_PIN_LCD_SEG21&=~0x01
#define   MD_HIDE_P18_DOT                MD_45_PIN_LCD_SEG49&=~0x01
#define   MD_HIDE_P19_DOT                MD_44_PIN_LCD_SEG48&=~0x01
#define   MD_HIDE_P20_DOT                MD_43_PIN_LCD_SEG45&=~0x01
#define   MD_HIDE_P21_DOT                MD_42_PIN_LCD_SEG44&=~0x01
#define   MD_HIDE_P22_DOT                MD_41_PIN_LCD_SEG39&=~0x01
#define   MD_HIDE_P23_DOT                MD_39_PIN_LCD_SEG37&=~0x80
#define   MD_HIDE_P24_DOT                MD_38_PIN_LCD_SEG36&=~0x80
#define   MD_HIDE_P25_DOT                MD_37_PIN_LCD_SEG35&=~0x80
#define   MD_HIDE_P26_DOT                MD_36_PIN_LCD_SEG34&=~0x80
#define   MD_HIDE_P27_DOT                MD_35_PIN_LCD_SEG33&=~0x80
#define   MD_HIDE_P29_DOT                MD_32_PIN_LCD_SEG30&=~0x80
#define   MD_HIDE_P30_DOT                MD_31_PIN_LCD_SEG28&=~0x80
#define   MD_HIDE_P31_DOT                MD_30_PIN_LCD_SEG27&=~0x80
#define   MD_HIDE_P32_DOT                MD_29_PIN_LCD_SEG26&=~0x80

#define   MD_HIDE_COL1                   MD_19_PIN_LCD_SEG15&=~0x04
#define   MD_HIDE_COL2                   MD_19_PIN_LCD_SEG15&=~0x08

#define   MD_HIDE_D0_BAT                 MD_26_PIN_LCD_SEG22&=~0x01
#define   MD_HIDE_D1_BAT                 MD_26_PIN_LCD_SEG22&=~0x04
#define   MD_HIDE_D2_BAT                 MD_26_PIN_LCD_SEG22&=~0x02
#define   MD_HIDE_D3_BAT                 MD_26_PIN_LCD_SEG22&=~0x08

#define   MD_HIDE_N1_CSQ                 MD_27_PIN_LCD_SEG23&=~0x01
#define   MD_HIDE_N2_CSQ                 MD_27_PIN_LCD_SEG23&=~0x02
#define   MD_HIDE_N3_CSQ                 MD_27_PIN_LCD_SEG23&=~0x04
#define   MD_HIDE_N4_CSQ                 MD_27_PIN_LCD_SEG23&=~0x08




   



#define   MD_HIDE_DISP                  (sizeof(hum_comps.SEG_TAB)-1)
#define   MD_DIS_P                      16
#define   MD_DIS_U                      17
#define   MD_DIS_T                      18
#define   MD_DIS_N                      19
#define   MD_DIS_R                      20









#define   MD_FL_VER                      2
typedef enum  
{
	EM_NORMAL_MODE=0,
	EM_DEBUG_MODE,
	EM_PWD_MODE,
	EM_CAL_QUERY_MODE,   // Calibration  query mode
	EM_CAL_MODIFY_MODE,  //Calibration modify mode
	EM_PARAM_QUERY_MODE, //param query mode
	EM_PARAM_MODIFY_MODE,//param modify mode
	EM_SELF_TEST_MODE,    //test mode used to check ad54x0,e2prom,ds18b20
	EM_REPORT_MODE,
	EM_AIR_LEAK_MODE,
	EM_LORA_MODE
	//....TODO.....
	
}mode_type_t;

typedef struct //Handling of keys in different modes
{
	char *desc;
	struct _MODE_COMPONENTS *this;
	mode_type_t type;
	void (*on_s_key)(void);
	void (*on_m_key)(void);
	void (*on_j_key)(void);
	void (*on_long_s_key)(void);
	void (*on_long_m_key)(void);
	void (*on_long_j_key)(void);
	void (*on_long_s_and_j_key)(void);
	void (*display)(unsigned char opt);
	unsigned char dis_option;
	unsigned char displayTimer;
}mode_comps_t;

typedef enum
{
	EM_NO_KEY,               //em:enum 
	EM_SHORT_KEY,
	EM_LONG_KEY
} key_type_t;

/*	  display erea
      |******0 erea******|	   |********1 erea**********|
                 |**************2 erea****************|
*/

typedef struct _HUM_COMPONENTS
{
	char *desc;
	struct _HUM_COMPONENTS *this;
	int   do_init;//Whether to initialize,1:init 0,no init
	void (*const init)(struct _HUM_COMPONENTS *);
	unsigned long count;            //task_handle Called counter
	
	/******************************lcd seg define******************************/
    const unsigned char  SEG_TAB[23];
    const unsigned char  SEG_TAB_R[23];
    unsigned char dig0_0;
    unsigned char dig0_1;
    unsigned char dig0_2;
    unsigned char dig0_3;
    unsigned char dig0_4;
    unsigned char dig0_5;
    char          dot0_pos;
	
	unsigned char dig1_0;
	unsigned char dig1_1;
	unsigned char dig1_2;
	unsigned char dig1_3;
	unsigned char dig1_4;
    char          dot1_pos;
	
	unsigned char dig2_0;
	unsigned char dig2_1;
	unsigned char dig2_2;
	unsigned char dig2_3;
	unsigned char dig2_4;
	unsigned char dig2_5;
	char          dot2_pos;

	unsigned char dig3_0;
	unsigned char dig3_1;
	unsigned char dig3_2;
	unsigned char dig3_3;
	unsigned char dig3_4;
	unsigned char dig3_5;
	char          dot3_pos;

	unsigned char dig4_0;
	unsigned char dig4_1;
	unsigned char dig4_2;
	unsigned char dig4_3;
	unsigned char dig4_4;
	unsigned char dig4_5;
	unsigned char dig4_6;
	unsigned char dig4_7;
	unsigned char dig4_8;
	unsigned char dig4_9;
    char          dot4_pos;
    union 
    {
    	unsigned long All;
    	struct{
                unsigned char cur0		            :1;
                unsigned char dis0		            :1;
                unsigned char cur1		            :1;
                unsigned char dis1		            :1;
                unsigned char cur2		            :1;
                unsigned char dis2                  :1;
                unsigned char cur3                  :1;
                unsigned char dis3                  :1;
               
                
                unsigned char refresh_date		    :1;
                unsigned char refresh_time		    :1;
                unsigned char refresh_device_id_high:1;
                unsigned char refresh_device_id_low :1;
                unsigned char refresh_temp		    :1;
                unsigned char refresh_press		    :1;//switch press press unit,data ,time
                unsigned char refresh_option      	:1;
                unsigned char refresh_ver      		:1;
                
                unsigned char air_leak_mode_refresh_timer          	    :1;	
                unsigned char refresh_cal_param     :1;
                unsigned char refresh_batt_symbol   :1;
                unsigned char refresh_press_adc	    :1;//debug mode use
                unsigned char refresh_temp_adc	    :1;//debug mode use
                unsigned char test_ok               :1;
                unsigned char refresh_press_max     :1;
                unsigned char refresh_err_code      :1;

                unsigned char cur4		            :1;
                unsigned char dis4		            :1;
                unsigned char refresh_deltaP        :1;
                unsigned char refresh_deltaP_max    :1;
                unsigned char refresh_deltaP_adc	:1;//debug mode use
                unsigned char refresh_deltaP_cal_param     :1;
                unsigned char refresh_res_cal_param :1;
               

    	}_bit;
    }dis_oper_mark;
    union 
    {
    	unsigned long All;
    	struct{
                unsigned char refresh_flow		            :1;
                unsigned char refresh_total		            :1;
                
               

    	}_bit;
    }dis_extern_oper_mark;
    
	int cursor_0;//0 line cursor position
	int cursor_1;
	int cursor_2;
	int cursor_3;
	int cursor_4;
	int cursor_0_count;
	int cursor_1_count;
	int cursor_2_count;
	int cursor_3_count;
	int cursor_4_count;
	/*******************end lcd seg define **************************/ 
	
	mode_type_t   current_mode;
	key_type_t key_type;//[EM_NO_KEY, EM_SHORT_KEY,EM_LONG_KEY]
	int key;
	int  (*const get_key)(struct _HUM_COMPONENTS *);
    void (*const task_handle)(void);
	
	
	//TODO
	
}hum_comps_t;
extern mode_comps_t  mode_comps[];
extern hum_comps_t hum_comps;
extern void clr_lcd(void);
extern void enter_normal_mode(void);
extern void enter_air_leak_mode(void);
extern void enter_cal_modify_mode(void);
void enter_cal_query_mode(void);
extern void enter_report_mode(void);
extern void enter_lora_mode(void);
#endif