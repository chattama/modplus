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

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commdlg.h>
#include "ClippingControl.h"
#include "LevelControl.h"
#include "PositionControl.h"
#include "oshelper.h"

#include "I18NVersion.h"

// functions
bool I18N_Init(void);

HACCEL I18N_LoadAccelerators(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTableName   // address of table-name string
);

HICON I18N_LoadIcon(
  HINSTANCE hInstance, // handle to application instance
  LPCTSTR lpIconName   // icon-name string or icon resource 
                       // identifier
);

HMENU I18N_LoadMenu(HINSTANCE hInstance,  LPCTSTR lpMenuName);
int I18N_DialogBox(
	HINSTANCE hInstance,  // handle to application instance
	LPCTSTR lpTemplateName,  // identifies dialog box template
	HWND hWndParent,      // handle to owner window
	DLGPROC lpDialogFunc // pointer to dialog box procedure
);

int I18N_LoadString(
  HINSTANCE hInstance,  // handle to module containing string resource
  UINT uID,             // resource identifier
  LPTSTR lpBuffer,      // pointer to buffer for resource
  int nBufferMax        // size of buffer
);

HWND I18N_CreateDialog(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTemplate,   // identifies dialog box template name
  HWND hWndParent,      // handle to owner window
  DLGPROC lpDialogFunc  // pointer to dialog box procedure
);

HWND I18N_CreateDialogParam(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTemplateName,  // identifies dialog box template
  HWND hWndParent,      // handle to owner window
  DLGPROC lpDialogFunc, // pointer to dialog box procedure
  LPARAM dwInitParam    // initialization value
);

int I18N_DialogBox(
	HINSTANCE hInstance,  // handle to application instance
	LPCTSTR lpTemplateName,  // identifies dialog box template
	HWND hWndParent,      // handle to owner window
	DLGPROC lpDialogFunc // pointer to dialog box procedure
);

int I18N_DialogBoxParam(
	HINSTANCE hInstance,  // handle to application instance
	LPCTSTR lpTemplateName,  // identifies dialog box template
	HWND hWndParent,      // handle to owner window
	DLGPROC lpDialogFunc, // pointer to dialog box procedure
	LPARAM dwInitParam    // initialization value
);

BOOL I18N_GetOpenFileName(
  LPOPENFILENAME lpofn   // address of structure with initialization 
                         // data
);

BOOL I18N_GetSaveFileName(
  LPOPENFILENAME lpofn   // address of structure with initialization                          
						 // data
);

HGLOBAL LoadStuff(
  HINSTANCE hInstance,  // handle to module containing stuff resource
  LPCTSTR uID             // resource identifier
);

static bool CheckVersion(HMODULE hDll);
static void GetResourceVersion(char *fileName, char *pVersion, unsigned int length);
static UINT GetResVersion(UINT uID, UINT *iResVersion);

typedef UINT (__stdcall *fnResVersion)(UINT);
fnResVersion dllGetDialogResVersion;
fnResVersion dllGetMenuResVersion;
fnResVersion dllGetStringResVersion;

// variables
static HMODULE s_hResDll;

/*
 * body
 */

bool I18N_LoadLibrary(char *lib)
{
	bool rc = true;

	s_hResDll = LoadLibrary(lib);
	if (s_hResDll != NULL) {
		dllGetDialogResVersion = (fnResVersion)GetProcAddress(s_hResDll, "GetDialogResVersion");
		if (!dllGetDialogResVersion) rc = false;
		dllGetMenuResVersion = (fnResVersion)GetProcAddress(s_hResDll, "GetMenuResVersion");
		if (!dllGetMenuResVersion) rc = false;
		dllGetStringResVersion = (fnResVersion)GetProcAddress(s_hResDll, "GetStringResVersion");
		if (!dllGetStringResVersion) rc = false;
		if (!rc) {
			FreeLibrary(s_hResDll);
			s_hResDll = NULL;
		}
	}
	return rc;
}

bool I18N_Init(void)
{
	int n = 0;
	char language_code[10];
	char dll_name[MAX_PATH];

	if ((!QueryConfigString(NULL, "LanguageDLL", dll_name, MAX_PATH)) || (strcmp(dll_name, "") == 0)) { 
		// get language code (ISO639x 3-letter code) from system default locale
		n = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, 
			LOCALE_SABBREVLANGNAME, (LPTSTR)language_code, 10);
		if (n == 0) {
			return false;
		}
		sprintf(dll_name, "VDMLang%s.dll", language_code);
	}

	// Load localized resource dll


	return I18N_LoadLibrary(dll_name);
}
/*
HACCEL I18N_LoadAccelerators(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTableName   // address of table-name string
)
{
	HACCEL rc = NULL;

	if (s_hResDll == NULL) {
		rc = LoadAccelerators(hInstance, lpTableName);
	} else {
		rc = LoadAccelerators(s_hResDll, lpTableName);
		if (rc == NULL) {
			rc = LoadAccelerators(hInstance, lpTableName);
		}
	}

	return rc;
}

HICON I18N_LoadIcon(
  HINSTANCE hInstance, // handle to application instance
  LPCTSTR lpIconName   // icon-name string or icon resource 
                       // identifier
)
{
	HICON rc = NULL;

	if (s_hResDll == NULL) {
		rc = LoadIcon(hInstance, lpIconName);
	} else {
		rc = LoadIcon(s_hResDll, lpIconName);
		if (rc == NULL) {
			rc = LoadIcon(hInstance, lpIconName);
		}
	}

	return rc;
}
*/
HMENU I18N_LoadMenu(HINSTANCE hInstance,  LPCTSTR lpMenuName)
{
	HMENU rc = NULL;

	if (s_hResDll == NULL) {
		rc = LoadMenu(hInstance, lpMenuName);
	} else {
		if (GetResVersion((UINT)lpMenuName, iMenuVersion)>dllGetMenuResVersion((UINT)lpMenuName)) {
			rc = LoadMenu(hInstance, lpMenuName);
		} else {
			rc = LoadMenu(s_hResDll, lpMenuName);
			if (rc == NULL) {
				rc = LoadMenu(hInstance, lpMenuName);
			}
		}
	}

	return rc;
}

int I18N_LoadString(
  HINSTANCE hInstance,  // handle to module containing string resource
  UINT uID,             // resource identifier
  LPTSTR lpBuffer,      // pointer to buffer for resource
  int nBufferMax        // size of buffer
)
{
	int rc = 0;

	if (s_hResDll == NULL) {
		rc = LoadString(hInstance, uID, lpBuffer, nBufferMax);
	} else {
		if (GetResVersion(uID, iStringVersion)>dllGetMenuResVersion(uID)) {
			rc = LoadString(hInstance, uID, lpBuffer, nBufferMax);
		} else {
			rc = LoadString(s_hResDll, uID, lpBuffer, nBufferMax);
			if (rc == 0) {
				rc = LoadString(hInstance, uID, lpBuffer, nBufferMax);
			}
		}
	}

	return rc;
}

HWND I18N_CreateDialog(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTemplate,   // identifies dialog box template name
  HWND hWndParent,      // handle to owner window
  DLGPROC lpDialogFunc  // pointer to dialog box procedure
)
{
	HWND rc = NULL;

	if (s_hResDll == NULL) {
		rc = CreateDialog(hInstance, lpTemplate, hWndParent, lpDialogFunc);
	} else {
		if (GetResVersion((UINT)lpTemplate, iDialogVersion)>dllGetMenuResVersion((UINT)lpTemplate)) {
			rc = CreateDialog(hInstance, lpTemplate, hWndParent, lpDialogFunc);
		} else {
			rc = CreateDialog(s_hResDll, lpTemplate, hWndParent, lpDialogFunc);
			if (rc == NULL) {
				rc = CreateDialog(hInstance, lpTemplate, hWndParent, lpDialogFunc);
			}
		}
	}

	return rc;
}

HWND I18N_CreateDialogParam(
  HINSTANCE hInstance,  // handle to application instance
  LPCTSTR lpTemplateName,  // identifies dialog box template
  HWND hWndParent,      // handle to owner window
  DLGPROC lpDialogFunc, // pointer to dialog box procedure
  LPARAM dwInitParam    // initialization value
)
{
	HWND rc = NULL;

	if (s_hResDll == NULL) {
		rc = CreateDialogParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	} else {
		if (GetResVersion((UINT)lpTemplateName, iDialogVersion)>dllGetMenuResVersion((UINT)lpTemplateName)) {
			rc = CreateDialogParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
		} else {
			rc = CreateDialogParam(s_hResDll, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
			if (rc == NULL) {
				rc = CreateDialogParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
			}
		}
	}

	return rc;
}

int I18N_DialogBox(
	HINSTANCE hInstance,  // handle to application instance
	LPCTSTR lpTemplateName,  // identifies dialog box template
	HWND hWndParent,      // handle to owner window
	DLGPROC lpDialogFunc // pointer to dialog box procedure
)
{
	int rc = 0;

	if (s_hResDll == NULL) {
		rc = DialogBox(hInstance, lpTemplateName, hWndParent, lpDialogFunc);
	} else {
		if (GetResVersion((UINT)lpTemplateName, iDialogVersion)>dllGetMenuResVersion((UINT)lpTemplateName)) {
			rc = DialogBox(hInstance, lpTemplateName, hWndParent, lpDialogFunc);
		} else {
			HRSRC h = NULL;
			h = FindResource(s_hResDll, lpTemplateName, RT_DIALOG);
			if (h != NULL) {
				rc = DialogBox(s_hResDll, lpTemplateName, hWndParent, lpDialogFunc);
			} else {
				rc = DialogBox(hInstance, lpTemplateName, hWndParent, lpDialogFunc);
			}
		}
	}

	return rc;
}

int I18N_DialogBoxParam(
	HINSTANCE hInstance,  // handle to application instance
	LPCTSTR lpTemplateName,  // identifies dialog box template
	HWND hWndParent,      // handle to owner window
	DLGPROC lpDialogFunc, // pointer to dialog box procedure
	LPARAM dwInitParam    // initialization value
)
{
	int rc = 0;

	if (s_hResDll == NULL) {
		rc = DialogBoxParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	} else {
		if (GetResVersion((UINT)lpTemplateName, iDialogVersion)>dllGetMenuResVersion((UINT)lpTemplateName)) {
			rc = DialogBoxParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
		} else {
			HRSRC h = NULL;
			h = FindResource(s_hResDll, lpTemplateName, RT_DIALOG);
			if (h != NULL) {
				rc = DialogBoxParam(s_hResDll, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
			} else {
				rc = DialogBoxParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
			}
		}
	}

	return rc;
}

BOOL I18N_GetOpenFileName(
  LPOPENFILENAME lpofn   // address of structure with initialization 
                         // data
)
{
	BOOL rc = false;

	if (s_hResDll == NULL) {
		rc = GetOpenFileName(lpofn);
	} else {
		if (((lpofn->Flags | OFN_ENABLETEMPLATEHANDLE) != 0) || 
			((lpofn->Flags | OFN_ENABLETEMPLATE) != 0)) {
			if (GetResVersion((UINT)lpofn->lpTemplateName, iDialogVersion)<=dllGetMenuResVersion((UINT)lpofn->lpTemplateName)) {
				HRSRC h = NULL;
				h = FindResource(s_hResDll, lpofn->lpTemplateName, RT_DIALOG);
				if (h != NULL) {
					lpofn->hInstance = s_hResDll;
				}
			}
		}
		rc = GetOpenFileName(lpofn);
	}

	return rc;
}

BOOL I18N_GetSaveFileName(
  LPOPENFILENAME lpofn   // address of structure with initialization                          
						 // data
)
{
	BOOL rc = false;

	if (s_hResDll == NULL) {
		rc = GetSaveFileName(lpofn);
	} else {
		if (((lpofn->Flags | OFN_ENABLETEMPLATEHANDLE) != 0) || 
			((lpofn->Flags | OFN_ENABLETEMPLATE) != 0)) {
			if (GetResVersion((UINT)lpofn->lpTemplateName, iDialogVersion)<=dllGetMenuResVersion((UINT)lpofn->lpTemplateName)) {
				HRSRC h = NULL;
				h = FindResource(s_hResDll, lpofn->lpTemplateName, RT_DIALOG);
				if (h != NULL) {
					lpofn->hInstance = s_hResDll;
				}
			}
		}
		rc = GetSaveFileName(lpofn);
	}

	return rc;
}
/*
HGLOBAL LoadStuff(
  HINSTANCE hInstance,  // handle to module containing stuff resource
  LPCTSTR uID             // resource identifier
)
{
	HGLOBAL rc = NULL;
	HRSRC hRsrc = NULL;

	if (s_hResDll == NULL) {
		hRsrc = FindResource(hInstance, uID, "STUFF");
		if (hRsrc != NULL) {
			rc = LoadResource(hInstance, hRsrc);
		}
	} else {
		hRsrc = FindResource(s_hResDll, uID, "STUFF");
		if (hRsrc != NULL) {
			rc = LoadResource(s_hResDll, hRsrc);
		} else {
			hRsrc = FindResource(hInstance, uID, "STUFF");
			if (hRsrc != NULL) {
				rc = LoadResource(hInstance, hRsrc);
			}
		}
	}

	return rc;
}

static bool CheckVersion(HMODULE hDll)
{
	bool rc = true;
	char fileName[_MAX_PATH];
	char dllVersion[10], exeVersion[10];

	try {
		// get exe resource version string
		if (GetModuleFileName(NULL, (LPTSTR)fileName, _MAX_PATH) == 0)
			throw "Can't get exe name";
		GetResourceVersion(fileName, exeVersion, 10);
		
		// get dll resource version string
		if (GetModuleFileName(hDll, (LPTSTR)fileName, _MAX_PATH) == 0)
			throw "Can't get dll name";
		GetResourceVersion(fileName, dllVersion, 10);
		
		// compare them
		if (strcmp(exeVersion, dllVersion) == 0)
			rc = true;
		else
			rc = false;

	} catch (...) {
		rc = false;
	}
	
	return rc;
}

static void GetResourceVersion(char *fileName, char *pVersion, unsigned int length)
{
	WORD *pLangID;
	UINT tmpLen;
	unsigned int len = 0;
	DWORD size;
	char query[100];
	LPVOID ptr, pInfo;

	try {
		// get version infomation from module
		size = GetFileVersionInfoSize((LPTSTR)fileName, NULL);
		if (size == 0) throw "Can't get size of version info";
		pInfo = new char[size];
		if (GetFileVersionInfo(fileName, NULL, size, pInfo) == 0)
			throw "Can't get version info";

		// get resource version string from module
		if (VerQueryValue(pInfo, "\\VarFileInfo\\Translation", (LPVOID *)&pLangID, &tmpLen) == 0)
			throw "Can't get language code";
		sprintf(query, "\\StringFileInfo\\%04x%04x\\ResourceVersion", pLangID[0], pLangID[1]);
		if (VerQueryValue(pInfo, query, &ptr, &tmpLen) == 0)
			throw "Can't get version string";

		// copy it
		if (tmpLen >= length)
			len = length - 1;
		else
			len = tmpLen;
		strncpy(pVersion, (const char*)ptr, len);
		pVersion[len] = '\0';

		delete pInfo;
	} catch (char *msg) {
		delete pInfo;
		throw msg;
	}
}
*/
static UINT GetResVersion(UINT uID, UINT *iResVersion) {
	UINT *idPtr = iResVersion;

	while(idPtr[0]) {
		if (idPtr[0] == uID)
			return idPtr[1];
		idPtr += 2;
	}
	return (UINT)-1;
}

