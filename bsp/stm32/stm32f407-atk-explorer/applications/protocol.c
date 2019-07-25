/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         protcol.c
* @author       liusen
* @version      V1.0
* @date         2017.08.18
* @brief        Э����������
* @details      
* @par History  ������˵��
*                 
* version:	liusen_20170818
*/

#include "protocol.h"
//#include "usart.h"
//#include "app_buzzer.h"
//#include "bsp_servo.h"
//#include "bsp_colorful.h"
//#include "app_bluetooth.h"
//#include "bsp_fire.h"
#include <stdlib.h>
#include <string.h>

extern unsigned char newLineReceived ;
extern unsigned char inputString[80] ;  
extern unsigned int g_num;
extern unsigned char  startBit ;
extern int g_packnum ;

unsigned char ProtocolString[80] = {0};
int CarSpeedControl = 2400;


/*С������״̬ö��*/

enum {

  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT

}enCarState;

#define 	run_car     '1'//����ǰ
#define 	back_car    '2'//������
#define 	left_car    '3'//������
#define 	right_car   '4'//������
#define 	stop_car    '0'//����ͣ


int g_CarState = enSTOP; //  1ǰ2��3��4��0ֹͣ

/**
* Function       StringFind
* @author        liusen
* @date          2017.08.18    
* @brief         �ַ�������
* @param[in]     pSrc:Դ�ַ���; pDst:���ҵ��ַ���; v_iStartPos:Դ�ַ�����ʼλ��
* @param[out]    void
* @retval        void
* @par History   ��
*/

int StringFind(const char *pSrc, const char *pDst, int v_iStartPos)  
{  
    int i, j;  
    for (i = v_iStartPos; pSrc[i]!='\0'; i++)  
    {  
        if(pSrc[i]!=pDst[0])  
            continue;         
        j = 0;  
        while(pDst[j] !='\0' && pSrc[i+j]!='\0')  
        {  
            j++;  
            if(pDst[j]!=pSrc[i+j])  
            break;  
        }  
        if(pDst[j]=='\0')  
            return i;  
    }  
    return -1;  
} 


/**
* Function       ProtocolCpyData
* @author        liusen
* @date          2017.08.18    
* @brief         �������ݿ������µ�buf�з�ֹ��������б������ݸ���
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/

/**
* Function       Protocol
* @author        liusen
* @date          2017.08.18    
* @brief         Э�����
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/

int g_modeSelect = 0;  //0��Ĭ��APK��λ��״̬;  1:����ң�� 2:Ѳ��ģʽ 3:���������� 4: �߲�̽�� 5: Ѱ��ģʽ 6: �������
unsigned char g_Boolfire = 0;	    //�����ʱ�ر��ϱ��������ϱ����ݻ�ʹ�Ҷ������IOģʽ��ͻ��

void Protocol(void)
{

  memcpy(ProtocolString, inputString, g_packnum + 1);
	memset(inputString, 0x00, sizeof(inputString));

	/*����ģʽ�л�*/
  	//���ж��Ƿ���ģʽѡ��	
	if(StringFind((const char *)ProtocolString, (const char *)"MODE", 0) > 0 
		&& StringFind((const char *)ProtocolString, (const char *)"4WD", 0) > 0)
	{
		
	    if (ProtocolString[10] == '0') //ֹͣģʽ
	    {
	    Car_Stop();
			 g_CarState = enSTOP;
			 g_modeSelect = 0;
			// BeepOnOffMode();
	    }
	    else
	    {
//						switch (ProtocolString[9])
//					{
//						case '0': g_modeSelect = 0; ModeBEEP(0); break;
//						case '1': g_modeSelect = 1; ModeBEEP(1); break;
//						case '2': g_modeSelect = 2; ModeBEEP(2); break;
//						case '3': g_modeSelect = 3; ModeBEEP(3); break;
//						case '4': g_modeSelect = 4; ModeBEEP(4); break;
//						case '5': g_modeSelect = 5; ModeBEEP(5); break;
//						case '6': g_modeSelect = 6; ModeBEEP(6); break;
//						default: g_modeSelect = 0; break;
//					}
//			  BeepOnOffMode();
	    }
	    newLineReceived = 0;  
		  memset(ProtocolString, 0x00, sizeof(ProtocolString));
	    return;
  	}

/*
	//������λ�������Ķ����̨�Ŀ���ָ�ִ�ж����ת
	//��:$4WD,PTZ180# ���ת����180��
	if (StringFind((const char *)ProtocolString, (const char *)"PTZ", 0) > 0)
	{
		int m_kp, i, ii;

		i = StringFind((const char *)ProtocolString, (const char *)"PTZ", 0); //Ѱ����PTZ��ͷ,#�����м���ַ�
		ii = StringFind((const char *)ProtocolString, (const char *)"#", i);
		if (ii > i)
		{
			char m_skp[5] = {0};
			memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
			
			m_kp = atoi(m_skp);        //���ҵ����ַ����������

			Angle_J1 = (180 - m_kp);//ת����ָ���Ƕ�m_kp

			newLineReceived = 0;  
			memset(ProtocolString, 0x00, sizeof(ProtocolString));
			return;
		}
  	}
	//������λ���������߲�̽�յ�ָ�������Ӧ����ɫ
	//��:$4WD,CLR255,CLG0,CLB0# �߲ʵ�����ɫ
	else if (StringFind((const char *)ProtocolString, (const char *)"CLR", 0) > 0)
	{
		int m_kp, i, ii, red, green, blue;
		char m_skp[5] = {0};
		
		i = StringFind((const char *)ProtocolString, (const char *)"CLR", 0);
		ii = StringFind((const char *)ProtocolString, (const char *)",", i);
		if (ii > i)
		{			
			memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
			m_kp = atoi(m_skp);
			red =   m_kp;
		}
		i = StringFind((const char *)ProtocolString, (const char *)"CLG", 0);
		ii = StringFind((const char *)ProtocolString, (const char *)",", i);
		if (ii > i)
		{
			memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
			m_kp = atoi(m_skp);
			green =   m_kp;
		}
		i = StringFind((const char *)ProtocolString, (const char *)"CLB", 0);
		ii = StringFind((const char *)ProtocolString, (const char *)"#", i);
		if (ii > i)
		{
			memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
			m_kp = atoi(m_skp);
			blue =  m_kp;
			bsp_Colorful_RGB_PWM(red, green, blue);//������Ӧ��ɫ�ĵ�
			newLineReceived = 0;  
			memset(ProtocolString, 0x00, sizeof(ProtocolString));
			return;
		}
	}
	
	*/
	//������λ��������ͨ��Э��ָ��,��ִ����Ӧ�Ķ���
	//��:$1,0,0,0,0,0,0,0,0,0#    С��ǰ��
	if (StringFind((const char *)ProtocolString, (const char *)"4WD", 0) == -1)
	{
		//С��ԭ�����������ж�
		if (ProtocolString[3] == '1')      //С��ԭ������
		{
			g_CarState = enTLEFT;
		}
		else if (ProtocolString[3] == '2') //С��ԭ������
		{
			g_CarState = enTRIGHT;
		}
		else
		{
			g_CarState = enSTOP;
		}

		//С�������ж�
//		if (ProtocolString[5] == '1')     //����
//		{
//			whistle();
//		}

		//С���Ӽ����ж�
		if (ProtocolString[7] == '1')    //���٣�ÿ�μ�600
		{
			CarSpeedControl += 600;
			if (CarSpeedControl > 7200)
			{
				CarSpeedControl = 7200;
			}
		}
		if (ProtocolString[7] == '2')	//���٣�ÿ�μ�600
		{
			CarSpeedControl -= 600;
			if (CarSpeedControl < 3600)
			{
				CarSpeedControl = 3600;
			}
		}
#if 0
		//������������ж�
		if (ProtocolString[9] == '1') //�����ת��180��
		{
			Angle_J1 = 180;
		}
		if (ProtocolString[9] == '2') //�����ת��0��
		{
			Angle_J1 = 0;
		}

		//����ж�
		if (ProtocolString[13] == '1')//�߲ʵ�����ɫ
		{
			bsp_Colorful_RGB_PWM(255, 255, 255);
		}
		else if (ProtocolString[13] == '2')//�߲ʵ�����ɫ
		{
			bsp_Colorful_RGB_PWM(255, 0, 0);
		}
		else if (ProtocolString[13] == '3')//�߲ʵ����̵�
		{
			bsp_Colorful_RGB_PWM(0, 255, 0);
		}
		else if (ProtocolString[13] == '4') //�߲ʵ�������
		{
			bsp_Colorful_RGB_PWM(0, 0, 255);
		}
		else if (ProtocolString[13] == '5') //�߲ʵ�����ɫ
		{
			bsp_Colorful_RGB_PWM(0, 255, 255);
		}
		else if (ProtocolString[13] == '6') //�߲ʵ���Ʒ��
		{
			bsp_Colorful_RGB_PWM(255, 0, 255);
		}
		else if (ProtocolString[13] == '7') //�߲ʵ�����ɫ
		{
			bsp_Colorful_RGB_PWM(255, 255, 0);
		}
		else if (ProtocolString[13] == '8') //�߲ʵ���
		{
			bsp_Colorful_RGB_PWM(0, 0, 0);
		}

		//����ж�
		if (ProtocolString[15] == '1')  //���
		{
			//digitalWrite(OutfirePin, !digitalRead(OutfirePin) );
			Fire_Control(ON);
			g_Boolfire = 1;
		}
		else if (ProtocolString[15] == '0')  //���
		{
			//digitalWrite(OutfirePin, !digitalRead(OutfirePin) );
			Fire_Control(OFF);
			g_Boolfire = 0;
		}

		//�����Ϊ�ж�
		if (ProtocolString[17] == '1') //�����ת��90��
		{
			Angle_J1 = 90;
		}
#endif
		//С����ǰ��,����,��ת,��ת,ֹͣ����
		if (g_CarState != enTLEFT && g_CarState != enTRIGHT)
		{
			switch (ProtocolString[1])
			{
				case run_car:   g_CarState = enRUN;  break;
				case back_car:  g_CarState = enBACK;  break;
				case left_car:  g_CarState = enLEFT;  break;
				case right_car: g_CarState = enRIGHT;  break;
				case stop_car:  g_CarState = enSTOP;  break;
				default: g_CarState = enSTOP; break;
			}
		}

		newLineReceived = 0;  
		memset(ProtocolString, 0x00, sizeof(ProtocolString));  

		//����С��״̬����Ӧ�Ķ���
		switch (g_CarState)
		{
			case enSTOP: Car_Stop(); break;
			case enRUN: Car_Run(CarSpeedControl); break;
			case enLEFT: Car_Left(CarSpeedControl); break;
			case enRIGHT: Car_Right(CarSpeedControl); break;
			case enBACK: Car_Back(CarSpeedControl); break;
			case enTLEFT: Car_SpinLeft(CarSpeedControl, CarSpeedControl); break;
			case enTRIGHT: Car_SpinRight(CarSpeedControl, CarSpeedControl); break;
			default: Car_Stop(); break;
		}
	}
	

}

