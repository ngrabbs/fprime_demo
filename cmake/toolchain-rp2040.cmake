# Example toolchain file for cross‑compiling F´ to the Raspberry Pi Pico (RP2040)
#
# This file sets the cross‑compiler and basic compilation flags.  It assumes
# that you have installed the ARM GNU toolchain (e.g. `arm-none-eabi-gcc`) and
# that it is available in your `PATH`.  Adjust the CPU flags to suit your
# specific hardware and performance needs.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Compilers
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Cross compile flags for the Cortex‑M0+ core used by the RP2040
set(RP2040_CPU_FLAGS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")

set(CMAKE_C_FLAGS   "${RP2040_CPU_FLAGS} -Os -ffreestanding" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${RP2040_CPU_FLAGS} -Os -fno-exceptions -fno-rtti -ffreestanding" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${RP2040_CPU_FLAGS} -Wl,--gc-sections" CACHE STRING "" FORCE)

# Specify that we are building for a bare‑metal system.  This tells F´ to
# substitute its own OS abstraction instead of linking against POSIX or Linux.
set(FPRIME_USE_BAREMETAL TRUE CACHE BOOL "Use F´ bare metal OS abstraction")

# Toolchain does not provide standard libraries.  If you wish to link against
# newlib or other C libraries, set these paths accordingly.
# set(CMAKE_FIND_ROOT_PATH /path/to/your/arm/toolchain/sysroot)

# Allow cmake to search host paths for programs (required to find Python etc.)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)