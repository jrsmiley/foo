#include "stm32f4xx.h"

/* Set the System Clock, AHB, APB1, and APB2 prescalers and set flash latency to 5
 *
 * Target SYSCLK = 168MHz using 25MHz HSE crystal/ceramic oscillator
 *
 * Flash latency = 5
 * PLLM = 25
 * PLLN = 336
 * PLLP = 2
 * PLLQ = 7  (48 MHz)
 * AHB prescaler = 1
 * APB1 prescaler = 4 (42 MHz)
 * APB2 prescaler = 2 (84 MHz)

 * The voltage scaling is adjusted to fHCLK frequency as follows:
 * – Scale 2 for fHCLK ≤ 144 MHz
 * – Scale 1 for 144 MHz < fHCLK ≤ 168 MHz. (default)
 */

void SystemClockConfig(void)
{
    // Enable HSE oscillator (25MHz for 1BitSy)
    SET_BIT(RCC->CR, RCC_CR_HSEON); 

    // Wait for HSERDY
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY));

    // Set FLASH latency 
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);

    // Main PLL configuration and activation 

    // PLLM = 25
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_0));

    // PLLN = 336
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (RCC_PLLCFGR_PLLN_8 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_4));

    // PLLP = 2 (both bits cleared)
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, 0x00);

    // PLLQ = 7 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, (RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2));

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

