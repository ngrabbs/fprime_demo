cmake_minimum_required(VERSION 3.13)
project(SBandDemo_TelemetryFormatter LANGUAGES C CXX)

set(SOURCE_FILES
    TelemetryFormatterComponentImpl.cpp
)

register_fprime_module()

target_link_libraries(SBandDemo_TelemetryFormatter PUBLIC sband_drivers sband_config)