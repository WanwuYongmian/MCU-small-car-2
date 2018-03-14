#ifndef _ACC_GYRO_H
#define _ACC_GYRO_H

#include "common.h"
#include "sample.h" 
#include "MK60_adc.h"
#include "mma845x.h"
#include "MPU6050.h"

 struct GESTURE{
  int16 real_angle;
  int16 gyro_x_std;
  int16 gyro_y_std;
  int16 acc_std;
  
};
 struct RAW_ACC_GYRO{
  int16 acc_x_raw;
  int16 acc_y_raw;
  int16 acc_z_raw;
  int16 gyro_x_raw;
  int16 gyro_y_raw;
  int16 gyro_z_raw;
};
extern struct RAW_ACC_GYRO raw_acc_gyro;
extern struct GESTURE gesture;
extern int16 gyro_integral;
extern int16 tg;

extern int16 k_acc,k_gyro;  //stand up loop parameter

extern int16 v_stand; 

extern int16 v_stand,v_stand_last,v_stand_inc;
/*-------------------------------------------------------------------------*/
/*                              º¯ÊýÉùÃ÷                                   */
/*-------------------------------------------------------------------------*/
void acc_gyro_initial (void);
int16 acc_standardzation (int16 acc_raw);
int16 gyro_standardzation(int16 gyro_raw);
int16 real_angle_get (int16 acc_ang,int16 real_ang,int16 gyro);
 int16 stand (int16 real_ang,int16 gyro);
 void fresh_gesture_data( struct RAW_ACC_GYRO *p_str);

#endif