#ifndef MONITOR_H
#define MONITOR_H

// 多国语言支持
struct MsgList {
    const char* os_info_title;            // 操作系统信息标题
    const char* os_name;                  // 操作系统名称
    const char* os_version;               // 操作系统版本
    const char* os_kernel;                // 操作系统内核版本
    const char* os_architecture;          // 操作系统架构

    const char* cpu_info_title;           // CPU 信息标题
    const char* cpu_model;                // 处理器型号
    const char* cpu_mhz;                  // 处理器频率 (MHz)

    const char* cpu_thread_usage_title;   // CPU 线程信息标题
    const char* cpu_thread_usage;         // CPU 线程使用率
    const char* cpu_thread_usage_unit;    // CPU 线程使用率单位

    const char* gpu_info_title;           // GPU 信息标题
    const char* gpu_no_detect;            // 未检测到 GPU 的提示信息
    const char* gpu_name;                 // GPU 名称
    const char* gpu_usage;                // GPU 使用率
    const char* gpu_mem_usage;            // 显存使用率
    const char* gpu_mem_total;            // 显存总量
    const char* gpu_mem_used;             // 已用显存
    const char* gpu_mem_free;             // 可用显存

    const char* memory_info_title;        // 内存信息标题
    const char* mem_total;                // 总内存
    const char* mem_free;                 // 空闲内存
    const char* swap_total;               // 交换分区总量
    const char* swap_free;                // 交换分区空闲量
};

// 定义不同语言的信息
extern const MsgList LANG_CN;   // 中文
extern const MsgList LANG_EN;   // 英文
extern const MsgList LANG_JP;   // 日文

// 函数接口
void GetOSInfo(const MsgList& lang);
void DisplayCPUInfo(const MsgList& lang);
void DisplayCPUThreadUsage(const MsgList& lang);
void DisplayGPUInfo(const MsgList& lang);
void DisplayMemoryInfo(const MsgList& lang);

// 中文
const MsgList LANG_CN = {
    "操作系统信息",
    "操作系统名称",
    "操作系统版本",
    "操作系统内核版本",
    "操作系统架构",

    "CPU 信息", 
    "处理器型号", 
    "处理器频率 (MHz)", 

    "CPU 线程信息", 
    "CPU 线程使用率",    // 示例字段（如果有多个线程的显示）
    "%",                 // 使用率单位（百分比）

    "GPU 信息", 
    "未检测到 NVIDIA GPU 或未安装 nvidia-smi。",
    "GPU 名称", 
    "GPU 使用率", 
    "显存使用率", 
    "显存总量", 
    "已用显存", 
    "可用显存", 

    "内存信息", 
    "总内存", 
    "空闲内存", 
    "交换分区总量", 
    "交换分区空闲量"
};

// 英文
const MsgList LANG_EN = {
    "Operating System Information",
    "Operating System Name",
    "Operating System Version",
    "Operating System Kernel Version",
    "Operating System Architecture",

    "CPU Information", 
    "Processor Model", 
    "Processor Frequency (MHz)", 

    "CPU Thread Information", 
    "CPU Thread Usage",   // 示例字段（如果有多个线程的显示）
    "%",                  // Usage unit (percentage)

    "GPU Information", 
    "No NVIDIA GPU detected or nvidia-smi not installed.",
    "GPU Name", 
    "GPU Utilization", 
    "Memory Utilization", 
    "Total Memory", 
    "Used Memory", 
    "Free Memory",

    "Memory Information", 
    "Total Memory", 
    "Free Memory", 
    "Swap Total", 
    "Swap Free"
};

// 日语
const MsgList LANG_JP = {
    "オペレーティングシステム情報",
    "オペレーティングシステム名",
    "オペレーティングシステムバージョン", 
    "オペレーティングシステムカーネルバージョン",
    "オペレーティングシステムアーキテクチャ",

    "CPU 情報", 
    "プロセッサモデル", 
    "プロセッサ周波数 (MHz)", 

    "CPU スレッド情報", 
    "CPU スレッド使用率",    // 示例字段（如果有多个线程的显示）
    "%",                      // 使用率単位（百分比）

    "GPU 情報", 
    "NVIDIA GPU が検出されていないか、nvidia-smi がインストールされていません。",
    "GPU 名称", 
    "GPU 使用率", 
    "メモリ使用率", 
    "総メモリ", 
    "使用済みメモリ", 
    "空きメモリ", 

    "メモリ情報", 
    "総メモリ", 
    "空きメモリ", 
    "スワップ総量", 
    "スワップ空き量"
};




#endif // MONITOR_H
