//	VirtualDubMod - Video processing and capture application
//	Based on VirtualDub Copyright (C) 1998-2001 Avery Lee
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef f_VIRTUALDUBMOD_MESSAGES_H
#define f_VIRTUALDUBMOD_MESSAGES_H

#include <windows.h>

typedef struct _vdm_basic_range {
	int from;
	int to;
} VDM_BASIC_RANGE, * PVDM_BASIC_RANGE;

// These messages can be sent to the main window and in some cases the
// script editor to request information.
// wParam is the requesting windows handle, lParam a pointer to the respective
// struct. the tag field gets passed through and is meant for user
// information.

#define WM_VDM_REQUESTFILENAME		WM_USER + 42
typedef struct _vdm_filename {
	int tag;
	wchar_t *name;
} VDM_FILENAME, * PVDM_FILENAME;

#define WM_VDM_REQUESTPOS			WM_USER + 43
typedef struct _vdm_pos {
	int tag;
	int pos;
} VDM_POS, * PVDM_POS;

#define WM_VDM_REQUESTRANGE			WM_USER + 44
typedef struct _vdm_range {
	int tag;
	VDM_BASIC_RANGE range;
} VDM_RANGE, * PVDM_RANGE;

#define WM_VDM_REQUESTFRAMESET		WM_USER + 45
typedef struct _vdm_frameset {
	int tag;
	int count;
	PVDM_BASIC_RANGE ranges;	// must be assigned by the recipient
} VDM_FRAMESET, * PVDM_FRAMESET;

#define WM_VDM_REQUESTFRAMESIZE		WM_USER + 46
typedef struct _vdm_framesize {
	int tag;
	int frametype;				// 0 = input / 1 = output
	RECT frame;					// right and bottom hold width and height respectively
} VDM_FRAMESIZE, * PVDM_FRAMESIZE;

// These messages get sent back as answers to the requests or can be sent
// to set the according values (the latter being not fully implemented)
// lParam a pointer to the respective struct

#define WM_VDM_SENDFILENAME			WM_USER + 62
#define WM_VDM_SENDPOS				WM_USER + 63
#define WM_VDM_SENDRANGE			WM_USER + 64
#define WM_VDM_SENDFRAMESET			WM_USER + 65
#define WM_VDM_SENDFRAMESIZE		WM_USER + 66
#define WM_VDM_SCIEDIT_UPDATE		WM_USER + 101

#endif
