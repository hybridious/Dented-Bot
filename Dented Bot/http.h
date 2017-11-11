#pragma once
#include <Windows.h>
VOID PostRoutine();

BOOL HttpTrack(CHAR *data);

BOOL HttpPost(CHAR *data);
BOOL dl(char *url);