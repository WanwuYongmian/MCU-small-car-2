#include "sample.h" 



//==============================================================
//函数名  ： sample_all_channel
//功能描述： 将数字电压信号转换为 角度
//输入参数: 无
//输出参数：各个通道ad值
//返回值  ：无
//==============================================================
int16 gyro_x_ad,gyro_y_ad,acc_ad,gyro_y_ad_his[3],gyro_y_ad_his_2[10];
int16 left_level,left_normol,right_level,right_normol,middle_level,middle_normol;
int16 left_level_max,right_level_max,middle_level_max;

int16 left_level_buff[2],right_level_buff[2],middle_level_buff[2];
uint8 level_used_flag,buff_num; //用于电感值滤波
#define RECURRENCE_TIMES 10   //定义递推均值滤波次数


int acc_ad_histor[RECURRENCE_TIMES];

 void asmple_all_channel(void)
 {
    uint8 buff[2];
    int16 i;
//    
    buff[0] = MMA845xReadRegister(OUT_Z_MSB_REG);
    buff[1] = MMA845xReadRegister(OUT_Z_LSB_REG);
    acc_ad = ((buff[0]<<8)+buff[1]);
    acc_ad = acc_ad/16;
    raw_acc_gyro.acc_x_raw=acc_ad;
    
    if (iic_wait_out >= 300 && fault_number == 0)
    {
      fault_number = 1;
    }
    
    if(level_used_flag == 1 || buff_num == 2)
    {
      buff_num = 0;
      level_used_flag = 0;
    }
    left_level_buff[buff_num] = (adc_once(CH_SIGN_L1, ADC_10bit) + adc_once(CH_SIGN_L1, ADC_10bit) + adc_once(CH_SIGN_L1, ADC_10bit) + adc_once(CH_SIGN_L1, ADC_10bit) + adc_once(CH_SIGN_L1, ADC_10bit))/5;

//    
    L3G4200ReadRegister(OUT_X_H,buff[0]);
    L3G4200ReadRegister(OUT_X_L,buff[1]);
    gyro_x_ad = ((buff[0]<<8)+buff[1]);
    raw_acc_gyro.gyro_x_raw=gyro_x_ad;
//    
//    buff[0] = MPU6050ReadRegister(MPU6050_GYRO_XOUT_H);
//    buff[1] = MPU6050ReadRegister(MPU6050_GYRO_XOUT_L);
//    gyro_y_ad = ((buff[0]<<8)+buff[1]);
    
   right_level_buff[buff_num] = (adc_once(CH_SIGN_R1, ADC_10bit) + adc_once(CH_SIGN_R1, ADC_10bit) + adc_once(CH_SIGN_R1, ADC_10bit) + adc_once(CH_SIGN_R1, ADC_10bit) + adc_once(CH_SIGN_R1, ADC_10bit))/5;

   
   middle_level_buff[buff_num] = (adc_once(CH_SIGN_MIDDLE, ADC_10bit) + adc_once(CH_SIGN_MIDDLE, ADC_10bit) + adc_once(CH_SIGN_MIDDLE, ADC_10bit) + adc_once(CH_SIGN_MIDDLE, ADC_10bit) + adc_once(CH_SIGN_MIDDLE, ADC_10bit))/5;
   
   buff_num++;
    L3G4200ReadRegister(OUT_Y_H,buff[0]);
    L3G4200ReadRegister(OUT_Y_L,buff[1]);
    gyro_y_ad_his[2] = gyro_y_ad_his[1];
     gyro_y_ad_his[1] = gyro_y_ad_his[0];
     gyro_y_ad_his[0] = ((buff[0]<<8)+buff[1]) - gyro_turn_middle;
     
     for(i = 9;i > 0;i--)
     {
       gyro_y_ad_his_2[i] = gyro_y_ad_his_2[i - 1];
     }
     
     if((gyro_y_ad_his[0] <= gyro_y_ad_his[1] && gyro_y_ad_his[0] >= gyro_y_ad_his[2]) || (gyro_y_ad_his[0] <= gyro_y_ad_his[2] && gyro_y_ad_his[0] >= gyro_y_ad_his[1]))
     {
        gyro_y_ad_his_2[0] = gyro_y_ad_his[0];
     }
     else if((gyro_y_ad_his[1] <= gyro_y_ad_his[0] && gyro_y_ad_his[1] >= gyro_y_ad_his[2]) || (gyro_y_ad_his[1] <= gyro_y_ad_his[2] && gyro_y_ad_his[1] >= gyro_y_ad_his[0]))
     {
       gyro_y_ad_his_2[0] = gyro_y_ad_his[1];
     }
     else if((gyro_y_ad_his[2] <= gyro_y_ad_his[0] && gyro_y_ad_his[2] >= gyro_y_ad_his[1]) || (gyro_y_ad_his[2] <= gyro_y_ad_his[1] && gyro_y_ad_his[2] >= gyro_y_ad_his[0]))
     {
        gyro_y_ad_his_2[0] = gyro_y_ad_his[2];
     }
     
     gyro_y_ad = (long)(gyro_y_ad_his_2[0] + gyro_y_ad_his_2[1] + gyro_y_ad_his_2[2] +gyro_y_ad_his_2[3] +gyro_y_ad_his_2[4] +gyro_y_ad_his_2[5] +gyro_y_ad_his_2[6] +gyro_y_ad_his_2[7] +gyro_y_ad_his_2[8] +gyro_y_ad_his_2[9])/10;
   // 
//    buff[0] = MPU6050ReadRegister(MPU6050_GYRO_YOUT_H);
//    buff[1] = MPU6050ReadRegister(MPU6050_GYRO_YOUT_L);
//     gyro_x_ad = ((buff[0]<<8)+buff[1]);
    if (iic_wait_out >= 300 && fault_number == 0)
    {
      fault_number = 2;
    }

 }