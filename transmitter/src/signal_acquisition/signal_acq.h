#ifndef SIGNAL_ACQUISITION_H
#define SIGNAL_ACQUISITION_H

#include <stdio.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <pico/util/queue.h>
#include <RF24.h>

#define ADC0 0     // select the first ADC as input
#define ADC_PIN 26 // use the GPIO26 for ADC0

#define ADC_DEFAULT_SAMPLE_FREQUENCY 500'000 // default sample rate: 500 kHz
#define ADC_TARGET_SAMPLE_FREQUENCY 20'000   // target sample rate: 20 kHz
#define CYCLES_PER_CLOCK 96                  // as per the datasheet, each sample conversion takes 96 cycles

/// @brief Inits the ADC and reads values from it, where each byte represents a single raw audio data value.
/// @param frequency specifies the target sample rate frequency for the ADC
/// @param capture_buffer points to the capture buffer that will be filled from the ADC
/// @param capture_buffer_size indicates the size of the buffer
/// @return the DMA channel number after claiming one to control the data flow
uint8_t sig_acq_init(float frequency, uint8_t *capture_buffer, uint8_t capture_buffer_size);

#endif