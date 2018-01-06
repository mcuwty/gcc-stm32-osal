/****************************************************************************************
 * 文件名  ：usart.c
 * 描述    ：串口通信驱动 
 * 开发平台：STM32F103		
 * 库版本  ：ST3.5.0
 * 作者    ：吴泰月
 ***************************************************************************************/
#include "usart.h" 

#define PreemptionPriority 		3 			//串口抢占优先级
#define SubPriority 			3 			//串口子优先级

USART_TypeDef* UsartInstance[3] = {USART1, USART2, USART3};

/****************************************************************************************
 * 函数名  ：Usart_Init
 * 描述    ：Usart初始化
 * 输入    ：Com - 串口编号，定义见头文件，下同
 *			 Baud - 波特率
 *			 StopBit - 停止位
 *  		 Datalen - 数据长度
 *			 Parity - 奇偶校验
 *			 HardControl - 硬件流控制
 * 输出    ：串口输出
 * 返回    ：无
 * 调用    ：外部调用
 ***************************************************************************************/ 
void Usart_Init(uint8_t Com,uint32_t Baud,uint8_t StopBit,
                uint8_t Datalen,uint8_t Parity,uint8_t HardControl)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_InitStructure.USART_BaudRate = Baud;
	switch(StopBit)
	{
		case STOP_1_B:
            USART_InitStructure.USART_StopBits = USART_StopBits_1;
            break;
        case STOP_0_5_B:
        	USART_InitStructure.USART_StopBits = USART_StopBits_0_5;
        	break;
        case STOP_1_5_B:
            USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
            break;
        case STOP_2_B:
            USART_InitStructure.USART_StopBits = USART_StopBits_2;
            break;
        default :                   //默认1个停止位
            USART_InitStructure.USART_StopBits = USART_StopBits_1;
            break;
	}
	switch(Datalen)
    {
        case WROD_LEN_8B:
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            break;
        case WROD_LEN_9B:
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;
            break;
        default :                   //默认字长8字节
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            break;
    }
    switch(Parity)
    {
        case PARITY_EVEN:
            USART_InitStructure.USART_Parity = USART_Parity_Even;
            break;
        case PARITY_ODD:
            USART_InitStructure.USART_Parity = USART_Parity_Odd;
            break;
        case PARITY_NO:
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;
        default:                    //默认无奇偶校验位
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;
    }
    switch(HardControl)
    {
        case HARD_RTS:
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS;
            break;
        case HARD_CTS:
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_CTS;
            break;
        case HARD_RTS_CTS:
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
            break;
        case HARD_NO:
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
            break;
        default :                   //默认无硬件流控制
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
            break;
    }
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发使能

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    switch(Com)
    {
    	case COM1:
    		RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
    		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);

		    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		    GPIO_Init(GPIOA, &GPIO_InitStructure);
		    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		    GPIO_Init(GPIOA, &GPIO_InitStructure);  

			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			USART_Init(UsartInstance[Com], &USART_InitStructure);
		    USART_ITConfig(UsartInstance[Com], USART_IT_RXNE, ENABLE);
		    USART_Cmd(UsartInstance[Com], ENABLE);
    		break;
    	case COM2:
    		RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
    		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);

		    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		    GPIO_Init(GPIOA, &GPIO_InitStructure);
		    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		    GPIO_Init(GPIOA, &GPIO_InitStructure);  

			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			USART_Init(UsartInstance[Com], &USART_InitStructure);
		    USART_ITConfig(UsartInstance[Com], USART_IT_RXNE, ENABLE);
		    USART_Cmd(UsartInstance[Com], ENABLE);
    		break;
    	case COM3:
    		break;
    }
} 

/****************************************************************************************
 * 函数名  ：USART1_IRQHandler
 * 描述    ：串口1中断服务程序
 * 输入    ：无
 * 输出    ：无
 * 返回    ：无
 * 调用    ：内部调用
 ***************************************************************************************/
void USART1_IRQHandler(void)                
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//接收中断
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		//Receive = USART_ReceiveData(USART1);//(USART1->DR);		//读取接收到的数据
        USART_ReceiveData(USART1);//(USART1->DR);		//读取接收到的数据  	        
	}
}
/****************************************************************************************
 * 函数名  ：USART2_IRQHandler
 * 描述    ：串口2中断服务程序
 * 输入    ：无
 * 输出    ：无
 * 返回    ：无
 * 调用    ：内部调用
 ***************************************************************************************/
void USART2_IRQHandler(void)                
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)		//接收中断
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		//Receive = USART_ReceiveData(USART2);//(USART2->DR);		//读取接收到的数据
        USART_ReceiveData(USART2);//(USART2->DR);		//读取接收到的数据  	        
	}
}
/****************************************************************************************
 * 函数名：Usart_transmit
 * 描述  ：Usart发送单字符数据
 * 输入  ：data - 字符数据
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内部调用
 ***************************************************************************************/
void Usart_Transmit(uint8_t Com, unsigned int data)
{
	while(USART_GetFlagStatus(UsartInstance[Com],USART_FLAG_TC)==RESET); 
	USART_SendData(UsartInstance[Com],data);
}
/****************************************************************************************
 * 函数名：Usart_puts
 * 描述  ：Usart发送字符串数据
 * 输入  ：*string - 字符串
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内部调用
 ***************************************************************************************/
void Usart_Puts(uint8_t Com,char *string)
{
	while(*string)
		Usart_Transmit(Com,*string++);
}
/****************************************************************************************
 * 函数名：Usart_printf
 * 描述  ：Usart打印格式字符串数据
 * 输入  ：*format,...- 格式字符串
 * 输出  ：串口输出
 * 返回  ：无
 * 调用  ：内、外部调用
 ***************************************************************************************/
void Usart_Printf(uint8_t Com,char *format, ...)
{
	__va_list arg_ptr;
	char str[100];

	va_start(arg_ptr, format);
	vsprintf(str, format, arg_ptr);
	va_end(arg_ptr);

	Usart_Puts(Com, str);
}
/************************************END OF FILE****************************************/
