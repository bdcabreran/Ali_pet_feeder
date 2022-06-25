#ifndef INIT_PERIPH_H
#define INIT_PERIPH_H

#include "stm32f1xx_hal.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t      pin;
}gpio_port_pin_t;

extern ADC_HandleTypeDef  hadc1;
extern I2C_HandleTypeDef  hi2c1;
extern SPI_HandleTypeDef  hspi2;
extern DMA_HandleTypeDef  hdma_spi2_rx;
extern DMA_HandleTypeDef  hdma_spi2_tx;
extern TIM_HandleTypeDef  htim1;
extern UART_HandleTypeDef huart2;

// comment if no used
// #define USE_UART2_PRINTF


void init_peripherals(void);


/*Pin Labels */

///////// Navigation Buttons /////////////
#define RIGHT_Pin           GPIO_PIN_7
#define RIGHT_GPIO_Port     GPIOA
#define UP_Pin              GPIO_PIN_4
#define UP_GPIO_Port        GPIOC
#define DOWN_Pin            GPIO_PIN_5
#define DOWN_GPIO_Port      GPIOC
#define ENTER_Pin           GPIO_PIN_0
#define ENTER_GPIO_Port     GPIOB
#define LEFT_Pin            GPIO_PIN_5
#define LEFT_GPIO_Port      GPIOA

#define BAT_DETEC_Pin GPIO_PIN_0
#define BAT_DETEC_GPIO_Port GPIOC
#define POWER_CONTROL_Pin GPIO_PIN_1
#define POWER_CONTROL_GPIO_Port GPIOC
#define RECORDING_STOP_Pin GPIO_PIN_2
#define RECORDING_STOP_GPIO_Port GPIOC
#define PLAY_Pin GPIO_PIN_3
#define PLAY_GPIO_Port GPIOC
#define FWD_1_Pin GPIO_PIN_0
#define FWD_1_GPIO_Port GPIOA
#define BWD_1_Pin GPIO_PIN_1
#define BWD_1_GPIO_Port GPIOA
#define FWD_2_Pin GPIO_PIN_2
#define FWD_2_GPIO_Port GPIOA
#define BWD_2_Pin GPIO_PIN_3
#define BWD_2_GPIO_Port GPIOA
#define SHUTDOWN_Pin GPIO_PIN_4
#define SHUTDOWN_GPIO_Port GPIOA

#define PA6_TEMPERATURE_Pin GPIO_PIN_6
#define PA6_TEMPERATURE_GPIO_Port GPIOA

#define LCD_RESET_Pin GPIO_PIN_10
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_11
#define LCD_DC_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_MISO_Pin GPIO_PIN_14
#define LCD_MISO_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_15
#define LCD_MOSI_GPIO_Port GPIOB
#define LCD_LED_Pin GPIO_PIN_6
#define LCD_LED_GPIO_Port GPIOC
#define S1_Pin GPIO_PIN_7
#define S1_GPIO_Port GPIOC
#define S2_Pin GPIO_PIN_8
#define S2_GPIO_Port GPIOC
#define S3_Pin GPIO_PIN_9
#define S3_GPIO_Port GPIOC
#define FWD_3_Pin GPIO_PIN_8
#define FWD_3_GPIO_Port GPIOA
#define BWD_3_Pin GPIO_PIN_9
#define BWD_3_GPIO_Port GPIOA
#define FWD_4_Pin GPIO_PIN_10
#define FWD_4_GPIO_Port GPIOA
#define BWD_4_Pin GPIO_PIN_11
#define BWD_4_GPIO_Port GPIOA
#define S4_Pin GPIO_PIN_12
#define S4_GPIO_Port GPIOA
#define S5_Pin GPIO_PIN_15
#define S5_GPIO_Port GPIOA
#define S6_Pin GPIO_PIN_10
#define S6_GPIO_Port GPIOC
#define S7_Pin GPIO_PIN_11
#define S7_GPIO_Port GPIOC
#define S8_Pin GPIO_PIN_12
#define S8_GPIO_Port GPIOC
#define RTC_SCL_Pin GPIO_PIN_6
#define RTC_SCL_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_7
#define RTC_SDA_GPIO_Port GPIOB

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA


#endif

