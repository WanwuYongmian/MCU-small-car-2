 #ifndef _TURN_H
#define _TURN_H


#include "common.h"
#include "speed.h"
extern int16 v_turn,v_turn_last,v_turn_inc;
extern int16 turn_kp,turn_kd,turn_kd_signal;

extern int16 weight_sum; 


int16 turn (int16 left,int16 right,int16 gyro,int16 middle);
void normoallize (void);


extern int16 gyro_turn_middle;
extern int16 direction_error; //·½ÏòµÄÆ«²î
extern int16 direction_differential;
extern int16 direction_error_his[30];
extern int16 v_turn_p,v_turn_d,v_turn_d_signal;
extern uint16 signal_sum;
extern int16 ramp_level;
extern int8 ramp_flag;
extern int16 direction_differential_ident;

extern int16 target_turn_gyro,gyro_error,ineer_out,ineer_kp;
#endif