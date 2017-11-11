#pragma once

#include <Windows.h>

typedef struct _trackres {
	LPCSTR result;
	DWORD length;
	DWORD track;
} track_result;

typedef struct _track {
	DWORD offset;
	LPCSTR buffer;
	DWORD bufferSize;

	track_result result;
} track_search;

#define TRACK_SUCCESS 1
#define TRACK_FAILURE 0

DWORD findFirstTrack(LPCSTR buffer, DWORD bufferSize, track_search* ts);
DWORD findNextTrack(track_search* ts);

DWORD isTrackOne(LPCSTR s, DWORD maxSize, PDWORD length);
DWORD isTrackTwo(LPCSTR s, DWORD maxSize, PDWORD length);