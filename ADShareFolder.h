#include "StdAfx.h"
#include <vector>
using namespace std;

#include "ADObject.h"
class ADACE;
class ADDomain;
class ADDotReader;
class CMyShape;

class ADShareFolder : public ADObject
{
	DECLARE_SERIAL(ADShareFolder)
public:
	CString cn;
	CString dn;
	ADDomain* domain;
	vector<ADACE*> ntfsPermissions;
	vector<ADACE*> sharePermissions;
	vector<ADACE*> finalPermissions;

	CPoint ptGUI;
	CMyShape *shape;
	
public:
	ADShareFolder();
	void Serialize(CArchive &ar);
	ADShareFolder(CString strRemoteName, ADDomain* a_domain);
	CString remoteName2DNName(CString strRemoteName);
	void generateFinalPermissions();
	void generateFinalPermissions2();
	void generateFinalPermissions3();
	void clarifyPermissionTypes();
	void clarifyAllGroupsAndUsers();
	void clarifyAllGroupsAndUsers2();
	void deleteNTFSPermission(int iIndex);
	void deleteSharePermission(int iIndex);
	void connectFolderWithACEs();

	CString Node2String();
	void parseGUIData(ADDotReader *reader);
	CString Edge2String();
	CString Edge2String2();
	void generateLink(ADDotReader *reader);
	void addToProperties();
};