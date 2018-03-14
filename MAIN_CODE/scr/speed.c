#include "speed.h"

int16 v_speed,v_speed_last,v_speed_inc;
int16 v_speed_k,v_speed_i; //����ı�����ͻ�����

int16 right_speed;
int16 left_speed;
int16 forward_speed,forward_speed_last[2];

uint16 speed_kp,speed_ki;
int16 speed_goal,speed_goal_give;

int16 speed_error;
int16 speed_error_integral;

uint8 start_race_flag;
uint16 out_cut,out_cut_turn;
uint16 speed_out_set;
//==============================================================
//������  �� speed
//���������� �ٶȻ��ܺ���
//�����������
//�����������
//����ֵ  ��
//==============================================================
int16 speed_contral (int16 goal,int16 nowspeed)
{
  int16 out;

  
  
  /////////////////////////////////////////////////
  /////////////////////////////////////////
  /////////////////////////////////////
  ///////////////////////////////            һ���ٶ�  170
  
  
  
  if(gear_flag == 0)
  {
                        /////////////////////////////////////////////////////////////////////������
                speed_error = goal - nowspeed;

                 if(goal > 100 && run_mode == go_mode)
                 {
                   speed_out_set = 2400;  
                 }
                 else
                 {
                   speed_out_set = 0;
                 }

                if(start_race_flag == 1 &&nowspeed > 50 && speed_error <= 50)
                {
                  start_race_flag = 0; //�������
                }
                
                if(start_race_flag == 0)
                {
                    speed_kp = 1700;
                       v_speed_k = (long)speed_error * speed_kp/100;
                       if(v_speed_k>400)
                          v_speed_k = 400;
                      else if(v_speed_k<-400)
                         v_speed_k = -400;
                }
                else//////////////////////����
                {
                  if(nowspeed < 0)
                      speed_kp = 500;
                  else if(nowspeed >= 50)
                  {
                    speed_kp = 2000;
                  }
                      
                      
                       v_speed_k = (long)speed_error * speed_kp/100;   
                  if(v_speed_k>1100)
                          v_speed_k = 1100;
                      else if(v_speed_k<-1100)
                         v_speed_k = -1100;
                }
                       

              //////////////////////////////////////////////////////////////////������
              if(abs(speed_error) < 10)
                {
                    speed_ki = 1800;  //150
                 }
              else if(abs(speed_error) < 50)
              {
                 speed_ki = 600;
              }
              else
              {
                      speed_ki = 0;
              }

               if(speed_error < -5  ) ///����ʱѸ�ٻظ�
              {
                speed_ki = 2500;
              }

               v_speed_i = v_speed_i + (long)speed_error * speed_ki/1000;
               

                    if(v_speed_i > 500)/////////////////////////////////////////�����޷�
                      v_speed_i = 500;
                    else if(v_speed_i<-600)
                      v_speed_i = -600;
                 
                ///////////////////////////////////////////////////////////////////////////////    
                    
                   if(ramp_flag == 1||ramp_flag == -1)  //�µ����⴦��
                  {
                    v_speed_k = 0;
                      v_speed_i = 0;
                      
                      if(middle_normol < 1500)
                      {v_speed_k = 400;}
                  }
                
                  
                out = v_speed_k + v_speed_i;

                if(start_race_flag == 0)
                {
                if(out > 1100)
                      out = 1100;
                    else if(out<-1100)
                      out = -1100;
                }
  }
 
  
    /////////////////////////////////////////////////
  /////////////////////////////////////////
  /////////////////////////////////////
  ///////////////////////////////            �����ٶ�  180
  
  if(gear_flag == 1)
  {
                        /////////////////////////////////////////////////////////////////////������
                speed_error = goal - nowspeed;

                 if(goal > 100 && run_mode == go_mode)
                 {
                   speed_out_set = 2700;  
                 }
                 else
                 {
                   speed_out_set = 0;
                 }

                if(start_race_flag == 1 &&nowspeed > 50 && speed_error <= 50)
                {
                  start_race_flag = 0; //�������
                }
                
                if(start_race_flag == 0)
                {
                    speed_kp = 1700;
                       v_speed_k = (long)speed_error * speed_kp/100;
                       if(v_speed_k>400)
                          v_speed_k = 400;
                      else if(v_speed_k<-400)
                         v_speed_k = -400;
                }
                else//////////////////////����
                {
                  if(nowspeed < 0)
                      speed_kp = 500;
                  else if(nowspeed >= 50)
                  {
                    speed_kp = 2000;
                  }
                      
                      
                       v_speed_k = (long)speed_error * speed_kp/100;   
                  if(v_speed_k>1100)
                          v_speed_k = 1100;
                      else if(v_speed_k<-1100)
                         v_speed_k = -1100;
                }
                       

              //////////////////////////////////////////////////////////////////������
              if(abs(speed_error) < 10)
                {
                    speed_ki = 1800;  //150
                 }
              else if(abs(speed_error) < 50)
              {
                 speed_ki = 600;
              }
              else
              {
                      speed_ki = 0;
              }

               if(speed_error < -5  ) ///����ʱѸ�ٻظ�
              {
                speed_ki = 2500;
              }

               v_speed_i = v_speed_i + (long)speed_error * speed_ki/1000;
               

                    if(v_speed_i > 500)/////////////////////////////////////////�����޷�
                      v_speed_i = 500;
                    else if(v_speed_i<-600)
                      v_speed_i = -600;
                 
                ///////////////////////////////////////////////////////////////////////////////    
                    
                   if(ramp_flag == 1||ramp_flag == -1)  //�µ����⴦��
                  {
                    v_speed_k = 0;
                      v_speed_i = 0;
                      
                      if(middle_normol < 1500)
                      {v_speed_k = 400;}
                  }
                
                  
                out = v_speed_k + v_speed_i;

                if(start_race_flag == 0)
                {
                if(out > 1100)
                      out = 1100;
                    else if(out<-1100)
                      out = -1100;
                }
  }
  
  
    /////////////////////////////////////////////////
  /////////////////////////////////////////
  /////////////////////////////////////
  ///////////////////////////////            �����ٶ�  190
  
  
  if(gear_flag == 2)
  {
                        /////////////////////////////////////////////////////////////////////������
                speed_error = goal - nowspeed;

                 if(goal > 100 && run_mode == go_mode)
                 {
                   speed_out_set = 3000;  
                 }
                 else
                 {
                   speed_out_set = 0;
                 }

                if(start_race_flag == 1 &&nowspeed > 50 && speed_error <= 50)
                {
                  start_race_flag = 0; //�������
                }
                
                if(start_race_flag == 0)
                {
                    speed_kp = 1700;
                       v_speed_k = (long)speed_error * speed_kp/100;
                       if(v_speed_k>400)
                          v_speed_k = 400;
                      else if(v_speed_k<-400)
                         v_speed_k = -400;
                }
                else//////////////////////����
                {
                  if(nowspeed < 0)
                      speed_kp = 500;
                  else if(nowspeed >= 50)
                  {
                    speed_kp = 2000;
                  }
                      
                      
                       v_speed_k = (long)speed_error * speed_kp/100;   
                  if(v_speed_k>1100)
                          v_speed_k = 1100;
                      else if(v_speed_k<-1100)
                         v_speed_k = -1100;
                }
                       

              //////////////////////////////////////////////////////////////////������
              if(abs(speed_error) < 10)
                {
                    speed_ki = 1800;  //150
                 }
              else if(abs(speed_error) < 50)
              {
                 speed_ki = 600;
              }
              else
              {
                      speed_ki = 0;
              }

               if(speed_error < -5  ) ///����ʱѸ�ٻظ�
              {
                speed_ki = 2500;
              }

               v_speed_i = v_speed_i + (long)speed_error * speed_ki/1000;
               

                    if(v_speed_i > 500)/////////////////////////////////////////�����޷�
                      v_speed_i = 500;
                    else if(v_speed_i<-600)
                      v_speed_i = -600;
                 
                ///////////////////////////////////////////////////////////////////////////////    
                    
                   if(ramp_flag == 1||ramp_flag == -1)  //�µ����⴦��
                  {
                    v_speed_k = 0;
                      v_speed_i = 0;
                      
                      if(middle_normol < 1500)
                      {v_speed_k = 400;}
                  }
                
                  
                out = v_speed_k + v_speed_i;

                if(start_race_flag == 0)
                {
                if(out > 1100)
                      out = 1100;
                    else if(out<-1100)
                      out = -1100;
                }
  }
  
  
  
  
  
  
    /////////////////////////////////////////////////
  /////////////////////////////////////////
  /////////////////////////////////////
  ///////////////////////////////            �ĵ��ٶ�  200
  
  
  if(gear_flag == 3)
  {
                        /////////////////////////////////////////////////////////////////////������
                speed_error = goal - nowspeed;

                 if(goal > 100 && run_mode == go_mode)
                 {
                   if(gear_flag_high == 0)
                   speed_out_set = 3200; 
                   else if(gear_flag_high == 1)
                   {
                     speed_out_set = 3300;
                   }
                 }
                 else
                 {
                   speed_out_set = 0;
                 }

                if(start_race_flag == 1 &&nowspeed > 50 && speed_error <= 50)
                {
                  start_race_flag = 0; //�������
                }
                
                if(start_race_flag == 0)
                {
                    speed_kp = 1700;
                       v_speed_k = (long)speed_error * speed_kp/100;
                       if(v_speed_k>400)
                          v_speed_k = 400;
                      else if(v_speed_k<-400)
                         v_speed_k = -400;
                }
                else//////////////////////����
                {
                  if(nowspeed < 0)
                      speed_kp = 500;
                  else if(nowspeed >= 50)
                  {
                    speed_kp = 2000;
                  }
                      
                      
                       v_speed_k = (long)speed_error * speed_kp/100;   
                  if(v_speed_k>1100)
                          v_speed_k = 1100;
                      else if(v_speed_k<-1100)
                         v_speed_k = -1100;
                }
                       

              //////////////////////////////////////////////////////////////////������
              if(abs(speed_error) < 10)
                {
                    speed_ki = 1800;  //150
                 }
              else if(abs(speed_error) < 50)
              {
                 speed_ki = 600;
              }
              else
              {
                      speed_ki = 0;
              }

               if(speed_error < -5  ) ///����ʱѸ�ٻظ�
              {
                speed_ki = 2500;
              }

               v_speed_i = v_speed_i + (long)speed_error * speed_ki/1000;
               

                    if(v_speed_i > 500)/////////////////////////////////////////�����޷�
                      v_speed_i = 500;
                    else if(v_speed_i<-600)
                      v_speed_i = -600;
                 
                ///////////////////////////////////////////////////////////////////////////////    
                    
                   if(ramp_flag == 1||ramp_flag == -1)  //�µ����⴦��
                  {
                    v_speed_k = 0;
                      v_speed_i = 0;
                      
                      if(middle_normol < 1500)
                      {v_speed_k = 400;}
                  }
                
                  
                out = v_speed_k + v_speed_i;

                if(start_race_flag == 0)
                {
                if(out > 1100)
                      out = 1100;
                    else if(out<-1100)
                      out = -1100;
                }
  }
  
  
  
  
  
  
  
  
  
  
  //
  
  out = out + speed_out_set;
  out = -out;
      
  return out;
}


void speed_get (void)
{

               right_speed = ftm_quad_get(FTM1);
               left_speed  = -ftm_quad_get(FTM2);

                if(left_speed > 250)
                {
                  left_speed = 250;
                }
                if(right_speed > 250)
                {
                  right_speed = 250;
                }

          
              forward_speed = ((left_speed + right_speed)/2 + forward_speed_last[0] + forward_speed_last[1])/3;
              
              
              forward_speed_last[1] = forward_speed_last[0];
              forward_speed_last[0] = forward_speed;

}