/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

void PIT2_IRQHandler(void);

/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       FTM �������� ����
 */
void PTMuse(void)
{
    printf("\n*****FTM �������� ����*****\n");

    ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��

    pit_init_ms(PIT1, 500);                                 //��ʼ��PIT0����ʱʱ��Ϊ�� 1000ms
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
    enable_irq (PIT2_IRQn);                                 //ʹ��PIT0�ж�

    while(1);
}

/*!
 *  @brief      PIT0�жϷ�����
 *  @since      v5.0
 */
void PIT2_IRQHandler(void)
{

    int16 val;
    val = ftm_quad_get(FTM1);          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM1);

    if(val>=0)
    {
        printf("\n��ת��%d",val);
    }
    else
    {
        printf("\n��ת��%d",-val);
    }

    PIT_Flag_Clear(PIT2);       //���жϱ�־λ
}

