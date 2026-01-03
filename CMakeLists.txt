cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts -fcommon -fno-rtti -fno-exceptions")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PICO_DEOPTIMIZED_DEBUG 1)

# path to PICO utilities (if needed)
if (NOT DEFINED ENV{PIOASM_DIR})
        message("PIOASM: set PIOASM_DIR env variable if you use PIO")
else()
        message("PIOASM: PIOASM_DIR env variable is set to '$ENV{PIOASM_DIR}'")
        set(pioasm_DIR $ENV{PIOASM_DIR})
endif()
if (NOT DEFINED ENV{PICOTOOL_DIR})
        message("PICOTOOL: set PICOTOOL_DIR env variable if you create uf2 output with pico_add_extra_outputs()")
else()
        message("PICOTOOL: PICOTOOL_DIR env variable is set to '$ENV{PICOTOOL_DIR}'")
        set(picotool_DIR $ENV{PICOTOOL_DIR})
endif()

if (DEFINED ENV{PICO_PLATFORM})
        message("PICO_PLATFORM: PICO_PLATFORM env variable is set to '$ENV{PICO_PLATFORM}'")
        set(PICO_PLATFORM $ENV{PICO_PLATFORM} CACHE STRING "Pico Platform")
else()
        message("PICO_PLATFORM: PICO_PLATFORM env variable is not set. Using default")
endif()

if (DEFINED ENV{PICO_BOARD})
        message("PICO_BOARD: PICO_BOARD env variable is set to '$ENV{PICO_BOARD}'")
        set(PICO_BOARD $ENV{PICO_BOARD} CACHE STRING "Board type")
else()
        message("PICO_BOARD: PICO_BOARD env variable is not set. Using default")
endif()

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(uart_stream_example C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include(FetchContent)

FetchContent_Declare(pico_uart_stream
  GIT_REPOSITORY "https://github.com/jancumps/pico_uart_stream.git"
  GIT_TAG "origin/main"
)
FetchContent_MakeAvailable(pico_uart_stream)


add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
)
target_sources(${CMAKE_PROJECT_NAME}
        PUBLIC
        uart_stream.cpp
)


# Modify the below lines to enable/disable output over UART/USB
#pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)
#pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(${CMAKE_PROJECT_NAME}
        pico_stdlib
        hardware_uart
        pico_uart_stream
)

# Add the standard include files to the build
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

