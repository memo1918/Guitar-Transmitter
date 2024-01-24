#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/multicore.h>
#include "Yin.h"
#include "signal_acquisition/signal_acq.h"

#define NSAMP 516 //->res 4Hz
float freqs[NSAMP];
int16_t cap_buf[NSAMP];

//res = sample_rate / fft_size tuners need at least 4.9Hz res

void core1_entry()
{
	while (1)
	{
		void (*func)() = (void (*)())multicore_fifo_pop_blocking();
		// int32_t p = multicore_fifo_pop_blocking();
		func();
		// multicore_fifo_push_blocking(result);
	}
}

void YinTuner(){

	int buffer_length = NSAMP;
	Yin yin;
	float pitch;

	Yin_init(&yin, buffer_length, 0.2);
	pitch = Yin_getPitch(&yin, cap_buf);
	

	printf("Pitch is found to be %f and %d \n ",pitch,buffer_length);
}

int main()
{
	stdio_init_all();

	sig_acq_init(nullptr);

	YinTuner();

	while (true)
	{
		
		// multicore_fifo_push_blocking((uintptr_t)&yin_al);
		// multicore_fifo_push_blocking((uintptr_t)&data);
		// sleep_ms(2000);

	}
	return 0;
}
