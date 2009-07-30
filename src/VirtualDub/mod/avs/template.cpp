/***********************************************************************
 template.cpp - Defines the TemplateInfo-related stuff.

 Created 2001.08.23 by Warren Young
 $Revision: 1.1 $  $Date: 2003/02/17 19:41:32 $

 Copyright (c) 2001 by Warren Young

 This program is hereby released under the terms of the GPL.  See the
 file COPYING, which should have accompanied this program, for full
 license details.
***********************************************************************/

#include "stdafx.h"

#include "template.h"

#include <strstream>
#include <fstream>

using namespace std;


////////////////////////////////////////////////////////////////////////
// Constants

static const char* kpcTemplatesDir = "template";
static const char* kpcAsyntherTag = "#ASYNTHER ";
static int kATLength = 10;


////////////////////////////////////////////////////////////////////////
// Globals

TemplateInfoList gTemplateList;


//// GetTemplateDescription ////////////////////////////////////////////
// Given a template file name, open that file and extract the 
// description and put it in the ti block.  If we succeed, return true,
// else false.

static bool
GetTemplateDescription(TemplateInfo& ti)
{
	ifstream ifs(ti.sFileName_.c_str());
	char ac[100];

	while (ifs) {
		ifs.getline(ac, sizeof(ac));
		if (ifs) {
			if ((strncmp(ac, kpcAsyntherTag, kATLength) == 0) &&
					(strlen(ac) > kATLength)) {
				ti.sDescription_.append(ac + kATLength);
				return true;
			}
		}
	}

	return false;
}


//// GetTemplateInfo ///////////////////////////////////////////////////
// Scan for a list of AVISynth script templates.  Returns false if we 
// encounter some error, or populates TemplateList and returns true
// otherwise.

bool
GetTemplateInfo()
{
	// Find program directory -- the templates will be under that 
	// directory.
    char acProgDir[_MAX_PATH];
    if (::GetModuleFileName(0, acProgDir, sizeof(acProgDir)) == 0) {
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//			AfxMessageBox(ac); 
// END ***************************************************************
		return false;
    }
	char* pc = strrchr(acProgDir, '\\');
	if (pc) {
		*pc = '\0';
	}

	// Handle the case where we're running the program from within 
	// MSVC++.  There are other ugly workarounds I could have chosen.
	// This one happens to be the most palatable.
	if (strstr(acProgDir, "\\Debug") != 0) {
		*(strrchr(acProgDir, '\\')) = '\0';
	}

	// Build the template file name mask
	char acTemplateMask[_MAX_PATH];
	_snprintf(acTemplateMask, sizeof(acTemplateMask), "%s\\%s\\*.avst",
			acProgDir, kpcTemplatesDir);

	// Find all the templates that match the file name mask
	int err = 0;
	WIN32_FIND_DATA wfd;
	HANDLE hFF = FindFirstFile(acTemplateMask, &wfd);
	if (hFF == INVALID_HANDLE_VALUE) {
		err = GetLastError();
	}
	else {
		do {
			TemplateInfo ti;
			ti.sFileName_ = acProgDir;
			ti.sFileName_ += '\\';
			ti.sFileName_ += kpcTemplatesDir;
			ti.sFileName_ += '\\';
			ti.sFileName_ += wfd.cFileName;
			if (GetTemplateDescription(ti)) {
				gTemplateList.push_back(ti);
			}
		} while (FindNextFile(hFF, &wfd));
		
		if ((err = GetLastError()) == ERROR_NO_MORE_FILES) {
			err = 0;
		}

		FindClose(hFF);
	}

	// Did everything go as planned?
	if (err != 0) {
		char ac[100];
		ostrstream outs(ac, sizeof(ac));
		outs << "Failed while scanning for Avisynth template files. " <<
				"Error code " << err << "." << ends;
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//			AfxMessageBox(ac); 
// END ***************************************************************
		return false;
	}
	else if (gTemplateList.size() == 0) {
		char ac[300];
		ostrstream outs(ac, sizeof(ac));
		outs << "Cannot find any template files in directory:\n\n" <<
				acProgDir << "\\" << kpcTemplatesDir;
#if !defined(_NDEBUG)
		outs << "\n\nMask: " << acTemplateMask;
#endif
		outs << ends;
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//			AfxMessageBox(ac); 
// END ***************************************************************
		return false;
	}
	else {
		return true;
	}
}


