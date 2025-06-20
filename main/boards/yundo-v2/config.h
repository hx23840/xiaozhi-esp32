#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// 音频采样率配置
#define AUDIO_INPUT_SAMPLE_RATE  16000
#define AUDIO_OUTPUT_SAMPLE_RATE 16000

// 麦克风 ICS43434 I2S接口 (I2S0)
#define AUDIO_I2S_MIC_GPIO_SCK  GPIO_NUM_4  // SCK
#define AUDIO_I2S_MIC_GPIO_WS   GPIO_NUM_5  // WS
#define AUDIO_I2S_MIC_GPIO_DIN  GPIO_NUM_6  // SD

// 功放 MAX98357 I2S接口 (I2S1)
#define AUDIO_I2S_SPK_GPIO_BCLK GPIO_NUM_7  // SCK
#define AUDIO_I2S_SPK_GPIO_LRCK GPIO_NUM_8  // WS
#define AUDIO_I2S_SPK_GPIO_DOUT GPIO_NUM_1  // SD

// 按钮和LED配置
#define BUILTIN_LED_GPIO        GPIO_NUM_48  // 根据实际情况调整
#define BOOT_BUTTON_GPIO        GPIO_NUM_0   // BOOT按钮
#define RECORD_BUTTON_GPIO      GPIO_NUM_2   // 录音按钮，根据实际调整

// 电源管理引脚
#define BATTERY_ADC_PIN         ADC1_CHANNEL_0  // 电池电量检测引脚，根据实际调整

#endif // _BOARD_CONFIG_H_ 