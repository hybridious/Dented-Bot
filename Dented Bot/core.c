#include <Windows.h>
#include <Shlobj.h>
#include "http.h"
#include "core.h"	
#include "stdlib.h"

typedef NTSTATUS (__stdcall *nt_RtlGetVersion)(
	_Out_ PRTL_OSVERSIONINFOW lpVersionInformation
	);
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
typedef BOOL(WINAPI *PGMSE)(LPMEMORYSTATUSEX);
LPFN_ISWOW64PROCESS fnIsWow64Process;
nt_RtlGetVersion RtlGetVersion;
HW_PROFILE_INFO hwProfileInfo;
char *getHwid()
{
	if (GetCurrentHwProfile(&hwProfileInfo) != 0) {
		return hwProfileInfo.szHwProfileGuid;
	}
		return "";
}
BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "IsWow64Process");

	if (fnIsWow64Process != NULL)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			
		}
	}
	return bIsWow64;
}
char *getSys() {

		if (IsWow64()) {
			return "x64";
		}
		else {
			return "x32";
		}
		return 0;
}
char out[6];
char *getversion() {
	HMODULE hNtDll = LoadLibraryA("ntdll.dll");
	if (hNtDll == NULL) {
		return 0;
	}
	RtlGetVersion = (nt_RtlGetVersion)GetProcAddress(hNtDll, "RtlGetVersion");
	RTL_OSVERSIONINFOW rtosv;
	RtlSecureZeroMemory(&rtosv, sizeof(rtosv));
	RtlGetVersion(&rtosv);
	
	wsprintf(out, "%lu.%lu", rtosv.dwMajorVersion, rtosv.dwMinorVersion);
	return out;
}
INT getDisk() { 
	int total, free;
	char Dnames[3];
	TCHAR ok[MAX_PATH];
	RtlSecureZeroMemory(ok, smstrlen(ok));
	ExpandEnvironmentStrings(TEXT("%SystemDrive%"), ok, MAX_PATH);
	wsprintf(Dnames, ok);
	//printf("Drive %s\n",Dnames);
	if (GetDriveType(Dnames) != DRIVE_FIXED) {
		//printf("not a fixed drive, skipping");
	}
	else {
		GetDiskFreeSpaceEx(Dnames, NULL, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
		return total / 1024 / 1024 / 1024;
	}
	return 0;
}

DISPLAY_DEVICE disp_dev_info;
char *getGPU() {
	int disp_num = 0;
	RtlSecureZeroMemory(&disp_dev_info, sizeof(DISPLAY_DEVICE));
	disp_dev_info.cb = sizeof(DISPLAY_DEVICE);
	EnumDisplayDevices(0, disp_num++, &disp_dev_info, 0x00000001);
	return disp_dev_info.DeviceString;
}
char CPUBrandString[0x40];
char *getProc() {
	//memset(CPUBrandString, 0, sizeof(CPUBrandString));
	RtlSecureZeroMemory(CPUBrandString, sizeof(CPUBrandString));
	// Get extended ids.
	int CPUInfo[4] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	// Get the information associated with each extended ID.
	
	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		
		// Interpret CPU brand string and cache information.
		if (i == 0x80000002)
		{
			//  m_memcpy( CPUBrandString, CPUInfo,     sizeof(CPUInfo));
			_memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000003)
		{
			//   m_memcpy( CPUBrandString + 16,            CPUInfo,            sizeof(CPUInfo));
			_memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		}
		else if (i == 0x80000004)
		{
			//   m_memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
			_memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));

		}
	}
	return CPUBrandString;
}
 
INT getRam() {
	// make my own GetProcAddress type
	PGMSE pGMSE = (PGMSE)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), TEXT("GlobalMemoryStatusEx"));
	if (pGMSE != 0)
	{
		MEMORYSTATUSEX mi;
		RtlSecureZeroMemory(&mi, sizeof(MEMORYSTATUSEX));
		mi.dwLength = sizeof(MEMORYSTATUSEX);
		if (pGMSE(&mi) == TRUE) {
			return mi.ullTotalPhys / 1048576;
		}
		else {
			pGMSE = 0;
		}
	}
		return 0;
	
}

char *IsAdmin() {
	if (IsElevated()) {
		return "yes";
	}
	else {
		return "no";
	}
	return 0;
}

BOOL IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}