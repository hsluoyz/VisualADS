#include "StdAfx.h"
#include "MyMessageBox.h"
#include "MainFrm.h"
#include "PropertiesViewBar.h"
#include "resource.h"
#define ID_GUI_MESSAGE 0x2000

CMainFrame *g_pMainFrame = NULL;

PMParam::PMParam(CString strParam1)
{
	this->strParam1 = strParam1;
}

PMParam::PMParam(CString strParam1, CString strParam2)
{
	this->strParam1 = strParam1;
	this->strParam2 = strParam2;
}

void MyMessageBox_Error(CString strText, CString strCaption)
{
	//return; //akisn0w
	PMParam *pParam = new PMParam(strText, strCaption);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 0, (WPARAM) pParam, NULL);
}

void MyMessageBox_Error(CString strFunction)
{
	//return; //akisn0w
	PMParam *pParam = new PMParam(strFunction);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 0, (WPARAM) pParam, NULL);
}

void Output_ConsoleList(CString strContent)
{
	PMParam *pParam = new PMParam(_T(""), strContent);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 1, (WPARAM) pParam, NULL);
}

void Output_RacerProCache(CString strNo, CString strContent)
{
	PMParam *pParam = new PMParam(strNo, strContent);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 2, (WPARAM) pParam, NULL);
}

void Output_ReportList(CString strNo, CString strContent)
{
	PMParam *pParam = new PMParam(strNo, strContent);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 3, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_Basic(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 23, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_Extend(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 24, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_Father(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 25, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_ChildGroup(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 26, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_ChildUser(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 27, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_Access(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 28, (WPARAM) pParam, NULL);
}

void Property_AddKeyValue_Network(CString strKey, CString strValue)
{
	PMParam *pParam = new PMParam(strKey, strValue);
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 29, (WPARAM) pParam, NULL);
}

void Property_ClearKeyValues()
{
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 5, NULL, NULL);
}

void Property_UpdateKeyValues()
{
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 6, NULL, NULL);
}

void ProgressBar_Start()
{
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 7, NULL, NULL);
}

void ProgressBar_Stop()
{
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 8, NULL, NULL);
	/*
	if (bResult == FALSE)
	{
		DWORD dwError = GetLastError();
		MyMessageBox_Error(_T("ProgressBar_Stop"));
	}
	*/
}

void Canvas_Invalidate()
{
	AfxGetApp()->m_pMainWnd->SendMessage(ID_GUI_MESSAGE + 9, NULL, NULL);
}