#ifndef SBANDDEMO_HOUSEKEEPING_COMPONENT_IMPL_HPP
#define SBANDDEMO_HOUSEKEEPING_COMPONENT_IMPL_HPP

#include "SBandDemo/HousekeepingComponentAc.hpp"  // Auto‑generated base

namespace SBandDemo {

  /**
   * \brief Housekeeping component
   *
   * Periodically gathers system telemetry (temperature, uptime, heap free,
   * task count and radio counters) using the functions from the
   * original firmware and formats it into a SpaceCAN beacon frame
   * before sending it to the radio.  This reproduces the logic in
   * `task_housekeeping.c`【740037136390698†L14-L37】.
   */
  class HousekeepingComponentImpl : public HousekeepingComponentBase {
    public:
      explicit HousekeepingComponentImpl(const char* compName);
      void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance = 0);

    private:
      void schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) override;
  };

}

#endif /* SBANDDEMO_HOUSEKEEPING_COMPONENT_IMPL_HPP */