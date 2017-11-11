#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#define ONESECOND 1000
#include "http.h"
#include "stdlib.h"
int main() {
//	FreeConsole();
	while (1) {
		PostRoutine();
		Sleep(5*1000);
	}
	//getchar();
	return 0;
}