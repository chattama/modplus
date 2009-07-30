/***********************************************************************
 genscript.cpp - Generates script files from templates.

 Created 2001.04.13 (Friday the thirteenth!) by Warren Young
 $Revision: 1.1 $  $Date: 2003/02/17 19:41:32 $

 Portions copyright (c) 2001 by Educational Technology Resources, Inc.
 Portions copyright (c) 2001 by Warren Young

 This program is hereby released under the terms of the GPL.  See the
 file COPYING, which should have accompanied this program, for full
 license details.
***********************************************************************/

#include "stdafx.h"

#include "genscript.h"

#include "cmdline.h"
#include "utility.h"

#include <fstream>
#include <strstream>

using namespace std;


////////////////////////////////////////////////////////////////////////
// Constants

static const char* kpcWarningTag = "#AVISynthesizer WARNING: ";


//// OpenScript ////////////////////////////////////////////////////////
// Given a video file name, open a new *.avs file named after it.
// Returns false if this fails.

static bool
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//OpenScript(ofstream& Script, const string& sVideoFile,
//		const string& sTemplateFile)
OpenScript(ofstream& Script, const string& sVideoFile,
		const string& sTemplateFile, char * pcScriptName)
// END ***************************************************************
{
	// Max tries to create a new script file before we give up.  If we
	// hit this maximum, the user needs to intervene -- something's
	// very wrong with their system.
	static const int kMaxTries = 100;

	// Get base name of input file names
	char acSNBase[MAX_PATH];
	char acTemplateBase[MAX_PATH];
	Basename(acSNBase, sizeof(acSNBase), sVideoFile.c_str());
	Basename(acTemplateBase, sizeof(acTemplateBase), 
			sTemplateFile.c_str());

	// cd to the location of the first file passed on the command line.
	// This is required on Win9x, which sets the CWD to c:\, whereas 
	// Win2K puts you in the files' directory, if you run asynther.exe 
	// from Explorer.  If you run asynther by hand against files not in
	// the current directory, this is still helpful on Win2K.
	char acFirstFileDir[MAX_PATH];
	strcpy(acFirstFileDir, sVideoFile.c_str());
	char* pcCursor = strrchr(acFirstFileDir, '\\');
	if (pcCursor != 0) {
		*pcCursor = '\0';
		SetCurrentDirectory(acFirstFileDir);
	}

	// Check the file named video--template.avs.  If it exists, append
	// a number before the ".avs" and try again until we succeed or we
	// have tried too many times.
	char acScriptName[MAX_PATH];
	int nScriptNum = 0;
	do {
		int nLen = _snprintf(acScriptName, sizeof(acScriptName), 
				"%s--%s", acSNBase, acTemplateBase);
		++nScriptNum;
		if ((nScriptNum > 1) && (nLen < (MAX_PATH - 5))) {
			nLen += _snprintf(acScriptName + nLen, 
					sizeof(acScriptName) - nLen,
					"%d", nScriptNum);
		}
		if (nLen < (MAX_PATH - 5)) {
			strcat(acScriptName, ".avs");
		}
			
		if (!FileExists(acScriptName)) {
			break;
		}
	} while (nScriptNum < kMaxTries);

	// Do or die: open the script or complain that we gave up looking 
	// for an unused script name.
	if (nScriptNum < kMaxTries) {
		Script.open(acScriptName);
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
		strcpy(pcScriptName, acFirstFileDir);
		strcat(pcScriptName, "\\");
		strcat(pcScriptName, acScriptName);
// END ***************************************************************
		return Script.is_open();
	}
	else {
/*		AfxMessageBox("Too many retries while trying to create "
				"the generated script file.");*/ //MOD
		return false;
	}
}


//// FindRepeatingSection //////////////////////////////////////////////
// Given a line of text, find the section in square brackets, if any.
// If we find one, remove the repeating section including the brackets
// and return the repeating section and a pointer to the place in the
// input buffer where that repeating section came from.  If one or both 
// brackets are missing, leave them alone.

static void
FindRepeatingSection(char* pcLine, char* pcRep, 
		const char** ppcRepStart)
{
	*ppcRepStart = 0;
	
	char* pcRS = strchr(pcLine, '[');
	if (!pcRS) return;
	char* pcRE = strrchr(pcLine, ']');
	if (!pcRE) return;

	memcpy(pcRep, pcRS + 1, pcRE - pcRS - 1);
	pcRep[pcRE - pcRS - 1] = '\0';
	memmove(pcRS, pcRE + 1, strlen(pcRE + 1) + 1);

	*ppcRepStart = pcRS;
}


//// CreateScript //////////////////////////////////////////////////////
// Create an AVISynth script based on the given template.

bool
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
// CreateScript(const char* pcTemplateFileName)
CreateScript(const char* pcTemplateFileName, char* pcScriptName)
// END ***************************************************************
{
	ifstream Template(pcTemplateFileName);
	ofstream Script;
	char acLine[100], acRep[100];

// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//	if (!OpenScript(Script, *(gFileList.begin()), pcTemplateFileName)) { 
	if (!OpenScript(Script, *(gFileList.begin()), pcTemplateFileName, pcScriptName)) { 
// END ***************************************************************
		return false;
	}

	while (Template) {
		Template.getline(acLine, sizeof(acLine));
		if (!Template) {
			continue;
		}

		if (acLine[0] == '#') {
			// Don't bother processing this line.  Just a comment.
			Script << acLine << endl;
			continue;
		}

		const char* pcRepStart;
		FindRepeatingSection(acLine, acRep, &pcRepStart);
		char* pcFileNameLoc;
		if (pcRepStart == 0) {
			// No repeating section on this line, so look for file name 
			// location within full-line buffer
			pcFileNameLoc = strstr(acLine, "%f");
		}
		else {
			// Look for file name location within the repeating section.
			pcFileNameLoc = strstr(acRep, "%f");
		}

		if (!pcFileNameLoc && !pcRepStart) {
			// No repeating section or %f on this line, so echo it
			// literally.
			Script << acLine << endl;
		}
		else if (pcFileNameLoc && !pcRepStart) {
			// Simple file name substitution
			*pcFileNameLoc = '\0';
			Script << acLine << *(gFileList.begin()) << 
					(pcFileNameLoc + 2) << endl;
			if (gFileList.size() > 1) {
				Script << kpcWarningTag << "Ignored " <<
						(gFileList.size() - 1) << 
						" file(s) on command line." << endl;
			}
		}
		else if (pcFileNameLoc && pcRepStart) {
			// Multiple file name substitution
			Script.write(acLine, pcRepStart - acLine);
			*pcFileNameLoc = '\0';
			SLConstIterator it = gFileList.begin();
			while (it != gFileList.end()) {
				Script << acRep << *it << (pcFileNameLoc + 2);
				++it;
				if (it != gFileList.end()) {
					Script << " + ";
				}
			}
			Script << pcRepStart << endl;
		}
		else {
			// Error: no file name location given, but repeating section
			// defined.  Yell at user.
			char ac[200];
			ostrstream outs(ac, sizeof(ac));
			outs << "Probable script error: no file name location was "
					"given in the script, but the script defines a "
					"repeating section. Either the script is broken, "
					"or AVISynthesizer is." << ends;
// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
//			AfxMessageBox(ac); 
// END ***************************************************************
			return false;
		}
	}

// *******************************************************************
// *** Modification	for inclusion with VirtualDub AVS			   ***
// *** Tobias Minich, September 2002							   ***
// BEGIN *************************************************************
	return true;
}


