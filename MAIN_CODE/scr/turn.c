#include "turn.h"


int16 turn_out_control (int16 gyro);
void center_line (int16 left,int16 right);
void weight_count (void);
void signal_deal (int16 left,int16 right);
void fuzzy_parameter(void);
void ramp_ident(int16 angle, int16 signal,int16 dericetion);
int16 turn_inner_loop (void);
int16 gyro_turn_middle;
int16 v_turn,v_turn_last,v_turn_inc;
int16 turn_kp,turn_kd,turn_kd_signal;
uint16 signal_sum;
int16 v_turn_p,v_turn_d,v_turn_d_signal;
int16 target_turn_gyro;

//==============================================================
//函数名  ： turn
//功能描述： 方向环总处理
//输入参数： 左右信号
//输出参数：无
//返回值  ： 
//==============================================================
int16 turn (int16 left,int16 right,int16 gyro,int16 middle)
{
  int16 out;
  
  signal_deal(left,right);
  weight_count();
  center_line (left,right);
  ramp_ident(0,middle,direction_error);
  out = turn_out_control(gyro);
  turn_inner_loop();
  return out;
}

//==============================================================
//函数名  ： turn_inner_loop
//功能描述： 方向环总处理
//输入参数： 左右信号
//输出参数：无
//返回值  ： 
//==============================================================
int16 gyro_error,ineer_out,ineer_kp;
int16 turn_inner_loop (void)
{
  ineer_kp = 3;
  gyro_error = target_turn_gyro - gyro_y_ad/30;
  ineer_out = ineer_kp * gyro_error;
  return ineer_out;
}
//==============================================================
//函数名  ： signal_deal
//功能描述： 信号预处理
//输入参数：左右信号
//输出参数：无
//返回值  ： 
//==============================================================
void signal_deal (int16 left,int16 right)
{
  signal_sum = left + right;
}

//==============================================================
//函数名  ： weight_count
//功能描述： 权重计算
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
int16 weight_sum; //左右信号差值的权重
void weight_count (void)
{
        if (signal_sum >= 1300)
      {
          weight_sum = 0;
      }
      else if (signal_sum > 900)
      {
          weight_sum = (int16)((1300 - signal_sum) / 4);
      }
      else
      {
          weight_sum = 100;
      }

      if (abs(direction_error) < 200)
      {
          weight_sum = 0;
      }
      else if (abs(direction_error) < 400)
      {
          weight_sum = (int16)((long)weight_sum * (abs(direction_error) - 200) / 200);
      }
      else
      {
         
      }
  
}
//==============================================================
//函数名  ： center_line
//功能描述： 方向环寻线
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
int16 direction_error; //方向的偏差
int16 direction_error_his[30];
int16 direction_differential;
void center_line (int16 left,int16 right)
{
  int16 bas_error = 0;
  int16 i = 0;
  
  if(ramp_flag == 0 )
  {
    bas_error = ((long)1000 * (left - right) / (left + right));
  }
  else if(ramp_flag == 1||ramp_flag == -1)
  {
    if(middle_normol < 500)
    {
      bas_error = (left - right);
    }
    else 
    {
      bas_error = 0;
    }
    weight_sum = 0;
  }
  
  weight_sum = 0;
  
      if (direction_error == 0)
    {
        direction_error = (int16)(bas_error);
    }
    else if (direction_error < 0)
    {
        direction_error = (int16)((long)(bas_error) * (100 - weight_sum) / 100 - (long)(900 - left) * weight_sum / 100);
    }
    else if (direction_error > 0)
    {
        direction_error = (int16)((long)(bas_error) * (100 - weight_sum) / 100 + (long)(900 - right) * weight_sum / 100);
    }
    
    if(gear_flag == 3) //4档限制最大偏差
    {
      if(direction_error>700)
      {
        direction_error = 700;
      }
      else if(direction_error<-700)
      {
        direction_error = -700;
      }
    }
    else if(gear_flag == 2) //3档限制最大偏差
    {
      if(direction_error>600)
      {
        direction_error = 600;
      }
      else if(direction_error<-600)
      {
        direction_error = -600;
      }
    }
    
    
    for(i = 29; i > 0;i--)
    {
      direction_error_his[i] = direction_error_his[i-1];
    }
    direction_error_his[0] = direction_error;
    
    direction_differential = direction_error_his[0] - direction_error_his[15];
    direction_differential_ident = direction_error_his[0] - direction_error_his[20];
}

//==============================================================
//函数名  ： turn_out_control
//功能描述： 方向环输出量
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
int16 turn_out_control (int16 gyro)
{
  int16 out;
  

  fuzzy_parameter();
  v_turn_p = (long)direction_error * turn_kp/1000;
  v_turn_d = -(long)(gyro)*turn_kd/1000;
  v_turn_d_signal = (long)direction_differential*turn_kd_signal/1000;
  
  out = v_turn_p + v_turn_d + v_turn_d_signal;
    target_turn_gyro = v_turn_p + v_turn_d_signal ;
  
    if(out > 1000)
    {out = 1000;}
    else if(out < -1000)
    {
      out = -1000;
    }
  return out;
}
//==============================================================
//函数名  ： fuzzy_parameter
//功能描述： 模糊化各个参数
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
int16 direction_differential_ident=0;
int16 weight_direction_ident;
void fuzzy_parameter (void)
{
  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////一档
  
  if(gear_flag == 0)
  {
                 ////////////////////////////////////比例项 
        if(abs(direction_error) < 300)
        {

          turn_kp = 1400 + (long)direction_error*direction_error/150;
        }
        else
        {
          turn_kp = 2000;
        }
        
        if(forward_speed> 100)
        {
        }
        else if(forward_speed <= 100&&forward_speed>30)
        {
          turn_kp = (long)turn_kp * ((long)(forward_speed-30)*5/7+50)/100;
        }
        else
        {
          turn_kp = (long)turn_kp*50/100;
         }  
         
        /////////////////////////////////////////////////////////////////阻尼项  
        turn_kd = 55;


        
       ///////////////////////////////////////////////////////////////////////////微分项 
        
        turn_kd_signal = 200;
  }
  
  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
  //////////////////////////////////////二挡
  if(gear_flag == 1)
  {
                 ////////////////////////////////////比例项 
        if(abs(direction_error) < 300)
        {

          turn_kp = 1400 + (long)direction_error*direction_error/150;
        }
        else
        {
          turn_kp = 2000;
        }
        
        if(forward_speed> 100)
        {
        }
        else if(forward_speed <= 100&&forward_speed>30)
        {
          turn_kp = (long)turn_kp * ((long)(forward_speed-30)*5/7+50)/100;
        }
        else
        {
          turn_kp = (long)turn_kp*50/100;
         }  
         
        /////////////////////////////////////////////////////////////////阻尼项  
        turn_kd = 55;


        
       ///////////////////////////////////////////////////////////////////////////微分项 
        
        turn_kd_signal = 200;
  }
    //////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
  //////////////////////////////////////三挡
  if(gear_flag == 2)
  {
                 ////////////////////////////////////比例项 
        if(abs(direction_error) < 300)
        {

          turn_kp = 1400 + (long)direction_error*direction_error/90;
        }
        else
        {
          turn_kp = 2400;
        }
        
        if(forward_speed > 150)
        {
        }
        else if(forward_speed <= 150&&forward_speed>80)
        {
          turn_kp = (long)turn_kp * ((long)(forward_speed-80)*5/7+50)/100;
        }
        else
        {
          turn_kp = (long)turn_kp*50/100;
         }  
         
        /////////////////////////////////////////////////////////////////阻尼项 
        if(abs(direction_error) < 200)
        {
          turn_kd = 70 - (abs(direction_error))*3/20;
        }
        else if(abs(direction_error) < 700)
        {
          turn_kd = 40 + (abs(direction_error)-200)/20;
        }
        else
        {
          turn_kd = 65;
        }


        
       ///////////////////////////////////////////////////////////////////////////微分项 
        
        turn_kd_signal = 200;
  }
    //////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
  //////////////////////////////////////四挡
  
  if(gear_flag == 3)
  {
                 ////////////////////////////////////比例项 
        if(abs(direction_error) <212)
        {

          turn_kp = 1400 + (long)direction_error*direction_error/75;
        }
        else
        {
          turn_kp = 1999;
        }
        
        if(forward_speed> 180)
        {
        }
        else if(forward_speed <= 180&&forward_speed>110)
        {
          turn_kp = (long)turn_kp * ((long)(forward_speed-110)*5/7+50)/100;
        }
        else
        {
          turn_kp = (long)turn_kp*50/100;
         }  
         
        /////////////////////////////////////////////////////////////////阻尼项  
         if(abs(direction_error) < 200)
        {
          turn_kd = 75 - (abs(direction_error))/8;
        }
        else if(abs(direction_error) < 700)
        {
          turn_kd = 50 + (abs(direction_error)-200)/33;
        }
        else
        {
          turn_kd = 65;
        }


        
       ///////////////////////////////////////////////////////////////////////////微分项 
        
        turn_kd_signal = 200;
  }
  
  
  
  
  
  if(ramp_flag != 0)
  {
    turn_kd_signal = 0;
  }
    
}

//==============================================================
//函数名  ： ramp_ident
//功能描述： 坡道识别
//输入参数：无
//输出参数：无
//返回值  ： 
//==============================================================
int16 ramp_level;
int8 ramp_flag = 0;
//int16 direction_differential_ident;
void ramp_ident(int16 angle, int16 signal,int16 dericetion)
{
  
  
    ramp_level = (int16)(1000 - (long)(2700 - signal) * 4 / 7);

    ramp_level = (int16)(ramp_level - (long)angle * 23 / 50);

    
    
    
    
    
    if (abs(dericetion) > 100 && abs(dericetion) < 400)
    {
        ramp_level = (int16)((long)ramp_level * (400 - abs(dericetion)) / 3 / 100);
    }
    else if(abs(dericetion) >= 400)
    {
        ramp_level = 0;
    }
    
    
    
    
    
    
    
    if(ramp_flag == 0 && ramp_level > 500)
    {
    //  ramp_flag = 1;
 //     buzz_mode_flag = 1;
    }
    else if(ramp_flag == 1 && ramp_level < -300)
    {
      ramp_flag = -1;
    }
    else if(ramp_flag == -1 && ramp_level > -50)
    {
      ramp_flag = 0;
  //    buzz_mode_flag = 1;
    }
    
    
    
}

//归一化程序
void normoallize (void)
{
      uint32  data32,data32_2;  //用于合并数据
      RUN_mode run_mode_now,run_mode_last; //用于模式跳变沿识别
      left_level_max = 0;
      right_level_max = 0;
      middle_level_max = 0;
     for(;;)
     {
      
       if(left_level > left_level_max)
       {
         left_level_max = left_level; //找出最大值
       }
       if(right_level > right_level_max)
       {
         right_level_max = right_level;
       }
       if(middle_level > middle_level_max)
       {
         middle_level_max = middle_level;
       }
       
       function_choose();
       
       run_mode_last = run_mode_now;
       run_mode_now = run_mode;
       
       if(run_mode_last == normoallization_mode && run_mode_now == starting_mode) //存入flash
       {
         flash_erase_sector(SECTOR_NUM);                     //擦除扇区
         data32 = ((uint32)left_level_max)<<16;
         data32 +=  (uint32)right_level_max;
         data32_2 = (uint32)middle_level_max;
            if( 1 == flash_write(SECTOR_NUM, 0, data32) && 1 == flash_write(SECTOR_NUM, 4, data32_2))   //写入数据到扇区，偏移地址为0，必须一次写入4字节
            //if是用来检测是否写入成功
            {
                buzz_mode_flag = 1;
               
            }
       }                
       
       
       if(SDS_on_flag==1)
      {
#ifndef USEING_BLUETOOTH_LOWER_COMPUTER    //run these code when enable lower computer
      WirelessDateSend();
#endif
      }
      else
      {
        display_menu();
      }
            buzz_run();                  //蜂鸣器系统
     }
}
