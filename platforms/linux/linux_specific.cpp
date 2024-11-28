#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>  // for sleep
#include <thread>
#include <chrono>
#include <iomanip>
#include <sys/utsname.h>
#include <termios.h>
#include <unistd.h>
#include "message.h"  // 包含语言消息定义
#include "system_info.h"  // 包含系统信息函数声明


/**
 * @brief 获取操作系统信息
 * 
 * 该函数通过读取 `/proc/cpuinfo` 文件，获取并显示操作系统的基本信息。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void GetOSInfo(const MsgList& lang) {
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        std::cout << lang.os_info_title << buffer.sysname << std::endl;
        std::cout << lang.os_name << buffer.nodename << std::endl;
        std::cout << lang.os_kernel << buffer.release << std::endl;
        std::cout << lang.os_version << buffer.version << std::endl;
        std::cout << lang.os_architecture << buffer.machine << std::endl;
    } else {
        std::cerr << "Unable to retrieve Linux system information!" << std::endl;
    }
}

/**
 * @brief 显示每个 CPU 线程的基本信息
 * 
 * 该函数通过读取 `/proc/cpuinfo` 文件，获取并显示每个 CPU 线程的基本信息。
 * 包括 CPU 型号（仅输出一次）和每个线程的 CPU 频率。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayCPUInfo(const MsgList& lang) {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;

    // 显示标题
    std::cout << "\033[36m" << lang.cpu_info_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 检查文件是否成功打开
    if (!cpuinfo.is_open()) {
        std::cerr << "Unable to open /proc/cpuinfo" << std::endl;
        return;
    }

    std::string model;
    std::string mhz;
    int thread_id = -1;
    bool model_printed = false;  // 标记是否已输出过 CPU 型号

    // 逐行读取 /proc/cpuinfo 文件
    while (std::getline(cpuinfo, line)) {
        // 提取 CPU 型号，只输出一次
        if (line.find("model name") != std::string::npos && !model_printed) {
            model = line.substr(line.find(':') + 2);
            std::cout << lang.cpu_model << ": " << model << std::endl;
            model_printed = true;  // 更新标记，避免重复输出
        }
        // 提取 CPU 频率并输出每个线程的信息
        else if (line.find("cpu MHz") != std::string::npos) {
            mhz = line.substr(line.find(':') + 2);

            // 每读取到一个线程的频率信息时，输出该线程的 CPU 频率
            thread_id++;
            std::cout << lang.cpu_mhz << " (Thread " << thread_id << "): " << mhz << " MHz" << std::endl;
        }
    }

    cpuinfo.close();
}


/**
 * @brief 显示每个 CPU 线程的使用率
 * 
 * 该函数通过读取 `/proc/stat` 文件，计算并显示每个 CPU 线程的使用率。
 * 使用率基于 CPU 活跃时间与总时间的比率，输出结果保留两位小数。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayCPUThreadUsage(const MsgList& lang) {
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) {
        std::cerr << "Unable to open /proc/stat" << std::endl;
        return;
    }

    std::vector<std::string> cpuLines;
    std::string line;

    // 获取所有 CPU 线程的统计数据
    while (std::getline(statFile, line)) {
        if (line.find("cpu") == 0 && line != "cpu") {
            cpuLines.push_back(line);
        }
    }

    statFile.close();

    std::cout << "\033[36m" << lang.cpu_thread_usage_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 计算并显示每个线程的使用率
    for (const auto& cpuLine : cpuLines) {
        std::istringstream iss(cpuLine);
        std::string cpuName;
        long user, nice, system, idle, iowait, irq, softirq;

        // 解析每一行的数据
        iss >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

        // 计算总时间和活跃时间
        long totalTime = user + nice + system + idle + iowait + irq + softirq;
        long activeTime = totalTime - idle;
        double usagePercent = (static_cast<double>(activeTime) / totalTime) * 100;

        // 设置输出格式，保留两位小数
        std::cout << lang.cpu_thread_usage << " (" << cpuName << "): "
                  << std::fixed << std::setprecision(2)  // 保留两位小数
                  << usagePercent << lang.cpu_thread_usage_unit << "\n";
    }
}


/**
 * @brief 显示 GPU 信息
 * 
 * 该函数通过执行 `nvidia-smi` 命令，获取并显示每个 NVIDIA GPU 的使用情况。
 * 如果系统没有安装 NVIDIA GPU 或者 `nvidia-smi` 命令不可用，将输出提示信息。
 * 如果系统检测到 GPU，将显示详细的 GPU 使用率、显存信息等。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayGPUInfo(const MsgList& lang) {
    std::cout << "\033[36m" << lang.gpu_info_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 检测是否安装了 nvidia-smi 并可用
    if (system("nvidia-smi > /dev/null 2>&1") != 0) {
        std::cout << lang.gpu_no_detect << "\n";  // 没有检测到 GPU 或命令不可用
        return;
    }

    // 使用 nvidia-smi 获取 GPU 信息
    FILE* pipe = popen("nvidia-smi --query-gpu=gpu_name,utilization.gpu,utilization.memory,memory.total,memory.used,memory.free --format=csv,noheader,nounits", "r");
    if (!pipe) {
        std::cerr << "Error: Unable to execute nvidia-smi command.\n";
        return;
    }

    // 逐行读取输出
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::istringstream gpuData(buffer);
        std::string gpuName;
        int gpuUsage, memoryUsage;
        int totalMemory, usedMemory, freeMemory;

        // 解析 nvidia-smi 输出的字段
        if (!(gpuData >> std::ws).getline(buffer, sizeof(buffer), ','))
            break; // 读取 GPU 名称
        gpuName = buffer;

        gpuData >> gpuUsage; // GPU 使用率
        gpuData.ignore();    // 跳过逗号
        gpuData >> memoryUsage; // 显存使用率
        gpuData.ignore();
        gpuData >> totalMemory; // 显存总量
        gpuData.ignore();
        gpuData >> usedMemory; // 已用显存
        gpuData.ignore();
        gpuData >> freeMemory; // 可用显存

        // 输出详细的 GPU 使用状态
        std::cout << "\033[33mGPU Name: \033[0m" << gpuName << "\n";
        std::cout << "  \033[32mGPU Utilization: \033[0m" << gpuUsage << "%\n";
        std::cout << "  \033[32mMemory Utilization: \033[0m" << memoryUsage << "%\n";
        std::cout << "  \033[32mTotal Memory: \033[0m" << totalMemory << " MB\n";
        std::cout << "  \033[32mUsed Memory: \033[0m" << usedMemory << " MB\n";
        std::cout << "  \033[32mFree Memory: \033[0m" << freeMemory << " MB\n";
    }

    pclose(pipe);
}


/**
 * @brief 显示内存信息
 * 
 * 该函数通过读取 `/proc/meminfo`，获取并显示系统的内存信息，包括总内存、空闲内存、交换分区信息等。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayMemoryInfo(const MsgList& lang) {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;

    // 显示内存信息标题
    std::cout << "\033[36m" << lang.memory_info_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 打开文件失败时的错误提示
    if (!meminfo.is_open()) {
        std::cerr << "Unable to open /proc/meminfo" << std::endl;
        return;
    }

    // 逐行读取内存信息并解析
    while (std::getline(meminfo, line)) {
        // 解析总内存
        if (line.find("MemTotal") != std::string::npos) {
            std::cout << lang.mem_total << ": " << line.substr(line.find(':') + 1) << std::endl;
        }
        // 解析空闲内存
        else if (line.find("MemFree") != std::string::npos) {
            std::cout << lang.mem_free << ": " << line.substr(line.find(':') + 1) << std::endl;
        }
        // 解析交换分区内存
        else if (line.find("SwapTotal") != std::string::npos) {
            std::cout << lang.swap_total << ": " << line.substr(line.find(':') + 1) << std::endl;
        }
    }

    meminfo.close();
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);  // 获取当前终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 禁用规范模式和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // 设置新的终端属性
    ch = getchar();  // 获取单个字符输入
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 恢复终端属性
    return ch;
}