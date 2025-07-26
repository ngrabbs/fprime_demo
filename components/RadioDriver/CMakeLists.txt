cmake_minimum_required(VERSION 3.13)
project(SBandDemo_RadioDriver LANGUAGES C CXX)

# Source files for this component.  The auto‑coded C++ files generated from
# RadioDriver.fpp (RadioDriverComponentAc.cpp/hpp) will be built automatically
# by the F´ autocoder when `fprime-util generate` is invoked.  Only the
# implementation file needs to be specified here.
set(SOURCE_FILES
    RadioDriverComponentImpl.cpp
)

register_fprime_module()

# Link against the configuration and driver libraries
target_link_libraries(SBandDemo_RadioDriver PUBLIC sband_config sband_drivers)