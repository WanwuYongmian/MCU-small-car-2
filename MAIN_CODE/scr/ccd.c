/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：main.c
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：

 *               CCD的PIN定义   
                           ADC1_SE5a -- PTE1
                           CLK_ClrVal()  PTE5_OUT = 0
                           SI_SetVal()   PTE4_OUT = 1



 * 作者         ：野火嵌入式开发工作室/landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/

#include "include.h"
#include "calculation.h"

void PIT1_IRQHandler(void);
void PIT0_IRQHandler(void);
/*************************
设置系统的全局变量
*************************/



unsigned  char TIME0flag_5ms =0  ;
unsigned  char TIME0flag_10ms=0  ;
unsigned  char TIME0flag_15ms=0  ;
unsigned  char TIME0flag_20ms=0  ;
unsigned  char TIME0flag_80ms;
unsigned  char TIME1flag_1ms=0 ;
unsigned  char TIME1flag_20ms =0;
unsigned  char TimerFlag20ms=0;
unsigned  char TimeCount = 0 ;
unsigned 	char AtemP ;
unsigned 	char Pixel[128];






void CCDuse()
{
   volatile    unsigned char i;
   unsigned 	char send_data_cnt = 0;
   unsigned 	char *pixel_pt;    
  DisableInterrupts;                             //禁止总中断 
   
  /*********************************************************
  初始化程序
  *********************************************************/
   //自行添加代码
 
   uart_init (UART0 , 115200);                      //初始化UART0，输出脚PTA15，输入脚PTA14，串口频率 115200

   adc_init(ADC1_SE4b) ;
    
   gpio_init (PTA17, GPO,HIGH); 
   gpio_init (PTB17, GPO,LOW);   
   pit_init_ms(PIT0, 5);                                    //初始化PIT0，定时时间为： 5ms
   set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
   pit_init(PIT1, 10000);                                   //初始化PIT1，定时时间为： 0.2ms 
   set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);
   
   CCD_init1() ;             //CCD传感器初始化

    
  pixel_pt = Pixel;
  for(i=0; i<128+10; i++) {
    *pixel_pt++ = 0;//(*pixel_pt)++是正确顺序
  }
  

   EnableInterrupts;			                    //开总中断  
   
   /******************************************
    执行程序
    ******************************************/
    while(1)
    {
      if(TIME1flag_1ms == 1)
      {
        TIME1flag_1ms = 0 ;
        
      }
      
      
      if(TIME1flag_20ms == 1)
      {
        TIME1flag_20ms = 0 ; 
        /* Sampling CCD data */
        ImageCapture(Pixel);
        /* Calculate Integration Time */
        CalculateIntegrationTime();
        /* Send data to CCDView every 100ms */
        if(++send_data_cnt >= 5) {
          send_data_cnt = 0;
          SendImageData(Pixel);
        }
        
        /**********
        计算黑线位置
        **********/
    
      } 

    }

}

  
/*************************************************************************
*                             蓝宙嵌入式开发工作室
*
*  函数名称：PIT1_IRQHandler
*  功能说明：PIT1定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-9-18    已测试
*  备    注：
*************************************************************************/


void PIT1_IRQHandler(void)
{
   PIT_Flag_Clear(PIT1);       //清中断标志位

  extern unsigned char  IntegrationTime ;             //曝光时间
  extern void StartIntegration(void);     //曝光函数
  static unsigned char TimerCnt20ms = 0;
  unsigned char integration_piont;
  TIME1flag_1ms = 1 ;
   
  TimerCnt20ms++;

   /* 根据曝光时间计算20ms周期内的曝光点*/
  integration_piont = 100 - IntegrationTime; 
  if(integration_piont >= 2) {      /* 曝光时间小于2则不进行再曝光 */
      if(integration_piont == TimerCnt20ms)
     StartIntegration();          ///曝光开始
  }


  if(TimerCnt20ms >= 100) {
    TimerCnt20ms = 0;
    TIME1flag_20ms = 1;
  }

   
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PIT0_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：
*************************************************************************/



void PIT3_IRQHandler(void)
{
   PIT_Flag_Clear(PIT0);       //清中断标志位
   
   TimeCount ++ ;

  if(TimeCount%1 == 0 ){
     TIME0flag_5ms = 1;

 } 
  if(TimeCount%2 == 0 ){
     TIME0flag_10ms = 1;

  } 
  if(TimeCount%3 == 0 ){
     TIME0flag_15ms = 1;
  }
  if(TimeCount%4 == 0 ){
     TIME0flag_20ms = 1;
  }
  if(TimeCount%64 == 0 ){
     TIME0flag_80ms = 1;
  }
  
  if(TimeCount == 192)
  {
    TimeCount = 0 ;
  }

   
}
