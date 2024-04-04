/************************************************************************************************
Copyright (c) 2024, Flavio Miravete <flavio.miravete@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file
 ** @brief Capa de abstracción para manejo generador de ondas
 **/

/* === Headers files inclusions =============================================================== */

#include "API_i2s.h"
#include <math.h>
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

#define INITIAL_FREQ   1000
#define AMPLITUDE_MAX  100
#define AMPLITUDE_MIN  0
#define QUANT_CHANNELS 2

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

static channel * ch_0;
static channel * ch_1;

/* === Private function declarations =========================================================== */

static void setChannel(channel * h_ch);
static void setSizeBuffer(channel * h_ch, uint16_t frequency);
static void Error_Handler(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/*
**********************************************************************************************************
Funcion : void setSizeBuffer(uint16_t frequency)
Funcion que calcula el tamaño del buffer que sera
enviado por I2S dependiendo de la frecuencia de la señal.
Recibe como parametro el valor de frecuencia en Hz
**********************************************************************************************************
*/
static void setSizeBuffer(channel * h_ch, uint16_t frequency) {
    uint16_t size_buffer = FREQ_SAMPLING / frequency;
    if (size_buffer > BUFFER_SIZE_MAX)
        size_buffer = BUFFER_SIZE_MAX;
    if (size_buffer < BUFFER_SIZE_MIN)
        size_buffer = BUFFER_SIZE_MIN;
    h_ch->freq = frequency;
    h_ch->size_buffer = size_buffer;
}

/*
**********************************************************************************************************
Funcion : void setChannel(channel * h_ch)
Funcion que genera las formas de onda seleccionadas
para cada canal. Recibe como parametro un puntero del tipo
channel que contiene los atributos de cada canal
**********************************************************************************************************
*/
static void setChannel(channel * h_ch) {
    uint16_t size_buffer = h_ch->size_buffer;
    if (h_ch != NULL) {
        for (uint16_t i = 0; i < size_buffer; i++) {
            if (h_ch->wave_type == SINUSOIDAL)
                h_ch->wdata[i] = (h_ch->amplitude / 100.0) * SCALE_SIN_WAVE * 1 *
                                 (i * 2 * M_PI / size_buffer); // 1 = sinf
            if (h_ch->wave_type == SAWTOOTH)
                h_ch->wdata[i] = (h_ch->amplitude / 100.0) * i * (SCALE_SAW_WAVE / size_buffer);
        }
    } else
        Error_Handler();
}

/*
**********************************************************************************************************
* @brief  This function is executed in case of error occurrence.
**********************************************************************************************************
*/
void Error_Handler(void) {

    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

/* === Public function implementation ========================================================== */

/*
**********************************************************************************************************
Funcion : void channelsInit(void)
Funcion que inicializa los canales del generador de onda.
Canal 0 -> Sinusoidal, 1000Hz, Amplitud 100%
Canal 1 -> Sawtooth, 1000Hz, Amplitud 100%
**********************************************************************************************************
*/
void channelsInit(channel * ch0, channel * ch1) {
    ch_0 = ch0;
    ch_1 = ch1;
    ch0->amplitude = AMPLITUDE_MAX;
    ch0->n_ch = CHANNEL_0;
    ch0->wave_type = SINUSOIDAL;
    ch0->freq = INITIAL_FREQ;
    ch1->amplitude = AMPLITUDE_MAX;
    ch1->n_ch = CHANNEL_1;
    ch1->wave_type = SAWTOOTH;
    ch1->freq = INITIAL_FREQ;
    setSizeBuffer(ch0, INITIAL_FREQ);
    setSizeBuffer(ch1, INITIAL_FREQ);
    setChannel(ch0);
    setChannel(ch1);
}

/*
**********************************************************************************************************
Funcion : void setFreqChannels(uint16_t freq)
Funcion que cambia el valor de frecuencia de un canal y el tamaño de los datos.
Recibe como parametro el puntero al handle del canal y el valor de frecuencia en Hz.
**********************************************************************************************************
*/
void setFreqChannels(channel * h_ch0, channel * h_ch1, uint16_t freq) {
    if (freq > FREQ_MAX)
        freq = FREQ_MAX;
    if (freq < FREQ_MIN)
        freq = FREQ_MIN;
    setSizeBuffer(h_ch0, freq);
    setSizeBuffer(h_ch1, freq);
    setChannel(h_ch0);
    setChannel(h_ch1);
}

/*
**********************************************************************************************************
Funcion : void setAmpChannel(channel * h_ch , uint8_t amplitude)
Funcion que modifica la amplitud de un canal.
Recibe como parametro el handle de canal y el valor de amplitud.
**********************************************************************************************************
*/
void setAmpChannel(channel * h_ch, uint8_t amplitude) {
    uint8_t n_channel = h_ch->n_ch;
    if (n_channel != CHANNEL_0 && n_channel != CHANNEL_1)
        Error_Handler();
    if (amplitude > AMPLITUDE_MAX)
        amplitude = AMPLITUDE_MAX;
    if (amplitude < AMPLITUDE_MIN)
        amplitude = AMPLITUDE_MIN;
    h_ch->amplitude = amplitude;
    setChannel(h_ch);
}

/*
**********************************************************************************************************
Funcion : void setWaveChannel(channel * h_ch, uint8_t amplitude)
Funcion que define el tipo de señal de un canal.
Recibe como parametros el handle de canal y la forma de señal.
**********************************************************************************************************
*/
void setWaveChannel(channel * h_ch, wave_t wave_type) {
    uint8_t n_channel = h_ch->n_ch;
    if (n_channel != CHANNEL_0 && n_channel != CHANNEL_1)
        Error_Handler();
    h_ch->wave_type = wave_type;
    setChannel(h_ch);
}

/*
**********************************************************************************************************
Funcion : void setBufferI2S(channel * h_ch0 , channel * h_ch1 , int32_t * pBuffI2S)
Funcion que arma el buffer con los datos de los 2 canales para ser enviados por I2S.
Recibe como parametro el handle de cada canal y el puntero al buffer I2S
**********************************************************************************************************
*/
void setBufferI2S(channel * h_ch0, channel * h_ch1, int32_t * pBuffI2S) {
    int32_t aux;
    uint16_t size_buffer = h_ch0->size_buffer;
    for (uint16_t i = 0; i < size_buffer; i++) {
        pBuffI2S[i] = h_ch0->wdata[i];
        aux = pBuffI2S[i] << 16;
        pBuffI2S[i] = aux;
        pBuffI2S[i] = pBuffI2S[i] + (int32_t)h_ch1->wdata[i];
    }
}

/* === End of documentation ==================================================================== */
