#include "acc_gyro.h"



int16 tg;//ʱ�䳣��
 
int16 gyro_middle_volt;     
int16 gyro_integral;

int16 acc_angle;
int16 gyro_deg;
int16 real_angle;

int16 k_acc,k_gyro;

int16 v_stand; //ֱ���������ѹ

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
/*                                ��ֵ��ʼ��                               */
/*-------------------------------------------------------------------------*/
void acc_gyro_initial (void)
{

      unsigned char i;
      long he = 0;
      int16 buff;
      
      MMA845x_Init();
  //   MPU6050_Init();
      L3G4200_Init();
      
      
      tg = 400;   //3000̫�� 2000̫�� 1000̫��10s  500�е��� 7s  300 5s  
      zero_point.zero_acc=260;
      zero_point.zero_x_gyro=60;
      gyro_middle_volt = 86; 
      gyro_integral = 139; //�����ǻ���ϵ��     //��23��24֮��
      gyro_turn_middle = -40; 
      ang_erro_remd = 0;         //��ʼ����
      
      
       k_acc = 2300;
       k_gyro = 55;
      
   

      down_flag = 0;
      
      for(i = 0; i < 200; i++)     //������������ʼ����
      {
          asmple_all_channel();
      }
      
      
      acc_angle = 0;
      gyro_deg = 0;
      real_angle = 0;

}



//==============================================================
//������  �� acc_standardzation
//���������� ����⵽�ļ��ٶ����ݱ�׼��
//������� acc_raw   Ϊ��Ƭ����⵽��  ���ٶȼƵ�ԭʼ����
//�����������׼���ļ��ٶ�����
//
//==============================================================

#define ACC_RANGE 2   ///// ����2g������д2
//#define ACC_RANGE 4   ///// ����4g��������д4
//#define ACC_RANGE 8   ///// ����8g��������д8
#define ACC_DIGIT  2048  ////12λ�����ݸ�ʽ
//#define ACC_DIGIT 8192  //////14λ���ݸ�ʽ

int16 acc_standardzation (int16 acc_raw) 
{
   int16 acc;
   acc = (long)acc_raw * ACC_RANGE * 8000/ ACC_DIGIT;
   
   return -acc;
}
//==============================================================
//������  �� gyro_standardzation
//���������� ����⵽�Ľ��ٶ����ݱ�׼��
//�������:  gyro_raw  Ϊ��Ƭ����⵽��  �����ǵ�ԭʼ����
//�����������׼���Ľ��ٶ�����
//
//==============================================================
#define GYRO_RANGE 2000////����2000������
//#define GYRO_RANGE 500////����500������
#define  GYRO_DIGIT  32768////16λ�����ݸ�ʽ
int16 gyro_standardzation(int16 gyro_raw)
{
    int16 gyro;
    
   // gyro = (long)gyro_raw*GYRO_RANGE*15/GYRO_DIGIT;
   
    return gyro;
}

/*-------------------------------------------------------------------------*/
/*                             ������������Ʈ                              */
/*-------------------------------------------------------------------------*/
//2msһ�λ���
int16 real_angle_get (int16 acc_ang,int16 real_ang,int16 gyro)
{

    
   
        
     real_ang = real_ang  + (long)gyro*gyro_integral/30000 +  (acc_ang - real_ang + ang_erro_remd) / tg;
     

     /////////////��¼����   ��С��Ϊ��ʵ�ǶȺ������ǽǶ�̫������ĳ������
     ang_erro_remd = (acc_ang - real_ang + ang_erro_remd) % tg;
 
     return real_ang;
}


/*-------------------------------------------------------------------------*/
/*                                  վ��                                   */
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



















