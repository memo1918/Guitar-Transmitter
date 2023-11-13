#include "signal_acq.h"

static uint8_t dma_channel;

#define CAPTURE_BUFFER_SIZE 128
static uint8_t capture_buffer[CAPTURE_BUFFER_SIZE];

static queue_t *queue;

void dma_handler()
{
    // Clear the interrupt request.
    dma_hw->ints0 = 1u << dma_channel;

    for (int i = 0; i < CAPTURE_BUFFER_SIZE; i++)
    {
        queue_try_add(queue, &capture_buffer[i]);
    }

    dma_channel_set_write_addr(dma_channel, capture_buffer, true);
}

void sig_acq_init(queue_t *q)
{
    queue = q;

    printf("Initializing the ADC ...\n");

    adc_init();          // turn the adc on and wait for ready state
    adc_set_clkdiv(0);   // select fastes divider for sample rate
    adc_gpio_init(26);   // enable the gpio 26 pin for adc voltage measurement
    adc_select_input(0); // select ADC0

    adc_fifo_setup( // enable the fifo setup for working with the dma
        true,       // enable the fifo setup
        true,       // enable the dma request for fifo setup
        1,          // dma treshhold
        false,      // no error bit information on bit 15
        true        // shift 12 bit value to fit in 8bit
    );

    printf("Initializing the DMA ...\n");

    dma_channel = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8); // use 8 bit for the ADC FIFO bit shift
    channel_config_set_read_increment(&cfg, false);          // dont change the source pointer!
    channel_config_set_write_increment(&cfg, true);          // increment to the next pointer pos
    channel_config_set_dreq(&cfg, DREQ_ADC);                 // use data request handshake with the ADC
    channel_config_set_irq_quiet(&cfg, false);               // disable quiet irq
    dma_channel_configure(
        dma_channel,
        &cfg,
        capture_buffer,
        &adc_hw->fifo,
        CAPTURE_BUFFER_SIZE,
        false);

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq0_enabled(dma_channel, true);

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    // first DMA and then ADC
    dma_channel_start(dma_channel);
    adc_run(true);

    // Manually call the handler once, to trigger the first transfer
    sleep_ms(10);

    dma_handler();
}