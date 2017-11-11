#include "core.h"
#include "stdlib.h"
// some is from dietlibc
BOOL FileExists(LPCSTR file) {
	DWORD dwAttrib = GetFileAttributes(file);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

#define UC(c)	((unsigned char)c)

int sisxdigit(unsigned char c)
{
	if ((c >= UC('0') && c <= UC('9')) ||
		(c >= UC('a') && c <= UC('f')) ||
		(c >= UC('A') && c <= UC('F')))
		return 1;
	return 0;
}

#define UC(c)	((unsigned char)c)

int sisdigit(unsigned char c)
{
	if (c >= UC('0') && c <= UC('9'))
		return 1;
	return 0;
}


DWORD smstrlen(LPCSTR s) {
	CHAR* c = (CHAR*)s;

	DWORD a = 0;

	while (*c != 0) {
		c++;
		a++;
	}

	return a;
}
LPCSTR smstrstr(LPCSTR haystack, LPCSTR needle) {
	DWORD hi = 0, length = smstrlen(needle), counter = 0;

	while (1) {
		if (haystack[hi] == 0)
			break;

		if (needle[counter] == haystack[hi]) {
			counter++;

			if (counter == length)
				return &haystack[hi - counter + 1];
		}
		else {
			counter = 0;
		}

		hi++;
	}

	return NULL;
}

char *sstrchr(register const char *t, int c) {
	register char ch;

	ch = c;
	for (;;) {
		if ((*t == ch)) break; if ((!*t)) return 0; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
		if ((*t == ch)) break; if ((!*t)) return 0; ++t;
		if ((*t == ch)) break; if ((!*t)) return 0; ++t;
		if ((*t == ch)) break; if ((!*t)) return 0; ++t;
#endif
	}
	return (char*)t;
}
char *zstrtok(char *str, const char *delim) {
	static char *static_str = 0;      
	int index = 0, strlength = 0;  
	int found = 0;
	if (delim == 0 || (str == 0 && static_str == 0))
		return 0;

	if (str == 0)
		str = static_str;

	while (str[strlength])
		strlength++;

	for (index = 0; index<strlength; index++)
		if (str[index] == delim[0]) {
			found = 1;
			break;
		}

	if (!found) {
		static_str = 0;
		return str;
	}


	if (str[0] == delim[0]) {
		static_str = (str + 1);
		return (char *)delim;
	}


	str[index] = '\0';

	if ((str + index + 1) != 0)
		static_str = (str + index + 1);
	else
		static_str = 0;

	return str;
}
int convert(const char *input, int base) 
{
	char conversion1[] = { "0123456789abcdefghijklmnopqrstuvwxyz" };
	char conversion2[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	int index;

	for (index = 0; index < base; index++)
	{
		if (input[0] == conversion1[index] || input[0] == conversion2[index])
			return index;
	}
	return -1;
}
long zstrtol(const char *input, const char **Endptr, int base)
{
	int i, str_length, start_point = 0, sign = 1;
	long result = 0;

	str_length = strlen(input);
	if (base && (base < 2 || base > 36))
	{
		return 0;
	}
	if (str_length == 0)
	{
		return 0;
	}

	for (i = 0; i < str_length; i++)
	{
		if (input[start_point] == ' ' || input[start_point] == '\t' || input[start_point] == '\r' || input[start_point] == '\n' || input[start_point] == '+')
			start_point++;
		else
		{
			break;
		}
	}

	if (input[start_point] == '-')
	{
		sign = (-1);
		start_point++;
	}

	if (base == 16)
	{
		if (input[start_point] == '0' && (input[start_point + 1] == 'x' || input[start_point + 1] == 'X'))
		{
			start_point += 2;
		}
	}
	if (base == 8)
		if (input[start_point] == '0')
			start_point++;

	if (base == 0)
	{
		if (input[start_point] == '0')
		{
			if (input[start_point + 1] == 'x' || input[start_point + 1] == 'X')
			{
				base = 16;
				start_point += 2;
			}
			else
			{
				base = 8;
				start_point++;
			}
		}
		else base = 10;
	}

	for (i = start_point; i < str_length; i++)
	{
		if (convert(&input[i], base) == (-1))
		{
			str_length = i;
			*Endptr = &input[i];
			break;
		}
	}
	if (str_length - start_point == 0)
		return 0;


	for (i = start_point; i<str_length; i++)
	{
		if (convert(&input[i], base) != -1)
		{
			if (result>((LONG_MAX - convert(&input[i], base)) / base))
			{
				{
					if (sign == 1)
					{
						return LONG_MAX;
					}
					else return LONG_MIN;
				}
			}
			result = convert(&input[i], base) + result*base;
		}
	}
	return sign *result;

}

size_t swcslen(const wchar_t* s) {
	size_t i;
	for (i = 0; s[i]; ++i);
	return i;
}

void *_memcpy(void* dest, const void* src, size_t count) {
	char* dst8 = (char*)dest;
	char* src8 = (char*)src;

	while (count--) {
		*dst8++ = *src8++;
	}
	return dest;
}
void* m_memset(void * dst, int s, size_t count) {
	register char * a = dst;
	count++;	/* this actually creates smaller code than using count-- */
	while (--count)
		*a++ = s;
	return dst;
}


CHAR* smmemdup(LPCSTR mem, DWORD allocSize, DWORD copySize) {
	CHAR* s = HeapAlloc(GetProcessHeap(), 0, allocSize);
	if (s == NULL) {
		return "fail";
	}
	_memcpy(s, mem, copySize);
	return s;


}

BOOL IsCharset(LPCSTR s, LPCSTR charset, DWORD length) {
	DWORD i, i2;

	DWORD clen = smstrlen(charset);

	DWORD properChars = 0;

	for (i = 0; i < length; i++) {
		for (i2 = 0; i2 < clen; i2++) {
			if (s[i] == charset[i2]) {
				properChars++;
				break;
			}
		}
	}

	return properChars == length;
}

int zisspace(int ch)
{
	return (unsigned int)(ch - 9) < 5u || ch == ' ';
}


double zstrtod(const char* s, char** endptr) {
	const char*  p = s;
	double     value = 0.0;
	int                   sign = 1;
	double              factor = 0.0;
	unsigned int          expo = 0.0;

	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;

	switch (*p) {
	case '-': sign = -1;
	case '+': p++;
	default: break;
	}

	while ((unsigned int)(*p - '0') < 10u)
		value = value * 10 + (*p++ - '0');

	if (*p == '.') {
		factor = 1.0;

		p++;
		while ((unsigned int)(*p - '0') < 10u) {
			factor *= 0.1;
			value += (*p++ - '0') * factor;
		}
	}

	if ((*p | 32) == 'e') {
		expo = 0;
		factor = 10.;

		switch (*++p) {
		case '-': factor = 0.1;
		case '+': p++;
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			break;
		default: value = 0.0;
			p = s;
			goto done;
		}

		while ((unsigned int)(*p - '0') < 10u)
			expo = 10 * expo + (*p++ - '0');

		while (1) {
			if (expo & 1)
				value *= factor;
			if ((expo >>= 1) == 0)
				break;
			factor *= factor;
		}
	}

done:
	if (endptr != NULL)
		*endptr = (char*)p;

	return value * sign;
}

int stolower(int ch) {
	if ((unsigned int)(ch - 'A') < 26u)
		ch += 'a' - 'A';
	return ch;
}


unsigned long int Strtoul(const char *ptr, char **endptr, int base)
{
	int neg = 0, overflow = 0;
	unsigned long int v = 0;
	const char* orig;
	const char* nptr = ptr;

	while (zisspace(*nptr)) ++nptr;

	if (*nptr == '-') { neg = 1; nptr++; }
	else if (*nptr == '+') ++nptr;
	orig = nptr;
	if (base == 16 && nptr[0] == '0') goto skip0x;
	if (base) {
		register unsigned int b = base - 2;
		if ((b>34)) { SetLastError(EINVAL); return 0; }
	}
	else {
		if (*nptr == '0') {
			base = 8;
		skip0x:
			if ((nptr[1] == 'x' || nptr[1] == 'X') && sisxdigit(nptr[2])) {
				nptr += 2;
				base = 16;
			}
		}
		else
			base = 10;
	}
	while ((*nptr)) {
		register unsigned char c = *nptr;
		c = (c >= 'a' ? c - 'a' + 10 : c >= 'A' ? c - 'A' + 10 : c <= '9' ? c - '0' : 0xff);
		if ((c >= base)) break;	/* out of base */
		{
			register unsigned long x = (v & 0xff)*base + c;
			register unsigned long w = (v >> 8)*base + (x >> 8);
			if (w>(ULONG_MAX >> 8)) overflow = 1;
			v = (w << 8) + (x & 0xff);
		}
		++nptr;
	}
	if ((nptr == orig)) {		/* no conversion done */
		nptr = ptr;
		SetLastError(EINVAL);
		v = 0;
	}
	if (endptr) *endptr = (char *)nptr;
	if (overflow) {
		SetLastError(ERANGE);
		return ULONG_MAX;
	}
	return (neg ? v : v);
}
int sstrncmp(const char *s1, const char *s2, size_t n) {
	register const unsigned char* a = (const unsigned char*)s1;
	register const unsigned char* b = (const unsigned char*)s2;
	register const unsigned char* fini = a + n;
	while (a != fini) {
		register int res = *a - *b;
		if (res) return res;
		if (!*a) return 0;
		++a; ++b;
	}
	return 0;
}
int zstrncmp(const char *s1, const char *s2, size_t n) {
	register const unsigned char* a = (const unsigned char*)s1;
	register const unsigned char* b = (const unsigned char*)s2;
	register const unsigned char* fini = a + n;
	while (a != fini) {
		register int res = *a - *b;
		if (res) return res;
		if (!*a) return 0;
		++a; ++b;
	}
	return 0;
}
wchar_t* swcscat(wchar_t * dest, const wchar_t *src) {
	wchar_t* orig = dest;
	for (; *dest; ++dest);	/* go to end of dest */
	for (; (*dest = *src); ++src, ++dest);	/* then append from src */
	return orig;
}

wchar_t* swcscpy(wchar_t * dest, const wchar_t * src) {
	wchar_t* orig = dest;
	for (; (*dest = *src); ++src, ++dest);
	return orig;
}





