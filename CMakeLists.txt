cmake_minimum_required(VERSION 3.13)
project(fprime_sband_demo LANGUAGES C CXX)

# -----------------------------------------------------------------------------
# Root CMake file for the F´ S‑Band demo
#
# This project assumes that the F´ framework has been checked out into the
# `fprime` subdirectory.  If you have placed it elsewhere, set the
# `FPRIME_ROOT` cache variable accordingly when invoking CMake.  The build
# system will include F´ and register the components defined in this repository.
#
# Example usage:
#   cmake -DFPRIME_ROOT=../fprime -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-rp2040.cmake ..
#
# The RP2040 toolchain file configures the ARM cross‑compiler.  For native
# builds (e.g. running unit tests on your host) you can omit the toolchain file.

if(NOT DEFINED FPRIME_ROOT)
    set(FPRIME_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/fprime CACHE PATH "Path to the F´ core")
endif()

if(NOT EXISTS ${FPRIME_ROOT}/cmake/FPrime.cmake)
    message(FATAL_ERROR "F´ core not found.  Please initialise the fprime submodule or set FPRIME_ROOT.")
endif()

include(${FPRIME_ROOT}/cmake/FPrime.cmake)

# Add include directories for config and drivers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/radio
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/can
    ${CMAKE_CURRENT_SOURCE_DIR}/protocols/include
    ${CMAKE_CURRENT_SOURCE_DIR}/subsystems/housekeeping/include
)

# Register modules for drivers and protocols.  These are plain C files copied
# from the original firmware and are not F´ components.  They are added as
# libraries so that component implementations can link against them.

add_library(sband_config STATIC
    config/config.c
)
target_include_directories(sband_config PUBLIC config)

add_library(sband_drivers STATIC
    # Radio drivers
    drivers/radio/peripheral_lora1280.c
    drivers/radio/peripheral_rfm9x.c
    drivers/radio/sx1280_wrapper.cpp
    drivers/radio/radio_hal.c
    drivers/radio/SX1280.cpp
    # CAN driver
    drivers/can/peripheral_mcp2515.c
    # Protocols
    protocols/protocol_spacecan.c
    protocols/spacecan_assembler.c
    # Housekeeping
    subsystems/housekeeping/housekeeping.c
)
target_include_directories(sband_drivers PUBLIC
    drivers/radio
    drivers/can
    protocols/include
    subsystems/housekeeping/include
    config
)
set_target_properties(sband_drivers PROPERTIES LINKER_LANGUAGE CXX)

# Add F´ components
add_fprime_subdirectory(components/RadioDriver)
add_fprime_subdirectory(components/CanHandler)
add_fprime_subdirectory(components/Housekeeping)
add_fprime_subdirectory(components/TelemetryFormatter)
add_fprime_subdirectory(top)

## Unit test targets
# Each component may define its own unit tests.  See the component level
# CMakeLists.txt files for details.