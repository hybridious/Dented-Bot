#include <Windows.h>
#include <Shlwapi.h>
#include "../stdlib.h"
#include "headers.h"


DWORD HeaderPrase(char* headerprase, DWORD headerprasesize, char*destination, DWORD destinationsize)
{
	char* temporary = 0;
	char* POSTdata = 0;
	char HOSTDATA[MAX_PATH];
	DWORD contentlengtha = 0;

	RtlSecureZeroMemory(HOSTDATA, MAX_PATH);
	if (sstrncmp(headerprase, "POST ", 5) == 0)
	{

		temporary = (char*)HeapAlloc(GetProcessHeap(), 0, headerprasesize + 1);
		if (temporary)
		{
			RtlSecureZeroMemory(temporary, headerprasesize + 1);
			for (unsigned int i = 0; i < headerprasesize; i++) {
				temporary[i] = headerprase[i];
			}
			if (GetPostData(temporary, NULL)> 0)
			{

				POSTdata = smstrstr(temporary, "\r\n\r\n");
				if (POSTdata)
				{
					POSTdata += 4;
					if (ParseHeader(temporary, "Host: ", HOSTDATA, sizeof(HOSTDATA)))
					{
							MessageBox(0, HOSTDATA, "HOST", MB_ICONHAND);

							MessageBox(0, POSTdata, "POST", MB_ICONHAND);
						
					}
				}

			}
			else
			{
				if (destination && destinationsize)
				{
					ParseHeader(temporary, "Host: ", destination, destinationsize);
					contentlengtha = -1;
				}
			}
			HeapFree(GetProcessHeap(), 0, temporary);
		}
	}
	return contentlengtha;
}

DWORD GetPostData(const void* contentbuffer, char* bufferdest)
{
	char* endingparam = 0;
	DWORD headerprasesize = 0;
// POST DATA HAS \r\n\r\n at the end before having post data after
	endingparam = smstrstr((LPCSTR*)contentbuffer, "\r\n\r\n");
	if (endingparam)
	{
		endingparam += 4;
		headerprasesize = strlen(endingparam);
		if (headerprasesize)
		{
			if (bufferdest)
			{
				for (unsigned int i = 0; i < headerprasesize; i++) {
					bufferdest[i] = endingparam[i];
				}
			}
		}
	}
	return headerprasesize;
}

BOOL ParseHeader(const void *contentbuffer, char* inputdata, char*destination, DWORD destinationsize)
{

	if (contentbuffer && inputdata && destination && destinationsize > 0)
	{
		DWORD startsize = 0;
		char* begginning = 0;
		char* endingparam = smstrstr((LPCSTR*)contentbuffer, inputdata);

		if (endingparam)
		{

			while (*endingparam++ != ' ');
			begginning = endingparam;
			while (*endingparam++ != '\r');
			*endingparam = 0;

			startsize = strlen(begginning);

			if (startsize > destinationsize)
				startsize = destinationsize;

			for (unsigned int i = 0; i < startsize; i++) {
				destination[i] = begginning[i];
			}
			destination[startsize - 1] = 0;

			return TRUE;
		}
	}
	return FALSE;
}



