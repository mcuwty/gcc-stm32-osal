#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"

#define COM1 					0
#define COM2 					1
#define COM3 					2
#define COM4 					3
#define COM5 					4

#define STOP_1_B				1
#define STOP_0_5_B				3
#define STOP_1_5_B				4
#define STOP_2_B 				2

#define WROD_LEN_7B				7
#define WROD_LEN_8B				8
#define WROD_LEN_9B				9

#define PARITY_NO				0
#define PARITY_ODD				1
#define PARITY_EVEN				2

#define HARD_NO					1
#define HARD_RTS				2
#define HARD_CTS				3
#define HARD_RTS_CTS			4

void Usart_Init( uint8_t Com,uint32_t Baud,uint8_t StopBit,
                 uint8_t Datalen,uint8_t Parity,uint8_t HardControl);
void Usart_Transmit(uint8_t Com,unsigned int data);
void Usart_Puts(uint8_t Com,char *string);
void Usart_Printf(uint8_t Com,char *format, ...);

#endif
