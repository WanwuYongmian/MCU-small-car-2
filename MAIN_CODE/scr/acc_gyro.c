#include "acc_gyro.h"



int16 tg;//时间常数
 
int16 gyro_middle_volt;     
int16 gyro_integral;

int16 acc_angle;
int16 gyro_deg;
int16 real_angle;

int16 k_acc,k_gyro;

int16 v_stand; //直立环输出电压

int16 ang_erro_remd;

int16 v_stand,v_stand_last,v_stand_inc;


struct GESTURE gesture;
static struct {
  
  int16 zero_acc;
  int16 zero_x_gyro;
  int16 zero_y_gyro;
  
  
}zero_point;
 struct RAW_ACC_GYRO raw_acc_gyro;
/*-------------------------------------------------------------------------*/
/*                                数值初始化                               */
/*-------------------------------------------------------------------------*/
void acc_gyro_initial (void)
{

      unsigned char i;
      long he = 0;
      int16 buff;
      
      MMA845x_Init();
  //   MPU6050_Init();
      L3G4200_Init();
      
      
      tg = 400;   //3000太慢 2000太慢 1000太慢10s  500有点慢 7s  300 5s  
      zero_point.zero_acc=260;
      zero_point.zero_x_gyro=60;
      gyro_middle_volt = 86; 
      gyro_integral = 139; //陀螺仪积分系数     //在23到24之间
      gyro_turn_middle = -40; 
      ang_erro_remd = 0;         //初始余数
      
      
       k_acc = 2300;
       k_gyro = 55;
      
   

      down_flag = 0;
      
      for(i = 0; i < 200; i++)     //采样并丢弃初始数据
      {
          asmple_all_channel();
      }
      
      
      acc_angle = 0;
      gyro_deg = 0;
      real_angle = 0;

}



//==============================================================
//函数名  ： acc_standardzation
//功能描述： 将检测到的加速度数据标准化
//输入参数 acc_raw   为单片机监测到的  加速度计的原始数据
//输出参数：标准化的加速度数据
//
//==============================================================

#define ACC_RANGE 2   ///// 正负2g的量程写2
//#define ACC_RANGE 4   ///// 正负4g的量程则写4
//#define ACC_RANGE 8   ///// 正负8g的量程则写8
#define ACC_DIGIT  2048  ////12位的数据格式
//#define ACC_DIGIT 8192  //////14位数据格式

int16 acc_standardzation (int16 acc_raw) 
{
   int16 acc;
   acc = (long)acc_raw * ACC_RANGE * 8000/ ACC_DIGIT;
   
   return -acc;
}
//==============================================================
//函数名  ： gyro_standardzation
//功能描述： 将检测到的角速度数据标准化
//输入参数:  gyro_raw  为单片机检测到的  陀螺仪的原始数据
//输出参数：标准化的角速度数据
//
//==============================================================
#define GYRO_RANGE 2000////正负2000的量程
//#define GYRO_RANGE 500////正负500的量程
#define  GYRO_DIGIT  32768////16位的数据格式
int16 gyro_standardzation(int16 gyro_raw)
{
    int16 gyro;
    
   // gyro = (long)gyro_raw*GYRO_RANGE*15/GYRO_DIGIT;
   
    return gyro;
}

/*-------------------------------------------------------------------------*/
/*                             消除陀螺仪零飘                              */
/*-------------------------------------------------------------------------*/
//2ms一次积分
int16 real_angle_get (int16 acc_ang,int16 real_ang,int16 gyro)
{

    
   
        
     real_ang = real_ang  + (long)gyro*gyro_integral/30000 +  (acc_ang - real_ang + ang_erro_remd) / tg;
     

     /////////////记录余数   减小因为真实角度和陀螺仪角度太近引起的除法误差
     ang_erro_remd = (acc_ang - real_ang + ang_erro_remd) % tg;
 
     return real_ang;
}


/*-------------------------------------------------------------------------*/
/*                                  站立                                   */
/*-------------------------------------------------------------------------*/



 int16 stand (int16 real_ang,int16 gyro)
 {
     int16 pwm;
     
     if(real_ang > 7300 || real_ang < -2100)
      down_flag = 1;
     
     

    
          pwm = (long)k_acc * real_ang/1000 + (long)k_gyro * gyro/1000;          
          
          
//          if (real_ang > 0)
//            pwm = 1;
//          else if(real_ang < 0)
//            pwm = -1;

     
     
     
     
     return pwm;
     

     
 }

void fresh_gesture_data(struct RAW_ACC_GYRO *p_str)
{
   //gesture.acc_std = acc_standardzation(p_str->zero_point.zero_acc);
  //gesture.gyro_x_std = gyro_standardzation(p_str->gyro_x_raw.zero_point.zero_x_gyro);
   gesture.real_angle = real_angle_get(gesture.acc_std,gesture.real_angle,gesture.gyro_x_std);
}



















