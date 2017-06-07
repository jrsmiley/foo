#include "stm32f4xx.h"

void     SystemClock_Config(void);

int main(void)
{
    /* Configure the system clock to 168 MHz */
    SystemClock_Config();
  
    /* Infinite loop */
    while (1)
    {
    }
}

/*
 *The voltage scaling is adjusted to fHCLK frequency as follows:
 *– Scale 2 for fHCLK ≤ 144 MHz
 *– Scale 1 for 144 MHz < fHCLK ≤ 168 MHz.
 */

void SystemClock_Config(void)
{
    /* Enable HSE oscillator */
    SET_BIT(RCC->CR, RCC_CR_HSEON); 

    // Wait for HSERDY
    while(!READ_BIT(RCC->CR, RCC_CR_HSERDY));

  /* Set FLASH latency */
  //LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* Main PLL configuration and activation */
  //LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 336, LL_RCC_PLLP_DIV_2);
  //LL_RCC_PLL_Enable();
  //while(LL_RCC_PLL_IsReady() != 1)
  //{
  //};

  /* Sysclk activation on the main PLL */
  //LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  //LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  //while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  //{
  //};

  /* Set APB1 & APB2 prescaler */
  //LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  //LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  //SystemCoreClock = 168000000;
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
