#include "signal_acq.h"

uint8_t sig_acq_init(float frequency, uint8_t *capture_buffer, uint8_t capture_buffer_size)
{
    printf("Initializing the ADC ...\n");

    adc_init();
    // calculate the divider based on cycles per sample
    float adc_clk_div = (ADC_DEFAULT_SAMPLE_FREQUENCY / ADC_TARGET_SAMPLE_FREQUENCY) * CYCLES_PER_CLOCK;
    adc_set_clkdiv(adc_clk_div); // enables the ADC and waits for the ready state.
    adc_gpio_init(ADC_PIN);      // enable GPIO 26 pin for ADC voltage measurement
    adc_select_input(ADC0);      // select ADC0 as input

    adc_fifo_setup( // enable the fifo setup for working with the DMA
        true,       // enable the fifo setup
        true,       // enable DMA request for FIFO setup
        1,          // DMA threshold set to read every next byte
        false,      // no error bit information on bit 15
        true        // shift 12 bit value to fit in 8 bits
    );

    printf("Initializing the DMA ...\n");

    uint8_t dma_channel = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8); // use 8 bit for the ADC FIFO bit shift
    channel_config_set_read_increment(&cfg, false);          // dont change the source pointer
    channel_config_set_write_increment(&cfg, true);          // increment to the next pointer position
    channel_config_set_dreq(&cfg, DREQ_ADC);                 // use data request handshake with the ADC
    channel_config_set_irq_quiet(&cfg, false);               // disable quiet irq

    // configure all DMA parameters and dont start transfer
    dma_channel_configure(
        dma_channel,
        &cfg,
        capture_buffer,
        &adc_hw->fifo,
        capture_buffer_size,
        false);

    // activates DMA before ADC
    dma_channel_start(dma_channel);
    adc_run(true);

    sleep_ms(10);

    dma_channel_wait_for_finish_blocking(dma_channel); // wait and fill till capture buffer a is filled

    return dma_channel;
}