#include "download.h"
#include <Windows.h>
#include <WinInet.h>
#include "../Config.h"
#define CHUNKSIZE 1024
HANDLE getfile(LPCSTR url, PBYTE *ppBuf, PDWORD pdwSize)
{
	DWORD dwAllocated = CHUNKSIZE * 5, dwRead = 0, dwTotalRead = 0;
	BOOL bDownloaded = FALSE;
	// can also do
	HANDLE hHeap = HeapCreate(0, 0, 0);
	if (hHeap == NULL) {
		return 0;
	}
	*ppBuf = (PBYTE)HeapAlloc(hHeap, 0, dwAllocated);
	if (ppBuf == NULL) {
		return 0;
	}
	PBYTE pTmp = NULL;
	LPCSTR szHeaders = "Accept: */*\r\n\r\n";
	HINTERNET hInternet = InternetOpenA(USERAGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet)
	{
		HINTERNET hFile = InternetOpenUrlA(hInternet, url, szHeaders, strlen(szHeaders), INTERNET_FLAG_RELOAD, 0);
		if (hFile)
		{
			do {
				bDownloaded = InternetReadFile(hFile, (LPVOID)(*ppBuf + dwTotalRead), CHUNKSIZE, &dwRead);
				dwTotalRead += dwRead;
				if (dwTotalRead + CHUNKSIZE > dwAllocated)
				{
					dwAllocated += CHUNKSIZE * 10;
					pTmp = HeapReAlloc(hHeap, 0, *ppBuf, dwAllocated);
					if (pTmp)
					{
						*ppBuf = pTmp;
					}
					else
					{
						HeapFree(hHeap, 0, *ppBuf);
						InternetCloseHandle(hFile);
						InternetCloseHandle(hInternet);
						*pdwSize = 0;
						*ppBuf = NULL;
						return NULL;
					}
				}
			} while (bDownloaded && dwRead);
			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInternet);
	}
	*ppBuf = HeapReAlloc(hHeap, 0, *ppBuf, dwTotalRead);
	if (ppBuf == NULL) {
		return 0;
	}
	*pdwSize = dwTotalRead;
	return hHeap;
}
