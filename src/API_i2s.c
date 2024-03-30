//**********************************************************************************************************
//
// TP Final PdM y PCSE (CESE 2023)
// Titulo: Modulo API_i2s (SOURCE)
// Autor: F.D.M.
//
//**********************************************************************************************************

#include "API_i2s.h"
#include <math.h>
#include <stdbool.h>

#define INITIAL_FREQ 1000
#define AMPLITUDE_MAX 100
#define AMPLITUDE_MIN 0
#define QUANT_CHANNELS 2

static void channelsInit(void);
static void setChannel(channel *h_ch);
static void setSizeBuffer(uint16_t frequency);
static void setBufferI2S();
static void Error_Handler(void);

static int32_t dataBufferI2S[BUFFER_SIZE_MAX];
static int16_t dataChannel[QUANT_CHANNELS][BUFFER_SIZE_MAX];
static channel channel_0, channel_1;
static uint32_t size_buffer;

/*

//**********************************************************************************************************
// Funcion : bool_t i2sInit()
//			 Funcion que inicializa el periferico I2S2
//			 Retorno True si la inicializacion fue exitosa
//**********************************************************************************************************
bool i2sInit() {
  bool flag_RET = true;
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_MSB;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_96K; // I2S_AUDIOFREQ_48K -
I2S_AUDIOFREQ_96K hi2s2.Init.CPOL = I2S_CPOL_LOW; hi2s2.Init.ClockSource =
I2S_CLOCK_PLL; hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE; if
(HAL_I2S_Init(&hi2s2) != HAL_OK) flag_RET = false; channelsInit(); return
flag_RET;
}

*/

//**********************************************************************************************************
// Funcion : void setSizeBuffer(uint16_t frequency)
//			 Funcion que calcula el tamaño del buffer que sera
// enviado por I2S dependiendo de la frecuencia de la señal
// Recibe como parametro el valor de frecuencia en Hz
//**********************************************************************************************************
static void setSizeBuffer(uint16_t frequency) {
  size_buffer = FREQ_SAMPLING / frequency;
  if (size_buffer > BUFFER_SIZE_MAX)
    size_buffer = BUFFER_SIZE_MAX;
  if (size_buffer < BUFFER_SIZE_MIN)
    size_buffer = BUFFER_SIZE_MIN;
  channel_0.freq = frequency;
  channel_1.freq = frequency;
}

//**********************************************************************************************************
// Funcion : void setChannel(channel * h_ch)
//			 Funcion que genera las formas de onda seleccionadas
// para
// cada canal 			 Recibe como parametro un puntero del tipo
// channel que contiene los atributos de cada canal
//**********************************************************************************************************
static void setChannel(channel *h_ch) {
  if (h_ch != NULL) {
    for (uint16_t i = 0; i < size_buffer; i++) {
      if (h_ch->wave_type == SINUSOIDAL)
        dataChannel[h_ch->n_ch][i] =
            (h_ch->amplitude / 100.0) * i * (SCALE_SIN_WAVE / size_buffer);
      if (h_ch->wave_type == SAWTOOTH)
        dataChannel[h_ch->n_ch][i] =
            (h_ch->amplitude / 100.0) * i * (SCALE_SAW_WAVE / size_buffer);
    }
  } else
    Error_Handler();
}

//**********************************************************************************************************
// Funcion : void setBufferI2S()
//			 Funcion que arma el buffer con los datos de los 2
// canales para ser enviados por I2S
//
//**********************************************************************************************************
static void setBufferI2S() {
  int32_t aux;
  for (uint16_t i = 0; i < size_buffer; i++) {
    dataBufferI2S[i] = dataChannel[CHANNEL_0][i];
    aux = dataBufferI2S[i] << 16;
    dataBufferI2S[i] = aux;
    dataBufferI2S[i] = dataBufferI2S[i] + (int32_t)dataChannel[CHANNEL_1][i];
  }
}

//**********************************************************************************************************
// Funcion : void channelsInit(void)
//			 Funcion que inicializa los canales del generador de
// onda 			 Canal 0 -> Sinusoidal, 1000Hz, Amplitud 100%
// Canal 1 -> Sawtooth, 1000Hz, Amplitud 100%
//**********************************************************************************************************
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

//**********************************************************************************************************
// Funcion : channel* readChannelProperty(uint8_t ch)
//			 Funcion que devuelve un puntero a la estructura que
// define los parametros de cada canal 			 Recibe como parametro
// el numero de canal
//**********************************************************************************************************
channel *readChannelProperty(uint8_t ch) {
  channel *RET;
  if (ch == CHANNEL_0)
    RET = &channel_0;
  else if (ch == CHANNEL_1)
    RET = &channel_1;
  else
    RET = NULL;
  return RET;
}

//**********************************************************************************************************
// Funcion : void setFreqChannels(uint16_t freq)
//			Funcion que ajusta el tamaño de los datos de cada canal
// y el
// buffer I2S para un valor de frecuencia determinado 			Recibe
// como parametro el valor de frecuencia en Hz
//**********************************************************************************************************
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

//**********************************************************************************************************
// Funcion : void setAmpChannel(uint8_t n_channel, uint8_t amplitude)
//			Funcion que modifica la amplitud de un canal
//			Recibe como parametro el numero de canal y el valor de
// amplitud
//**********************************************************************************************************
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

//**********************************************************************************************************
// Funcion : void setWaveChannel(uint8_t n_channel, uint8_t amplitude)
//			Funcion que define el tipo de señal de un canal
//			Recibe como parametros el numero de canal .y la forma de
// señal
//**********************************************************************************************************
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

/*

//**********************************************************************************************************
// Funcion : void startI2S(void)
//			 Funcion que arranca el envio de datos por el puerto I2S
//			 (Habilita la IRQ del periferico I2S)
//**********************************************************************************************************
void startI2S(void) {
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
  HAL_I2S_Transmit_IT(&hi2s2, (uint16_t *)dataBufferI2S, size_buffer * 2);
}

//**********************************************************************************************************
// Funcion : void stopI2S(void)
//			 Funcion que detiene el envio de datos por el puerto I2S
//			 (Deshabilita la IRQ del periferico I2S)
//**********************************************************************************************************
void stopI2S(void) { HAL_NVIC_DisableIRQ(SPI2_IRQn); }

//**********************************************************************************************************
// Funcion : void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//			 Rutina de atencion cuando ingresa interrupcion
//			 Recibe como parametro: - un puntero a la instancia del
// periferico I2S
//**********************************************************************************************************
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  if (hi2s->Instance == SPI2) {
    HAL_I2S_Transmit_IT(&hi2s2, (uint16_t *)dataBufferI2S, size_buffer * 2);
  }
}

*/
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {

  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
