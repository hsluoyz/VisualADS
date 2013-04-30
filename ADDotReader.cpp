#include "StdAfx.h"

#include "ADDotReader.h"
#include "MyShape.h"
#include "ADDomain.h"
#include "resource.h"
#include "VisualADSView.h"

#pragma warning (disable : 4244)

ADDotReader::ADDotReader(CVisualADSView *pView, vector<ADDomain*> npDomains)
{
	m_pView = pView;
	m_npDomains = npDomains;
	m_lfExpandRatio = 1;//1.5f;
	m_lfCustomX = 1;//1.1f;
	m_lfCustomY = 1;//1.5f;
	initDataDir();
	readDotFile(_T("ADDomain_Buffer.dot"));
	parseGUIData2ModelAndGenerateShape();
	generateLink();
}

CString ADDotReader::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

int ADDotReader::stoi(CString s)
{
	return atoi(s);
}

void ADDotReader::Tokenize(CString s, CString delimits, CStringList* pList, BOOL trim, CString nullSubst)
{
	ASSERT(!s.IsEmpty() && !delimits.IsEmpty());
	s += delimits[0];
	for (long index = -1; (index = s.FindOneOf((LPCTSTR) delimits)) != -1;)
	{
		if(index != 0)
			pList->AddTail(s.Left(index));
		else if(!trim)
			pList->AddTail(nullSubst);
		s = s.Right(s.GetLength() - index - 1);
	}
}

BOOL ADDotReader::FolderExist(CString strPath)
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

void ADDotReader::initDataDir()
{
	CString cstrDir;
	TCHAR cBuffer[260];
	GetCurrentDirectory(MAX_PATH, cBuffer);
	CString cstrPath(cBuffer);
	if (FolderExist(cstrPath + _T("\\data")))
	{
		cstrDir = cstrPath;
	}
	else
	{
		int nCount = cstrPath.ReverseFind(_T('\\'));
		cstrPath.Delete(nCount, cstrPath.GetLength() - nCount);
		
		if (FolderExist(cstrPath + _T("\\data")))
		{
			cstrDir = cstrPath;
		}
		else
		{
			MyMessageBox_Error(_T("FolderExist Error"), _T("Error"));
			return;
		}
	}
	cstrDir += _T("\\data\\");
	m_strDataDirectory = cstrDir;
}

int ADDotReader::ConvUtf8ToAnsi(CString &strSource, CString &strChAnsi)
{
    if (strSource.GetLength() <= 0)
        return 0;
    CString strWChUnicode;
    strSource.TrimLeft();
    strSource.TrimRight();
    strChAnsi.Empty();
    int iLenByWChNeed = MultiByteToWideChar(CP_UTF8, 0, strSource.GetBuffer(0), strSource.GetLength(), NULL, 0);
    int iLenByWchDone = MultiByteToWideChar(CP_UTF8, 0, strSource.GetBuffer(0), strSource.GetLength(),
		(LPWSTR)strWChUnicode.GetBuffer(iLenByWChNeed * 2), iLenByWChNeed);
    strWChUnicode.ReleaseBuffer(iLenByWchDone * 2);
	
    int iLenByChNeed  = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strWChUnicode.GetBuffer(0), iLenByWchDone, NULL, 0, NULL, NULL);
    int iLenByChDone  = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strWChUnicode.GetBuffer(0), iLenByWchDone,
		strChAnsi.GetBuffer(iLenByChNeed), iLenByChNeed, NULL, NULL);
	strChAnsi.ReleaseBuffer(iLenByChDone);
	
    if (iLenByWChNeed != iLenByWchDone || iLenByChNeed != iLenByChDone)
        return 1;
    return 0;
}


void ADDotReader::readDotFile(CString strDotFileName)
{
	CStdioFile dotFile;
	dotFile.Open(m_strDataDirectory + strDotFileName, CFile::modeRead | CFile::typeText);

	CString strLineUTF8;
	CString strLineMBCS;
	while (dotFile.ReadString(strLineUTF8))
	{
		ConvUtf8ToAnsi(strLineUTF8, strLineMBCS);
		m_nstrDotLines.push_back(strLineMBCS);
	}

	dotFile.Close();
}

int ADDotReader::getLineByString(CString strTarget)
{
	int iNumber = 0;
	int iTargetLine = -1;
	for (int i = 0; i < m_nstrDotLines.size(); i ++)
	{
		if (m_nstrDotLines[i].Find(strTarget, 0) != -1)
		{
			iTargetLine = i;
			iNumber ++;
		}
	}

	if (iNumber == 0)
		return -1;
	else if (iNumber == 1)
		return iTargetLine;
	else
		return -2;
}

int ADDotReader::getFirstLineByString(CString strTarget)
{
	for (int i = 0; i < m_nstrDotLines.size(); i ++)
	{
		if (m_nstrDotLines[i].Find(strTarget, 0) != -1)
		{
			return i;
		}
	}
	return -1;
}

CRect ADDotReader::getBBFromLine(int iLine)
{
	CString strLine = m_nstrDotLines[iLine];
	CRect resultRect = CRect(-1, -1, -1, -1);
	int iStart = -1;
	int iEnd = -1;

	CString strBBFlag = _T("bb=");
	iStart = strLine.Find(strBBFlag);
	if (iStart == -1)
	{
		MyMessageBox_Error(_T("getBBFromLine Error"), _T("Error"));
		return resultRect;
	}
	iStart = strLine.Find(_T("\""), iStart);
	iEnd = strLine.Find(_T("\""), iStart + 1);

	CStringList nstrList;
	CString strTargetFragment = strLine.Mid(iStart + 1, iEnd - iStart - 1);
	Tokenize(strTargetFragment, _T(","), &nstrList, TRUE, "no use");

	if (nstrList.GetCount() != 4)
	{
		MyMessageBox_Error(_T("getBBFromLine Error"), _T("Error"));
		return resultRect;
	}
	else
	{
		resultRect.left = stoi(nstrList.GetAt(nstrList.FindIndex(0)));
		resultRect.top = stoi(nstrList.GetAt(nstrList.FindIndex(1)));
		resultRect.right = stoi(nstrList.GetAt(nstrList.FindIndex(2)));
		resultRect.bottom = stoi(nstrList.GetAt(nstrList.FindIndex(3)));
		return resultRect;
	}
}

CPoint ADDotReader::getPosFromLine(int iLine)
{
	CString strLine = m_nstrDotLines[iLine];
	CPoint resultPoint = CPoint(-1, -1);
	int iStart = -1;
	int iEnd = -1;
	
	CString strPosFlag = _T("pos=");
	iStart = strLine.Find(strPosFlag);
	if (iStart == -1)
	{
		MyMessageBox_Error(_T("getPosFromLine Error"), _T("Error"));
		return resultPoint;
	}
	iStart = strLine.Find(_T("\""), iStart);
	iEnd = strLine.Find(_T("\""), iStart + 1);
	
	CStringList nstrList;
	CString strTargetFragment = strLine.Mid(iStart + 1, iEnd - iStart - 1);
	Tokenize(strTargetFragment, _T(","), &nstrList, TRUE, "no use");
	
	if (nstrList.GetCount() != 2)
	{
		MyMessageBox_Error(_T("getPosFromLine Error"), _T("Error"));
		return resultPoint;
	}
	else
	{
		resultPoint.x = stoi(nstrList.GetAt(nstrList.FindIndex(0)));
		resultPoint.y = stoi(nstrList.GetAt(nstrList.FindIndex(1)));
		return resultPoint;
	}
}

CPoint ADDotReader::getGraphSize()
{
	CPoint resultPoint = CPoint(-1, -1);
	CRect rectGraphSize = getBBFromLine(4);
	if (rectGraphSize.left == -1)
	{
		MyMessageBox_Error(_T("getGraphSize Error"), _T("Error"));
		return resultPoint;
	}
	m_ptGraphSize.x = rectGraphSize.right;
	m_ptGraphSize.y = rectGraphSize.bottom;
	resultPoint = m_ptGraphSize;
	return resultPoint;
}

CRect ADDotReader::getOURectByString(CString strOUDN)
{
	int iLine = getLineByString(strOUDN);
	return getBBFromLine(iLine + 2);
}

CRect ADDotReader::getBigComputerRectByString(CString strBigComputerDN)
{
	int iLine = getLineByString(strBigComputerDN);
	return getBBFromLine(iLine + 3);
}

CPoint ADDotReader::getElementPointByString(CString strElementDN)
{
	int iLine = getFirstLineByString(strElementDN);
	return getPosFromLine(iLine);
}

void ADDotReader::parseGUIData2ModelAndGenerateShape()
{
	getGraphSize();
	m_ptGraphSize.x *= (m_lfExpandRatio * m_lfCustomX);
	m_ptGraphSize.y *= m_lfExpandRatio;
	//m_pView->DoChangeCanvasSize(m_ptGraphSize);
	m_pView->MyDoChangeModel(m_ptGraphSize);
	for (int i = 0; i < m_npDomains.size(); i ++)
	{
		m_npDomains[i]->parseGUIData(this);
	}
}

void ADDotReader::generateLink()
{
	for (int i = 0; i < m_npDomains.size(); i ++)
	{
		m_npDomains[i]->generateLink(this);
	}
}

CMyShape* ADDotReader::addDomain(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	return m_pView->addDomain(aDObject, strName, iX1 * m_lfExpandRatio * m_lfCustomX, iY1 * m_lfExpandRatio * m_lfCustomY, iX2 * m_lfExpandRatio * m_lfCustomX, iY2 * m_lfExpandRatio * m_lfCustomY);
}

CMyShape* ADDotReader::addOU(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	return m_pView->addOU(aDObject, strName, iX1 * m_lfExpandRatio * m_lfCustomX, iY1 * m_lfExpandRatio * m_lfCustomY, iX2 * m_lfExpandRatio * m_lfCustomX, iY2 * m_lfExpandRatio * m_lfCustomY);
}

CMyShape* ADDotReader::addBigComputer(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	return m_pView->addBigComputer(aDObject, strName, iX1 * m_lfExpandRatio * m_lfCustomX, iY1 * m_lfExpandRatio * m_lfCustomY, iX2 * m_lfExpandRatio * m_lfCustomX, iY2 * m_lfExpandRatio * m_lfCustomY);
}

CMyShape* ADDotReader::addGroup(ADObject *aDObject, CString strName, int iX, int iY)
{
	return m_pView->addGroup(aDObject, strName, iX * m_lfExpandRatio, iY * m_lfExpandRatio);
}

CMyShape* ADDotReader::addUser(ADObject *aDObject, CString strName, int iX, int iY)
{
	return m_pView->addUser(aDObject, strName, iX * m_lfExpandRatio, iY * m_lfExpandRatio);
}

CMyShape* ADDotReader::addComputer(ADObject *aDObject, CString strName, int iX, int iY)
{
	return m_pView->addComputer(aDObject, strName, iX * m_lfExpandRatio, iY * m_lfExpandRatio);
}

CMyShape* ADDotReader::addResource(ADObject *aDObject, CString strName, int iX, int iY)
{
	return m_pView->addResource(aDObject, strName, iX * m_lfExpandRatio, iY * m_lfExpandRatio);
}

CFOLinkShape* ADDotReader::addInheritRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2)
{
	return m_pView->addInheritRelation(strName, pShape1, pShape2);
}

CFOLinkShape* ADDotReader::addNTFSRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission,
	BOOL bBothRelation)
{
	return m_pView->addNTFSRelation(strName, pShape1, pShape2, iPermission, bBothRelation);
}

CFOLinkShape* ADDotReader::addShareRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission,
	BOOL bBothRelation)
{
	return m_pView->addShareRelation(strName, pShape1, pShape2, iPermission, bBothRelation);
}