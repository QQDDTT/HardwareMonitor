file(GLOB WINDOWS_SOURCES "*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_library(WindowsSpecific ${WINDOWS_SOURCES})
