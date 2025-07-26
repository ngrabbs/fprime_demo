# Stub CMakeLists.txt for FreeRTOS integration
#
# In a real system you should add the FreeRTOS source files here and
# register them as an fprime module or link them directly into your
# application.  The Os layer in F´ will call FreeRTOS API functions.

cmake_minimum_required(VERSION 3.13)
project(FreeRTOSPlatform LANGUAGES C CXX)

# Add your FreeRTOS sources here.  For example:
# set(SOURCE_FILES
#     ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/tasks.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/queue.c
#     ...
# )

# add_library(FreeRTOS STATIC ${SOURCE_FILES})

# target_include_directories(FreeRTOS PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/include
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )

# register_fprime_module() is not called here because FreeRTOS is not an F´
# component; instead it will be linked into the deployment directly.