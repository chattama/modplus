
#include "stdafx.h"

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>

#include <vd2/Dita/basetypes.h>
#include <vd2/Dita/controls.h>
#include <vd2/Dita/w32peer.h>
#include <vd2/Dita/services.h>
#include <vd2/system/registry.h>
#include "job.h"
#include "projectui.h"
#include "prefs.h"

#include "modplus.h"

ATOM		g_classAVS;
Tdll		*g_dllScintilla;
Tdll		*g_dllAVSLexer;
Tdll		*g_dllRichedit;
CAviSynth	*g_dllAviSynth;

StringList	gFileList;

HACCEL		g_hAccelAVS;

bool g_bUseUnicodeWinAPI = (sint32(GetVersion()) >= 0);

wchar_t g_szInputAVIFileLastWorking[MAX_PATH] = L"";

static char g_szVDubModPrefs[] = "VDubMod prefs";
static char g_szKeytableAVS[]  = "Keytable AVS";

extern VDProject *g_project;
extern vdautoptr<VDProjectUI> g_projectui;

extern const wchar_t fileFilters0[];
extern const wchar_t fileFiltersAppend[];

extern const char g_szError[];
extern const char g_szWarning[];
extern const char g_szRegKeyPersistence[];

extern const ACCELKEYTABLE_AVS g_accelAVSDefault;
extern char g_accelKEYDescription[256][255];
extern char g_accelAVSDescription[VDM_ACCEL_AVS_COUNT][255];
extern WORD g_accelKEYKeycode[256];
extern WORD g_accelAVSCommand[VDM_ACCEL_AVS_COUNT];

// Init ***********************************************************************

bool InitApplicationMod(HINSTANCE hInstance) {

	InitDescriptions();
	g_classAVS = RegisterAVSEditor();
	if (!g_classAVS) return false;
	InitAVSEditor();
	g_dllScintilla = new Tdll("SciLexer.dll", NULL);
	g_dllAVSLexer = new Tdll("AviSynthLexer.lexer", NULL);
	g_dllRichedit = new Tdll("Riched20.dll", NULL);
	if (!g_dllRichedit->ok) return false;
	g_dllAviSynth = new CAviSynth("avisynth.dll");

	if (!(g_hAccelAVS = CreateAVSAccelerators())) return false;
}

void DeinitMod() {
	DeinitAVSEditor();
	delete g_dllAVSLexer;
	delete g_dllScintilla;
	delete g_dllAviSynth;
	delete g_dllRichedit;
	if (g_hAccelAVS) DestroyAcceleratorTable(g_hAccelAVS);
}

// Main ***********************************************************************

void OpenAVI(bool ext_opt) {
	bool fExtendedOpen = false;
	bool fAutoscan = false;

	IVDInputDriver *pDriver = 0;

	std::vector<int> xlat;
	tVDInputDrivers inputDrivers;

	VDGetInputDrivers(inputDrivers, IVDInputDriver::kF_Video);

	VDStringW fileFilters(VDMakeInputDriverFileFilter(inputDrivers, xlat));

	std::vector<void *> templates;
	templates.clear();

	templates.push_back(L"Use AviSynth template");
	templates.push_back(_T("[none]"));

	gTemplateList.clear();
	if (GetTemplateInfo()) {
		TemplateInfoList::iterator walkit;
		for(walkit = gTemplateList.begin(); walkit != gTemplateList.end(); walkit++) {
			templates.push_back((void *)walkit->sDescription_.data());
		}
	}
	templates.push_back(NULL);
	const VDFileDialogOption sOptions[]={
		{ VDFileDialogOption::kBool, 0, L"Ask for e&xtended options after this dialog", 0, 0 },
		{ VDFileDialogOption::kBool, 1, L"Automatically load linked segments", 0, 0 },
		{ VDFileDialogOption::kSelectedFilter, 2, 0, 0, 0 },
		{ VDFileDialogOption::kComboBox, 3, (const wchar_t *)&templates.front(), 0, 0 },
		{0}
	};

	int optVals[4]={0,1,0,0};

	VDStringW fname(VDGetLoadFileName(VDFSPECKEY_LOADVIDEOFILE, (VDGUIHandle)g_hWnd, L"Open video file", fileFilters.c_str(), NULL, sOptions, optVals));

	if (fname.empty())
		return;

	fExtendedOpen = !!optVals[0];
	fAutoscan = !!optVals[1];

	if (xlat[optVals[2]-1] >= 0)
		pDriver = inputDrivers[xlat[optVals[2]-1]];

	if (optVals[3]>0) {
		gFileList.clear();
		gFileList.push_back(VDTextWToA(fname.c_str()).c_str());

		TemplateInfoList::iterator theTemplate;

		theTemplate = gTemplateList.begin();

		theTemplate += (optVals[3]-1);

		char filetemp[MAX_PATH];

		if ((theTemplate>=gTemplateList.end()) || !CreateScript(theTemplate->sFileName_.c_str(), filetemp)) {
			gFileList.clear();
			guiMessageBox(g_hWnd, IDS_ERR_CREATE_SCRIPT, IDS_ERR_CAPTION, MB_ICONERROR|MB_OK);
			return;
		}

		fname = VDTextAToW(filetemp);

		gFileList.clear();
	}

	wcscpy(g_szInputAVIFileLastWorking, fname.c_str());

	VDAutoLogDisplay logDisp;
	g_project->Open(fname.c_str(), pDriver, fExtendedOpen, false, fAutoscan);
	logDisp.Post((VDGUIHandle)g_hWnd);

	AVSViewerOpen(g_hWnd);
}

void SaveAVI(HWND hWnd, bool fUseCompatibility) {
	if (!inputVideoAVI) {
		MessageBox(hWnd, "No input video stream to process.", g_szError, MB_OK);
		return;
	}

	static const VDFileDialogOption sOptions[]={
		{ VDFileDialogOption::kBool, 0, L"Don't run this job now; &add it to job control so I can run it in batch mode.", 0, 0 },
		{0}
	};

	VDRegistryAppKey key(g_szRegKeyPersistence);
	int optVals[1]={
		key.getBool("Run as job", false),
	};

	VDStringW fname(VDGetSaveFileName(VDFSPECKEY_SAVEVIDEOFILE, (VDGUIHandle)hWnd, fUseCompatibility ? L"Save AVI 1.0 File" : L"Save AVI 2.0 File", fileFilters0, L"avi", sOptions, optVals));
	if (!fname.empty()) {
		bool fAddAsJob = !!optVals[0];

		key.setBool("Run as job", fAddAsJob);

		if (fAddAsJob) {
			JobAddConfiguration(&g_dubOpts, g_szInputAVIFile, NULL, fname.c_str(), fUseCompatibility, &inputAVI->listFiles, 0, 0);
		} else {
			SaveAVI(fname.c_str(), false, NULL, fUseCompatibility);
		}
	}
}

// uiframe ********************************************************************

bool TranslateAcceleratorAvs(HWND hwnd, MSG& msg) {
	if (hwnd != g_hWnd)
		if (GetClassLong(hwnd, GCW_ATOM) == g_classAVS)
			if (TranslateAccelerator(hwnd, g_hAccelAVS, &msg)) return true;
	return false;
}

// gui ************************************************************************

int guiMessageBox(HWND hwnd, UINT idText, UINT idCaption, UINT uType) {
	char caption[256];
	char text[4096]; // increased by Fizick (for Avisynth functions list)
	bool error = false;

	// get caption
	if (LoadString(g_hInst, idCaption, (LPTSTR)caption, sizeof caption) == 0)
		error = true;

	// get message body
	if (LoadString(g_hInst, idText, (LPTSTR)text, sizeof text) == 0)
		error = true;

	if (error) {
		return MessageBox(hwnd, "Can't retrieve message!", "Internal Error", MB_OK);
	}

	return MessageBox(hwnd, text, caption, uType);
}

// projectui ******************************************************************

LRESULT VDProjectUI::MainWndProcMod(UINT msg, WPARAM wParam, LPARAM lParam, bool& isBreak) {
	isBreak = false;

	switch (msg) {

	case WM_VDM_REQUESTFILENAME:
		{
			VDM_FILENAME *fn = (PVDM_FILENAME) lParam;
			if (g_szInputAVIFile[0] != 0) {
				wcscpy(fn->name, g_szInputAVIFile);
			} else {
				*fn->name = 0;
			}
			PostMessage((HWND) wParam, WM_VDM_SENDFILENAME, 0, (LPARAM) fn);
			return 0;
		}
		break;

	case WM_VDM_SENDFILENAME:
		{
			VDM_FILENAME *pFN;
			pFN = (PVDM_FILENAME) lParam;
			wchar_t *pTemp = pFN->name;
			if (pTemp[0] != 0) {
				try	{
					wcscpy(g_szInputAVIFile, pTemp);
					g_project->Open(g_szInputAVIFile, NULL, false);
					wcscpy(g_szInputAVIFileLastWorking, g_szInputAVIFile);
				} catch(const MyError& e) {
					e.post((HWND)mhwnd, g_szError);
				}
			}
			delete [] pTemp;
			delete [] pFN;
			return 0;
		}
		break;

	case WM_VDM_REQUESTPOS:
		{
			VDM_POS *pos = (PVDM_POS) lParam;
			pos->pos = mpPosition->GetPosition();
			PostMessage((HWND) wParam, WM_VDM_SENDPOS, 0, (LPARAM) pos);
			return 0;
		}
		break;

	case WM_VDM_REQUESTRANGE:
		{
			VDM_RANGE *range = (PVDM_RANGE) lParam;
			range->range.from = GetSelectionStartFrame();
			range->range.to = GetSelectionEndFrame() + 1;
			PostMessage((HWND) wParam, WM_VDM_SENDRANGE, 0, (LPARAM) range);
			return 0;
		}
		break;

	case WM_VDM_REQUESTFRAMESET:
		{
			VDM_FRAMESET *fs = (PVDM_FRAMESET) lParam;
			FrameSubset& s = g_project->GetTimeline().GetSubset();
				FrameSubset::iterator walkit;
				fs->count = 0;
				for(walkit=s.begin(); walkit!=s.end();walkit++) 
					fs->count++;
				if (fs->count>0) {
					fs->ranges = new VDM_BASIC_RANGE[fs->count];
					int i = 0;
					for(walkit=s.begin(); walkit!=s.end();walkit++) {
						fs->ranges[i].from = walkit->start + 1;
						fs->ranges[i].to = walkit->start + walkit->len + 1;
						i++;
					}
				} else {
					fs->count = 0;
				}
			PostMessage((HWND) wParam, WM_VDM_SENDFRAMESET, 0, (LPARAM) fs);
			return 0;
		}
		break;

	case WM_VDM_REQUESTFRAMESIZE:
		{
			VDM_FRAMESIZE *fs = (PVDM_FRAMESIZE) lParam;
			fs->frame.left = 0;
			fs->frame.top = 0;
			fs->frame.right = 0;
			fs->frame.bottom = 0;
			if (inputVideoAVI) {
				BITMAPINFOHEADER *formatIn = inputVideoAVI->getImageFormat();

				if (formatIn) {
					BITMAPINFOHEADER *dcf = inputVideoAVI->getDecompressedFormat();
					switch (fs->frametype) {
						case 0: {
							fs->frame.right = dcf->biWidth;
							fs->frame.bottom = dcf->biHeight;
						}
						break;
						case 1: {
							if (!filters.isRunning()) {
								if (g_dubber)
									break;
								filters.prepareLinearChain(&g_listFA, (Pixel *)(dcf+1), dcf->biWidth, dcf->biHeight, 16+8*g_dubOpts.video.mOutputFormat);
							}

							FilterInstance *fa_list;
							int x = 0, y = 0;
							fa_list = (FilterInstance *)g_listFA.tail.next;

							while(fa_list->next) {
								x += fa_list->x1;
								y += fa_list->y1;
								fa_list = (FilterInstance *)fa_list->next;
							}
							
							fs->frame.left = x;
							fs->frame.top = y;
							fs->frame.right = filters.OutputBitmap()->w;
							fs->frame.bottom = filters.OutputBitmap()->h;
						}
						default: break;
					}
				}
			}
			PostMessage((HWND) wParam, WM_VDM_SENDFRAMESIZE, 0, (LPARAM) fs);
		}
		break;

	case WM_VDM_SCIEDIT_UPDATE:
		AVSViewerChangePrefs((HWND)0);
		break;
	}

	isBreak = true;
	return 0;
}

bool MenuHitMod(HWND hwnd, UINT id) {
	switch (id) {

	case ID_EDIT_REFRESH:
		g_project->Reopen();
		return true;

	case ID_TOOLS_AVS:
		if (g_dllScintilla->ok) {
			AVSEdit(NULL, hwnd, true);
		} else {
			guiMessageBox(hwnd, IDS_ERR_SCILEXER_MISSING, IDS_ERR_CAPTION, MB_OK | MB_ICONERROR);
		}
		return true;
	}
}

// Preferences ****************************************************************

VDubModPreferences2 g_VDMPrefs2;

VDubModPreferences g_VDMPrefs = {
	FALSE,	// rewrite VBR headers with CBR values
	FALSE,	// don't really pay attention to AC3/DTS frame boundaries for AVI
	1,		// pack as many frames as necessary when muxing
	TRUE,	// keep corrupted data in MP3/AC3/DTS files
	500,	// default preload set to 500ms
	TRUE,	// disable preload for non AVI outputs
	FALSE,	// Well actually Priss rocks :p
	DubVideoOptions::M_FULL,	// default video mode : Full processing
	TRUE,	// show input video
	TRUE,	// show output video
	FALSE,	// don't auto-resize / center
	250,	// limit of 250 (kB or frames) for rec lists
	0,		// don't use AVI 'rec ' lists
};

int VDMKeysSortFunc(const VDMKeys* pka, const VDMKeys* pkb) {
	return strcmp(pka->desc[0].c_str(), pkb->desc[0].c_str());
}

void VDMKeysSort(VDMKeys* keys, int len) {
	qsort(keys, len, sizeof(VDMKeys), (int (__cdecl *)(const void *,const void *))VDMKeysSortFunc);
}

bool VDDialogPrefsScriptEditor::HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item) {
	switch(type) {
	case kEventAttach:
		mpBase = pBase;
		SetValue(kIDC_SINGLEINSTANCE, mPrefs.mModPrefs.m_bScriptEditorSingleInstance);
		SetValue(kIDC_AUTOPOPUP, mPrefs.mModPrefs.m_bScriptEditorAutoPopup);
		SetFontLabel();
		pBase->ExecuteAllLinks();
		return true;
	case kEventDetach:
	case kEventSync:
		mPrefs.mModPrefs.m_bScriptEditorSingleInstance = 0 != GetValue(kIDC_SINGLEINSTANCE);
		mPrefs.mModPrefs.m_bScriptEditorAutoPopup = 0 != GetValue(kIDC_AUTOPOPUP);
		return true;
	case kEventSelect:
		if (id == kIDC_FONT) {
			AVSViewerChooseFont(vdpoly_cast<IVDUIWindowW32 *>(pBase)->GetHandleW32());
		}
		return true;
	}
	return false;
}

void VDDialogPrefsScriptEditor::SetFontLabel() {
	VDStringW face(VDTextAToW(mPrefs.mAVSViewerFontFace));
	const wchar_t *s = face.c_str();
	SetCaption(kIDS_FONT_TEXT, VDswprintf(L"%s, %d pt", 2, &s, &mPrefs.mAVSViewerFontSize).c_str());
}

void VDDialogPrefsScriptEditor::InitFont(HWND hwnd, LPLOGFONT lplf) {
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), lplf);

	HDC hdc = GetDC(hwnd);

	lplf->lfHeight = -MulDiv(mPrefs.mAVSViewerFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	lplf->lfCharSet = GetTextCharset(hdc);

	ReleaseDC(hwnd, hdc);

	strcpy(lplf->lfFaceName, mPrefs.mAVSViewerFontFace.c_str());
}

void VDDialogPrefsScriptEditor::AVSViewerChooseFont(HWND hwnd) {
	LOGFONT lf;
	CHOOSEFONT cf;

	InitFont(hwnd, &lf);

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.Flags = CF_BOTH | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;

	if (!::ChooseFont(&cf)) return;

	mPrefs.mAVSViewerFontSize = cf.iPointSize / 10;
	mPrefs.mAVSViewerFontFace = lf.lfFaceName;

	SetFontLabel();

	return;
}

bool VDDialogPrefsScriptEditorKeys::HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item) {
	char buffer[255];
	switch(type) {
	case kEventAttach:
		mpBase = pBase;
		{
			IVDUIList *pList = vdpoly_cast<IVDUIList *>(pBase->GetControl(kIDC_LISTVIEW));
			IVDUIListView *pListView = vdpoly_cast<IVDUIListView *>(pList);
			
			if (pListView) {
				pListView->AddColumn(L"Description",  200, 0);
				pListView->AddColumn(L"Primary Key",  100, 0);
				pListView->AddColumn(L"Secondary Key",100, 0);

				for (int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
					mKeys[i].idx = -1;
					mKeys[i].row = -1;
					if (g_accelAVSCommand[i]>-1) {

						VDStringA desc[3], tmp;
						VDStringA::size_type pos;
						char* d = g_accelAVSDescription[i];

						for (int j=0, pos=0; j<3 && pos!=VDStringSpanA::npos; j++) {
							tmp = VDStringA(d);
							pos = tmp.find('\t');
							desc[j] = (pos == VDStringSpanA::npos) ? VDStringA(d) : VDStringA(d, pos);
							d += pos+1;
						}

						if (mPrefs.mKeytableAVS.command[i].altkey[0].key!=0) {
							GetKeyString(mPrefs.mKeytableAVS.command[i].altkey[0].fVirt, mPrefs.mKeytableAVS.command[i].altkey[0].key, (char *) &buffer, 255);
							desc[1] = buffer;
						}
						if (mPrefs.mKeytableAVS.command[i].altkey[1].key!=0) {
							GetKeyString(mPrefs.mKeytableAVS.command[i].altkey[1].fVirt, mPrefs.mKeytableAVS.command[i].altkey[1].key, (char *) &buffer, 255);
							desc[2] = buffer;
						}

						mKeys[i].idx = i;
						mKeys[i].desc[0] = desc[0];
						mKeys[i].desc[1] = desc[1];
						mKeys[i].desc[2] = desc[2];
						mKeys[i].command = mPrefs.mKeytableAVS.command[i];
					}
				}

				VDMKeysSort(mKeys, VDM_ACCEL_AVS_COUNT);

				LV_COLUMN lvcol;
				lvcol.mask = LVCF_FMT;
				lvcol.fmt = LVCFMT_CENTER;

				HWND hwnd = vdpoly_cast<IVDUIWindowW32 *>(pBase->GetControl(kIDC_LISTVIEW))->GetHandleW32();

				ListView_SetColumn(hwnd, 1, &lvcol);
				ListView_SetColumn(hwnd, 2, &lvcol);

				for (int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
					int idx = mKeys[i].idx;
					if (g_accelAVSCommand[idx]>-1) {
						mKeys[i].row = pList->AddItem(VDTextAToW(mKeys[i].desc[0]).c_str(), (uintptr)&mKeys[i]);
						pListView->SetItemText(i, 1, VDTextAToW(mKeys[i].desc[1]).c_str());
						pListView->SetItemText(i, 2, VDTextAToW(mKeys[i].desc[2]).c_str());
					}
				}
			}
		}
		pBase->ExecuteAllLinks();
		return true;
	case kEventDetach:
	case kEventSync:
		for (int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
			mPrefs.mKeytableAVS.command[mKeys[i].idx] = mKeys[i].command;
		}
		return true;
	case kEventSelect:
		if (id == kIDC_DEFAULT) {

			IVDUIList *pList = vdpoly_cast<IVDUIList *>(pBase->GetControl(kIDC_LISTVIEW));
			IVDUIListView *pListView = vdpoly_cast<IVDUIListView *>(pList);

			for (int i=0; i<VDM_ACCEL_AVS_COUNT; i++) {
				mKeys[i].command = g_accelAVSDefault.command[mKeys[i].idx];

				GetKeyString(mKeys[i].command.altkey[0].fVirt, mKeys[i].command.altkey[0].key, (char *) &buffer, 255);
				mKeys[i].desc[1] = buffer;

				GetKeyString(mKeys[i].command.altkey[1].fVirt, mKeys[i].command.altkey[1].key, (char *) &buffer, 255);
				mKeys[i].desc[2] = buffer;

				if (mKeys[i].row>-1) {
					pListView->SetItemText(mKeys[i].row, 1, VDTextAToW(mKeys[i].desc[1]).c_str());
					pListView->SetItemText(mKeys[i].row, 2, VDTextAToW(mKeys[i].desc[2]).c_str());
				}
			}
		}
		return true;
	case kEventDoubleClick:
		{
			IVDUIList *pList = vdpoly_cast<IVDUIList *>(pBase->GetControl(kIDC_LISTVIEW));
			IVDUIListView *pListView = vdpoly_cast<IVDUIListView *>(pList);

			if (pListView) {
				VDMKeys* keys = (VDMKeys*)pList->GetItemData(item);
				if (!keys) return true;

				IVDUIWindow* pDlg = VDCreateDialogFromResource(kIDD_SUBDIALOG, pWin);
				IVDUIBase* pBase = vdpoly_cast<IVDUIBase *>(pDlg);

				VDDialogScriptEditorKeysSub subDlg(keys);

				pBase->SetCallback(&subDlg, false);

				if (pBase->DoModal()) {

					GetKeyString(keys->command.altkey[0].fVirt, keys->command.altkey[0].key, (char *) &buffer, 255);
					keys->desc[1] = buffer;

					GetKeyString(keys->command.altkey[1].fVirt, keys->command.altkey[1].key, (char *) &buffer, 255);
					keys->desc[2] = buffer;

					pListView->SetItemText(item, 1, VDTextAToW(keys->desc[1]).c_str());
					pListView->SetItemText(item, 2, VDTextAToW(keys->desc[2]).c_str());
				}
			}
		}
		return true;
	}
	return false;
}

bool VDDialogScriptEditorKeysSub::HandleUIEvent(IVDUIBase *pBase, IVDUIWindow *pWin, uint32 id, eEventType type, int item) {
	char b[2];
	bool fVkP;
	bool fVkS;
	switch (type) {
	case kEventAttach:
		mpBase = pBase;
		{
			IVDUIWindow* setP = pBase->GetControl(kIDC_P_GROUP);
			IVDUIWindow* setS = pBase->GetControl(kIDC_S_GROUP);

			fVkP = mKeys->command.altkey[0].fVirt & FVIRTKEY;
			fVkS = mKeys->command.altkey[1].fVirt & FVIRTKEY;

			mEditPKey = VDCreateUIComboBox();
			mEditPAscii = VDCreateUITextEdit();
			mEditSKey = VDCreateUIComboBox();
			mEditSAscii = VDCreateUITextEdit();

			setP->AddChild(mEditPKey);
			setP->AddChild(mEditPAscii);
			setS->AddChild(mEditSKey);
			setS->AddChild(mEditSAscii);

			VDUIParameters params;

			params.Clear();
			params.SetI(nsVDUI::kUIParam_VAlign, nsVDUI::kCenter);
			params.SetI(nsVDUI::kUIParam_Align, nsVDUI::kRight);
			params.SetI(nsVDUI::kUIParam_MinW, 50);

			mEditPKey->SetID(kIDC_P_KEY);
			mEditSKey->SetID(kIDC_S_KEY);

			mEditPKey->Create(&params);
			mEditSKey->Create(&params);

			params.Clear();
			params.SetI(nsVDUI::kUIParam_VAlign, nsVDUI::kCenter);
			params.SetI(nsVDUI::kUIParam_Align, nsVDUI::kRight);
			params.SetI(nsVDUI::kUIParam_MinW, 12);
			params.SetB(nsVDUI::kUIParam_Sunken, true);

			mEditPAscii->SetID(kIDC_P_ASCII);
			mEditSAscii->SetID(kIDC_S_ASCII);

			mEditPAscii->Create(&params);
			mEditSAscii->Create(&params);

			pBase->Relayout();

			pWin->SetCaption(VDTextAToW(mKeys->desc[0]).c_str());

			int p, s;
			IVDUIList *pListP = vdpoly_cast<IVDUIList *>(mEditPKey);
			IVDUIList *pListS = vdpoly_cast<IVDUIList *>(mEditSKey);
			p = s = 0;
			for (int i=0; i<g_accelKEYKeycode[i]!=0; i++) {
				VDStringW temp = VDTextAToW(VDStringA(&g_accelKEYDescription[i][0]));
				pListP->AddItem(temp.c_str());
				pListS->AddItem(temp.c_str());
				if ((mKeys->command.altkey[0].key == g_accelKEYKeycode[i]) && fVkP) p = i;
				if ((mKeys->command.altkey[1].key == g_accelKEYKeycode[i]) && fVkS) s = i;
			}
			mEditPKey->SetValue(p);
			mEditSKey->SetValue(s);

			SetValue(kIDC_P_MODE, (fVkP)?1:0);
			SetValue(kIDC_S_MODE, (fVkS)?1:0);
	
			b[1]=0;
			if (!fVkP) {
				b[0]=mKeys->command.altkey[0].key;
				mEditPAscii->SetCaption(VDTextAToW(VDStringA(b)).c_str());
			}

			b[1]=0;
			if (!fVkS) {
				b[0]=mKeys->command.altkey[1].key;
				mEditSAscii->SetCaption(VDTextAToW(VDStringA(b)).c_str());
			}

			SendMessage(vdpoly_cast<IVDUIWindowW32 *>(mEditPAscii)->GetHandleW32(), EM_LIMITTEXT, 1, 0);
			SendMessage(vdpoly_cast<IVDUIWindowW32 *>(mEditSAscii)->GetHandleW32(), EM_LIMITTEXT, 1, 0);

			SetValue(kIDC_P_ALT,     (mKeys->command.altkey[0].fVirt & FALT)     ?1:0);
			SetValue(kIDC_P_CONTROL, (mKeys->command.altkey[0].fVirt & FCONTROL) ?1:0);
			SetValue(kIDC_P_SHIFT,   (mKeys->command.altkey[0].fVirt & FSHIFT)   ?1:0);
			SetValue(kIDC_S_ALT,     (mKeys->command.altkey[1].fVirt & FALT)     ?1:0);
			SetValue(kIDC_S_CONTROL, (mKeys->command.altkey[1].fVirt & FCONTROL) ?1:0);
			SetValue(kIDC_S_SHIFT,   (mKeys->command.altkey[1].fVirt & FSHIFT)   ?1:0);

			EnableDisableKeyEntries();
		}
		pBase->ExecuteAllLinks();
		return true;
	case kEventDetach:
	case kEventSync:
		return true;
	case kEventSelect:
		switch (id) {
		case kIDC_P_MODE:
		case kIDC_S_MODE:
		case kIDC_P_KEY:
		case kIDC_P_ASCII:
			EnableDisableKeyEntries();
			return true;
		case kID_OK:
			fVkP = !(GetValue(kIDC_P_MODE)==0);
			fVkS = !(GetValue(kIDC_S_MODE)==0);
			b[0]=0;
			WORD akey;
			if (fVkP)
				akey = g_accelKEYKeycode[GetValue(kIDC_P_KEY)];
			else {
				strncpy(b, VDTextWToA(mEditPAscii->GetCaption()).c_str(), 1);
				akey = b[0];
				if (akey==0) akey = (WORD) -1;
			}
			if (akey == (WORD) -1) {
				mKeys->command.altkey[0].key = 0;
				mKeys->command.altkey[1].key = 0;
				mKeys->command.altkey[0].fVirt = 0;
				mKeys->command.altkey[1].fVirt = 0;
			} else {
				mKeys->command.altkey[0].key = akey;
				if (fVkP)
					mKeys->command.altkey[0].fVirt = FVIRTKEY | (GetValue(kIDC_P_ALT)?FALT:0) | (GetValue(kIDC_P_CONTROL)?FCONTROL:0) | (GetValue(kIDC_P_SHIFT)?FSHIFT:0);
				else
					mKeys->command.altkey[0].fVirt = (GetValue(kIDC_P_ALT)?FALT:0);
				b[0]=0;
				if (fVkS)
					akey = g_accelKEYKeycode[GetValue(kIDC_S_KEY)];
				else {
					strncpy(b, VDTextWToA(mEditSAscii->GetCaption()).c_str(), 1);
					akey = b[0];
					if (akey==0) akey = (WORD) -1;
				}
				if (akey == (WORD) -1) {
					mKeys->command.altkey[1].key = 0;
					mKeys->command.altkey[1].fVirt = 0;
				} else {
					mKeys->command.altkey[1].key = akey;
					if (fVkS)
						mKeys->command.altkey[1].fVirt = FVIRTKEY | (GetValue(kIDC_S_ALT)?FALT:0) | (GetValue(kIDC_S_CONTROL)?FCONTROL:0) | (GetValue(kIDC_S_SHIFT)?FSHIFT:0);
					else
						mKeys->command.altkey[1].fVirt = (GetValue(kIDC_S_ALT)?FALT:0);
				}
			}
			pBase->EndModal(true);
			return true;
		case kID_Cancel:
			pBase->EndModal(false);
			return true;
		}
	}
	return false;
}

void VDDialogScriptEditorKeysSub::EnableDisableKeyEntries() {
	if (!mpBase) return;

	bool bPVirt = !(GetValue(kIDC_P_MODE)==0);
	bool bSVirt = !(GetValue(kIDC_S_MODE)==0);
	bool bPEmpty = (GetValue(kIDC_P_KEY)==0);

	mEditPKey->SetVisible(bPVirt);
	mEditPAscii->SetVisible(!bPVirt);
	mEditSKey->SetVisible(bSVirt);
	mEditSAscii->SetVisible(!bSVirt);

	mpBase->GetControl(kIDC_P_CONTROL)->SetEnabled(bPVirt);
	mpBase->GetControl(kIDC_P_SHIFT)->SetEnabled(bPVirt);

	mpBase->GetControl(kIDC_S_MODE)->SetEnabled(!bPEmpty);
	mpBase->GetControl(kIDC_S_ALT)->SetEnabled(!bPEmpty);
	mpBase->GetControl(kIDC_S_CONTROL)->SetEnabled(bSVirt&&(!bPEmpty));
	mpBase->GetControl(kIDC_S_SHIFT)->SetEnabled(bSVirt&&(!bPEmpty));

	mEditSKey->SetEnabled(!bPEmpty);
	mEditSAscii->SetEnabled(!bPEmpty);
}

void VDMSavePreferences(VDubModPreferences2& prefs) {
	SetConfigBinary("", g_szVDubModPrefs, (char *)&prefs.mModPrefs, sizeof prefs.mModPrefs);
	SetConfigBinary("", g_szKeytableAVS, (char *)&prefs.mKeytableAVS, sizeof prefs.mKeytableAVS);

	VDRegistryAppKey key("AVSViewer");

	key.setString("fontface", prefs.mAVSViewerFontFace.c_str());
	key.setInt("fontsize", prefs.mAVSViewerFontSize);
}

void LoadPreferencesMod() {
	DWORD dwSize;

	VDubModPreferences vdmPrefs;

	dwSize = QueryConfigBinary("", g_szVDubModPrefs, (char *)&vdmPrefs, sizeof vdmPrefs);

	if (dwSize) {
		if(dwSize > sizeof g_VDMPrefs)
			dwSize = sizeof g_VDMPrefs;
		memcpy(&g_VDMPrefs, &vdmPrefs, dwSize);
	}

	ACCELKEYTABLE_AVS KeytableAVS_t;

	dwSize = QueryConfigBinary("", g_szKeytableAVS, (char *)&KeytableAVS_t, sizeof KeytableAVS_t);

	if (dwSize) {
		if (dwSize > sizeof KeytableAVS_t) dwSize = sizeof KeytableAVS_t;
		memcpy(&g_accelAVS, &KeytableAVS_t, dwSize);
	}

	VDRegistryAppKey keyAvs("AVSViewer");

	g_VDMPrefs2.mAVSViewerFontSize = keyAvs.getInt("fontsize", 14);

	if (!keyAvs.getString("fontface", g_VDMPrefs2.mAVSViewerFontFace))
		g_VDMPrefs2.mAVSViewerFontFace = "FixedSys";

	VDMPreferencesUpdated();
}

void VDMPreferencesUpdated() {
	DestroyAcceleratorTable(g_hAccelAVS);
	g_hAccelAVS = CreateAVSAccelerators();

	PostMessage(g_hWnd, WM_VDM_SCIEDIT_UPDATE, 0, 0);
}

// AVSViewer ******************************************************************

void AVSViewerOpen(HWND hwnd) {
	if (IsScriptType(g_szInputAVIFile,SCRIPTTYPE_AVS) && g_VDMPrefs.m_bScriptEditorAutoPopup && g_dllScintilla->ok) {
		HWND tmp = AVSEdit(NULL, (HWND)hwnd, false);
		if (g_VDMPrefs.m_bScriptEditorSingleInstance) {
			VDM_FILENAME *fn = new VDM_FILENAME;
			fn->name = new wchar_t[MAX_PATH];
			if (g_szInputAVIFile[0] != 0) {
				wcscpy(fn->name, g_szInputAVIFile);
			} else {
				*fn->name = 0;
			}
			PostMessage(tmp, WM_VDM_SENDFILENAME, 0, (LPARAM) fn);
		}
	}
}

void AVSViewerLoadSettings(HWND hwnd) {
	if (!hwnd) return;
	VDUIRestoreWindowPlacementW32(hwnd, REG_KEY_SCRIPTEDITOR);
}

void AVSViewerSaveSettings(HWND hwnd) {
	if (!hwnd) return;
	VDUISaveWindowPlacementW32(hwnd, REG_KEY_SCRIPTEDITOR);
}

BOOL CALLBACK SendMessageProc(HWND hwnd, LPARAM lParam) {
	char szClass[1024];
	GetClassName(hwnd, szClass, sizeof(szClass));
	if (strcmp(szClass, szAVSEditorClassName) == 0)
		SendMessage(hwnd, lParam, 0, 0);
	return TRUE;
}

void AVSViewerChangePrefs(HWND hwnd) {
	EnumChildWindows(hwnd, SendMessageProc, (LPARAM) WM_VDM_SCIEDIT_UPDATE);
}
