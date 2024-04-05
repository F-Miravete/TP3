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

#define TEST_FREQ_SAMPLING   96000
#define TEST_FREQ_MAX        24000
#define TEST_FREQ_MIN        20
#define TEST_BUFFER_SIZE_MAX 4800
#define TEST_BUFFER_SIZE_MIN 4
#define TEST_SCALE_SIN_WAVE  16383
#define TEST_SCALE_SAW_WAVE  32767
#define TEST_CHANNEL_0       0
#define TEST_CHANNEL_1       1
#define RETURN_ERROR         -1
#define RETURN_OK            0
#define TEST_INITIAL_FREQ    1000
#define TEST_AMPLITUDE_MAX   100
#define TEST_AMPLITUDE_MIN   0

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

static channel T_channel_0, T_channel_1;
static int32_t T_bufferI2S[TEST_BUFFER_SIZE_MAX];

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

/**
 * @brief Test 1.1
 *        Verificar puntero valido en la inicializacion de canales
 *
 * @param  -
 * @return -
 */
void test_creacion_correcta_de_canales(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, channelsInit((void *)0, &T_channel_1));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, channelsInit(&T_channel_0, (void *)0));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, channelsInit((void *)0, (void *)0));
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
}
/**
 * @brief Test 1.2
 *        Verificar la inicializacion de valores de los canales
 *
 * @param  -
 * @return -
 */
void test_inicializacion_valores_de_canales(void) {
    // Cargo valores distintos a los iniciales en canal 0
    T_channel_0.amplitude = 1;
    T_channel_0.freq = 1;
    T_channel_0.n_ch = 1;
    T_channel_0.size_buffer = 1;
    T_channel_0.wave_type = 1;
    for (uint16_t i = 0; i < TEST_BUFFER_SIZE_MAX; i++)
        T_channel_0.wdata[i] = 0;

    // Cargo valores distintos a los iniciales en canal 1
    T_channel_1.amplitude = 2;
    T_channel_1.freq = 2;
    T_channel_1.n_ch = 2;
    T_channel_1.size_buffer = 2;
    T_channel_1.wave_type = 2;
    for (uint16_t i = 0; i < TEST_BUFFER_SIZE_MAX; i++)
        T_channel_1.wdata[i] = 0;

    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));

    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_0.amplitude);
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT8(TEST_CHANNEL_0, T_channel_0.n_ch);
    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_SAMPLING / TEST_INITIAL_FREQ, T_channel_0.size_buffer);
    TEST_ASSERT_EQUAL(SINUSOIDAL, T_channel_0.wave_type);
    bool flag_0 = false;
    for (uint16_t i = 0; i < TEST_BUFFER_SIZE_MAX; i++) {
        if (T_channel_0.wdata[i] != 0)
            flag_0 = true;
    }
    TEST_ASSERT_TRUE(flag_0);

    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_1.amplitude);
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_1.freq);
    TEST_ASSERT_EQUAL_UINT8(TEST_CHANNEL_1, T_channel_1.n_ch);
    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_SAMPLING / TEST_INITIAL_FREQ, T_channel_1.size_buffer);
    TEST_ASSERT_EQUAL(SAWTOOTH, T_channel_1.wave_type);
    bool flag_1 = false;
    for (uint16_t i = 0; i < TEST_BUFFER_SIZE_MAX; i++) {
        if (T_channel_1.wdata[i] != 0)
            flag_1 = true;
    }
    TEST_ASSERT_TRUE(flag_1);
}

/**
 * @brief Test 2.1
 *        Verificar puntero valido en cambio de frecuencia de ambos canales
 *
 * @param  -
 * @return -
 */
void test_verificar_puntero_cambio_de_frecuencia_canales(void) {
    uint16_t test_frequency = 2500;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setFreqChannels((void *)0, &T_channel_1, test_frequency));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setFreqChannels(&T_channel_0, (void *)0, test_frequency));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setFreqChannels((void *)0, (void *)0, test_frequency));

    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_1.freq);
}

/**
 * @brief Test 2.2
 *        Verificar el cambio de frecuencia de ambos canales
 *
 * @param  -
 * @return -
 */
void test_cambio_de_frecuencia_canales(void) {
    uint16_t test_frequency = 2500;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_1.freq);

    TEST_ASSERT_EQUAL_INT(RETURN_OK, setFreqChannels(&T_channel_0, &T_channel_1, test_frequency));

    TEST_ASSERT_EQUAL_UINT16(test_frequency, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(test_frequency, T_channel_1.freq);
}

/**
 * @brief Test 2.3
 *        Verificar el cambio de frecuencia con valores prohibidos
 *
 * @param  -
 * @return -
 */
void test_cambio_de_frecuencia_con_valores_prohibidos(void) {
    uint16_t test_frequency_1 = 25000;
    uint16_t test_frequency_2 = 12;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(TEST_INITIAL_FREQ, T_channel_1.freq);

    TEST_ASSERT_EQUAL_INT(RETURN_OK, setFreqChannels(&T_channel_0, &T_channel_1, test_frequency_1));

    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_MAX, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_MAX, T_channel_1.freq);

    TEST_ASSERT_EQUAL_INT(RETURN_OK, setFreqChannels(&T_channel_0, &T_channel_1, test_frequency_2));

    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_MIN, T_channel_0.freq);
    TEST_ASSERT_EQUAL_UINT16(TEST_FREQ_MIN, T_channel_1.freq);
}

/**
 * @brief Test 3.1
 *        Verificar puntero valido al llamar a la funcion cambio de amplitud
 *
 * @param  -
 * @return -
 */
void test_puntero_valido_al_llamar_cambio_de_amplitud_canal(void) {
    uint8_t test_amplitude = 33;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));

    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setAmpChannel((void *)0, test_amplitude));
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_0.amplitude);
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_1.amplitude);
}

/**
 * @brief Test 3.2
 *        Verificar el cambio de amplitud del canal distinto a los permitidos (0 y 1)
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_distinto(void) {
    uint8_t test_amplitude = 33;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    T_channel_0.n_ch = 2;
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setAmpChannel(&T_channel_0, test_amplitude));
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_0.amplitude);
}

/**
 * @brief Test 3.3
 *        Verificar el cambio de amplitud del canal 0
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_0(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_0.amplitude);
    uint8_t test_amplitude = 33;
    setAmpChannel(&T_channel_0, test_amplitude);
    TEST_ASSERT_EQUAL_UINT8(test_amplitude, T_channel_0.amplitude);
}

/**
 * @brief Test 3.4
 *        Verificar el cambio de amplitud del canal 1
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_1(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_1.amplitude);
    uint8_t test_amplitude = 55;
    setAmpChannel(&T_channel_1, test_amplitude);
    TEST_ASSERT_EQUAL_UINT8(test_amplitude, T_channel_1.amplitude);
}

/**
 * @brief Test 3.5
 *        Verificar el cambio de amplitud del canal con valores prohibidos
 *
 * @param  -
 * @return -
 */
void test_cambio_de_amplitud_canal_para_valores_prohibidos(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_0.amplitude);
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_1.amplitude);
    uint8_t test_amplitude = 105;
    setAmpChannel(&T_channel_1, test_amplitude);
    TEST_ASSERT_EQUAL_UINT8(TEST_AMPLITUDE_MAX, T_channel_1.amplitude);
}

/**
 * @brief Test 4.1
 *        Verificar puntero valido al llamar a la funcion cambio de tipo de onda
 *
 * @param  -
 * @return -
 */
void test_puntero_valido_al_llamar_cambio_tipo_de_onda(void) {
    wave_t test_wave = SAWTOOTH;
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));

    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setWaveChannel((void *)0, test_wave));
    TEST_ASSERT_EQUAL_UINT8(SINUSOIDAL, T_channel_0.wave_type);
    TEST_ASSERT_EQUAL_UINT8(SAWTOOTH, T_channel_1.wave_type);
}

/**
 * @brief Test 4.2
 *        Verificar el cambio de forma de onda del canal 0
 *
 * @param  -
 * @return -
 */
void test_cambio_de_forma_de_onda_canal_0(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(SINUSOIDAL, T_channel_0.wave_type);
    wave_t test_wave = SAWTOOTH;
    setWaveChannel(&T_channel_0, test_wave);
    TEST_ASSERT_EQUAL_UINT8(test_wave, T_channel_0.wave_type);
}

/**
 * @brief Test 4.3
 *        Verificar el cambio de forma de onda del canal 1
 *
 * @param  -
 * @return -
 */
void test_cambio_de_forma_de_onda_canal_1(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(SAWTOOTH, T_channel_1.wave_type);
    wave_t test_wave = SINUSOIDAL;
    setWaveChannel(&T_channel_1, test_wave);
    TEST_ASSERT_EQUAL_UINT8(test_wave, T_channel_1.wave_type);
}

/**
 * @brief Test 4.4
 *        Verificar el cambio de forma de onda con valores de canal prohibidos
 *
 * @param  -
 * @return -
 */
void test_cambio_de_forma_de_onda_canal_con_valor_prohibido(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_UINT8(SAWTOOTH, T_channel_1.wave_type);
    uint8_t n_channel = 4;
    T_channel_1.n_ch = n_channel;
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setWaveChannel(&T_channel_1, SINUSOIDAL));
    TEST_ASSERT_EQUAL_UINT8(SAWTOOTH, T_channel_1.wave_type);
}

/**
 * @brief Test 5.1
 *        Verificar punteros validos en la llamada al armado del buffer I2S
 *
 * @param  -
 * @return -
 */
void test_chequeo_punteros_validos_armado_buffer_I2S(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setBufferI2S((void *)0, &T_channel_1, T_bufferI2S));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setBufferI2S(&T_channel_0, (void *)0, T_bufferI2S));
    TEST_ASSERT_EQUAL_INT(RETURN_ERROR, setBufferI2S(&T_channel_0, &T_channel_1, (void *)0));
}

/**
 * @brief Test 5.2
 *        Verificar el el armado del buffer I2S con los 2 canales
 *
 * @param  -
 * @return -
 */
void test_chequeo_armado_buffer_I2S(void) {
    TEST_ASSERT_EQUAL_INT(RETURN_OK, channelsInit(&T_channel_0, &T_channel_1));
    for (uint16_t i = 0; i < TEST_BUFFER_SIZE_MAX; i++)
        T_bufferI2S[i] = 0;
    setBufferI2S(&T_channel_0, &T_channel_1, T_bufferI2S);
    bool flag = false;
    for (uint16_t i = 0; i < BUFFER_SIZE_MAX; i++) {
        if (T_bufferI2S[i] != 0)
            flag = true;
    }
    TEST_ASSERT_TRUE(flag);
}
