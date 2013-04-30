#include "StdAfx.h"
#include "DLRacerInterface.h"
#include "MainFrm.h"
#include "MyMessageBox.h"
#include "resource.h"

void msgbox(CString str)
{
	MyMessageBox_Error(str);
}

//a TCP interface interacting with RacerPro rewritten by me in C++, by consulting the original JRacer interface (a Java TCP interface).
DLRacerInterface::DLRacerInterface(CString a_strIP, int a_iPort)
{
	racerSocket = NULL;
	m_strIP = a_strIP;
	m_iPort = a_iPort;
	WORD wVersionRequested =MAKEWORD(1, 1);
	int iError = WSAStartup(wVersionRequested, &wsaData);
	if (iError != 0)
	{
		MyMessageBox_Error(_T("DLRacerInterface"));
	}
	else
	{
		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		{
			MyMessageBox_Error(_T("DLRacerInterface"));
		}
	}
}

DLRacerInterface::~DLRacerInterface()
{
	closesocket(racerSocket);
}

BOOL DLRacerInterface::openConnection()
{
	int result;

	
	racerSocket = socket(AF_INET, SOCK_STREAM, 0);

	//CMainFrame *pMainFrm = (CMainFrame*) AfxGetMainWnd();
	//WSAAsyncSelect(racerSocket, pMainFrm->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE | FD_READ | FD_WRITE);
	if(racerSocket == INVALID_SOCKET)
	{
		msgbox(_T("openConnection"));
		return FALSE;
	}
	else
	{
		racerAddr.sin_family = AF_INET;
		racerAddr.sin_port = htons(m_iPort); 
		racerAddr.sin_addr.s_addr = inet_addr(_bstr_t(m_strIP));
		result = connect(racerSocket, (SOCKADDR*) &racerAddr, sizeof(racerAddr));
		if (result == -1)
		{
			//msgbox(_T("openConnection"));
			int dwError = WSAGetLastError(); //akisn0w
			if (dwError == 10035)
			{
				//远程没启动，还算正常
				return FALSE;
			}
			else
			{
				MyMessageBox_Error(_T("openConnection"));
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}
}

void DLRacerInterface::closeConnection()
{
	if (racerSocket)
	{
		closesocket(racerSocket);
		racerSocket = INVALID_SOCKET;
	}
	//WSACleanup();
}

CString DLRacerInterface::readFromSocket()
{
	char c;
	
	CString strTemp;
	
	recv(racerSocket, &c, 1, 0);
	strTemp += c;

	while (c != 10)
	{
		recv(racerSocket, &c, 1, 0);
		if (c != 10)
		{
			strTemp += c;
		}
	}

//	char buf[1024];
// 	recv(racerSocket, buf, 1024, 0);
// 	strTemp += buf;
// 	
// 	while (c != 10)
// 	{
// 		recv(racerSocket, &c, 1, 0);
// 		if (c != 10)
// 		{
// 			strTemp += c;
// 		}
// 	}

	return strTemp;
}

CString DLRacerInterface::sendCommand(CString strCommand)
{
	strCommand += _T("\n");
	int status;

	status = send(racerSocket, strCommand, strCommand.GetLength(), 0);

	if (status == -1)
	{
		int dwError = WSAGetLastError();
		MyMessageBox_Error(_T("sendCommand"));
		return _T("null");
	}
	CString strResult = readFromSocket();
	return parseResult(strCommand, strResult);
}

CString DLRacerInterface::parseResult(CString strCommand, CString strResult)
{
	if (strResult[1] == _T('o'))
	{
		CString strWarning = getWarningFromOK(strResult);
		if (strWarning != _T(""))
			printWarning(strWarning);
		return _T("null");
	}
	else if (strResult[1] == _T('e'))
	{
		CString strError = getError(strResult);
		return strError;
	}
	else if (strResult[1] == _T('a'))
	{
		vector<CString> strAnsAndWar = getResultAndWarningFromAnswer(strResult);
		if (strAnsAndWar[1] != _T(""))
		{
			printWarning(strAnsAndWar[1]);
		}
		return strAnsAndWar[0];
	}
	else
	{
		msgbox(_T("parseResult"));
		return _T("parseResult");
	}
}

CString DLRacerInterface::getWarningFromOK(CString strResult)
{
	CString strWarning = _T("");
	int ini = strResult.Find(_T('"'), 6);
	int fi = strResult.GetLength() - 1;
	if (ini < fi - 1)
	{
		strWarning = strResult.Mid(ini + 1, fi - ini - 1);
	}
	return strWarning;
}

void DLRacerInterface::printWarning(CString strWarning)
{
	msgbox(strWarning);
// 	for (int i = 0; i < strWarning.GetLength(); i ++)
// 	{
// 		char c = strWarning[i];
// 		if (c == 9)
// 		{
// 			//cout << endl;
// 		}
// 		else
// 		{
// 			msgbox(strWarning[i]);
// 		}
// 	}
}

CString DLRacerInterface::getError(CString strResult)
{
	CString s;
	int iniMessage = strResult.Find(_T(' '), 7);
	int ini = strResult.Find(_T('"'), iniMessage);
	int fi = strResult.Find(_T('"'), ini + 1);
	s = (iniMessage + 1 < ini - 1)? strResult.Mid(iniMessage + 1, ini - 1 - iniMessage - 1) : _T("");
	if ((iniMessage + 1 < ini - 1) && (ini + 1 < fi))
	{
		s = s + strResult.Mid(ini + 1, fi - ini - 1);
	}
	return s;
}

vector<CString> DLRacerInterface::getResultAndWarningFromAnswer(CString strResult)
{
	vector<CString> s;
	int ini = strResult.Find(_T('"'), 10);
	int fi = ini;
	bool esFinal = FALSE;
	while (!esFinal)
	{
		fi = strResult.Find(_T('"'), fi + 1);
		esFinal = (strResult[fi -1] != _T('\\'));
	}
	s.push_back(strResult.Mid(ini + 1, fi - ini - 1));
	if (fi + 4 < strResult.GetLength())
	{
		s.push_back(strResult.Mid(fi + 3, strResult.GetLength() - 1 - fi - 3));
	}
	else
	{
		s.push_back(_T(""));
	}
	return s;
}
