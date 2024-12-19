# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/platforms/include)

# 查找源文件
file(GLOB WINDOWS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB WINDOWS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 添加库
add_library(WindowsSpecific ${WINDOWS_SOURCES})
