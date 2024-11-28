cmake_minimum_required(VERSION 3.10)
project(HardwareMonitor VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含源文件目录
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/include)
include_directories(${CMAKE_SOURCE_DIR}/platforms/linux)
include_directories(${CMAKE_SOURCE_DIR}/platforms/windows)

# 查找源文件并添加到项目
file(GLOB SOURCES "src/*.cpp")

# 添加可执行文件
add_executable(HardwareMonitor.exe ${SOURCES})

# 针对不同平台的配置
if (WIN32)
    add_subdirectory(platforms/windows)
    target_link_libraries(HardwareMonitor.exe WindowsSpecific wbemuuid)
elseif (UNIX)
    add_subdirectory(platforms/linux)
    target_link_libraries(HardwareMonitor.exe LinuxSpecific)
endif()
