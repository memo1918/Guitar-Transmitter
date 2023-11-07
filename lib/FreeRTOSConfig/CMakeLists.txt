add_library(freertos_config INTERFACE)

target_include_directories(
	freertos_config
	SYSTEM
	INTERFACE
	include
)

target_compile_definitions(
	freertos_config
	INTERFACE
	projCOVERAGE_TEST=0
)

# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "TEMPLATE" CACHE STRING "" FORCE)

FetchContent_Declare(freertos_kernel
	GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
	GIT_TAG main # Note: Best practice to use specific git-hash or tagged version
)

FetchContent_MakeAvailable(freertos_kernel)
