#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>
#include "message.h"
#include "system_info.h"

#ifdef _WIN32 
#include "windows_specific.cpp" 
#elif __linux__ 
#include "linux_specific.cpp"
#endif

// 使用原子指针存储语言，确保线程安全
std::atomic<const MsgList*> currentLang(&LANG_EN);  // 默认语言为英文
std::atomic<bool> stopFlag(false);  // 全局停止标志，确保线程安全
/**
 * @brief 显示系统信息
 */ 
void displayInfo() {
    while (!stopFlag.load()) {
        system("clear");  // 清屏
        // 获取当前语言
        const MsgList* lang = currentLang.load();
        // 显示系统信息
        GetOSInfo(*lang);
        std::cout << std::endl;
        DisplayCPUInfo(*lang);
        std::cout << std::endl;
        DisplayCPUThreadUsage(*lang);
        std::cout << std::endl;
        DisplayGPUInfo(*lang);
        std::cout << std::endl;
        DisplayMemoryInfo(*lang);
        std::this_thread::sleep_for(std::chrono::seconds(1));  // 每秒刷新一次
    }
}

/**
 * @brief 主函数
 * @return int
 */
int main() {
    // 开始
    std::cout << "Starting the program..." << std::endl;

    // 启动线程
    std::thread refreshThread(displayInfo);

    char userInput;
    while (true) {
        userInput = getch();  // 获取单个字符输入
        // 根据输入字符执行操作
        if (userInput == 'q') {
            stopFlag.store(true);  // 设置停止标志
            break;
        } else if (userInput == 'c') {
            currentLang.store(&LANG_CN);  // 切换为中文
        } else if (userInput == 'e') {
            currentLang.store(&LANG_EN);  // 切换为英文
        } else if (userInput == 'j') {
            currentLang.store(&LANG_JP);  // 切换为日文
        }
    }
    // 等待线程结束
    refreshThread.join();
    system("clear");
    std::cout << "Program exited successfully." << std::endl;
    return 0;
}

