 #ifndef _SPEED_H
#define _SPEED_H


#include "common.h"
#include "acc_gyro.h"
#include "turn.h"
extern int16 v_speed,v_speed_last,v_speed_inc;
extern int16 v_speed_k,v_speed_i; //输出的比例项和积分项


extern int16 right_speed;
extern int16 left_speed;
extern int16 forward_speed,forward_speed_last[2];


extern uint16 speed_kp,speed_ki;
extern int16 speed_goal,speed_goal_give;

extern int16 speed_error;
extern int16 speed_error_integral;
extern uint8 start_race_flag;
extern uint16 out_cut,out_cut_turn;
extern uint16 speed_unstable[25],gyro_unstable[6];
extern  uint16 speed_unstable_sum,gyro_unstable_sum;
extern uint16 speed_out_set;
int16 speed_contral (int16 goal,int16 nowspeed);
void speed_get (void);
#endif