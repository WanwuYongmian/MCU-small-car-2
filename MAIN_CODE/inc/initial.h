#ifndef _INITIAL_H
#define _INITIAL_H

#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全
extern unsigned int battery_power;
extern uint8 oled_flag;
extern unsigned int pro_ran_time;

///////////////////////////////定义时序
extern uint8 time_sequential; //时序
extern uint16 real_time_ms;  //实时时间 单位ms
extern uint16  real_time_s;   //实时时间  单位s
//////////////////////////////flag_system////////////////////////////
////emergency///////////////
extern int8   down_flag                     //倒下
      ,lost_line_flag                //丢线
      ,out_control_flag              //失控
      ,stop_flag;                    //紧急停车

/////function_choose//////////
extern int8   gear_flag                     //档位
             , gear_flag_high
      ,ramp_on_flag                  //坡道
      ,SDS_on_flag;                  //蓝牙示波器
extern int8 stop_signal_last,stop_signal_now,stop_signal_mag_last,stop_signal_mag_now;
extern uint8 fault_number;

typedef enum
{
  go_mode,starting_mode,stand_mode,normoallization_mode  // 运行 开机 归一化
} RUN_mode;

extern RUN_mode run_mode;
//拨码开关设置
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
#define CH_SIGN_R1  ADC0_SE12 //PTB2芯片1
#define CH_SIGN_MIDDLE  ADC0_SE15//PTC1芯片2

#define CH_UART     UART4
#define VECTOR_UART UART4_RX_TX_VECTORn


int8 initial_all(void);
int16 power_get(void);
void function_choose (void);
void fault_treat (void);








#endif