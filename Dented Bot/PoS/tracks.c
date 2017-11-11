#include "tracks.h"
#include "../stdlib.h"
#include "scan.h"

 


DWORD findFirstTrack(LPCSTR buffer, DWORD bufferSize, track_search* ts) {
	track_result res = { .result = NULL,.length = 0,.track = -1 };
	ts->result = res;
	ts->buffer = buffer;
	ts->bufferSize = bufferSize;
	ts->offset = 0;

	char* searcher = (char*)buffer;

	int length = -1;

	while (1) {
		if (ts->offset == (bufferSize - 1)) {
			break;
		}

		if (isTrackTwo(searcher, bufferSize - ts->offset, &length)) {
			ts->result.result = searcher;
			ts->result.length = length;
			ts->result.track = 2;
			ts->offset += length;
			return TRACK_SUCCESS;
		}
		else if (isTrackOne(searcher, bufferSize - ts->offset, &length)) {
			ts->result.result = searcher;
			ts->result.length = length;
			ts->result.track = 1;
			ts->offset += length;
			return TRACK_SUCCESS;
		}

		searcher++;
		ts->offset++;
	}

	return TRACK_FAILURE;
}

DWORD findNextTrack(track_search *ts) {
	track_result res = { .result = NULL,.length = 0,.track = -1 };
	ts->result = res;

	char* searcher = (char*)ts->buffer + ts->offset;

	DWORD length = -1;

	while (1) {
		if (ts->offset == (ts->bufferSize - 1)) {
			break;
		}

		if (isTrackTwo(searcher, ts->bufferSize - ts->offset, &length)) {
			ts->result.result = searcher;
			ts->result.length = length;
			ts->result.track = 2;
			ts->offset += length;
			return TRACK_SUCCESS;
		}
		else if (isTrackOne(searcher, ts->bufferSize - ts->offset, &length)) {
			ts->result.result = searcher;
			ts->result.length = length;
			ts->result.track = 1;
			ts->offset += length;
			return TRACK_SUCCESS;
		}

		searcher++;
		ts->offset++;
	}

	return TRACK_FAILURE;
}

DWORD isTrackOne(LPCSTR s, DWORD maxSize, PDWORD length) {
	if (*s != '%')
		return 0;

	if (maxSize <= 2)
		return 0;

	if (!IS_LETTER(s[1])) {
		return 0;
	}

	DWORD index = 2;

	while (index < maxSize) {

		char c = s[index];

		index++;

		if (c != '?') {
			continue;
		}

		int foundCaret = 0;

		if (!IsCharset(s, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!&*^.,/\\?=;% ", index - 1)) {
			return 0;
		}

		if (index > 150)
			return 0;

		if (index < 8)
			return 0;

		for (DWORD i = 0; i < index - 1; i++) {
			if (s[i] == '^') {
				foundCaret = 1;
			}
		}

		*length = index;

		return foundCaret;
	}

	return 0;
}

DWORD isTrackTwo(LPCSTR s, DWORD maxSize, PDWORD length) {
	if (*s != ';')
		return 0;

	DWORD index = 1;  // skip the initial ;

	while (index < maxSize) {
		char c = s[index];

		if (!IS_ALPHANUM(c) && c != '?' && c != '=')
			return 0;

		if (c == '?') {
			if (!IsCharset(s, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!&*^.,/\\?=;% ", index)) {
				return 0;
			}

			if (index > 150)
				return 0;

			if (index < 8)
				return 0;

			int foundEq = 0;

			for (DWORD i = 1; i < index; i++) {
				if (s[i] == '=') {
					foundEq = 1;
				}
				else {
					c = s[i];
					if (!IS_ALPHANUM(c)) {
						return 0;
					}
				}
			}

			*length = index;
			return foundEq;
		}
		index++;
	}

	return 0;
}