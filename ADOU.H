#include "StdAfx.h"

#include <vector>
using namespace std;

#include "ADObject.h"
class ADDomain;
class ADUser;
class ADGroup;
class ADComputer;
class ADOU;
class ADEntry;
class ADDotReader;
class CMyShape;

class ADOU : public ADObject
{
	DECLARE_SERIAL(ADOU)
public:
	CString cn;
	CString dn;
	ADDomain* domain;
	
	vector<ADUser*> users;
	vector<ADGroup*> groups;
	vector<ADComputer*> computers;
	vector<ADOU*> ous;

	CRect rectGUI;
	CMyShape *shape;

public:
	ADOU();
	void Serialize(CArchive &ar);
	ADOU(CString a_cn, CString a_dn, ADDomain* a_domain);
	ADUser* addUser(CString strUserName, CString strDNName);
	ADGroup* addGroup(CString strGroupName, CString strDNName, ADDomain* domain, CString strType);
	ADComputer* addComputer(CString strComputerName, CString strDNName, ADDomain* domain);
	ADOU* addOU(CString strOUName, CString strDNName, ADDomain* domain);
	ADOU* findOU(CString strOUName, CString strDNName);

	void performEntry(ADEntry* entry);

	vector<ADUser*> getAllUsers();
	vector<ADGroup*> getAllGroups();
	vector<ADComputer*> getAllComputers();
	vector<ADOU*> getAllOUs();
	vector<ADShareFolder*> getAllShareFolders();

	CString Node2String();
	void parseGUIData(ADDotReader *reader);
	void generateLink(ADDotReader *reader);
};