#ifndef SIGNAL_ACQUISITION_H
#define SIGNAL_ACQUISITION_H

#define SIGNAL_ACQUISITON_CLOCKSPEED 500'000 // 48'000'000  // 48 MHz

#include <stdio.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <pico/util/queue.h>

void sig_acq_init(queue_t *q, float frequency);

#endif