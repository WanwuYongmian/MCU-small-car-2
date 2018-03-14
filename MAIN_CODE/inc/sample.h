#ifndef _SAMPLE_H
#define _SAMPLE_H
 
#include "common.h"
#include "mma845x.h"
#include "MPU6050.h"
#include "MK60_adc.h"
#include "MK60_adc.h"
#include "sd_use.h"

extern int16 gyro_x_ad,gyro_y_ad,acc_ad,gyro_y_ad_his[3],gyro_y_ad_his_2[10];
extern int16 left_level,left_normol,right_level,right_normol,middle_level,middle_normol;
extern int16 left_level_max,right_level_max,middle_level_max;
extern uint8 level_used_flag,buff_num; //用于电感值滤波
extern int16 left_level_buff[2],right_level_buff[2],middle_level_buff[2];
 void asmple_all_channel(void);
 
 
 
#endif