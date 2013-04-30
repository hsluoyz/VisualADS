#include "StdAfx.h"

#include <vector>
using namespace std;

#include "ADObject.h"
class ADGroup;
class ADDotReader;
class CMyShape;
class ADDomain;

class ADUser : public ADObject
{
	DECLARE_SERIAL(ADUser)
public:
	CString cn;
	CString dn;
	ADDomain* domain;
	vector<ADGroup*> fathers;

	CPoint ptGUI;
	CMyShape *shape;
	
public:
	ADUser();
	void Serialize(CArchive &ar);
	ADUser(CString a_cn, CString a_dn, ADDomain* a_domain);
	CString itos(int i);
	void addFather(ADGroup* group);
	CString Node2String();
	void parseGUIData(ADDotReader *reader);
	void generateLink(ADDotReader *reader);
	void addToProperties();
};
