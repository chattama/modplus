//	VirtualDub - Video processing and capture application
//	Copyright (C) 1998-2001 Avery Lee
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
#ifndef f_I18N_H
#define f_I18N_H

#include <windows.h>
#include <commdlg.h>

// functions
bool I18N_Init(void);
bool I18N_LoadLibrary(char *);
/*
HACCEL I18N_LoadAccelerators(HINSTANCE, LPCTSTR);
HICON I18N_LoadIcon(HINSTANCE, LPCTSTR);
*/
HMENU I18N_LoadMenu(HINSTANCE,  LPCTSTR);
int I18N_LoadString(HINSTANCE, UINT, LPTSTR, int);
HWND I18N_CreateDialog(HINSTANCE, LPCTSTR, HWND, DLGPROC);
HWND I18N_CreateDialogParam(HINSTANCE, LPCTSTR, HWND, DLGPROC, LPARAM);
int I18N_DialogBox(HINSTANCE, LPCTSTR,	HWND, DLGPROC);
int I18N_DialogBoxParam(HINSTANCE, LPCTSTR,	HWND, DLGPROC, LPARAM);
BOOL I18N_GetOpenFileName(LPOPENFILENAME);
BOOL I18N_GetSaveFileName(LPOPENFILENAME);
/*
HGLOBAL LoadStuff(
  HINSTANCE hInstance,  // handle to module containing stuff resource
  LPCTSTR uID             // resource identifier
);
*/
// macros that override standard API function
/*
#undef LoadAccelerators
#define LoadAccelerators(A, B) I18N_LoadAccelerators((A), (B))

#undef LoadIcon
#define LoadIcon(A, B) I18N_LoadIcon((A), (B))
*/
#undef LoadMenu
#define LoadMenu(A, B) I18N_LoadMenu((A), (B))

#undef LoadString
#define LoadString(A, B, C, D) I18N_LoadString((A), (B), (C), (D))

#undef CreateDialog
#define CreateDialog(A, B, C, D) I18N_CreateDialog((A), (B), (C), (D))

#undef CreateDialogParam
#define CreateDialogParam(A, B, C, D, E) I18N_CreateDialogParam((A), (B), (C), (D), (E))

#undef DialogBox
#define DialogBox(A, B, C, D) I18N_DialogBox((A), (B), (C), (D))

#undef DialogBoxParam
#define DialogBoxParam(A, B, C, D, E) I18N_DialogBoxParam((A), (B), (C), (D), (E))

#undef GetOpenFileName
#define GetOpenFileName(A) I18N_GetOpenFileName((A))

#undef GetSaveFileName
#define GetSaveFileName(A) I18N_GetSaveFileName((A))

#endif // f_I18N_H