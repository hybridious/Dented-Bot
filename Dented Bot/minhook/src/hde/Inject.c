#include <Windows.h>
typedef HANDLE (WINAPI *ntCreateRemoteThread)(
	_In_ HANDLE hProcess,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	_In_ LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_ LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId
);
ntCreateRemoteThread NtCreateRemoteThread;
VOID Inject(PBYTE *buf, DWORD size) {
	HANDLE process = OpenProcess(PAGE_EXECUTE_READWRITE, NULL, 420);
	if (!OpenProcess) {
		return;
	}
	NtCreateRemoteThread = (ntCreateRemoteThread)GetProcAddress(GetModuleHandle("kernel32.lib"), "CreateRemoteThread");
	//NtCreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)ShittyThreadToInject, ShittyThreadToInject);
	//WriteProcessMemory(process, 0x00, )
}

VOID TrusterrBypass() {
	HWND hwd;
	GetProcessWIndo
	//rop injection by SetWindowLongPTr
	SetWindowLongPtr()

}