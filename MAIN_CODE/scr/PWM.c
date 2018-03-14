#include "PWM.h"      /* derivative-specific definitions */


#define uchar unsigned char //宏替换
#define uint unsigned int

#define LEFT_NOTO_FROUNT(num)      ftm_pwm_duty(FTM0, FTM_CH7, num)
#define LEFT_NOTO_BACK(num)        ftm_pwm_duty(FTM0, FTM_CH5, num)      

#define RIGHT_NOTO_FROUNT(num)     ftm_pwm_duty(FTM0, FTM_CH6, num)
#define RIGHT_NOTO_BACK(num)       ftm_pwm_duty(FTM0, FTM_CH4, num)


int16 left_dead,right_dead;
int16 left_out,right_out;
int16 power_cut;

//==============================================================
//函数名  ： PWM_Init
//功能描述： pwm模块初始化
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
void PWM_Init(int16 power)

{
  ftm_pwm_init(FTM0, FTM_CH4, 12*1000, 0); 
  ftm_pwm_init(FTM0, FTM_CH5, 12*1000, 0);
  ftm_pwm_init(FTM0, FTM_CH6, 12*1000, 0);  
  ftm_pwm_init(FTM0, FTM_CH7, 12*1000, 0);
  left_dead = 0;
  right_dead = 0;
  power_cut = (long)760*10000/power;
}
 
 //==============================================================
//函数名  ： left_moto_dir
//功能描述： 左轮驱动
//输入参数：左轮pwm   范围  正负 1000
//输出参数：无
//返回值  ： 
//==============================================================
 
  void left_moto_dir( int16 num) 
  {
 
   if(run_mode == normoallization_mode || run_mode == stand_mode)
   {
     if(num>0)
      num = num+left_dead;
     else if(num<0)
     num = num-left_dead;
   } 

 
   num = (long)num*power_cut/10000; //根据电压衰减占空比  电压越高 衰减越多
   
  // num = num + (long)forward_speed * speed_back/16;
       //数据限幅
   if(num >=950)
     num = 950;
   else if(num <= -950)
     num = -950;

//速度赋值  
    if(num>0)     //正档//                   
   {
      LEFT_NOTO_BACK(0);
      LEFT_NOTO_FROUNT(num);

    }    
    else if(num<0) //负档//
    {
    
      LEFT_NOTO_BACK(-num);
      LEFT_NOTO_FROUNT(0);
   
    } 
    else  //0档//
    {
      LEFT_NOTO_BACK(0); 
      LEFT_NOTO_FROUNT(0);
    
    }  
}

//==============================================================
//函数名  ： rignt_moto_dir
//功能描述： 右轮驱动
//输入参数：右轮pwm   范围  正负 1000
//输出参数：无
//返回值  ： 
//==============================================================
  void right_moto_dir( int16 num) 
  {
   if(run_mode == normoallization_mode || run_mode == stand_mode)
   {
       if(num>0)
        num = num+right_dead;
       else if(num<0)
       num = num-right_dead;
   }
// 
    //数据限幅

   
   num = (long)num*power_cut/10000; //根据电压衰减占空比  电压越高 衰减越多 
   
   
  // num = num + (long)forward_speed * speed_back/16;
     
      if(num >=950)
     num = 950;
   else if(num <= -950)
     num = -950;
   
    if(num>0)     //正档//                   
   {
     RIGHT_NOTO_BACK(0);
     RIGHT_NOTO_FROUNT(num);

    }    
    else if(num<0) //负档//
    {
    
      RIGHT_NOTO_FROUNT(0);
      RIGHT_NOTO_BACK(-num);
   
    } 
    else  //0档//
    {
      RIGHT_NOTO_FROUNT(0); 
      RIGHT_NOTO_BACK(0);
    
    }  
}

