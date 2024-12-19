# toolchain.cmake
# 设置系统类型为 Windows
set(CMAKE_SYSTEM_NAME Windows)

# 设置 C 和 C++ 编译器
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)

# 设置其他工具链路径
set(CMAKE_AR /usr/bin/x86_64-w64-mingw32-ar)
set(CMAKE_C_COMPILER_AR /usr/bin/x86_64-w64-mingw32-gcc-ar)
set(CMAKE_RANLIB /usr/bin/x86_64-w64-mingw32-ranlib)
set(CMAKE_C_COMPILER_RANLIB /usr/bin/x86_64-w64-mingw32-gcc-ranlib)
set(CMAKE_LINKER /usr/bin/x86_64-w64-mingw32-ld)

# 设置编译器版本
set(CMAKE_C_COMPILER_VERSION "11.4.0")
set(CMAKE_CXX_COMPILER_VERSION "11.4.0")

# 设置编译标准
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_C_FLAGS "-DWIN32_LEAN_AND_MEAN -DVC_EXTRALEAN -fpermissive")
set(CMAKE_CXX_FLAGS "-DWIN32_LEAN_AND_MEAN -DVC_EXTRALEAN -fpermissive")

# 设置 Windows SDK 的包含路径 (请将路径替换为实际的 Windows SDK 安装路径)
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/winrt")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/cppwinrt")

# 设置 Windows SDK 的库路径 (请将路径替换为实际的 Windows SDK 安装路径)
link_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64")
