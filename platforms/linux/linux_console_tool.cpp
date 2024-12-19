#include <stdlib.h>
#include <string>
#include "console_tool.h"

void clearScreen () {
    system("clear");  // 清屏
}

void setConsoleEncode(const std::string& encode) {
    system(("chcp " + encode).c_str());  // 设置控制台编码
}