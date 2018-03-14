
/*
*	文件名称: initial
*	文件描述: 用于各个模块初始化汇总  并进行开机前各项任务
*
*	当前版本: 1.1
*	作者	:   王敬凯
*	完成日期:  2016/3/2
*
*	取代版本:
*	原作者  :
*	完成日期:
*/
#include "initial.h"



void PIT0_IRQHandler(void);


unsigned int battery_power;
unsigned int pro_ran_time;

//////////////////////////////flag_system////////////////////////////
////emergency///////////////
int8   down_flag                     //倒下
      ,lost_line_flag                //丢线
      ,out_control_flag              //失控
      ,stop_flag;                    //紧急停车

/////function_choose//////////
int8   gear_flag                     //档位
      ,gear_flag_high
      ,ramp_on_flag                  //坡道
      ,SDS_on_flag;                  //蓝牙示波器

int8 stop_signal_last,stop_signal_now,stop_signal_mag_last,stop_signal_mag_now;


RUN_mode run_mode;
//==============================================================
//函数名  ： fault_treat
//功能描述： 故障处理
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
    left_moto_dir(0); //输出
    right_moto_dir(0);
    }
    //initial_all();
  //out_control_flag = 0;
     fault_once = 1;//只运行一次
  }
}

//==============================================================
//函数名  ： function_choose
//功能描述： 拨码开关功能选择
//==============================================================
void function_choose (void)
{

   if(gpio_get(DIP_SWITCH1) == ON_SIDE)
  {
    SDS_on_flag = 1;           
    run_mode = starting_mode; //运行
  }
  else
  {
    run_mode = stand_mode;  //站着不动
  }
  
  if((gpio_get(DIP_SWITCH2) == ON_SIDE))
  {
    run_mode = normoallization_mode;    //归一化
  }
  

  
}

void flag_init(void)
{
    down_flag = 0;                    //倒下
    lost_line_flag= 0;                //丢线
    out_control_flag= 0;             //失控
    stop_flag= 0;
    
    //////run_mode////////////
    run_mode = starting_mode;
    
    
    /////function_choose//////////
    gear_flag = 0;                     //档位
    ramp_on_flag = 0;                  //坡道
    SDS_on_flag = 0;                  //蓝牙示波器
}


uint8 oled_flag;


void AD_Init(void)
{
    adc_init(CH_BATTERY);     //电池   //PTE25
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
//函数名  ： initial_all
//功能描述： 所有模块初始化
//输入参数：无
//输出参数：无
//返回值  ： 0 表示正常启动  1表示异常 禁止启动
//==============================================================
int8 initial_all(void) 
{
  uint32 data32;
  uint8 gear_buff,gear_buff2;
  pit_delay_ms(PIT1,100);
  
  #ifdef USEING_BLUETOOTH_LOWER_COMPUTER    //run these code when enable lower computer
  pit_delay_ms(PIT1,1000); //启用蓝牙上位机时再延时1s
  #endif
  
  ////////////////////////////时序变量初始化
  time_sequential = 0;
  real_time_ms = 0;
  real_time_ms = 0;
  

  flag_init(); //标志位初始化
  buzz_initial(); //初始化蜂鸣器 
  pit_init_us(PIT0,1000);      //PIT中断
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
  AD_Init(); //AD初始化
  
  i2c_init(I2C1,400*1000);
  acc_gyro_initial();//加速度计 陀螺仪初始化
  
#ifdef USEING_SD_CARD   //启用sd卡时才启用此程序
  SD_lowcomp_init();
#endif
  
  
 
    //////////////////////拨码开关
   port_init(DIP_SWITCH1, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH1, GPI); //上拉 输入 gpi
   port_init(DIP_SWITCH2, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH2, GPI);
   port_init(DIP_SWITCH3, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH3, GPI);
   port_init(DIP_SWITCH4, ALT1 | PULLUP );gpio_ddr(DIP_SWITCH4, GPI);    
   
   function_choose();           //功能选择
  
   
          uart_init(CH_UART,115200); 
#ifdef USEING_BLUETOOTH_LOWER_COMPUTER    //run these code when enable lower computer
      UART_C2_REG(UARTN[CH_UART]) |= (0
                                        //| UART_C2_TE_MASK                     //发送使能
                                        | UART_C2_RE_MASK                     //接收使能
                                        //| UART_C2_TIE_MASK                  //发送中断或DMA传输请求使能（注释了表示禁用）
                                        //| UART_C2_TCIE_MASK                 //发送完成中断使能（注释了表示禁用）
                                        //| UART_C2_RIE_MASK                  //接收满中断或DMA传输请求使能（注释了表示禁用）
                                       );
  set_vector_handler(VECTOR_UART , lower_computer_handler);
 //         uart_rx_irq_en(CH_UART);      //蓝牙接收中断
#endif
                   KEY_and_LCD12864_Iint();//液晶屏按键初始化
     
  
  
  
  
  
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
  
  battery_power = power_get(); //检测电池电压
  
  PWM_Init(battery_power);
  
 flash_init();  //初始化flash                                              
  data32 = flash_read(SECTOR_NUM, 0, uint32);  //读取4字节
  left_level_max = (uint16)(data32 >> 16);
  right_level_max = (uint16)data32; 
   data32 = flash_read(SECTOR_NUM, 4, uint32);  //读取4字节
   middle_level_max = (uint16)data32; 
  ///////////////////////停车键
   port_init(STOP_SIGNAL_PIN, ALT1| PULLUP );gpio_ddr(STOP_SIGNAL_PIN, GPI);
   stop_signal_last = gpio_get(STOP_SIGNAL_PIN);
   ///////////////////////停车磁标
   port_init(STOP_SIGNAL_mag_PIN, ALT1| PULLUP );gpio_ddr(STOP_SIGNAL_mag_PIN, GPI);
   stop_signal_mag_last = gpio_get(STOP_SIGNAL_mag_PIN);
   
  if(battery_power < 760) 
  {
      buzz_mode_flag = 3; //低电压报警
     return 1;
  }

  
  ////////////////////////控制参数初始化
  
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
  
  fault_number = 0;//错误处理
  
  return 0;
}