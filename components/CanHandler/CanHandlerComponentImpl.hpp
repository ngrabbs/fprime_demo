#ifndef SBANDDEMO_CAN_HANDLER_COMPONENT_IMPL_HPP
#define SBANDDEMO_CAN_HANDLER_COMPONENT_IMPL_HPP

#include "SBandDemo/CanHandlerComponentAc.hpp"  // Auto‑generated base

namespace SBandDemo {

  /**
   * \brief CAN Handler component
   *
   * This component configures the MCP2515 CAN controller and polls
   * it for incoming frames.  When a SpaceCAN command is detected it
   * assembles the command, decodes the parameters and emits a
   * `RadioMessage` via the `radioOut` port.  This functionality
   * replaces the original `task_can.c` and `spacecan_assembler.c`
   * modules【805012935155573†L26-L35】.
   */
  class CanHandlerComponentImpl : public CanHandlerComponentBase {
    public:
      explicit CanHandlerComponentImpl(const char* compName);
      void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance = 0);

    private:
      void schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) override;

      // Internal state for assembling two‑frame commands
      bool waitingForSecondFrame;
      can_frame_t frameBuffer;
  };

}

#endif /* SBANDDEMO_CAN_HANDLER_COMPONENT_IMPL_HPP */