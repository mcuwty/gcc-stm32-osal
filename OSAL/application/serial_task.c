/****************************************************************************************
 * 文件名  ：serial_task.c
 * 描述    ：系统串口通信任务
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "usart.h"
#include "led.h"

uint8 Serial_TaskID;					//系统串口通信任务ID

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
//串口通信任务初始化
void Serial_Task_Init(uint8 task_id)
{
	Serial_TaskID = task_id;

	//串口配置初始化
	Usart_Init(COM1,115200,STOP_1_B,WROD_LEN_8B,PARITY_NO,HARD_NO);
	LED_Init();
}

//串口通信任务事件处理
uint16 Serial_Task_EventProcess(uint8 task_id,uint16 task_event)
{
	if ( task_event & SYS_EVENT_MSG )   	//判断系统消息事件
  	{
  		osal_sys_msg_t *MSGpkt;    			//定义一个指向接受系统消息结构体的指针
	    //从消息队列获取消息  
	    MSGpkt = (osal_sys_msg_t *)osal_msg_receive( task_id ); 
    
	    while ( MSGpkt )
	    {
	      	switch ( MSGpkt->hdr.event )  	//判断消息事件
	      	{
	          	case OSAL_PRINTF:
	          		break;

	        	default:
	          		break;
	      	}

	      	// Release the memory
	      	osal_msg_deallocate( (uint8 *)MSGpkt );

	      	// Next  获取下一个消息
	      	MSGpkt = (osal_sys_msg_t *)osal_msg_receive( task_id );
	    }

    	// return unprocessed events
    	return (task_event ^ SYS_EVENT_MSG);
  	}
  	
	if(task_event & PRINTF_STR)
	{
		static int dir = 1;

		if(dir)
		{
			dir = 0;
			LED0 = 0;
		}
		else
		{
			dir = 1;
			LED0 = 1;
		}
		
		Usart_Printf(COM1,"Linux GCC STM32F103 printf !\r\n");
	  	
		return task_event ^ PRINTF_STR;
	}

	return 0;
}
