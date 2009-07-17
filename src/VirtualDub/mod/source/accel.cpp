//	VirtualDubMod - Video processing and capture application based on VirtualDub by Avery Lee
//	Copyright (C) 1998-2002 VirtualDubMod Developers
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

//////////////////////////////////////////////////////////////////////////////
// Configurable hotkeys for main window, capture window and script editor	//
// Tobias Minich, November 2002												//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//																			//
// IMPORTANT NOTICE FOR DEVELOPERS!!                                        //
//																			//
// - Accelerator tables for main, capture & script editor are gone now,		//
//	 being replaced by this code. It is no longer necessary to put hotkeys	//
//	 into the menu resource.												//
//																			//
// - Whenever you add a menu item, include it here. Some user might want to	//
//	 define a hotkey for it.												//
//																			//
// - To add a new item:														//
//		* APPEND the menu id TO THE END of the respective				 	//
//		  g_accel____Command array.											//
//		  (NEVER INSERT! This will completely fubar changes made by the		//
//		  user!)															//
//		* append a default keymapping to g_accel____Default. I hope what's	//
//		  already there is enough to figure out how to do it.				//
//		* increase the respective VDM_ACCEL_____COUNT define in accel.h by	//
//		  the number of items you added.									//
//		* if you added a menu item, thats all. If you want to define a hot-	//
//		  key without a menu item do as above but you also have to add a	//
//		  string resource with the same id describing what the key does.	//
//		  (see ID_AVS_STARTCOMPLETE for an example)							//
//																			//
// - To remove an item:														//
//		* NEVER EVER DELETE IT! (Thats for the same reasons mentioned above)//
//		* Replace the command by ID_COMMAND_DEPRECATED (defined as 0)		//
//		* Put zeros as default keybinding.									//
//																			//
// - If you change a menu id, simply do so here								//
//																			//
// I hope this is clear enough. If you have questions, dont hesitate to ask //
//																			//
//////////////////////////////////////////////////////////////////////////////


#pragma warning(disable : 4786)

#define f_ACCEL_CPP

#include "stdafx.h"

#include "resource.h"
#include "accel.h"
#include "gui.h"
#include <string>

extern HINSTANCE g_hInst;

// Script Editor

WORD g_accelAVSCommand[VDM_ACCEL_AVS_COUNT] = {
	ID_AVS_COMMENT_LINES,
	ID_AVS_INSERT_FRAMESET,
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_AVS_INSERT_POS,
	ID_AVS_INSERT_RANGE,
	ID_AVS_FILE_SAVE,
	ID_AVS_INSERT_TRIM,
	ID_AVS_UNCOMMENT_LINES,
	ID_HELP_KEYS,
	ID_REFRESH,
	ID_AVS_SAVE_OPEN,
	ID_AVS_STARTCOMPLETE,
	ID_EDIT_REDO,
	ID_EDIT_UNDO,
	ID_AVS_SCRIPT_NONE,
	ID_AVS_SCRIPT_AVS,
	ID_FILE_REVERT,
	ID_FILE_EXIT,
	ID_AVS_EDIT_LINE,
	ID_HELP_TOFF,
	ID_HELP_AVISYNTH,
	ID_EDIT_FIND,
	ID_EDIT_FINDNEXT,
	ID_AVS_SCRIPT_DECOMB,
	ID_AVS_INSERT_FILENAME,
	ID_AVS_INSERT_CROP
	,ID_EDIT_FINDPREV
};

extern const ACCELKEYTABLE_AVS g_accelAVSDefault = {{
	{{{FVIRTKEY|FALT|FCONTROL|FSHIFT,	VK_C},{0, 0}}},							//ID_AVS_COMMENT_LINES,
	{{{FVIRTKEY|FCONTROL,				VK_I},{0, 0}}},							//ID_AVS_INSERT_FRAMESET,
	{{{FVIRTKEY|FCONTROL,				VK_N},{0, 0}}},							//ID_FILE_NEW,
	{{{FVIRTKEY|FCONTROL,				VK_O},{0, 0}}},							//ID_FILE_OPEN,
	{{{FVIRTKEY|FCONTROL,				VK_P},{0, 0}}},							//ID_AVS_INSERT_POS,
	{{{FVIRTKEY|FCONTROL,				VK_R},{0, 0}}},							//ID_AVS_INSERT_RANGE,
	{{{FVIRTKEY|FCONTROL,				VK_S},{FVIRTKEY,			VK_F2}}},		//ID_AVS_FILE_SAVE,
	{{{FVIRTKEY|FCONTROL,				VK_T},{0, 0}}},							//ID_AVS_INSERT_TRIM,
	{{{FVIRTKEY|FALT|FCONTROL|FSHIFT,	VK_U},{0, 0}}},							//ID_AVS_UNCOMMENT_LINES,
	{{{FVIRTKEY,						VK_F1},{0, 0}}},						//ID_HELP_KEYS,
	{{{FVIRTKEY,						VK_F5},{0, 0}}},						//ID_REFRESH,
	{{{FVIRTKEY,						VK_F7},{0, 0}}},						//ID_AVS_SAVE_OPEN,
	{{{FVIRTKEY|FCONTROL,				VK_SPACE},{FVIRTKEY|FSHIFT,	VK_SPACE}}},	//ID_AVS_STARTCOMPLETE,
	{{{FVIRTKEY|FCONTROL,				VK_Y},{0, 0}}},							//ID_EDIT_REDO,
	{{{FVIRTKEY|FCONTROL,				VK_Z},{0, 0}}},							//ID_EDIT_UNDO};
	{{{FVIRTKEY|FCONTROL,				VK_0},{0, 0}}},							//ID_AVS_SCRIPT_NONE,
	{{{FVIRTKEY|FCONTROL,				VK_1},{0, 0}}},							//ID_AVS_SCRIPT_AVS
	{{{0,								0},{0, 0}}},							//ID_FILE_REVERT
	{{{FVIRTKEY|FCONTROL,				VK_Q},{0, 0}}},							//ID_FILE_EXIT
	{{{0,								0},{0, 0}}},							//ID_AVS_EDIT_LINE
	{{{0,								0},{0, 0}}},							//ID_HELP_TOFF
	{{{0,								0},{0, 0}}},							//ID_HELP_AVISYNTH
	{{{FVIRTKEY|FCONTROL,				VK_F},{0, 0}}},							//ID_EDIT_FIND
	{{{FVIRTKEY,						VK_F3},{0, 0}}},						//ID_EDIT_FINDNEXT
	{{{FVIRTKEY|FCONTROL,				VK_2},{0, 0}}},							//ID_AVS_SCRIPT_DECOMB
	{{{FVIRTKEY|FCONTROL|FSHIFT,		VK_O},{0, 0}}},							//ID_INSERT_FILENAME
	{{{FVIRTKEY|FCONTROL|FSHIFT,		VK_C},{0, 0}}}							//ID_INSERT_CROP
	,{{{FVIRTKEY|FSHIFT,					VK_F3},{0, 0}}},						//ID_EDIT_FINDPREV
}};

ACCELKEYTABLE_AVS g_accelAVS = g_accelAVSDefault;
ACCEL g_accelAVSAccelerator[VDM_ACCEL_AVS_COUNT*2];
char g_accelAVSDescription[VDM_ACCEL_AVS_COUNT][255];

// Capture

WORD g_accelCaptureCommand[VDM_ACCEL_CAPTURE_COUNT] = {
	ID_FILE_SETCAPTUREFILE,
	ID_FILE_SETSTRIPINGSYSTEM,
	ID_FILE_ALLOCATEDISKSPACE,
	ID_FILE_DECREMENT,
	ID_FILE_INCREMENT,
	ID_FILE_EXITCAPTUREMODE,
	ID_AUDIO_COMPRESSION,
	ID_AUDIO_VOLUMEMETER,
	ID_VIDEO_OVERLAY,
	ID_VIDEO_PREVIEW,
	ID_VIDEO_PREVIEWHISTOGRAM,
	ID_VIDEO_FORMAT,
	ID_VIDEO_SOURCE,
	ID_VIDEO_DISPLAY,
	ID_VIDEO_COMPRESSION_INTERNAL,
	ID_VIDEO_COMPRESSION_AVICAP,
	ID_VIDEO_CLIPPING,
	ID_VIDEO_CLIPPING_SET,
	ID_VIDEO_SWAPFIELDS,
	ID_VIDEO_NOISEREDUCTION,
	ID_VIDEO_NOISEREDUCTION_THRESHOLD,
	ID_VIDEO_VRNONE,
	ID_VIDEO_VR2LINEAR,
	ID_VIDEO_VR2CUBIC,
	ID_VIDEO_SQUISH_RANGE,
	ID_VIDEO_CUSTOMFORMAT,
	ID_VIDEO_FILTERS,
	ID_VIDEO_ENABLEFILTERING,
	ID_VIDEO_STRETCH,
	ID_VIDEO_HISTOGRAM,
	ID_VIDEO_BT8X8TWEAKER,
	ID_CAPTURE_CAPTUREVIDEOINTERNAL,
	ID_CAPTURE_CAPTUREVIDEO,
	ID_CAPTURE_TEST,
	ID_CAPTURE_SETTINGS,
	ID_CAPTURE_PREFERENCES,
	ID_CAPTURE_STOPCONDITIONS,
	ID_CAPTURE_TIMING,
	ID_CAPTURE_DISKIO,
	ID_CAPTURE_SPILLSYSTEM,
	ID_CAPTURE_ENABLELOGGING,
	ID_CAPTURE_DISPLAYCAPTURELOG,
	ID_CAPTURE_HIDEONCAPTURE,
	ID_CAPTURE_DISPLAYLARGETIMER,
	ID_CAPTURE_INFOPANEL,
	ID_CAPTURE_ENABLESPILL,
	ID_CAPTURE_STARTONLEFT,
	ID_CAPTURE_AUTOINCREMENT,
	ID_CAPTURE_HWACCEL_NONE,
	ID_CAPTURE_HWACCEL_TOP,
	ID_CAPTURE_HWACCEL_BOTTOM,
	ID_CAPTURE_HWACCEL_BOTH,
	ID_HELP_CONTENTS
};

const ACCELKEYTABLE_CAPTURE g_accelCaptureDefault = {{
	{{{FVIRTKEY,						VK_F2},{0, 0}}},							//ID_FILE_SETCAPTUREFILE,
	{{{0,								0},{0, 0}}},								//ID_FILE_SETSTRIPINGSYSTEM,
	{{{0,								0},{0, 0}}},								//ID_FILE_ALLOCATEDISKSPACE,
	{{{0,								'['},{0, 0}}},								//ID_FILE_DECREMENT,
	{{{0,								']'},{0, 0}}},								//ID_FILE_INCREMENT,
	{{{0,								0},{0, 0}}},								//ID_FILE_EXITCAPTUREMODE,
	{{{FVIRTKEY,						VK_A},{0, 0}}},								//ID_AUDIO_COMPRESSION,
	{{{FVIRTKEY,						VK_V},{0, 0}}},								//ID_AUDIO_VOLUMEMETER,
	{{{FVIRTKEY,						VK_O},{0, 0}}},								//ID_VIDEO_OVERLAY,
	{{{FVIRTKEY,						VK_P},{0, 0}}},								//ID_VIDEO_PREVIEW,
	{{{FVIRTKEY|FSHIFT,					VK_P},{0, 0}}},								//ID_VIDEO_PREVIEWHISTOGRAM,
	{{{FVIRTKEY,						VK_F},{0, 0}}},								//ID_VIDEO_FORMAT,
	{{{FVIRTKEY,						VK_S},{0, 0}}},								//ID_VIDEO_SOURCE,
	{{{FVIRTKEY,						VK_D},{0, 0}}},								//ID_VIDEO_DISPLAY,
	{{{FVIRTKEY|FSHIFT,					VK_C},{0, 0}}},								//ID_VIDEO_COMPRESSION_INTERNAL,
	{{{FVIRTKEY,						VK_C},{0, 0}}},								//ID_VIDEO_COMPRESSION_AVICAP,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_CLIPPING,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_CLIPPING_SET,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SWAPFIELDS,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_NOISEREDUCTION,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_NOISEREDUCTION_THRESHOLD,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_VRNONE,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_VR2LINEAR,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_VR2CUBIC,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SQUISH_RANGE,
	{{{FVIRTKEY|FSHIFT,					VK_F},{0, 0}}},								//ID_VIDEO_CUSTOMFORMAT,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_FILTERS,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_ENABLEFILTERING,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_STRETCH,
	{{{FVIRTKEY,						VK_H},{0, 0}}},								//ID_VIDEO_HISTOGRAM,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_BT8X8TWEAKER,
	{{{FVIRTKEY,						VK_F6},{0, 0}}},							//ID_CAPTURE_CAPTUREVIDEOINTERNAL,
	{{{FVIRTKEY,						VK_F5},{0, 0}}},							//ID_CAPTURE_CAPTUREVIDEO,
	{{{FVIRTKEY,						VK_F7},{0, 0}}},							//ID_CAPTURE_TEST,
	{{{FVIRTKEY,						VK_F9},{0, 0}}},							//ID_CAPTURE_SETTINGS,
	{{{FVIRTKEY,						VK_F10},{0, 0}}},							//ID_CAPTURE_PREFERENCES,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_STOPCONDITIONS,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_TIMING,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_DISKIO,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_SPILLSYSTEM,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_ENABLELOGGING,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_DISPLAYCAPTURELOG,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_HIDEONCAPTURE,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_DISPLAYLARGETIMER,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_INFOPANEL,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_ENABLESPILL,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_STARTONLEFT,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_AUTOINCREMENT,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_HWACCEL_NONE,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_HWACCEL_TOP,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_HWACCEL_BOTTOM,
	{{{0,								0},{0, 0}}},								//ID_CAPTURE_HWACCEL_BOTH,
	{{{FVIRTKEY,						VK_F1},{0, 0}}}								//ID_HELP_CONTENTS
}};

ACCELKEYTABLE_CAPTURE g_accelCapture = g_accelCaptureDefault;
ACCEL g_accelCaptureAccelerator[VDM_ACCEL_CAPTURE_COUNT*2];
char g_accelCaptureDescription[VDM_ACCEL_CAPTURE_COUNT][255];

// Main

// Replacing stuff by deprecated till things are fixed.

WORD g_accelMainCommand[VDM_ACCEL_MAIN_COUNT] = {
	ID_FILE_OPENAVI,
	ID_COMMAND_DEPRECATED, //ID_FILE_OPENAVS,
	ID_FILE_APPENDSEGMENT,
	ID_FILE_PREVIEWAVI,
	ID_COMMAND_DEPRECATED,//ID_FILE_SAVEOGM,
	ID_FILE_SAVEAS,//was ID_FILE_SAVEAVI
	ID_COMMAND_DEPRECATED,//ID_FILE_SAVECOMPATIBLEAVI,
	ID_COMMAND_DEPRECATED,//ID_FILE_SAVESEGMENTEDAVI,
	ID_FILE_CLOSEAVI,
	ID_FILE_AVIINFO,
	ID_FILE_SAVESTRIPEDAVI,
	ID_FILE_SAVESTRIPEMASTER,
	ID_FILE_SAVEIMAGESEQ,
	ID_COMMAND_DEPRECATED,//ID_FILE_SAVEWAV,
	ID_COMMAND_DEPRECATED,//ID_FILE_DEMUXAUDIO,
	ID_FILE_LOADCONFIGURATION,
	ID_FILE_SAVECONFIGURATION,
	ID_FILE_STARTSERVER,
	ID_FILE_CAPTUREAVI,
	ID_FILE_RUNSCRIPT,
	ID_FILE_JOBCONTROL,
	ID_FILE_QUIT,
	ID_EDIT_REFRESH,
	ID_VIDEO_SEEK_START,
	ID_VIDEO_SEEK_END,
	ID_VIDEO_SEEK_PREV,
	ID_VIDEO_SEEK_NEXT,
	ID_VIDEO_SEEK_KEYPREV,
	ID_VIDEO_SEEK_KEYNEXT,
	ID_VIDEO_SEEK_PREVONESEC,
	ID_VIDEO_SEEK_NEXTONESEC,
	ID_VIDEO_SEEK_PREVDROP,
	ID_VIDEO_SEEK_NEXTDROP,
	ID_EDIT_PREVRANGE,
	ID_EDIT_NEXTRANGE,
	ID_VIDEO_SEEK_SELSTART,
	ID_VIDEO_SEEK_SELEND,
	ID_EDIT_JUMPTO,
	ID_EDIT_JUMP_LAST_KEY,
	ID_EDIT_DELETE,
	ID_EDIT_SETSELSTART,
	ID_EDIT_SETSELEND,
	ID_EDIT_MASK,
	ID_EDIT_UNMASK,
	ID_EDIT_RESET,
	ID_VIDEO_FILTERS,
	ID_VIDEO_FRAMERATE,
	ID_VIDEO_COLORDEPTH,
	ID_VIDEO_COMPRESSION,
	ID_VIDEO_CLIPPING,
	ID_VIDEO_MODE_DIRECT,
	ID_VIDEO_MODE_FASTRECOMPRESS,
	ID_VIDEO_MODE_NORMALRECOMPRESS,
	ID_VIDEO_MODE_FULL,
	ID_VIDEO_COPYSOURCEFRAME,
	ID_VIDEO_COPYOUTPUTFRAME,
	ID_VIDEO_SCANFORERRORS,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_CONVERSION,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_INTERLEAVE,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_COMPRESSION,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_VOLUME,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_NONE,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_AVI,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_WAV,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_MP3,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_AC3,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_OGG,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_MODE_DIRECT,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_MODE_FULL,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_CONVERSION,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_INTERLEAVE,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_COMPRESSION,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_VOLUME,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_NONE,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_AVI,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_WAV,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_MP3,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_AC3,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_OGG,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_MODE_DIRECT,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_MODE_FULL,
	ID_STREAMS_REPORT,//was ID_OGM_OGMINFO,
	ID_VIDEO_COMMENTS,//was ID_OGM_VIDEOCOMMENTS,
	ID_COMMAND_DEPRECATED,//ID_OGM_AUDIOCOMMENTS,
	ID_COMMAND_DEPRECATED,//ID_OGM_AUDIO2COMMENTS,
	ID_STREAM_LIST,//was ID_OGM_SHOWINPUTS,
	ID_OPTIONS_PERFORMANCE,
	ID_OPTIONS_DYNAMICCOMPILATION,
	ID_OPTIONS_PREFERENCES,
	ID_OPTIONS_DISPLAYINPUTVIDEO,
	ID_OPTIONS_DISPLAYOUTPUTVIDEO,
	ID_OPTIONS_DISPLAYDECOMPRESSEDOUTPUT,
	ID_OPTIONS_SHOWSTATUSWINDOW,
	ID_OPTIONS_SWAPPANES,
	ID_OPTIONS_SYNCHRONOUSBLIT,
	ID_OPTIONS_VERTICALDISPLAY,
	ID_OPTIONS_DRAWHISTOGRAMS,
	ID_OPTIONS_SYNCTOAUDIO,
	ID_OPTIONS_DROPFRAMES,
	ID_OPTIONS_ENABLEDIRECTDRAW,
	ID_OPTIONS_PREVIEWPROGRESSIVE,
	ID_OPTIONS_PREVIEWFIELDA,
	ID_OPTIONS_PREVIEWFIELDB,
	ID_TOOLS_AVS,
	ID_TOOLS_HEXVIEWER,
	ID_TOOLS_CREATESPARSEAVI,
	ID_TOOLS_EXPANDSPARSEAVI,
	ID_HELP_CONTENTS,
	ID_HELP_LICENSE,
	ID_HELP_CHANGELOG,
	ID_HELP_RELEASENOTES,
	ID_HELP_CHANGELOG_MOD,
	ID_HELP_ONLINE_HOME,
	ID_HELP_ONLINE_MOD,
	ID_HELP_ONLINE_FAQ,
	ID_HELP_ONLINE_NEWS,
	ID_HELP_ONLINE_KB,
	ID_HELP_ABOUT,
	ID_HELP_ABOUT_MOD,
	ID_HELP_ABOUT_SUPPORTS,//was ID_HELP_PNGSUPPORT,
	ID_COMMAND_DEPRECATED,//ID_AUDIO_SOURCE_CBRMP3,
	ID_COMMAND_DEPRECATED,//ID_AUDIO2_SOURCE_CBRMP3,
	ID_HELP_VDUBMODHELP,
	ID_OPTIONS_SAVE_PERFORMANCE,
	ID_OPTIONS_TICKS_NONE,
	ID_OPTIONS_TICKS_FRAMES,
	ID_OPTIONS_TICKS_KEYSOURCE,
	ID_OPTIONS_TICKS_KEYSTATS,
	ID_TOOLS_OPEN_STATS,
	ID_TOOLS_SAVE_STATS,
	ID_TOOLS_TOGGLE_FRAMETYPE,
	ID_COMMAND_DEPRECATED,
	ID_VIDEO_SCANFORERRORS_SHOW,
	ID_VIDEO_SCANFORERRORS_MASK,
	ID_VIDEO_SCANFORERRORS_DEL,
	ID_AUDIO_ADVANCEDFILTERING,
	ID_AUDIO_FILTERS,
	ID_VIDEO_CHAPTERS,
	ID_TOOLS_KEYCHAPTERS,
	ID_FILE_PREVIEWVBR,
	ID_VIDEO_SNAPSHOTSOURCEFRAME,
	ID_VIDEO_SNAPSHOTOUTPUTFRAME,
	ID_FILE_PREVIEWINPUT,
	ID_FILE_PREVIEWOUTPUT,
	ID_VIDEO_ERRORMODE,
	ID_AUDIO_ERRORMODE,
	ID_OPTIONS_SHOWLOG,
	ID_OPTIONS_SHOWPROFILER,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_EDIT_CLEAR,
	ID_EDIT_SELECTALL
};

const ACCELKEYTABLE_MAIN g_accelMainDefault = {{
	{{{FVIRTKEY|FCONTROL,				VK_O},{0, 0}}},								//ID_FILE_OPENAVI,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_OPENAVS,
	{{{0,								0},{0, 0}}},								//ID_FILE_APPENDSEGMENT,
	{{{FVIRTKEY|FCONTROL,				VK_F5},{0, 0}}},							//ID_FILE_PREVIEWAVI,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_SAVEOGM,
	{{{FVIRTKEY,						VK_F7},{0, 0}}},							//ID_FILE_SAVEAS, was ID_FILE_SAVEAS
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_SAVECOMPATIBLEAVI,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_SAVESEGMENTEDAVI,
	{{{FVIRTKEY|FCONTROL,				VK_W},{0, 0}}},								//ID_FILE_CLOSEAVI,
	{{{0,								0},{0, 0}}},								//ID_FILE_AVIINFO,
	{{{0,								0},{0, 0}}},								//ID_FILE_SAVESTRIPEDAVI,
	{{{0,								0},{0, 0}}},								//ID_FILE_SAVESTRIPEMASTER,
	{{{0,								0},{0, 0}}},								//ID_FILE_SAVEIMAGESEQ,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_SAVEWAV,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_FILE_DEMUXAUDIO,
	{{{FVIRTKEY|FCONTROL,				VK_L},{0, 0}}},								//ID_FILE_LOADCONFIGURATION,
	{{{FVIRTKEY|FCONTROL,				VK_S},{0, 0}}},								//ID_FILE_SAVECONFIGURATION,
	{{{0,								0},{0, 0}}},								//ID_FILE_STARTSERVER,
	{{{0,								0},{0, 0}}},								//ID_FILE_CAPTUREAVI,
	{{{0,								0},{0, 0}}},								//ID_FILE_RUNSCRIPT,
	{{{FVIRTKEY,						VK_F4},{0, 0}}},							//ID_FILE_JOBCONTROL,
	{{{0,								0},{0, 0}}},								//ID_FILE_QUIT,
	{{{FVIRTKEY,						VK_F5},{0, 0}}},							//ID_REFRESH,
	{{{FVIRTKEY|FCONTROL,				VK_LEFT},{0, 0}}},							//ID_VIDEO_SEEK_START,
	{{{FVIRTKEY|FCONTROL,				VK_RIGHT},{0, 0}}},							//ID_VIDEO_SEEK_END,
	{{{FVIRTKEY,						VK_LEFT},{0, 0}}},							//ID_VIDEO_SEEK_PREV,
	{{{FVIRTKEY,						VK_RIGHT},{0, 0}}},							//ID_VIDEO_SEEK_NEXT,
	{{{FVIRTKEY|FSHIFT,					VK_LEFT},{0, 0}}},							//ID_VIDEO_SEEK_KEYPREV,
	{{{FVIRTKEY|FSHIFT,					VK_RIGHT},{0, 0}}},							//ID_VIDEO_SEEK_KEYNEXT,
	{{{FVIRTKEY|FALT,					VK_LEFT},{0, 0}}},							//ID_VIDEO_SEEK_PREVONESEC,
	{{{FVIRTKEY|FALT,					VK_RIGHT},{0, 0}}},							//ID_VIDEO_SEEK_NEXTONESEC,
	{{{0,								'{'},{0, 0}}},								//ID_VIDEO_SEEK_PREVDROP,
	{{{0,								'}'},{0, 0}}},								//ID_VIDEO_SEEK_NEXTDROP,
	{{{0,								'<'},{0, 0}}},								//ID_EDIT_PREVRANGE,
	{{{0,								'>'},{0, 0}}},								//ID_EDIT_NEXTRANGE,
	{{{0,								'['},{0, 0}}},								//ID_VIDEO_SEEK_SELSTART,
	{{{0,								']'},{0, 0}}},								//ID_VIDEO_SEEK_SELEND,
	{{{FVIRTKEY|FCONTROL,				VK_G},{0, 0}}},								//ID_EDIT_JUMPTO,
	{{{FVIRTKEY|FCONTROL|FSHIFT,		VK_J},{0, 0}}},								//ID_EDIT_JUMP_LAST_KEY,
	{{{FVIRTKEY,						VK_DELETE},{0, 0}}},						//ID_EDIT_DELETE,
	{{{FVIRTKEY,						VK_HOME},{0, 0}}},							//ID_EDIT_SETSELSTART,
	{{{FVIRTKEY,						VK_END},{0, 0}}},							//ID_EDIT_SETSELEND,
	{{{0,								0},{0, 0}}},								//ID_EDIT_MASK,
	{{{0,								0},{0, 0}}},								//ID_EDIT_UNMASK,
	{{{0,								0},{0, 0}}},								//ID_EDIT_RESET,
	{{{FVIRTKEY|FCONTROL,				VK_F},{0, 0}}},								//ID_VIDEO_FILTERS,
	{{{FVIRTKEY|FCONTROL,				VK_R},{0, 0}}},								//ID_VIDEO_FRAMERATE,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_COLORDEPTH,
	{{{FVIRTKEY|FCONTROL,				VK_P},{0, 0}}},								//ID_VIDEO_COMPRESSION,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_CLIPPING,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_MODE_DIRECT,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_MODE_FASTRECOMPRESS,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_MODE_NORMALRECOMPRESS,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_MODE_FULL,
	{{{FVIRTKEY|FCONTROL,				VK_1},{0, 0}}},								//ID_VIDEO_COPYSOURCEFRAME,
	{{{FVIRTKEY|FCONTROL,				VK_2},{0, 0}}},								//ID_VIDEO_COPYOUTPUTFRAME,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SCANFORERRORS,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_CONVERSION,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_INTERLEAVE,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_COMPRESSION,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_VOLUME,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_NONE,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_AVI,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_WAV,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_MP3,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_AC3,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_SOURCE_OGG,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_MODE_DIRECT,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO_MODE_FULL,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_CONVERSION,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_INTERLEAVE,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_COMPRESSION,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_VOLUME,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_NONE,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_AVI,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_WAV,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_MP3,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_AC3,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_SOURCE_OGG,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_MODE_DIRECT,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_AUDIO2_MODE_FULL,
	{{{0,								0},{0, 0}}},								//ID_STREAMS_REPORT, was ID_OGM_OGMINFO,
	{{{0,								0},{0, 0}}},								//ID_VIDEO_COMMENTS, was ID_OGM_VIDEOCOMMENTS,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_OGM_AUDIOCOMMENTS,
	{{{0,								0},{0, 0}}},								//DEPRECATED ID_OGM_AUDIO2COMMENTS,
	{{{0,								0},{0, 0}}},								//ID_STREAM_LIST, was ID_OGM_SHOWINPUTS,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_PERFORMANCE,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_DYNAMICCOMPILATION,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_PREFERENCES,
	{{{FVIRTKEY,						VK_F9},{0, 0}}},							//ID_OPTIONS_DISPLAYINPUTVIDEO,
	{{{FVIRTKEY,						VK_F10},{0, 0}}},							//ID_OPTIONS_DISPLAYOUTPUTVIDEO,
	{{{FVIRTKEY|FSHIFT,					VK_F10},{0, 0}}},							//ID_OPTIONS_DISPLAYDECOMPRESSEDOUTPUT,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_SHOWSTATUSWINDOW,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_SWAPPANES,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_SYNCHRONOUSBLIT,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_VERTICALDISPLAY,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_DRAWHISTOGRAMS,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_SYNCTOAUDIO,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_DROPFRAMES,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_ENABLEDIRECTDRAW,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_PREVIEWPROGRESSIVE,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_PREVIEWFIELDA,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_PREVIEWFIELDB,
	{{{FVIRTKEY|FCONTROL,				VK_E},{0, 0}}},								//ID_TOOLS_AVS,
	{{{0,								0},{0, 0}}},								//ID_TOOLS_HEXVIEWER,
	{{{0,								0},{0, 0}}},								//ID_TOOLS_CREATESPARSEAVI,
	{{{0,								0},{0, 0}}},								//ID_TOOLS_EXPANDSPARSEAVI,
	{{{FVIRTKEY,						VK_F1},{0, 0}}},							//ID_HELP_CONTENTS,
	{{{0,								0},{0, 0}}},								//ID_HELP_LICENSE,
	{{{0,								0},{0, 0}}},								//ID_HELP_CHANGELOG,
	{{{0,								0},{0, 0}}},								//ID_HELP_RELEASENOTES,
	{{{0,								0},{0, 0}}},								//ID_HELP_CHANGELOG_MOD,
	{{{0,								0},{0, 0}}},								//ID_HELP_ONLINE_HOME,
	{{{0,								0},{0, 0}}},								//ID_HELP_ONLINE_MOD,
	{{{0,								0},{0, 0}}},								//ID_HELP_ONLINE_FAQ,
	{{{0,								0},{0, 0}}},								//ID_HELP_ONLINE_NEWS,
	{{{0,								0},{0, 0}}},								//ID_HELP_ONLINE_KB,
	{{{0,								0},{0, 0}}},								//ID_HELP_ABOUT,
	{{{0,								0},{0, 0}}},								//ID_HELP_ABOUT_MOD,
	{{{0,								0},{0, 0}}},								//ID_HELP_PNGSUPPORT
	{{{0,								0},{0, 0}}},								//ID_AUDIO_SOURCE_CBRMP3,
	{{{0,								0},{0, 0}}},								//ID_AUDIO2_SOURCE_CBRMP3,
	{{{0,								0},{0, 0}}},								//ID_HELP_VDUBMODHELP
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_SAVE_PERFORMANCE
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_TICKS_NONE,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_TICKS_FRAMES,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_TICKS_KEYSOURCE,
	{{{0,								0},{0, 0}}},								//ID_OPTIONS_TICKS_KEYSTATS,
	{{{0,								0},{0, 0}}},								//ID_TOOLS_OPEN_STATS
	{{{0,								0},{0, 0}}},								//ID_TOOLS_SAVE_STATS
	{{{FVIRTKEY|FSHIFT,					VK_SPACE},{0, 0}}},							//ID_TOOLS_TOGGLE_FRAMETYPE
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SCANFORERRORS2
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SCANFORERRORS_SHOW
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SCANFORERRORS_MASK
	{{{0,								0},{0, 0}}},								//ID_VIDEO_SCANFORERRORS_DEL
	{{{0,								0},{0, 0}}},								//ID_AUDIO_ADVANCEDFILTERING
	{{{0,								0},{0, 0}}},								//ID_AUDIO_FILTERS
	{{{0,								0},{0, 0}}},								//ID_VIDEO_CHAPTERS
	{{{0,								0},{0, 0}}},								//ID_TOOLS_KEYCHAPTERS
	{{{FVIRTKEY|FSHIFT,					VK_F5},{0, 0}}},							//ID_FILE_PREVIEWVBR
	{{{FVIRTKEY|FSHIFT,					VK_1},{0, 0}}},								//ID_VIDEO_SNAPSHOTSOURCEFRAME
	{{{FVIRTKEY|FSHIFT,					VK_2},{0, 0}}},								//ID_VIDEO_SNAPSHOTOUTPUTFRAME
	{{{FVIRTKEY,						VK_SPACE},{0, 0}}},							//ID_FILE_PREVIEWINPUT
	{{{FVIRTKEY,						VK_RETURN},{0, 0}}},						//ID_FILE_PREVIEWOUTPUT
	{{{0,								0},{0, 0}}},								//ID_VIDEO_ERRORMODE
	{{{0,								0},{0, 0}}},								//ID_AUDIO_ERRORMODE
	{{{FVIRTKEY,						VK_F8},{0, 0}}},							//ID_OPTIONS_SHOWLOG
	{{{FVIRTKEY|FSHIFT,					VK_F8},{0, 0}}},							//ID_OPTIONS_SHOWPROFILER
	{{{FVIRTKEY|FCONTROL,				VK_X},{0, 0}}},								//ID_EDIT_CUT
	{{{FVIRTKEY|FCONTROL,				VK_C},{0, 0}}},								//ID_EDIT_COPY
	{{{FVIRTKEY|FCONTROL,				VK_V},{0, 0}}},								//ID_EDIT_PASTE
	{{{FVIRTKEY|FCONTROL,				VK_D},{0, 0}}},								//ID_EDIT_CLEAR
	{{{FVIRTKEY|FCONTROL,				VK_A},{0, 0}}}								//ID_EDIT_SELECTALL
}};

ACCELKEYTABLE_MAIN g_accelMain = g_accelMainDefault;
ACCEL g_accelMainAccelerator[VDM_ACCEL_MAIN_COUNT*2];
char g_accelMainDescription[VDM_ACCEL_MAIN_COUNT][255];


// Popup audio menu

WORD g_accelAudioCommand[VDM_ACCEL_AUDIO_COUNT] = {
	ID_AUDIO_INTERLEAVE,
	ID_AUDIO_COMPRESSION,
	ID_AUDIO_ADVANCEDFILTERING,
	ID_AUDIO_FILTERS,
	ID_AUDIO_CONVERSION,
	ID_AUDIO_VOLUME,
	ID_AUDIO_MODE_DIRECT,
	ID_AUDIO_MODE_FULL
};

const ACCELKEYTABLE_AUDIO g_accelAudioDefault = {{
	{{{FVIRTKEY|FCONTROL,				VK_I},{0, 0}}},								//ID_AUDIO_INTERLEAVE,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_COMPRESSION,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_ADVANCEDFILTERING,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_FILTERS,
	{{{FVIRTKEY|FCONTROL,				VK_O},{0, 0}}},								//ID_AUDIO_CONVERSION,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_VOLUME,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_MODE_DIRECT,
	{{{0,								0},{0, 0}}},								//ID_AUDIO_MODE_FULL,
}};

ACCELKEYTABLE_AUDIO g_accelAudio = g_accelAudioDefault;
ACCEL g_accelAudioAccelerator[VDM_ACCEL_AUDIO_COUNT*2];
char g_accelAudioDescription[VDM_ACCEL_AUDIO_COUNT][255];


// General

char g_accelKEYDescription[256][255];
WORD g_accelKEYKeycode[256];

int GetOffsetFromIDAVS(WORD id) {
	for(int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
		if (id==g_accelAVSCommand[i]) return i;
	}
	return -1;
}

void HandleMenuAVS(std::string str, HMENU mnu) {
	std::string locstr = str;
	char buf[255];
	int c = GetMenuItemCount(mnu);
	for(int i=0; i<c; i++) {
		WORD id = GetMenuItemID(mnu, i);
		GetMenuString(mnu, i, (char *) &buf, 255, MF_BYPOSITION);
		locstr += "\\";
		locstr += buf;
		int pos = GetOffsetFromIDAVS(id);
		if (pos>=0) {
			strcpy((char *) &g_accelAVSDescription[pos][0], locstr.c_str());
		}
		if (HMENU submnu = GetSubMenu(mnu, i)) HandleMenuAVS(locstr, submnu);
		locstr = str;
	}
}

int GetOffsetFromIDCapture(WORD id) {
	for(int i=0; i<VDM_ACCEL_CAPTURE_COUNT; i++) {
		if (id==g_accelCaptureCommand[i]) return i;
	}
	return -1;
}

void HandleMenuCapture(std::string str, HMENU mnu) {
	std::string locstr = str;
	char buf[255];
	int c = GetMenuItemCount(mnu);
	for(int i=0; i<c; i++) {
		WORD id = GetMenuItemID(mnu, i);
		GetMenuString(mnu, i, (char *) &buf, 255, MF_BYPOSITION);
		locstr += "\\";
		locstr += buf;
		int pos = GetOffsetFromIDCapture(id);
		if (pos>=0) {
			strcpy((char *) &g_accelCaptureDescription[pos][0], locstr.c_str());
		}
		if (HMENU submnu = GetSubMenu(mnu, i)) HandleMenuCapture(locstr, submnu);
		locstr = str;
	}
}

int GetOffsetFromIDMain(WORD id) {
	for(int i=0; i<VDM_ACCEL_MAIN_COUNT; i++) {
		if (id==g_accelMainCommand[i]) return i;
	}
	return -1;
}

void HandleMenuMain(std::string str, HMENU mnu) {
	std::string locstr = str;
	char buf[255];
	int c = GetMenuItemCount(mnu);
	for(int i=0; i<c; i++) {
		WORD id = GetMenuItemID(mnu, i);
		GetMenuString(mnu, i, (char *) &buf, 255, MF_BYPOSITION);
		locstr += "\\";
		locstr += buf;
		int pos = GetOffsetFromIDMain(id);
		if (pos>=0) {
			strcpy((char *) &g_accelMainDescription[pos][0], locstr.c_str());
		}
		if (HMENU submnu = GetSubMenu(mnu, i)) HandleMenuMain(locstr, submnu);
		locstr = str;
	}
}

int GetOffsetFromIDAudio(WORD id) {
	for(int i=0; i<VDM_ACCEL_AUDIO_COUNT; i++) {
		if (id==g_accelAudioCommand[i]) return i;
	}
	return -1;
}

void HandleAudioMain(std::string str, HMENU mnu) {
	std::string locstr = str;
	char buf[255];
	int c = GetMenuItemCount(mnu);
	for(int i=0; i<c; i++) {
		WORD id = GetMenuItemID(mnu, i);
		GetMenuString(mnu, i, (char *) &buf, 255, MF_BYPOSITION);
		locstr += "\\";
		locstr += buf;
		int pos = GetOffsetFromIDAudio(id);
		if (pos>=0) {
			strcpy((char *) &g_accelAudioDescription[pos][0], locstr.c_str());
		}
		if (HMENU submnu = GetSubMenu(mnu, i)) HandleAudioMain(locstr, submnu);
		locstr = str;
	}
}

void InitDescriptions() {
	char buffer[255];
	int i;
	int c = 0;
	FillMemory(&g_accelKEYKeycode, sizeof(g_accelKEYKeycode), 0);
	g_accelKEYKeycode[c] = -1;
	strcpy(g_accelKEYDescription[c], "NONE");
	for(i=0; i<=255; i++) {
		if (LoadString(g_hInst, 5000+i, (char *) &buffer, 255)>0) {
			c++;
			g_accelKEYKeycode[c] = i;
			strcpy(g_accelKEYDescription[c], buffer);
		}
	}

	FillMemory(&g_accelAVSDescription, sizeof(g_accelAVSDescription), 0);
	HMENU menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_AVSVIEWER_MENU));
	std::string str = "";
	HandleMenuAVS(str, menu);
	DestroyMenu(menu);
	for(i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
		if (g_accelAVSDescription[i][0]==0) {
	 		LoadString(g_hInst, IDS_SHORTCUT_NOMENU, (char *) &buffer, 255);
			strcpy(g_accelAVSDescription[i], "\\");
			strcat(g_accelAVSDescription[i], buffer);
			strcat(g_accelAVSDescription[i], "\\");
	 		LoadString(g_hInst, g_accelAVSCommand[i], (char *) &buffer, 255);
			strcat(g_accelAVSDescription[i], buffer);
		} else {
			str = g_accelAVSDescription[i];
			for(c=0; c<str.length(); c++) {
				if (str[c]=='&') str.erase(c, 1);
			}
			strcpy(g_accelAVSDescription[i], str.c_str());
		}
	}

	FillMemory(&g_accelCaptureDescription, sizeof(g_accelCaptureDescription), 0);
	menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_CAPTURE_MENU));
	str = "";
	HandleMenuCapture(str, menu);
	DestroyMenu(menu);
	for(i=0; i<VDM_ACCEL_CAPTURE_COUNT; i++) {
		if (g_accelCaptureDescription[i][0]==0) {
	 		LoadString(g_hInst, IDS_SHORTCUT_NOMENU, (char *) &buffer, 255);
			strcpy(g_accelCaptureDescription[i], "\\");
			strcat(g_accelCaptureDescription[i], buffer);
			strcat(g_accelCaptureDescription[i], "\\");
	 		LoadString(g_hInst, g_accelCaptureCommand[i], (char *) &buffer, 255);
			strcat(g_accelCaptureDescription[i], buffer);
		} else {
			str = g_accelCaptureDescription[i];
			for(c=0; c<str.length(); c++) {
				if (str[c]=='&') str.erase(c, 1);
			}
			strcpy(g_accelCaptureDescription[i], str.c_str());
		}
	}

	FillMemory(&g_accelMainDescription, sizeof(g_accelMainDescription), 0);
	menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MAIN_MENU));
	str = "";
	HandleMenuMain(str, menu);
	DestroyMenu(menu);
	for(i=0; i<VDM_ACCEL_MAIN_COUNT; i++) {
		if (g_accelMainDescription[i][0]==0) {
	 		LoadString(g_hInst, IDS_SHORTCUT_NOMENU, (char *) &buffer, 255);
			strcpy(g_accelMainDescription[i], "\\");
			strcat(g_accelMainDescription[i], buffer);
			strcat(g_accelMainDescription[i], "\\");
	 		LoadString(g_hInst, g_accelMainCommand[i], (char *) &buffer, 255);
			strcat(g_accelMainDescription[i], buffer);
		} else {
			str = g_accelMainDescription[i];
			for(c=0; c<str.length(); c++) {
				if (str[c]=='&') str.erase(c, 1);
			}
			strcpy(g_accelMainDescription[i], str.c_str());
		}
	}

	FillMemory(&g_accelAudioDescription, sizeof(g_accelAudioDescription), 0);
	menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_AUDIO_MENU));
	str = "";
	HandleAudioMain(str, menu);
	DestroyMenu(menu);
	for(i=0; i<VDM_ACCEL_AUDIO_COUNT; i++) {
		if (g_accelAudioDescription[i][0]==0) {
	 		LoadString(g_hInst, IDS_SHORTCUT_NOMENU, (char *) &buffer, 255);
			strcpy(g_accelAudioDescription[i], "\\");
			strcat(g_accelAudioDescription[i], buffer);
			strcat(g_accelAudioDescription[i], "\\");
	 		LoadString(g_hInst, g_accelAudioCommand[i], (char *) &buffer, 255);
			strcat(g_accelAudioDescription[i], buffer);
		} else {
			str = g_accelAudioDescription[i];
			for(c=0; c<str.length(); c++) {
				if (str[c]=='&') str.erase(c, 1);
			}
			strcpy(g_accelAudioDescription[i], str.c_str());
		}
	}
}

HACCEL CreateAVSAccelerators() {
	ACCEL *avsAccel = (LPACCEL) &g_accelAVSAccelerator;
	int c = -1;
	ACCELKEY *avsKey;
	for(int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
		avsKey = &g_accelAVS.command[i].altkey[0];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelAVSCommand[i];
		}
		avsKey = &g_accelAVS.command[i].altkey[1];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelAVSCommand[i];
		}
	}
	return CreateAcceleratorTable(avsAccel, c+1);
}

HACCEL CreateCaptureAccelerators() {
	ACCEL *avsAccel = (LPACCEL) &g_accelCaptureAccelerator;
	int c = -1;
	ACCELKEY *avsKey;
	for(int i=0; i<VDM_ACCEL_CAPTURE_COUNT; i++) {
		avsKey = &g_accelCapture.command[i].altkey[0];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelCaptureCommand[i];
		}
		avsKey = &g_accelCapture.command[i].altkey[1];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelCaptureCommand[i];
		}
	}
	return CreateAcceleratorTable(avsAccel, c+1);
}

HACCEL CreateMainAccelerators() {
	ACCEL *avsAccel = (LPACCEL) &g_accelMainAccelerator;
	int c = -1;
	ACCELKEY *avsKey;
	for(int i=0; i<VDM_ACCEL_MAIN_COUNT; i++) {
		avsKey = &g_accelMain.command[i].altkey[0];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelMainCommand[i];
		}
		avsKey = &g_accelMain.command[i].altkey[1];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelMainCommand[i];
		}
	}
	return CreateAcceleratorTable(avsAccel, c+1);
}

HACCEL CreateAudioAccelerators() {
	ACCEL *avsAccel = (LPACCEL) &g_accelAudioAccelerator;
	int c = -1;
	ACCELKEY *avsKey;
	for(int i=0; i<VDM_ACCEL_AUDIO_COUNT; i++) {
		avsKey = &g_accelAudio.command[i].altkey[0];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelAudioCommand[i];
		}
		avsKey = &g_accelAudio.command[i].altkey[1];
		if (avsKey->key>0) {
			c++;
			(avsAccel + c)->fVirt = avsKey->fVirt;
			(avsAccel + c)->key = avsKey->key;
			(avsAccel + c)->cmd = g_accelAudioCommand[i];
		}
	}
	return CreateAcceleratorTable(avsAccel, c+1);
}

void GetKeyString(BYTE fVirt, WORD key, char *buffer, int len) {
	bool bRem = false;
	*buffer = 0;
	char buf[255];
	char b[2] = "\0";
	if (fVirt & FALT) {
		if (LoadString(g_hInst, 5018, (char *) &buf, 255)>0) {
			strncat(buffer, buf, len);
			strncat(buffer, "+", len-strlen(buffer));
			bRem = true;
		}
	}
	if (fVirt & FCONTROL) {
		if (LoadString(g_hInst, 5017, (char *) &buf, 255)>0) {
			strncat(buffer, buf, len-strlen(buffer));
			strncat(buffer, "+", len-strlen(buffer));
			bRem = true;
		}
	}
	if (fVirt & FSHIFT) {
		if (LoadString(g_hInst, 5016, (char *) &buf, 255)>0) {
			strncat(buffer, buf, len-strlen(buffer));
			strncat(buffer, "+", len-strlen(buffer));
			bRem = true;
		}
	}
	if (bRem) *(buffer + (strlen(buffer)-1)) = '-';
	if (fVirt & FVIRTKEY) {
		if (LoadString(g_hInst, 5000+key, (char *) &buf, 255)>0)
			strncat(buffer, buf, len-strlen(buffer));
	} else {
		b[0]=key;
		if (key!=0) strcat(buffer, b);
	}
}


#define WIN95_MENUITEMINFO_SIZE (offsetof(MENUITEMINFO, cch) + sizeof(UINT))


HMENU CreateAVSMenu() {
	HMENU menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_AVSVIEWER_MENU));
	char buffer[255];
	char buf[255];
	MENUITEMINFO mi;
	mi.cbSize = WIN95_MENUITEMINFO_SIZE;
	mi.fMask = MIIM_TYPE;
	mi.dwTypeData = (char *) &buffer;
	buffer[0] = 0;

	for(int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
		if (g_accelAVS.command[i].altkey[0].key>0) {
			mi.cch = 255;
			GetMenuItemInfo(menu, g_accelAVSCommand[i], MF_BYCOMMAND, &mi);
			strcat(buffer, "\t");
			GetKeyString(g_accelAVS.command[i].altkey[0].fVirt, g_accelAVS.command[i].altkey[0].key, (char *) &buf, 255);
			strcat(buffer, buf);
			if (g_accelAVS.command[i].altkey[1].key>0) {
				strcat(buffer, ", ");
				GetKeyString(g_accelAVS.command[i].altkey[1].fVirt, g_accelAVS.command[i].altkey[1].key, (char *) &buf, 255);
				strcat(buffer, buf);
			}
			SetMenuItemInfo(menu, g_accelAVSCommand[i], MF_BYCOMMAND, &mi);
		}
	}
	return menu;
}

HMENU CreateCaptureMenu() {
	HMENU menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_CAPTURE_MENU));
	char buffer[255];
	char buf[255];
	MENUITEMINFO mi;
	mi.cbSize = WIN95_MENUITEMINFO_SIZE;
	mi.fMask = MIIM_TYPE;
	mi.dwTypeData = (char *) &buffer;
	buffer[0] = 0;

	for(int i=0; i<VDM_ACCEL_CAPTURE_COUNT; i++) {
		if (g_accelCapture.command[i].altkey[0].key>0) {
			mi.cch = 255;
			GetMenuItemInfo(menu, g_accelCaptureCommand[i], MF_BYCOMMAND, &mi);
			strcat(buffer, "\t");
			GetKeyString(g_accelCapture.command[i].altkey[0].fVirt, g_accelCapture.command[i].altkey[0].key, (char *) &buf, 255);
			strcat(buffer, buf);
			if (g_accelCapture.command[i].altkey[1].key>0) {
				strcat(buffer, ", ");
				GetKeyString(g_accelCapture.command[i].altkey[1].fVirt, g_accelCapture.command[i].altkey[1].key, (char *) &buf, 255);
				strcat(buffer, buf);
			}
			SetMenuItemInfo(menu, g_accelCaptureCommand[i], MF_BYCOMMAND, &mi);
		}
	}
	return menu;
}

HMENU CreateMainMenu() {
	HMENU menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MAIN_MENU));
	char buffer[255];
	char buf[255];
	MENUITEMINFO mi;
	mi.cbSize = WIN95_MENUITEMINFO_SIZE;
	mi.fMask = MIIM_TYPE;
	mi.dwTypeData = (char *) &buffer;
	buffer[0] = 0;

	for(int i=0; i<VDM_ACCEL_MAIN_COUNT; i++) {
		if ((g_accelMain.command[i].altkey[0].key>0) && (g_accelMainCommand[i]!=ID_COMMAND_DEPRECATED)) {
			mi.cch = 255;
			GetMenuItemInfo(menu, g_accelMainCommand[i], MF_BYCOMMAND, &mi);
			strcat(buffer, "\t");
			GetKeyString(g_accelMain.command[i].altkey[0].fVirt, g_accelMain.command[i].altkey[0].key, (char *) &buf, 255);
			strcat(buffer, buf);
			if (g_accelMain.command[i].altkey[1].key>0) {
				strcat(buffer, ", ");
				GetKeyString(g_accelMain.command[i].altkey[1].fVirt, g_accelMain.command[i].altkey[1].key, (char *) &buf, 255);
				strcat(buffer, buf);
			}
			SetMenuItemInfo(menu, g_accelMainCommand[i], MF_BYCOMMAND, &mi);
			buffer[0] = 0;
		}
	}
	return menu;
}

HMENU CreateAudioMenu() {
	HMENU menu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_AUDIO_MENU));
	char buffer[255];
	char buf[255];
	MENUITEMINFO mi;
	mi.cbSize = WIN95_MENUITEMINFO_SIZE;
	mi.fMask = MIIM_TYPE;
	mi.dwTypeData = (char *) &buffer;
	buffer[0] = 0;

	for(int i=0; i<VDM_ACCEL_AUDIO_COUNT; i++) {
		if ((g_accelAudio.command[i].altkey[0].key>0) && (g_accelAudioCommand[i]!=ID_COMMAND_DEPRECATED)) {
			mi.cch = 255;
			GetMenuItemInfo(menu, g_accelAudioCommand[i], MF_BYCOMMAND, &mi);
			strcat(buffer, "\t");
			GetKeyString(g_accelAudio.command[i].altkey[0].fVirt, g_accelAudio.command[i].altkey[0].key, (char *) &buf, 255);
			strcat(buffer, buf);
			if (g_accelAudio.command[i].altkey[1].key>0) {
				strcat(buffer, ", ");
				GetKeyString(g_accelAudio.command[i].altkey[1].fVirt, g_accelAudio.command[i].altkey[1].key, (char *) &buf, 255);
				strcat(buffer, buf);
			}
			SetMenuItemInfo(menu, g_accelAudioCommand[i], MF_BYCOMMAND, &mi);
			buffer[0] = 0;
		}
	}
	return menu;
}
