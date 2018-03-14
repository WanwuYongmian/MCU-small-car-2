
/*
*	�ļ�����: initial
*	�ļ�����: ���ڸ���ģ���ʼ������  �����п���ǰ��������
*
*	��ǰ�汾: 1.1
*	����	:   ������
*	�������:  2016/3/2
*
*	ȡ���汾:
*	ԭ����  :
*	�������:
*/
#include "initial.h"



void PIT0_IRQHandler(void);


unsigned int battery_power;
unsigned int pro_ran_time;

//////////////////////////////flag_system////////////////////////////
////emergency///////////////
int8   down_flag                     //����
      ,lost_line_flag                //����
      ,out_control_flag              //ʧ��
      ,stop_flag;                    //����ͣ��

/////function_choose//////////
int8   gear_flag                     //��λ
      ,gear_flag_high
      ,ramp_on_flag                  //�µ�
      ,SDS_on_flag;                  //����ʾ����

int8 stop_signal_last,stop_signal_now,stop_signal_mag_last,stop_signal_mag_now;


RUN_mode run_mode;
//==============================================================
//������  �� fault_treat
//���������� ���ϴ���
//==============================================================

uint8 fault_number,fault_once;
void fault_treat (void)
{
 
  if(iic_wait_out >= 300 &&  fault_once != 1)
  {
    if(fault_number == 1)
    {
      buzz_mode_flag = 1;
    }
    else if(fault_number == 2)
    {
      buzz_mode_flag = 1;
    }
    else 
    {
     // buzz_mode_flag = 3;
    out_control_flag = 1; 
    left_moto_dir(0); //���
    right_moto_dir(0);
    }
    //initial_all();
  //out_control_flag = 0;
     fault_once = 1;//ֻ����һ��
  }
}

//==============================================================
//������  �� function_choose
//���������� ���뿪�ع���ѡ��
//==============================================================
void function_choose (void)
{

   if(gpio_get(DIP_SWITCH1) == ON_SIDE)
  {
    SDS_on_flag = 1;           
    run_mode = starting_mode; //����
  }
  else
  {
    run_mode = stand_mode;  //վ�Ų���
  }
  
  if((gpio_get(DIP_SWITCH2) == ON_SIDE))
  {
    run_mode = normoallization_mode;    //��һ��
  }
  

  
}

void flag_init(void)
{
    down_flag = 0;                    //����
    lost_line_flag= 0;                //����
    out_control_flag= 0;             //ʧ��
    stop_flag= 0;
    
    //////run_mode////////////
    run_mode = starting_mode;
    
    
    /////function_choose//////////
    gear_flag = 0;                     //��λ
    ramp_on_flag = 0;                  //�µ�
    SDS_on_flag = 0;                  //����ʾ����
}


uint8 oled_flag;


void AD_Init(void)
{
    adc_init(CH_BATTERY);     //���   //PTE25
    adc_init(CH_SIGN_L1);     
    adc_init(CH_SIGN_R1); 
    adc_init(CH_SIGN_MIDDLE);
}




///////////////////sample the voltage of the battery
int16 power_get(void)
{
  uint16 i;
  long he;
  uint16 buff;
  he = 0;
  for(i = 0; i < 1000; i++)
  {
      he += adc_once(CH_BATTERY, ADC_12bit);
  }
  
  buff = he/1000;
  buff = (long) buff * 1277 / 4095;
  
  return (buff);
}





//==============================================================
//������  �� initial_all
//���������� ����ģ���ʼ��
//�����������
//�����������
//����ֵ  �� 0 ��ʾ��������  1��ʾ�쳣 ��ֹ����
//==============================================================
int8 initial_all(void) 
{
  uint32 data32;
  uint8 gear_buff,gear_buff2;
  pit_delay_ms(PIT1,100);
  
  #ifdef USEING_BLUETOOTH_LOWER_COMPUTER    //run these code when enable lower computer
  pit_delay_ms(PIT1,1000); //����������λ��ʱ����ʱ1s
  #endif
  
  ////////////////////////////ʱ�������ʼ��
  time_sequential = 0;
  real_time_ms = 0;
  real_time_ms = 0;
  

  flag_init(); //��־λ��ʼ��
  buzz_initial(); //��ʼ�������� 
  pit_init_us(PIT0,1000);      //PIT�ж�
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
  AD_Init(); //AD��ʼ��
  
  i2c_init(I2C1,400*1000);
  acc_gyro_initial();//���ٶȼ� �����ǳ�ʼ��
  
#ifdef USEING_SD_CARD   //����sd��ʱ�����ô˳���
  SD_lowcomp_init();
#endif
  
  
 
    //////////////////////���뿪��
   port_init(DIP_SWITCH1, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH1, GPI); //���� ���� gpi
   port_init(DIP_SWITCH2, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH2, GPI);
   port_init(DIP_SWITCH3, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH3, GPI);
   port_init(DIP_SWITCH4, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH4, GPI);    
   
   function_choose();           //����ѡ��
  
   
          uart_init(CH_UART,115200); 
#ifdef USEING_BLUETOOTH_LOWER_COMPUTER    //run these code when enable lower computer
      UART_C2_REG(UARTN[CH_UART]) |= (0
                                        //| UART_C2_TE_MASK                     //����ʹ��
                                        | UART_C2_RE_MASK                     //����ʹ��
                                        //| UART_C2_TIE_MASK                  //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                        //| UART_C2_TCIE_MASK                 //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                                        //| UART_C2_RIE_MASK                  //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                       );
  set_vector_handler(VECTOR_UART , lower_computer_handler);
 //         uart_rx_irq_en(CH_UART);      //���������ж�
#endif
                   KEY_and_LCD12864_Iint();//Һ����������ʼ��
     
  
  
  
  
  
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
  
  battery_power = power_get(); //����ص�ѹ
  
  PWM_Init(battery_power);
  
 flash_init();  //��ʼ��flash                                              
  data32 = flash_read(SECTOR_NUM, 0, uint32);  //��ȡ4�ֽ�
  left_level_max = (uint16)(data32 >> 16);
  right_level_max = (uint16)data32; 
   data32 = flash_read(SECTOR_NUM, 4, uint32);  //��ȡ4�ֽ�
   middle_level_max = (uint16)data32; 
  ///////////////////////ͣ����
   port_init(STOP_SIGNAL_PIN, ALT1| PULLUP );gpio_ddr(STOP_SIGNAL_PIN, GPI);
   stop_signal_last = gpio_get(STOP_SIGNAL_PIN);
   ///////////////////////ͣ���ű�
   port_init(STOP_SIGNAL_mag_PIN, ALT1| PULLUP );gpio_ddr(STOP_SIGNAL_mag_PIN, GPI);
   stop_signal_mag_last = gpio_get(STOP_SIGNAL_mag_PIN);
   
  if(battery_power < 760) 
  {
      buzz_mode_flag = 3; //�͵�ѹ����
     return 1;
  }

  
  ////////////////////////���Ʋ�����ʼ��
  
  gear_buff = gpio_get(DIP_SWITCH3);
  gear_buff2 = gpio_get(DIP_SWITCH4);
  
  if(gear_buff == OFF_SIDE && gear_buff2 == OFF_SIDE)
  {
    gear_flag = 3;
    gear_flag_high = 1;
  }
  else if(gear_buff == ON_SIDE && gear_buff2 == OFF_SIDE)
  {
    gear_flag = 1;
  }
  else if(gear_buff == ON_SIDE && gear_buff2 == ON_SIDE)
  {
    gear_flag = 2;
  }
  else if(gear_buff == OFF_SIDE && gear_buff2 == ON_SIDE)
  {
    gear_flag = 3;
    gear_flag_high = 0;
  }
  

  if(gear_flag == 0)
  {
    speed_goal = 170;
  }
  else if(gear_flag == 1)
  {
    speed_goal = 180;
  }
  else if(gear_flag == 2)
  {
    speed_goal = 190;
  }
  else if(gear_flag == 3)
  {
    speed_goal = 200;
  }
  
  
  v_speed = 0;
  v_speed_last = 0;
  v_speed_inc = 0;
  start_race_flag = 1;
  v_turn = 0;
  v_turn_last = 0;
  v_turn_inc = 0;
  
  fault_number = 0;//������
  
  return 0;
}