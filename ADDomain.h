#include "StdAfx.h"
#include <vector>
using namespace std;

#include "ADObject.h"
class ADUser;
class ADGroup;
class ADComputer;
class ADOU;
class ADShareFolder;
class ADACE;
class ADEntry;
class ADEntryForTrustInfo;
class ADDotReader;
class CMyShape;

class ADDomain : public ADObject
{
	DECLARE_SERIAL(ADDomain)
public:
	CString dNSName;
	CString dnName;
	CString netBIOSName;
	CString forestName;
	CString siteName;
	CString dcName;
	CString dcDNSName;
	CString dcAddress;
	CString userName;
	CString passWord;
	vector<ADEntryForTrustInfo*> trustInfos;

	vector<ADDomain*> globalDomains;

	vector<ADUser*> users;
	vector<ADGroup*> groups;
	ADGroup* everyone;
	vector<ADComputer*> computers;
	vector<ADOU*> ous;
	
	vector<ADUser*> g_users;
	vector<ADGroup*> g_groups;
	vector<ADComputer*> g_computers;
	vector<ADOU*> g_ous;
	vector<ADShareFolder*> g_shareFolders;
	vector<ADACE*> g_ADACEs;
	
	CRect rectGUI;
	CMyShape *shape;

public:
	ADDomain();
	void Serialize(CArchive &ar);
	ADDomain(CString a_dcName, CString a_userName, CString a_passWord);
	~ADDomain();
	CString getDCNameFromDNSName(CString strDNSName);
	BOOL checkIP(CString strIP);
	int getDomainInfo(CString strDCName);
	void getTrustInfo();
	void getUsers();
	void getComputers();

	ADUser* addUser(CString strUserName, CString strDNName);
	ADGroup* addGroup(CString strGroupName, CString strDNName, ADDomain* domain, CString strType);
	ADComputer* addComputer(CString strComputerName, CString strDNName, ADDomain* domain);
	ADOU* addOU(CString strOUName, CString strDNName, ADDomain* domain);
	ADOU* findOU(CString strOUName, CString strDNName);

	void performEntry(ADEntry* entry);
	void performEntries(vector<ADEntry*> entries);
	void getAndPerformEntries();

	vector<ADUser*> getAllUsers();
	vector<ADGroup*> getAllGroups();
	vector<ADComputer*> getAllComputers();
	vector<ADOU*> getAllOUs();
	vector<ADShareFolder*> getAllShareFolders();
	void connectFolderWithACEs();
	vector<ADACE*> getAllACEs(int iMixOrShareOrNTFS);

	void refreshDCName();
	void refreshAllShareFolderInfos();
	void clarifyAllGroupsAndUsers();
	void updateComputerCNs();
	void addToProperties();

	ADGroup* findGlobalGroupByDN(CString strGroupDNName);
	ADUser* findGlobalUserByDN(CString strUserDNName);
	void refreshAllGroupMembers(BOOL bDisplay = TRUE);
	void refreshAllGroupMembers_LoadMode(BOOL bDisplay = TRUE);

	CString Node2String();
	CString Edge2String();
	void parseGUIData(ADDotReader *reader);
	void generateLink(ADDotReader *reader);
	void setConsoleText(CString strText);
	void setGlobalDomains(vector<ADDomain*> a_globalDomains);
};