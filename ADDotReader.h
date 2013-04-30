#include "StdAfx.h"
#include <vector>
using namespace std;

class ADDomain;
class CMyShape;
class CVisualADSView;
class CFOLinkShape;
class CFODrawPortsShape;
class ADObject;

class ADDotReader
{
public:
	CString m_strDataDirectory;
	vector<CString> m_nstrDotLines;
	CVisualADSView *m_pView;
	vector<ADDomain*> m_npDomains;

	CPoint m_ptGraphSize;
	double m_lfExpandRatio;
	double m_lfCustomX;
	double m_lfCustomY;

public:
	ADDotReader(CVisualADSView *pView, vector<ADDomain*> npDomains);
	void Tokenize(CString s, CString delimits, CStringList* pList, BOOL trim = FALSE, CString nullSubst = "");
	CString itos(int i);
	int stoi(CString s);
	BOOL FolderExist(CString strPath);
	void initDataDir();

	int ConvUtf8ToAnsi(CString &strSource, CString &strChAnsi);
	void readDotFile(CString strDotFileName);
	int getLineByString(CString strTarget);
	int getFirstLineByString(CString strTarget);
	CRect getBBFromLine(int iLine);
	CPoint getPosFromLine(int iLine);

	CPoint getGraphSize();
	CRect getOURectByString(CString strOUDN);
	CRect getBigComputerRectByString(CString strBigComputerDN);
	CPoint getElementPointByString(CString strElementDN);

	void parseGUIData2ModelAndGenerateShape();
	void generateLink();

	CMyShape* addDomain(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addOU(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addBigComputer(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addGroup(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addUser(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addComputer(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addResource(ADObject *aDObject, CString strName, int iX, int iY);
	CFOLinkShape* addInheritRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2);
	CFOLinkShape* addNTFSRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission, BOOL bBothRelation);
	CFOLinkShape* addShareRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission, BOOL bBothRelation);
};
