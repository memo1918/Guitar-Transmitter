#include "signal_acq.h"

#define TRANSMIT_LED_PIN 20

static uint8_t dma_channel;

#define CAPTURE_BUFFER_SIZE 16
static uint8_t capture_buffer_a[CAPTURE_BUFFER_SIZE];
static uint8_t capture_buffer_b[CAPTURE_BUFFER_SIZE];
static queue_t *queue;
static RF24 *radio;
volatile uint8_t id = 0;

void dma_handler()
{
    // gpio_put(20, true);
    //  Clear the interrupt request.
    // dma_hw->ints0 = 1u << dma_channel;

    //// bool success = queue_try_add(queue, &capture_buffer); // before 120ns after 600ns with buffer size 1
    // bool success = queue_try_add(queue, &capture_buffer); // before 120ns after 600ns with buffer size 31
    // capture_buffer[0] = id;
    // id++;
    // radio->startFastWrite(capture_buffer, 32, 0);
    //  if (!success)
    //  {
    //      // printf("[ ERROR ] the dma could not add bytes to the queue, be sure to read from the queue.\n");
    //  }

    // dma_channel_set_write_addr(dma_channel, capture_buffer+1, true);
    // gpio_put(20, false);
}

/**
 * @brief Inits the adc and reads values from it. The queue has to be
 * configured with the correct payload size in order to work correctly. The
 * payload size wichi the adc adds to the queue is 32bytes. Where each byte
 * represents is a singel value. The lowest byte is also the oldest.
 *
 * @param q The queue where the values will be added. If the queue is full, the
 * values will be thrown away, without furter notice
 * @param frequency The frequency at wich the adc reads. The highest possible
 * frequency is 500.000Hz
 */
void sig_acq_init(queue_t *q, float frequency, RF24 *r)
{
    queue = q;
    radio = r;

    printf("Initializing the ADC ...\n");

    adc_init();
    adc_set_clkdiv(2400); // turn the adc on and wait for ready state
    // adc_set_clkdiv(SIGNAL_ACQUISITON_CLOCKSPEED / frequency); // select fastes divider for sample rate
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
        capture_buffer_a,
        &adc_hw->fifo,
        CAPTURE_BUFFER_SIZE,
        false);

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    // dma_channel_set_irq0_enabled(dma_channel, true);

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    //  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    // irq_set_enabled(DMA_IRQ_0, true);

    // first DMA and then ADC
    dma_channel_start(dma_channel);
    adc_run(true);

    // Manually call the handler once, to trigger the first transfer
    sleep_ms(10);

    dma_channel_wait_for_finish_blocking(dma_channel);

    while (true)
    {
        gpio_put(20, true);

        dma_channel_set_write_addr(dma_channel, capture_buffer_b, true);
        //  radio->flush_tx();
        radio->startFastWrite(capture_buffer_a, CAPTURE_BUFFER_SIZE, 0);
        dma_channel_wait_for_finish_blocking(dma_channel);

        dma_channel_set_write_addr(dma_channel, capture_buffer_a, true);
        // radio->flush_tx();
        radio->startFastWrite(capture_buffer_b, CAPTURE_BUFFER_SIZE, 0);

        dma_channel_wait_for_finish_blocking(dma_channel);
        gpio_put(20, false);
    }

    // dma_handler();
}