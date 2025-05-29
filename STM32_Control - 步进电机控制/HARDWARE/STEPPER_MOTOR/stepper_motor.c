#include "stepper_motor.h"
#include "motor.h"
#include "LED.h"
#include "math.h"
#include "stdlib.h"

/**************************************************************************
函数功能：电机初始化配置，把PE9,PE11,PE13,PE14初始化为输出口，把TIM1初始化为PWM输出模式，4路通道输出10KHzPWM波
入口参数：无
返回  值：无
**************************************************************************/
void Stepper_Motor_Init(void)
{
	RCC->AHB1ENR |= 1<<4;  // 使能GPIOE
	GPIO_Set(GPIOE, PIN0|PIN1|PIN2|PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // 使能IO口
	GPIO_Set(GPIOE, PIN9|PIN11|PIN13|PIN14, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // PWM输出口
	GPIO_AF_Set(GPIOE, 9, 1);
	GPIO_AF_Set(GPIOE, 11, 1);
	GPIO_AF_Set(GPIOE, 13, 1);
	GPIO_AF_Set(GPIOE, 14, 1);
	
	RCC->APB2ENR |= 1<<0;  // TIM1使能
	TIM1->ARR =	0xFFFF;  // 电机功率越大，PWM频率越低
	TIM1->PSC = Motor_PSC;  // APB2ENR 168M,APB1ENR 84M
	TIM1->CR1 |= 0X01;      // 使能
	TIM1->CR1 |= 1<<7;      // 自动重装载使能
	TIM1->BDTR |= 1<<15;  // 高级定时器输出PWM 主输出使能
	//CH1
	TIM1->CCMR1 |= 6<<4;    //PWM模式1 OC1M[2:0]=110
	TIM1->CCMR1 |= 1<<3;    //OC1PE=1 预装载使能
	TIM1->CCER |= 1<<0;     //CC1E=1 输出使能
	TIM1->CCER |= 1<<1;     //CC1P=0 输出极性高
	//CH2
	TIM1->CCMR1 |= 6<<12;
	TIM1->CCMR1 |= 1<<11;
	TIM1->CCER |= 1<<4;
	TIM1->CCER |= 1<<5;
	//CH3
	TIM1->CCMR2 |= 6<<4;
	TIM1->CCMR2 |= 1<<3;
	TIM1->CCER |= 1<<8;
	TIM1->CCER |= 1<<9;
	//CH4
	TIM1->CCMR2 |= 6<<12;
	TIM1->CCMR2 |= 1<<11;
	TIM1->CCER |= 1<<12;
	TIM1->CCER |= 1<<13;
	
	TIM1->DIER |= 1<<0;	//使能更新中断
	
    MY_NVIC_Init(1 ,1, TIM1_UP_TIM10_IRQn, 3);
}


void Start_TIM1_PWM(u8 ch)
{
	if(ch == 1)
	{
		TIM1->CCER |= (1<<0);   // CH1 CC1E
		TIM1->CCER |= (1<<1);   // CH1 CC1NE
	}
	else if(ch == 2)
	{
		TIM1->CCER |= (1<<4);   // CH2 CC2E
		TIM1->CCER |= (1<<5);   // CH2 CC2NE
	}
	else if(ch == 3)
	{
		TIM1->CCER |= (1<<8);   // CH3 CC3E
		TIM1->CCER |= (1<<9);   // CH3 CC3NE
	}
	else if(ch == 4)
	{
		TIM1->CCER |= (1<<12);  // CH4 CC4E
		TIM1->CCER |= (1<<13);  // CH4 CC4NE
	}
	else //四路全开
	{
		TIM1->CCER |= (1<<0);   // CH1 CC1E
		TIM1->CCER |= (1<<1);   // CH1 CC1NE
		TIM1->CCER |= (1<<4);   // CH2 CC2E
		TIM1->CCER |= (1<<5);   // CH2 CC2NE
		TIM1->CCER |= (1<<8);   // CH3 CC3E
		TIM1->CCER |= (1<<9);   // CH3 CC3NE
	}
}


void Stop_TIM1_PWM(u8 ch)
{
	if(ch == 1)
	{
		TIM1->CCER &= ~(1<<0);  // CH1 CC1E
		TIM1->CCER &= ~(1<<1);  // CH1 CC1NE
	}
	else if(ch == 2)
	{
		TIM1->CCER &= ~(1<<4);  // CH2 CC2E
		TIM1->CCER &= ~(1<<5);  // CH2 CC2NE
	}
	else if(ch == 3)
	{
		TIM1->CCER &= ~(1<<8);  // CH3 CC3E
		TIM1->CCER &= ~(1<<9);  // CH3 CC3NE
	}
	else if(ch == 4)
	{
		TIM1->CCER &= ~(1<<12); // CH4 CC4E
		TIM1->CCER &= ~(1<<13); // CH4 CC4NE
	}
	else
	{
		TIM1->CCER &= ~(1<<0);  // CH1 CC1E
		TIM1->CCER &= ~(1<<1);  // CH1 CC1NE
		TIM1->CCER &= ~(1<<4);  // CH2 CC2E
		TIM1->CCER &= ~(1<<5);  // CH2 CC2NE
		TIM1->CCER &= ~(1<<8);  // CH3 CC3E
		TIM1->CCER &= ~(1<<9);  // CH3 CC3NE
		TIM1->CCER &= ~(1<<12); // CH4 CC4E
		TIM1->CCER &= ~(1<<13); // CH4 CC4NE
	}
}


void TIM_Base_Start_IT(TIM_TypeDef* TIMx)
{
	TIMx->DIER |= TIM_DIER_UIE;   // 使能更新中断
	TIMx->CR1 |= TIM_CR1_CEN;     // 启动定时器计数器
}

void TIM_Base_Stop_IT(TIM_TypeDef* TIMx)
{
	TIMx->DIER &= ~TIM_DIER_UIE;  // 禁用更新中断
    TIMx->CR1 &= ~TIM_CR1_CEN;    // 停止定时器计数器
}


/*****************************************************插补实验*****************************************************/

inter_pol_def g_pol_par= {0}; /* 直线插补参数值 */


__IO st_motor_status_def  g_motor_sta = STATE_STOP;     /* 步进电机运动状态 */
__IO int32_t  sign_dir[2] = {1,1};                      /* 偏差方程的计算公式符号位 */







/*****************************************************直线插补相关函数*****************************************************/

/**
 * @brief       直线增量插补函数实现直线插补功能,两个步进电机分别向X轴和Y轴步进IncX,IncY步
 * @param       IncX    ：终点X轴坐标
 * @param       IncY    ：终点Y轴坐标
 * @param       Speed   ：进给速度
 * @retval      无
 */
void line_incmove(uint32_t IncX,uint32_t IncY,uint32_t Speed)
{
    /* 偏差方程置零 */
    g_pol_par.f_e = 0;

    /* 计算起点到终点坐标对应的脉冲数位置*/
    g_pol_par.end_x = IncX;
    g_pol_par.end_y = IncY;
    g_pol_par.end_pulse = g_pol_par.end_y + g_pol_par.end_x;

    /* 根据终点判断在直线上的进给方向,减少偏差 */
    if(g_pol_par.end_y > g_pol_par.end_x)
    {
        g_pol_par.act_axis = AXIS_Y;                    /* 第一步进给Y轴 */
        g_pol_par.f_e = g_pol_par.f_e + g_pol_par.end_x;
    }
    else
    {
        g_pol_par.act_axis = AXIS_X;                    /* 第一步进给X轴 */
        g_pol_par.f_e = g_pol_par.f_e - g_pol_par.end_y;
    }
    /* 设置通道的比较值 */
    TIM1->CCR1 = Speed;  
    TIM1->CCR2 = Speed;
    TIM1->ARR = Speed *2;  /* ARR设置为比较值2倍，这样输出的波形就是50%的占空比 */

    if(g_pol_par.act_axis == AXIS_X)//使能通道输出
	{
		Start_TIM1_PWM(1);
		Stop_TIM1_PWM(2);
	}
	else //使能通道输出
	{
		Start_TIM1_PWM(2);
		Stop_TIM1_PWM(1);
	}
    TIM_Base_Start_IT(TIM1);                    /* 使能定时器以及开启更新中断 */
    g_motor_sta = STATE_RUN;                    /* 标记电机正在运动 */
}





/**
 * @brief       实现任意象限直线插补
 * @param       coordsX    ：终点X轴坐标
 * @param       coordsY    ：终点Y轴坐标
 * @param       Speed      ：进给速度
 * @retval      无
*/
void line_inpolation( int32_t coordsX,int32_t coordsY, int32_t Speed)
{
    if(g_motor_sta != STATE_STOP)   /* 当前电机正在运转 */
       return ;
    /* 其他象限的直线跟第一象限是一样,只是电机运动方向不一样 */
    g_pol_par.moving_mode = LINE;
    if(coordsX < 0)                 /* 当x轴小于0时，电机方向设为反向*/
    {
        g_pol_par.x_dir = CCW;
        coordsX = -coordsX;         /* 取绝对值 */
        ST_LINE_DIR_X(CCW);
    }
    else
    {
        g_pol_par.x_dir = CW;
        ST_LINE_DIR_X(CW);
    }
    if(coordsY < 0)                 /* 当y轴小于0时，电机方向设为反向*/
    {
        g_pol_par.y_dir = CCW;
        coordsY = -coordsY;         /* 取绝对值 */
        ST_LINE_DIR_Y(CCW);
    }
    else
    {
        g_pol_par.y_dir = CW;
        ST_LINE_DIR_Y(CW);
    }
    line_incmove(coordsX,coordsY,Speed);
}






/*****************************************************圆弧插补相关函数*****************************************************/

/**
 * @brief       设置圆弧插补方向
 * @param       coordsX,coordsY:x,y坐标,dir: 圆弧的插补方向
 * @retval      无
 */
void setarcdir(int32_t coords_x,int32_t coords_y,int8_t dir)
 {
    g_pol_par.inter_dir = dir;
    if(g_pol_par.inter_dir == CCW)
    {
        if(coords_x > 0)                /* 坐标x > 0 起点在X轴的正半轴,顺时针Y轴进给方向是负的 */
        {
            if(coords_y >= 0)           /* 如果y > 0,则是第一象限 */
            {
                g_pol_par.qua_points = FIRST_QUADRANT;
                g_pol_par.x_dir = CCW;
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CW);
            }
            else
            {
                g_pol_par.qua_points = FOURTH_QUADRANT;
                g_pol_par.x_dir = CW;   /* y <= 0, 在第四象限 */
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CW);
                ST_LINE_DIR_Y(CW);
            }
        }
        else if(coords_x < 0)           /* X轴的负半轴 */
        {
            if(coords_y <= 0)           /* y < 0,则是第三象限 */
            {
                g_pol_par.qua_points = THIRD_QUADRANT;
                g_pol_par.x_dir = CW;
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CW);
                ST_LINE_DIR_Y(CCW);
            }
            else                        /* y >= 0,第二象限 */
            {
                g_pol_par.qua_points = SECOND_QUADRANT;
                g_pol_par.x_dir = CCW;
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CCW);
            }
        }
        else if(coords_x == 0)          /* x = 0,在Y轴上的特殊起点 */
        {
            if(coords_y > 0)
            {
                g_pol_par.qua_points = SECOND_QUADRANT;
                g_pol_par.x_dir = CCW;
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CCW);
            }
            else
            {
                g_pol_par.qua_points = FOURTH_QUADRANT;
                g_pol_par.x_dir = CW;
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CW);
                ST_LINE_DIR_Y(CW);
            }
        }
    }
    else /* CW方向 */
    {
        if(coords_x > 0)                /* 坐标x > 0 起点在X轴的正半轴,顺时针Y轴进给方向是负的 */
        {
            if(coords_y >0)             /* 如果y > 0,则是第一象限 */
            {
                g_pol_par.qua_points = FIRST_QUADRANT;
                g_pol_par.x_dir = CW;
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CW);
                ST_LINE_DIR_Y(CCW);
            }
            else
            {
                g_pol_par.qua_points = FOURTH_QUADRANT;
                g_pol_par.x_dir = CCW;  /* y <= 0, 在第四象限 */
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CCW);
            }
        }
        else if(coords_x < 0)           /* X轴的负半轴 */
        {
            if(coords_y < 0)            /* y < 0,则是第三象限 */
            {
                g_pol_par.qua_points = THIRD_QUADRANT;
                g_pol_par.x_dir = CCW;
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CW);
            }
            else                        /* y >= 0,第二象限 */
            {
                g_pol_par.qua_points = SECOND_QUADRANT;
                g_pol_par.x_dir = CW;
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CW);
            }
        }
        else if(coords_x == 0)          /* x = 0,在Y轴上的特殊起点 */
        {
            if(coords_y > 0)
            {
                g_pol_par.qua_points = FIRST_QUADRANT;
                g_pol_par.x_dir = CW;
                g_pol_par.y_dir = CCW;
                sign_dir[AXIS_X] = 1;
                sign_dir[AXIS_Y] = -1;
                ST_LINE_DIR_X(CW);
                ST_LINE_DIR_Y(CCW);
            }
            else
            {
                g_pol_par.qua_points = THIRD_QUADRANT;
                g_pol_par.x_dir = CCW;
                g_pol_par.y_dir = CW;
                sign_dir[AXIS_X] = -1;
                sign_dir[AXIS_Y] = 1;
                ST_LINE_DIR_X(CCW);
                ST_LINE_DIR_Y(CW);
            }
        }
    }
}


/**
 * @brief       在XOY平面内画任意圆弧
 * @param       start_x_point,start_y_point:  分别是起点坐标X,Y
 * @param       stop_x_point,stop_y_point:  分别是终点坐标X,Y
 * @param       speed:  速度值
 * @param       dir:    圆弧的方向
 * @retval      无
 */
void arc_incmove(int32_t start_x_point,int32_t start_y_point,int32_t stop_x_point,int32_t stop_y_point,uint32_t speed,int8_t dir)
{
    if(g_motor_sta == STATE_RUN)                            /*  当前电机正在运动 */
        return ;

    /* 不符合圆的坐标方程 */
    if( (pow(start_x_point,2)+pow(start_y_point,2)) != (pow(stop_x_point,2)+pow(stop_y_point,2)))   /* 需要满足半径一致才是一个圆 */
        return ;
    g_pol_par.moving_mode = ARC;                            /* 圆弧标志 */
    
    g_pol_par.f_e = 0;                                      /* 偏差方程置零 */
    g_pol_par.start_point[AXIS_X] = start_x_point;
    g_pol_par.start_point[AXIS_Y] = start_y_point;
    g_pol_par.end_x = stop_x_point;
    g_pol_par.end_y = stop_y_point;
    /* 设置电机做逆时针圆弧轨迹的运动方向 */
    setarcdir(g_pol_par.start_point[AXIS_X],g_pol_par.start_point[AXIS_Y],dir);                     /* 设置圆弧插补方向 */
    /* 计算总的步数 */  
    g_pol_par.end_pulse = abs((stop_y_point-start_y_point))+ abs((stop_x_point-start_x_point));     /* 从起点到终点的脉冲数 */
    /* 起点坐标x = 0,说明起点位于y轴上,此时往X轴进给误差会减少 */
    if(g_pol_par.start_point[AXIS_X] == 0)
    {
        g_pol_par.act_axis = AXIS_X;                        /* 第一步给X轴 */
        /* 根据圆弧方向决定向X轴进给的偏差方程 */
        g_pol_par.f_e = g_pol_par.f_e + sign_dir[AXIS_X]*g_pol_par.start_point[AXIS_X]*2 + 1;       /* 偏差方程的计算 */
    }
    else
    {
        g_pol_par.act_axis = AXIS_Y;                        /* 第一步给Y轴 */
        g_pol_par.f_e = g_pol_par.f_e + sign_dir[AXIS_Y]*g_pol_par.start_point[AXIS_Y]*2 + 1;       /* 偏差方程的计算 */
    }
    
//    ST_LINE_EN(EN,AXIS_X);
//    ST_LINE_EN(EN,AXIS_Y);
	
     /* 设置通道的比较值 */
    TIM1->CCR1 = speed;  
    TIM1->CCR2 = speed;
    TIM1->ARR = speed *2;  /* ARR设置为比较值2倍，这样输出的波形就是50%的占空比 */

    if(g_pol_par.act_axis == AXIS_X)//使能通道输出
	{
		Start_TIM1_PWM(1);
		Stop_TIM1_PWM(2);
	}
	else //使能通道输出
	{
		Start_TIM1_PWM(2);
		Stop_TIM1_PWM(1);
	}
    TIM_Base_Start_IT(TIM1);                    /* 使能定时器以及开启更新中断 */
    g_motor_sta = STATE_RUN;                    /* 标记电机正在运动 */
}



int Num;
void TIM1_UP_TIM10_IRQHandler(void)
{
	static int Num = 0;
	static uint32_t axis = 0;
	if(TIM1->SR&0X0001)  // 中断标志位
    {
        TIM1->SR&=~(1<<0);  // 清除中断标志位
        /* 插补算法逻辑 */
		
        axis = g_pol_par.act_axis;
		
		/* 判断是否到达终点或者还没开始运动 */
		if(g_pol_par.end_pulse == 0)
			return;
		
		
		
		
		
		
		
		if(g_pol_par.act_axis == AXIS_X)
		{
			if(g_pol_par.x_dir == CCW)
			{
				g_pol_par.start_point[AXIS_X]--;
			}
			else
			{
				g_pol_par.start_point[AXIS_X]++;
			}
		}
		if(g_pol_par.act_axis == AXIS_Y)
		{
			if(g_pol_par.y_dir == CCW)
			{
				g_pol_par.start_point[AXIS_Y]--;
			}
			else
			{
				g_pol_par.start_point[AXIS_Y]++;
			}
		}    
		
		/* 根据进给方向 更新坐标值 */
		if(g_pol_par.moving_mode == ARC)
		{
			/* 根据上一次的偏差判断下一步进给方向,同时计算下一次的偏差 */
			if(g_pol_par.inter_dir == CCW)        /* 插补方向:逆时针圆弧 */
			{
				if(g_pol_par.f_e < 0)             /* 偏差方程 < 0 ,说明当前位置位于圆弧内侧,应向圆外进给 */
				{
					if( (g_pol_par.qua_points == SECOND_QUADRANT) || (g_pol_par.qua_points == FOURTH_QUADRANT) )/*  第二和第四象限,当偏差<0时都是向X轴进给 */
					{
						g_pol_par.act_axis = AXIS_X;
					}
					else    /*  在第一和第三象限,偏差<0,都是向Y轴进给 */
					{
						g_pol_par.act_axis = AXIS_Y;
					}
				}
				else if(g_pol_par.f_e >= 0)       /* 偏差方程 >= 0 ,说明当前位置位于圆弧外侧,应向圆内进给 */
				{
					if( (g_pol_par.qua_points == SECOND_QUADRANT) || (g_pol_par.qua_points == FOURTH_QUADRANT) )
					{
						g_pol_par.act_axis = AXIS_Y;
					}
					else
					{
						g_pol_par.act_axis = AXIS_X;
					}
				}
			}
			else
			{
				if(g_pol_par.f_e < 0)             /* 偏差方程 < 0 ,说明当前位置位于圆弧内侧,应向圆外进给 */
				{
					if( (g_pol_par.qua_points == FIRST_QUADRANT) || (g_pol_par.qua_points == THIRD_QUADRANT) )/*  第一和第三象限,当偏差<0时都是向X轴进给 */
					{
						g_pol_par.act_axis = AXIS_X;
					}
					else    /*  在第二和第四象限,偏差<0,都是向Y轴进给 */
					{
						g_pol_par.act_axis = AXIS_Y;
					}
				}
				else if(g_pol_par.f_e >= 0)       /* 偏差方程 >= 0 ,说明当前位置位于圆弧外侧,应向圆内进给 */
				{
					if( (g_pol_par.qua_points == FIRST_QUADRANT) || (g_pol_par.qua_points == THIRD_QUADRANT) )
					{
						g_pol_par.act_axis = AXIS_Y;
					}
					else
					{
						g_pol_par.act_axis = AXIS_X;
					}
				}
			}
			/* 计算当前坐标与目标曲线的偏差 */
			g_pol_par.f_e = g_pol_par.f_e + 2*sign_dir[g_pol_par.act_axis]*g_pol_par.start_point[g_pol_par.act_axis] + 1;/* 偏差方程的计算 */
		}
		
		
		
		
		
		
		
		/* 根据进给方向 更新坐标值 */
		if(g_pol_par.moving_mode == LINE)
		{
			if(g_pol_par.f_e > 0)                                   /* 偏差方程 > 0 ,说明当前位置位于直线上方,应向X轴进给 */
			{
				g_pol_par.act_axis = AXIS_X;
				g_pol_par.f_e = g_pol_par.f_e - g_pol_par.end_y;    /* 第一象限的X轴进给时,偏差计算 */
			}
			else if(g_pol_par.f_e < 0)                              /* 偏差方程 < 0 ,说明当前位置位于直线下方,应向Y轴进给 */
			{
				g_pol_par.act_axis = AXIS_Y;
				g_pol_par.f_e = g_pol_par.f_e+g_pol_par.end_x;      /* 第一象限的Y轴进给时,偏差计算 */
			}
			/* 偏差为0的时候,判断x,y轴终点的大小决定进给方向 */
			else if(g_pol_par.f_e == 0)                             /* 偏差方程 = 0 ,说明当前位置位于直线,应判断终点坐标再进给 */
			{
				if(g_pol_par.end_y > g_pol_par.end_x)               /* 当Y轴更长的话，应向Y轴进给 */
				{
					g_pol_par.act_axis = AXIS_Y;
					g_pol_par.f_e = g_pol_par.f_e+g_pol_par.end_x;  /* 第一象限的Y轴进给时,偏差计算 */
				}
				else
				{
					g_pol_par.act_axis = AXIS_X;
					g_pol_par.f_e = g_pol_par.f_e - g_pol_par.end_y;
				}
			}
		}
		
		/* 判断是否需要跟换进给轴 */
		if(axis != g_pol_par.act_axis)
		{
			if(g_pol_par.act_axis == AXIS_X)//使能通道输出
			{
				Start_TIM1_PWM(1);
				Stop_TIM1_PWM(2);
			}
			else //使能通道输出
			{
				Start_TIM1_PWM(2);
				Stop_TIM1_PWM(1);
			}
		}
		/* 终点判别:总步长 */
		g_pol_par.end_pulse--;
		if(g_pol_par.end_pulse == 0)
		{
//			TIM1->SR&=~(1<<0);  // 清除中断标志位
			TIM_Base_Stop_IT(TIM1);// 停止定时器,禁用更新中断
			g_motor_sta = STATE_STOP;                   /* 到达终点 */
		}
		
		
		
		
		
		
		
		
        // 保留原有的LED闪烁逻辑
        
        Num++;
        if(Num >= 535)
		{
            LED1 = !LED1;
            Num = 0;
        }
    }

}

