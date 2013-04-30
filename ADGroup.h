#include "StdAfx.h"

#include <vector>
using namespace std;

#include "ADObject.h"
class ADDomain;
class ADUser;
class ADDotReader;
class CMyShape;

class ADGroup : public ADObject
{
	DECLARE_SERIAL(ADGroup)
public:
	CString cn;
	CString dn;
	ADDomain* domain;
	CString type;
	vector<CString> groupMembers;
	vector<ADGroup*> childGroups;
	vector<ADUser*> childUsers;
	vector<ADGroup*> fathers;

	CPoint ptGUI;
	CMyShape *shape;
	
public:
	ADGroup();
	void Serialize(CArchive &ar);
	ADGroup(CString a_cn, CString a_dn, ADDomain* a_domain, CString a_type);
	CString itos(int i);
	void addChildGroup(ADGroup* group);
	void addChildUser(ADUser* user);
	void addFather(ADGroup* group);
	CString Node2String();
	CString Edge2String();
	void parseGUIData(ADDotReader *reader);
	void generateLink(ADDotReader *reader);
	void addToProperties();
};