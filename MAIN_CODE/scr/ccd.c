/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��main.c
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��landzo���ӿ�����
 * ��汾       ��
 * Ƕ��ϵͳ     ��

 *               CCD��PIN����   
                           ADC1_SE5a -- PTE1
                           CLK_ClrVal()  PTE5_OUT = 0
                           SI_SetVal()   PTE4_OUT = 1



 * ����         ��Ұ��Ƕ��ʽ����������/landzo ������
 * �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/

#include "include.h"
#include "calculation.h"

void PIT1_IRQHandler(void);
void PIT0_IRQHandler(void);
/*************************
����ϵͳ��ȫ�ֱ���
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
  DisableInterrupts;                             //��ֹ���ж� 
   
  /*********************************************************
  ��ʼ������
  *********************************************************/
   //������Ӵ���
 
   uart_init (UART0 , 115200);                      //��ʼ��UART0�������PTA15�������PTA14������Ƶ�� 115200

   adc_init(ADC1_SE4b) ;
    
   gpio_init (PTA17, GPO,HIGH); 
   gpio_init (PTB17, GPO,LOW);   
   pit_init_ms(PIT0, 5);                                    //��ʼ��PIT0����ʱʱ��Ϊ�� 5ms
   set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
   pit_init(PIT1, 10000);                                   //��ʼ��PIT1����ʱʱ��Ϊ�� 0.2ms 
   set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);
   
   CCD_init1() ;             //CCD��������ʼ��

    
  pixel_pt = Pixel;
  for(i=0; i<128+10; i++) {
    *pixel_pt++ = 0;//(*pixel_pt)++����ȷ˳��
  }
  

   EnableInterrupts;			                    //�����ж�  
   
   /******************************************
    ִ�г���
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
        �������λ��
        **********/
    
      } 

    }

}

  
/*************************************************************************
*                             ����Ƕ��ʽ����������
*
*  �������ƣ�PIT1_IRQHandler
*  ����˵����PIT1��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-9-18    �Ѳ���
*  ��    ע��
*************************************************************************/


void PIT1_IRQHandler(void)
{
   PIT_Flag_Clear(PIT1);       //���жϱ�־λ

  extern unsigned char  IntegrationTime ;             //�ع�ʱ��
  extern void StartIntegration(void);     //�ع⺯��
  static unsigned char TimerCnt20ms = 0;
  unsigned char integration_piont;
  TIME1flag_1ms = 1 ;
   
  TimerCnt20ms++;

   /* �����ع�ʱ�����20ms�����ڵ��ع��*/
  integration_piont = 100 - IntegrationTime; 
  if(integration_piont >= 2) {      /* �ع�ʱ��С��2�򲻽������ع� */
      if(integration_piont == TimerCnt20ms)
     StartIntegration();          ///�ع⿪ʼ
  }


  if(TimerCnt20ms >= 100) {
    TimerCnt20ms = 0;
    TIME1flag_20ms = 1;
  }

   
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PIT0_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��
*************************************************************************/



void PIT3_IRQHandler(void)
{
   PIT_Flag_Clear(PIT0);       //���жϱ�־λ
   
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
