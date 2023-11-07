#ifndef SIGNAL_ACQUISITION_H
#define SIGNAL_ACQUISITION_H

#include <stdio.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include "pico/util/queue.h"
#include <stdint-gcc.h>

void sig_acq_init(queue_t *q);

#endif