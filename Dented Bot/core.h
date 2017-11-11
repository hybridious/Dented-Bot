#pragma once
#include <Windows.h>
BOOL IsWow64();

void *_memcpy(void* dest, const void* src, size_t count);
LPCSTR smstrstr(LPCSTR haystack, LPCSTR needle);
INT getDisk();
char *getversion();
char *getGPU();
char *getProc();
INT getRam();
char *IsAdmin();
char *getSys();
BOOL IsElevated();
char *getHwid();
