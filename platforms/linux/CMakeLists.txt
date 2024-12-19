# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/platforms/include)

# 查找源文件
file(GLOB LINUX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB LINUX_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 添加库
add_library(LinuxSpecific STATIC ${LINUX_SOURCES})
