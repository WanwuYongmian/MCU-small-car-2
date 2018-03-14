 #ifndef _PWM_H
#define _PWM_H


#include "common.h"
#include  "MK60_FTM.h"
#include "initial.h"
#include "speed.h"

extern int16 left_out,right_out;
extern int16 left_dead,right_dead; 

void PWM_Init(int16 power);
void left_moto_dir(int16 num) ;
void right_moto_dir(int16 num) ;

#endif