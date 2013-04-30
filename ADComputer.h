#include "StdAfx.h"

#include <vector>
using namespace std;

#include "ADObject.h"
class ADDomain;
class ADShareFolder;
class ADDotReader;
class CMyShape;

class ADComputer : public ADObject
{
	DECLARE_SERIAL(ADComputer)
public:
	CString cn;
	CString cnOld;
	CString dn;
	ADDomain* domain;
	CString ip;
	int run;
	vector<ADShareFolder*> shareFolders;

	CPoint ptGUI;
	CRect rectGUI;
	CMyShape *shape;
	
public:
	ADComputer();
	void Serialize(CArchive &ar);
	ADComputer(CString a_cn, CString a_dn, ADDomain* a_domain);
	CString Node2String();
	void parseGUIData(ADDotReader *reader);
	void generateLink(ADDotReader *reader);
	void addToProperties();
};