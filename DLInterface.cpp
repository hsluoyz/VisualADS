#include "StdAfx.h"

#include "DLInterface.h"
#include "DLRacerInterface.h"

#include "ADDomain.h"
#include "ADGroup.h"
#include "ADUser.h"
#include "ADShareFolder.h"
#include "ADDirectorySearch.h"

#include "MyMessageBox.h"

#include <fstream>
#include <iostream>
using namespace std;

#include <TCHAR.H>

//This class provides methods to detect policy conflicts via DLRacerInterface TCP interface.
//these two methods are mostly used for "Start RacerPro Engine" button, they just connect to RacerPro instead of doing any detection stuff.
DLInterface::DLInterface()
{
	bChineseOrEnglish = Global_GetLanguage();
	m_iMixOrShareOrNTFS = 2; //just to differentiate the Share and NTFS detection, used in queryAllConflicts function.
	initInputDir(); //initiate the directory
	m_strReportPathFile = m_strInputDir + _T("Report.txt");
	m_iSelfStartedEngine = startEngine(); //if we start the engine, we'll be responsible to put it to an end.
	if (m_iSelfStartedEngine == 0)
	{
		MyMessageBox_Error(_T("RacerPro推理引擎已经启动！"), _T("错误提示信息"));
	}
	m_racerpro = NULL;
	return; //akisn0w

	m_racerpro = new DLRacerInterface(_T("127.0.0.1"), 8088); //local RacerPro
	int iTry = 0; //the time to attempt connecting with RacerPro, the below iTry == 2 means only we give it 2 chances.
	int iWindowStatePrev = 0;
	int iWindowState = checkEngine(); //check RacerPro started
	while (m_racerpro->openConnection() == FALSE) //these check codes may not make much sense, igoring these is a choice.
	{
		iTry ++;
		iWindowStatePrev = iWindowState;
		iWindowState = checkEngine();
		if (iWindowStatePrev == 1 && iWindowState == 0) //can't believe RacerPro shutdown by somebody.
		{
			MyMessageBox_Error(_T("DLInterface"));
			return;
		}
		if (iTry == 2)
		{
			MyMessageBox_Error(_T("DLInterface"));
			return;
		}
		Sleep(500); //wait for 0.5s
	}

	submitBlankInformation(); //send 4 commands to RacerPro for initialization purpose.
}

DLInterface::DLInterface(CString strRacerIP)
{
	bChineseOrEnglish = Global_GetLanguage();
	m_iMixOrShareOrNTFS = 2;
	initInputDir();
	m_strReportPathFile = m_strInputDir + _T("Report.txt");
	m_iSelfStartedEngine = 0;
	m_racerpro = new DLRacerInterface(strRacerIP, 8088); //remote RacerPro, no check code needed.
	int iTry = 0;
	if (m_racerpro->openConnection() == FALSE)
	{
		return;
	}

	submitBlankInformation();
}

//these two methods actually provide the detecting functionality.
DLInterface::DLInterface(vector<ADDomain*> npDomains, int iMixOrShareOrNTFS)
{
	bChineseOrEnglish = Global_GetLanguage();
	m_npDomains = npDomains;
	m_iMixOrShareOrNTFS = iMixOrShareOrNTFS; //Share or NTFS detection , the same method, only literal difference.
	initInputDir();
	m_strReportPathFile = m_strInputDir + _T("Report.txt");
	m_iSelfStartedEngine = startEngine();
	m_racerpro = new DLRacerInterface(_T("127.0.0.1"), 8088);
	int iTry = 0;
	int iWindowStatePrev = 0;
	int iWindowState = checkEngine();
	while (m_racerpro->openConnection() == FALSE)
	{
		iTry ++;
		iWindowStatePrev = iWindowState;
		iWindowState = checkEngine();
		if (iWindowStatePrev == 1 && iWindowState == 0)
		{
			MyMessageBox_Error(_T("DLInterface3"));
			return;
		}
		if (iTry == 2)
		{
			MyMessageBox_Error(_T("DLInterface3"));
			return;
		}
		Sleep(500);
	}
	
	initTBoxVectors();
	initSWRLRVectors();
	initABoxVectors();
	for (int i = 0; i < npDomains.size(); i ++)
	{
		addDomain(npDomains[i], iMixOrShareOrNTFS);
	}
	submitInformation();
}

DLInterface::DLInterface(vector<ADDomain*> npDomains, CString strRacerIP, int iMixOrShareOrNTFS)
{
	bChineseOrEnglish = Global_GetLanguage();
	m_npDomains = npDomains;
	m_iMixOrShareOrNTFS = iMixOrShareOrNTFS;
	initInputDir();
	m_strReportPathFile = m_strInputDir + _T("Report.txt");
	m_iSelfStartedEngine = 0;
	m_racerpro = new DLRacerInterface(strRacerIP, 8088);
	int iTry = 0;
	if (m_racerpro->openConnection() == FALSE)
	{
		return;
	}
	
	initTBoxVectors();
	initSWRLRVectors();
	initABoxVectors();
	for (int i = 0; i < npDomains.size(); i ++)
	{
		addDomain(npDomains[i], iMixOrShareOrNTFS);
	}
	submitInformation();
}

DLInterface::DLInterface(vector<ADDomain*> npDomains, BOOL iMixOrShareOrNTFS, BOOL bFileInterface)
{
	bChineseOrEnglish = Global_GetLanguage();
	if (!bFileInterface)
	{
		return;
	}

	m_npDomains = npDomains;
	m_racerpro = NULL;

	m_iMixOrShareOrNTFS = iMixOrShareOrNTFS;
	m_bFileInterface = bFileInterface;
	initInputDir();
	m_strReportPathFile = m_strInputDir + _T("Report.txt");
	m_strOntologyPathFile = m_strInputDir + _T("RacerPro-1-9-2-beta\\ontology.racer");
	m_strQueryPathFile = m_strInputDir + _T("RacerPro-1-9-2-beta\\query.racer");
	m_strOutputPathFile = m_strInputDir + _T("RacerPro-1-9-2-beta\\output1.txt");
	updateOutputFileName();
	
	initTBoxVectors();
	initABoxVectors();
	for (int i = 0; i < npDomains.size(); i ++)
	{
		addDomain(npDomains[i], iMixOrShareOrNTFS);
	}
	initSWRLRVectors();

	prepareOntologyString();
	generateOntologyFile();
}

DLInterface::~DLInterface()
{
	if (m_racerpro)
	{
		m_racerpro->closeConnection();
	}
	//endEngine();
}

void DLInterface::addToReport(CString strText)
{
	m_nstrReports.push_back(strText);
}

void DLInterface::writeReportFile()
{
	ofstream fOut(m_strReportPathFile, ios::out);
	if (!fOut)
	{
		MyMessageBox_Error(_T("writeReportFile"));
		return;
	}
	
	for (int i = 0; i < m_nstrReports.size(); i ++)
	{
		fOut.write(m_nstrReports[i], m_nstrReports[i].GetLength());
		fOut.write(_T("\n"), 1);
	}
	fOut.close();

	m_nstrReports.clear();
}

int DLInterface::getNumberFromOutputFileName()
{
	int iEnd = m_strOutputPathFile.ReverseFind(_T('.'));
	int iStart = m_strOutputPathFile.ReverseFind(_T('\\'));
	if (iStart == -1 || iEnd == -1)
	{
		MyMessageBox_Error(_T("getNumberFromOutputFileName"));
		return -1;
	}
	iStart ++;
	iStart = m_strOutputPathFile.Find(_T("output"), iStart);
	if (iStart == -1)
	{
		MyMessageBox_Error(_T("getNumberFromOutputFileName"));
		return -1;
	}
	iStart += 6;
	
	CString strNumber = m_strOutputPathFile.Mid(iStart, iEnd - iStart);
	int iNumber = stoi(strNumber);
	return iNumber;
}

void DLInterface::updateOutputFileName()
{
	while (FileExist(m_strOutputPathFile))
	{
		CString strNumber = itos(getNumberFromOutputFileName() + 1);
		m_strOutputPathFile = m_strInputDir + _T("RacerPro-1-9-2-beta\\output");
		m_strOutputPathFile += strNumber;
		m_strOutputPathFile += _T(".txt");
	}
}

void DLInterface::generateOntologyFile()
{
	CStdioFile pFile;
	pFile.Open(m_strOntologyPathFile, CFile::modeCreate | CFile::modeWrite);

	//TBox
	for (int i = 0; i < m_nstrTBox.size(); i ++)
	{
		pFile.WriteString(m_nstrTBox[i]);
		pFile.WriteString(_T("\n"));
	}

	pFile.WriteString(_T("\n"));

	//ABox
	for (i = 0; i < m_nstrABox.size(); i ++)
	{
		pFile.WriteString(m_nstrABox[i]);
		pFile.WriteString(_T("\n"));
	}

	pFile.WriteString(_T("\n"));

	//SWRLR
	for (i = 0; i < m_nstrSWRLR.size(); i ++)
	{
		pFile.WriteString(m_nstrSWRLR[i]);
		pFile.WriteString(_T("\n"));
	}

	pFile.Close();
}

void DLInterface::generateQueryFile()
{
	CStdioFile pFile;
	pFile.Open(m_strQueryPathFile, CFile::modeCreate | CFile::modeWrite);
	
	for (int i = 0; i < m_nstrQuerys.size(); i ++)
	{
		pFile.WriteString(m_nstrQuerys[i]);
		pFile.WriteString(_T("\n"));
	}
	
	pFile.Close();
}

void DLInterface::clearQueryString()
{
	m_nstrQuerys.clear();
}

BOOL DLInterface::deleteFile(CString strFilePathName)
{
	SHFILEOPSTRUCT FileOp={0};
	FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
		FOF_NOCONFIRMATION; //不出现确认对话框
	FileOp.pFrom = strFilePathName;
	FileOp.pTo = NULL;      //一定要是NULL
	FileOp.wFunc = FO_DELETE;    //删除操作
	return SHFileOperation(&FileOp) == 0;
}

void DLInterface::deleteOutputFile()
{
// 	CString strOutputPathFile = m_strInputDir + _T("RacerPro-1-9-2-beta\\output.txt");
// 	if (FileExist(strOutputPathFile))
// 	{
// 		deleteFile(strOutputPathFile);
// 	}
}

void DLInterface::MyWinExec(CString strCmdLine)
{
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
	
	TCHAR buf[1024];
	_tcscpy(buf, strCmdLine);
    // Start the child process. 
    if (!CreateProcess( NULL, // No module name (use command line). 
        buf,			 // Command line. 
        NULL,            // Process handle not inheritable. 
        NULL,            // Thread handle not inheritable. 
        FALSE,           // Set handle inheritance to FALSE. 
        0,               // No creation flags. 
        NULL,            // Use parent's environment block. 
        NULL,            // Use parent's starting directory. 
        &si,             // Pointer to STARTUPINFO structure.
        &pi )            // Pointer to PROCESS_INFORMATION structure.
		) 
    {
        MyMessageBox_Error(_T("MyWinExec"));
		return;
    }
	
    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);
	
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void DLInterface::executeRacerForFile()
{
	updateOutputFileName();
	CString strRacerEXEPathName = m_strInputDir + _T("RacerPro-1-9-2-beta\\RacerPro.exe");
	//RacerPro.exe +c -- -f ontology.racer -q query.racer -o output.txt
	//CString strCommand = strRacerEXEPathName + _T(" +c -- -f ontology.racer -q query.racer -o output.txt");
	CString strCommand;
	strCommand.Format(_T("%s +c -- -f %s -q %s -o %s"), strRacerEXEPathName, m_strOntologyPathFile, m_strQueryPathFile,
		m_strOutputPathFile);
	//WinExec(strCommand, SW_HIDE);
	MyWinExec(strCommand);
}

BOOL DLInterface::evaluateLine(CString strLine)
{
	if (strLine.Find(_T("(retrieve"), 0) != -1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL DLInterface::FileExist(CString strFileName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strFileName, &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	else
	{
		FindClose(hFind);
		return true;
	}
	return false;
}

void DLInterface::parseOutputFileToAnswerString()
{
	BOOL bRetrieveMode = FALSE;
	m_nstrAnswers.clear();
	CString strLine;
	CString strTemp;
	CStdioFile pFile;
	CFileException fileException;

	int iFlag = 1;
	int iTryTime = 10;
	while (!FileExist(m_strOutputPathFile))
	{
		Sleep(300);
		iTryTime --;
		if (iTryTime == 0)
		{
			iFlag = 0;
			break;
		}
	}
	if (iFlag == 0)
	{
		MyMessageBox_Error(_T("parseOutputFileToAnswerString"));
		return;
	}

	char line[1024];
	ifstream readf(m_strOutputPathFile, ios::in);
	if (!readf.is_open())
	{
		MyMessageBox_Error(_T("parseOutputFileToAnswerString"));
		return;
	}
	Sleep(3000);

	while (!readf.eof())
	{
		readf.getline(line, sizeof(line));
		strLine = line;

		if (evaluateLine(strLine) == TRUE)
		{
			bRetrieveMode = TRUE;
		}
		
		if (bRetrieveMode == TRUE)
		{
			if (evaluateLine(strLine) == TRUE)
			{
				if (strTemp != _T(""))
				{
					m_nstrAnswers.push_back(strTemp);
					strTemp = _T("");
				}
				strTemp = strLine;
			}
			else
			{
				strTemp += strLine;
			}
		}
	}

// 	if (!pFile.Open(m_strOutputPathFile, CFile::modeRead | CFile::typeText, &fileException))
// 	{
// 		int aaa = fileException.m_cause;
// 		MyMessageBox_Error(_T("parseOutputFileToAnswerString"));
// 		return;
// 	}
// 
// 	while (pFile.ReadString(strLine))
// 	{
// 		if (strLine.GetLength() == 0)
// 		{
// 			continue;
// 		}
// 		if (evaluateLine(strLine) == TRUE)
// 		{
// 			bRetrieveMode = TRUE;
// 		}
// 
// 		if (bRetrieveMode == TRUE)
// 		{
// 			if (evaluateLine(strLine) == TRUE)
// 			{
// 				if (strTemp != _T(""))
// 				{
// 					m_nstrAnswers.push_back(strTemp);
// 					strTemp = _T("");
// 				}
// 				strTemp = strLine;
// 			}
// 			else
// 			{
// 				strTemp += strLine;
// 			}
// 		}
// 	}
//	pFile.Close();

	if (strTemp != _T(""))
	{
		m_nstrAnswers.push_back(strTemp);
		strTemp = _T("");
	}
	
	readf.close();
	deleteOutputFile();
}

void DLInterface::answerString2ConflictString()
{
	m_iCurConflict = 0;
	m_nstrConflicts.clear();
	for (int i = 0; i < m_nstrAnswers.size(); i ++)
	{
		int iPos = m_nstrAnswers[i].Find(_T(" --> "));
		if (iPos == -1)
		{
			MyMessageBox_Error(_T("answerString2ConflictString"));
			return;
		}
		else
		{
			iPos += 5;
		}
		m_nstrConflicts.push_back(m_nstrAnswers[i].Mid(iPos, m_nstrAnswers[i].GetLength() - iPos));
	}
}

void DLInterface::answerString2ReasonString()
{
// 	m_nstrReasons = m_nstrAnswers;
// 	m_iCurReason = 0;

	m_iCurReason = 0;
	m_nstrReasons.clear();
	for (int i = 0; i < m_nstrAnswers.size(); i ++)
	{
		int iPos = m_nstrAnswers[i].Find(_T(" --> "));
		if (iPos == -1)
		{
			MyMessageBox_Error(_T("answerString2ConflictString"));
			return;
		}
		else
		{
			iPos += 5;
		}
		m_nstrReasons.push_back(m_nstrAnswers[i].Mid(iPos, m_nstrAnswers[i].GetLength() - iPos));
	}
}

void DLInterface::answerString2CommonInstanceString()
{
// 	m_nstrCommonInstances = m_nstrAnswers;
// 	m_iCurCommonInstance = 0;

	m_iCurCommonInstance = 0;
	m_nstrCommonInstances.clear();
	for (int i = 0; i < m_nstrAnswers.size(); i ++)
	{
		int iPos = m_nstrAnswers[i].Find(_T(" --> "));
		if (iPos == -1)
		{
			MyMessageBox_Error(_T("answerString2ConflictString"));
			return;
		}
		else
		{
			iPos += 5;
		}
		m_nstrCommonInstances.push_back(m_nstrAnswers[i].Mid(iPos, m_nstrAnswers[i].GetLength() - iPos));
	}
}

BOOL DLInterface::getSelfStarted()
{
	return (BOOL) m_iSelfStartedEngine;
}

BOOL DLInterface::checkConnection(CString strRacerIP)
{
	DLRacerInterface *racerpro = new DLRacerInterface(strRacerIP, 8088);
	BOOL bResult = racerpro->openConnection();
	racerpro->closeConnection();
	return bResult;
}

CString DLInterface::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

int DLInterface::stoi(CString s)
{
	return atoi(s);
}

BOOL DLInterface::FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}

void DLInterface::initInputDir()
{
	CString cstrDir;
	TCHAR cBuffer[260];
	GetCurrentDirectory(MAX_PATH, cBuffer);
	CString cstrPath(cBuffer);
	if (FolderExist(cstrPath + _T("\\input")))
	{
		cstrDir = cstrPath;
	}
	else
	{
		int nCount = cstrPath.ReverseFind(_T('\\'));
		cstrPath.Delete(nCount, cstrPath.GetLength() - nCount);
		
		if (FolderExist(cstrPath + _T("\\input")))
		{
			cstrDir = cstrPath;
		}
		else
		{
			MyMessageBox_Error(_T("initInputDir Error"), _T("Error"));
			return;
		}
	}
	cstrDir += _T("\\input\\");
	m_strInputDir = cstrDir;
}

CString DLInterface::doubleBackSlash(CString strLine)
{
	CString strResult;
	CString strChar;
	for (int i = 0; i < strLine.GetLength(); i ++)
	{
		strChar = strLine.Mid(i, 1);
		if (strChar == _T("\\"))
		{
			strResult += strChar;
			strResult += strChar;
		}
		else
		{
			strResult += strChar;
		}
	}
	return strResult;
}

BOOL DLInterface::createProcess(CString strExeName, CString strCmdLine)
{
	TCHAR pszCmdLine[MAX_PATH];
	_tcscpy(pszCmdLine, strCmdLine);
	STARTUPINFO si= {sizeof(si)};
	PROCESS_INFORMATION pi;
	BOOL bResult = CreateProcess(strExeName, pszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	
	if(bResult)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	return bResult;
}

BOOL DLInterface::killProcess(CString strWindowTitle)
{
    HANDLE hProcessHandle;  
    ULONG nProcessID;
    HWND hWnd;
	
    hWnd = ::FindWindow(NULL, strWindowTitle);
	if (hWnd == NULL)
	{
		return TRUE;
	}
	else
	{
		::GetWindowThreadProcessId(hWnd, &nProcessID);
		hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
		return ::TerminateProcess(hProcessHandle, 4);
	}
}

//if there's no RacerPro process running, why not start one?
BOOL DLInterface::startEngine()
{
	HWND hWnd = ::FindWindow(NULL, _T("RacerPro"));
	if (hWnd)
	{
		return 0;
	}
	else
	{
		CString strRacerDir =  m_strInputDir + _T("RacerPro-1-9-2-beta");
		if (FolderExist(strRacerDir))
		{
			CString strRacerEXE = strRacerDir + _T("\\RacerPro.exe");
			CString strRunAsDateEXE = m_strInputDir + _T("RunAsDate.exe");
			CString strParameter = _T("/immediate /movetime 05\\05\\2011 00:00:00 \"") + strRacerEXE + _T("\"");
			//ShellExecute(NULL, _T("Open"), strRunAsDateEXE, strParameter, _T(""), SW_SHOWNORMAL); //SW_SHOWNORMAL | SW_HIDE
			ShellExecute(NULL, _T("Open"), strRacerEXE, _T(""), _T(""), SW_SHOWNORMAL); //SW_SHOWNORMAL | SW_HIDE
			//akisn0w
			return 1;
		}
		else
		{
			MyMessageBox_Error(_T("startEngine Error"), _T("Error"));
			return -1;
		}
	}
}

BOOL DLInterface::checkEngine()
{
	HWND hWnd = ::FindWindow(NULL, _T("RacerPro"));
	return (hWnd != NULL);
}

void DLInterface::endEngine()
{
	HWND hWnd = ::FindWindow(NULL, _T("RacerPro"));
	if (hWnd)
	{
		DWORD nProcessID;
		::GetWindowThreadProcessId(hWnd, &nProcessID);
		HANDLE hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE, nProcessID);
		::TerminateProcess(hProcessHandle, 4);
	}
}

void DLInterface::strVev2File(CString strFile, vector<CString> nsString)
{
	ofstream fOut(strFile, ios::out);
	if (!fOut)
	{
		MyMessageBox_Error(_T("strVev2File"));
		return;
	}

	for (int i = 0; i < nsString.size(); i ++)
	{
		fOut.write(nsString[i], nsString[i].GetLength());
		fOut.write(_T("\n"), 1);
	}
	fOut.close();
}

void DLInterface::file2StrVec(CString strFile, vector<CString>& nsString)
{
	ifstream fIn(strFile, ios::in);
	if (!fIn)
	{
		MyMessageBox_Error(_T("file2StrVec"));
		return;
	}
	CString s;
	char c;
	while (fIn.get(c))
	{
		if (c != '\n' && c != '\t')
			s += c;
	}
	fIn.close();
	
	int index = 0;
	int pos1, pos2;
	int level = 0;
	
	int head = 0;
	int rear = 0;
	
	int flag = 1;
	while (index < s.GetLength() && flag)
	{
		pos1 = s.Find(_T("("), index);
		pos2 = s.Find(_T(")"), index);
		
		if (pos1 == -1 && pos2 == -1)
		{
			break;
			flag  = 0;
		}
		else if (pos1 == -1)
		{
			level --;
			index = pos2 + 1;
		}
		else if (pos1 < pos2)
		{
			level ++;
			index = pos1 + 1;
		}
		else if (pos2 < pos1)
		{
			level --;
			index = pos2 + 1;
		}
		
		if (level == 0)
		{
			head = rear;
			rear = index;
			
			while (s[head] == _T(' ') || s[head] == _T('\t') || s[head] == _T('\n') || s[head] == -1)
				head ++;
			nsString.push_back(s.Mid(head, rear - head));
			//cout << endl << "///////////////////////////////////////////////////////////" << endl;
			//cout << s.substr(head, rear - head);
			//cout << endl << "///////////////////////////////////////////////////////////" << endl;
		}
		
	}
}

void DLInterface::initTBoxVectors()
{
	m_nstrTBox.clear();
	file2StrVec(m_strInputDir + _T("TBox.racer"), m_nstrTBox);
}

void DLInterface::initSWRLRVectors()
{
	m_nstrSWRLR.clear();
	file2StrVec(m_strInputDir + _T("SWRLR.racer"), m_nstrSWRLR);
}

void DLInterface::initResetVectors()
{
	m_nstrReset.clear();
	m_nstrReset.push_back(_T("(full-reset)"));
	m_nstrReset.push_back(_T("(restore-standard-settings)"));
	m_nstrReset.push_back(_T("(process-set-at-a-time)"));
	m_nstrReset.push_back(_T("(add-rule-consequences-automatically)"));
}

void DLInterface::initABoxVectors()
{
	m_nstrABox.clear();

	insertInstanceAssertion(_T("Einstein"), _T("Abstract"));

	insertInstanceAssertion(_T("R"), _T("Action"));
	insertInstanceAssertion(_T("X"), _T("Action"));
	insertInstanceAssertion(_T("W"), _T("Action"));

	insertRelationAssertion(_T("R"), _T("Einstein"), _T("is_Action_R"));
	insertRelationAssertion(_T("X"), _T("Einstein"), _T("is_Action_X"));
	insertRelationAssertion(_T("W"), _T("Einstein"), _T("is_Action_W"));

	insertInstanceAssertion(_T("Permit"), _T("Permission"));
	insertInstanceAssertion(_T("Deny"), _T("Permission"));
	insertRelationAssertion(_T("Permit"), _T("Deny"), _T("has_Permission_Overlap"));
	insertRelationAssertion(_T("Deny"), _T("Permit"), _T("has_Permission_Overlap"));
}

void DLInterface::insertInstanceAssertion(CString strInstance, CString strConcept)
{
	CString strAssertion = _T("(instance ") + dnName2dlName(strInstance) + _T(" ") + strConcept + _T(")");
	m_nstrABox.push_back(strAssertion);
}

void DLInterface::insertRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation)
{
	CString strAssertion = _T("(related ") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T(")");
	m_nstrABox.push_back(strAssertion);
}

void DLInterface::submitBlankInformation()
{
	m_racerpro->sendCommand(_T("(full-reset)"));
	m_racerpro->sendCommand(_T("(restore-standard-settings)"));
	m_racerpro->sendCommand(_T("(process-set-at-a-time)"));
	m_racerpro->sendCommand(_T("(add-rule-consequences-automatically)"));
	
	/////////MyMessageBox_Error(_T("submitBlankInformation [No]"));
}

void DLInterface::submitInformation()
{
	m_racerpro->sendCommand(_T("(full-reset)"));
	m_racerpro->sendCommand(_T("(restore-standard-settings)"));
	m_racerpro->sendCommand(_T("(process-set-at-a-time)"));
	m_racerpro->sendCommand(_T("(add-rule-consequences-automatically)"));
	//racerpro->sendCommand(_T("(use-injective-variables-by-default)"));
	
	for (int i = 0; i < m_nstrTBox.size(); i ++)
	{
		m_racerpro->sendCommand(m_nstrTBox[i]);
		Output_RacerProCache("TBox-" + itos(i), m_nstrTBox[i]);
		cout << "submit TBox Sentence-" + itos(i) + "." << endl;
	}
	cout << "submit TBox ok." << endl << endl;

	for (i = 0; i < m_nstrABox.size(); i ++)
	{
		m_racerpro->sendCommand(m_nstrABox[i]);
		Output_RacerProCache("ABox-" + itos(i), m_nstrABox[i]);
		cout << "submit ABox Sentence-" + itos(i) + "." << endl;
	}
	cout << "submit ABox ok." << endl << endl;

	for (i = 0; i < m_nstrSWRLR.size(); i ++)
	{
		m_racerpro->sendCommand(m_nstrSWRLR[i]);
		Output_RacerProCache("SWRLR-" + itos(i), m_nstrSWRLR[i]);
		cout << "submit SWRLR Rule-" + itos(i) + "." << endl;
	}
	cout << "submit SWRLR ok." << endl << endl;

	cout << "submitInformation ok." << endl << endl;
}

void DLInterface::prepareOntologyString()
{
	m_nstrOntology.clear();

	for (int i = 0; i < m_nstrReset.size(); i ++)
	{
		//m_racerpro->sendCommand(m_strInit[i]);
		m_nstrOntology.push_back(m_nstrReset[i]);
		Output_RacerProCache("Init-" + itos(i), m_nstrReset[i]);
		cout << "prepare Init Sentence-" + itos(i) + "." << endl;
	}
	cout << "prepare Init ok." << endl << endl;

	for (i = 0; i < m_nstrTBox.size(); i ++)
	{
		//m_racerpro->sendCommand(m_strTBox[i]);
		m_nstrOntology.push_back(m_nstrTBox[i]);
		Output_RacerProCache("TBox-" + itos(i), m_nstrTBox[i]);
		cout << "prepare TBox Sentence-" + itos(i) + "." << endl;
	}
	cout << "prepare TBox ok." << endl << endl;
	
	for (i = 0; i < m_nstrABox.size(); i ++)
	{
		//m_racerpro->sendCommand(m_strABox[i]);
		m_nstrOntology.push_back(m_nstrABox[i]);
		Output_RacerProCache("ABox-" + itos(i), m_nstrABox[i]);
		cout << "prepare ABox Sentence-" + itos(i) + "." << endl;
	}
	cout << "prepare ABox ok." << endl << endl;
	
	for (i = 0; i < m_nstrSWRLR.size(); i ++)
	{
		//m_racerpro->sendCommand(m_strSWRLR[i]);
		m_nstrOntology.push_back(m_nstrSWRLR[i]);
		Output_RacerProCache("SWRLR-" + itos(i), m_nstrSWRLR[i]);
		cout << "prepare SWRLR Rule-" + itos(i) + "." << endl;
	}
	cout << "prepare SWRLR ok." << endl << endl;
	
	cout << "prepareOntologyString ok." << endl << endl;
}

void DLInterface::submitOntologyAndQueryString()
{
	generateQueryFile();
	clearQueryString();
	deleteOutputFile();
	executeRacerForFile();
	parseOutputFileToAnswerString();
}

int DLInterface::toBoolean(CString strResult)
{
	if (strResult == _T("t") || strResult == _T("T"))
	{
		return 1;
	}
	else if (strResult == _T("NIL") || strResult == _T("nil"))
	{
		return 0;
	}
	else
	{
		MyMessageBox_Error(_T("toBoolean Error"), _T("Error"));
		return -1;
	}
}

int DLInterface::isNil(CString strResult)
{
	if (strResult == _T("NIL") || strResult == _T("nil"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

CString DLInterface::x(int i)
{
	return "?x" + itos(i);
}

CString DLInterface::parseX(CString strResult, int i)
{
	CString strX = "?x" + itos(i);
	int iStart = strResult.Find(strX);
	if (iStart == -1)
		return "";
	else
	{
		iStart = strResult.Find(strX, iStart);
		iStart += strX.GetLength();
		while (strResult[iStart] == ' ' || strResult[iStart] == '\t' || strResult[iStart] == '\n')
		{
			iStart ++;
		}
		int iEnd = strResult.Find(")", iStart);
		return strResult.Mid(iStart, iEnd - iStart);
	}
}

void DLInterface::addGroup(CString strGroup)
{
	insertInstanceAssertion(strGroup, _T("Subject"));
}

void DLInterface::addUser(CString strUser)
{
	insertInstanceAssertion(strUser, _T("Subject"));
}

void DLInterface::addShareFolder(CString strShareFolder)
{
	insertInstanceAssertion(strShareFolder, _T("Resource"));
}

void DLInterface::addChildRelation(CString strParent, CString strChild)
{
	insertRelationAssertion(strParent, strChild, _T("has_subSubject"));
	insertRelationAssertion(strParent, strChild, _T("has_subSubjectOrigin"));
}

void DLInterface::addAccessRelation(CString strADACE, CString strGroupOrUser, CString strShareFolder, CString strAction, 
									int iPermit)
{
	//insertInstanceAssertion(strADACE, _T("Policy"));
	insertRelationAssertion(strADACE, strGroupOrUser, _T("has_Subject"));
	insertRelationAssertion(strADACE, strShareFolder, _T("has_Resource"));
	insertRelationAssertion(strADACE, strAction, _T("has_Action"));
	if (iPermit == 1)
		insertRelationAssertion(strADACE, _T("Permit"), _T("has_Permission"));
	else if (iPermit == 0)
		insertRelationAssertion(strADACE, _T("Deny"), _T("has_Permission"));
	else
		MyMessageBox_Error(_T("addAccessRelation"));
}

CString DLInterface::dnName2dlName(CString strDNName)
{
	strDNName.Replace(_T(","), _T("_"));
	strDNName.Replace(_T(" "), _T("+"));
	return strDNName;
}

CString DLInterface::dlName2dnName(CString strDLName)
{
	strDLName.Replace(_T("_"), _T(","));
	strDLName.Replace(_T("+"), _T(" "));
	return strDLName;
}

void DLInterface::addDomain(ADDomain *domain, int iMixOrShareOrNTFS)
{
	//domain->getAllACEs();
	domain->connectFolderWithACEs();

	vector<ADGroup*> groups = domain->g_groups;
	for (int i = 0; i < groups.size(); i ++)
	{
		addGroup(groups[i]->dn);
		for (int j = 0; j < groups[i]->childGroups.size(); j ++)
		{
			addChildRelation(groups[i]->dn, groups[i]->childGroups[j]->dn);
		}
		for (j = 0; j < groups[i]->childUsers.size(); j ++)
		{
			addChildRelation(groups[i]->dn, groups[i]->childUsers[j]->dn);
		}
	}

	vector<ADUser*> users = domain->g_users;
	for (i = 0; i < users.size(); i ++)
	{
		addUser(users[i]->dn);
	}

	vector<ADShareFolder*> shareFolders = domain->g_shareFolders;
	for (i = 0; i < shareFolders.size(); i ++)
	{
		addShareFolder(shareFolders[i]->dn);
		vector<ADACE*> finalPermissions;
		if (iMixOrShareOrNTFS == 2)
		{
			finalPermissions = shareFolders[i]->sharePermissions;
			finalPermissions.insert(finalPermissions.end(), shareFolders[i]->ntfsPermissions.begin(), shareFolders[i]->ntfsPermissions.end());
		}
		else if (iMixOrShareOrNTFS == 1)
		{
			finalPermissions = shareFolders[i]->sharePermissions;
		}
		else if (iMixOrShareOrNTFS == 0)
		{
			finalPermissions = shareFolders[i]->ntfsPermissions;
		}
		else
		{
			MyMessageBox_Error(_T("addDomain"));
		}
		for (int j = 0; j < finalPermissions.size(); j ++)
		{
			CString strObjectDNName;
			if (finalPermissions[j]->user != NULL)
			{
				strObjectDNName = finalPermissions[j]->user->dn;
			}
			else if (finalPermissions[j]->group != NULL)
			{
				strObjectDNName = finalPermissions[j]->group->dn;
			}
			else
			{
				MyMessageBox_Error(_T("addDomain"));
				return;
			}

			insertInstanceAssertion(finalPermissions[j]->dn, _T("Policy"));
			if (finalPermissions[j]->r == 1)
				addAccessRelation(finalPermissions[j]->dn, strObjectDNName, shareFolders[i]->dn, _T("R"),
				finalPermissions[j]->allowed);
			if (finalPermissions[j]->x == 1)
				addAccessRelation(finalPermissions[j]->dn, strObjectDNName, shareFolders[i]->dn, _T("X"),
				finalPermissions[j]->allowed);
			if (finalPermissions[j]->w == 1)
				addAccessRelation(finalPermissions[j]->dn, strObjectDNName, shareFolders[i]->dn, _T("W"),
				finalPermissions[j]->allowed);
		}
	}
}

int DLInterface::queryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict");
	else
		MyMessageBox_Error(_T("queryAccessRelationConflict"));

	return queryRelationAssertion(strADACE1, strADACE2, strRelation);
}

int DLInterface::queryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict2");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict2");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict2");
	else
		MyMessageBox_Error(_T("queryAccessRelationConflict2"));
	
	return queryRelationAssertion(strADACE1, strADACE2, strRelation);
}

int DLInterface::queryRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation)
{

	CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
	//(retrieve nil (?po1 ?po2 has_Policy_Conflict))
	CString strRecv = m_racerpro->sendCommand(strSend);
	return toBoolean(strRecv);
}

vector<CString> DLInterface::queryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation)
{
	int i, j;
	for (i = 1; i < 5; i ++)
	{
		CString strQueryHead = "(";
		for (j = 0; j < i - 1; j ++)
		{
			strQueryHead += (x(j) + " ");
		}
		strQueryHead += (x(j) + ")");
		
		CString strQueryBody = "(and ";
		for (j = 0; j < i + 1; j ++)
		{
			if (j == 0)
			{
				strQueryBody += ("(" + dnName2dlName(strInstance1) + " " + x(j) + " " + strRelation + ") ");
			}
			else if (j != i)
			{
				strQueryBody += ("(" + x(j - 1) + " " + x(j) + " " + strRelation + ") ");
			}
			else
			{
				strQueryBody += ("(" + x(j - 1) + " " + dnName2dlName(strInstance2) + " " + strRelation + "))");
			}
		}
		CString strSend = "(retrieve " + strQueryHead + " " + strQueryBody + ")";
		
		//CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
		//(retrieve (?x1 ?x2 ?x3) (and (strInstance1 ?x1 has_Policy_Conflict) (?x1 ?x2 has_Policy_Conflict) (?x2 strInstance2 has_Policy_Conflict)))
		CString strRecv = m_racerpro->sendCommand(strSend);
		if (isNil(strRecv) == 0)
		{
			vector<CString> nstrLink;
			for (j = 0; j < i; j ++)
			{
				nstrLink.push_back(parseX(strRecv, j));
			}
			return nstrLink;
		}
	}
	vector<CString> nstrLink;
	return nstrLink;
}

CString DLInterface::queryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation)
{
	CString strSend = "(retrieve (?x0) (and (" + dnName2dlName(strInstance1) + " ?x0 " + strRelation + ") (" + dnName2dlName(strInstance2) + " ?x0 " + strRelation + ")))";
	//(retrieve (?x0) (and (strInstance1 ?x0 strRelation) (strInstance2 ?x0 strRelation)))
	CString strRecv = m_racerpro->sendCommand(strSend);
	if (isNil(strRecv) == 0)
	{
		CString strCommonInstance = parseX(strRecv, 0);
		return dlName2dnName(strCommonInstance);
	}
	else
	{
		MyMessageBox_Error(_T("queryCommonInstanceForTwo"));
		return "queryCommonInstanceForTwo Error.";
	}
}

void DLInterface::prepareQueryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict");
	else
		MyMessageBox_Error(_T("prepareQueryAccessRelationConflict"));
	
	prepareQueryRelationAssertion(strADACE1, strADACE2, strRelation);
}

void DLInterface::prepareQueryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict2");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict2");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict2");
	else
		MyMessageBox_Error(_T("prepareQueryAccessRelationConflict2"));
	
	prepareQueryRelationAssertion(strADACE1, strADACE2, strRelation);
}

void DLInterface::prepareQueryRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation)
{
	
	CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
	//(retrieve nil (?po1 ?po2 has_Policy_Conflict))
	m_nstrQuerys.push_back(strSend);
	return;
}

void DLInterface::prepareQueryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation)
{
	int i, j;
	for (i = 1; i < 5; i ++)
	{
		CString strQueryHead = "(";
		for (j = 0; j < i - 1; j ++)
		{
			strQueryHead += (x(j) + " ");
		}
		strQueryHead += (x(j) + ")");
		
		CString strQueryBody = "(and ";
		for (j = 0; j < i + 1; j ++)
		{
			if (j == 0)
			{
				strQueryBody += ("(" + dnName2dlName(strInstance1) + " " + x(j) + " " + strRelation + ") ");
			}
			else if (j != i)
			{
				strQueryBody += ("(" + x(j - 1) + " " + x(j) + " " + strRelation + ") ");
			}
			else
			{
				strQueryBody += ("(" + x(j - 1) + " " + dnName2dlName(strInstance2) + " " + strRelation + "))");
			}
		}
		CString strSend = "(retrieve " + strQueryHead + " " + strQueryBody + ")";
		
		//CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
		//(retrieve (?x1 ?x2 ?x3) (and (strInstance1 ?x1 has_Policy_Conflict) (?x1 ?x2 has_Policy_Conflict) (?x2 strInstance2 has_Policy_Conflict)))
		//CString strRecv = m_racerpro->sendCommand(strSend);
		m_nstrQuerys.push_back(strSend);
// 		if (isNil(strRecv) == 0)
// 		{
// 			vector<CString> nstrLink;
// 			for (j = 0; j < i; j ++)
// 			{
// 				nstrLink.push_back(parseX(strRecv, j));
// 			}
// 			return nstrLink;
// 		}
	}
// 	vector<CString> nstrLink;
// 	return nstrLink;
}

void DLInterface::prepareQueryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation)
{
	CString strSend = "(retrieve (?x0) (and (" + dnName2dlName(strInstance1) + " ?x0 " + strRelation + ") (" + dnName2dlName(strInstance2) + " ?x0 " + strRelation + ")))";
	//(retrieve (?x0) (and (strInstance1 ?x0 strRelation) (strInstance2 ?x0 strRelation)))
	//CString strRecv = m_racerpro->sendCommand(strSend);
	m_nstrQuerys.push_back(strSend);
// 	if (isNil(strRecv) == 0)
// 	{
// 		CString strCommonInstance = parseX(strRecv, 0);
// 		return dlName2dnName(strCommonInstance);
// 	}
// 	else
// 	{
// 		MyMessageBox_Error(_T("prepareQueryCommonInstanceForTwo"));
// 		return "queryCommonInstanceForTwo Error.";
// 	}
}

int DLInterface::getQueryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict");
	else
		MyMessageBox_Error(_T("getQueryAccessRelationConflict"));
	
	return getQueryRelationAssertion_Conflict(strADACE1, strADACE2, strRelation, m_iCurConflict ++);
}

int DLInterface::getQueryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType)
{
	CString strRelation;
	if (strActionType == _T("R"))
		strRelation = _T("has_Policy_R_Conflict2");
	else if (strActionType == _T("X"))
		strRelation = _T("has_Policy_X_Conflict2");
	else if (strActionType == _T("W"))
		strRelation = _T("has_Policy_W_Conflict2");
	else
		MyMessageBox_Error(_T("getQueryAccessRelationConflict2"));
	
	return getQueryRelationAssertion_Conflict(strADACE1, strADACE2, strRelation, m_iCurConflict ++);
}

int DLInterface::getQueryRelationAssertion_Conflict(CString strInstance1, CString strInstance2, CString strRelation, int iIndex)
{

	//CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
	//(retrieve nil (?po1 ?po2 has_Policy_Conflict))
	//CString strRecv = m_racerpro->sendCommand(strSend);
	if (iIndex < 0 || iIndex >= m_nstrConflicts.size())
	{
		MyMessageBox_Error(_T("getQueryRelationAssertion_Conflict"));
		return FALSE;
	}
	CString strRecv = m_nstrConflicts[iIndex];
	return toBoolean(strRecv);
}

int DLInterface::getQueryRelationAssertion_Reason(CString strInstance1, CString strInstance2, CString strRelation)
{
	
	//CString strSend = _T("(retrieve nil (") + dnName2dlName(strInstance1) + _T(" ") + dnName2dlName(strInstance2) + _T(" ") + strRelation + _T("))");
	//(retrieve nil (?po1 ?po2 has_Policy_Conflict))
	//CString strRecv = m_racerpro->sendCommand(strSend);
	if (m_iCurReason < 0 || m_iCurReason >= m_nstrReasons.size())
	{
		MyMessageBox_Error(_T("getQueryRelationAssertion_Reason"));
		return FALSE;
	}
	CString strRecv = m_nstrReasons[m_iCurReason ++];
	return toBoolean(strRecv);
}

vector<CString> DLInterface::getQueryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation)
{
	int iCurRelationAssertion = m_iCurReason;
	m_iCurReason += 4;

	int i, j;
	for (i = iCurRelationAssertion; i < iCurRelationAssertion + 4; i ++)
	{
		//(retrieve (?x1 ?x2 ?x3) (and (strInstance1 ?x1 has_Policy_Conflict) (?x1 ?x2 has_Policy_Conflict) (?x2 strInstance2 has_Policy_Conflict)))
		//CString strRecv = m_racerpro->sendCommand(strSend);
		if (i < 0 || i >= m_nstrReasons.size())
		{
			MyMessageBox_Error(_T("getQueryRelationAssertionLink"));
			return FALSE;
		}
		CString strRecv = m_nstrReasons[i];
		if (isNil(strRecv) == 0)
		{
			vector<CString> nstrLink;
			for (j = 0; j < i - iCurRelationAssertion + 1; j ++)
			{
				nstrLink.push_back(parseX(strRecv, j));
			}
			return nstrLink;
		}
	}
	vector<CString> nstrLink;
	return nstrLink;
}

CString DLInterface::getQueryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation)
{
	//(retrieve (?x0) (and (strInstance1 ?x0 strRelation) (strInstance2 ?x0 strRelation)))
	//CString strRecv = m_racerpro->sendCommand(strSend);
	if (m_iCurCommonInstance < 0 || m_iCurCommonInstance >= m_nstrCommonInstances.size())
	{
		MyMessageBox_Error(_T("getQueryCommonInstanceForTwo"));
		return _T("getQueryCommonInstanceForTwo Error");
	}
	CString strRecv = m_nstrCommonInstances[m_iCurCommonInstance ++];
	if (isNil(strRecv) == 0)
	{
		CString strCommonInstance = parseX(strRecv, 0);
		return dlName2dnName(strCommonInstance);
	}
	else
	{
		MyMessageBox_Error(_T("getQueryCommonInstanceForTwo"));
		return "getQueryCommonInstanceForTwo Error.";
	}
}

CString DLInterface::ACE2Action(ADACE *aDACE)
{
	CString strAction;
	strAction += "[" + ACE2Type(aDACE) + "::";
	if (aDACE->r == 1)
	{
		strAction += "R";
	}
	if (aDACE->w == 1)
	{
		strAction += "W";
	}
	if (aDACE->x == 1)
	{
		strAction += "X";
	}
	strAction += "]";
	return strAction;
}

CString DLInterface::ACE2Type(ADACE *aDACE)
{
	CString strType;
	if (aDACE->type == TYPE_NTFS)
	{
		strType += "NTFS";
	}
	else if (aDACE->type == TYPE_SHARE)
	{
		strType += "Share";
	}
	else
	{
		MyMessageBox_Error(_T("ACE2Type"));
		return _T("null");
	}
	return strType;
}

CString DLInterface::ACE2Permission(ADACE *aDACE)
{
	if (aDACE->allowed == 1)
	{
		return "";
	}
	else if (aDACE->allowed == 0)
	{
		if (bChineseOrEnglish)
		{
			return "不";
		}
		else
		{
			return " do not";
		}
	}
	else
	{
		MyMessageBox_Error(_T("ACE2Permission"));
		return "ACE2Permission Error.";
	}
}

CString DLInterface::user2Description(ADUser *user)
{
	if (bChineseOrEnglish)
	{
		return "[用户::" + user->cn + "]";
	}
	else
	{
		return "[USER::" + user->cn + "]";
	}
}

CString DLInterface::group2Description(ADGroup *group)
{
	if (bChineseOrEnglish)
	{
		return "[组::" + group->cn + "]";
	}
	else
	{
		return "[GROUP::" + group->cn + "]";
	}
}

CString DLInterface::shareFolder2Description(ADShareFolder *shareFolder)
{
	if (bChineseOrEnglish)
	{
		return "[文件夹::" + shareFolder->cn + "]";
	}
	else
	{
		return "[RESOURCE::" + shareFolder->cn + "]";
	}
}

CString DLInterface::getUserOrGroupDNNameFromACE(ADACE *ace)
{
	if (ace->user != NULL)
	{
		return ace->user->dn;
	}
	else if (ace->group != NULL)
	{
		return ace->group->dn;
	}
	else
	{
		MyMessageBox_Error(_T("getUserOrGroupDNNameFromACE"));
		return "getUserOrGroupDNNameFromACE Error.";
	}
}

CString DLInterface::getUserOrGroupCNNameFromACE(ADACE *ace)
{
	if (ace->user != NULL)
	{
		return ace->user->cn;
	}
	else if (ace->group != NULL)
	{
		return ace->group->cn;
	}
	else
	{
		MyMessageBox_Error(_T("getUserOrGroupCNNameFromACE"));
		return "getUserOrGroupCNNameFromACE Error.";
	}
}

CString DLInterface::getUserOrGroupDescriptionFromACE(ADACE *ace)
{
	if (ace->user != NULL)
	{
		return user2Description(ace->user);
	}
	else if (ace->group != NULL)
	{
		return group2Description(ace->group);
	}
	else
	{
		MyMessageBox_Error(_T("getUserOrGroupDescriptionFromACE"));
		return "getUserOrGroupDescriptionFromACE Error.";
	}
}

CString DLInterface::getShareFolderDescriptionFromACE(ADACE *ace)
{
	if (ace->fatherFolder != NULL)
	{
		return shareFolder2Description(ace->fatherFolder);
	}
	else
	{
		MyMessageBox_Error(_T("getShareFolderDescriptionFromACE"));
		return "getShareFolderDescriptionFromACE Error.";
	}
}

CString DLInterface::dn2cn(CString strDN)
{
	int iStart, iEnd;
	iStart = strDN.Find("CN=", 0);
	if (iStart == -1)
	{
		MyMessageBox_Error(_T("dn2cn"));
		return "dn2cn Error";
	}
	else
	{
		iEnd = strDN.Find(",", 0);
		if (iEnd == -1)
		{
			MyMessageBox_Error(_T("dn2cn"));
			return "dn2cn Error";
		}
		else
		{
			iStart += 3;
			if (iStart >= iEnd)
			{
				MyMessageBox_Error(_T("dn2cn"));
				return "dn2cn Error";
			}
			else
			{
				CString strCN = strDN.Mid(iStart, iEnd - iStart);
				return strCN;
			}
		}
	}
}

BOOL DLInterface::checkGroupOrUserByDN(CString strDN)
{
	int i, j;
	for (i = 0; i < m_npDomains.size(); i ++)
	{
		ADDomain *pDomain = m_npDomains[i];
		for (j = 0; j < pDomain->g_groups.size(); j ++)
		{
			if (pDomain->g_groups[j]->dn == strDN)
			{
				return TRUE;
			}
		}
		for (j = 0; j < pDomain->g_users.size(); j ++)
		{
			if (pDomain->g_users[j]->dn == strDN)
			{
				return FALSE;
			}
		}
	}

	MyMessageBox_Error(_T("checkGroupOrUserByDN"));
	return FALSE;
}

CString DLInterface::dl2Description(CString strDL)
{
	BOOL bGroup = checkGroupOrUserByDN(dlName2dnName(strDL));
	if (bChineseOrEnglish)
	{
		if (bGroup)
		{
			return "[组::" + dn2cn(dlName2dnName(strDL)) + "]";
		}
		else
		{
			return "[用户::" + dn2cn(dlName2dnName(strDL)) + "]";
		}
	}
	else
	{
		if (bGroup)
		{
			return "[GROUP::" + dn2cn(dlName2dnName(strDL)) + "]";
		}
		else
		{
			return "[USER::" + dn2cn(dlName2dnName(strDL)) + "]";
		}
	}
}

CString DLInterface::getConflictReasonFromIdentical(CString strNode)
{
	CString strContent;
	if (bChineseOrEnglish)
	{
		strContent = "冲突原因：两条策略具有同样的主体" + strNode + "。";
	}
	else
	{
		strContent = "Conflict Reason: Both policies share the same subject: " + strNode + ".";
	}
	return strContent;
}

CString DLInterface::getConflictReasonFromStrings(vector<CString> nstrNodes)
{
	if (nstrNodes.size() <= 1)
	{
		MyMessageBox_Error(_T("getConflictReasonFromStrings"));
		return "getConflictReasonFromStrings Error";
	}
	CString strContent;
	if (bChineseOrEnglish)
	{
		strContent = "冲突原因：";
		for (int i = nstrNodes.size() - 1; i > 0; i --)
		{
			if (i != 1)
			{
				strContent += (nstrNodes[i] + "继承" + nstrNodes[i - 1] + "，");
			}
			else
			{
				strContent += (nstrNodes[i] + "继承" + nstrNodes[i - 1] + "。");
			}
		}
	}
	else
	{
		strContent = "Conflict Reason: ";
		for (int i = nstrNodes.size() - 1; i > 0; i --)
		{
			if (i != 1)
			{
				strContent += (nstrNodes[i] + " inherits " + nstrNodes[i - 1] + ", ");
			}
			else
			{
				strContent += (nstrNodes[i] + " inherits " + nstrNodes[i - 1] + ".");
			}
		}
	}
	return strContent;
}

void DLInterface::queryOneKindOfConflict(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo, int iMixOrShareOrNTFS)
{
	vector<ADACE*> resultACEs;
	resultACEs = domain->getAllACEs(iMixOrShareOrNTFS);
	for (int i = 0; i < resultACEs.size(); i ++)
	{
		for (int j = i + 1; j < resultACEs.size(); j ++)
		{
			if (queryAccessRelationConflict(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}
			
			CString strObjectI = getUserOrGroupDescriptionFromACE(resultACEs[i]);
			CString strObjectJ = getUserOrGroupDescriptionFromACE(resultACEs[j]);
			if (resultACEs[i]->fatherFolder->cn != resultACEs[j]->fatherFolder->cn)
			{
				MyMessageBox_Error(_T("queryOneKindOfConflict"));
				return;
			}
			CString strFile = getShareFolderDescriptionFromACE(resultACEs[i]);
			
			CString strContent;
			if (bChineseOrEnglish)
			{
				strContent += (strObjectI + "与" + strObjectJ + "在操作" + strFile + "发生" + strActionWord + "继承冲突：");
				strContent += (strObjectI + "对" + strFile + ACE2Permission(resultACEs[i]) + "具有" + ACE2Action(resultACEs[i]) + "权限，而");
				strContent += (strObjectJ + "对" + strFile + ACE2Permission(resultACEs[j]) + "具有" + ACE2Action(resultACEs[j]) + "权限。");
				iConflictNo ++;
				Output_ReportList(itos(iConflictNo), strContent);
				CString strTemp = _T("第");
				strTemp += (itos(iConflictNo) + _T("条策略冲突："));
				addToReport(strTemp);
				addToReport(strContent);
			}
			else
			{
				strContent += (strObjectI + " and " + strObjectJ + " have " + strActionWord + " Inheritance Conflict for access to " + strFile + ":");
				strContent += (strObjectI + ACE2Permission(resultACEs[i]) + " have the " + ACE2Action(resultACEs[i]) + " permission, while ");
				strContent += (strObjectJ + ACE2Permission(resultACEs[j]) + " have the " + ACE2Action(resultACEs[j]) + " permission.");
				iConflictNo ++;
				Output_ReportList(itos(iConflictNo), strContent);
				CString strTemp = _T("No. ");
				strTemp += (itos(iConflictNo) + _T(" Policy Conflict: "));
				addToReport(strTemp);
				addToReport(strContent);
			}

			///////////////////////////////////////冲突原因查询显示///////////////////////////////////////
			if (getUserOrGroupDNNameFromACE(resultACEs[i]) == getUserOrGroupDNNameFromACE(resultACEs[j]))
			{
				strContent = getConflictReasonFromIdentical(getUserOrGroupDNNameFromACE(resultACEs[i]));
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (queryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[i]),
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject") == 1)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (queryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[j]),
				getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject") == 1)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else
			{
				vector<CString> nstrLink, nstrLink2;
				nstrLink = queryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
					getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
				nstrLink2 = queryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
					getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject");
				if (nstrLink.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					for (int k = 0; k < nstrLink.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink[k]));
					}
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
					addToReport(strContent);
				}
				else if (nstrLink2.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					for (int k = 0; k < nstrLink2.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink2[k]));
					}
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
					addToReport(strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict"));
					return;
				}
			}

			addToReport(_T(""));
		}
	}
}

void DLInterface::queryOneKindOfConflict2(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo, int iMixOrShareOrNTFS)
{
	vector<ADACE*> resultACEs;
	resultACEs = domain->getAllACEs(iMixOrShareOrNTFS);
	for (int i = 0; i < resultACEs.size(); i ++)
	{
		for (int j = i + 1; j < resultACEs.size(); j ++)
		{
			if (queryAccessRelationConflict2(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}
			
			CString strObjectI = getUserOrGroupDescriptionFromACE(resultACEs[i]);
			CString strObjectJ = getUserOrGroupDescriptionFromACE(resultACEs[j]);
			if (resultACEs[i]->fatherFolder->cn != resultACEs[j]->fatherFolder->cn)
			{
				MyMessageBox_Error(_T("queryOneKindOfConflict2"));
				return;
			}
			CString strFile = getShareFolderDescriptionFromACE(resultACEs[i]);
			
			CString strContent;
			if (bChineseOrEnglish)
			{
				strContent += (strObjectI + "与" + strObjectJ + "在操作" + strFile + "发生" + strActionWord + "职责分离冲突：");
				strContent += (strObjectI + "对" + strFile + ACE2Permission(resultACEs[i]) + "具有" + ACE2Action(resultACEs[i]) + "权限，而");
				strContent += (strObjectJ + "对" + strFile + ACE2Permission(resultACEs[j]) + "具有" + ACE2Action(resultACEs[j]) + "权限。");
				
				iConflictNo ++;
				Output_ReportList(itos(iConflictNo), strContent);
				CString strTemp = _T("第");
				strTemp += (itos(iConflictNo) + _T("条策略冲突："));
				addToReport(strTemp);
				addToReport(strContent);
			}
			else
			{
				strContent += (strObjectI + " and " + strObjectJ + " have " + strActionWord + " Separation of Duty Conflict for access to " + strFile + ":");
				strContent += (strObjectI + ACE2Permission(resultACEs[i]) + " have the " + ACE2Action(resultACEs[i]) + " permission, while ");
				strContent += (strObjectJ + ACE2Permission(resultACEs[j]) + " have the " + ACE2Action(resultACEs[j]) + " permission.");
				
				iConflictNo ++;
				Output_ReportList(itos(iConflictNo), strContent);
				CString strTemp = _T("No. ");
				strTemp += (itos(iConflictNo) + _T(" Policy Conflict: "));
				addToReport(strTemp);
				addToReport(strContent);
			}
			
			
			CString strCommonChildDN = queryCommonInstanceForTwo(getUserOrGroupDNNameFromACE(resultACEs[i]), 
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			///////////////////////////////////////第一个冲突原因查询显示///////////////////////////////////////
			if (queryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[i]),
				strCommonChildDN, "has_subSubjectOrigin") == 1)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				nstrNodes.push_back(dl2Description(strCommonChildDN));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else
			{
				vector<CString> nstrLink = queryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
					strCommonChildDN, "has_subSubjectOrigin");
				if (nstrLink.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					for (int k = 0; k < nstrLink.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink[k]));
					}
					nstrNodes.push_back(dl2Description(strCommonChildDN));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
					addToReport(strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict2"));
					return;
				}
			}
			///////////////////////////////////////第二个冲突原因查询显示///////////////////////////////////////
			if (queryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[j]),
				strCommonChildDN, "has_subSubjectOrigin") == 1)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				nstrNodes.push_back(dl2Description(strCommonChildDN));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else
			{
				vector<CString> nstrLink = queryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
					strCommonChildDN, "has_subSubjectOrigin");
				if (nstrLink.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					for (int k = 0; k < nstrLink.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink[k]));
					}
					nstrNodes.push_back(dl2Description(strCommonChildDN));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
					addToReport(strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict2"));
					return;
				}
			}

			addToReport(_T(""));
		}
	}
}

void DLInterface::queryAllConflicts(vector<ADDomain*> npDomains, int &iConflictNo)
{
	//int iConflictNo = 0;
	if (bChineseOrEnglish)
	{
		for (int i = 0; i < npDomains.size(); i ++)
		{
			iConflictNo = 0;
			if (m_iMixOrShareOrNTFS == 2)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[Mix-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[Mix-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[Mix-写]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[Mix-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[Mix-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[Mix-写]", iConflictNo, m_iMixOrShareOrNTFS);
				
				CString strContent;
				strContent += ("域" + npDomains[i]->dNSName + "一共检测出" + itos(iConflictNo) + "处混合策略冲突！");
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (m_iMixOrShareOrNTFS == 1)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[Share-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[Share-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[Share-写]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[Share-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[Share-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[Share-写]", iConflictNo, m_iMixOrShareOrNTFS);

				CString strContent;
				strContent += ("域" + npDomains[i]->dNSName + "一共检测出" + itos(iConflictNo) + "处Share策略冲突！");
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (m_iMixOrShareOrNTFS == 0)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[NTFS-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[NTFS-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[NTFS-写]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[NTFS-读]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[NTFS-执行]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[NTFS-写]", iConflictNo, m_iMixOrShareOrNTFS);

				CString strContent;
				strContent += ("域" + npDomains[i]->dNSName + "一共检测出" + itos(iConflictNo) + "处NTFS策略冲突！");
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else
			{
				MyMessageBox_Error(_T("queryAllConflicts"));
			}
			/*
			else
			{
				//queryOneKindOfConflict3(domain, "R", "[Share-NTFS间-读]", iConflictNo);
				//queryOneKindOfConflict3(domain, "X", "[Share-NTFS间-执行]", iConflictNo);
				//queryOneKindOfConflict3(domain, "W", "[Share-NTFS间-写]", iConflictNo);

				CString strContent;
				strContent += ("一共检测出" + itos(iConflictNo) + "处Share-NTFS间策略冲突");
				Output_ReportList(itos(iConflictNo ++), strContent);
			}
			*/
		}
	}
	else
	{
		for (int i = 0; i < npDomains.size(); i ++)
		{
			iConflictNo = 0;
			if (m_iMixOrShareOrNTFS == 2)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[Mix-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[Mix-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[Mix-Write]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[Mix-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[Mix-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[Mix-Write]", iConflictNo, m_iMixOrShareOrNTFS);
				
				CString strContent;
				strContent += ("VisualADS has detected " + itos(iConflictNo) + " Mix Policy Conflicts in domain: " + npDomains[i]->dNSName);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (m_iMixOrShareOrNTFS == 1)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[Share-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[Share-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[Share-Write]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[Share-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[Share-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[Share-Write]", iConflictNo, m_iMixOrShareOrNTFS);

				CString strContent;
				strContent += ("VisualADS has detected " + itos(iConflictNo) + " Share Policy Conflicts in domain: " + npDomains[i]->dNSName);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else if (m_iMixOrShareOrNTFS == 0)
			{
				queryOneKindOfConflict(npDomains[i], "R", "[NTFS-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "X", "[NTFS-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict(npDomains[i], "W", "[NTFS-Write]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "R", "[NTFS-Read]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "X", "[NTFS-Execute]", iConflictNo, m_iMixOrShareOrNTFS);
				queryOneKindOfConflict2(npDomains[i], "W", "[NTFS-Write]", iConflictNo, m_iMixOrShareOrNTFS);

				CString strContent;
				strContent += ("VisualADS has detected " + itos(iConflictNo) + " NTFS Policy Conflicts in domain: " + npDomains[i]->dNSName);
				Output_ReportList(itos(iConflictNo), strContent);
				addToReport(strContent);
			}
			else
			{
				MyMessageBox_Error(_T("queryAllConflicts"));
			}
		/*
		else
		{
			//queryOneKindOfConflict3(domain, "R", "[Share-NTFS间-读]", iConflictNo);
			//queryOneKindOfConflict3(domain, "X", "[Share-NTFS间-执行]", iConflictNo);
			//queryOneKindOfConflict3(domain, "W", "[Share-NTFS间-写]", iConflictNo);

			CString strContent;
			strContent += ("一共检测出" + itos(iConflictNo) + "处Share-NTFS间策略冲突");
			Output_ReportList(itos(iConflictNo ++), strContent);
		}
		*/
		}
	}

	writeReportFile();
}

void DLInterface::queryOneKindOfConflict_FileInterface(ADDomain *domain, CString strActionType, CString strActionWord,
													   int &iConflictNo, int iMixOrShareOrNTFS)
{
	int i, j;
	m_iCurConflict = 0;
	m_iCurReason = 0;
	m_iCurCommonInstance = 0;
	vector<ADACE*> resultACEs;
	resultACEs = domain->getAllACEs(iMixOrShareOrNTFS);

	clearQueryString();
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (int j = i + 1; j < resultACEs.size(); j ++)
		{
			prepareQueryAccessRelationConflict(resultACEs[i]->dn, resultACEs[j]->dn, strActionType);
		}
	}
	submitOntologyAndQueryString();
	answerString2ConflictString();

	clearQueryString();
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			if (getQueryAccessRelationConflict(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}

			prepareQueryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[i]),
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			prepareQueryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[j]),
				getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject");
			prepareQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			prepareQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
					getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject");
		}
	}
	submitOntologyAndQueryString();
	answerString2ReasonString();

	clearQueryString();
	m_iCurConflict = 0;
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			if (getQueryAccessRelationConflict(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}
			
			CString strObjectI = getUserOrGroupDescriptionFromACE(resultACEs[i]);
			CString strObjectJ = getUserOrGroupDescriptionFromACE(resultACEs[j]);
			if (resultACEs[i]->fatherFolder->cn != resultACEs[j]->fatherFolder->cn)
			{
				MyMessageBox_Error(_T("queryOneKindOfConflict_FileInterface"));
				return;
			}
			CString strFile = getShareFolderDescriptionFromACE(resultACEs[i]);
			
			CString strContent;
			strContent += (strObjectI + "与" + strObjectJ + "在操作" + strFile + "发生" + strActionWord + "继承冲突：");
			strContent += (strObjectI + "对" + strFile + ACE2Permission(resultACEs[i]) + "具有" + ACE2Action(resultACEs[i]) + "权限，而");
			strContent += (strObjectJ + "对" + strFile + ACE2Permission(resultACEs[j]) + "具有" + ACE2Action(resultACEs[j]) + "权限。");
			iConflictNo ++;
			Output_ReportList(itos(iConflictNo), strContent);

			///////////////////////////////////////冲突原因查询显示///////////////////////////////////////
			int iRelationAssertionForward = getQueryRelationAssertion_Reason(getUserOrGroupDNNameFromACE(resultACEs[i]),
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			int iRelationAssertionBackward = getQueryRelationAssertion_Reason(getUserOrGroupDNNameFromACE(resultACEs[j]),
				getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject");
			vector<CString> nstrLink = getQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			vector<CString> nstrLink2 = getQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
					getUserOrGroupDNNameFromACE(resultACEs[i]), "has_subSubject");


			if (iRelationAssertionForward)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
			}
			else if (iRelationAssertionBackward)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
			}
			else
			{
				if (nstrLink.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					for (int k = 0; k < nstrLink.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink[k]));
					}
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
				}
				else if (nstrLink2.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					for (int k = 0; k < nstrLink2.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink2[k]));
					}
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict_FileInterface"));
					return;
				}
			}
		}
	}
}

void DLInterface::queryOneKindOfConflict2_FileInterface(ADDomain *domain, CString strActionType, CString strActionWord,
														int &iConflictNo, int iMixOrShareOrNTFS)
{
	int i, j;
	m_iCurConflict = 0;
	m_iCurReason = 0;
	m_iCurCommonInstance = 0;
	vector<ADACE*> resultACEs;
	resultACEs = domain->getAllACEs(iMixOrShareOrNTFS);

	clearQueryString();
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			prepareQueryAccessRelationConflict2(resultACEs[i]->dn, resultACEs[j]->dn, strActionType);
		}
	}
	submitOntologyAndQueryString();
	answerString2ConflictString();

	clearQueryString();
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			if (getQueryAccessRelationConflict2(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}

			prepareQueryCommonInstanceForTwo(getUserOrGroupDNNameFromACE(resultACEs[i]), 
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
		}
	}
	submitOntologyAndQueryString();
	answerString2CommonInstanceString();

	clearQueryString();
	m_iCurConflict = 0;
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			if (getQueryAccessRelationConflict2(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}
			
			CString strCommonChildDN = getQueryCommonInstanceForTwo(getUserOrGroupDNNameFromACE(resultACEs[i]), 
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			prepareQueryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[i]),
				strCommonChildDN, "has_subSubjectOrigin");
			prepareQueryRelationAssertion(getUserOrGroupDNNameFromACE(resultACEs[j]),
				strCommonChildDN, "has_subSubjectOrigin");
			prepareQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
				strCommonChildDN, "has_subSubjectOrigin");
			prepareQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
				strCommonChildDN, "has_subSubjectOrigin");
		}
	}
	submitOntologyAndQueryString();
	answerString2ReasonString();

	clearQueryString();
	m_iCurConflict = 0;
	m_iCurCommonInstance = 0;
	for (i = 0; i < resultACEs.size(); i ++)
	{
		for (j = i + 1; j < resultACEs.size(); j ++)
		{
			if (getQueryAccessRelationConflict2(resultACEs[i]->dn, resultACEs[j]->dn, strActionType) == 0)
			{
				continue;
			}
			
			CString strObjectI = getUserOrGroupDescriptionFromACE(resultACEs[i]);
			CString strObjectJ = getUserOrGroupDescriptionFromACE(resultACEs[j]);
			if (resultACEs[i]->fatherFolder->cn != resultACEs[j]->fatherFolder->cn)
			{
				MyMessageBox_Error(_T("queryOneKindOfConflict2_FileInterface"));
				return;
			}
			CString strFile = getShareFolderDescriptionFromACE(resultACEs[i]);
			
			CString strContent;
			strContent += (strObjectI + "与" + strObjectJ + "在操作" + strFile + "发生" + strActionWord + "职责分离冲突：");
			strContent += (strObjectI + "对" + strFile + ACE2Permission(resultACEs[i]) + "具有" + ACE2Action(resultACEs[i]) + "权限，而");
			strContent += (strObjectJ + "对" + strFile + ACE2Permission(resultACEs[j]) + "具有" + ACE2Action(resultACEs[j]) + "权限。");
			iConflictNo ++;
			Output_ReportList(itos(iConflictNo), strContent);
			
			CString strCommonChildDN = getQueryCommonInstanceForTwo(getUserOrGroupDNNameFromACE(resultACEs[i]), 
				getUserOrGroupDNNameFromACE(resultACEs[j]), "has_subSubject");
			int iRelationAssertion1 = getQueryRelationAssertion_Reason(getUserOrGroupDNNameFromACE(resultACEs[i]),
				strCommonChildDN, "has_subSubjectOrigin");
			int iRelationAssertion2 = getQueryRelationAssertion_Reason(getUserOrGroupDNNameFromACE(resultACEs[j]),
				strCommonChildDN, "has_subSubjectOrigin");
			vector<CString> nstrLink1 = getQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[i]),
					strCommonChildDN, "has_subSubjectOrigin");
			vector<CString> nstrLink2 = getQueryRelationAssertionLink(getUserOrGroupDNNameFromACE(resultACEs[j]),
					strCommonChildDN, "has_subSubjectOrigin");
			///////////////////////////////////////第一个冲突原因查询显示///////////////////////////////////////
			if (iRelationAssertion1)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
				nstrNodes.push_back(dl2Description(strCommonChildDN));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
			}
			else
			{
				if (nstrLink1.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[i]));
					for (int k = 0; k < nstrLink1.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink1[k]));
					}
					nstrNodes.push_back(dl2Description(strCommonChildDN));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict2_FileInterface"));
					return;
				}
			}
			///////////////////////////////////////第二个冲突原因查询显示///////////////////////////////////////
			if (iRelationAssertion2)
			{
				vector<CString> nstrNodes;
				nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
				nstrNodes.push_back(dl2Description(strCommonChildDN));
				strContent = getConflictReasonFromStrings(nstrNodes);
				Output_ReportList(itos(iConflictNo), strContent);
			}
			else
			{
				if (nstrLink2.size() != 0)
				{
					vector<CString> nstrNodes;
					nstrNodes.push_back(getUserOrGroupDescriptionFromACE(resultACEs[j]));
					for (int k = 0; k < nstrLink2.size(); k ++)
					{
						nstrNodes.push_back(dl2Description(nstrLink2[k]));
					}
					nstrNodes.push_back(dl2Description(strCommonChildDN));
					strContent = getConflictReasonFromStrings(nstrNodes);
					Output_ReportList(itos(iConflictNo), strContent);
				}
				else
				{
					MyMessageBox_Error(_T("queryOneKindOfConflict2_FileInterface"));
					return;
				}
			}
		}
	}
}

void DLInterface::queryAllConflicts_FileInterface(vector<ADDomain*> npDomains, int &iConflictNo)
{
	//int iConflictNo = 0;
	for (int i = 0; i < npDomains.size(); i ++)
	{
		iConflictNo = 0;
		if (m_iMixOrShareOrNTFS == 1)
		{
			queryOneKindOfConflict_FileInterface(npDomains[i], "R", "[Share-读]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict_FileInterface(npDomains[i], "X", "[Share-执行]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict_FileInterface(npDomains[i], "W", "[Share-写]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "R", "[Share-读]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "X", "[Share-执行]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "W", "[Share-写]", iConflictNo, m_iMixOrShareOrNTFS);

			CString strContent;
			strContent += ("域" + npDomains[i]->dNSName + "一共检测出" + itos(iConflictNo) + "处Share策略冲突！");
			Output_ReportList(itos(iConflictNo), strContent);
		}
		else if (m_iMixOrShareOrNTFS == 0)
		{
			queryOneKindOfConflict_FileInterface(npDomains[i], "R", "[NTFS-读]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict_FileInterface(npDomains[i], "X", "[NTFS-执行]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict_FileInterface(npDomains[i], "W", "[NTFS-写]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "R", "[NTFS-读]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "X", "[NTFS-执行]", iConflictNo, m_iMixOrShareOrNTFS);
			queryOneKindOfConflict2_FileInterface(npDomains[i], "W", "[NTFS-写]", iConflictNo, m_iMixOrShareOrNTFS);

			CString strContent;
			strContent += ("域" + npDomains[i]->dNSName + "一共检测出" + itos(iConflictNo) + "处NTFS策略冲突！");
			Output_ReportList(itos(iConflictNo), strContent);
		}
		/*
		else
		{
			//queryOneKindOfConflict3(domain, "R", "[Share-NTFS间-读]", iConflictNo);
			//queryOneKindOfConflict3(domain, "X", "[Share-NTFS间-执行]", iConflictNo);
			//queryOneKindOfConflict3(domain, "W", "[Share-NTFS间-写]", iConflictNo);

			CString strContent;
			strContent += ("一共检测出" + itos(iConflictNo) + "处Share-NTFS间策略冲突");
			Output_ReportList(itos(iConflictNo ++), strContent);
		}
		*/
	}
}