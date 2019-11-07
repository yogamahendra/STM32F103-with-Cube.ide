/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "i2c-lcd.h"

I2C_HandleTypeDef hi2c2;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM2_Init(void);
void MX_USART1_UART_Init(void);
void MX_I2C2_Init(void);
void MX_TIM4_Init(uint32_t Periode);

unsigned int ms=0,dd=0,mm=0;
char buffer[20],baffer[20];
unsigned int runstop=0;

unsigned int lapA=0,lapB=0,lapC=0,totlap=5,serial1=0,serial2=0,serial3=0;
unsigned int ms1,ms2,ms3,dd1,dd2,dd3,mm1,mm2,mm3,mslast1=0,mslast2=0,mslast3=0,ddlast1=0,ddlast2=0,ddlast3=0,mmlast1=0,mmlast2=0,mmlast3=0;

unsigned char bouncing	=0xFF;
unsigned char bouncing2	=0xFF;
unsigned char bouncing3	=0xFF;
unsigned char bouncing4	=0xFF;
unsigned char bouncing5	=0xFF;

uint32_t Timeout_loop 	= 0;
uint32_t Timeout_value 	= 500;
uint32_t Timeout_loop2 	= 0;
uint32_t Timeout_value2 = 500;
uint32_t Timeout_loop3 	= 0;
uint32_t Timeout_value3 = 500;
uint32_t Timeout_loop4 	= 0;
uint32_t Timeout_value4 = 500;
uint32_t Timeout_loop5 	= 0;
uint32_t Timeout_value5 = 500;

void tombolrunstop(void)
{
	if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_RESET && (Timeout_loop++<=Timeout_value)){
		bouncing=(bouncing<<1);
	} //MEKBOUNCE1
	else if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_RESET && (Timeout_loop++>Timeout_value)){
		if(Timeout_loop>=Timeout_value+100){
			Timeout_loop=Timeout_value+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
		}
	}//MEKTIMEOUT

	else if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_SET && Timeout_loop<=Timeout_value){
		bouncing = (bouncing<<1)|1;
		Timeout_loop=0;
	} //MEKBOUNCE2
	else{
		Timeout_loop=0;
		bouncing=0xFF;
	} //KEMBALI_START
	if (bouncing==0x03){
		if(runstop==0){
			HAL_TIM_Base_Start_IT(&htim2);
			runstop=1;
		}//NYALA
		else{
			HAL_TIM_Base_Stop_IT(&htim2);
			runstop=0;
		}//STOP
	}
}

void tombolreset(void)
{
	if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++<=Timeout_value2)){
		bouncing2=(bouncing2<<1);
	}//MEKBOUNCE1

	else if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++>Timeout_value2)){
		if(Timeout_loop2>=Timeout_value2+100){
			Timeout_loop2=Timeout_value2+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
		}
	}//MEKTIMEOUT

	else if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_SET && Timeout_loop2<=Timeout_value2){
		bouncing2 = (bouncing2<<1)|1;
		Timeout_loop2=0;
	}//MEKBOUNCE2

	else{
		Timeout_loop2=0;
		bouncing2=0xFF;
	}//KEADAAN_AWAL

	if (bouncing2==0x03){
		HAL_TIM_Base_Stop_IT(&htim2);
		ms=0;	dd=0;	mm=0;	runstop=0;

		ms1=0;	dd1=0;	mm1=0;
		lapA=0;	lapB=0;	lapC=0;
	}//RESET
}

void sensor1(void)
{
	if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++<=Timeout_value3)){
		bouncing3=(bouncing3<<1);
	}//MEKBOUNCE1

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++>Timeout_value3)){
		if(Timeout_loop3>=Timeout_value3+100){
			Timeout_loop3=Timeout_value3+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
		}
	}//MEKTIMEOUT

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_SET && Timeout_loop3<=Timeout_value3){
		bouncing3 = (bouncing3<<1)|1;
		Timeout_loop3=0;
	}//MEKBOUNCE2

	else{
		Timeout_loop3=0;
		bouncing3=0xFF;
	}
	if (bouncing3==0x03){

		ms1=ms-mslast1;
		mslast1=ms;

		dd1=dd-ddlast1;
		ddlast1=dd;

		mm1=mm-mmlast1;
		mmlast1=mm;

		serial1++;
		lapA++;
		if(lapA>=5)lapA=5;
	}//cuman ngitung lap
}
void sensor2(void)
{
	if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++<=Timeout_value4)){
		bouncing4=(bouncing4<<1);
	}//MEKBOUNCE1

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++>Timeout_value4)){
		if(Timeout_loop4>=Timeout_value4+100){
			Timeout_loop4=Timeout_value4+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
		}
	}//MEKTIMEOUT

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_SET && Timeout_loop4<=Timeout_value4){
		bouncing4 = (bouncing4<<1)|1;
		Timeout_loop4=0;
	}//MEKBOUNCE2

	else{
		Timeout_loop4=0;
		bouncing4=0xFF;
	}
	if (bouncing4==0x03){

		ms2=ms-mslast2;
		mslast2=ms;

		dd2=dd-ddlast2;
		ddlast2=dd;

		mm2=mm-mmlast2;
		mmlast2=mm;

		serial2++;
		lapB++;
		if(lapB>=5)lapB=5;
	}//cuman ngitung lap
}
void sensor3(void)
{
	if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++<=Timeout_value5)){
		bouncing5=(bouncing5<<1);
	}//MEKBOUNCE1

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++>Timeout_value5)){
		if(Timeout_loop5>=Timeout_value5+100){
			Timeout_loop5=Timeout_value5+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
		}
	}//MEKTIMEOUT

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_SET && Timeout_loop5<=Timeout_value5){
		bouncing5 = (bouncing5<<1)|1;
		Timeout_loop5=0;
	}//MEKBOUNCE2

	else{
		Timeout_loop5=0;
		bouncing5=0xFF;
	}
	if (bouncing5==0x03){

		ms3=ms-mslast3;
		mslast3=ms;

		dd3=dd-ddlast3;
		ddlast3=dd;

		mm3=mm-mmlast3;
		mmlast3=mm;

		serial3++;
		lapC++;
		if(lapC>=5)lapC=5;
	}//cuman ngitung lap
}

void Serial (){
	if (serial1==1){
		HAL_UART_Transmit(&huart1,(uint8_t*)"Waktu Lap A yang tercatat adalah : ",35,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mmlast1,2,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)ddlast1,2,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mslast1,2,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)"\n",1,10);
		serial1=0;
	}
	else if (serial2==1){
		HAL_UART_Transmit(&huart1,(uint8_t*)"Waktu Lap B yang tercatat adalah : ",35,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mmlast2,sizeof(mmlast2),10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)ddlast2,sizeof(mmlast2),10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mslast2,sizeof(mmlast2),10);
		serial2=0;
	}
	else if (serial3==1){
		HAL_UART_Transmit(&huart1,(uint8_t*)"Waktu Lap C yang tercatat adalah : ",35,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mmlast3,sizeof(mmlast3),10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)ddlast3,sizeof(mmlast3),10);
		HAL_UART_Transmit(&huart1,(uint8_t*)" : ",3,10);
		HAL_UART_Transmit(&huart1,(uint8_t*)mslast3,sizeof(mmlast3),10);
		serial3=0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2){
		ms++;
		if(ms>=100){
			dd++;
			ms=0;
		}
		if(dd>=60){
			mm++;
			dd=0;
		}
		if(mm>=100){
			ms=0;
			dd=0;
			mm=0;
		}
	}

	if(htim->Instance==TIM4){

		tombolrunstop();
		tombolreset();
		sensor1();
		sensor2();
		sensor3();
		Serial();
		//HAL_UART_Transmit(&huart1,(uint8_t*)buffer,sizeof(buffer),10);
		//HAL_UART_Transmit(&huart1,(uint8_t*)"\n\r",2,10);

		if(lapA<totlap){
			lcd_send_cmd(0x80);
			sprintf(buffer,"%d%d:%d%d:%d%dA%d=%d%d:%d%d",(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10),(lapA),(mm1/10),(mm1%10),(dd1/10),(dd1%10));
			lcd_send_string(buffer);
		}
		else {
			lcd_send_cmd(0x80);
			sprintf(buffer,"%d%d:%d%d:%d%dA=FINISH",(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10));
			lcd_send_string(buffer);

		}

		if((lapB<totlap) && (lapC<totlap)){
			lcd_send_cmd(0xc0);
			sprintf(baffer,"B%d=%d%d:%d%dC%d=%d%d:%d%d",(lapB),(mm2/10),(mm2%10),(dd2/10),(dd2%10),(lapC),(mm3/10),(mm3%10),(dd3/10),(dd3%10));
			lcd_send_string(baffer);
		}

		else if((lapB>=totlap) && (lapC<totlap)){
			lcd_send_cmd(0xc0);
			sprintf(baffer,"B=FINISHC%d=%d%d:%d%d",(lapC),(mm3/10),(mm3%10),(dd3/10),(dd3%10));
			lcd_send_string(baffer);
		}

		else if((lapB<totlap) && (lapC>=totlap)){
			lcd_send_cmd(0xc0);
			sprintf(baffer,"B%d=%d%d:%d%dC=FINISH",(lapB),(mm2/10),(mm2%10),(dd2/10),(dd2%10));
			lcd_send_string(baffer);
		}

		else {
			lcd_send_cmd(0xc0);
			sprintf(baffer,"BFINISH CFINISH");
			lcd_send_string(baffer);
		}
	}
}

int main(void)
{

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM4_Init(2);

//  printf("****     Password   Discoveryku ***** \n\r");
  HAL_UART_Transmit(&huart1,(uint8_t*)"Waktu Lap C yang tercatat adalah : \n",36,10);
  HAL_TIM_Base_Start_IT(&htim4);

  lcd_init();
  HAL_UART_Transmit(&huart1,(uint8_t*)"kawtu Lap C yang tercatat adalah : \n",36,10);

  while (1)
  {
	  //isi looping kosong
  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c2);
}

void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 36000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim2);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

void MX_TIM4_Init(uint32_t Periode)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 36000-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = (Periode*2)-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim4);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
}

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(User_LED_GPIO_Port, User_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : User_LED_Pin */
  GPIO_InitStruct.Pin = User_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(User_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_RunStop_Pin Button_Reset_Pin */
  GPIO_InitStruct.Pin = Button_RunStop_Pin|Button_Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Sensor1_Pin Sensor2_Pin Sensor3_Pin */
  GPIO_InitStruct.Pin = Sensor1_Pin|Sensor2_Pin|Sensor3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
