/***********************************************************************
 utility.cpp - Misc utility functions

 Created 2001.08.24 by Warren Young
 $Revision: 1.1 $  $Date: 2003/02/17 19:41:32 $

 Copyright (c) 2001 by Warren Young

 This program is hereby released under the terms of the GPL.  See the
 file COPYING, which should have accompanied this program, for full
 license details.
***********************************************************************/

#include "stdafx.h"

#include "utility.h"


//// FileExists ////////////////////////////////////////////////////////
// Returns true if a file exists, false otherwise

bool
FileExists(const char* pcFileName)
{
	HANDLE h = CreateFile(pcFileName, 0, 0, 0, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 0);
	return (h != INVALID_HANDLE_VALUE);
}


//// Basename //////////////////////////////////////////////////////////
// Strips the path name and file extensions off of a path and returns
// that as pcStripped.

void
Basename(char* pcStripped, int nBufSize, const char* pcFileName)
{
	strncpy(pcStripped, pcFileName, nBufSize);
	
	char* pcCursor = strrchr(pcStripped, '\\');
	if (pcCursor != 0) {
		memmove(pcStripped, pcCursor + 1, strlen(pcCursor + 1) + 1);
	}

	pcCursor = strrchr(pcStripped, '.');
	if (pcCursor != 0) {
		*pcCursor = '\0';
	}
}