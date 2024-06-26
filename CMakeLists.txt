cmake_minimum_required(VERSION 3.13)

include(FetchContent)
FetchContent_Declare( freertos_kernel
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
  GIT_TAG        main #Note: Best practice to use specific git-hash or tagged version
)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
INTERFACE
    include
)

target_compile_definitions(freertos_config
  INTERFACE
    projCOVERAGE_TEST=0
)


project(tcp_ip C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(tcp_server
    tcp_server.c
)

target_compile_definitions(tcp_server PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)

)

pico_enable_stdio_usb(tcp_server 1)
pico_enable_stdio_uart(tcp_server 1)
target_include_directories(tcp_server PRIVATE ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries(tcp_server 
pico_cyw43_arch_lwip_sys_freertos
pico_stdlib
pico_lwip_iperf
FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
)
pico_add_extra_outputs(tcp_server)
