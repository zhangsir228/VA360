#ifndef __STMDATAPROCESS_H
#define __STMDATAPROCESS_H
#include "stm32f37x.h"



extern uint16_t datasize;


/*sdadc.c������Ϣ*/
extern float SDADC1_value[];
extern float SDADC2_value[];
extern float POWER_value[];
/*���ʵ���������*/
extern float maxv_value,maxi_value,minv_value,mini_value;//�����С|��ѹ������
extern float voltage_effective,current_effective;//��Ч��ѹ����Ч����
extern float inrush_current_effective_100ms;//100ms��ӿ����
extern float voltage_mean,current_mean;//ƽ����ѹ��ƽ������
extern float apparent_power,active_power,reactive_power;//���ڹ��ʡ��й����ʡ��޹�����
extern float kWh,kVAh,kVarh,KgCO2;
extern float voltage_foudamental_effective,current_foudamental_effective,power_foudamental_effective;//��ѹ/����/���ʻ�����Чֵ
extern float voltage_cf,current_cf;//��ѹ����������������������
extern float power_factor,d_power_factor;//��������[PF]��λ�ƹ�������[DPF]
extern float THD_r_voltage,THD_f_voltage,THD_r_current,THD_f_current,THD_r_power,THD_f_power;//��г�������ʣ����������Чֵ������+г��������[THD%r]/��Ի�����Чֵ[THD%f]

extern float voltage_fundamental_frequency , current_fundamental_frequency;//��ѹ/��������Ƶ��
extern float voltage_foudamental_phase , current_foudamental_phase , power_foudamental_phase;//��ѹ/����/���ʻ�����λ
/*��־*/
extern uint8_t collect_finished;
extern uint8_t voltage_capture_finished , current_capture_finished;


//extern uint32_t SAMPLING_FREQ_FFT;

extern uint8_t pause_flag,af_flag,r_or_f_flag;
extern u8 inrush_trigger_flag;

extern u8 timer_1s_blink;



union RAMsavedef
{
	float Input[2048+256];
	
	struct
	{		
		int32_t InjectedConvData[1280];		
		float Output[1024];
	}K4_tab;
	
	
};


extern union RAMsavedef RAMsave;

void dealwith_information(void);




void FFT(void);
//void Test_FFT(void) ;
float Adj_V(float sdadc_value);
uint8_t get_formed1024(void);
#endif