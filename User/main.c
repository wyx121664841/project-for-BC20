//////////////////////////////////////////////////////////////////////////////////

//******************************************************************************/

#include "stm32f0xx.h"
#include "string.h"
#include "stm32f0xx_syscfg.h"
#include "delay.h"
#include "USART1.h"
#include "math.h"
#define  APPLICATION_ADDRESS    ((uint32_t)0x08005000)
extern void  USART1_Init();
extern void  TIM2_Config();
//extern void sendchar();
extern void USART1_IRQHandler();
extern void TIM3_IRQHandler();

extern uint16_t num;
extern unsigned char RB[200];
extern int ReFlame_OneBak[200];
uint8_t Flame[100];
uint8_t buff[100];

//char strx[100];
const char *strx;
const char *stry;
const char *strz;
char z[200];
char xinh[200];
char xinxinh[200];
char xinxinxinh[25];
char xinxinxinxinh[25];
char front[]="AT+QISEND=0,25,";
char *addr[39];
char **stra=addr;
extern int h[200];
extern char* y;

int n,m,l=0;
 //uint8_t count=0;
 // uint8_t ReFlame_OneBak[200];
/*---------------------------函数定义--------------------------------------------------*/
int fprint(int ch, FILE *f)
{
    USART_SendData(USART1,ch);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET) {}
}
/*---------------------------------------------------------------------------------------*/
/*----------------------------变量定义-------------------------------------------------*/


int main(void)
{
#ifdef APP
    iRemapIrqVector();9
#endif
//	const char *strx=strx;	
    delay_init();
    USART1_Init(115200);
    TIM2_Config();
//    IWDG_Config();
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
		//printf("AT\n\t");
		//USART_SendData(USART2,"AT\n\t");
//		sendBUFF("AT+QGNSSCMD=0,\"$RESET,0,h11\"\r\n"); 
//			delay_ms(100000);
//			delay_ms(100000);
//			delay_ms(100000);
//			delay_ms(100000);
		sendBUFF("AT+QGNSSC=1\r\n"); 
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
		sendBUFF("AT+QGNSSCMD=0,\"$CFGSYS,h10\"\r\n");
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
		sendBUFF("AT+QIOPEN=1,0,\"UDP\",\"183.129.176.220\",\"9999\"\r\n");
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
//			delay_ms(100000);
//						delay_ms(100000);
//			delay_ms(100000);
//			delay_ms(100000);
//			delay_ms(100000);
//						delay_ms(100000);
//			delay_ms(100000);
//			delay_ms(100000);
//						delay_ms(100000);

			
		sendBUFF("AT+QGNSSRD=\"NMEA/GGA\"\r\n");
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);


//(char)RB;
strcpy(xinh,RB);//调出字符串

//	for(l=0;l<200;l++)
//		{
//		//		 *xinh=strx[0]; 			
//			xinh[l]=(char)RB[l];//强制转换
//		}

		
		
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
			delay_ms(100000);
//for(m=0;*(ReFlame_OneBak+m)!=' ';m++)
//{	
    strx=strstr(xinh,"GBGGA");//取出指针ESCAPE FROM ARRAY
//}


		if(strx==NULL)//早做判断防止干扰
		  {		
				sendBUFF("AT+QISEND=0,31,1__TIME OUT,WAITING FOR RESTART\r\n");					//dead	
				delay_ms(100000);
				delay_ms(100000);
				delay_ms(100000);
				delay_ms(100000);
				IWDG_Enable(); //直接放狗
			}	
				else		
			   {
//					 if('G'==(char)RB[0])//强制类型转换
//							{
//								 strcpy(xinh,RB);//调出字符串
//							}  

					 stry=strstr(xinh,"99.99,,");//无坐标标识
							delay_ms(100000);
							delay_ms(100000);
							if(stry!=NULL)
							 {
									sendBUFF("AT+QISEND=0,20,1__CONNECTION DENIED\r\n");  //no single
								 				delay_ms(100000);
												delay_ms(100000);
//												delay_ms(100000);
//												delay_ms(100000);
												IWDG_Enable(); //直接放狗
							 }
										else
										 {
												delay_ms(100000);
												delay_ms(100000);
											 	delay_ms(100000);
												delay_ms(100000);
											 	delay_ms(100000);
												delay_ms(100000);
											 //												sendBUFF("AT+QISEND=0,3,666\r\n");
												for(n=200;n>0;n--)
														{
                               xinxinh[200-n]=xinh[n];
														}
														for(n=18;n<55;n++)
														   {
																 if	(xinh[n]=='E')	
																   {
																	     for(m=0;m<25;m++)
																		       {
                                              xinxinxinh[m]=xinh[n-24+m];																					 
																					 }
																					 if(xinxinxinh[0]==',')
																					   {
																						     xinxinxinh[0]='0';  //截取坐标
																						 }
//																				for(m=0;m<25;m++)		 
//																					{
//																						xinxinxinxinh[m]=xinxinxinh[24-m];
//																					} 
																												 
																				*addr=strcat(front,xinxinxinh);
																				sendBUFF(*addr);
																						delay_ms(100000);
																						delay_ms(100000);
																						delay_ms(100000);
																						delay_ms(100000);
																						delay_ms(100000);
																						delay_ms(100000);
																	 }																	 
															 }
												delay_ms(100000);
												delay_ms(100000);
												delay_ms(100000);
														
														
										 }
         }
			
		
			
									
//								for(l=2;l<7;l++)
//							    {
//										z[l-2]=stry[l];
//									}								
//memcpy(xinh,strx,200);
//}

//    string buff=RB;
//	  strcpy(RB,"GBGGA");
//    memcpy(strx+100,RB,100);

//			if(strx!=NULL)
//				{

//									
//							sendBUFF("EVERY THING ALL RIGHT\r\n");
//							delay_ms(100000);
//							delay_ms(100000);
//							delay_ms(100000);
//							delay_ms(100000);
//							delay_ms(100000);
//							delay_ms(100000);
//							IWDG_Enable(); 
//									
//					  }
//					
//					
//					
//					
//						for(n=16;n<41;n++) 
//					    {
//						    z[n-16]=strx[n];
//                  {						   
//						        if (strz[n]=='E') 
//											break;
//									}
//									
//					     }

//				}		
//	if(strx==NULL)
//		{
//			 sendBUFF("LOST\r\n");
//		}
	delay_ms(100000);
	delay_ms(100000);
	delay_ms(100000);
	delay_ms(100000);
//	delay_ms(100000);
//	delay_ms(100000);
//	delay_ms(100000);	
//	delay_ms(100000);		
////for(num=0;num<200;num++) //接收坐标位控制test N test1 E
////{
//////	int *RB= &ReFlame_OneBak;

////// 	RB[num]=ReFlame_OneBak[num];

////////	int *i= &RB;
////////            (ReFlame_OneBak->*RB)();					 
////}
		  IWDG_Enable(); 
//					

//while(1)
//{
//	
////printf("AT+QGNSSRD=\"NMEA/GGA\"\r\n");
//strx=strstr((const char*)RB,(const char*)"GBGGA");
//  if(strx!=NULL)
//    {
//		  sendBUFF("baba\r\n");
//    }
//	   else
//			 {
//		     while(l==0)
//					 {
//				     sendBUFF("BROKEN\r\n");
//				     
//	         }
//	
//				 }
//	
//	
//	
//	
//                
//}

//buff[100]=USART_ReceiveData(USART1);
//buff[100]=USART_ReceiveData(USART2);

//USART_SendData(USART2,buff[100]);

//printf("AT+QGNSSC=1\n\t");
//printf("AT+QGNSSCMD=0,\"$RESET,0,h11\" \n\t");
	

//    while (1)
//    {
//     
//    }

}

