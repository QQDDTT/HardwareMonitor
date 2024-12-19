#define NTDDI_VERSION NTDDI_WIN10_CO  // Windows 11 21H2
#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>
#include <combaseapi.h>
#include <winnt.h>
#include <winbase.h>
#include <winuser.h>
#include <winreg.h>
#include <winternl.h>
#include <conio.h>

#include "monitor.h"
#include <VersionHelpers.h>
#include <minwinbase.h>
#include <sysinfoapi.h>


#pragma comment(lib, "wbemuuid.lib") 

/**
 * @brief 获取操作系统信息
 * 
 * 该函数使用 Windows API 获取操作系统的名称、版本和架构等信息，并输出到控制台。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void GetOSInfo(const MsgList& lang) {
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    // 获取操作系统版本信息
    if (IsWindows8Point1OrGreater()) {
        std::cout << lang.os_info_title << std::endl;

        // 获取操作系统名称
        std::cout << lang.os_name << ": ";
        // 根据不同版本判断操作系统名称
        if (osvi.dwMajorVersion == 10) {
            std::cout << "Windows 10" << std::endl;
        } else if (osvi.dwMajorVersion == 6) {
            if (osvi.dwMinorVersion == 3) {
                std::cout << "Windows 8.1" << std::endl;
            } else if (osvi.dwMinorVersion == 2) {
                std::cout << "Windows 8" << std::endl;
            } else if (osvi.dwMinorVersion == 1) {
                std::cout << "Windows 7" << std::endl;
            } else if (osvi.dwMinorVersion == 0) {
                std::cout << "Windows Vista" << std::endl;
            }
        } else if (osvi.dwMajorVersion == 5) {
            if (osvi.dwMinorVersion == 2) {
                std::cout << "Windows Server 2003" << std::endl;
            } else if (osvi.dwMinorVersion == 1) {
                std::cout << "Windows XP" << std::endl;
            }
        }

        // 输出操作系统版本
        std::cout << lang.os_version << ": " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
        // 输出操作系统内核版本
        std::cout << lang.os_kernel << ": " << osvi.dwBuildNumber << std::endl;

        // 获取和输出操作系统架构信息
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
            std::cout << lang.os_architecture << ": x64 (64-bit)" << std::endl;
        } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
            std::cout << lang.os_architecture << ": x86 (32-bit)" << std::endl;
        } else {
            std::cout << lang.os_architecture << ": Unknown architecture" << std::endl;
        }
    } else {
        std::cerr << "Unable to retrieve OS information!" << std::endl;
    }
}

/**
 * @brief 显示 CPU 信息
 * 
 * 该函数通过 WMI 查询获取 CPU 的基本信息，包括 CPU 名称和当前时钟速度，并输出到控制台。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayCPUInfo(const MsgList& lang) {
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;

    // 初始化 COM 库
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "初始化 COM 库失败" << std::endl;
        return;
    }

    // 设置 COM 安全级别
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        std::cerr << "初始化安全性失败" << std::endl;
        CoUninitialize();
        return;
    }

    // 创建 WMI 定位器对象
    hres = CoCreateInstance(
        CLSID_WbemLocator,           // 要创建的COM类的CLSID
        0,                           // 不需要聚合
        CLSCTX_INPROC_SERVER,        // 在同一进程中创建服务器
        IID_IWbemLocator,            // 请求的接口ID
        (LPVOID *)&pLoc              // 接收接口指针的地址
    );
    if (FAILED(hres)) {
        std::cerr << "创建 IWbemLocator 对象失败" << std::endl;
        CoUninitialize();
        return;
    }

    // 连接到 WMI
    hres = pLoc->ConnectServer(L"ROOT\\CIMV2", NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hres)) {
        std::cerr << "连接到 WMI 失败" << std::endl;
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // 设置安全级别
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                             RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hres)) {
        std::cerr << "设置代理空白失败" << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // 查询 WMI 获取 CPU 信息
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(L"WQL", L"SELECT * FROM Win32_Processor", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumerator);
    if (FAILED(hres)) {
        std::cerr << "查询 WMI 失败" << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    IWbemClassObject *pClassObject;
    ULONG uReturn = 0;

    // 输出 CPU 信息标题
    std::cout << "\033[36m" << lang.cpu_info_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 循环处理返回的 CPU 信息
    while (pEnumerator) {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        // 获取 CPU 名称
        hres = pClassObject->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres)) {
            std::wcout << lang.cpu_model << ": " << vtProp.bstrVal << std::endl;
        }
        VariantClear(&vtProp);

        // 获取当前时钟速度
        hres = pClassObject->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres)) {
            std::wcout << lang.cpu_mhz << ": " << vtProp.intVal << " MHz" << std::endl;
        }
        VariantClear(&vtProp);

        pClassObject->Release();
    }

    // 释放资源
    pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
}

/**
 * @brief 
 * 
 */
void DisplayCPUThreadUsage(const MsgList& lang) {

}

/**
 * @brief 显示 GPU 信息
 * 
 * 该函数通过调用 nvidia-smi 命令获取 GPU 的基本信息，包括 GPU 名称、使用率和显存信息，并输出到控制台。
 * 
 * @param lang 语言配置，用于多语言支持
 */
void DisplayGPUInfo(const MsgList& lang) {
    std::cout << "\033[36m" << lang.gpu_info_title << "\033[0m\n";
    std::cout << "=========================\n";

    // 检查是否安装了 NVIDIA GPU
    // 这里假设系统已安装支持查询的 GPU 驱动（如 nvml 或类似库）
    if (system("nvidia-smi > NUL 2>&1") != 0) {
        std::cout << lang.gpu_no_detect << "\n";  // 如果没有 GPU 或 nvidia-smi 不可用
        return;
    }

    // 调用 nvidia-smi 获取详细信息
    FILE* pipe = _popen("nvidia-smi --query-gpu=gpu_name,utilization.gpu,utilization.memory,memory.total,memory.used,memory.free --format=csv,noheader,nounits", "r");
    if (!pipe) {
        std::cerr << "错误: 无法执行 nvidia-smi 命令。\n";
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::istringstream gpuData(buffer);
        std::string gpuName;
        int gpuUsage, memoryUsage;
        int totalMemory, usedMemory, freeMemory;

        // 解析 nvidia-smi 输出
        if (!(gpuData >> std::ws).getline(buffer, sizeof(buffer), ',')) 
            break;  // 读取 GPU 名称
        gpuName = buffer;

        gpuData >> gpuUsage;
        gpuData.ignore();
        gpuData >> memoryUsage;
        gpuData.ignore();
        gpuData >> totalMemory;
        gpuData.ignore();
        gpuData >> usedMemory;
        gpuData.ignore();
        gpuData >> freeMemory;

        // 输出 GPU 信息
        std::cout << "\033[33mGPU Name: \033[0m" << gpuName << "\n";
        std::cout << "  \033[32mGPU Utilization: \033[0m" << gpuUsage << "%\n";
        std::cout << "  \033[32mMemory Utilization: \033[0m" << memoryUsage << "%\n";
        std::cout << "  \033[32mTotal Memory: \033[0m" << totalMemory << " MB\n";
        std::cout << "  \033[32mUsed Memory: \033[0m" << usedMemory << " MB\n";
        std::cout << "  \033[32mFree Memory: \033[0m" << freeMemory << " MB\n";
    }

    _pclose(pipe);
}

/**
 * @brief 显示内存信息
 * 
 * 该函数通过调用 GlobalMemoryStatusEx 函数获取内存的基本信息，包括总内存、可用内存、交换空间等信息，并输出到控制台。
 * 
 * @param lang 语言配置，用于多语言支持
 *
 */
void DisplayMemoryInfo(const MsgList& lang) {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        std::cout << "\033[36m" << lang.memory_info_title << "\033[0m\n";
        std::cout << "=========================\n";
        std::cout << lang.mem_total << ": " << memInfo.ullTotalPhys / 1024 / 1024 << " MB" << std::endl;
        std::cout << lang.mem_free << ": " << memInfo.ullAvailPhys / 1024 / 1024 << " MB" << std::endl;
        std::cout << lang.swap_total << ": " << memInfo.ullTotalPageFile / 1024 / 1024 << " MB" << std::endl;
    } else {
        std::cerr << "无法获取内存信息。" << std::endl;
    }
}

/**
 * @brief 显示系统信息
 * 
 * 该函数通过调用 GetSystemInfo 函数获取系统信息，包括操作系统版本、处理器架构、物理内存等信息，并输出到控制台。
 * 
 */
char getkey() {
    int ch = _getch(); // 获取 int 类型的字符 
    char char_ch = (char)ch; // 将 int 类型转换为 char 类型
    return char_ch; // 返回 char 类型的字符
}