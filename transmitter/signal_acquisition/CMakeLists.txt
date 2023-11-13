add_library(SIG_ACQ 
    src/signal_acq.c
    )
include_directories(include)

target_link_libraries(SIG_ACQ hardware_adc hardware_dma hardware_sync)