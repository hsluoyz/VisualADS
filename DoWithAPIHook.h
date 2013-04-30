#include "Hook.h"
HOOKINFO g_Hook_CreateFontIndirect;
HOOKINFO g_Hook_MessageBox;

typedef HFONT (WINAPI *TypeCreateFontIndirect)(LOGFONT* lplf);
typedef int (WINAPI *TypeMessageBox)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

HFONT WINAPI MyCreateFontIndirect(LOGFONT* lplf)
{
	TypeCreateFontIndirect pOrigin = (TypeCreateFontIndirect) g_Hook_CreateFontIndirect.Stub;
	if (lplf->lfHeight < 12)
		lplf->lfHeight = 12;
	_tcscpy(lplf->lfFaceName, _T("simsun"));
	//lplf->lfFaceName = L"Marlett"; //"Segoe UI" "Arial" "MS Shell Dlg"
	return pOrigin(lplf);
}

int WINAPI MyMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	TypeMessageBox pOrigin = (TypeMessageBox) g_Hook_MessageBox.Stub;

	CString strMask = _T("将改动保存到 Windows域安全卫士？");
	CString strToCheck = lpText;
	if (strMask == strToCheck)
	{
		return IDNO;
	}
	else
	{
		int iResult = pOrigin(hWnd, lpText, lpCaption, uType);
		return iResult;
	}
}

void doWithTheAPIHookStuff()
{
#ifdef UNICODE
	HEInitHook(&g_Hook_CreateFontIndirect, _T("gdi32"), _T("CreateFontIndirectW"), (void*) MyCreateFontIndirect);
	HEInitHook(&g_Hook_MessageBox, _T("user32"), _T("MessageBoxW"), (void*) MyMessageBox);
#else
	HEInitHook(&g_Hook_CreateFontIndirect, _T("gdi32"), _T("CreateFontIndirectA"), (void*) MyCreateFontIndirect);
	HEInitHook(&g_Hook_MessageBox, _T("user32"), _T("MessageBoxA"), (void*) MyMessageBox);
#endif
	HEStartHook(&g_Hook_CreateFontIndirect);
	HEStartHook(&g_Hook_MessageBox);
}
