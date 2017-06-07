#include "stm32f4xx.h"
#include "sysclk.h"

void SPI2GPIOConfig (void);

int main(void)
{
    SystemClockConfig();
    SPI2GPIOConfig();

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
