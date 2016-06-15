#ifndef __comm_h
#define __comm_h

#include "main.h"
#include "sdadc.h"
#include <math.h>
#include "comp.h"
#include "matrixkey.h"
#include "stmdataprocess.h"
#include "arm_math.h" 
#include "userVAinteraction.h"
#include "timer.h"


/****************************************
//���ڷ������������ַ�

******************************************/
void CMD_analyze_R(char * string);

/*******************************************************************************
* Function Name  : findCMD
* Description    : ���Ҷ�Ӧ��ָ��λ��
* Input          : find CMD.
*                : 
* Output         : s8
* Return         : none
*******************************************************************************/
int8_t findCMD(char *szCommand);

/*******************************************************************************
* Function Name  : findCMD2
* Description    : ���Ҷ�Ӧ��ָ��λ��
* Input          : find CMD2.
*                : 
* Output         : s8
* Return         : none
*******************************************************************************/
int8_t findCMD2(char *szCommand);

/*******************************************************************************
* Function Name  : findCMD2
* Description    : ���Ҷ�Ӧ��ָ��λ��
* Input          : find CMD2.
*                : 
* Output         : s8
* Return         : none
*******************************************************************************/
int8_t findCMD3(char *szCommand);

/*******************************************************************************
* Function Name  : Communicate
* Description    : �������������Ӧ��
* Input          : RxBuffer[]
*                : 
* Output         : 
* Return         : none
*******************************************************************************/
void Communicate(void);

//ͨ�Ŵ������ܺ���
void Communication_Service(void);

#endif