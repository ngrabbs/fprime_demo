cmake_minimum_required(VERSION 3.13)
project(SBandDemo_Top LANGUAGES C CXX)

set(SOURCE_FILES
    topology.cpp
)

register_fprime_module()

target_link_libraries(SBandDemo_Top PUBLIC
    SBandDemo_RadioDriver
    SBandDemo_CanHandler
    SBandDemo_Housekeeping
    SBandDemo_TelemetryFormatter
    sband_drivers
    sband_config
)