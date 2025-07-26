cmake_minimum_required(VERSION 3.13)
project(SBandDemo_Housekeeping LANGUAGES C CXX)

set(SOURCE_FILES
    HousekeepingComponentImpl.cpp
)

register_fprime_module()

target_link_libraries(SBandDemo_Housekeeping PUBLIC sband_drivers sband_config)