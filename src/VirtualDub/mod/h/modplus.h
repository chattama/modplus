#ifndef f_AVSPREFS_H
#define f_AVSPREFS_H

#include <tchar.h>
#include <windows.h>

#include "Tdll.h"
#include "accel.h"
#include "virtualdubmod_messages.h"
#include "avs/CAviSynth.h"
#include "avs/cmdline.h"
#include "avs/genscript.h"
#include "avs/template.h"
#include "avs/AVSViewer.h"

#define REG_KEY_SCRIPTEDITOR "VirtualDubMod Script Editor"

// Init ***********************************************************************

bool InitApplicationMod(HINSTANCE hInstance);
void DeinitMod();

// uiframe ********************************************************************

bool TranslateAcceleratorAvs(HWND hwnd, MSG& msg);

// gui ************************************************************************

int guiMessageBox(HWND hwnd, UINT idText, UINT idCaption, UINT uType);

// projectui ******************************************************************

bool MenuHitMod(HWND hwnd, UINT id);

// prefs **********************************************************************

#pragma pack(push)
#pragma pack(1)
class VDubModPreferences {
public:
	char	m_bNandubCompatibility;
	char	m_bAudioFrameMode;
	uint8	m_GroupFrames;
	char	m_bKeepCorruptedData;
	uint32	m_DefaultPreload;
	char	m_bDisablePreload;
	char	m_bPrissRocks;
	char	m_DefaultVideoMode;
	char	m_bShowInputFrame;
	char	m_bShowOutputFrame;
	char	m_bAutoResize;	//Stone-D
	uint16	m_AVIRevListsLimit;
	char	m_AVIRecListsMode;
	char	m_bScriptEditorSingleInstance;
	char	m_bScriptEditorAutoPopup;
};
#pragma pack(pop)

typedef struct VDubModPreferences2 {
	VDubModPreferences	mModPrefs;
	ACCELKEYTABLE_AVS	mKeytableAVS;
	uint32				mAVSViewerFontSize;
	VDStringA			mAVSViewerFontFace;
};

typedef struct VDMKeys {
	int idx, row;
	VDStringA desc[3];
	ACCELKEYS command;
};

class VDDialogPrefsScriptEditor : public VDDialogBase {
public:
	VDubModPreferences2& mPrefs;
	VDDialogPrefsScriptEditor(VDubModPreferences2& p) : mPrefs(p) {}
	bool HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item);
private:
	enum {
		kIDC_SINGLEINSTANCE		= 100,
		kIDC_AUTOPOPUP			= 101,
		kIDC_FONT				= 102,
		kIDS_FONT_TEXT			= 103,
		kIDS_SINGLEINSTANCE_TIP	= 200,
		kIDS_AUTOPOPUP_TIP		= 201,
	};
	void SetFontLabel();
	void InitFont(HWND hwnd, LPLOGFONT lplf);
	void AVSViewerChooseFont(HWND hwnd);
};

class VDDialogPrefsScriptEditorKeys : public VDDialogBase {
public:
	VDubModPreferences2& mPrefs;
	VDMKeys mKeys[VDM_ACCEL_AVS_COUNT];
	VDDialogPrefsScriptEditorKeys(VDubModPreferences2& p) : mPrefs(p) {}
	bool HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item);
private:
	enum {
		kIDC_LISTVIEW	= 100,
		kIDC_DEFAULT	= 200,
		kIDD_SUBDIALOG	= 10003,
	};
};

class VDDialogScriptEditorKeysSub : public VDDialogBase {
public:
	VDDialogScriptEditorKeysSub(VDMKeys* keys) : mKeys(keys) {}
	bool HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item);
private:
	enum {
		kIDC_P_GROUP	= 1,
		kIDC_S_GROUP	= 2,
		kID_OK			= 10,
		kID_Cancel		= 11,
		kIDC_P_MODE		= 100,
		kIDC_P_ALT,
		kIDC_P_CONTROL,
		kIDC_P_SHIFT,
		kIDC_P_KEY		= 105,
		kIDC_P_ASCII,
		kIDC_S_MODE		= 200,
		kIDC_S_ALT,
		kIDC_S_CONTROL,
		kIDC_S_SHIFT,
		kIDC_S_KEY		= 205,
		kIDC_S_ASCII,
	};
	IVDUIWindow* mEditPKey;
	IVDUIWindow* mEditPAscii;
	IVDUIWindow* mEditSKey;
	IVDUIWindow* mEditSAscii;
	VDMKeys* mKeys;
	void EnableDisableKeyEntries();
};

void LoadPreferencesMod();
void VDMPreferencesUpdated();
void VDMSavePreferences(VDubModPreferences2& prefs);

// AVSViewer ******************************************************************

typedef struct FindTextOption {
	char szFindString[1024];
	bool bFindReverse;
	bool bWholeWord;
	bool bMatchCase;
	bool bRegexp;
	bool bWrap;
};

void AVSViewerOpen(HWND hwnd);
void AVSViewerLoadSettings(HWND hwnd);
void AVSViewerSaveSettings(HWND hwnd);
void AVSViewerChangePrefs(HWND parent);

// ****************************************************************************

extern HINSTANCE				g_hInst;
extern HWND						g_hWnd;
extern HACCEL					g_hAccelAVS;

extern CAviSynth				*g_dllAviSynth;
extern Tdll						*g_dllScintilla;
extern ATOM						g_classAVS;
extern ACCELKEYTABLE_AVS		g_accelAVS;

extern VDubModPreferences		g_VDMPrefs;
extern VDubModPreferences2		g_VDMPrefs2;

extern wchar_t g_szInputAVIFileLastWorking[];
extern wchar_t g_szInputAVIFile[MAX_PATH];
extern wchar_t g_szInputWAVFile[MAX_PATH];

#endif
