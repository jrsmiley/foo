#include "stm32f4xx.h"
#include "sysclk.h"

void SystemClockConfig(struct sysclkSpec *spec)
{
    SET_BIT(RCC->CR, RCC_CR_HSEON); 

    // Wait for HSERDY
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY));

    // Set FLASH latency 
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, spec->flashLatency);

    // Main PLL configuration and activation 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (spec->PLLM << RCC_PLLCFGR_PLLM_Pos));
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (spec->PLLN << RCC_PLLCFGR_PLLN_Pos));
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, 0x00);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, (spec->PLLQ << RCC_PLLCFGR_PLLQ_Pos));

    // Set PLL source to HSE
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);

    // Enable the PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);

    // Wait for PLLRDY
    while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY));

    // Set AHB Prescaler to "not divided" (clear high-order bit)
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_HPRE_3);

    // Activeate PLL as SYSCLK
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_1);

    // Wait for SYSCLK to indicate that the clock source is now PLL
    while (!(READ_REG(RCC->CFGR) & RCC_CFGR_SWS_1));

    // Set APB1 (low speed) and APB2 (high speed) prescalers
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, (RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_0));  // 48MHz
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_2);                       // 84MHz
    
    // Update CMSIS SystemCoreClock variable 
    SystemCoreClock = 168000000;
}

