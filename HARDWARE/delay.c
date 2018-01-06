/****************************************************************************************
 *文件名  ：delay.c
 *描述    ：STM32F1微秒延时函数，systick时钟为72MHz
 *实验平台：STM32F1
 ***************************************************************************************/
#include "stm32f10x.h"

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//systick定时器重装值	    	 
	ticks=nus*72; 					//需要延时的节拍数，systick计数频率被设置为72MHz
	told=SysTick->VAL;        		//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;	//时间超过/等于要延迟的时间,则退出.
		}  
	}									    
}
