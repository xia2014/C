#include "wifi_function.h"
#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_ov7725.h"
#include <string.h> 
#include <stdio.h>  
#include <stdbool.h>

//#include "motor.h"
//#include "duoji.h"

//extern char *pStrDelimiter [2];
/*
 * ��������ESP8266_Choose
 * ����  ��ʹ��/����WF-ESP8266ģ��
 * ����  ��enumChoose = ENABLE��ʹ��ģ��
 *         enumChoose = DISABLE������ģ��
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		ESP8266_CH_HIGH_LEVEL();
	
	else
		ESP8266_CH_LOW_LEVEL();
	
}

/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ����ESP8266_AT_Test����
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 ESP8266_RST_LOW_LEVEL();
	 Delay_ms ( 500 ); 
	 ESP8266_RST_HIGH_LEVEL();
	 
	#endif

}

/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_AT_Test ( void )
{
	ESP8266_RST_HIGH_LEVEL();
	
	Delay_ms ( 1000 ); 
	
	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 200 ) ) ESP8266_Rst ();  	

}

/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );
	//PC_Usart("demo\r\n");
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	Delay_ms ( waittime );                 //��ʱ
	
	//strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
//���ж��У�FramLength����������ֱ�������ݼĴ���Ϊ�գ��ڽ��ջ����������������һ��'\0'
	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );//��WiFi���صķ����ַ�����ӡ������
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
  }
	
}

/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 7000 );
	
}

/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}

/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}

/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );
	
}

/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}

/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend ( void )
{
	return (
	  ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) &&
	  ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ) );
}

/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
		ESP8266_Usart ( "%s", pStr );

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;
}

/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, ">" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	}
	else
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	return pRecStr;
}

/*
 * ��������ESP8266_WebFetch_Test
 * ����  ��WF-ESP8266ģ�����ץȡ��ҳ����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_Connect_Server ( void )
{
	char /*cStrInput [100] = { 0 },*/ * pStrDelimiter [2]/*, * pBuf, * pStr*/;
	//u8 uc = 0;
	PC_Usart ( "\r\nWiFiʵ��\r\n" );
	//Delay_ms ( 2000 );
	//�ص�͸������
	ESP8266_Usart ( "+++" );
	Delay_ms ( 2000 );
	ESP8266_Net_Mode_Choose ( STA );
	//ESP8266_Cmd ( "AT+CWLAP", "OK", 0, 5000 );
	do
	{
		pStrDelimiter [0] = "NETGEAR65";
		pStrDelimiter [1] = "kindwindow635";
//		pStrDelimiter [0] = "TP-LINK_65F8";
//		pStrDelimiter [1] = "69306823";
  } while ( ! ESP8266_JoinAP ( pStrDelimiter [0], pStrDelimiter [1] ) );

	//while ( ! ESP8266_Link_Server ( enumTCP, "eat.weixincore.com", "80", Single_ID ) );
	//ESP8266_SendString ( DISABLE, "\'{\"type\":\"login\",\"client_name\":\"mcu\",\"room_id\":\"1\"}\'", 52, Single_ID );
	//while ( ! ESP8266_Link_Server ( enumTCP, "114.215.111.84", "1234", Single_ID ) );
	while ( ! ESP8266_Link_Server ( enumTCP, "192.168.1.3", "100", Single_ID ) );
	//while ( ! ESP8266_Link_Server ( enumTCP, "172.17.135.241", "100", Single_ID ) );
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,100);
	ESP8266_UnvarnishSend ();
	ESP8266_SendString ( ENABLE, "{\"type\":\"login\",\"client_name\":\"mcu\",\"room_id\":\"1\"}END", NULL, Single_ID );
	//ESP8266_SendString ( ENABLE, "webpage:This is a wifi demo", NULL, Single_ID );
	/*ESP8266_SendString ( ENABLE, "GET xiaoche.html HTTP/1.1\r\n", NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Host: xiaoche.html\r\n",                            NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "User-Agent: abc\r\n",                                     NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Connection: close\r\n",                                   NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "\r\n",                                                    NULL, Single_ID );*/

	//USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
}
///*
// * ��������Motor_Control
// * ����  �����Ƶ��
// * ����  �������ַ���
// * ����  : ��
// * ����  ����Motor_Duoji()����
// */
//void Motor_Control	( char ch )
//{
//		switch( ch )
//		{
//			case 'A':
//				Motor_GoStraight();
//				break;
//			case 'B':
//				Motor_Stop();
//				break;
//			case 'C':
//				Motor_Back();
//				break;
//		}
//}
///*
// * ��������Duoji_Control
// * ����  �����ƶ��
// * ����  �������ַ���
// * ����  : ��
// * ����  ����Motor_Duoji()����
// */
//void Duoji_Control( char ch )
//{
//		switch( ch )
//		{
//			case 'D':
//				Duoji_Right();
//				Delay_ms ( 4000 );
//				break;
//			case 'E':
//				Duoji_Left();
//				Delay_ms ( 4000 );
//				break;
//			case 'F':
//				Duoji_Zero();
//				Delay_ms ( 4000 );
//				break;
//		}
//}
///*
// * ��������Cmd_String
// * ����  �����ɶ��ŷָ����ַ�����ֳ������ַ���
// * ����  �������ַ���
// * ����  : ��
// * ����  ����Motor_Duoji()����
// */
//void Cmd_String( char *cStrInput )
//{
//	char/* * pStrDelimiter [2],*/ * pBuf, * pStr;
//	char uc = 0;
//	pBuf = cStrInput;
//	uc = 0;
//	while ( ( pStr = strtok ( pBuf, "," ) ) != NULL )
//	{
//		pStrDelimiter [ uc ++ ] = pStr;
//		pBuf = NULL;
//	} 
//	//PC_Usart("str1 is %s,str2 is %s",pStrDelimiter[0],pStrDelimiter[1]);
//}
///*
// * ��������Motor_Duoji
// * ����  �����������ֱַ���Ƶ���Ͷ��
// * ����  ��������1�Ϳ�����2
// * ����  : ��
// * ����  ����main�����е�whileѭ������
// */
//void Motor_Duoji( char *str1, char *str2 )
//{
//	Motor_Control( pStrDelimiter[0][0] );
//	Duoji_Control( pStrDelimiter[1][0] );
//}

//void Motor_Duoji2 ( char ch )
//{
//		switch( ch )
//		{
//			case 'A':
//				Motor_GoStraight();
//				break;
//			case 'B':
//				Motor_Stop();
//				break;
//			case 'C':
//				Motor_Back();
//				break;
//			case 'D':
//				Duoji_Right();
//				Delay_ms ( 4000 );
//				break;
//			case 'E':
//				Duoji_Left();
//				Delay_ms ( 4000 );
//				break;
//			case 'F':
//				Duoji_Zero();
//				Delay_ms ( 4000 );
//				break;
//		}
//}

//void Motor_Control2( char ch )
//{
//	u16 CCR1_Val, CCR3_Val;
//	switch( ch )
//	{
//		case 'D':
//			CCR1_Val = 500;
//			CCR3_Val = 48;
//			break;
//		case 'E':
//			CCR1_Val = 300;
//			CCR3_Val = 48;
//			break;
//		case 'F':
//			CCR1_Val = 48;
//			CCR3_Val = 48;
//			break;
//		case 'G':
//			CCR1_Val = 48;
//			CCR3_Val = 300;
//			break;
//		case 'H':
//			CCR1_Val = 48;
//			CCR3_Val = 500;
//			break;
//	}
//	Motor_Mode_Config( CCR1_Val, CCR3_Val );
//	//PC_Usart("CCR1_Val=%d, CCR3_Val=%d",CCR1_Val, CCR3_Val);
//}

//void Duoji_Control2( char ch )
//{
//	u16 angle;
//	switch( ch )
//	{
//		case 'I':
//			angle = 60;
//			break;
//		case 'J':
//			angle = 30;
//			break;
//		case 'K':
//			angle = 0;
//			break;
//		case 'L':
//			angle = -30;
//			break;
//		case 'M':
//			angle = -60;
//			break;
//	}
//	Duoji_Mode_Config( angle );
//}

//void Motor_Duoji3 ( char *str1, char *str2 )
//{
//	if( str1[0] == 'A' )
//	{
//		Motor_Control2( str2[0] );
//		//Motor_Mode_Config( 500, 48 );
//		//PC_Usart("It's A");
//	}else if( str1[0] == 'B' )
//	{
//		Duoji_Control2( str2[0] );
//	}else if( str1[0] == 'C' )
//	{
//		getImage = 1;
//	}/*else
//	{
//		PC_Usart("wrong!");
//	}*/
//}
void DMA_USART1_Send( u8 *addr, uint16_t counts,uint16_t ms )
{
	DMA_Cmd (DMA1_Channel4,DISABLE);	
	DMA1_Channel4->CNDTR = (u32)counts;
	DMA1_Channel4->CMAR = (u32)addr;
	DMA_Cmd (DMA1_Channel4,ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	Delay_ms(ms);
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
}

void DMA_USART2_Send( u8 *addr, uint16_t counts, uint16_t ms )
{
	DMA_Cmd (DMA1_Channel7,DISABLE);
	DMA1_Channel7->CNDTR = (u32)counts;
	DMA1_Channel7->CMAR = (u32)addr;
	DMA_Cmd (DMA1_Channel7,ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	Delay_ms(ms);
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
}
//void Camera(void)
//{
//		if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'C' )
//			screen_flag = 1;
//		if( Ov7725_vsync == 2 )
//		{
//			FIFO_PREPARE;  			/*FIFO׼��*/
//			ImagDisp();					/*�ɼ�����ʾ*/
//			Ov7725_vsync = 0;
//		}
//		if( screen_flag == 1 )
//		{
//			/* ����Һ��ɨ�跽��Ϊ ���½�->���Ͻ� */
//			Lcd_GramScan( 3 );
//			Screen_shot(0,0,320,240);
//			screen_flag = 0;
//		}
//}
