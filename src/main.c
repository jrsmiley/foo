#include "stm32f4xx.h"
#include "sysclk.h"

void SPI2GPIOConfig (void);
void SPIMasterInit (SPI_TypeDef *SPIx);
void SPIDisable (SPI_TypeDef *SPIx);

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

struct sysclkSpec sysclk25_168 = { .flashLatency = 5, .PLLM = 25, .PLLN = 336, .PLLP = 2, .PLLQ = 7, .AHBPrescaler = 1, .APB1Prescaler = 4, .APB2Prescaler = 2, .VRegScale = 2 };

int main(void)
{
    SystemClockConfig(&sysclk25_168);
    SPI2GPIOConfig();
    SPIMasterInit(SPI2);
    SPIDisable(SPI2);

    while (1)
    {
    }
}

/*
 * Configure GPIOB pins 12-15 for SPI2
 * PB12 = NSS
 * PB13 = SCK
 * PB14 = MISO
 * PB15 = MOSI
 *
 * MODE = AF
 * OTYPE = Output Push-Pull
 * OSPEED = Very High Speed
 * PUPD = NO Pull-up Pull-down
 * AF = AF_5
 */
void SPI2GPIOConfig (void) {
    // Eable the GPIOB clock
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);

    // Configure GPIOB pins 12-15

    // Set MODER to AF mode (0b10)
    MODIFY_REG(GPIOB->MODER, (GPIO_MODER_MODE12 | GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15), 
                             (GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1));

    // Set OTYPER to Output Push-Pull (0b0)
    MODIFY_REG(GPIOB->OTYPER, (GPIO_OTYPER_OT12 | GPIO_OTYPER_OT13 | GPIO_OTYPER_OT14 | GPIO_OTYPER_OT15), 0x00);

    // Set OSPEEDR to Very High Speed (0b11)
    MODIFY_REG(GPIOB->OSPEEDR, (GPIO_OSPEEDR_OSPEED12 | GPIO_OSPEEDR_OSPEED13 | GPIO_OSPEEDR_OSPEED14 | GPIO_OSPEEDR_OSPEED15),
                               (GPIO_OSPEEDR_OSPEED12_0 | GPIO_OSPEEDR_OSPEED12_1 | GPIO_OSPEEDR_OSPEED13_0 | GPIO_OSPEEDR_OSPEED13_1 |
                                GPIO_OSPEEDR_OSPEED14_0 | GPIO_OSPEEDR_OSPEED14_1 | GPIO_OSPEEDR_OSPEED15_0 | GPIO_OSPEEDR_OSPEED15_1));

    // Set PUPDR to No Pull-Up Pull-Down (0b00)
    MODIFY_REG(GPIOB->PUPDR, (GPIO_PUPDR_PUPD12 | GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15), 0x00);

    // Set AF5
    MODIFY_REG(GPIOB->AFR[1], (GPIO_AFRH_AFSEL12 | GPIO_AFRH_AFSEL13 | GPIO_AFRH_AFSEL14 | GPIO_AFRH_AFSEL14), 
                              (GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL13_0 | GPIO_AFRH_AFSEL13_2 |
                               GPIO_AFRH_AFSEL14_0 | GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2));
}

void SPIMasterInit (SPI_TypeDef *SPIx) {
    // Enable the SPI clock
    if (SPIx == SPI1) SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
    if (SPIx == SPI2) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
    if (SPIx == SPI3) SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3EN);

    SET_BIT(SPIx->CR2, SPI_CR2_SSOE);
    WRITE_REG(SPIx->CR1, 0b0000101101100111);
    // SET_BIT(SPIx->CR1, SPI_CR1_MSTR);
    // SET_BIT(SPIx->CR1, SPI_CR1_SPE);
}


void SPIDisable (SPI_TypeDef *SPIx) {
    // Wait for in-flight RX to complete
    while (SPIx->SR & SPI_SR_RXNE);

    // Wait for in-flight TX to complete
    while(!(SPIx->SR & SPI_SR_TXE));

    // Wait for SPI BSY = 0
    while(SPIx->SR & SPI_SR_BSY);

    // Disable the SPI
    CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);
}
