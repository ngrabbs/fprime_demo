cmake_minimum_required(VERSION 3.13)
project(SBandDemo_CanHandler LANGUAGES C CXX)

set(SOURCE_FILES
    CanHandlerComponentImpl.cpp
)

register_fprime_module()

target_link_libraries(SBandDemo_CanHandler PUBLIC sband_drivers sband_config)