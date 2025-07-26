#ifndef SBANDDEMO_RADIO_DRIVER_COMPONENT_IMPL_HPP
#define SBANDDEMO_RADIO_DRIVER_COMPONENT_IMPL_HPP

#include "SBandDemo/RadioDriverComponentAc.hpp"  // Auto‑generated base
#include <cstdint>

namespace SBandDemo {

  /**
   * \brief RadioDriver component implementation
   *
   * This class drives the SX1280 radio hardware.  It accepts
   * `RadioMessage` serialisables on its `cmdIn` port.  When a
   * configure message is received, it updates the active radio
   * configuration and reinitialises the radio.  When a payload
   * message is received, it frames the payload and transmits it via
   * `radio_hal_transmit()`.  This mirrors the logic found in the
   * original `task_radio.c`【720892950350647†L14-L83】.
   */
  class RadioDriverComponentImpl : public RadioDriverComponentBase {
    public:
      //! Construct the component
      RadioDriverComponentImpl(const char* compName);

      //! Initialise the component.  Queue depth and instance
      void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance = 0);

    private:
      /**
       * Handler called when a `RadioMessage` is received on the
       * `cmdIn` port.  This is analogous to popping a message off of
       * the original FreeRTOS queue and branching on the message
       * type.  See `task_radio.c` for the original implementation.
       */
      void cmdIn_handler(NATIVE_INT_TYPE portNum, const SBandDemo::RadioMessage& msg) override;
  };

} // namespace SBandDemo

#endif /* SBANDDEMO_RADIO_DRIVER_COMPONENT_IMPL_HPP */