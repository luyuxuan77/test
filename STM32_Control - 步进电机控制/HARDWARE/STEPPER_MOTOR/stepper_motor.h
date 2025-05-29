#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include "sys.h"
#include "delay.h"
#include "motor.h"

extern int Num;

void Stepper_Motor_Init(void);
void Stop_TIM1_PWM(u8 ch);
void TIM_Base_Start_IT(TIM_TypeDef* TIMx);
void TIM_Base_Stop_IT(TIM_TypeDef* TIMx);
void line_inpolation( int32_t coordsX,int32_t coordsY, int32_t Speed);
void line_incmove(uint32_t IncX,uint32_t IncY,uint32_t Speed);
void arc_incmove(int32_t start_x_point,int32_t start_y_point,int32_t stop_x_point,int32_t stop_y_point,uint32_t speed,int8_t dir);
void setarcdir(int32_t coords_x,int32_t coords_y,int8_t dir);




/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_PIN_MASK              0x0000FFFFU /* PIN mask for assert test */


#define TIM_CHANNEL_1                      0x00000000U                          /*!< Capture/compare channel 1 identifier      */
#define TIM_CHANNEL_2                      0x00000004U                          /*!< Capture/compare channel 2 identifier      */
#define TIM_CHANNEL_3                      0x00000008U                          /*!< Capture/compare channel 3 identifier      */
#define TIM_CHANNEL_4                      0x0000000CU                          /*!< Capture/compare channel 4 identifier      */
#define TIM_CHANNEL_ALL                    0x0000003CU                          /*!< Global Capture/compare channel identifier  */

/******************************************************************************************/
/* ֱ�߲岹���� */

#define AXIS_X              0   /* X���� */
#define AXIS_Y              1   /* Y���� */

#define EN                  0   /* ʧ���ѻ��ź� */
#define OFF                 1   /* ʹ���ѻ��ź� */

#define FIRST_QUADRANT      1   /* ��һ���� */
#define SECOND_QUADRANT     2   /* �ڶ����� */
#define THIRD_QUADRANT      3   /* �������� */
#define FOURTH_QUADRANT     4   /* �������� */

#define ARC                 1   /* Բ���岹 */
#define LINE                0   /* ֱ�߲岹 */

#define STEP_DIR_X            PEout(0)   /* X�᷽������ */
#define STEP_DIR_Y            PEout(1)   /* Y�᷽������ */

#define STEP_EN_X            PEout(2)   /* X�᷽������ */
#define STEP_EN_Y            PEout(3)   /* Y�᷽������ */


enum dir
{
    CCW = 0,                    /* ��ʱ����ת */
    CW ,                        /* ˳ʱ����ת */
};  

typedef enum                    /* ���״̬ */
{
    STATE_STOP = 0,
    STATE_RUN = 1,
} st_motor_status_def;

extern  __IO st_motor_status_def  g_motor_sta;



typedef struct {
    uint16_t        pulse_pin;      /* ��ʱ������������� */
    uint32_t        pulse_channel;  /* ��ʱ���������ͨ�� */
    uint16_t        en_pin;         /* ���ʹ�����ű�� */
    uint16_t        dir_pin;        /* ����������ű�� */
    GPIO_TypeDef    *dir_port;      /* ����������Ŷ˿� */
    GPIO_TypeDef    *en_port;       /* ���ʹ�����Ŷ˿� */
} st_motor_ctr_def;

/*  �岹�㷨���Ͷ��� */
typedef struct {
    __IO uint8_t    moving_mode;    /* �˶�ģʽ */
    __IO uint8_t    inter_dir;      /* �岹���� */
    __IO uint8_t    qua_points;     /* ���޵� */
    __IO uint8_t    x_dir;          /* X�᷽�� */
    __IO uint8_t    y_dir;          /* Y�᷽�� */
    __IO int32_t    start_point[2]; /* ������� */
    __IO int32_t    end_x;          /* �յ�����X */
    __IO int32_t    end_y;          /* �յ�����Y */
    __IO uint32_t   end_pulse;      /* �յ�λ���ܵ������� */
    __IO uint32_t   act_axis;       /* ��� */
    __IO int32_t    f_e;            /* �������� */
} inter_pol_def;




/* ------------------------���õ������,����axis:��ǰ���---------------------------- */
#define ST_LINE_DIR_X(x)    GPIOE->ODR = (GPIOE->ODR & ~GPIO_PIN_0) | ((x) ? GPIO_PIN_0 : 0)
#define ST_LINE_DIR_Y(x)    GPIOE->ODR = (GPIOE->ODR & ~GPIO_PIN_1) | ((x) ? GPIO_PIN_1 : 0)


/* -----------------------���õ��ʹ��,����axis:��ǰ��� -----------------------------*/
#define ST_LINE_EN_X(x)     GPIOE->ODR = (GPIOE->ODR & ~GPIO_PIN_2) | ((x) ? GPIO_PIN_2 : 0)

#define ST_LINE_EN_Y(x)     GPIOE->ODR = (GPIOE->ODR & ~GPIO_PIN_7) | ((x) ? GPIO_PIN_7 : 0)
#endif

