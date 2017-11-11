#pragma once
#include <Windows.h>
#include "tracks.h"
#include "../stdlib.h"
typedef void(*track_handler)(const track_result* res);

#define SCAN_FAILED 0
#define SCAN_SUCCEEDED 1

#define TOTALVMRESERVE 0x00100000
#define PAGESIZE 0x1000

typedef	struct VMOBJECT* LPVMOBJECT;

DWORD ScanForTracks(track_handler handler);

DWORD GetBaseAddress(HANDLE process, LPCSTR processName);

VOID FindTracks(HANDLE process, DWORD baseAddress, track_handler handler);


void ForceUpload();

HANDLE queueMutex;
DWORD queueSize;;
LPCSTR uploadQueue[UPLOAD_QUEUE_SIZE];

void QueueUpload(track_result* res);

// this requires control of queueMutex pre-call
void ClearQueue();

DWORD WINAPI UploadTimer(LPVOID param);