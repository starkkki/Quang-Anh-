#include "main.h"

#define GPIOD_BASE_ADDR 0x40020C00
#define GPIOA_BASE_ADDR 0x40020000
#define EXTI_BASE_ADDR 0x40013C00
#define SYSCFG_BASE_ADDR 0x40013800
#define RCC_BASE_ADDR 0x40023800
#define NVIC_ISER0 (*(volatile uint32_t*)(0xE000E100))
volatile int led_blocked = 0;
volatile int button_event = 0;

void LedInit()
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  uint32_t* GPIOD_MODER = (uint32_t*)(GPIOD_BASE_ADDR + 0x00);
  *GPIOD_MODER &= ~(0b11 << 24);
  *GPIOD_MODER |=  (0b01 << 24);
}

void buttonInit()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  uint32_t* GPIOA_MODER = (uint32_t*)(GPIOA_BASE_ADDR + 0x00);
  *GPIOA_MODER &= ~(0b11 << 0);


  volatile uint32_t* RCC_APB2ENR = (uint32_t*)(RCC_BASE_ADDR + 0x44);
  *RCC_APB2ENR |= (1 << 14);


  volatile uint32_t* SYSCFG_EXTICR1 = (uint32_t*)(SYSCFG_BASE_ADDR + 0x08);
  *SYSCFG_EXTICR1 &= ~(0xF << 0);


  volatile uint32_t* EXTI_RTSR = (uint32_t*)(EXTI_BASE_ADDR + 0x08);
  volatile uint32_t* EXTI_IMR  = (uint32_t*)(EXTI_BASE_ADDR + 0x00);
  *EXTI_RTSR |= (1 << 0);
  *EXTI_IMR  |= (1 << 0);


  NVIC_ISER0 |= (1 << 6);
}

void LedCtrl(int on_off)
{
  volatile uint32_t* GPIOD_BSRR = (uint32_t*)(GPIOD_BASE_ADDR + 0x18);
  if (on_off == 1)
    *GPIOD_BSRR = (1 << 12);
  else
    *GPIOD_BSRR = (1 << (12 + 16));
}

int readButton()
{
  uint32_t* GPIOA_IDR = (uint32_t*)(GPIOA_BASE_ADDR + 0x10);
  return (*GPIOA_IDR & (1 << 0)) ? 1 : 0;
}

void EXTI0_IRQHandler(void)
{
	volatile uint32_t* EXTI_PR = (uint32_t*)(EXTI_BASE_ADDR + 0x14);
	  if (*EXTI_PR & (1 << 0))
	  {
	    HAL_Delay(20);
	    if (readButton()) {
	      button_event = 1;
	    }

	    *EXTI_PR = (1 << 0);
	  }
}

int main()
{
  HAL_Init();
  LedInit();
  buttonInit();

  while (1)
    {
      if (button_event)
      {
        button_event = 0;

        if (readButton())
        {
          led_blocked = 1;
          LedCtrl(0);
        }
        else
        {
          led_blocked = 0;
        }
      }

      if (!led_blocked)
      {
        LedCtrl(1);
        HAL_Delay(500);
        LedCtrl(0);
        HAL_Delay(500);
      }
    }
  return 0;
}




