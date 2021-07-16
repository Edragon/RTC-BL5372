/*

作者：紫电青霜


*/


/*Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "usb_device.h"
#include "BL5372_IIC.h"
#include "usbd_cdc_if.h"


uint8_t  RRTCiic_buf[16];
uint8_t  RRTCiic_Buf_Out[26]={0x32,0x30,0x31,0x37,0xC4,0xEA,0x30,0x32,0xD4,0xC2,
                             0x32,0x35,0xC8,0xD5,0x32,0x32,0x3A,0x35,0x36,0x3A,0x00,0x00,0x0D,0x0A,0x0D,0x0A};


													
extern unsigned char Set_Year ;	
extern unsigned char Set_Month ;	
extern unsigned char Set_Day ;	
extern unsigned char Set_Hour ;	
extern unsigned char Set_Minute ;	
extern unsigned char Set_Second ;															 
														 
uint8_t   Rec_buff[18]={0};



/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void My_GPIO_Init(void);


unsigned char  DataStr[]=__DATE__;
unsigned char  TimeStr[]=__TIME__;
extern uint8_t UserRxBufferFS[];

int main(void)
{
  unsigned char i=0;
	unsigned char Miao_mark = 0; 

		


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
	IIC_Init();
	
	My_GPIO_Init();
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) == 0)//读取PB7管脚的电平，为低电平，则初始化BL5372时间
	{
		BL5372_init();
	}

  while (1)
  {
		
		
			HAL_Delay(50);


			if(UserRxBufferFS[0] == 'S'  && UserRxBufferFS[1] == 'T'  && UserRxBufferFS[10] == 'F' && UserRxBufferFS[11] == 'N' )
			{
				CDC_Transmit_FS("设置年月日\r\n",12);
				Set_Year = ((UserRxBufferFS[4]-0x30)<<4)+(UserRxBufferFS[5]-0x30);
				Set_Month = ((UserRxBufferFS[6]-0x30)<<4)+(UserRxBufferFS[7]-0x30);
				Set_Day = ((UserRxBufferFS[8]-0x30)<<4)+(UserRxBufferFS[9]-0x30);
				BL5372_init();
			}
			else if(UserRxBufferFS[0] == 'S'  && UserRxBufferFS[1] == 'T'  && UserRxBufferFS[8] == 'F' && UserRxBufferFS[9] == 'I' )
			{
				CDC_Transmit_FS("设置时分秒\r\n",12);
				Set_Hour = ((UserRxBufferFS[2]-0x30)<<4)+(UserRxBufferFS[3]-0x30);
				Set_Minute = ((UserRxBufferFS[4]-0x30)<<4)+(UserRxBufferFS[5]-0x30);
				Set_Second = ((UserRxBufferFS[6]-0x30)<<4)+(UserRxBufferFS[7]-0x30);
				BL5372_init();
			}
			

				for(i=0;i<18;i++)
				{
					UserRxBufferFS[i] = 0;
				}

//					HAL_Delay(5);
					BL5372_Read(0x00,RRTCiic_buf,7);
					if(RRTCiic_buf[0] != Miao_mark)
					{

								//秒
								RRTCiic_Buf_Out[20] = ((RRTCiic_buf[0]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[21] = (RRTCiic_buf[0]&0x0f) +0x30;
								
								//分
								RRTCiic_Buf_Out[17] = ((RRTCiic_buf[1]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[18] = (RRTCiic_buf[1]&0x0f) +0x30;
							
								//时
								RRTCiic_Buf_Out[14] = ((RRTCiic_buf[2]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[15] = (RRTCiic_buf[2]&0x0f) +0x30;	

								//日
								RRTCiic_Buf_Out[10] = ((RRTCiic_buf[4]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[11] = (RRTCiic_buf[4]&0x0f) +0x30;
								
								//月
								RRTCiic_Buf_Out[6] = ((RRTCiic_buf[5]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[7] = (RRTCiic_buf[5]&0x0f) +0x30;
							
								//年
								RRTCiic_Buf_Out[2] = ((RRTCiic_buf[6]>>4)&0x0f) +0x30;
								RRTCiic_Buf_Out[3] = (RRTCiic_buf[6]&0x0f) +0x30;	



							CDC_Transmit_FS((uint8_t *)RRTCiic_Buf_Out,26);
					
					}
					Miao_mark = RRTCiic_buf[0] ;
					for(i=0;i<16;i++)
					{
						RRTCiic_buf[i] = 0;
					}
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}





void My_GPIO_Init(void)
{					     
		GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	   
	  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7 ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//输入
  GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉输入
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}


#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
