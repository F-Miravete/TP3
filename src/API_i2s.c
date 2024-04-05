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
static int32_t * buff_I2S;

/* === Private function declarations =========================================================== */

static int setSizeBuffer(channel * h_ch, uint16_t frequency);
static int setChannel(channel * h_ch);

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
static int setSizeBuffer(channel * h_ch, uint16_t frequency) {
    // if (h_ch != NULL) {
    // if (frequency > FREQ_MAX)
    //     frequency = FREQ_MAX;
    // if (frequency < FREQ_MIN)
    //     frequency = FREQ_MIN;
    uint16_t size_buffer = FREQ_SAMPLING / frequency;
    // if (size_buffer > BUFFER_SIZE_MAX)
    //     size_buffer = BUFFER_SIZE_MAX;
    // if (size_buffer < BUFFER_SIZE_MIN)
    //     size_buffer = BUFFER_SIZE_MIN;
    h_ch->freq = frequency;
    h_ch->size_buffer = size_buffer;
    return 0;
    //} else
    //    return -1;
}

/*
**********************************************************************************************************
Funcion : void setChannel(channel * h_ch)
Funcion que genera las formas de onda seleccionadas
para cada canal. Recibe como parametro un puntero del tipo
channel que contiene los atributos de cada canal
**********************************************************************************************************
*/
static int setChannel(channel * h_ch) {
    // if (h_ch != NULL) {
    uint16_t size_buffer = h_ch->size_buffer;
    for (uint16_t i = 0; i < size_buffer; i++) {
        if (h_ch->wave_type == SINUSOIDAL)
            h_ch->wdata[i] = (h_ch->amplitude / 100.0) * SCALE_SIN_WAVE * 1 *
                             (i * 2 * M_PI / size_buffer); // 1 = sinf
        if (h_ch->wave_type == SAWTOOTH)
            h_ch->wdata[i] = (h_ch->amplitude / 100.0) * i * (SCALE_SAW_WAVE / size_buffer);
    }
    //} else
    //    return -1;
    return 0;
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
int channelsInit(channel * ch0, channel * ch1) {
    if (ch0 == NULL || ch1 == NULL)
        return -1;
    ch_0 = ch0;
    ch_1 = ch1;
    ch_0->amplitude = AMPLITUDE_MAX;
    ch_0->n_ch = CHANNEL_0;
    ch_0->wave_type = SINUSOIDAL;
    ch_0->freq = INITIAL_FREQ;
    ch_1->amplitude = AMPLITUDE_MAX;
    ch_1->n_ch = CHANNEL_1;
    ch_1->wave_type = SAWTOOTH;
    ch_1->freq = INITIAL_FREQ;
    setSizeBuffer(ch_0, INITIAL_FREQ);
    setSizeBuffer(ch_1, INITIAL_FREQ);
    setChannel(ch_0);
    setChannel(ch_1);
    return 0;
}

/*
**********************************************************************************************************
Funcion : void setFreqChannels(uint16_t freq)
Funcion que cambia el valor de frecuencia de un canal y el tamaño de los datos.
Recibe como parametro el puntero al handle del canal y el valor de frecuencia en Hz.
**********************************************************************************************************
*/
int setFreqChannels(channel * h_ch0, channel * h_ch1, uint16_t freq) {
    if (h_ch0 == NULL || h_ch1 == NULL)
        return -1;
    ch_0 = h_ch0;
    ch_1 = h_ch1;
    if (freq > FREQ_MAX)
        freq = FREQ_MAX;
    if (freq < FREQ_MIN)
        freq = FREQ_MIN;
    setSizeBuffer(ch_0, freq);
    setSizeBuffer(ch_1, freq);
    setChannel(ch_0);
    setChannel(ch_1);
    return 0;
}

/*
**********************************************************************************************************
Funcion : void setAmpChannel(channel * h_ch , uint8_t amplitude)
Funcion que modifica la amplitud de un canal.
Recibe como parametro el handle de canal y el valor de amplitud.
**********************************************************************************************************
*/
int setAmpChannel(channel * h_ch, uint8_t amplitude) {
    if (h_ch == NULL)
        return -1;
    if (amplitude > AMPLITUDE_MAX)
        amplitude = AMPLITUDE_MAX;
    if (amplitude < AMPLITUDE_MIN)
        amplitude = AMPLITUDE_MIN;
    uint8_t n_channel = h_ch->n_ch;
    if (n_channel == CHANNEL_0) {
        ch_0 = h_ch;
        ch_0->amplitude = amplitude;
        setChannel(ch_0);
    } else if (n_channel == CHANNEL_1) {
        ch_1 = h_ch;
        ch_1->amplitude = amplitude;
        setChannel(ch_1);
    } else
        return -1;
}

/*
**********************************************************************************************************
Funcion : void setWaveChannel(channel * h_ch, uint8_t amplitude)
Funcion que define el tipo de señal de un canal.
Recibe como parametros el handle de canal y la forma de señal.
**********************************************************************************************************
*/
int setWaveChannel(channel * h_ch, wave_t wave_type) {
    if (h_ch == NULL)
        return -1;
    uint8_t n_channel = h_ch->n_ch;
    if (n_channel == CHANNEL_0) {
        ch_0 = h_ch;
        ch_0->wave_type = wave_type;
        setChannel(ch_0);
    } else if (n_channel == CHANNEL_1) {
        ch_1 = h_ch;
        ch_1->wave_type = wave_type;
        setChannel(ch_1);
    } else
        return -1;
}

/*
**********************************************************************************************************
Funcion : void setBufferI2S(channel * h_ch0 , channel * h_ch1 , int32_t * pBuffI2S)
Funcion que arma el buffer con los datos de los 2 canales para ser enviados por I2S.
Recibe como parametro el handle de cada canal y el puntero al buffer I2S
**********************************************************************************************************
*/
int setBufferI2S(channel * h_ch0, channel * h_ch1, int32_t * pBuffI2S) {
    if (h_ch0 != NULL && h_ch1 != NULL && pBuffI2S != NULL) {
        ch_0 = h_ch0;
        ch_1 = h_ch1;
        buff_I2S = pBuffI2S;
        int32_t aux;
        uint16_t size_buffer = ch_0->size_buffer;
        for (uint16_t i = 0; i < size_buffer; i++) {
            buff_I2S[i] = ch_0->wdata[i];
            aux = buff_I2S[i] << 16;
            buff_I2S[i] = aux;
            buff_I2S[i] = buff_I2S[i] + (int32_t)ch_1->wdata[i];
        }
    } else
        return -1;
    return 0;
}

/* === End of documentation ==================================================================== */
