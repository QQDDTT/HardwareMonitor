file(GLOB LINUX_SOURCES "*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_library(LinuxSpecific ${LINUX_SOURCES})
