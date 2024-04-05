/****************************************************************************************
Copyright (c) 2024, Flavio Miravete <flavio.miravete@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
****************************************************************************************/

/** @file
 *  @brief TP Final PdM y PCSE (CESE 2023)
 *         Titulo: Modulo API_i2s (HEADER)
 *
 */

#ifndef API_INC_API_I2S_H_
#define API_INC_API_I2S_H_

/* === Headers files inclusions ====================================================== */

#include <stdbool.h>
#include <stdint.h>

/* === Public Macros definitions ===================================================== */

#define FREQ_SAMPLING   96000
#define FREQ_MAX        24000
#define FREQ_MIN        20
#define BUFFER_SIZE_MAX 4800
#define BUFFER_SIZE_MIN 4
#define SCALE_SIN_WAVE  16383
#define SCALE_SAW_WAVE  32767
#define CHANNEL_0       0
#define CHANNEL_1       1
#define NULL            ((void *)0)

/* === Public data type declarations ================================================ */

typedef enum { SINUSOIDAL, SAWTOOTH } wave_t;

typedef struct {
    uint8_t n_ch;                   // 0 o 1
    wave_t wave_type;               // SINUSOIDAL o SAWTOOTH
    uint8_t amplitude;              // 0 to 100 [%]
    uint16_t freq;                  // 20 to 24000 [Hz]
    uint16_t size_buffer;           // 4 to 4800
    int16_t wdata[BUFFER_SIZE_MAX]; // vector que contiene la forma de onda
} channel;

/* === Public variable declarations ================================================= */

/* === Public function declarations ================================================= */

/**
 * @brief  Inicializa canales
 *
 * @param  - handle de canal 0 y canal 1
 * @return - 0 = OK o -1 = ERROR
 */
int channelsInit(channel * ch0, channel * ch1);

/**
 * @brief  Setea un nuevo valor de frecuencia en ambos canales (0 y 1)
 *
 * @param  channel * h_ch0 : handle de canal 0
 *         channel * h_ch1 : handle de canal 1
 *          uint16_t freq : valor de frecuencia en Hz
 * @return - 0 = OK o -1 = ERROR
 */
int setFreqChannels(channel * h_ch0, channel * h_ch1, uint16_t freq);

/**
 * @brief  Setea un nuevo valor de amplitud en un canal
 *
 * @param  channel * h_ch : handle de canal
 *         uint8_t amplitude: valor de amplitud en % (0 a 100 %)
 * @return - 0 = OK o -1 = ERROR
 */
int setAmpChannel(channel * h_ch, uint8_t amplitude);

/**
 * @brief  Setea nueva forma de onda en un canal
 *
 * @param  channel * h_ch : handle de canal
 *         wave_t wave_type : forma de onda
 * @return - 0 = OK o -1 = ERROR
 */
int setWaveChannel(channel * h_ch, wave_t wave_type);

/**
 * @brief  Arma buffer para enviar datos I2S de los 2 canales
 *
 * @param  - handle de canal 0 y canal 1 y
 * @return - 0 = OK o -1 = ERROR
 */
int setBufferI2S(channel * h_ch0, channel * h_ch1, int32_t * pBufferI2S);

/* === End of documentation ========================================================== */

#endif /* API_INC_API_I2S_H_ */
