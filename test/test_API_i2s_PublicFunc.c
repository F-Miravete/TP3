/************************************************************************************************
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
*************************************************************************************************/

/** @file
 *  @brief Modulo de Testeos para funciones publicas del driver API_i2s (ceedling)
 *         Funciones en prueba:
 *         - void channelsInit(void)
 *         - channel * readChannelProperty(uint8_t ch)
 *         - void setFreqChannels(uint16_t freq)
 *         - void setAmpChannel(uint8_t n_channel, uint8_t amplitude)
 *         - void setWaveChannel(uint8_t n_channel, wave_t wave_type)
 */

/* === Headers files inclusions =============================================================== */

#include "unity.h"
#include "API_i2s.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

channel T_channel_0, T_channel_1;
int32_t bufferI2S[BUFFER_SIZE_MAX];

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

/**
 * @brief Test 1
 *        Verificar la inicializacion correcta de los canales
 *
 * @param  -
 * @return -
 */
void test_inicializacion_canales_driver_I2S(void) {
    // Cargo valores distintos a los iniciales en canal 0
    T_channel_0.amplitude = 1;
    T_channel_0.freq = 1;
    T_channel_0.n_ch = 1;
    T_channel_0.size_buffer = 1;
    T_channel_0.wave_type = 1;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++)
        T_channel_0.wdata[i] = 0;

    // Cargo valores distintos a los iniciales en canal 1
    T_channel_1.amplitude = 2;
    T_channel_1.freq = 2;
    T_channel_1.n_ch = 2;
    T_channel_1.size_buffer = 2;
    T_channel_1.wave_type = 2;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++)
        T_channel_1.wdata[i] = 0;

    channelsInit(&T_channel_0, &T_channel_1);

    TEST_ASSERT_EQUAL_UINT8(100, T_channel_0.amplitude);
    TEST_ASSERT_EQUAL_UINT16(1000, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT8(0, T_channel_0.n_ch);
    TEST_ASSERT_EQUAL_UINT16(96000 / 1000, T_channel_0.size_buffer);
    TEST_ASSERT_EQUAL(0, T_channel_0.wave_type);
    bool flag_0 = false;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++) {
        if (T_channel_0.wdata[i] != 0)
            flag_0 = true;
    }
    TEST_ASSERT_TRUE(flag_0);

    TEST_ASSERT_EQUAL_UINT8(100, T_channel_1.amplitude);
    TEST_ASSERT_EQUAL_UINT16(1000, T_channel_1.freq);
    TEST_ASSERT_EQUAL_UINT8(1, T_channel_1.n_ch);
    TEST_ASSERT_EQUAL_UINT16(96000 / 1000, T_channel_1.size_buffer);
    TEST_ASSERT_EQUAL(1, T_channel_1.wave_type);
    bool flag_1 = false;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++) {
        if (T_channel_1.wdata[i] != 0)
            flag_1 = true;
    }
    TEST_ASSERT_TRUE(flag_1);
}

/**
 * @brief Test 2
 *        Verificar el cambio de frecuencia de un canal
 *
 * @param  -
 * @return -
 */
void test_cambio_de_frecuencia_canales(void) {
    TEST_ASSERT_EQUAL_UINT16(1000, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(1000, T_channel_1.freq);
    uint16_t test_frequency = 2500;
    setFreqChannels(&T_channel_0, &T_channel_1, test_frequency);
    TEST_ASSERT_EQUAL_UINT16(test_frequency, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(test_frequency, T_channel_1.freq);
}

/**
 * @brief Test 3
 *        Verificar el cambio de amplitud del canal 0
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_0(void) {
    TEST_ASSERT_EQUAL_UINT8(100, T_channel_0.amplitude);
    uint8_t test_amplitude = 33;
    setAmpChannel(&T_channel_0, test_amplitude);
    TEST_ASSERT_EQUAL_UINT8(test_amplitude, T_channel_0.amplitude);
}

/**
 * @brief Test 4
 *        Verificar el cambio de amplitud del canal 1
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_1(void) {
    TEST_ASSERT_EQUAL_UINT8(100, T_channel_1.amplitude);
    uint8_t test_amplitude = 55;
    setAmpChannel(&T_channel_1, test_amplitude);
    TEST_ASSERT_EQUAL_UINT8(test_amplitude, T_channel_1.amplitude);
}

/**
 * @brief Test 5
 *        Verificar el cambio de forma de onda del canal 0
 *
 * @param  -
 * @return -
 */
void test_cambio_de_forma_de_onda_canal_0(void) {
    TEST_ASSERT_EQUAL_UINT8(SINUSOIDAL, T_channel_0.wave_type);
    wave_t test_wave = SAWTOOTH;
    setWaveChannel(&T_channel_0, test_wave);
    TEST_ASSERT_EQUAL_UINT8(test_wave, T_channel_0.wave_type);
}

/**
 * @brief Test 6
 *        Verificar el cambio de forma de onda del canal 1
 *
 * @param  -
 * @return -
 */
void test_cambio_de_forma_de_onda_canal_1(void) {
    TEST_ASSERT_EQUAL_UINT8(SAWTOOTH, T_channel_1.wave_type);
    wave_t test_wave = SINUSOIDAL;
    setWaveChannel(&T_channel_1, test_wave);
    TEST_ASSERT_EQUAL_UINT8(test_wave, T_channel_1.wave_type);
}

/**
 * @brief Test 7
 *        Verificar el el armado del buffer I2S con los 2 canales
 *
 * @param  -
 * @return -
 */
void test_chequeo_armado_buffer_I2S(void) {
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++)
        bufferI2S[i] = 0;
    setBufferI2S(&T_channel_0, &T_channel_1, bufferI2S);
    bool flag = false;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++) {
        if (bufferI2S[i] != 0)
            flag = true;
    }
    TEST_ASSERT_TRUE(flag);
}
