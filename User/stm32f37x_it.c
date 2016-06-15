/**
  ******************************************************************************
  * @file    Project/STM32F37x_StdPeriph_Templates/stm32f37x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x_it.h"
#include "main.h"
#include "nvic_systick.h"
#include "dta0660.h"
#include "matrixkey.h"
#include "timer.h"
#include "ht1621.h"
#include "comp.h"
#include "buzzer_led.h"
#include "userVAinteraction.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//��������2 ��ز�����
uint8_t Rx2Buffer[128]={0};
uint8_t	Rx2Counter=0;
uint8_t	Flag_Rx2End=0;
int8_t  Rx2Reset=0;


//uint32_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
uint32_t ReadValue = 0,ReadValue_before=0, ReadValue1 = 0,ReadValue_before1=0;
uint32_t CaptureNumber = 0, CaptureNumber1 = 0;
uint32_t Capture = 0, Capture1 = 0;

float Frequency[12] = {0}, Frequency1[12] = {0};
float TIM2Freq = 0, TIM5Freq = 0;

uint8_t coe[12]={1,2,3,4,5,6,7,8,9,10,11,12};
uint8_t coe_sum = 1+2+3+4+5+6+7+8+9+10+11+12;
//uint8_t coe[12]={1,1,1,1,1,1,1,1,1,1,1,1};


uint32_t  capture=0,capstats=0,TIM2_cap1=0,TIM2_cap2=0,TIM2_cap3=0,TIM5_cap1=0,TIM5_cap2=0,TIM5_cap3=0;
extern  uint32_t CCR4_Val;//TIM58 ���ڶ�ʱ�������

extern uint8_t VadENA;

u8 dta_temp=0,cs_temp=0;

u8 rdy;
u8 iii,jjj;

uint32_t SDADC1testread=0;
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles DMA2_Channel3 global interrupt request.
	* @brief	TIM��ʱSDADC�ɼ���DMA�����ж�-�õ�������ɱ�־
  * @param  None
  * @retval None
  */
void DMA2_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC3))
	{
		collect_finished = 1;
		TIM_Cmd(TIM19, DISABLE);//�ɼ���һ�����ݣ��رն�ʱ�����޲���Ƶ�ʴ����������������ݺ��ٴ򿪴˶�ʱ��
		DMA_ClearITPendingBit(DMA2_IT_GL3);
//		DMA_Cmd(DMA2_Channel3, DISABLE);  //�ر�DMA2 ��ָʾ��ͨ��
	}
}


/**************************************************************** 
*	��������: 
*	��    ��: 2015-4-28�±��жϷ�������
*						2016-06-08 lea�Ƚ��俪ʼ�󲶻�ڶ����ж�Ϊ��λ��ʼ�ͽ�����־
*						��A���ѹ����100V���ڿ����˱Ƚ��������벶��֮�󣬲���A�ྭ���Ƚ���֮��������أ���ʱ����ʼ������
						�رձȽ�����ʹ�ޱȽ���������޲������롣
*						��ʱ��ͬʱ��Ҫ�ж��ٴμ�⵽�ĵ�ѹҪ����100V���ٿ��������ܣ���¼���ٴβ��񵽵���һ���������ʱ�ļ���ֵ��
*						�������жϡ�
*	��    ��: �� 
*	�� �� ֵ: ��  
*****************************************************************/
void TIM5_IRQHandler(void)
{
	if(phasestatus != state0)//�������״̬�±Ƚ����ڴ��������ʱ��������
	{
		if((TIM5CH4_CAPTURE_STA&0x8000)==0)//��δ�ɹ�����A����B��orC��֮���ʱ��
		{
//			if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
//			{
//				if(TIM5CH4_CAPTURE_STA&0x4000)//�Ѿ�����A�����������
//				{
//					if((TIM5CH4_CAPTURE_STA&0x3FFF)==0x3FFF)//���β���A����B���C���ʱ��̫���ˣ�0x3F=4.194304s��1MHz����0xFFFF�Σ����ۼ�0x3F�Σ�
//					{
//						TIM5CH4_CAPTURE_STA|=0x8000;//��ǳɹ�������һ��
//						TIM5CH4_CAPTURE_VAL=0xFFFF;
//	//					COMP_Cmd(COMP_Selection_COMP1, DISABLE);//�رձȽ���
//					}
//					else TIM5CH4_CAPTURE_STA++;
//				}
//			}
			if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)//����4���������¼�
			{
				if(TIM5CH4_CAPTURE_STA&0x4000)		//�Ѿ�����һ�Σ��ٴβ���һ�������� 		
				{				
					/**************�������ζ�ȡ�жϵڶ�����Ƶ�������������*/
					//20160607lea �������ϵ���ʼ������ӵڶ�����ʼ����Ϊ�մ򿪱Ƚ���ʱ����ʱ�������崦����������������������벶׽�ж�
					//���Դӵڶ������忪ʼ�ж�������������� ��λ��Ĳ���������ȷ��
					if(capstats==0)
					{						
						capstats=1;
					}
					else if(capstats==1)
					{
						TIM2_cap1 = TIM2->CNT;
						TIM5_cap1 = TIM_GetCapture4(TIM5);
						capstats=2;
					}
					else
					{
						TIM5CH4_CAPTURE_VAL = TIM_GetCapture4(TIM5);
						TIM5_cap2 = TIM5CH4_CAPTURE_VAL;
						TIM_Cmd(TIM5, DISABLE);
						
						TIM2_cap2 = TIM2->CNT;//TIM_GetCapture4(TIM2);
						TIM_Cmd(TIM2, DISABLE);
						TIM5CH4_CAPTURE_STA|=0x8000;		//��ǳɹ�����B��C���������
						COMP_Cmd(COMP_Selection_COMP1, DISABLE);//�رձȽ���
		
						TIM_Cmd(TIM14, DISABLE);//0.5s��ʱ�ر�
						
						TIM5_cap3=TIM5_cap2-TIM5_cap1;
						//һ�����ڵļ������� ���ڶ��ܼ�������ȡ�������λ�ȷ���ڶ�������
						CCR4_Val = TIM5_cap3;
						
						TIM2_cap3=TIM2_cap2-TIM2_cap1;
						capstats=0;
						//BUZZER_Open(0);//����һ��						
					}
					/****************/
				}
				else  								//�Ѿ���⵽A���ѹ����100V����ʼ�Ƚ����Ͷ�ʱ����,��һ�β���������
				{
					if(capstats==0)
					{
						capstats=1;
					}
					else
					{
						TIM5CH4_CAPTURE_STA=0;			//��գ��˱�־��������0x4000,0x8000��A��B/C���εı���⣬��������ʱ�������ʱ
						TIM5CH4_CAPTURE_VAL=0;
						TIM_SetCounter(TIM5,0);
						TIM_SetCounter(TIM2,0);
						TIM5CH4_CAPTURE_STA|=0x4000;		//��ǲ�����A�ྭ���Ƚ���֮���������
						
						TIM_Cmd(TIM5, ENABLE);//������ʱ������
						
						TIM_Cmd(TIM2, ENABLE);//���� �ڲ�׽��A�������غ�򿪣����һ������Աȴ������λ��
	 
						COMP_Cmd(COMP_Selection_COMP1, DISABLE);//�رձȽ�����ʹ�ޱȽ���������޲������롣�ȴ��´μ�⵽100V���ϵ�ѹ���ٿ���
						BUZZER_Open(0);//����һ��
						capstats=0;
					}
					
				}
			}
		}
	}
	else//����ģʽ�£��Ƚ������ڹ���㴥�������Ĳ���
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET) 
		{
			/* Clear TIM2 Capture compare interrupt pending bit */
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);			
			
			if(VadENA!=1)
			{
				VadENA=1;
				TIM_Cmd(TIM19, ENABLE);
				//printf("*converting!*\r\n");
			}
				
		}

	}
		

	TIM_ClearITPendingBit(TIM5, TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
}
/**
  * @brief  This function handles TIM2 global interrupt request.
	* @brief	COMP2�Ƚϣ�TIM2���벶���ж�-�õ�����Ƶ�ʲ����
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{ 
		
//	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
//   {
//     TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
//     capture = TIM_GetCapture4(TIM2);
//     TIM_SetCompare4(TIM2, capture + CCR4_Val);
//   }
}



/*DTA0660���ڽ����ж�*/
void USART1_IRQHandler(void)
{
	//����CR3,bit0��EIE: Error interrupt enable, ����USART_IT_ERR,USART_IT_ORE_ER,USART_IT_NE,USART_IT_FE   ����
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{//ͬ  @arg USART_IT_ORE_ER : OverRun Error interrupt if the EIE bit is set  

		dta_temp = USART_ReceiveData(USART1); //ȡ�����ӵ�
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}

	if(USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)
	{//ͬ  @arg USART_IT_NE     : Noise Error interrupt
		USART_ClearFlag(USART1, USART_FLAG_NE);
	}


	if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{//ͬ   @arg USART_IT_FE     : Framing Error interrupt
		USART_ClearFlag(USART1, USART_FLAG_FE);
	}

	if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{//ͬ  @arg USART_IT_PE     : Parity Error interrupt
		USART_ClearFlag(USART1, USART_FLAG_PE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
	  /* Read one byte from the receive data register */
		dta_temp= USART_ReceiveData(USART1)&0xFF;
    RxBuffer[RxCounter++] = dta_temp;

		if(dta_temp==dta_cs)//�ж�һ��У���CS���յ�����dta_temp��ǰ��ĺ���ȣ������ټ��һ��RDY��ѹ��Ϊ���򷵻�dta_receive_flag= 1��Ϊ��������ȴ�У��͵�����
		{
			if(Read_RDY() == RESET)//RDY���ŵ�ѹ�жϣ�DTA0660���յ���Ч�������ὫRDY�ź���Ϊ�ߵ�ƽ����Ч����
			{													//��У�������ά��ԭ��ƽ״̬�������������ݷ�����ϻ�״̬׼����֮��ὫRDY
				dta_receive_flag= 1;		//�ź�������Ϊ�͵�ƽ��
				dta_cs=0;
				RxCounter=0;
			}
			else
				dta_cs+= dta_temp;
		}
		else
			dta_cs+= dta_temp;//ֻҪ�յ����ݣ��ͼ��㵱ǰֵ��֮ǰ�������ĺ͡�
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
//���������жϣ������ڵ���
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {		
    /* Read one byte from the receive data register */
    Rx2Buffer[Rx2Counter++] = USART_ReceiveData(USART2);
		Rx2Reset=200;
		if((Rx2Buffer[Rx2Counter-1]==0x0A)&&(Rx2Buffer[Rx2Counter-2]==0x0D))//һ֡����
		{
			Flag_Rx2End=1;
		}

  }
	  USART_ClearFlag(USART2,USART_FLAG_RXNE);  
}


/*������̰����ж�*/
void EXTI9_5_IRQHandler(void)	//�а����жϣ���EXTI�ж��д򿪶�ʱ���жϣ�ÿ��ʮ��ɨ�谴��
{ 
  EXTI_ClearITPendingBit(Keyboard_EXTI_Row1);//����ⲿ�жϱ�־
	EXTI_ClearITPendingBit(Keyboard_EXTI_Row2);
	EXTI_ClearITPendingBit(Keyboard_EXTI_Row3);
	EXTI_ClearITPendingBit(Keyboard_EXTI_Row4);
	
	EXTI->IMR &= ~Keyboard_EXTI_Line;//�����������ϵ��ж����󣬷�ֹ�ڶ�ʱ��ɨ��������ʱ������ж�
	
	TIM_Cmd(TIM3, ENABLE);//�򿪶�ʱ��3���ڶ�ʱ�������¿�ʼɨ��������
}

/*�������10msɨ���ж�*/
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		/*�����Ƕ�ʱ���жϵ��������ĳ���*/
		
		//ÿ�ΰ���ɨ�趼��ɨһ�Σ����һ��Once����ת���أ��ж��Ƿ���֮ǰ����ת����ֵ��ͬ��
		//����ͬ����ȴ�5��TIM3��ʱʱ�䣬�ٴ�ɨһ����ת���أ�
		if(iii == 0)
			KeyValue = OnceRotaryKey()&0x0F;
		/*��ת���ش�������*/
		if(KeyValue != RotaryKeyValue_before)//��ɨ����ת���ذ���ֵ��֮ǰ��ͬ��Ӧ�����п��ر仯
		{
			iii++;
			if(iii > 5)
			{
				u8 temp=KEY_NULL;
				temp = OnceRotaryKey()&0x0F;
				if(temp == KeyValue)
				{
					if(KeyValue >= KEY_VALUE_6)//0x06->KEY_VALUE_6,KeyValue��0x46,0x47,0x48,0x49,0x4A
					{
						RotaryKeyChanged_flag = 1;
						RotaryKeyValue = (KeyValue&0x0F);//ֻ����KeyValue�еİ���ֵ6-10
						RotaryKeyValue_before = RotaryKeyValue;//RotaryKeyValue_beforeֻ��ǰ��if(KeyValue != RotaryKeyValue_before)�Ƚ��õ���
					}
				}
				iii=0;
				KEY_LINE_write_high_all();
				KeyValue = KEY_NULL;//����ֵ��״̬����
				TIM_Cmd(TIM3, DISABLE);//ͬʱ�رն�ʱ��
				EXTI->IMR |= Keyboard_EXTI_Line;//ͬʱ�����������ϵ��ж�����
			}
		}
		/*����ť�������򣬼����ض�����������*/
		else//��ɨ����ת���ذ���ֵ��֮ǰ��ͬ����������ת���ض�����������������ɨ����������û����������Ҳ��TIM3�رգ����ⲿ�ж�
		{
			jjj++;
			KeyValue = ScanKey();
			/*���ض�����������*/
			if(((KeyValue&0x0F) == KEY_NULL) && jjj > 100+6)//��ʾ��������,100��ʾ����������  ����������ֵⷧΪ100 ��������ĳ�ʱӦ�ô���100��
			{
				jjj=0;
				KEY_LINE_write_high_all();
				KeyValue = KEY_NULL;//����ֵ��״̬����
				TIM_Cmd(TIM3, DISABLE);//ͬʱ�رն�ʱ��
				EXTI->IMR |= Keyboard_EXTI_Line;//ͬʱ�����������ϵ��ж����� 6789
			}
			/*����ť��������*/
			if((KeyValue&0xF0) != 0x00 && (KeyValue&0x0F) < KEY_VALUE_6)//�а���Ҳ��״̬���̰���������������0x0F->KEY_NULL
			{
				jjj=0;
				
				SoftKeyChanged_flag = 1;
				SoftKeyValue_before = SoftKeyValue;
				SoftKeyValue = (KeyValue&0x0F);//ֻ����KeyValue�еİ���ֵ0-5
				
				if((KeyValue&0xF0)==0x90)//0x90=0b10010000
					ShortKey_flag = 1;
				else if((KeyValue&0xF0)==KEY_LONG)
					LongKey_flag = 1;
				
				KEY_LINE_write_high_all();
				KeyValue = KEY_NULL;//����ֵ��״̬����
				TIM_Cmd(TIM3, DISABLE);//ͬʱ�رն�ʱ��
				EXTI->IMR |= Keyboard_EXTI_Line;//ͬʱ�����������ϵ��ж�����
			}
		}
  }
}


/*1s��ʱ*/
void TIM12_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
		powertimer++;
		powertimer_1s_flag=1;
	}
}

/*0.5s��ʱ*/
void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		powertimer_0_5s_flag=1;
	}
}

void TIM13_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
		timer_1s_flag_for_Standby=1;
	}
}


void TIM4_IRQHandler(void)
{
	static u16 buzzer_counter;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		Rx2Reset--;if(Rx2Reset<=0){Rx2Reset=0;Rx2Counter=0;}//������ͨ��ʱ���ý��ռ�����
		if(longshort==0)
		{
			if(buzzer_counter<500)
			{
				BUZZER_Toggle();
				buzzer_counter++;
			}
			else
			{
				BUZZER_Close();
				buzzer_counter=0;
			}
		}
		else if(longshort==1)
		{
			BUZZER_Toggle();
		}
	}
}

/**
  * @brief  This function handles SDADC1 interrupt request.
  * @param  None
  * @retval : None
  */
void SDADC1_IRQHandler(void)
{
  uint32_t ChannelIndex;

  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    /* Get the converted value */
    SDADC1testread = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
  }
}
/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f37x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/