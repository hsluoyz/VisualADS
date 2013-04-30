#include "StdAfx.h"
#include <vector>
using namespace std;

class ADDomain;

typedef struct
{
	const char *data;
	int len;
	int cur;
} rdr_t;

class ADGraphGenerator
{
public:
	CString m_strGraphPathName;
	CString m_strDataDirectory;
	CString m_strGraphString;

public:
	ADGraphGenerator(vector<ADDomain*> globalDomains, CString strGraphName);
	CString getGraphPathName();
	CString getGraphString();
	CString closureDigraph(CString strIn, CString strName);
	CString closureSubgraph(CString strIn, CString strName, CString strLabel);
	CString makeNode(CString strName, CString strLabel, CString strShape, CString strColor, BOOL bFilled);
	CString makeEdge(vector<CString> nstrNames);
	CString domain2String(ADDomain* domain);
	BOOL string2Graph(CString strDotFormat, CString strPathName);
	BOOL validateString(CString strDotFormat);

	BOOL FolderExist(CString strPath);
	BOOL FileExist(CString strFileName);
	void initDataDir();
	void registryDll(CString strDllPathName);
	void initWingraphvizInterface();
	void initFontFile();

	string ws2s(const wstring& ws);
	wstring s2ws(const string& s);
	string ConvertGBKToUtf8(string& strGBK);
	void string2Graph2(CString strDotFormat, CString strPathName);
};