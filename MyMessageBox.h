class CMainFrame;
extern CMainFrame *g_pMainFrame;

#ifndef _CLASS_PMPARAM
#define  _CLASS_PMPARAM

class PMParam
{
public:
	CString strParam1;
	CString strParam2;

public:
	PMParam(CString strParam1);
	PMParam(CString strParam1, CString strParam2);
};

#endif

void MyMessageBox_Error(CString strText, CString strCaption);
void MyMessageBox_Error(CString strFunction);

void Output_ConsoleList(CString strContent);
void Output_RacerProCache(CString strNo, CString strContent);
void Output_ReportList(CString strNo, CString strContent);

void Property_AddKeyValue_Basic(CString strKey, CString strValue);
void Property_AddKeyValue_Extend(CString strKey, CString strValue);
void Property_AddKeyValue_Father(CString strKey, CString strValue);
void Property_AddKeyValue_ChildGroup(CString strKey, CString strValue);
void Property_AddKeyValue_ChildUser(CString strKey, CString strValue);
void Property_AddKeyValue_Access(CString strKey, CString strValue);
void Property_AddKeyValue_Network(CString strKey, CString strValue);
void Property_ClearKeyValues();
void Property_UpdateKeyValues();

void ProgressBar_Start();
void ProgressBar_Stop();

void Canvas_Invalidate();

void Output_Update_Connection(UINT nEvent, void *pThis);
BOOL Global_GetLanguage();