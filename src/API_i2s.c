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

static int32_t dataBufferI2S[BUFFER_SIZE_MAX];
static int16_t dataChannel[QUANT_CHANNELS][BUFFER_SIZE_MAX];
static channel channel_0, channel_1;
static uint32_t size_buffer;

/* === Private function declarations =========================================================== */

static void channelsInit(void);
static void setChannel(channel * h_ch);
static void setSizeBuffer(uint16_t frequency);
static void setBufferI2S();
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
static void setSizeBuffer(uint16_t frequency) {
    size_buffer = FREQ_SAMPLING / frequency;
    if (size_buffer > BUFFER_SIZE_MAX)
        size_buffer = BUFFER_SIZE_MAX;
    if (size_buffer < BUFFER_SIZE_MIN)
        size_buffer = BUFFER_SIZE_MIN;
    channel_0.freq = frequency;
    channel_1.freq = frequency;
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
    if (h_ch != NULL) {
        for (uint16_t i = 0; i < size_buffer; i++) {
            if (h_ch->wave_type == SINUSOIDAL)
                dataChannel[h_ch->n_ch][i] =
                    (h_ch->amplitude / 100.0) * SCALE_SIN_WAVE * sinf(i * 2 * M_PI / size_buffer);
            if (h_ch->wave_type == SAWTOOTH)
                dataChannel[h_ch->n_ch][i] =
                    (h_ch->amplitude / 100.0) * i * (SCALE_SAW_WAVE / size_buffer);
        }
    } else
        Error_Handler();
}

/*
**********************************************************************************************************
Funcion : void setBufferI2S()
Funcion que arma el buffer con los datos de los 2
canales para ser enviados por I2S
**********************************************************************************************************
*/
static void setBufferI2S() {
    int32_t aux;
    for (uint16_t i = 0; i < size_buffer; i++) {
        dataBufferI2S[i] = dataChannel[CHANNEL_0][i];
        aux = dataBufferI2S[i] << 16;
        dataBufferI2S[i] = aux;
        dataBufferI2S[i] = dataBufferI2S[i] + (int32_t)dataChannel[CHANNEL_1][i];
    }
}

/*
**********************************************************************************************************
Funcion : void channelsInit(void)
Funcion que inicializa los canales del generador de onda.
Canal 0 -> Sinusoidal, 1000Hz, Amplitud 100%
Canal 1 -> Sawtooth, 1000Hz, Amplitud 100%
**********************************************************************************************************
*/
static void channelsInit(void) {
    channel_0.amplitude = AMPLITUDE_MAX;
    channel_0.n_ch = CHANNEL_0;
    channel_0.wave_type = SINUSOIDAL;
    channel_0.freq = INITIAL_FREQ;
    channel_1.amplitude = AMPLITUDE_MAX;
    channel_1.n_ch = CHANNEL_1;
    channel_1.wave_type = SAWTOOTH;
    channel_1.freq = INITIAL_FREQ;
    setSizeBuffer(INITIAL_FREQ);
    setChannel(&channel_0);
    setChannel(&channel_1);
    setBufferI2S();
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
Funcion : channel* readChannelProperty(uint8_t ch)
Funcion que devuelve un puntero a la estructura que define los parametros de cada canal.
Recibe como parametro el numero de canal.
**********************************************************************************************************
*/
channel * readChannelProperty(uint8_t ch) {
    channel * RET;
    if (ch == CHANNEL_0)
        RET = &channel_0;
    else if (ch == CHANNEL_1)
        RET = &channel_1;
    else
        RET = NULL;
    return RET;
}

/*
**********************************************************************************************************
Funcion : void setFreqChannels(uint16_t freq)
Funcion que ajusta el tamaño de los datos de cada canal y el buffer I2S para un valor
de frecuencia determinado.
Recibe como parametro el valor de frecuencia en Hz.
**********************************************************************************************************
*/
void setFreqChannels(uint16_t freq) {
    if (freq > FREQ_MAX)
        freq = FREQ_MAX;
    if (freq < FREQ_MIN)
        freq = FREQ_MIN;
    setSizeBuffer(freq);
    setChannel(&channel_0);
    setChannel(&channel_1);
    setBufferI2S();
}

/*
**********************************************************************************************************
Funcion : void setAmpChannel(uint8_t n_channel, uint8_t amplitude)
Funcion que modifica la amplitud de un canal.
Recibe como parametro el numero de canal y el valor de amplitud.
**********************************************************************************************************
*/
void setAmpChannel(uint8_t n_channel, uint8_t amplitude) {
    if (n_channel != CHANNEL_0 && n_channel != CHANNEL_1)
        Error_Handler();
    if (amplitude > AMPLITUDE_MAX)
        amplitude = AMPLITUDE_MAX;
    if (amplitude < AMPLITUDE_MIN)
        amplitude = AMPLITUDE_MIN;

    if (n_channel == CHANNEL_0) {
        channel_0.amplitude = amplitude;
        setChannel(&channel_0);
    }
    if (n_channel == CHANNEL_1) {
        channel_1.amplitude = amplitude;
        setChannel(&channel_1);
    }
    setBufferI2S();
}

/*
**********************************************************************************************************
Funcion : void setWaveChannel(uint8_t n_channel, uint8_t amplitude)
Funcion que define el tipo de señal de un canal.
Recibe como parametros el numero de canal y la forma de señal.
**********************************************************************************************************
*/
void setWaveChannel(uint8_t n_channel, wave_t wave_type) {
    if (n_channel != CHANNEL_0 && n_channel != CHANNEL_1)
        Error_Handler();

    if (n_channel == CHANNEL_0) {
        channel_0.wave_type = wave_type;
        setChannel(&channel_0);
    }
    if (n_channel == CHANNEL_1) {
        channel_1.wave_type = wave_type;
        setChannel(&channel_1);
    }
    setBufferI2S();
}

/* === End of documentation ==================================================================== */
