#include "StdAfx.h"

#include "ADGraphGenerator.h"
#include "ADDomain.h"

#include "WinGraphviz.h"
#include <atlbase.h>
#include <comutil.h>

#include <gvc.h>
#pragma comment(lib, "gvc.lib")
#pragma comment(lib, "graph.lib")

#pragma comment(lib, "jpeglibD.lib")
#pragma comment(lib, "lpng103D.lib")
#pragma comment(lib, "ZLibD.lib")

ADGraphGenerator::ADGraphGenerator(vector<ADDomain*> globalDomains, CString strGraphName)
{
	initDataDir();
	initFontFile();
	m_strGraphPathName = m_strDataDirectory + strGraphName;

	for (int i = 0; i < globalDomains.size(); i ++)
	{
		m_strGraphString += domain2String(globalDomains[i]);
	}
	m_strGraphString = closureDigraph(m_strGraphString, _T("G"));
	//
	//MyMessageBox_Error(m_strGraphString, _T("Dot Format"));
	//
	if (validateString(m_strGraphString))
	{
		string2Graph2(m_strGraphString, m_strGraphPathName);
	}
}

CString ADGraphGenerator::getGraphPathName()
{
	return m_strGraphPathName;
}

CString ADGraphGenerator::getGraphString()
{
	return m_strGraphString;
}

CString ADGraphGenerator::closureDigraph(CString strIn, CString strName)
{
	CString strOut = _T("digraph ") + strName + _T("\n{\nrankdir=LR;\nsplines=false;\nfontname=\"Microsoft YaHei Bold\";\nnode [fontname=\"Microsoft YaHei Bold\"];\nedge [fontname=\"Microsoft YaHei Bold\"];\n") + strIn + _T("}\n");
	return strOut;
}

CString ADGraphGenerator::closureSubgraph(CString strIn, CString strName, CString strLabel)
{
	CString strOut = _T("subgraph cluster_") + strName + _T("\n{\n") + strIn + _T("label=") + strLabel + _T(";\n") + _T("}\n");
	return strOut;
}

CString ADGraphGenerator::makeNode(CString strName, CString strLabel, CString strShape, CString strColor, BOOL bFilled)
{
	CString strOut;
	if (bFilled)
		strOut = strName + _T(" [label=") + strLabel + _T(", shape=") + strShape + _T(", color=") + strColor + _T(", style=filled];\n");
	else
		strOut = strName + _T(" [label=") + strLabel + _T(", shape=") + strShape + _T(", color=") + strColor + _T("];\n");
	return strOut;
}

CString ADGraphGenerator::makeEdge(vector<CString> nstrNames)
{
	CString strOut;
	if (nstrNames.size() == 0)
		return _T("");
	for (int i = 0; i < nstrNames.size() - 1; i ++)
	{
		strOut += nstrNames[i];
		strOut += _T("->");
	}
	strOut += nstrNames[i];
	strOut += _T(";\n");
	return strOut;
}



CString ADGraphGenerator::domain2String(ADDomain* domain)
{
	return domain->Node2String() + domain->Edge2String();

	/*
	CString strNodes = makeNode(_T("A"), _T("A"), _T("polygon"), _T("red"), FALSE);
	strNodes += makeNode(_T("B"), _T("B"), _T("polygon"), _T("green"), TRUE);
	vector<CString> nstrEdges;
	nstrEdges.push_back(_T("A"));
	nstrEdges.push_back(_T("B"));
	CString strEdges = makeEdge(nstrEdges);
	CString strSubGraph = closureSubgraph(strNodes + strEdges, _T("Phase1"), _T("Phase1"));
	CString strSubGraph2 = closureSubgraph(strNodes + strEdges, _T("Phase2"), _T("Phase2"));
	CString strGraph = closureDigraph(strSubGraph + strSubGraph2, _T("G"));
	return strGraph;
	*/
}

BOOL ADGraphGenerator::string2Graph(CString strDotFormat, CString strPathName)
{
	const IID IID_IDOT =
	{
		0xA1080582, 0xD33F, 0x486E, {0xBD,0x5F,0x58,0x19,0x89,0xA3,0xC7,0xE9}
	};
	const CLSID CLSID_DOT =
	{
		0x55811839, 0x47B0, 0x4854, {0x81,0xB5,0x0A,0x00,0x31,0xB8,0xAC,0xEC}
	};

	USES_CONVERSION;
	HRESULT hr;
	IDOT* pIDOT;
	CComBSTR result;
	IBinaryImage* resultBitImage;
	int iHasRegistered = 0;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MyMessageBox_Error(_T("string2Graph Error"), _T("Error"));
		return FALSE;
	}
	else
	{
		label1 : hr = CoCreateInstance(CLSID_DOT, NULL, CLSCTX_ALL, IID_IDOT, (void**)&pIDOT);
		if (FAILED(hr))
		{
			if (iHasRegistered == 0)
			{
				initWingraphvizInterface();
				iHasRegistered = 1;
				goto label1;
			}
			else
			{
				MyMessageBox_Error(_T("string2Graph Error"), _T("Error"));
				return FALSE;
			}
		}
		else
		{
			//hr = pIDOT->ToSvg(A2BSTR("digraph G {A -> B -> C -> A;}"), &result);
			VARIANT_BOOL varResult;
			pIDOT->Validate(_bstr_t(strDotFormat), &varResult);
			varResult = VARIANT_TRUE;
			if (varResult == VARIANT_TRUE)
			{
				hr = pIDOT->ToPNG(A2BSTR(_bstr_t(strDotFormat)), &resultBitImage);
				if (SUCCEEDED(hr))
				{
					resultBitImage->Save(_bstr_t(strPathName), &varResult);
					return TRUE;
				}
				else
				{
					MyMessageBox_Error(_T("string2Graph Error"), _T("Error"));
					return FALSE;
				}
			}
			else
			{
				MyMessageBox_Error(_T("string2Graph Error"), _T("Error"));
				return FALSE;
			}
		}
		CoUninitialize();
	}
}

BOOL ADGraphGenerator::validateString(CString strDotFormat)
{
	const IID IID_IDOT =
	{
		0xA1080582, 0xD33F, 0x486E, {0xBD,0x5F,0x58,0x19,0x89,0xA3,0xC7,0xE9}
	};
	const CLSID CLSID_DOT =
	{
		0x55811839, 0x47B0, 0x4854, {0x81,0xB5,0x0A,0x00,0x31,0xB8,0xAC,0xEC}
	};

	USES_CONVERSION;
	HRESULT hr;
	IDOT* pIDOT;
	CComBSTR result;
	int iHasRegistered = 0;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MyMessageBox_Error(_T("validateString Error"), _T("Error"));
		return FALSE;
	}
	else
	{
		label1 : hr = CoCreateInstance(CLSID_DOT, NULL, CLSCTX_ALL, IID_IDOT, (void**)&pIDOT);
		if (FAILED(hr))
		{
			if (iHasRegistered == 0)
			{
				initWingraphvizInterface();
				iHasRegistered = 1;
				goto label1;
			}
			else
			{
				MyMessageBox_Error(_T("validateString Error"), _T("Error"));
				return FALSE;
			}
		}
		else
		{
			//hr = pIDOT->ToSvg(A2BSTR("digraph G {A -> B -> C -> A;}"), &result);
			VARIANT_BOOL varResult;
			pIDOT->Validate(_bstr_t(strDotFormat), &varResult);
			varResult = VARIANT_TRUE;
			if (varResult == VARIANT_TRUE)
			{
				return TRUE;
			}
			else
			{
				MyMessageBox_Error(_T("validateString Error"), _T("Error"));
				return FALSE;
			}
		}
		CoUninitialize();
	}
}

BOOL ADGraphGenerator::FolderExist(CString strPath)
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

BOOL ADGraphGenerator::FileExist(CString strFileName)
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

void ADGraphGenerator::initDataDir()
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
			MyMessageBox_Error(_T("initDataDir Error"), _T("Error"));
			return;
		}
	}
	cstrDir += _T("\\data\\");
	m_strDataDirectory = cstrDir;
}

void ADGraphGenerator::registryDll(CString strDllPathName)
{
	if (!FileExist(strDllPathName))
	{
		MyMessageBox_Error(_T("registryDll Error"), _T("Error"));
		return;
	}   
	LRESULT (CALLBACK* lpDllEntryPoint)();    
	HINSTANCE hLib = LoadLibrary(strDllPathName);   
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
	{
		MyMessageBox_Error(_T("registryDll Error"), _T("Error"));
		return;
	}
	(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");    
	BOOL bRet = FALSE;   
	if (lpDllEntryPoint != NULL)
	{
		HRESULT hr = (*lpDllEntryPoint)();   
		bRet = SUCCEEDED(hr);   
		if (FAILED(hr))
		{
			MyMessageBox_Error(_T("registryDll Error"), _T("Error"));
			return;
		}
	}
	else
	{
		MyMessageBox_Error(_T("registryDll Error"), _T("Error"));
		return;
	}
	FreeLibrary(hLib);
}

void ADGraphGenerator::initWingraphvizInterface()
{
	CString strFromPathName = m_strDataDirectory + _T("Wingraphviz.dll");
	TCHAR path[MAX_PATH];
	GetSystemDirectory(path, MAX_PATH) ;
	CString strToPathName = path;
	strToPathName += _T("\\Wingraphviz.dll");

	if (!FileExist(strToPathName))
	{
		if (!FileExist(strFromPathName))
		{
			MyMessageBox_Error(_T("initWingraphvizInterface Error"), _T("Error"));
			return;
		}
		else
		{
			if (!CopyFileEx(strFromPathName, strToPathName, (LPPROGRESS_ROUTINE)NULL, NULL, FALSE, COPY_FILE_FAIL_IF_EXISTS))
			{
				MyMessageBox_Error(_T("initWingraphvizInterface Error"), _T("Error"));
				return;
			}
		}
	}
	registryDll(strToPathName);
}

void ADGraphGenerator::initFontFile()
{
	CString strFromPathName = m_strDataDirectory + _T("msyhbd.ttf");

	TCHAR path[MAX_PATH];
	GetSystemDirectory(path, MAX_PATH) ;
	CString strToPathName = path;
	int nCount = strToPathName.ReverseFind(_T('\\'));
	strToPathName.Delete(nCount, strToPathName.GetLength() - nCount);
	strToPathName += _T("\\Fonts\\msyhbd.ttf");

	if (!FileExist(strToPathName))
	{
		if (!FileExist(strFromPathName))
		{
			MyMessageBox_Error(_T("initFontFile Error"), _T("Error"));
			return;
		}
		else
		{
			if (!CopyFileEx(strFromPathName, strToPathName, (LPPROGRESS_ROUTINE)NULL, NULL, FALSE, COPY_FILE_FAIL_IF_EXISTS))
			{
				MyMessageBox_Error(_T("initFontFile Error"), _T("Error"));
				return;
			}
		}
	}
}

char* graph_reader(char* str, int num, FILE* stream)
{
	if (num == 0)
		return str;
	const char* ptr;
	char* optr;
	char c;
	int l;
	rdr_t* s = (rdr_t*)stream;
	if (s->cur >= s->len)
		return NULL;
	l = 0;
	ptr = s->data + s->cur;
	optr = str;
	do
	{
		*optr++ = c = *ptr++;
		l++;
	}
	while (c && (c != '\n') && (l < num - 1));
	*optr = '\0';
	s->cur += l;
	return str;
}

string ADGraphGenerator::ws2s(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring ADGraphGenerator::s2ws(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

string ADGraphGenerator::ConvertGBKToUtf8(string& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0); 
	unsigned short* wszUtf8 = new unsigned short[len + 1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 
	char* szUtf8 = new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL); 
	string result = szUtf8; 
	delete[] szUtf8; 
	delete[] wszUtf8;
	return result;
} 

void ADGraphGenerator::string2Graph2(CString strDotFormat, CString strPathName)
{
	GVC_t* gvc;
	Agraph_t* g;
	gvc = gvContext();

	//wstring wstrBuffer = strDotFormat;
	//string strBufferMBCS = ws2s(wstrBuffer);
	_bstr_t bstrBuffer = strDotFormat;
	LPSTR lpstrBuffer = bstrBuffer;
	string strBufferMBCS = lpstrBuffer;
	string strBufferUTF8 = ConvertGBKToUtf8(strBufferMBCS);

	rdr_t rdr;
	rdr.data = strBufferUTF8.c_str();
	rdr.len = strlen(rdr.data);
	rdr.cur = 0;
	g = agread_usergets((FILE *)&rdr, (gets_f)graph_reader);

	gvLayout(gvc, g, "dot");
	CStdioFile file;
	file.Open(_bstr_t(strPathName + _T("_origin.dot")),CFile::modeCreate | CFile::modeWrite);
	file.WriteString(strDotFormat);
	file.Close();

	gvRenderFilename(gvc, g, "png", _bstr_t(strPathName + _T(".png")));
	gvRenderFilename(gvc, g, "dot", _bstr_t(strPathName + _T(".dot")));

	gvFreeLayout(gvc, g);
	agclose(g);
	gvFreeContext(gvc);
}