#ifndef __PWM_H_
#define __PWM_H_

#define PWM_PERIODE 1000

//@HackOS: 最大时间为0.8倍的PWM_PERIODE
//@HackOS: 确保有足够的时间来改变设置
#define MAX_CNT (PWM_PERIODE * 8 / 10)

typedef enum
{
    ROLL,
    PITCH,
    YAW,
    NUMAXIS
}tAxis;

extern int MaxCnt[NUMAXIS];
extern int MinCnt[NUMAXIS];
extern int IrqCnt[NUMAXIS];


extern void PWM_Init(void);


#endif

