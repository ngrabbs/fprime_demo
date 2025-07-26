#ifndef SBANDDEMO_TELEMETRY_FORMATTER_COMPONENT_IMPL_HPP
#define SBANDDEMO_TELEMETRY_FORMATTER_COMPONENT_IMPL_HPP

#include "SBandDemo/TelemetryFormatterComponentAc.hpp"  // Auto‑generated base

namespace SBandDemo {
  class TelemetryFormatterComponentImpl : public TelemetryFormatterComponentBase {
    public:
      TelemetryFormatterComponentImpl(const char* compName);
      void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance = 0);

    private:
      // Add handler functions here when ports are defined
  };
}

#endif /* SBANDDEMO_TELEMETRY_FORMATTER_COMPONENT_IMPL_HPP */