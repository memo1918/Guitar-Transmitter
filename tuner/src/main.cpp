#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/multicore.h>
#include <math.h>
#include "kiss_fftr.h"
#include "Yin.h"
#include "signal_acquisition/signal_acq.h"

#define NSAMP 516 //->res 4Hz
float freqs[NSAMP];
int16_t cap_buf[NSAMP];
#define FSAMP 48000  //sample rate





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

void fft_cal()
{
	kiss_fft_scalar fft_in[NSAMP]; // kiss_fft_scalar is a float
	kiss_fft_cpx fft_out[NSAMP];
	kiss_fftr_cfg cfg = kiss_fftr_alloc(NSAMP, false, 0, 0);

	for (int i = 0; i < NSAMP; i++){
		fft_in[i] = (float)cap_buf[i];
	} 

	kiss_fftr(cfg, fft_in, fft_out);

	// compute power and calculate max freq component
	float max_power = 0;
	int max_idx = 0;
	// any frequency bin over NSAMP/2 is aliased (nyquist sampling theorum)
	for (int i = 0; i < NSAMP / 2; i++){
		float power = fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i;
		if (power > max_power){
			max_power = power;
			max_idx = i;
		}
	}

	float max_freq = freqs[max_idx];
	printf("Greatest Frequency Component: %0.1f Hz\n", max_freq);
}

void test(){

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

	// float f_max = FSAMP;
	// float f_res = f_max / NSAMP;
	// for (int i = 0; i < NSAMP; i++){
	// 	freqs[i] = f_res * i;//filling the bins
	// }

	// multicore_launch_core1(core1_entry);


	// for (int i = 0; i < NSAMP; ++i) {
    //     double time = i / 44100.0; // Convert sample index to time in seconds
    //     double fundamental = 440.0; // Fundamental frequency for guitar (A4)
    //     double sin_value = sin(2 * 3.14159 * fundamental * time);
    //     // sin_value += 0.5 * sin(2 * 3.14159 * 2 * fundamental * time); // 1st harmonic
    //     // sin_value += 0.2 * sin(2 * 3.14159 * 3 * fundamental * time); // 2nd harmonic
    //     double envelope = exp(-time * 5.0); // Adjust the decay factor as needed
    //     sin_value *= envelope;

    //     // Scale and convert the sin value to the  range [0, 255]
    //     cap_buf[i] = (int16_t)((sin_value + 1.0) * 127.5);
    //     // cap_buf[i] = (int16_t)((sin_value + 1.0) * 66) + 65;
	// 	printf("%d, \n",cap_buf[i]);
    // 	}

	// test();

	

	while (true)
	{
		
		// multicore_fifo_push_blocking((uintptr_t)&yin_al);
		// multicore_fifo_push_blocking((uintptr_t)&data);
		// sleep_ms(2000);

	}
	return 0;
}
