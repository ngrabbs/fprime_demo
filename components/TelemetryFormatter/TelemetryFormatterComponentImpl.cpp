#include "TelemetryFormatterComponentImpl.hpp"

using namespace SBandDemo;

TelemetryFormatterComponentImpl::TelemetryFormatterComponentImpl(const char* compName)
    : TelemetryFormatterComponentBase(compName) {}

void TelemetryFormatterComponentImpl::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) {
    TelemetryFormatterComponentBase::init(queueDepth, instance);
}