#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_lcd.h"
#include "r_cg_sau.h"
#include "r_cg_rtc.h"
#include "r_cg_adc.h"
#include "hum.h"
#include "irc.h"
#include "lora.h"
#include "device.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
//#define  static 
          //key_io

int       MD_KEY_VDD;
#define   MD_K1                 P4_bit.no1   
#define   MD_K2                 P12_bit.no2
#define   MD_K3                 P12_bit.no1
#define   MD_KEY_MASK           (int)0x07
#define   MD_KEY_DATA           ((MD_K1<<2)+(MD_K2<<1)+MD_K3)
#define   MD_S_KEY              0x03//011
#define   MD_M_KEY              0x05//101
#define   MD_J_KEY              0x06//110


#define MD_GREEN_LIGHT    0
#define MD_RED_LIGHT      0
#define MD_SET_GREEN_LIGHT_ON    NOP()
#define MD_SET_GREEN_LIGHT_OFF   NOP()
#define MD_SET_RED_LIGHT_ON      NOP()
#define MD_SET_RED_LIGHT_OFF     NOP()

#define isLightOn(a)  (a)  


static void delay_us(int t)
{
	while(t>0)
	{
		NOP();NOP();
		t--;
	}
}

static void dis_all_unit()
{
    MD_DIS_T1_TOTAL;
    MD_DIS_T2_TON;
    MD_DIS_T3_M_3;
    MD_DIS_T4_L;
    MD_DIS_T5_ML;
    MD_DIS_T6_KG;
    MD_DIS_T7_MPA;
    MD_DIS_T8_KPA;
    MD_DIS_T9_DELTAP;
    MD_DIS_T10_KPA;
    MD_DIS_T11_MPA;
    MD_DIS_T12_PRESS;
    MD_DIS_T13_FLOW;
    MD_DIS_T14_COMMUNICATION;
    MD_DIS_T15_MAX;
    MD_DIS_T16_P;
    MD_DIS_T17_DELTAP;
    MD_DIS_T18_CELSIUS;
    ;
    MD_DIS_S1_M_3_H;
    MD_DIS_S2_NM_3_H;
    MD_DIS_S3_L_H;
    MD_DIS_S4_L_M;
    MD_DIS_S5_NL_H;
    MD_DIS_S6_KG_H;
    MD_DIS_S7_T_H;
    MD_DIS_S8_TEMP;
    MD_DIS_S9_SM_3_H;
    ;
    //MD_DIS_P1_DOT;
    MD_DIS_P2_DOT;
    MD_DIS_P3_DOT;
    MD_DIS_P4_DOT;
    MD_DIS_P5_DOT;
    MD_DIS_P6_DOT;
    MD_DIS_P7_DOT;
    MD_DIS_P8_DOT;
    MD_DIS_P9_DOT;
    MD_DIS_P12_DOT;
    MD_DIS_P13_DOT;
    MD_DIS_P14_DOT;
    MD_DIS_P15_DOT;
    MD_DIS_P16_DOT;
    MD_DIS_P18_DOT;
    MD_DIS_P19_DOT;
    MD_DIS_P20_DOT;
    MD_DIS_P21_DOT;
    MD_DIS_P22_DOT;
    MD_DIS_P23_DOT;
    MD_DIS_P24_DOT;
    MD_DIS_P25_DOT;
    MD_DIS_P26_DOT;
    MD_DIS_P27_DOT;
    MD_DIS_P29_DOT;
    MD_DIS_P30_DOT;
    MD_DIS_P31_DOT;
    MD_DIS_P32_DOT;
    ;
    MD_DIS_COL1;
    MD_DIS_COL2;
    ;
    MD_DIS_D0_BAT;
    MD_DIS_D1_BAT;
    MD_DIS_D2_BAT;
    MD_DIS_D3_BAT;
    ;
    MD_DIS_N1_CSQ;
    MD_DIS_N2_CSQ;
    MD_DIS_N3_CSQ;
    MD_DIS_N4_CSQ;
    
}
static void hide_all_unit(void)
{
    MD_HIDE_T1_TOTAL;
    MD_HIDE_T2_TON;
    MD_HIDE_T3_M_3;
    MD_HIDE_T4_L;
    MD_HIDE_T5_ML;
    MD_HIDE_T6_KG;
    MD_HIDE_T7_MPA;
    MD_HIDE_T8_KPA;
    MD_HIDE_T9_DELTAP;
    MD_HIDE_T10_KPA;
    MD_HIDE_T11_MPA;
    MD_HIDE_T12_PRESS;
    MD_HIDE_T13_FLOW;
    MD_HIDE_T14_COMMUNICATION;
    MD_HIDE_T15_MAX;
    MD_HIDE_T16_P;
    MD_HIDE_T17_DELTAP;
    MD_HIDE_T18_CELSIUS;
    ;
    MD_HIDE_S1_M_3_H;
    MD_HIDE_S2_NM_3_H;
    MD_HIDE_S3_L_H;
    MD_HIDE_S4_L_M;
    MD_HIDE_S5_NL_H;
    MD_HIDE_S6_KG_H;
    MD_HIDE_S7_T_H;
    MD_HIDE_S8_TEMP;
    MD_HIDE_S9_SM_3_H;
    ;
//    //MD_HIDE_P1_DOT;
//    MD_HIDE_P2_DOT;
//    MD_HIDE_P3_DOT;
//    MD_HIDE_P4_DOT;
//    MD_HIDE_P5_DOT;
//    MD_HIDE_P6_DOT;
//    MD_HIDE_P7_DOT;
//    MD_HIDE_P8_DOT;
//    MD_HIDE_P9_DOT;
//    MD_HIDE_P12_DOT;
//    MD_HIDE_P13_DOT;
//    MD_HIDE_P14_DOT;
//    MD_HIDE_P15_DOT;
//    MD_HIDE_P16_DOT;
//    MD_HIDE_P18_DOT;
//    MD_HIDE_P19_DOT;
//    MD_HIDE_P20_DOT;
//    MD_HIDE_P21_DOT;
//    MD_HIDE_P22_DOT;
//    MD_HIDE_P23_DOT;
//    MD_HIDE_P24_DOT;
//    MD_HIDE_P25_DOT;
//    MD_HIDE_P26_DOT;
//    MD_HIDE_P27_DOT;
//    MD_HIDE_P29_DOT;
//    MD_HIDE_P30_DOT;
//    MD_HIDE_P31_DOT;
//    MD_HIDE_P32_DOT;
//    ;
//    MD_HIDE_COL1;
//    MD_HIDE_COL2;
//    ;
//    MD_HIDE_D0_BAT;
//    MD_HIDE_D1_BAT;
//    MD_HIDE_D2_BAT;
//    MD_HIDE_D3_BAT;
//    ;
//    MD_HIDE_N1_CSQ;
//    MD_HIDE_N2_CSQ;
//    MD_HIDE_N3_CSQ;
//    MD_HIDE_N4_CSQ;
//    ;
//    ;


}

void clr_lcd(void)
{
    MD_10_PIN_LCD_SEG04=0;
    MD_11_PIN_LCD_SEG05=0;
    MD_12_PIN_LCD_SEG08=0;
    MD_13_PIN_LCD_SEG09=0;
    MD_14_PIN_LCD_SEG10=0;
    MD_15_PIN_LCD_SEG11=0;
    MD_16_PIN_LCD_SEG12=0;
    MD_17_PIN_LCD_SEG13=0;
    MD_18_PIN_LCD_SEG14=0;
    MD_19_PIN_LCD_SEG15=0;
    MD_20_PIN_LCD_SEG16=0;
    MD_21_PIN_LCD_SEG17=0;
    MD_22_PIN_LCD_SEG18=0;
    MD_23_PIN_LCD_SEG19=0;
    MD_24_PIN_LCD_SEG20=0;
    MD_25_PIN_LCD_SEG21=0;
    MD_26_PIN_LCD_SEG22=0;
    MD_27_PIN_LCD_SEG23=0;
    MD_28_PIN_LCD_SEG24=0;
    MD_29_PIN_LCD_SEG26=0;
    MD_30_PIN_LCD_SEG27=0;
    MD_31_PIN_LCD_SEG28=0;
    MD_32_PIN_LCD_SEG30=0;
    MD_33_PIN_LCD_SEG31=0;
    MD_34_PIN_LCD_SEG32=0;
    MD_35_PIN_LCD_SEG33=0;
    MD_36_PIN_LCD_SEG34=0;
    MD_37_PIN_LCD_SEG35=0;
    MD_38_PIN_LCD_SEG36=0;
    MD_39_PIN_LCD_SEG37=0;
    MD_40_PIN_LCD_SEG38=0;
    MD_41_PIN_LCD_SEG39=0;
    MD_42_PIN_LCD_SEG44=0;
    MD_43_PIN_LCD_SEG45=0;
    MD_44_PIN_LCD_SEG48=0;
    MD_45_PIN_LCD_SEG49=0;
    MD_46_PIN_LCD_SEG50=0;
}
static void MD_DISPLAY_WRITE_D0_0(unsigned char a)	//  D00  H   L 
{
    MD_34_PIN_LCD_SEG32&=0x80;
    MD_34_PIN_LCD_SEG32|=hum_comps.SEG_TAB[a];
}
static void MD_DISPLAY_WRITE_D0_1(unsigned char a)//  D01  H   L 
{
    MD_35_PIN_LCD_SEG33&=0x00;
    MD_35_PIN_LCD_SEG33|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot0_pos==1)  MD_DIS_P27_DOT;
}

static void MD_DISPLAY_WRITE_D0_2(unsigned char a)	//  D00  H   L 
{
    MD_36_PIN_LCD_SEG34&=0x00;
    MD_36_PIN_LCD_SEG34|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot0_pos==2)  MD_DIS_P26_DOT;
}
static void MD_DISPLAY_WRITE_D0_3(unsigned char a)//  D01  H   L 
{
    MD_37_PIN_LCD_SEG35&=0x00;
    MD_37_PIN_LCD_SEG35|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot0_pos==3)  MD_DIS_P25_DOT;
}

static void MD_DISPLAY_WRITE_D0_4(unsigned char a)	//  D00  H   L 
{
    MD_38_PIN_LCD_SEG36&=0x00;
    MD_38_PIN_LCD_SEG36|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot0_pos==4)  MD_DIS_P24_DOT;
 
}
static void MD_DISPLAY_WRITE_D0_5(unsigned char a)//  D01  H   L 
{
    MD_39_PIN_LCD_SEG37&=0x00;
    MD_39_PIN_LCD_SEG37|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot0_pos==5)  MD_DIS_P23_DOT;
}

static void MD_DISPLAY_WRITE_D1_0(unsigned char a)	//  D02  H   L 
{
   MD_28_PIN_LCD_SEG24&=0x80;
   MD_28_PIN_LCD_SEG24|=hum_comps.SEG_TAB[a];
}
static void MD_DISPLAY_WRITE_D1_1(unsigned char a)	//  D03  H   L 
{
    MD_29_PIN_LCD_SEG26&=0x00;
    MD_29_PIN_LCD_SEG26|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot1_pos==1)  MD_DIS_P32_DOT;
}

static void MD_DISPLAY_WRITE_D1_2(unsigned char a)//  D04  H   L  
{
    MD_30_PIN_LCD_SEG27&=0x00;
    MD_30_PIN_LCD_SEG27|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot1_pos==1)  MD_DIS_P31_DOT;
}

                 //  D10  H   L  
 static void  MD_DISPLAY_WRITE_D1_3(unsigned char a)   
 {
    MD_31_PIN_LCD_SEG28&=0x00;
    MD_31_PIN_LCD_SEG28|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot1_pos==1)  MD_DIS_P30_DOT;
 }
                 //  D10  H   L  
 static void  MD_DISPLAY_WRITE_D1_4(unsigned char a)   
 {
    MD_32_PIN_LCD_SEG30&=0x00;
    MD_32_PIN_LCD_SEG30|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot1_pos==1)  MD_DIS_P29_DOT;
 }

 
 static void  MD_DISPLAY_WRITE_D2_0(unsigned char a) //  D11  H   L
 {
    MD_46_PIN_LCD_SEG50&=0x01;
    MD_46_PIN_LCD_SEG50|=hum_comps.SEG_TAB[a];
 }
 
 static void  MD_DISPLAY_WRITE_D2_1(unsigned char a) //  D12  H   L 
 {
    MD_45_PIN_LCD_SEG49&=0x00;
    MD_45_PIN_LCD_SEG49|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot2_pos==1)((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?(MD_DIS_P18_DOT) :(MD_HIDE_P18_DOT)):(MD_DIS_P18_DOT);
 }
 static void  MD_DISPLAY_WRITE_D2_2(unsigned char a) //  D11  H   L
 {
    MD_44_PIN_LCD_SEG48&=0x00;
    MD_44_PIN_LCD_SEG48|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot2_pos==2)((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?(MD_DIS_P19_DOT) :(MD_HIDE_P19_DOT)):(MD_DIS_P19_DOT);
 }
 static void  MD_DISPLAY_WRITE_D2_3(unsigned char a) //  D12  H   L 
 {
    MD_43_PIN_LCD_SEG45&=0x00;
    MD_43_PIN_LCD_SEG45|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot2_pos==3) ((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?(MD_DIS_P20_DOT) :(MD_HIDE_P20_DOT)):(MD_DIS_P20_DOT);
 }
 static void  MD_DISPLAY_WRITE_D2_4(unsigned char a) //  D12  H   L 
 {
    MD_42_PIN_LCD_SEG44&=0x00;
    MD_42_PIN_LCD_SEG44|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot2_pos==4) ((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?(MD_DIS_P21_DOT) :(MD_HIDE_P21_DOT)):(MD_DIS_P21_DOT);
 }
 static void  MD_DISPLAY_WRITE_D2_5(unsigned char a) //  D12  H   L 
  {
    MD_41_PIN_LCD_SEG39&=0x00;
    MD_41_PIN_LCD_SEG39|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot2_pos==5) ((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?(MD_DIS_P22_DOT) :(MD_HIDE_P22_DOT)):(MD_DIS_P22_DOT);
  }


 static void  MD_DISPLAY_WRITE_D3_0(unsigned char a) //  D11  H   L
  {
     MD_20_PIN_LCD_SEG16&=0x01;
     MD_20_PIN_LCD_SEG16|=hum_comps.SEG_TAB[a];
  }
  
  static void  MD_DISPLAY_WRITE_D3_1(unsigned char a) //  D12  H   L 
  {
     MD_21_PIN_LCD_SEG17&=0x00;
     MD_21_PIN_LCD_SEG17|=hum_comps.SEG_TAB[a];
     if(hum_comps.dot3_pos==1)((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?(MD_DIS_P12_DOT) :(MD_HIDE_P12_DOT)):(MD_DIS_P12_DOT);
  }
  static void  MD_DISPLAY_WRITE_D3_2(unsigned char a) //  D11  H   L
  {
     MD_22_PIN_LCD_SEG18&=0x00;
     MD_22_PIN_LCD_SEG18|=hum_comps.SEG_TAB[a];
     if(hum_comps.dot3_pos==2)((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?(MD_DIS_P13_DOT) :(MD_HIDE_P13_DOT)):(MD_DIS_P13_DOT);
  }
  static void  MD_DISPLAY_WRITE_D3_3(unsigned char a) //  D12  H   L 
  {
     MD_23_PIN_LCD_SEG19&=0x00;
     MD_23_PIN_LCD_SEG19|=hum_comps.SEG_TAB[a];
     if(hum_comps.dot3_pos==3) ((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?(MD_DIS_P14_DOT) :(MD_HIDE_P14_DOT)):(MD_DIS_P14_DOT);
  }
  static void  MD_DISPLAY_WRITE_D3_4(unsigned char a) //  D12  H   L 
  {
     MD_24_PIN_LCD_SEG20&=0x00;
     MD_24_PIN_LCD_SEG20|=hum_comps.SEG_TAB[a];
     if(hum_comps.dot3_pos==4) ((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?(MD_DIS_P15_DOT) :(MD_HIDE_P15_DOT)):(MD_DIS_P15_DOT);
  }
  static void  MD_DISPLAY_WRITE_D3_5(unsigned char a) //  D12  H   L 
   {
     MD_25_PIN_LCD_SEG21&=0x00;
     MD_25_PIN_LCD_SEG21|=hum_comps.SEG_TAB[a];
     if(hum_comps.dot3_pos==5) ((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?(MD_DIS_P16_DOT) :(MD_HIDE_P16_DOT)):(MD_DIS_P16_DOT);
   }


 static void  MD_DISPLAY_WRITE_D4_0(unsigned char a) //  D11  H   L
 {
    MD_18_PIN_LCD_SEG14&=0x80;
    MD_18_PIN_LCD_SEG14|=hum_comps.SEG_TAB[a];
 }
 
 static void  MD_DISPLAY_WRITE_D4_1(unsigned char a) //  D12  H   L 
 {
    MD_17_PIN_LCD_SEG13&=0x00;
    MD_17_PIN_LCD_SEG13|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==1)  MD_DIS_P9_DOT ;
}
 static void  MD_DISPLAY_WRITE_D4_2(unsigned char a) //  D11  H   L
 {
    MD_16_PIN_LCD_SEG12&=0x00;
    MD_16_PIN_LCD_SEG12|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==2)  MD_DIS_P8_DOT ;
 }
 static void  MD_DISPLAY_WRITE_D4_3(unsigned char a) //  D12  H   L 
 {
    MD_15_PIN_LCD_SEG11&=0x00;
    MD_15_PIN_LCD_SEG11|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==3)  MD_DIS_P7_DOT ;
 }
 static void  MD_DISPLAY_WRITE_D4_4(unsigned char a) //  D12  H   L 
 {
    MD_14_PIN_LCD_SEG10&=0x00;
    MD_14_PIN_LCD_SEG10|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==4)  MD_DIS_P6_DOT ;
 }
 static void  MD_DISPLAY_WRITE_D4_5(unsigned char a) //  D12  H   L 
  {
    MD_13_PIN_LCD_SEG09&=0x00;
    MD_13_PIN_LCD_SEG09|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==5)  MD_DIS_P5_DOT ;
  }

static void MD_DISPLAY_WRITE_D4_6(unsigned char a)  //  D02  H   L 
 {
    MD_12_PIN_LCD_SEG08&=0x00;
    MD_12_PIN_LCD_SEG08|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==6)  MD_DIS_P4_DOT ;    
 }
 static void MD_DISPLAY_WRITE_D4_7(unsigned char a)  //  D03  H   L 
 {
    MD_11_PIN_LCD_SEG05&=0x00;
    MD_11_PIN_LCD_SEG05|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==7)  MD_DIS_P3_DOT ;
 }
 
 static void MD_DISPLAY_WRITE_D4_8(unsigned char a)//  D04  H   L  
 {
    MD_10_PIN_LCD_SEG04&=0x00;
    MD_10_PIN_LCD_SEG04|=hum_comps.SEG_TAB[a];
    if(hum_comps.dot4_pos==8)  MD_DIS_P2_DOT ;  
 }
 
  


static void display_line4_data()//display 4th line data
{
   #define MD_DIS_ALL_4_LINE_DATA (MD_DISPLAY_WRITE_D4_8(hum_comps.dig4_8),MD_DISPLAY_WRITE_D4_7(hum_comps.dig4_7),MD_DISPLAY_WRITE_D4_6(hum_comps.dig4_6),MD_DISPLAY_WRITE_D4_5(hum_comps.dig4_5),MD_DISPLAY_WRITE_D4_4(hum_comps.dig4_4),MD_DISPLAY_WRITE_D4_3(hum_comps.dig4_3),MD_DISPLAY_WRITE_D4_2(hum_comps.dig4_2),MD_DISPLAY_WRITE_D4_1(hum_comps.dig4_1),MD_DISPLAY_WRITE_D4_0(hum_comps.dig4_0))
   #define MD_HID_ALL_4_LINE_DATA (MD_DISPLAY_WRITE_D4_8(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D4_7(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D4_6(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D4_5(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D4_4(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D4_3(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D4_2(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D4_1(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D4_0(MD_HIDE_DISP)    )
       if(hum_comps.cursor_4>-1)
       {
            ((hum_comps.cursor_4==8)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_8(hum_comps.dig4_8):MD_DISPLAY_WRITE_D4_8(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_8(hum_comps.dig4_8);
            ((hum_comps.cursor_4==7)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_7(hum_comps.dig4_7):MD_DISPLAY_WRITE_D4_7(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_7(hum_comps.dig4_7);
            ((hum_comps.cursor_4==6)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_6(hum_comps.dig4_6):MD_DISPLAY_WRITE_D4_6(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_6(hum_comps.dig4_6);
            ((hum_comps.cursor_4==5)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_5(hum_comps.dig4_5):MD_DISPLAY_WRITE_D4_5(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_5(hum_comps.dig4_5);
            ((hum_comps.cursor_4==4)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_4(hum_comps.dig4_4):MD_DISPLAY_WRITE_D4_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_4(hum_comps.dig4_4);
            ((hum_comps.cursor_4==3)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_3(hum_comps.dig4_3):MD_DISPLAY_WRITE_D4_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_3(hum_comps.dig4_3);
            ((hum_comps.cursor_4==2)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_2(hum_comps.dig4_2):MD_DISPLAY_WRITE_D4_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_2(hum_comps.dig4_2);
            ((hum_comps.cursor_4==1)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_1(hum_comps.dig4_1):MD_DISPLAY_WRITE_D4_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_1(hum_comps.dig4_1);
            ((hum_comps.cursor_4==0)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DISPLAY_WRITE_D4_0(hum_comps.dig4_0):MD_DISPLAY_WRITE_D4_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D4_0(hum_comps.dig4_0);
        }
        else
        {
            ((hum_comps.cursor_4==-1)&&(hum_comps.dis_oper_mark._bit.cur4))?((hum_comps.dis_oper_mark._bit.dis4)?MD_DIS_ALL_4_LINE_DATA                  :MD_HID_ALL_4_LINE_DATA             ):MD_DIS_ALL_4_LINE_DATA;
        }
   #undef MD_DIS_ALL_4_LINE_DATA
   #undef MD_HID_ALL_4_LINE_DATA

}


static void display_line3_data()//display 3th line data
{
       #define MD_DIS_ALL_3_LINE_DATA (MD_DISPLAY_WRITE_D3_5(hum_comps.dig3_5),MD_DISPLAY_WRITE_D3_4(hum_comps.dig3_4),MD_DISPLAY_WRITE_D3_3(hum_comps.dig3_3),MD_DISPLAY_WRITE_D3_2(hum_comps.dig3_2),MD_DISPLAY_WRITE_D3_1(hum_comps.dig3_1),MD_DISPLAY_WRITE_D3_0(hum_comps.dig3_0))
       #define MD_HID_ALL_3_LINE_DATA (MD_DISPLAY_WRITE_D3_5(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D3_4(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D3_3(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D3_2(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D3_1(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D3_0(MD_HIDE_DISP)    )
       if(hum_comps.cursor_3>-1)
       {
        	((hum_comps.cursor_3==5)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_5(hum_comps.dig3_5):MD_DISPLAY_WRITE_D3_5(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_5(hum_comps.dig3_5);
          	((hum_comps.cursor_3==4)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_4(hum_comps.dig3_4):MD_DISPLAY_WRITE_D3_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_4(hum_comps.dig3_4);
        	((hum_comps.cursor_3==3)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_3(hum_comps.dig3_3):MD_DISPLAY_WRITE_D3_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_3(hum_comps.dig3_3);
        	((hum_comps.cursor_3==2)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_2(hum_comps.dig3_2):MD_DISPLAY_WRITE_D3_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_2(hum_comps.dig3_2);
        	((hum_comps.cursor_3==1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_1(hum_comps.dig3_1):MD_DISPLAY_WRITE_D3_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_1(hum_comps.dig3_1);
        	((hum_comps.cursor_3==0)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DISPLAY_WRITE_D3_0(hum_comps.dig3_0):MD_DISPLAY_WRITE_D3_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D3_0(hum_comps.dig3_0);
        }
    	else
    	{
            ((hum_comps.cursor_3==-1)&&(hum_comps.dis_oper_mark._bit.cur3))?((hum_comps.dis_oper_mark._bit.dis3)?MD_DIS_ALL_3_LINE_DATA                  :MD_HID_ALL_3_LINE_DATA             ):MD_DIS_ALL_3_LINE_DATA;
        }
       #undef MD_DIS_ALL_3_LINE_DATA
       #undef MD_HID_ALL_3_LINE_DATA

}
static void display_line2_data()//display 0th line data
{
   
       #define MD_DIS_ALL_2_LINE_DATA (MD_DISPLAY_WRITE_D2_5(hum_comps.dig2_5),MD_DISPLAY_WRITE_D2_4(hum_comps.dig2_4),MD_DISPLAY_WRITE_D2_3(hum_comps.dig2_3),MD_DISPLAY_WRITE_D2_2(hum_comps.dig2_2),MD_DISPLAY_WRITE_D2_1(hum_comps.dig2_1),MD_DISPLAY_WRITE_D2_0(hum_comps.dig2_0))
       #define MD_HID_ALL_2_LINE_DATA (MD_DISPLAY_WRITE_D2_5(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D2_4(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D2_3(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D2_2(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D2_1(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D2_0(MD_HIDE_DISP)    )
       if(hum_comps.cursor_2>-1)
       {
        	((hum_comps.cursor_2==5)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_5(hum_comps.dig2_5):MD_DISPLAY_WRITE_D2_5(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_5(hum_comps.dig2_5);
          	((hum_comps.cursor_2==4)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_4(hum_comps.dig2_4):MD_DISPLAY_WRITE_D2_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_4(hum_comps.dig2_4);
        	((hum_comps.cursor_2==3)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_3(hum_comps.dig2_3):MD_DISPLAY_WRITE_D2_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_3(hum_comps.dig2_3);
        	((hum_comps.cursor_2==2)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_2(hum_comps.dig2_2):MD_DISPLAY_WRITE_D2_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_2(hum_comps.dig2_2);
        	((hum_comps.cursor_2==1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_1(hum_comps.dig2_1):MD_DISPLAY_WRITE_D2_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_1(hum_comps.dig2_1);
        	((hum_comps.cursor_2==0)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DISPLAY_WRITE_D2_0(hum_comps.dig2_0):MD_DISPLAY_WRITE_D2_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D2_0(hum_comps.dig2_0);
        }
    	else
    	{
            ((hum_comps.cursor_2==-1)&&(hum_comps.dis_oper_mark._bit.cur2))?((hum_comps.dis_oper_mark._bit.dis2)?MD_DIS_ALL_2_LINE_DATA                  :MD_HID_ALL_2_LINE_DATA             ):MD_DIS_ALL_2_LINE_DATA;
        }
       #undef MD_DIS_ALL_2_LINE_DATA
       #undef MD_HID_ALL_2_LINE_DATA
}

static void display_line1_data()//display 1th line data
{
	 #define MD_DIS_ALL_1_LINE_DATA (MD_DISPLAY_WRITE_D1_4(hum_comps.dig1_4),MD_DISPLAY_WRITE_D1_3(hum_comps.dig1_3),MD_DISPLAY_WRITE_D1_2(hum_comps.dig1_2),MD_DISPLAY_WRITE_D1_1(hum_comps.dig1_1),MD_DISPLAY_WRITE_D1_0(hum_comps.dig1_0))
     #define MD_HID_ALL_1_LINE_DATA (MD_DISPLAY_WRITE_D1_4(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D1_3(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D1_2(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D1_1(MD_HIDE_DISP)    ,MD_DISPLAY_WRITE_D1_0(MD_HIDE_DISP)    )
     if(hum_comps.cursor_1>-1)
     {
        ((hum_comps.cursor_1==4)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DISPLAY_WRITE_D1_4(hum_comps.dig1_4):MD_DISPLAY_WRITE_D1_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D1_4(hum_comps.dig1_4);
    	((hum_comps.cursor_1==3)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DISPLAY_WRITE_D1_3(hum_comps.dig1_3):MD_DISPLAY_WRITE_D1_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D1_3(hum_comps.dig1_3);
    	((hum_comps.cursor_1==2)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DISPLAY_WRITE_D1_2(hum_comps.dig1_2):MD_DISPLAY_WRITE_D1_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D1_2(hum_comps.dig1_2);
    	((hum_comps.cursor_1==1)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DISPLAY_WRITE_D1_1(hum_comps.dig1_1):MD_DISPLAY_WRITE_D1_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D1_1(hum_comps.dig1_1);
    	((hum_comps.cursor_1==0)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DISPLAY_WRITE_D1_0(hum_comps.dig1_0):MD_DISPLAY_WRITE_D1_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D1_0(hum_comps.dig1_0);
     }
     else
     {
       ((hum_comps.cursor_1==-1)&&(hum_comps.dis_oper_mark._bit.cur1))?((hum_comps.dis_oper_mark._bit.dis1)?MD_DIS_ALL_1_LINE_DATA                  :MD_HID_ALL_1_LINE_DATA             ):MD_DIS_ALL_1_LINE_DATA;
     }
      #undef MD_DIS_ALL_1_LINE_DATA
      #undef MD_HID_ALL_1_LINE_DATA
}

static void display_line0_data()//display 1th line data
{
	 #define MD_DIS_ALL_0_LINE_DATA (MD_DISPLAY_WRITE_D0_5(hum_comps.dig0_5),MD_DISPLAY_WRITE_D0_4(hum_comps.dig0_4),MD_DISPLAY_WRITE_D0_3(hum_comps.dig0_3),MD_DISPLAY_WRITE_D0_2(hum_comps.dig0_2),MD_DISPLAY_WRITE_D0_1(hum_comps.dig0_1),MD_DISPLAY_WRITE_D0_0(hum_comps.dig0_0))
     #define MD_HID_ALL_0_LINE_DATA (MD_DISPLAY_WRITE_D0_5(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D0_4(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D0_3(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D0_2(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D0_1(MD_HIDE_DISP),    MD_DISPLAY_WRITE_D0_0(MD_HIDE_DISP) )
     if(hum_comps.cursor_0>-1)
     {
    	((hum_comps.cursor_0==5)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_5(hum_comps.dig0_5):MD_DISPLAY_WRITE_D0_5(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_5(hum_comps.dig0_5);
    	((hum_comps.cursor_0==4)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_4(hum_comps.dig0_4):MD_DISPLAY_WRITE_D0_4(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_4(hum_comps.dig0_4);
        ((hum_comps.cursor_0==3)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_3(hum_comps.dig0_3):MD_DISPLAY_WRITE_D0_3(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_3(hum_comps.dig0_3);
    	((hum_comps.cursor_0==2)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_2(hum_comps.dig0_2):MD_DISPLAY_WRITE_D0_2(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_2(hum_comps.dig0_2);
    	((hum_comps.cursor_0==1)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_1(hum_comps.dig0_1):MD_DISPLAY_WRITE_D0_1(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_1(hum_comps.dig0_1);
    	((hum_comps.cursor_0==0)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DISPLAY_WRITE_D0_0(hum_comps.dig0_0):MD_DISPLAY_WRITE_D0_0(MD_HIDE_DISP)):MD_DISPLAY_WRITE_D0_0(hum_comps.dig0_0);
     }
     else
     {
       ((hum_comps.cursor_0==-1)&&(hum_comps.dis_oper_mark._bit.cur0))?((hum_comps.dis_oper_mark._bit.dis0)?MD_DIS_ALL_0_LINE_DATA                  :MD_HID_ALL_0_LINE_DATA             ):MD_DIS_ALL_0_LINE_DATA;
     }
      #undef MD_DIS_ALL_0_LINE_DATA
      #undef MD_HID_ALL_0_LINE_DATA
}


//return low level key_valve
static int get_key(hum_comps_t *const this )
{
	static unsigned long key_delay_count=0;
        static int           current_key=MD_KEY_MASK;
	int                  key_valve;
	this->key_type=EM_NO_KEY;
	MD_KEY_VDD=1;
	delay_us(1);
	key_valve=(MD_KEY_DATA&MD_KEY_MASK);
	MD_KEY_VDD=0;
	if(key_valve!=current_key)
	{
		current_key=key_valve;
		key_delay_count=83;
		key_valve=MD_KEY_MASK;
	}
	else
	{
		if(key_delay_count>0)
		{
			key_delay_count--;
		}
		if(key_delay_count==81)
		{
			if((key_valve&MD_KEY_MASK)!=MD_KEY_MASK)
			{
				this->key_type=EM_SHORT_KEY;
				this->key=key_valve;
			}
		}
		else if(key_delay_count==23)
		{
			if((key_valve&MD_KEY_MASK)!=MD_KEY_MASK)
			{
				this->key_type=EM_LONG_KEY;
				this->key=key_valve;
			}
		}
		else
		{
			key_valve=MD_KEY_MASK;
		}
	}
	return key_valve;
}

static long isNum(int dig)
{
    if(dig<10)
    {
        return dig;
    }
    else 
    {
        return 0;
    }
}


/////////////////////////////////////mode enter and exit//////////////////
void enter_self_test_mode(void)
{
    hum_comps.current_mode=EM_SELF_TEST_MODE;
    clr_lcd();
    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;
}
void enter_normal_mode(void)
{
    hum_comps.current_mode=EM_NORMAL_MODE;
    mode_comps[hum_comps.current_mode].dis_option=0;
    clr_lcd();
    hum_comps.dis_oper_mark._bit.refresh_press;
	hum_comps.dis_oper_mark._bit.refresh_temp;
	hum_comps.dis_oper_mark._bit.refresh_deltaP;
    hum_comps.dis_oper_mark._bit.refresh_press_max=1;

    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;
    hum_comps.dis_oper_mark._bit.cur3=0;
    hum_comps.dis_oper_mark._bit.cur4=0;
}
void enter_lora_mode(void)
{
    hum_comps.current_mode=EM_LORA_MODE;
    mode_comps[hum_comps.current_mode].dis_option=0;
    clr_lcd();
    hum_comps.dis_oper_mark.All|=0x0000ff00;

    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;

}
void enter_cal_modify_mode()
{
    hum_comps.current_mode=EM_CAL_MODIFY_MODE;
    clr_lcd();
    mode_comps[hum_comps.current_mode].dis_option=mode_comps[EM_CAL_QUERY_MODE].dis_option;
    if(device_comps.cal_type==EM_CAL_PRESS)
    {
        hum_comps.cursor_2=5;
        hum_comps.cursor_2_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_press_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur2=1;
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        hum_comps.cursor_3=5;
        hum_comps.cursor_3_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur3=1;
    }
 else if(device_comps.cal_type==EM_CAL_RES)
    {
         hum_comps.cursor_1=4;
        hum_comps.cursor_1_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_temp_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_res_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur1=1;
    }
    else
    {
        NOP();
    }
  
}

void enter_cal_query_mode()
{
    hum_comps.current_mode=EM_CAL_QUERY_MODE;
    clr_lcd();
    mode_comps[hum_comps.current_mode].dis_option=0;
    if(device_comps.cal_type==EM_CAL_PRESS)
    {
        hum_comps.cursor_2=5;
        hum_comps.cursor_2_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_press_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur2=0;
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        hum_comps.cursor_3=5;
        hum_comps.cursor_3_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur3=0;
    }
   
    else if(device_comps.cal_type==EM_CAL_RES)
    {
         hum_comps.cursor_1=4;
        hum_comps.cursor_1_count=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_temp_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_res_cal_param=1;
        hum_comps.dis_oper_mark._bit.cur1=0;
    }
    else
    {
        NOP();
    }
}

void enter_param_query_mode()
{
    hum_comps.current_mode=EM_PARAM_QUERY_MODE;
    clr_lcd();
    mode_comps[hum_comps.current_mode].dis_option=0;

    hum_comps.cursor_2=5;
    hum_comps.cursor_2_count=2;
    hum_comps.dis_oper_mark._bit.refresh_option=1;
   // hum_comps.dis_oper_mark._bit.refresh_temp_adc=1;
   // hum_comps.dis_oper_mark._bit.refresh_cal_param=1;

    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;

}

void enter_param_modify_mode()
{
    int opt;
    hum_comps.current_mode=EM_PARAM_MODIFY_MODE;
    clr_lcd();
    opt= mode_comps[hum_comps.current_mode].dis_option=mode_comps[EM_PARAM_QUERY_MODE].dis_option;
    hum_comps.cursor_2_count=0;
    hum_comps.dis_oper_mark._bit.cur2=1;
    if(opt==0||opt==4||opt==5)
    {
        hum_comps.cursor_2=0;
    }
    else if(opt==1||opt==2)
    {
        hum_comps.cursor_2=4;
    }
    else if(opt==3)
    {
        hum_comps.cursor_2=2;
    }
    else if(opt==6||opt==7)
    {
        hum_comps.cursor_2=3;

    }
}

static void enter_debug_mode(void)
{
    hum_comps.current_mode=EM_DEBUG_MODE;
    clr_lcd();
    hum_comps.dis_oper_mark._bit.refresh_press_adc;
	hum_comps.dis_oper_mark._bit.refresh_temp_adc;
	hum_comps.dis_oper_mark._bit.refresh_deltaP_adc;

    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;
    hum_comps.dis_oper_mark._bit.cur3=0;
    hum_comps.dis_oper_mark._bit.cur4=0;
    mode_comps[hum_comps.current_mode].displayTimer=-1;
}

void  enter_report_mode(void)
{
    hum_comps.current_mode=EM_REPORT_MODE;
    clr_lcd();
   
    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;
    mode_comps[hum_comps.current_mode].displayTimer=0;
    
}

static void enter_pawd_mode(void)
{
    hum_comps.current_mode=EM_PWD_MODE;
    clr_lcd();
    hum_comps.dis_oper_mark._bit.cur0=1;
    hum_comps.dis_oper_mark._bit.dis0=0;
    hum_comps.dig0_0=0;
    hum_comps.dig0_1=0;
    hum_comps.dig0_2=0;
    hum_comps.dig0_3=0;
    hum_comps.dig0_4=0;
    hum_comps.dig0_5=0;
    hum_comps.cursor_0=5;
    hum_comps.dot0_pos=0;
    hum_comps.cursor_0_count=0;
      
}
void enter_air_leak_mode(void)
{

    hum_comps.current_mode=EM_AIR_LEAK_MODE;
    mode_comps[hum_comps.current_mode].dis_option=0;
    clr_lcd();
    hum_comps.dis_oper_mark._bit.air_leak_mode_refresh_timer=1;
    hum_comps.dis_oper_mark._bit.refresh_press=1;
    hum_comps.dis_oper_mark._bit.cur0=0;
    hum_comps.dis_oper_mark._bit.cur1=0;
    hum_comps.dis_oper_mark._bit.cur2=0;


}
/*********************************start  key function************************************************************/

static void on_s_key()
{
	mode_comps[hum_comps.current_mode].on_s_key();
}
static void on_m_key()
{
	mode_comps[hum_comps.current_mode].on_m_key();
}
static void on_j_key()
{
	mode_comps[hum_comps.current_mode].on_j_key();
}
static void on_long_s_key()
{
	mode_comps[hum_comps.current_mode].on_long_s_key();
}
static void on_long_m_key()
{
	mode_comps[hum_comps.current_mode].on_long_m_key();
}
static void on_long_j_key()
{
	mode_comps[hum_comps.current_mode].on_long_j_key();
}
static void on_long_s_and_j_key()
{
	mode_comps[hum_comps.current_mode].on_long_s_and_j_key();
}


static void normal_mode_on_s_key()
{
     device_comps.max_press=0;
	 hum_comps.dis_oper_mark._bit.refresh_press_max=1;
}
static void normal_mode_on_m_key()
{
	mode_comps[hum_comps.current_mode].dis_option++;
	//mode_comps[hum_comps.current_mode].dis_option%=8;
	hum_comps.dis_oper_mark.All|=0x0000ff00;
	hum_comps.dis_oper_mark._bit.refresh_press_max=1;
	//if(mode_comps[hum_comps.current_mode].dis_option>2)
	//{
	 //   hum_comps.dis_oper_mark._bit.cur2=0;
	//}
	//mode_comps[hum_comps.current_mode].displayTimer=-1;
}

static void normal_mode_on_j_key()
{
 
}

static void normal_mode_on_long_s_key()
{
   //protocolComps.triggerIrq._bit.key=1;
}

static void normal_mode_on_long_m_key()
{
   // device_comps.clr_press();
   enter_pawd_mode();
}
static void normal_mode_on_long_s_and_j_key()
{
	enter_debug_mode();
}

static void normal_mode_on_long_j_key (void)
{
	if(!ircComps.sw._bit.runing)
   {
        MD_IR_VCM_ON;//o¨¬¨ªa1?¦Ì?//	 
        ircComps.op_window_time=30;
        *ircComps.recv_pos_pt = 0;
        #ifndef MD_MODBUS
        enable_irc_receive();
        #endif
        ircComps.sw._bit.runing=1;
    }
    else
   {
        ircComps.op_window_time=0;
        *ircComps.recv_pos_pt = 0;
       #ifndef MD_MODBUS
        disable_irc_receive();
        disable_irc_send();
       #endif
        MD_IR_VCM_OFF;//o¨¬¨ªa1?¦Ì?//
	
	
        ircComps.sw._bit.runing=0;
    }
    
	
}







static void debug_mode_on_s_key()
{
	
}
static void debug_mode_on_m_key()
{
	
}
static void debug_mode_on_j_key()
{
	
}
static void debug_mode_on_long_m_key()
{


}
static void debug_mode_on_long_s_and_j_key()
{
    #if(MD_PRODUCT_NAME==MD_LORA)
    {
      enter_lora_mode();
    }
    #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
    {
      enter_air_leak_mode();
    }
    #elif (MD_PRODUCT_NAME==MD_NORMAL)
    {
        enter_normal_mode();
    }
    #endif
}

static void pwd_mode_on_s_key()
{
	hum_comps.cursor_0+=5;
	hum_comps.cursor_0%=6;//4->3->2...>0
}
static void pwd_mode_on_m_key()
{
    long pwd=isNum(hum_comps.dig0_2)*100+isNum(hum_comps.dig0_1)*10+isNum(hum_comps.dig0_0);
    if(pwd==235)
    {
        if(hum_comps.dig0_5>2) //dig0_5 cal_type 0:press 1:pt100  2:deltaP 
    	{
            return;
        }
        if(hum_comps.dig0_5!=1)
        {
           if(hum_comps.dig0_4<3||hum_comps.dig0_4>5||hum_comps.dig0_3>1)//dig0_4 dot 3-5 dig0_3 unit 0Mpa,1Kpa
           {
              return;
           }
        }
        device_comps.cal_type=(cal_type_t)hum_comps.dig0_5;//0,press,1 pt100 2,deltaP 
        if(device_comps.cal_type==EM_CAL_PRESS)
        {
            device_comps.calibration_param_bak.dot=hum_comps.dig0_4;
            device_comps.calibration_param_bak.unit=hum_comps.dig0_3;
        }
        else if(device_comps.cal_type==EM_CAL_DELTAP)
        {
            device_comps.deltaP_calibration_param_bak.dot=hum_comps.dig0_4;
            device_comps.deltaP_calibration_param_bak.unit=hum_comps.dig0_3;
        }
        else if(device_comps.cal_type==EM_CAL_RES)
        {
            device_comps.res_calibration_param_bak.dot=hum_comps.dig0_4;
            device_comps.res_calibration_param_bak.unit=hum_comps.dig0_3;
        }
        enter_cal_query_mode();
    }
    else if(pwd==1237)
    {
        enter_param_query_mode();
    }
}
static void pwd_mode_on_j_key()
{
	*(&hum_comps.dig0_0+hum_comps.cursor_0)=(*(&hum_comps.dig0_0+hum_comps.cursor_0)+1)%10;
}
static void pwd_mode_on_long_m_key()
{
  #if(MD_PRODUCT_NAME==MD_LORA)
  {
	  enter_lora_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
  {
	  enter_air_leak_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_NORMAL)
  {
        enter_normal_mode();
  }
  #endif
//	hum_comps.current_mode=EM_NORMAL_MODE;
//	hum_comps.dis_oper_mark|=MD_OPER_refresh_press_MARK;
//	hum_comps.dis_oper_mark|=MD_OPER_REFRESH_TEMP_MARK;
//	hum_comps.dis_oper_mark&=~MD_OPER_CUR_MARK;
}
static void pwd_mode_on_long_s_and_j_key()
{
	
}


static void cal_query_mode_on_s_key(void)
{
  
    enter_cal_modify_mode();
	
}
static void cal_query_mode_on_m_key(void)
{
	mode_comps[hum_comps.current_mode].dis_option++;
    if(device_comps.cal_type==EM_CAL_PRESS)
    {
        mode_comps[hum_comps.current_mode].dis_option%=4;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_press_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_cal_param=1;
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        mode_comps[hum_comps.current_mode].dis_option%=4;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=1;
    }
    else if(device_comps.cal_type==EM_CAL_RES)
    {
        mode_comps[hum_comps.current_mode].dis_option%=2;
        hum_comps.dis_oper_mark._bit.refresh_option=1;
        hum_comps.dis_oper_mark._bit.refresh_temp_adc=1;
        hum_comps.dis_oper_mark._bit.refresh_res_cal_param=1;
    }
}
static void cal_query_mode_on_j_key(void)
{
   
}

static void cal_query_mode_on_long_m_key(void)
{
  #if(MD_PRODUCT_NAME==MD_LORA)
  {
	  enter_lora_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
  {
	  enter_air_leak_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_NORMAL)
  {
        enter_normal_mode();
  }
  #endif
	
}

static void cal_modify_mode_on_s_key(void)
{
	if(device_comps.cal_type==EM_CAL_PRESS)
    {
       hum_comps.cursor_2+=5;
	   hum_comps.cursor_2%=6;//5->4->3->2...>0
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        hum_comps.cursor_3+=5;
	   hum_comps.cursor_3%=6;//5->4->3->2...>0
    }
    else if(device_comps.cal_type==EM_CAL_RES)
    {
        hum_comps.cursor_1+=4;
	    hum_comps.cursor_1%=5;//4->3->2...>0
    }

}
static void cal_modify_mode_on_m_key(void)
{
     int save_data_en=0;
	 int opt=mode_comps[hum_comps.current_mode].dis_option;
	       
    if(device_comps.cal_type==EM_CAL_PRESS)
	{
		if(device_comps.sw._bit.adc_stb)
		{
            long num=isNum(hum_comps.dig2_5)*100000+isNum(hum_comps.dig2_4)*10000+isNum(hum_comps.dig2_3)*1000+
					 isNum(hum_comps.dig2_2)*100+isNum(hum_comps.dig2_1)*10+isNum(hum_comps.dig2_0);
			save_data_en=1;
			device_comps.calibration_param.x[mode_comps[hum_comps.current_mode].dis_option]=device_comps.ad1_ad2_average_result;
			device_comps.calibration_param.y[mode_comps[hum_comps.current_mode].dis_option]=num;
			device_comps.calibration_param.t[mode_comps[hum_comps.current_mode].dis_option];
			device_comps.calibration_param.dot=device_comps.calibration_param_bak.dot;
			device_comps.calibration_param.unit=device_comps.calibration_param_bak.unit;
			device_comps.calibration_param.cs=Check_Sum_5A(&device_comps.calibration_param, & device_comps.calibration_param.cs-(unsigned char *)&device_comps.calibration_param);
			device_comps.save_calibration_param(&device_comps.calibration_param,sizeof(device_comps.calibration_param));
			hum_comps.dis_oper_mark._bit.refresh_cal_param=1;
		    hum_comps.dis_oper_mark._bit.cur2=0;
		}
	}
    else if(device_comps.cal_type==EM_CAL_DELTAP)
	{
		if(device_comps.sw._bit.deltaP_adc_stb)
		{
		    long num=isNum(hum_comps.dig3_5)*100000+isNum(hum_comps.dig3_4)*10000+isNum(hum_comps.dig3_3)*1000+
					 isNum(hum_comps.dig3_2)*100+isNum(hum_comps.dig3_1)*10+isNum(hum_comps.dig3_0);   
			save_data_en=1;
			device_comps.deltaP_calibration_param.x[mode_comps[hum_comps.current_mode].dis_option]=device_comps.deltaP_p_deltaP_n_average_result;
			device_comps.deltaP_calibration_param.y[mode_comps[hum_comps.current_mode].dis_option]=num;
            device_comps.deltaP_calibration_param.t[mode_comps[hum_comps.current_mode].dis_option];
            device_comps.deltaP_calibration_param.dot=device_comps.deltaP_calibration_param_bak.dot;
			device_comps.deltaP_calibration_param.unit=device_comps.deltaP_calibration_param_bak.unit;
			device_comps.deltaP_calibration_param.cs=Check_Sum_5A(&device_comps.deltaP_calibration_param, & device_comps.deltaP_calibration_param.cs-(unsigned char *)&device_comps.deltaP_calibration_param);
			device_comps.save_deltaP_calibration_param(&device_comps.deltaP_calibration_param,sizeof(device_comps.deltaP_calibration_param));
			hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=1;
		    hum_comps.dis_oper_mark._bit.cur3=0;
		}
	}
	else if(device_comps.cal_type==EM_CAL_RES)
	{
		if(device_comps.sw._bit.temp_adc_stb)
		{
            long num=isNum(hum_comps.dig1_4)*10000+isNum(hum_comps.dig1_3)*1000+
					 isNum(hum_comps.dig1_2)*100+isNum(hum_comps.dig1_1)*10+isNum(hum_comps.dig1_0);
			save_data_en=1;
			device_comps.res_calibration_param.x[mode_comps[hum_comps.current_mode].dis_option]=device_comps.temp_p_temp_n_average_result;
			device_comps.res_calibration_param.y[mode_comps[hum_comps.current_mode].dis_option]=num;
			device_comps.res_calibration_param.t[mode_comps[hum_comps.current_mode].dis_option];
			device_comps.res_calibration_param.dot=device_comps.res_calibration_param_bak.dot;
			device_comps.res_calibration_param.unit=device_comps.res_calibration_param_bak.unit;
			device_comps.res_calibration_param.cs=Check_Sum_5A(&device_comps.res_calibration_param, & device_comps.res_calibration_param.cs-(unsigned char *)&device_comps.res_calibration_param);
			device_comps.save_res_calibration_param(&device_comps.res_calibration_param,sizeof(device_comps.res_calibration_param));
			hum_comps.dis_oper_mark._bit.refresh_res_cal_param=1;
		    hum_comps.dis_oper_mark._bit.cur1=0;
		}
	}
	if(save_data_en==1)
	{
		hum_comps.current_mode=EM_CAL_QUERY_MODE;
		mode_comps[hum_comps.current_mode].dis_option=mode_comps[EM_CAL_MODIFY_MODE].dis_option;
    }
}
static void cal_modify_mode_on_j_key(void)
{

	if(device_comps.cal_type==EM_CAL_PRESS)
    {
      *(&hum_comps.dig2_0+hum_comps.cursor_2)=(*(&hum_comps.dig2_0+hum_comps.cursor_2)+1)%10;
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        *(&hum_comps.dig3_0+hum_comps.cursor_3)=(*(&hum_comps.dig3_0+hum_comps.cursor_3)+1)%10;
    }
    
    else if(device_comps.cal_type==EM_CAL_RES)
    {
        *(&hum_comps.dig1_0+hum_comps.cursor_1)=(*(&hum_comps.dig2_0+hum_comps.cursor_1)+1)%10;
    }
	
}
static void cal_modify_mode_on_long_s_key(void)
{
  #if(MD_PRODUCT_NAME==MD_LORA)
  {
	  enter_lora_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
  {
	  enter_air_leak_mode();
  }
  #elif (MD_PRODUCT_NAME==MD_NORMAL)
  {
        enter_normal_mode();
  }
  #endif
}
static void param_query_mode_on_s_key(void)
{
	enter_param_modify_mode();
}
static void param_query_mode_on_m_key(void)
{
    mode_comps[hum_comps.current_mode].dis_option++;
	mode_comps[hum_comps.current_mode].dis_option%=8;
	hum_comps.dis_oper_mark._bit.refresh_option=1;
}

static void param_query_mode_on_long_m_key(void)
{
	
    #if(MD_PRODUCT_NAME==MD_LORA)
    {
      enter_lora_mode();
    }
    #elif (MD_PRODUCT_NAME==MD_AIR_LEAK)
    {
      enter_air_leak_mode();
    }
    #elif (MD_PRODUCT_NAME==MD_NORMAL)
    {
        enter_normal_mode();
    }
    #endif
}


static void param_modify_mode_on_m_key(void)
{
	 int save_data_en=0;
	 int opt=mode_comps[hum_comps.current_mode].dis_option;
	 long num=isNum(hum_comps.dig2_5)*100000+isNum(hum_comps.dig2_4)*10000+isNum(hum_comps.dig2_3)*1000+
								isNum(hum_comps.dig2_2)*100+
									       isNum(hum_comps.dig2_1)*10+           
								               isNum(hum_comps.dig2_0);        
    save_data_en=1;
    *(&device_comps.air_leak_param.unit+opt)=num;
    device_comps.air_leak_param.cs=Check_Sum_5A(&device_comps.air_leak_param, & device_comps.air_leak_param.cs-(unsigned char *)&device_comps.air_leak_param);
	device_comps.save_air_leak_param(&device_comps.air_leak_param,sizeof(device_comps.air_leak_param));
    if(save_data_en==1)
	{
		hum_comps.current_mode=EM_PARAM_QUERY_MODE;
		opt=mode_comps[hum_comps.current_mode].dis_option=mode_comps[EM_PARAM_MODIFY_MODE].dis_option;
        hum_comps.dis_oper_mark._bit.cur2=0;
	}
	
}
static void param_modify_mode_on_s_key(void)
{
	int opt=mode_comps[hum_comps.current_mode].dis_option;
       if(opt==0||opt==4||opt==5)
        {
            hum_comps.cursor_2=0;
        }
        else if(opt==1||opt==2)
        {
            hum_comps.cursor_2+=4;
	        hum_comps.cursor_2%=5;//4->3->2...>0
        }
        else if(opt==3)
        {
            hum_comps.cursor_2+=2;
	        hum_comps.cursor_2%=3;//4->3->2...>0
        }
        else if(opt==6||opt==7)
        {
            hum_comps.cursor_2+=3;
	        hum_comps.cursor_2%=4;//4->3->2...>0
    
        }
}
static void param_modify_mode_on_j_key(void)
{
	int opt=mode_comps[hum_comps.current_mode].dis_option;
	  if(opt==0)
        {
            *(&hum_comps.dig2_0+hum_comps.cursor_2)=(*(&hum_comps.dig2_0+hum_comps.cursor_2)+1)%4;
        }
        else if(opt==4)
        {
            *(&hum_comps.dig2_0+hum_comps.cursor_2)=(*(&hum_comps.dig2_0+hum_comps.cursor_2)+1)%3;
        }
       else if(opt==5)
       {
         *(&hum_comps.dig2_0+hum_comps.cursor_2)=(*(&hum_comps.dig2_0+hum_comps.cursor_2)+1)%2;
       }
        else if(opt==1||opt==2||opt==3||opt==6||opt==7)
        {
          *(&hum_comps.dig2_0+hum_comps.cursor_2)=(*(&hum_comps.dig2_0+hum_comps.cursor_2)+1)%10;
        }
}
static void param_modify_mode_on_long_s_key(void)
{
	
	
}
static void air_leak_mode_on_j_key(void)
{
    if(device_comps.calibration_param.is_calibrated)
    {
        if(device_comps.air_leak.sw._bit.test_finished)
        {
              device_comps.air_leak.sw._bit.isWarn=0;
              device_comps.air_leak.sw._bit.test_finished=0;
        }
        else
        {
            if(device_comps.air_leak_param.count_direction==0)//up count
            {
                device_comps.air_leak.timer=0;
            }
            else if(device_comps.air_leak_param.count_direction==1)//down count
            {
                device_comps.air_leak.timer=device_comps.air_leak_param.timer;
            }
            
            if(device_comps.air_leak_param.delay_timer<3)
            {
                device_comps.air_leak_param.delay_timer=3;
            }
            device_comps.air_leak.delay_timer=device_comps.air_leak_param.delay_timer;
            device_comps.air_leak.sw._bit.count_en=1;
        }
    }
 
}
static void air_leak_mode_on_long_m_key(void)
{
    if(device_comps.calibration_param.is_calibrated)
    {
        enter_pawd_mode();
        //device_comps.air_leak.sw._bit.count_en=0;
        //device_comps.air_leak.sw._bit.test_finished=0;
    }
	
}

static void lora_mode_on_m_key(void)
{
    mode_comps[hum_comps.current_mode].dis_option++;
    mode_comps[hum_comps.current_mode].dis_option%=8;
    hum_comps.dis_oper_mark.All|=0x0000ff00;
    if(mode_comps[hum_comps.current_mode].dis_option>2)
    {
        hum_comps.dis_oper_mark._bit.cur2=0;
    }
    mode_comps[hum_comps.current_mode].displayTimer=-1;
}


///////////////////////END KEY FUNCTION////////////////////


//////////////START DISPLAY///////////////////////////////

static void hide_zero(unsigned char  *const hi_addr,unsigned char total_width,unsigned char reserved_num)//Number of digits reserved
{
	int i=0;
	unsigned char  *buf=hi_addr;
	for(i=0;i<total_width-reserved_num;i++)
	{
		if(*(buf-i)>0 && *(buf-i)<10)
		{
			break;
		}
		else
		{
			*(buf-i)=MD_HIDE_DISP;
				
		}
	}
}


void  display_opt(int opt)
{
    //hum_comps.dig0_0=MD_HIDE_DISP;
    hum_comps.dig0_5=opt%10;
    hum_comps.dot0_pos=0;
    MD_DISPLAY_WRITE_D0_5(hum_comps.dig0_5);
   // display_line0_data();
}


void display_temp_adc(void)
{
    if(hum_comps.dis_oper_mark._bit.refresh_temp_adc)
	{
        long num=device_comps.temp_p_temp_n_average_result; 
        if(num<0)
        {
            hum_comps.dot1_pos=4;// -adc dis 4dot
            num=-num;
        }
        else
        {
            hum_comps.dot1_pos=0;
        }
        hum_comps.dig1_0=num%10;
        hum_comps.dig1_1=num/10%10;
        hum_comps.dig1_2=num/100%10;
        hum_comps.dig1_3=num/1000%10;
        hum_comps.dig1_4=num/10000%10;//
        hum_comps.dot1_pos=0;//
        MD_DIS_S8_TEMP;
        hum_comps.dis_oper_mark._bit.refresh_temp_adc=0;
    }              

    if(device_comps.sw._bit.temp_adc_stb)
    {
        hum_comps.dis_oper_mark._bit.cur1=0;
        hum_comps.dis_oper_mark._bit.dis1=1;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur1=1;
        hum_comps.cursor_1=-1;
        hum_comps.cursor_1_count++;
        if(hum_comps.cursor_1_count>4)
        {
            hum_comps.cursor_1_count=0;
            if(hum_comps.dis_oper_mark._bit.dis1)
            {
                hum_comps.dis_oper_mark._bit.dis1=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis1=1;
            }
        }
    }
    display_line1_data();
}

void display_cal_adc_on_line4(unsigned char opt)
{
   long num=0;
   cal_type_t cal_type=device_comps.cal_type;
   
   if(cal_type==EM_CAL_PRESS)
   {
        num=device_comps.calibration_param.y[opt];
   }
   else if(cal_type==EM_CAL_DELTAP)
   {
       num=device_comps.deltaP_calibration_param.y[opt];
   }
   else if(cal_type==EM_CAL_RES)
   {
        num=device_comps.res_calibration_param.y[opt];
   }
   if(num<0)
    {
       hum_comps.dig4_5=MD_HIDE_DISP-1;// 
       num=-num;
    }
    else
    {
        hum_comps.dig4_5=0;
    }
    hum_comps.dig4_0=num%10;
    hum_comps.dig4_1=num/10%10;
    hum_comps.dig4_2=num/100%10;
    hum_comps.dig4_3=num/1000%10;
    hum_comps.dig4_4=num/10000%10;//
    hum_comps.dig4_5=MD_HIDE_DISP;
    hum_comps.dig4_6=MD_HIDE_DISP;
    hum_comps.dig4_7=0x0d;
    hum_comps.dig4_8=0x0a;
    hum_comps.dot4_pos=0;//
    hum_comps.dis_oper_mark._bit.cur4=0;
    display_line4_data();

}

void display_current_adc_on_line4(unsigned char opt)
{
   int stb=1;
   int refresh=0;
   long num=0;
   cal_type_t cal_type=device_comps.cal_type;
   if(cal_type==EM_CAL_PRESS)
   {
        stb=device_comps.sw._bit.adc_stb;
        if(hum_comps.dis_oper_mark._bit.refresh_press_adc)
        {
            refresh=1;
            num=device_comps.ad1_ad2_average_result;
            hum_comps.dis_oper_mark._bit.refresh_press_adc=0;
        }
   }
   else if(cal_type==EM_CAL_RES)
   {
        stb=device_comps.sw._bit.temp_adc_stb;
         if(hum_comps.dis_oper_mark._bit.refresh_temp_adc)
        {
            refresh=1;
            num=device_comps.temp_p_temp_n_average_result;
            hum_comps.dis_oper_mark._bit.refresh_temp_adc=0;
        }
   }
   else if(cal_type==EM_CAL_DELTAP)
   {
        stb=device_comps.sw._bit.deltaP_adc_stb;
        if(hum_comps.dis_oper_mark._bit.refresh_deltaP_adc)
        {
            refresh=1;
            num=device_comps.deltaP_p_deltaP_n_average_result;
            hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=0;
        }
   }

   
   if(refresh)
   {
        if(num<0)
        {
           hum_comps.dig4_5=MD_HIDE_DISP-1;// 
            num=-num;
        }
        else
        {
            hum_comps.dig4_5=0;
        }
        hum_comps.dig4_0=num%10;
        hum_comps.dig4_1=num/10%10;
        hum_comps.dig4_2=num/100%10;
        hum_comps.dig4_3=num/1000%10;
        hum_comps.dig4_4=num/10000%10;//
        hum_comps.dig4_5=MD_HIDE_DISP;
        hum_comps.dig4_6=MD_HIDE_DISP;
        hum_comps.dig4_7=0x0d;
        hum_comps.dig4_8=0x0a;
        hum_comps.dig4_9=0x0a;//
        hum_comps.dot4_pos=0;//
        refresh=0;
    }              

    if(stb)
    {
        hum_comps.dis_oper_mark._bit.cur4=0;
        hum_comps.dis_oper_mark._bit.dis4=1;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur4=1;
        hum_comps.cursor_4=-1;
        hum_comps.cursor_4_count++;
        if(hum_comps.cursor_4_count>4)
        {
            hum_comps.cursor_4_count=0;
            if(hum_comps.dis_oper_mark._bit.dis4)
            {
                hum_comps.dis_oper_mark._bit.dis4=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis4=1;
            }
        }
    }
    display_line4_data();

}

void display_press_adc(void)
{
 
    if(hum_comps.dis_oper_mark._bit.refresh_press_adc)
    {
        long num=device_comps.ad1_ad2_average_result; 
        if(num<0)
        {
           hum_comps.dig2_5=MD_HIDE_DISP-1;// 
            num=-num;
        }
        else
        {
            hum_comps.dig2_5=0;
        }
        hum_comps.dig2_0=num%10;
        hum_comps.dig2_1=num/10%10;
        hum_comps.dig2_2=num/100%10;
        hum_comps.dig2_3=num/1000%10;
        hum_comps.dig2_4=num/10000%10;//
        hum_comps.dot2_pos=0;//
        MD_DIS_T12_PRESS;
        hum_comps.dis_oper_mark._bit.refresh_press_adc=0;
    }              

    if(device_comps.sw._bit.adc_stb)
    {
        hum_comps.dis_oper_mark._bit.cur2=0;
        hum_comps.dis_oper_mark._bit.dis2=1;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur2=1;
        hum_comps.cursor_2=-1;
        hum_comps.cursor_2_count++;
        if(hum_comps.cursor_2_count>4)
        {
            hum_comps.cursor_2_count=0;
            if(hum_comps.dis_oper_mark._bit.dis2)
            {
                hum_comps.dis_oper_mark._bit.dis2=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis2=1;
            }
        }
    }
    display_line2_data();

}

void display_deltaP_adc(void)
{
 
    if(hum_comps.dis_oper_mark._bit.refresh_deltaP_adc)
    {
        long num=device_comps.deltaP_p_deltaP_n_average_result; 
        if(num<0)
        {
           hum_comps.dig3_5=MD_HIDE_DISP-1;// 
            num=-num;
        }
        else
        {
            hum_comps.dig3_5=0;
        }
        hum_comps.dig3_0=num%10;
        hum_comps.dig3_1=num/10%10;
        hum_comps.dig3_2=num/100%10;
        hum_comps.dig3_3=num/1000%10;
        hum_comps.dig3_4=num/10000%10;//
        hum_comps.dot3_pos=0;//
        MD_DIS_T9_DELTAP;
        hum_comps.dis_oper_mark._bit.refresh_deltaP_adc=0;
    }              

    if(device_comps.sw._bit.deltaP_adc_stb)
    {
        hum_comps.dis_oper_mark._bit.cur3=0;
        hum_comps.dis_oper_mark._bit.dis3=1;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur3=1;
        hum_comps.cursor_3=-1;
        hum_comps.cursor_3_count++;
        if(hum_comps.cursor_3_count>4)
        {
            hum_comps.cursor_3_count=0;
            if(hum_comps.dis_oper_mark._bit.dis3)
            {
                hum_comps.dis_oper_mark._bit.dis3=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis3=1;
            }
        }
    }
    display_line3_data();

}

void display_temp()//refresh temperature
{
    if(!device_comps.res_calibration_param.is_calibrated)
    {
                hum_comps.dig1_0=MD_HIDE_DISP-1;
                hum_comps.dig1_1=MD_HIDE_DISP-1;
                hum_comps.dig1_2=MD_HIDE_DISP-1;
                hum_comps.dig1_3=MD_HIDE_DISP-1;
                hum_comps.dig1_4=MD_HIDE_DISP-1;
                hum_comps.dot1_pos=0;
                display_line1_data();//display 1 line data
                return;
    }
    if(hum_comps.dis_oper_mark._bit.refresh_temp)
    {
        long num=device_comps.current_temp;
        if(num<0)
    	{
    	    num=-num;
    	    hum_comps.dig1_4=MD_HIDE_DISP-1;
    	}
        hum_comps.dig1_0=num%10;
		hum_comps.dig1_1=num/10%10;
		hum_comps.dig1_2=num/100%10;
		hum_comps.dig1_3=num/1000%10;
		hum_comps.dot1_pos=1;
        MD_DIS_T18_CELSIUS;
        MD_DIS_S8_TEMP;
        hide_zero(&hum_comps.dig1_3,4,hum_comps.dot1_pos+1);
		hum_comps.dis_oper_mark._bit.refresh_temp=0;
		
    }
    display_line1_data();//display 1 line data
}
		
		


void display_press(int index)
{
    if(!device_comps.calibration_param.is_calibrated)
    {
                hum_comps.dig2_0=MD_HIDE_DISP-1;
                hum_comps.dig2_1=MD_HIDE_DISP-1;
                hum_comps.dig2_2=MD_HIDE_DISP-1;
                hum_comps.dig2_3=MD_HIDE_DISP-1;
                hum_comps.dig2_4=MD_HIDE_DISP-1;
                hum_comps.dig2_5=MD_HIDE_DISP-1;
                hum_comps.dot2_pos=0;
                display_line2_data();//display 1 line data
                return;
    }
    if(hum_comps.dis_oper_mark._bit.refresh_press)
    {
        long num=device_comps.current_press;
        if(num<0)
    	{
    	    num=-num;
    	    hum_comps.dig2_5=MD_HIDE_DISP-1;
    	}
        hum_comps.dig2_0=num%10;
		hum_comps.dig2_1=num/10%10;
		hum_comps.dig2_2=num/100%10;
		hum_comps.dig2_3=num/1000%10;
		hum_comps.dig2_4=num/10000%10;
        hum_comps.dot2_pos=device_comps.calibration_param.dot-1;///////////////////////
        if(device_comps.calibration_param.unit==0)
        {
            MD_DIS_T11_MPA;
        }
        else if(device_comps.calibration_param.unit==1)
        {
            MD_DIS_T10_KPA;
        }
        MD_DIS_T12_PRESS;
        hide_zero(&hum_comps.dig2_4,5,hum_comps.dot2_pos+1);
		hum_comps.dis_oper_mark._bit.refresh_press=0;
		
    }
    if(!device_comps.sw._bit.over_range)
    {
        hum_comps.dis_oper_mark._bit.cur2=0;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur2=1;
        hum_comps.cursor_2=-1;
        hum_comps.cursor_2_count++;
        if(hum_comps.cursor_2_count>9)
        {
            hum_comps.cursor_2_count=0;
            if(hum_comps.dis_oper_mark._bit.dis2)
            {
                hum_comps.dis_oper_mark._bit.dis2=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis2=1;
            }
        }
    }
    display_line2_data();    // 
}

void display_deltaP(int index)
{
    if(!device_comps.deltaP_calibration_param.is_calibrated)
    {
                hum_comps.dig3_0=MD_HIDE_DISP-1;
                hum_comps.dig3_1=MD_HIDE_DISP-1;
                hum_comps.dig3_2=MD_HIDE_DISP-1;
                hum_comps.dig3_3=MD_HIDE_DISP-1;
                hum_comps.dig3_4=MD_HIDE_DISP-1;
                hum_comps.dig3_5=MD_HIDE_DISP-1;
                hum_comps.dot3_pos=0;
                display_line3_data();//display 1 line data
                return;
    }
    if(hum_comps.dis_oper_mark._bit.refresh_deltaP)
    {
        long num=device_comps.current_deltaP/10;
        if(num<0)
    	{
    	    num=-num;
    	    hum_comps.dig3_5=MD_HIDE_DISP-1;
    	}
        hum_comps.dig3_0=num%10;
		hum_comps.dig3_1=num/10%10;
		hum_comps.dig3_2=num/100%10;
		hum_comps.dig3_3=num/1000%10;
		hum_comps.dig3_4=num/10000%10;
		hum_comps.dot3_pos=device_comps.deltaP_calibration_param.dot-1;///////////////////////
         if(device_comps.deltaP_calibration_param.unit==0)
        {
            MD_DIS_T7_MPA;
        }
        else if(device_comps.deltaP_calibration_param.unit==1)
        {
            MD_DIS_T8_KPA;
        }
        MD_DIS_T9_DELTAP;
        hide_zero(&hum_comps.dig3_4,5,hum_comps.dot3_pos+1);
		hum_comps.dis_oper_mark._bit.refresh_deltaP=0;
		
    }
    if(!device_comps.sw._bit.deltaP_over_range)
    {
        hum_comps.dis_oper_mark._bit.cur3=0;
    }
    else
    {
        hum_comps.dis_oper_mark._bit.cur3=1;
        hum_comps.cursor_3=-1;
        hum_comps.cursor_3_count++;
        if(hum_comps.cursor_3_count>9)
        {
            hum_comps.cursor_3_count=0;
            if(hum_comps.dis_oper_mark._bit.dis3)
            {
                hum_comps.dis_oper_mark._bit.dis3=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis3=1;
            }
        }
    }
    display_line3_data();    // 
}


static char lg(const long n)
{   
    if(n==1)
    {
        return 0;
    }
    if(n==10)
    {
        return 1;
    }
    if(n==100)
    {
        return 2;
    }
    if(n==1000)
    {
        return 3;
    }
    return 1;
}
static long pwr(int n)
{
    if(n==0)
    {
        return 1;
    }
    if(n==1)
    {
        return 10;
    }
    if(n==2)
    {
        return 100;
    }
    if(n==3)
    {
        return 1000;
    }
    return 1;
}

static long normal_mode_Mpa_switch(long num,int n,int *dis_dot,int opt,int *dis_opt)
{
//    long tmp;
//    const int  dot=device_comps.calibration_param.dot-lg(n);    //main  variable dot
//    num=num/n;  //main  variable num
//    switch((opt>4)?(opt=0):(opt)) 
//    {
//         case 0:
//                MD_DIS_T5_MPA;
//                *dis_dot=dot; 
//                *dis_opt=0;
//                break;
//         case 1:
//                MD_DIS_T4_BAR;
//                *dis_dot=dot-1; 
//                *dis_opt=1;
//                break;
//         case 2:
//                *dis_dot=dot-3; 
//                if(*dis_dot>-1)
//                {
//                    tmp=num*145.0377439;
//                    tmp/=1000;
//                    num=tmp;
//                    MD_DIS_T3_PSI;
//                    *dis_opt=2;
//                    break;
//                }
                 
//         case 3:
//                    *dis_dot=dot-3; 
//                    if(*dis_dot>-1)
//                    {
//                        MD_DIS_T2_KPA;
//                        *dis_opt=3;
//                        break;
//                    }
                 
//         case 4: 
//                *dis_dot=dot-2; 
//                if(*dis_dot>-1)
//                {
//                    tmp=num*10.19716;
//                    tmp/=100;
//                    num=tmp;
//                    MD_DIS_T9_KGF_CM_2;
//                    *dis_opt=4;
//                    break;
//                }
               
//         default:
//                  *dis_dot=dot;
//                   MD_DIS_T5_MPA;
//                  *dis_opt=0;
//                   break;
//    }
//    return num;
 }
                
static long normal_mode_Kpa_switch(long num,int n,int *dis_dot,int opt,int *dis_opt)
{
//    long tmp;
//    const int  dot=device_comps.calibration_param.dot-lg(n);    //main  variable dot
//    num=num/n;  //main  variable num
//    switch((opt>5)?(opt=0):(opt)) 
//    {
//       case 0:
//                MD_DIS_T2_KPA;
//                *dis_dot=dot;
//                *dis_opt=0;
//                break;
//       case 1:
//                MD_DIS_T4_BAR;
//                #if(MD_DIS_DIG==MD_DIS5)
//                    num=num/pwr(dot-2);
//                    *dis_dot=4;
//                #elif(MD_DIS_DIG==MD_DIS4)
//                    num=num/pwr(dot+1-2);
//                    *dis_dot=3;
//                #endif
//                *dis_opt=1;
//                break;
//       case 2:
//                MD_DIS_T3_PSI;
//                num=num*145.0377439/1000;
//                *dis_dot=dot;
//                *dis_opt=2;
//                break;
//       case 3:
//                MD_DIS_T5_MPA;
//               #if(MD_DIS_DIG==MD_DIS5)
//                  num=num/pwr(dot-1);
//                  *dis_dot=4;
//               #elif(MD_DIS_DIG==MD_DIS4)
//                  num=num/pwr(dot+1-1);
//                  *dis_dot=3;
//               #endif
//                 *dis_opt=3;
//                break;
//       case 4: 
//              MD_DIS_T9_KGF_CM_2;
//              num=num*10.19716/100;
//              *dis_dot=dot+1;
//              *dis_opt=4;
//              break;
//       case 5: 
              
//              *dis_dot=dot-3;
//              if(*dis_dot>-1)
//              {
//                  MD_DIS_T8_PA;
//                   *dis_opt=5;
//                  break; 
//              }
                    
//       default:
              
//            MD_DIS_T2_KPA;
//            *dis_dot=dot; 
//            *dis_opt=0;
//            break;

//    }
//    return num;
}
static void normal_mode_display(unsigned char opt)
{
    display_temp();
    display_press();
    display_deltaP();
}
static void debug_mode_display(unsigned char opt)
{
    display_temp_adc();
    display_press_adc();
    display_deltaP_adc();
}
static void pwd_mode_display(unsigned char opt)
{
    if(hum_comps.dis_oper_mark._bit.cur0)
    {
        hum_comps.cursor_0_count++;
        if(hum_comps.cursor_0_count>2)
        {
           hum_comps.cursor_0_count=0;
            if(hum_comps.dis_oper_mark._bit.dis0)
            {
                hum_comps.dis_oper_mark._bit.dis0=0;
            }
            else
            {
               hum_comps.dis_oper_mark._bit.dis0=1;
            }
            display_line0_data();
        }
    }

}
static void param_query_mode_display(unsigned char opt)
{
//  //  if(hum_comps.dis_oper_mark._bit.refresh_option)
//    {
//        display_opt(opt);
//        hum_comps.dis_oper_mark._bit.refresh_option=0;
//    }
    
//    hum_comps.dig2_0=*(&device_comps.air_leak_param.unit+opt)%10;
//    hum_comps.dig2_1=*(&device_comps.air_leak_param.unit+opt)/10%10;
//    hum_comps.dig2_2=*(&device_comps.air_leak_param.unit+opt)/100%10;
//    hum_comps.dig2_3=*(&device_comps.air_leak_param.unit+opt)/1000%10;
//    hum_comps.dig2_4=*(&device_comps.air_leak_param.unit+opt)/10000%10;
//    hum_comps.dig2_5=*(&device_comps.air_leak_param.unit+opt)/100000%10;
//    switch(opt)
//    {
//        case 0:  //unit
//             hide_all_unit();
//             hide_zero(&hum_comps.dig2_5,6,1);
//             hum_comps.dot2_pos=0;
//             break;
//        case 1://Constant_pressure_value
              
//        case 2://buck_delta_value
//               hide_all_unit();
//               if(device_comps.calibration_param.unit==0)
//               {
//                   if(device_comps.calibration_param.dot==5)
//                   {
//                       if(device_comps.air_leak_param.unit==0)//Mpa
//                       {
//                             MD_DIS_T5_MPA;
//                             hum_comps.dot2_pos=4;
//                       }
//                       else if(device_comps.air_leak_param.unit==1)//Kpa
//                       {
//                             MD_DIS_T2_KPA;
//                             hum_comps.dot2_pos=1;
//                       }
//                       else if(device_comps.air_leak_param.unit==2)//psi
//                       {
//                             MD_DIS_T3_PSI;
//                             hum_comps.dot2_pos=2;
//                       }
//                       else if(device_comps.air_leak_param.unit==3)//bar
//                       {
//                             MD_DIS_T4_BAR;
//                             hum_comps.dot2_pos=3;
//                       }
//                   }
//                   else if(device_comps.calibration_param.dot==4)
//                   {
//                        if(device_comps.air_leak_param.unit==0)//Mpa
//                       {
//                             MD_DIS_T5_MPA;
//                             hum_comps.dot2_pos=3;
//                       }
//                       else if(device_comps.air_leak_param.unit==1)//Kpa
//                       {
//                             MD_DIS_T2_KPA;
//                             hum_comps.dot2_pos=0;
//                       }
//                       else if(device_comps.air_leak_param.unit==2)//psi
//                       {
//                             MD_DIS_T3_PSI;
//                             hum_comps.dot2_pos=1;
//                       }
//                       else if(device_comps.air_leak_param.unit==3)//bar
//                       {
//                             MD_DIS_T4_BAR;
//                             hum_comps.dot2_pos=2;
//                       }
//                   }
                  
//               }
//               else if((device_comps.calibration_param.unit&0x0f)==1)
//               {
//                   if(device_comps.calibration_param.dot==3)
//                   {
//                       if(device_comps.air_leak_param.unit==0)//Mpa
//                       {
//                             MD_DIS_T5_MPA;
//                             hum_comps.dot2_pos=4;
//                       }
//                       else if(device_comps.air_leak_param.unit==1)//Kpa
//                       {
//                             MD_DIS_T2_KPA;
//                             hum_comps.dot2_pos=2;
//                       }
//                       else if(device_comps.air_leak_param.unit==2)//psi
//                       {
//                             MD_DIS_T3_PSI;
//                             hum_comps.dot2_pos=3;
//                       }
//                       else if(device_comps.air_leak_param.unit==3)//bar
//                       {
//                             MD_DIS_T4_BAR;
//                             hum_comps.dot2_pos=4;
//                       }
//                   }

//               }
//               hide_zero(&hum_comps.dig2_5,6,5);
//               break;
//         case 3://buck_percent
//               hide_all_unit();
//               hum_comps.dot2_pos=1;
//               hide_zero(&hum_comps.dig2_5,6,3);
//               break;
//         case 4://work_mode
//           hide_all_unit();
//           hum_comps.dot2_pos=0;
//           hide_zero(&hum_comps.dig2_5,6,1);
//           break;
//         case 5://count_direction
//           hide_all_unit();
//           hum_comps.dot2_pos=0;
//           hide_zero(&hum_comps.dig2_5,6,1);
//           break;  
//         case 6://delay_timer
//           hide_all_unit();
//           hum_comps.dot2_pos=0;
//           hide_zero(&hum_comps.dig2_5,6,4);
//           break;  
//         case 7://timer
//           hide_all_unit();
//           hum_comps.dot2_pos=0;
//           hide_zero(&hum_comps.dig2_5,6,4);
//           break;
           
//     }
//    display_line2_data();
   
       
}
static void param_modify_mode_display(unsigned char opt)
{
     //   if(hum_comps.dis_oper_mark._bit.refresh_option)
        {
            display_opt(opt);
            hum_comps.dis_oper_mark._bit.refresh_option=0;
        }
        if(hum_comps.dis_oper_mark._bit.cur2)
        {
            hum_comps.cursor_2_count++;
            if(hum_comps.cursor_2_count>2)
            {
                hum_comps.cursor_2_count=0;
                if(hum_comps.dis_oper_mark._bit.dis2)
                {
                    hum_comps.dis_oper_mark._bit.dis2=0;
                }
                else
                {
                    hum_comps.dis_oper_mark._bit.dis2=1;
                }
                
                display_line2_data();
            /*  if(sensor_comps.sw&MD_DENSITY_STB)
                {
                     DIS_S5_STB; 
                }
                else
                {
                    FLASH_S5_STB;
                }
            */
            }
        }

	
}

static void self_test_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark._bit.refresh_err_code)
	{
        hum_comps.dig4_0=0;
		hum_comps.dig4_1=0;
		hum_comps.dig4_2=0;
		hum_comps.dig4_3=0;
		hum_comps.dig4_4=0;
	    if(device_comps.sw._bit.e2prom_driver_err)
		{
			hum_comps.dig4_0=1;
		}
	#if(MD_PRODUCT_NAME==MD_LORA)
		if(!loraComps.sw._bit.init_ok)
		{
			hum_comps.dig4_1=1;
		}
	#endif	
		if(device_comps.sw._bit.cs123x_driver_err)
		{
			hum_comps.dig4_2=1;
		}
		if(device_comps.sw._bit.cs1237_driver_err)
		{
			hum_comps.dig4_3=1;
		}
		hum_comps.dig4_5=0x0e;
		hum_comps.dig4_6=MD_HIDE_DISP;
        hum_comps.dig4_7=MD_HIDE_DISP;
		hum_comps.dig4_8=MD_HIDE_DISP;
		hum_comps.dot4_pos=0;// 
		display_line4_data();

	    hum_comps.dis_oper_mark._bit.refresh_err_code=0;
	}
}
/*********************************END DISPLAY FNCTION************************************************************/






static void lcd_test(hum_comps_t *const this)
{
    void (*p[])(unsigned char dig)={ MD_DISPLAY_WRITE_D0_0,MD_DISPLAY_WRITE_D0_1,MD_DISPLAY_WRITE_D0_2,MD_DISPLAY_WRITE_D0_3,MD_DISPLAY_WRITE_D0_4,MD_DISPLAY_WRITE_D0_5,
	                                 MD_DISPLAY_WRITE_D1_0,MD_DISPLAY_WRITE_D1_1,MD_DISPLAY_WRITE_D1_2,MD_DISPLAY_WRITE_D1_3,MD_DISPLAY_WRITE_D1_4,
	                                 MD_DISPLAY_WRITE_D2_0,MD_DISPLAY_WRITE_D2_1,MD_DISPLAY_WRITE_D2_2,MD_DISPLAY_WRITE_D2_3,MD_DISPLAY_WRITE_D2_4,MD_DISPLAY_WRITE_D2_5,
	                                 MD_DISPLAY_WRITE_D3_0,MD_DISPLAY_WRITE_D3_1,MD_DISPLAY_WRITE_D3_2,MD_DISPLAY_WRITE_D3_3,MD_DISPLAY_WRITE_D3_4,MD_DISPLAY_WRITE_D3_5,
                                     MD_DISPLAY_WRITE_D4_0,MD_DISPLAY_WRITE_D4_1,MD_DISPLAY_WRITE_D4_2,MD_DISPLAY_WRITE_D4_3,MD_DISPLAY_WRITE_D4_4,MD_DISPLAY_WRITE_D4_5,
	                                 MD_DISPLAY_WRITE_D4_6,MD_DISPLAY_WRITE_D4_7,MD_DISPLAY_WRITE_D4_8};  
    if(this->count%2==0)
    {
        int k=this->count/2;
        if(k<6)
        {
            p[k]( *(&this->dig0_0+k));
        }
        else if(k<11)
        {
             p[k]( *(&this->dig1_0+k-6));
        }
        else if(k<17)
        {
             p[k]( *(&this->dig2_0+k-11));
        }
        else if(k<23)
        {
             p[k]( *(&this->dig3_0+k-17));
        }
        else if(k<33)
        {
             p[k]( *(&this->dig4_0+k-23));
        }
        else if(k==33)
        {
            dis_all_unit();
        }
    }
}




static void handle_key(hum_comps_t *this)
{
	this->get_key(this);
	if(this->key_type==EM_NO_KEY)
	{
		return;
	}
	else if(this->key_type==EM_SHORT_KEY)
	{
		switch(this->key)
		{
			case MD_S_KEY:
					on_s_key();
			 		break;
			case MD_M_KEY:
					on_m_key();
	 				break;
			case MD_J_KEY:
					on_j_key();
	 				break;
			default:
					break;
		}
	}
	else if(this->key_type==EM_LONG_KEY)
	{
		switch(this->key)
		{
			case MD_S_KEY:
					on_long_s_key();
					break;
			case MD_M_KEY:
					on_long_m_key();
					break;
			case MD_J_KEY:
					on_long_j_key();
					break;
			case MD_S_KEY&MD_J_KEY:
					on_long_s_and_j_key();
					break;	
			
			default:
					break;
		}
	}
	
}


static void cal_query_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark._bit.refresh_option)
    {
        display_opt(opt);
        hum_comps.dis_oper_mark._bit.refresh_option=0;
    }
    if(device_comps.cal_type==EM_CAL_PRESS)
    {
     
        if(hum_comps.dis_oper_mark._bit.refresh_cal_param)
        {
            display_cal_adc_on_line4(opt);
            hum_comps.dig2_0=device_comps.calibration_param.y[opt]%10;
            hum_comps.dig2_1=device_comps.calibration_param.y[opt]/10%10;
            hum_comps.dig2_2=device_comps.calibration_param.y[opt]/100%10;
            hum_comps.dig2_3=device_comps.calibration_param.y[opt]/1000%10;
            hum_comps.dig2_4=device_comps.calibration_param.y[opt]/10000%10;
            hum_comps.dig2_5=device_comps.calibration_param.y[opt]/100000%10;
            hum_comps.dot2_pos=device_comps.calibration_param.dot;
            display_line2_data();
            hum_comps.dis_oper_mark._bit.refresh_cal_param=0;
            
       }
        
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        if(hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param)
        {
            display_cal_adc_on_line4(opt);
            hum_comps.dig3_0=device_comps.deltaP_calibration_param.y[opt]%10;
            hum_comps.dig3_1=device_comps.deltaP_calibration_param.y[opt]/10%10;
            hum_comps.dig3_2=device_comps.deltaP_calibration_param.y[opt]/100%10;
            hum_comps.dig3_3=device_comps.deltaP_calibration_param.y[opt]/1000%10;
            hum_comps.dig3_4=device_comps.deltaP_calibration_param.y[opt]/10000%10;
            hum_comps.dig3_5=device_comps.deltaP_calibration_param.y[opt]/100000%10;
            hum_comps.dot3_pos=device_comps.deltaP_calibration_param.dot;
            display_line3_data();
            hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=0;
        }
    }
    else if(device_comps.cal_type==EM_CAL_RES)
    {
        if(hum_comps.dis_oper_mark._bit.refresh_res_cal_param)
        {
            display_cal_adc_on_line4(opt);
            hum_comps.dig1_0=device_comps.res_calibration_param.y[opt]%10;
            hum_comps.dig1_1=device_comps.res_calibration_param.y[opt]/10%10;
            hum_comps.dig1_2=device_comps.res_calibration_param.y[opt]/100%10;
            hum_comps.dig1_3=device_comps.res_calibration_param.y[opt]/1000%10;
            hum_comps.dig1_4=device_comps.res_calibration_param.y[opt]/10000%10;
            hum_comps.dot1_pos=2;
            display_line1_data();
            hum_comps.dis_oper_mark._bit.refresh_res_cal_param=0;
        }
    }
    
 
    
}
static void cal_modify_mode_display(unsigned char opt)
{
	if(hum_comps.dis_oper_mark._bit.refresh_option)
    {
        display_opt(opt);
        hum_comps.dis_oper_mark._bit.refresh_option=0;
    }
    display_current_adc_on_line4(opt);
    if(device_comps.cal_type==EM_CAL_PRESS)
    {
        if(hum_comps.dis_oper_mark._bit.refresh_cal_param)
        {
            hum_comps.dig2_0=device_comps.calibration_param.y[opt]%10;
            hum_comps.dig2_1=device_comps.calibration_param.y[opt]/10%10;
            hum_comps.dig2_2=device_comps.calibration_param.y[opt]/100%10;
            hum_comps.dig2_3=device_comps.calibration_param.y[opt]/1000%10;
            hum_comps.dig2_4=device_comps.calibration_param.y[opt]/10000%10;
            hum_comps.dig2_5=device_comps.calibration_param.y[opt]/100000%10;
            hum_comps.dot2_pos=device_comps.calibration_param_bak.dot;
            hum_comps.dis_oper_mark._bit.refresh_cal_param=0;
        }
       if(hum_comps.dis_oper_mark._bit.cur2)
       {
    		hum_comps.cursor_2_count++;
    		if(hum_comps.cursor_2_count>2)
    		{
    		    hum_comps.cursor_2_count=0;
    			if(hum_comps.dis_oper_mark._bit.dis2)
    			{
    				hum_comps.dis_oper_mark._bit.dis2=0;
    			}
    			else
    			{
    				hum_comps.dis_oper_mark._bit.dis2=1;
    			}
            }
       }
       display_line2_data();
    }
    else if(device_comps.cal_type==EM_CAL_DELTAP)
    {
        if(hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param)
        {
            
            hum_comps.dig3_0=device_comps.deltaP_calibration_param.y[opt]%10;
            hum_comps.dig3_1=device_comps.deltaP_calibration_param.y[opt]/10%10;
            hum_comps.dig3_2=device_comps.deltaP_calibration_param.y[opt]/100%10;
            hum_comps.dig3_3=device_comps.deltaP_calibration_param.y[opt]/1000%10;
            hum_comps.dig3_4=device_comps.deltaP_calibration_param.y[opt]/10000%10;
            hum_comps.dig3_5=device_comps.deltaP_calibration_param.y[opt]/100000%10;
            hum_comps.dot3_pos=device_comps.deltaP_calibration_param_bak.dot;
            hum_comps.dis_oper_mark._bit.refresh_deltaP_cal_param=0;
        } 
       if(hum_comps.dis_oper_mark._bit.cur3)
       {
    		hum_comps.cursor_3_count++;
    		if(hum_comps.cursor_3_count>2)
    		{
    		    hum_comps.cursor_3_count=0;
    			if(hum_comps.dis_oper_mark._bit.dis3)
    			{
    				hum_comps.dis_oper_mark._bit.dis3=0;
    			}
    			else
    			{
    				hum_comps.dis_oper_mark._bit.dis3=1;
    			}
            }
       }
       display_line3_data();    
    }
     else if(device_comps.cal_type==EM_CAL_RES)
    {
         if(hum_comps.dis_oper_mark._bit.refresh_res_cal_param)
        {
 
            hum_comps.dig1_0=device_comps.res_calibration_param.y[opt]%10;
            hum_comps.dig1_1=device_comps.res_calibration_param.y[opt]/10%10;
            hum_comps.dig1_2=device_comps.res_calibration_param.y[opt]/100%10;
            hum_comps.dig1_3=device_comps.res_calibration_param.y[opt]/1000%10;
            hum_comps.dig1_4=device_comps.res_calibration_param.y[opt]/10000%10;
            hum_comps.dot1_pos=2;
            hum_comps.dis_oper_mark._bit.refresh_res_cal_param=0;
        }   
       if(hum_comps.dis_oper_mark._bit.cur1)
       {
    		hum_comps.cursor_1_count++;
    		if(hum_comps.cursor_1_count>2)
    		{
    		    hum_comps.cursor_1_count=0;
    			if(hum_comps.dis_oper_mark._bit.dis1)
    			{
    				hum_comps.dis_oper_mark._bit.dis1=0;
    			}
    			else
    			{
    				hum_comps.dis_oper_mark._bit.dis1=1;
    			}
            }
       }
       display_line1_data();   
    }
   
	
}
static air_leak_mode_display(unsigned char opt)
{
//   if(hum_comps.dis_oper_mark._bit.air_leak_mode_refresh_timer)
//   {
//         long num=device_comps.air_leak.timer; 
//         hum_comps.dig1_0=num%10;
//         hum_comps.dig1_1=num/10%10;
//         hum_comps.dig1_2=num/100%10;
//         hum_comps.dig1_3=num/1000%10;
//         hum_comps.dis_oper_mark._bit.air_leak_mode_refresh_timer=0;
//   }
    
//    if(device_comps.air_leak.sw._bit.count_en&&device_comps.air_leak.delay_timer>0)
//    {
//        hum_comps.dis_oper_mark._bit.cur1=1;
//        hum_comps.cursor_1=-1;
//        hum_comps.cursor_1_count++;
//        if(hum_comps.cursor_1_count>9)
//        {
//            hum_comps.cursor_1_count=0;
//            if(hum_comps.dis_oper_mark._bit.dis1)
//            {
//                hum_comps.dis_oper_mark._bit.dis1=0;
//            }
//            else
//            {
//               hum_comps.dis_oper_mark._bit.dis1=1;
//            }
//        }
//    }
//    else 
//    {
//        hum_comps.dis_oper_mark._bit.cur1=0;
//        hum_comps.dis_oper_mark._bit.dis1=1;
   
//    }
//    display_line1_data();
    

//    if(device_comps.calibration_param.is_calibrated)
//    {
//        if(hum_comps.dis_oper_mark._bit.refresh_press)
//        {
//           long num;
//           if(device_comps.calibration_param.unit==0)
//           {
//               if(device_comps.air_leak_param.unit==2)//psi
//               {
//                   num=device_comps.current_press/10*145.0377439;
//                   num/=100;
//               }
//               else
//               {
//                   num=device_comps.current_press/10;
//               }
//                hum_comps.dig2_0=num%10;
//                hum_comps.dig2_1=num/10%10;
//                hum_comps.dig2_2=num/100%10;
//                hum_comps.dig2_3=num/1000%10;
//                hum_comps.dig2_4=num/10000%10;
//                hum_comps.dig2_5=num/100000%10;
//               if(device_comps.calibration_param.dot==5)
//               {
                   
//                   if(device_comps.air_leak_param.unit==0)//Mpa
//                   {
//                         MD_DIS_T5_MPA;
//                         hum_comps.dot2_pos=4;
//                   }
//                   else if(device_comps.air_leak_param.unit==1)//Kpa
//                   {
//                         MD_DIS_T2_KPA;
//                         hum_comps.dot2_pos=1;
//                   }
//                   else if(device_comps.air_leak_param.unit==2)//psi
//                   {
//                         MD_DIS_T3_PSI;
//                         hum_comps.dot2_pos=2;
//                   }
//                   else if(device_comps.air_leak_param.unit==3)//bar
//                   {
//                         MD_DIS_T4_BAR;
//                         hum_comps.dot2_pos=3;
//                   }
//               }
//               else if(device_comps.calibration_param.dot==4)
//               {
//                    if(device_comps.air_leak_param.unit==0)//Mpa
//                   {
//                         MD_DIS_T5_MPA;
//                         hum_comps.dot2_pos=3;
//                   }
//                   else if(device_comps.air_leak_param.unit==1)//Kpa
//                   {
//                         MD_DIS_T2_KPA;
//                         hum_comps.dot2_pos=0;
//                   }
//                   else if(device_comps.air_leak_param.unit==2)//psi
//                   {
//                         MD_DIS_T3_PSI;
//                         hum_comps.dot2_pos=1;
//                   }
//                   else if(device_comps.air_leak_param.unit==3)//bar
//                   {
//                         MD_DIS_T4_BAR;
//                         hum_comps.dot2_pos=2;
//                   }
//               }
              
//           }
//           else if((device_comps.calibration_param.unit&0x0f)==1)
//           {
//               if(device_comps.air_leak_param.unit==2)//psi
//               {
//                   num=device_comps.current_press/10*145.0377439;
//                   num/=100;
//               }
//               else
//               {
//                   num=device_comps.current_press/10;
//               }
//                hum_comps.dig2_0=num%10;
//                hum_comps.dig2_1=num/10%10;
//                hum_comps.dig2_2=num/100%10;
//                hum_comps.dig2_3=num/1000%10;
//                hum_comps.dig2_4=num/10000%10;
//                hum_comps.dig2_5=num/100000%10;
//               if(device_comps.calibration_param.dot==3)
//               {
//                   if(device_comps.air_leak_param.unit==0)//Mpa
//                   {
//                         MD_DIS_T5_MPA;
//                         hum_comps.dot2_pos=5;
//                   }
//                   else if(device_comps.air_leak_param.unit==1)//Kpa
//                   {
//                         MD_DIS_T2_KPA;
//                         hum_comps.dot2_pos=2;
//                   }
//                   else if(device_comps.air_leak_param.unit==2)//psi
//                   {
//                         MD_DIS_T3_PSI;
//                         hum_comps.dot2_pos=3;
//                   }
//                   else if(device_comps.air_leak_param.unit==3)//bar
//                   {
//                         MD_DIS_T4_BAR;
//                         hum_comps.dot2_pos=4;
//                   }
                   
//               }

//           }
//           hide_zero(&hum_comps.dig2_5,6,hum_comps.dot2_pos+1);
//          hum_comps.dis_oper_mark._bit.refresh_press=0;
//       }
//        if(!device_comps.sw._bit.over_range)
//        {
//            hum_comps.dis_oper_mark._bit.cur2=0;
//            hum_comps.dis_oper_mark._bit.dis2=1;
//        }
//        else
//        {
//            hum_comps.dis_oper_mark._bit.cur2=1;
//            hum_comps.cursor_2=-1;
//            hum_comps.cursor_2_count++;
//            if(hum_comps.cursor_2_count>9)
//            {
//                hum_comps.cursor_2_count=0;
//                if(hum_comps.dis_oper_mark._bit.dis2)
//                {
//                    hum_comps.dis_oper_mark._bit.dis2=0;
//                    //hum_comps.dot2_pos=0;//
//                }
//                else
//                {
//                   hum_comps.dis_oper_mark._bit.dis2=1;
//                }
//            }
//        }     
//    }    
//    else
//    {
//        hum_comps.dis_oper_mark._bit.cur2=0;
//        hum_comps.dig2_0=MD_HIDE_DISP-1;
//        hum_comps.dig2_1=MD_HIDE_DISP-1;
//        hum_comps.dig2_2=MD_HIDE_DISP-1;
//        hum_comps.dig2_3=MD_HIDE_DISP-1;
//        hum_comps.dig2_4=MD_HIDE_DISP-1;
//        hum_comps.dig2_5=MD_HIDE_DISP-1;
//        hum_comps.dot2_pos=0;
        
//    }
//    display_line2_data();
    
  

//    if(1)
//    {
//        static unsigned char  onTimer=0;
//        static unsigned char  OffTimer=0;
//        static unsigned char  init_led_once=1;
//        if(device_comps.air_leak.sw._bit.test_finished)
//        {

//            if(device_comps.air_leak.sw._bit.isWarn)
//            {
//                if(init_led_once)
//                {
//                    MD_SET_RED_LIGHT_ON;
//                    init_led_once=0;
//                }
//                if(isLightOn(MD_RED_LIGHT))
//                {
//                    onTimer++;
//                    if(onTimer>3)
//                    {
//                        MD_SET_RED_LIGHT_OFF;
//                        onTimer=0;
//                    }
//                }
//                else
//                {
//                    OffTimer++;
//                    if(OffTimer>49)
//                    {
//                          MD_SET_RED_LIGHT_ON;
//                          OffTimer=0;
                        
//                    }
//                }
//            }
//            else 
//            {
//                 if(init_led_once)
//                {
//                    MD_SET_GREEN_LIGHT_ON;
//                    init_led_once=0;
//                }
//                 if(isLightOn(MD_GREEN_LIGHT))
//                {
//                    onTimer++;
//                    if(onTimer>3)
//                    {
//                        MD_SET_GREEN_LIGHT_OFF;
//                        onTimer=0;
//                    }
//                }
//                else
//                {
//                    OffTimer++;
//                    if(OffTimer>49)
//                    {
//                          MD_SET_GREEN_LIGHT_ON;
//                          OffTimer=0;
                        
//                    }
//                }
//            }
//        }
//        else 
//        {
//             MD_SET_RED_LIGHT_OFF;
//             MD_SET_GREEN_LIGHT_OFF;
//             init_led_once=1;

//        }
//    }
}
static void display_temp_to_line2(void)
{
     
//	long num=device_comps.current_temp;
//	 MD_DIS_T10_CELSIUS;
//	switch(num<0)
//	{
//		case 1: num=-num;
//			if(num<100)
//			{
//				hum_comps.dig2_0=num%10;
//				hum_comps.dig2_1=num/10%10;
//				hum_comps.dot2_pos=1;//                                                                
//				hum_comps.dig2_2=MD_HIDE_DISP-1;//dis -  
//				hum_comps.dig2_3=MD_HIDE_DISP;//
//				hum_comps.dig2_4=MD_HIDE_DISP;// 
//				hum_comps.dig2_5=MD_HIDE_DISP;// 
//			}
//			else if(num<1000)
//			{
//			    hum_comps.dig2_0=num%10;
//				hum_comps.dig2_1=num/10%10;
//				hum_comps.dig2_2=num/100%10;
//				hum_comps.dot2_pos=1;
//				hum_comps.dig2_3=MD_HIDE_DISP-1;//
//				hum_comps.dig2_4=MD_HIDE_DISP;// 
//				hum_comps.dig2_5=MD_HIDE_DISP;//
//			}
//			else
//			{
//				hum_comps.dig2_0=MD_HIDE_DISP;
//				hum_comps.dig2_1=MD_HIDE_DISP;
//				hum_comps.dig2_2=MD_HIDE_DISP;// 
//				hum_comps.dot2_pos=0;
//				hum_comps.dig2_3=MD_HIDE_DISP;//
//				hum_comps.dig2_4=MD_HIDE_DISP;//
//				hum_comps.dig2_5=MD_HIDE_DISP-1;//Overrange display - _ _
//			}
//			break;
		
//		case 0:
//			if(num<100)
//			{
//				hum_comps.dig2_0=num%10;
//				hum_comps.dig2_1=num/10%10;
//				hum_comps.dot2_pos=1;//P2    
//				hum_comps.dig2_2=MD_HIDE_DISP;
//				hum_comps.dig2_3=MD_HIDE_DISP;
//				hum_comps.dig2_4=MD_HIDE_DISP;//dis - 
//				hum_comps.dig2_5=MD_HIDE_DISP;//dis -
//			}
//			else if(num<1000)
//			{
//				hum_comps.dig2_0=num%10;
//				hum_comps.dig2_1=num/10%10;
//				hum_comps.dot2_pos=1;//P2     
//				hum_comps.dig2_2=num/100%10;
//				hum_comps.dig2_3=MD_HIDE_DISP;
//				hum_comps.dig2_4=MD_HIDE_DISP;//dis - 
//				hum_comps.dig2_5=MD_HIDE_DISP;//dis -
//			}
//			else if(num<10000)
//			{
//				hum_comps.dig2_0=num%10;
//				hum_comps.dig2_1=num/10%10;
//				hum_comps.dig2_2=num/100%10;
//				hum_comps.dig2_3=num/1000%10;
//				hum_comps.dot2_pos=1;//  
//				hum_comps.dig2_4=MD_HIDE_DISP;//dis - 
//				hum_comps.dig2_5=MD_HIDE_DISP;//dis -
//			}
//			else   // Overrange display 1_ _
//			{
//				hum_comps.dig2_0=MD_HIDE_DISP;
//				hum_comps.dig2_1=MD_HIDE_DISP;
//				hum_comps.dig2_2=MD_HIDE_DISP;
//				hum_comps.dig2_3=MD_HIDE_DISP;
//				hum_comps.dig2_4=MD_HIDE_DISP;//dis 
//				hum_comps.dig2_5=1;
//				hum_comps.dot2_pos=0;
//		    }
//			break;
//		default:
//			break;
//	}
//	display_line2_data();//display 1 line data

}

static void lora_mode_display(unsigned char opt) 
{

//        if(hum_comps.dis_oper_mark._bit.refresh_option)
//        {
//            display_opt(opt);
//            hum_comps.dis_oper_mark._bit.refresh_option=0;
//        }
        
//        switch(opt)
//        {
//            case 0: 
//            case 1: 
//            case 2: 
//                   #ifdef MD_TEMP
//                        if(hum_comps.dis_oper_mark._bit.refresh_temp)
//                    	{
//                            display_temp_to_line2();//refresh temperature
//                    		hum_comps.dis_oper_mark._bit.refresh_temp=0;
//                    	}
//                    	mode_comps[hum_comps.current_mode].dis_option=2;
//                	#else
//                        display_press(opt);
//                    #endif
//                    break;
//            case 3:
//                   if(hum_comps.dis_oper_mark._bit.refresh_date)
//                   {
//                        hum_comps.dot2_pos=0;
//                        hum_comps.dig2_0=rtc_valve.day&0x0f;
//                        hum_comps.dig2_1=rtc_valve.day>>4;
//                        hum_comps.dig2_2=rtc_valve.month&0x0f;
//                        hum_comps.dig2_3=rtc_valve.month>>4;
//                        hum_comps.dig2_4=rtc_valve.year&0x0f;
//                        hum_comps.dig2_5=rtc_valve.year>>4;
//                        display_line2_data();    // 
//                        MD_HIDE_T10_CELSIUS;MD_HIDE_T5_MPA;MD_HIDE_T2_KPA;MD_HIDE_T7_M;
//                        MD_DIS_P6_DOT;
//                        MD_DIS_P8_DOT;
//                        hum_comps.dis_oper_mark._bit.refresh_date=0;
//                    }
                   
//                    break;
//                //display date
//            case 4: 
//                    if(hum_comps.dis_oper_mark._bit.refresh_time)
//                    {
//                        hum_comps.dot2_pos=0;
//                        hum_comps.dig2_0=rtc_valve.sec&0x0f;
//                        hum_comps.dig2_1=rtc_valve.sec>>4;
//                        hum_comps.dig2_2=rtc_valve.min&0x0f;
//                        hum_comps.dig2_3=rtc_valve.min>>4;
//                        hum_comps.dig2_4=rtc_valve.hour&0x0f;
//                        hum_comps.dig2_5=rtc_valve.hour>>4;
//                         display_line2_data();    // 
//                         MD_HIDE_T5_MPA;MD_HIDE_T2_KPA;MD_HIDE_T7_M;
//                         MD_HIDE_P6_DOT;
//                        MD_HIDE_P8_DOT;
//                        MD_DIS_COL;
//                        hum_comps.dis_oper_mark._bit.refresh_time=0;
//                    }
//            //display time
//                break;  

//                case 5:     //display device id hi
//                    if(hum_comps.dis_oper_mark._bit.refresh_device_id_high)
//                    {
//                        if(loraComps.sw._bit.noParameter)
//                        {
//                            hum_comps.dot2_pos=0;
//                            hum_comps.dig2_0=MD_HIDE_DISP;
//                            hum_comps.dig2_1=MD_HIDE_DISP;
//                            hum_comps.dig2_2=MD_HIDE_DISP;
//                            hum_comps.dig2_3=MD_HIDE_DISP;
//                            hum_comps.dig2_4=MD_HIDE_DISP;
//                            hum_comps.dig2_5=MD_HIDE_DISP;
//                        }
//                        else
//                        {
//                            hum_comps.dot2_pos=0;
//                            hum_comps.dig2_0=(loraComps.yl701_info_p->freq)&0x0000000f;
//                            hum_comps.dig2_1=(loraComps.yl701_info_p->freq>>4)&0x0000000f;
//                            hum_comps.dig2_2=(loraComps.yl701_info_p->freq>>8)&0x0000000f;
//                            hum_comps.dig2_3=(loraComps.yl701_info_p->freq>>12)&0x0000000f;
//                            hum_comps.dig2_4=(loraComps.yl701_info_p->freq>>16)&0x0000000f;
//                            hum_comps.dig2_5=(loraComps.yl701_info_p->freq>>20)&0x0000000f;
//                        }
                       
//                        display_line2_data();    // 
//                        MD_HIDE_COL;MD_HIDE_T5_MPA;MD_HIDE_T2_KPA;MD_HIDE_T7_M;
//                        hum_comps.dis_oper_mark._bit.refresh_device_id_high=0;
//                    }
        
//                    break;
//                case 6:     //display device id low
//                    if(hum_comps.dis_oper_mark._bit.refresh_device_id_low)
//                    {
//                         if(loraComps.sw._bit.noParameter)
//                        {
//                            hum_comps.dot2_pos=0;
//                            hum_comps.dig2_0=MD_HIDE_DISP;
//                            hum_comps.dig2_1=MD_HIDE_DISP;
//                            hum_comps.dig2_2=MD_HIDE_DISP;
//                            hum_comps.dig2_3=MD_HIDE_DISP;
//                            hum_comps.dig2_4=MD_HIDE_DISP;
//                            hum_comps.dig2_5=MD_HIDE_DISP;
//                        }
//                        else
//                        {
//                            hum_comps.dot2_pos=0;
//                            hum_comps.dig2_0=loraComps.yl701_info_p->NodeId%10;
//                            hum_comps.dig2_1=loraComps.yl701_info_p->NodeId/10%10;
//                            hum_comps.dig2_2=loraComps.yl701_info_p->NodeId/100%10;
//                            hum_comps.dig2_3=loraComps.yl701_info_p->netId%10;
//                            hum_comps.dig2_4=loraComps.yl701_info_p->breathTime%10;
//                            hum_comps.dig2_5=loraComps.yl701_info_p->breathPeriod%10;
//                        }
//                        display_line2_data();    // 
//                        MD_HIDE_COL;MD_HIDE_T5_MPA;MD_HIDE_T2_KPA;MD_HIDE_T7_M;
//                        hum_comps.dis_oper_mark._bit.refresh_device_id_low=0;
//                    }
        
//                    break;
//                case 7:     //refresh ver
//                    if(hum_comps.dis_oper_mark._bit.refresh_ver)
//                    {
//                        hum_comps.dot2_pos=1;
//                        hum_comps.dig2_0=MD_FL_VER%10;
//                        hum_comps.dig2_1=MD_FL_VER/10%10;
//                        hum_comps.dig2_2=MD_HIDE_DISP-1;
//                        hum_comps.dig2_3=0x0f;
//                        hum_comps.dig2_4=MD_HIDE_DISP;
//                        hum_comps.dig2_5=MD_HIDE_DISP;
//                        display_line2_data();    // 
//                        MD_HIDE_COL;MD_HIDE_T5_MPA;MD_HIDE_T2_KPA;MD_HIDE_T7_M;
//                        hum_comps.dis_oper_mark._bit.refresh_ver=0;
//                    }
        
//                    break;
//             default:
//                    mode_comps[hum_comps.current_mode].dis_option=0;
//                break;
        
        
//    }
//    #ifndef MD_TEMP
//    if(hum_comps.dis_oper_mark._bit.refresh_temp)
//    {
//        display_temp();//refresh temperature
//        hum_comps.dis_oper_mark._bit.refresh_temp=0;
//    }
//    #endif
    
}

static void report_mode_display(unsigned char opt)
{
//    opt= netComps.disCode;
//	switch(opt)
//     {
//        case EM_DIS_ACT://---act
//           if(mode_comps[hum_comps.current_mode].displayTimer<60)
//           {
//                    hum_comps.dig1_0=device_comps.access_param.ip[2]&0x0f;
//                    hum_comps.dig1_1=(device_comps.access_param.ip[2]>>4)&0x0f;
//                    hum_comps.dig1_2=device_comps.access_param.ip[1]&0x0f;
//                     hum_comps.dig1_3=(device_comps.access_param.ip[1]>>4)&0x0f;
//                    hum_comps.dot1_pos=2;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=device_comps.access_param.ip[0]&0x0f;
//                    hum_comps.dig0_1=(device_comps.access_param.ip[0]>>4)&0x0f;
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//           }
//           else if(mode_comps[hum_comps.current_mode].displayTimer<120)
//           {
//                    hum_comps.dig1_0=device_comps.access_param.port&0x0f;
//                    hum_comps.dig1_1=(device_comps.access_param.port>>4)&0x0f;;
//                    hum_comps.dig1_2=(device_comps.access_param.port>>8)&0x0f;
//                     hum_comps.dig1_3=(device_comps.access_param.port>>12)&0x0f;
//                    hum_comps.dot1_pos=0;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=device_comps.access_param.ip[3]&0x0f;
//                    hum_comps.dig0_1=(device_comps.access_param.ip[3]>>4)&0x0f;
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//           }
//           mode_comps[hum_comps.current_mode].displayTimer++;
//           if(mode_comps[hum_comps.current_mode].displayTimer>=140)
//           {
//                mode_comps[hum_comps.current_mode].displayTimer=0;
//           }
//            hum_comps.dig2_0=MD_DIS_T;
//            hum_comps.dig2_1=0x0c;
//            hum_comps.dig2_2=0x0a;//
//            hum_comps.dig2_3=MD_HIDE_DISP-1;
//            hum_comps.dig2_4=*protocolComps.event_index_pt%10;
//            hum_comps.dig2_5=*protocolComps.event_index_pt/10%10;
//            hum_comps.dot2_pos=0;
//            display_line2_data();
//             break;
//        case EM_DIS_OFF:// suc   or fal
//            if(protocolComps.sw._bit.dataPushYet)
//            {
//                hum_comps.dig2_0=0x0c;
//                hum_comps.dig2_1=MD_DIS_U;
//                hum_comps.dig2_2=5;//s
//                hum_comps.dig2_3=MD_HIDE_DISP;
//                hum_comps.dig2_4=MD_HIDE_DISP;
//                hum_comps.dig2_5=MD_HIDE_DISP;
//                hum_comps.dot2_pos=0;
//                
//            }
//            else
//            {
//                hum_comps.dig2_0=0x01;
//                hum_comps.dig2_1=0x0a;
//                hum_comps.dig2_2=0x0f;//
//                hum_comps.dig2_3=MD_HIDE_DISP;
//                hum_comps.dig2_4=MD_HIDE_DISP;
//                hum_comps.dig2_5=MD_HIDE_DISP;
//                hum_comps.dot2_pos=0;
//            }
//           display_line2_data();
//            break;
//        case EM_DIS_SEARCH_NET:
//            {
//                int temp=0;
//                hum_comps.dig2_0=MD_HIDE_DISP;
//                hum_comps.dig2_1=netComps.net_info.ecl;
//                hum_comps.dig2_2=0x0e;
//                hum_comps.dig2_3=MD_HIDE_DISP-1;
//                hum_comps.dig2_4=netComps.net_info.Csq%10;
//                hum_comps.dig2_5=netComps.net_info.Csq/10%10;
//                display_line2_data();
//                if(mode_comps[hum_comps.current_mode].displayTimer<40)//50ms*40
//                {
//                    temp=netComps.net_info.sina;
//                    if(temp<0)
//                    {
//                        temp=-netComps.net_info.sina;
//                        hum_comps.dig1_3=MD_HIDE_DISP-1;
//                        
//                    }
//                    else
//                    {
//                         hum_comps.dig1_3=MD_HIDE_DISP;
//                    }
//                    hum_comps.dig1_0=temp%10;
//                    hum_comps.dig1_1=temp/10%10;
//                    hum_comps.dig1_2=temp/100%10;
//                    hum_comps.dot1_pos=0;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=MD_HIDE_DISP;
//                    hum_comps.dig0_1=5;//sinr
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//
//                }
//                else if(mode_comps[hum_comps.current_mode].displayTimer<80)
//                {
//                    temp=netComps.net_info.rsrp;
//                    if(temp<0)
//                    {
//                        temp=-netComps.net_info.rsrp;
//                        hum_comps.dig1_3=MD_HIDE_DISP-1;
//                        
//                    }
//                    else
//                    {
//                         hum_comps.dig1_3=MD_HIDE_DISP;
//                    }
//                    hum_comps.dig1_0=temp%10;
//                    hum_comps.dig1_1=temp/10%10;
//                    hum_comps.dig1_2=temp/100%10;
//                    hum_comps.dot1_pos=0;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=MD_HIDE_DISP;
//                    hum_comps.dig0_1=MD_DIS_R;//rsip
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//               }
//                else if(mode_comps[hum_comps.current_mode].displayTimer<120)
//                {
//                    temp=netComps.net_info.pci;
//                  
//                    hum_comps.dig1_0=temp%10;
//                    hum_comps.dig1_1=temp/10%10;
//                    hum_comps.dig1_2=temp/100%10;
//                    hum_comps.dig1_3=temp/1000%10;
//                    hum_comps.dot1_pos=0;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=MD_HIDE_DISP;
//                    hum_comps.dig0_1=MD_DIS_P;//pci
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//
//                }
//                else if(mode_comps[hum_comps.current_mode].displayTimer<160)
//                {
//                    temp=netComps.net_info.earFcn;
//                  
//                    hum_comps.dig1_0=temp%10;
//                    hum_comps.dig1_1=temp/10%10;
//                    hum_comps.dig1_2=temp/100%10;
//                    hum_comps.dig1_3=temp/1000%10;
//                    hum_comps.dot1_pos=0;// 
//                    display_line1_data(); 
//                    
//                    hum_comps.dig0_0=MD_HIDE_DISP;
//                    hum_comps.dig0_1=0x0f;//earfcn
//                    hum_comps.dot0_pos=0;// 
//                    display_line0_data(); 
//
//                }
//                mode_comps[hum_comps.current_mode].displayTimer++;
//                if(mode_comps[hum_comps.current_mode].displayTimer>=200)
//                {
//                    mode_comps[hum_comps.current_mode].displayTimer=0;
//                }
//            }
//            break;
//        
//        case EM_DIS_REGISTER_NET:
//			break;
//        case EM_DIS_SEND://send
//                hum_comps.dig2_0=0x0d;
//                hum_comps.dig2_1=MD_DIS_N;
//                hum_comps.dig2_2=0x0e;//
//                hum_comps.dig2_3=5;//s
//                hum_comps.dig2_4=MD_HIDE_DISP;
//                hum_comps.dig2_5=MD_HIDE_DISP;
//                hum_comps.dot2_pos=0;
//                display_line2_data();
//                break;
//        case EM_DIS_RECV://rece 
//                hum_comps.dig2_0=0x0e;
//                hum_comps.dig2_1=0x0c;
//                hum_comps.dig2_2=0x0e;//
//                hum_comps.dig2_3=MD_DIS_R;//
//                hum_comps.dig2_4=MD_HIDE_DISP;
//                hum_comps.dig2_5=MD_HIDE_DISP;
//                hum_comps.dot2_pos=0;
//                display_line2_data();
//                break;
//        
//     }

}


static void  display_special(void)
{
    if(!ircComps.sw._bit.runing)
	{
        MD_HIDE_T1_COMMUNICATION;
	}
	else
	{
        MD_DIS_T1_COMMUNICATION;
	}
	if(loraComps.sw._bit.refresh_rssi)
	{

	   int rssi=-164+(int)(loraComps.yl701_info_p->rssi);
        if(rssi<-110)
        {
            MD_DIS_N1_CSQ;
            MD_HIDE_N2_CSQ;
            MD_HIDE_N3_CSQ;
            MD_HIDE_N4_CSQ;
        }
        else if(rssi<-90)
        {
            MD_DIS_N1_CSQ;
            MD_DIS_N2_CSQ;
            MD_HIDE_N3_CSQ;
            MD_HIDE_N4_CSQ;
        }
        else if(rssi<-70)
        {
            MD_DIS_N1_CSQ;
            MD_DIS_N2_CSQ;
            MD_DIS_N3_CSQ;
            MD_HIDE_N4_CSQ;
        }
        else 
        {
            MD_DIS_N1_CSQ;
            MD_DIS_N2_CSQ;
            MD_DIS_N3_CSQ;
            MD_DIS_N4_CSQ;
        }
     
         loraComps.sw._bit.refresh_rssi=0;
    }
  if(hum_comps.dis_oper_mark._bit.refresh_batt_symbol)
  {
       #if(MD_DEVICE_BATT ==MD_3_6V)
       {
            if(device_comps.batt<31)
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_HIDE_D2_BAT;
                MD_HIDE_D3_BAT;
        		
            }
            else if(device_comps.batt<33)
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_DIS_D3_BAT;
                MD_HIDE_D2_BAT;
        		
            }
            else 
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_DIS_D3_BAT;
                MD_DIS_D2_BAT;
            }
       }
       #elif(MD_DEVICE_BATT ==MD_4_5V)
       {
            if(device_comps.batt<33)
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_HIDE_D2_BAT;
                MD_HIDE_D3_BAT;
        		
            }
            else if(device_comps.batt<38)
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_DIS_D3_BAT;
                MD_HIDE_D2_BAT;
        		
            }
            else 
            {
                MD_DIS_D1_BAT;
                MD_DIS_D4_BAT;
                MD_DIS_D3_BAT;
                MD_DIS_D2_BAT;
            }
        }
        #endif
    }
    hum_comps.dis_oper_mark._bit.refresh_batt_symbol=0;
}

static void handle_lcd_refresh()
{
    mode_comps[hum_comps.current_mode].display(mode_comps[hum_comps.current_mode].dis_option);
    if(hum_comps.current_mode!=EM_SELF_TEST_MODE)
    {
        display_special();
    }
}

static void hum_comps_init(hum_comps_t *const this)
{
    clr_lcd();   
}
static void nop()
{
	NOP();
}


mode_comps_t  mode_comps[]=//Handling of keys in different modes
{	//mode_comps_t  mode_comps[5];
	{"normal mode"       ,mode_comps+0  ,EM_NORMAL_MODE        ,normal_mode_on_s_key      ,normal_mode_on_m_key      ,normal_mode_on_j_key      ,normal_mode_on_long_s_key       ,normal_mode_on_long_m_key      ,normal_mode_on_long_j_key      ,normal_mode_on_long_s_and_j_key,normal_mode_display      ,0,0},
	{"debug mode"        ,mode_comps+1  ,EM_DEBUG_MODE         ,debug_mode_on_s_key       ,debug_mode_on_m_key       ,debug_mode_on_j_key       ,nop                             ,debug_mode_on_long_m_key       ,nop                            ,debug_mode_on_long_s_and_j_key ,debug_mode_display       ,0,0},
	{"password mode"     ,mode_comps+2  ,EM_PWD_MODE           ,pwd_mode_on_s_key         ,pwd_mode_on_m_key         ,pwd_mode_on_j_key         ,nop		                     ,pwd_mode_on_long_m_key         ,nop                            ,pwd_mode_on_long_s_and_j_key   ,pwd_mode_display         ,0,0},
	{"cal_query mode"    ,mode_comps+3  ,EM_CAL_QUERY_MODE     ,cal_query_mode_on_s_key   ,cal_query_mode_on_m_key   ,cal_query_mode_on_j_key   ,nop		                     ,cal_query_mode_on_long_m_key   ,nop                            ,nop                            ,cal_query_mode_display   ,0,0},
	{"cal_modify mode"   ,mode_comps+4  ,EM_CAL_MODIFY_MODE    ,cal_modify_mode_on_s_key  ,cal_modify_mode_on_m_key  ,cal_modify_mode_on_j_key  ,cal_modify_mode_on_long_s_key   ,nop                            ,nop                            ,nop                            ,cal_modify_mode_display  ,0,0},
	
    {"param_query mode"  ,mode_comps+5  ,EM_PARAM_QUERY_MODE   ,param_query_mode_on_s_key ,param_query_mode_on_m_key ,nop                       ,nop		                     ,param_query_mode_on_long_m_key ,nop                            ,nop                            ,param_query_mode_display ,0,0},
	{"param_modify mode" ,mode_comps+6  ,EM_PARAM_MODIFY_MODE  ,param_modify_mode_on_s_key,param_modify_mode_on_m_key,param_modify_mode_on_j_key,param_modify_mode_on_long_s_key ,nop                            ,nop                            ,nop                            ,param_modify_mode_display,0,0},
    {"self_test_mode"    ,mode_comps+7  ,EM_SELF_TEST_MODE     ,nop                       ,nop                       ,nop          	            ,nop			                 ,nop               	         ,nop                            ,nop                            ,self_test_mode_display   ,0,0},
    {"report_mode"       ,mode_comps+8  ,EM_REPORT_MODE        ,nop                       ,nop                       ,nop          	            ,nop			                 ,nop               	         ,nop                            ,nop                            ,report_mode_display      ,0,0},
    {"air_leak_mode"     ,mode_comps+9  ,EM_AIR_LEAK_MODE      ,nop                       ,nop                       ,air_leak_mode_on_j_key    ,nop			                 ,air_leak_mode_on_long_m_key    ,normal_mode_on_long_j_key      ,normal_mode_on_long_s_and_j_key,air_leak_mode_display    ,0,0},
    {"lora mode"         ,mode_comps+10 ,EM_LORA_MODE          ,nop                       ,lora_mode_on_m_key        ,nop                       ,nop			                 ,nop                            ,normal_mode_on_long_j_key      ,normal_mode_on_long_s_and_j_key,lora_mode_display        ,0,0},
};

static void hum_comps_task_handle()////Execution interval is 50 ms
{
	
	hum_comps_t *const this=hum_comps.this;
	if(this->do_init==1)
	{
		this->init(this);
		this->do_init=0;
	}
	if(!(this->count%2)&&(this->count<55))
	{
		 lcd_test(this);//interval time 300ms
		
	}
	if(this->count==55)
	{
		handle_key(this);
		handle_lcd_refresh();
	}
    else
    {
        this->count++;
        if(this->count==55)
        {
             hum_comps.dis_oper_mark._bit.test_ok=1;
	     enter_self_test_mode();
        }
    }
}

hum_comps_t hum_comps=
{

    "Human machine interface",     //char *desc;
	&hum_comps,                    //struct _HUM_COMPONENTS *this;
	1,                             //int   do_init;//Whether to initialize,1:init 0,no init
	hum_comps_init,                //void (*const init)(struct _HUM_COMPONENTS *);
	0,                             //unsigned long count;            //task_handle Called counter
	
	///******************************lcd seg define******************************/
     /*0,  1,   2,    3,   4,  5,  6,   7,   8,   9,    A,   b,   C,   d,   E,    F,   P,   U,  t,   n,   r,   -,   NULL,*/ 
    {0x77,0x24, 0x5D ,0x6D, 0x2E,0x6B,0x7B,0x25, 0x7F, 0x6F,0x3F,0x7A, 0x53, 0x7C,0x5B, 0x1B, 0x1F,0x76, 0x5A,0x38,0x18, 0x08,  0},//const unsigned char  SEG_TAB[]
    /*0,   1,   2,    3,   4,  5,  6,   7,   8,   9,    A,   b,   C,   d,   E,    F,   P,   U,  t,   n,   r,   -,    NULL,*/
    {0xEE,0x24, 0xBA ,0xB6, 0x74,0xD6,0xDE,0xA4, 0xFE, 0xF6,0xFC,0x5E, 0xCA, 0x3E,0xDA, 0xD8,0xF8, 0x6E, 0x5A,0x1c,0x18, 0x10,  0},//const unsigned char  SEG_TAB_R[]
    8,//unsigned char dig0_0;
    8,//unsigned char dig0_1;
    8,//unsigned char dig0_2;
    8,//unsigned char dig0_3;
    8,//unsigned char dig0_4;
    8,//unsigned char dig0_5;
    0,//unsigned      dot0_pos;
	
	8,//unsigned char dig1_0;
	8,//unsigned char dig1_1;
	8,//unsigned char dig1_2;
	8,//unsigned char dig1_3;
	8,//unsigned char dig1_4;
    0,//unsigned      dot1_pos;
	
	8,//unsigned char dig2_0;
	8,//unsigned char dig2_1;
	8,//unsigned char dig2_2;
	8,//unsigned char dig2_3;
	8,//unsigned char dig2_4;
	8,//unsigned char dig2_5
	0,//unsigned      dot2_pos;

	8,//unsigned char dig3_0;
	8,//unsigned char dig3_1;
	8,//unsigned char dig3_2;
	8,//unsigned char dig3_3;
	8,//unsigned char dig3_4;
	8,//unsigned char dig3_5;
	8,//char          dot3_pos;
	
	8,//unsigned char dig4_0;
	8,//unsigned char dig4_1;
	8,//unsigned char dig4_2;
	8,//unsigned char dig4_3;
	8,//unsigned char dig4_4;
	8,//unsigned char dig4_5;
	8,//unsigned char dig4_6;
	8,//unsigned char dig4_7;
	8,//unsigned char dig4_8;
	8,//unsigned char dig4_9;
    0,//char          dot4_pos;

    {0},//union dis_oper_mark;
	0,//int cursor_0;//0 line cursor position
	0,//int cursor_1;
	0,//int cursor_2;
	0,//int cursor_3;
	0,//int cursor_4;
	0,//int cursor_0_count;
	0,//int cursor_1_count;
	0,//int cursor_2_count;
	0,//int cursor_3_count;
	0,//int cursor_4_count;
	/*******************end lcd seg define **************************/ 
	
	EM_SELF_TEST_MODE,//mode_type_t   current_mode;
	EM_NO_KEY,        //key_type_t key_type;//[EM_NO_KEY, EM_SHORT_KEY,EM_LONG_KEY]
	MD_KEY_MASK,      //int key;
	get_key,//int  (*const get_key)(struct _HUM_COMPONENTS *);
        hum_comps_task_handle,//void (*const task_handle)(void);
	
	
	//TODO
	
};
















