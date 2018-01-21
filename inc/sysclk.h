#pragma once

struct sysclkSpec {
    uint16_t flashLatency;
    uint16_t PLLM;
    uint16_t PLLN;
    uint16_t PLLP;
    uint16_t PLLQ;
    uint16_t AHBPrescaler;
    uint16_t APB1Prescaler;
    uint16_t APB2Prescaler;
    uint16_t VRegScale;
};


void SystemClockConfig(struct sysclkSpec *spec);
