#ifndef _INITIAL_H
#define _INITIAL_H

#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //������������������ȷ����ȫ
extern unsigned int battery_power;
extern uint8 oled_flag;
extern unsigned int pro_ran_time;

///////////////////////////////����ʱ��
extern uint8 time_sequential; //ʱ��
extern uint16 real_time_ms;  //ʵʱʱ�� ��λms
extern uint16  real_time_s;   //ʵʱʱ��  ��λs
//////////////////////////////flag_system////////////////////////////
////emergency///////////////
extern int8   down_flag                     //����
      ,lost_line_flag                //����
      ,out_control_flag              //ʧ��
      ,stop_flag;                    //����ͣ��

/////function_choose//////////
extern int8   gear_flag                     //��λ
             , gear_flag_high
      ,ramp_on_flag                  //�µ�
      ,SDS_on_flag;                  //����ʾ����
extern int8 stop_signal_last,stop_signal_now,stop_signal_mag_last,stop_signal_mag_now;
extern uint8 fault_number;

typedef enum
{
  go_mode,starting_mode,stand_mode,normoallization_mode  // ���� ���� ��һ��
} RUN_mode;

extern RUN_mode run_mode;
//���뿪������
#define DIP_SWITCH1      PTC4
#define DIP_SWITCH2      PTC6
#define DIP_SWITCH3      PTC7
#define DIP_SWITCH4      PTC8
#define STOP_SIGNAL_PIN  PTE2
#define STOP_SIGNAL_mag_PIN  PTE3

#define ON_SIDE    0
#define OFF_SIDE  1 

#define CH_BATTERY  ADC1_DM1
#define CH_SIGN_L1  ADC0_SE14 //PTC0
#define CH_SIGN_R1  ADC0_SE12 //PTB2оƬ1
#define CH_SIGN_MIDDLE  ADC0_SE15//PTC1оƬ2

#define CH_UART     UART4
#define VECTOR_UART UART4_RX_TX_VECTORn


int8 initial_all(void);
int16 power_get(void);
void function_choose (void);
void fault_treat (void);








#endif