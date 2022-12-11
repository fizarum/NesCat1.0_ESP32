#ifndef AUDIO_H
#define AUDIO_H

// AUDIO_i2S:
#define I2S_BCK_IO (GPIO_NUM_27)  // BCK
#define I2S_WS_IO (GPIO_NUM_32)   // LCK
#define I2S_DO_IO (GPIO_NUM_25)   // DIN
#define I2S_DI_IO (-1)

#define SOUND_ENABLED true
#define DEFAULT_SAMPLE_RATE 24000

#endif  // AUDIO_H