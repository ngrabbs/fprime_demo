# F´ S‑Band Demo for RP2040

This repository contains a work‑in‑progress port of the original **S‑Band Transceiver Payload** firmware into the NASA F´ (F Prime) flight software framework.  The goal of the project is to demonstrate how the payload’s FreeRTOS‑based tasks and driver code can be restructured into F´ components, enabling reusable, modular flight software for a Raspberry Pi Pico or Adafruit Feather RP2040.  The original project implements a CubeSat payload that communicates with an upstream flight computer over CAN and a 2.4 GHz LoRa transceiver over SPI【273335158193062†L316-L343】.  It provides tasks for radio transmission, CAN reception, and housekeeping/telemetry collection【273335158193062†L344-L370】.

## Repository layout

```
fprime_sband_demo/
├── CMakeLists.txt               # Root build system entry point
├── cmake/
│   └── toolchain-rp2040.cmake   # Example toolchain file for the RP2040
├── platform/FreeRTOS/           # Hook for FreeRTOS integration (stub)
├── config/                      # System configuration structures copied from the original project
│   ├── config.h
│   └── config.c
├── drivers/                     # Low level hardware drivers
│   ├── radio/                   # SX1280 LoRa wrapper and vendor port
│   └── can/                     # MCP2515 CAN controller driver
├── protocols/                   # SpaceCAN protocol implementation
│   ├── include/
│   └── protocol_spacecan.c
├── subsystems/housekeeping/     # Housekeeping telemetry (temperature, uptime, heap, etc.)
├── components/                  # F´ components implementing the application logic
│   ├── RadioDriver/
│   ├── CanHandler/
│   ├── Housekeeping/
│   └── TelemetryFormatter/
└── top/                         # Top‑level F´ topology and deployment
    ├── Top.fpp
    ├── topology.cpp
    └── CMakeLists.txt
```

### Components

The original firmware uses three FreeRTOS tasks:

* **Radio task:** Performs radio initialisation and transmits queued payloads.  It receives configuration messages and payloads from a simple message queue and calls into the SX1280 HAL to transmit data【720892950350647†L14-L83】.
* **CAN task:** Configures the MCP2515 controller and polls for incoming CAN frames.  When a frame is received it is passed into a protocol parser which decodes SpaceCAN commands and pushes messages onto the radio queue【805012935155573†L26-L35】.
* **Housekeeping task:** Periodically collects system telemetry such as temperature, uptime, free heap and transmit counters.  The data are formatted into a small beacon packet using a SpaceCAN formatter and then sent to the radio queue for transmission【740037136390698†L14-L37】.

In F´ each of these tasks becomes a component with well‑defined input and output ports.  The new components are:

* **`RadioDriver`** – An active component responsible for initialising the radio hardware and transmitting packets.  It provides a command input port accepting a `RadioMessage` serialisable containing either configuration parameters or data to transmit.  Internally it calls the `radio_hal_init()` and `radio_hal_transmit()` functions from the original driver.  Future enhancements could add a telemetry channel reporting radio status.

* **`CanHandler`** – An active component that configures the MCP2515 CAN controller and polls it for new frames.  When a frame is received it decodes SpaceCAN packets using the original `spacecan_assembler_handle()`/`spacecan_parse_frame()` code and emits the appropriate `RadioMessage` to the `RadioDriver` component.  Additional ports could be added for uplinking commands or forwarding telemetry to the upstream flight computer.

* **`Housekeeping`** – A passive or periodically driven component that reads the RP2040’s internal temperature sensor, uptime counter and FreeRTOS statistics using the existing `collect_housekeeping_data()` function.  It formats the data into a beacon using `protocol_spacecan_format_beacon()` and sends the result to the radio for transmission.  This mirrors the original housekeeping FreeRTOS task【740037136390698†L14-L37】.

* **`TelemetryFormatter`** – A simple component that encapsulates the SpaceCAN formatting and parsing logic so it can be reused by both the `Housekeeping` and `CanHandler` components.  It provides a port for formatting housekeeping data and a port for parsing incoming CAN frames.

### Platform and toolchain

This repository does **not** include the F´ core as a submodule.  To build the project you must clone the upstream F´ repository and initialise the build environment.  A minimal workflow is:

```bash
# Install fprime bootstrap tools if not already installed
pip install fprime-bootstrap

# Create a new project based off of this repository
git clone https://github.com/ngrabbs/fprime_sband_demo.git
cd fprime_sband_demo

# Bring in fprime as a submodule (replace with your preferred source)
git submodule add https://github.com/nasa/fprime.git fprime
git submodule update --init --recursive

# Initialise an fprime build directory
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-rp2040.cmake ..
make -j

# The resulting binary can be flashed to the RP2040 using your preferred method
```

The provided `cmake/toolchain-rp2040.cmake` file illustrates how to set the cross‑compiler for the Raspberry Pi Pico.  It chooses `arm-none-eabi-gcc` and configures the build as a bare‑metal system.  You must adjust the `FPRIME_CMAKE_FRAMEWORK_PATH` and FreeRTOS paths to match your environment.

### FreeRTOS integration

The original firmware is based on FreeRTOS.  F´ abstracts the underlying operating system through its **Os** layer.  To run F´ on the RP2040 you will need a FreeRTOS port as well as F´ platform adaptations.  The `platform/FreeRTOS` directory contains a stub `FreeRTOSConfig.h` and placeholder CMakeLists.txt to integrate FreeRTOS into the build.  In a real deployment you should supply the full FreeRTOS source and ensure that the F´ Os implementation calls into FreeRTOS for thread, queue and time functions.

### How to extend

* Flesh out the `.fpp` model files in the `components` and `top` directories to define commands, events, telemetry channels and ports.  The provided files define a `RadioMessage` serialisable and the high‑level component interfaces, but they leave many details as TODO items.
* Implement the `ComponentImpl.cpp` files to call into the original driver code.  See `drivers/radio` and `drivers/can` for the low‑level API.  The original message queue has been replaced with direct component port calls in F´.
* Add unit tests under the `tests/` directory.  F´ uses GoogleTest for component unit testing.  You can register unit test sources in the component CMakeLists.txt by setting the `UT_SOURCE_FILES` variable and calling `register_fprime_ut()`【568629997924700†L136-L149】.

### Status

This code base is a **skeleton** intended to help you port the original S‑Band Transceiver Payload into F´.  It mirrors the directory structure and copies the driver and protocol code verbatim from the original project for convenience.  Many files contain `TODO` comments indicating where F´‑specific logic must be implemented.  Refer to the original README and source code for functional details and to the F´ user guide for guidance on writing components and topologies.

---

**Original firmware summary:**

The original S‑Band Transceiver Payload firmware is a modular FreeRTOS application for an RP2040‑based CubeSat payload board.  It receives commands over CAN, decodes and processes those commands, and transmits messages over a LoRa radio【273335158193062†L324-L331】.  It also collects housekeeping telemetry and transmits beacon frames【740037136390698†L14-L37】.  The firmware provides a flexible, plug‑and‑play interface for any CubeSat platform that supports CAN and provides 3.3 V power【273335158193062†L324-L331】.