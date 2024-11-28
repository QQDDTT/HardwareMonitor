#pragma once

#include "message.h"

void GetOSInfo(const MsgList& lang);
void DisplayCPUInfo(const MsgList& lang);
void DisplayCPUThreadUsage(const MsgList& lang);
void DisplayGPUInfo(const MsgList& lang);
void DisplayMemoryInfo(const MsgList& lang);
char getch();
