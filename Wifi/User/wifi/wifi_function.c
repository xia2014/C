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
 * 函数名：ESP8266_Choose
 * 描述  ：使能/禁用WF-ESP8266模块
 * 输入  ：enumChoose = ENABLE，使能模块
 *         enumChoose = DISABLE，禁用模块
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		ESP8266_CH_HIGH_LEVEL();
	
	else
		ESP8266_CH_LOW_LEVEL();
	
}

/*
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被ESP8266_AT_Test调用
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
 * 函数名：ESP8266_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_AT_Test ( void )
{
	ESP8266_RST_HIGH_LEVEL();
	
	Delay_ms ( 1000 ); 
	
	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 200 ) ) ESP8266_Rst ();  	

}

/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	ESP8266_Usart ( "%s\r\n", cmd );
	//PC_Usart("demo\r\n");
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	Delay_ms ( waittime );                 //延时
	
	//strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
//在中断中，FramLength不断自增，直到读数据寄存器为空，在接收机构体的数组最后加上一个'\0'
	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );//把WiFi返回的反馈字符串打印到电脑
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 7000 );
	
}

/*
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}

/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}

/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_UnvarnishSend ( void )
{
	return (
	  ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) &&
	  ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ) );
}

/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_WebFetch_Test
 * 描述  ：WF-ESP8266模块进行抓取网页测试
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_Connect_Server ( void )
{
	char /*cStrInput [100] = { 0 },*/ * pStrDelimiter [2]/*, * pBuf, * pStr*/;
	//u8 uc = 0;
	PC_Usart ( "\r\nWiFi实验\r\n" );
	//Delay_ms ( 2000 );
	//关掉透明传输
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
// * 函数名：Motor_Control
// * 描述  ：控制电机
// * 输入  ：控制字符串
// * 返回  : 无
// * 调用  ：被Motor_Duoji()调用
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
// * 函数名：Duoji_Control
// * 描述  ：控制舵机
// * 输入  ：控制字符串
// * 返回  : 无
// * 调用  ：被Motor_Duoji()调用
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
// * 函数名：Cmd_String
// * 描述  ：将由逗号分隔的字符串拆分成两个字符串
// * 输入  ：控制字符串
// * 返回  : 无
// * 调用  ：被Motor_Duoji()调用
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
// * 函数名：Motor_Duoji
// * 描述  ：两个控制字分别控制电机和舵机
// * 输入  ：控制字1和控制字2
// * 返回  : 无
// * 调用  ：被main函数中的while循环调用
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
//			FIFO_PREPARE;  			/*FIFO准备*/
//			ImagDisp();					/*采集并显示*/
//			Ov7725_vsync = 0;
//		}
//		if( screen_flag == 1 )
//		{
//			/* 设置液晶扫描方向为 右下角->左上角 */
//			Lcd_GramScan( 3 );
//			Screen_shot(0,0,320,240);
//			screen_flag = 0;
//		}
//}
