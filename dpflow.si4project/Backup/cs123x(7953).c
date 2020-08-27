#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_adc.h"
#include "r_cg_sau.h"
#include "device.h"
#include "string.h"
#include "cs123x.h"
#define  MD_CS213X_CLK               P4_bit.no2
#define  MD_CS123X_DATA              P5_bit.no3
#define  MD_CS123X_DATA_DIR          PM5_bit.no3

#define  MD_CS2137_CLK               P4_bit.no3
#define  MD_CS1237_DATA              P5_bit.no1
#define  MD_CS1237_DATA_DIR          PM5_bit.no1






typedef unsigned char   uint8;
typedef long            int32;



static void io_dir_in(void)
{
   MD_CS123X_DATA_DIR=1; 
}
static void io_dir_out(void)
{
   MD_CS123X_DATA_DIR=0;   
}
static void clk_high(void)
{
  MD_CS213X_CLK=1;
}
static void clk_low(void)
{
  MD_CS213X_CLK=0;
}
static void data_high(void)
{
  MD_CS123X_DATA=1;
}
static void data_low(void)
{
  MD_CS123X_DATA=0;
}
static uint8 read_io(void)
{
  return (uint8)MD_CS123X_DATA;
}

static void Delay(int n)
{
   n*=1;
   while(n--);
}

static void CS1237_Power_Down(void)
{

    io_dir_in();
	clk_high();
	Delay(2);//must big than 100us
}

static void CS1237_Restart(void)
{
	io_dir_in();
	clk_low();
	Delay(20);
}



static void CS1237_Clock(void)
{
    clk_high();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
   
    clk_low();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
   
}




static uint8 CS1237_ReadReg(int32 *ad)
{
  uint8 i,command = 0x56;
  
  //for(i = 0; i < 27; i++)//clk1-clk27
  //CS1237_Clock();

 // uint8 i;
   int32 addat = 0;
   for(i = 0; i < 24; i++) //
   {
     addat <<= 1;
     CS1237_Clock();
     if(read_io())
     addat ++;
   }
   CS1237_Clock();
   CS1237_Clock();
   CS1237_Clock(); //
  

  
 // io_dir_out();//
  CS1237_Clock();//clk28
  CS1237_Clock();//clk29
  io_dir_out();///////////
  for(i = 0; i < 7; i ++) //clk30-clk36
  {
    if(command & 0x40) //MSB -> LSB
      data_high();
    else
      data_low();
    command <<= 1;
    CS1237_Clock();
  }
  io_dir_in();//
  CS1237_Clock();//clk37
  command = 0;
  for(i = 0; i < 8; i++) //clk38-clk45
  {
    command <<= 1;
    CS1237_Clock();
    if(read_io())
      command++;
  }
  CS1237_Clock();//clk46
  
   if(addat&((long)1<<23))
   {
     addat|=0xff000000;
   }
   addat>>=7;
   *ad = addat;
   
  return command;
}

static void CS1237_WriteReg(int32 *ad ,uint8 adreg)
{
  uint8 i,command = 0x65;
  
 // for(i = 0; i < 27; i++)//clk1-clk27
  //CS1237_Clock();




   int32 addat = 0;
   for(i = 0; i < 24; i++) //
   {
     addat <<= 1;
     CS1237_Clock();
     if(read_io())
     addat ++;
   }
   CS1237_Clock();
   CS1237_Clock();
   CS1237_Clock(); //
  
//io_dir_out();//
  CS1237_Clock();//clk28
  CS1237_Clock();//clk29
  io_dir_out();//
  for(i = 0; i < 7; i ++) //clk30-clk36
  {
    if(command & 0x40) //MSB -> LSB
      data_high();
    else
      data_low();
    command <<= 1;
    CS1237_Clock();
  }
  CS1237_Clock();//clk37
  
  for(i = 0; i < 8; i++) //clk38-clk45
  {
    if(adreg & 0x80) //MSB -> LSB
      data_high();
    else
      data_low();
    adreg <<= 1;
    CS1237_Clock();
  }
  io_dir_in();//
  CS1237_Clock();//clk46
  
   if(addat&((long)1<<23))
   {
     addat|=0xff000000;
   }
   addat>>=7;
   *ad = addat;
}


static int CS1237_ReadAD(int32 *ad)
{
  uint8 i;
  int32 addat = 0;
  for(i = 0; i < 24; i++) //
  {
    addat <<= 1;
    CS1237_Clock();
    if(read_io())
    addat ++;
  }
  CS1237_Clock();
  CS1237_Clock();
  CS1237_Clock(); //
  if(addat&((long)1<<23))
  {
    addat|=0xff000000;
  }
  addat>>=7;
  *ad = addat;
  return 1;
}



static int CS1237_Init(unsigned char dat)
{
      long ad;
      unsigned char reg;
      CS1237_Restart();
      while(MD_CS123X_DATA);
      CS1237_WriteReg(&ad,dat);        //
      NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
      
      while(MD_CS123X_DATA);
      reg=CS1237_ReadReg(&ad);
      if(dat!=reg)
      {
        return 1;
      }
      else
      {
        return 0;
      }
}





cs123x_comps_t cs1237_comps=
{
    "",
	&cs1237_comps,
	CS1237_Power_Down,//void (*const power_down)(void); //
	CS1237_Restart,//void (*const restart)(void);// 
	CS1237_Init,//int (*const Init )(unsigned char dat);     
	CS1237_ReadAD,//int (*const read_adc)(long *const p);   // 
	CS1237_ReadReg, //unsigned char (*const read_reg )(int32 *ad);        
	CS1237_WriteReg//	void (*const write_reg )(long * ad,unsigned char dat);  
};






cs123x_comps_t cs123x_comps=
{
    "",
	&cs123x_comps,
	CS123x_Power_Down,//void (*const power_down)(void); //
	CS123x_Restart,//void (*const restart)(void);// 
	CS123x_Init,//int (*const Init )(unsigned char dat);     
	CS123x_ReadAD,//int (*const read_adc)(long *const p);   // 
	CS123x_ReadReg, //unsigned char (*const read_reg )(int32 *ad);        
	CS123x_WriteReg//	void (*const write_reg )(long * ad,unsigned char dat);  
};


