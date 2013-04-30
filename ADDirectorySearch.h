#include "StdAfx.h"

#include <vector>
#include <map>
using namespace std;

class ADShareFolder;
class ADGroup;
class ADUser;
class ADDomain;

#define DNS_NAME 7//DS_CANONICAL_NAME
#define DN_NAME 1//DS_FQDN_1779_NAME
#define NETBIOS_NAME 2//DS_NT4_ACCOUNT_NAME

#define READ_PERMISSIONS (FILE_READ_DATA | FILE_READ_ATTRIBUTES)
#define WRITE_PERMISSIONS (FILE_WRITE_DATA | FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA)
#define EXECUTE_PERMISSIONS (FILE_READ_DATA | FILE_EXECUTE)

class ADEntry : public CObject
{
DECLARE_SERIAL(ADEntry)
public:
	CString cn;
	CString dn;
	vector<CString> dnKeys;
	vector<CString> dnValues;
	CString type;
	map<CString, CString> infos;

public:
	ADEntry();
	void Serialize(CArchive &ar);
	void setCN(CString a_cn);
	CString getOUDN();
	void setDN(CString a_dn);
	CString popKeyValue();
	void setType(CString a_type);
	void addKeyValue2Info(CString strKey, CString strValue);
};

class ADEntryForTrustInfo : public CObject
{
DECLARE_SERIAL(ADEntryForTrustInfo)
public:
	CString cn;
	CString dn;
	CString trustAttributes;
	CString trustDirection;
	CString trustType;

public:
	ADEntryForTrustInfo();
	void Serialize(CArchive &ar);
	void setCN(CString a_cn);
	void setDN(CString a_dn);
};

#define TYPE_NTFS 0
#define TYPE_SHARE 1
#define TYPE_MIX 2

class ADACE : public CObject
{
	DECLARE_SERIAL(ADACE)
public:
	CString dn;
	int allowed;
	CString name;
	CString refDomain;
	ADGroup* group;
	ADUser* user;
	int r;
	int w;
	int x;
	int type; //0-NTFS, 1-Share
	ADShareFolder* fatherFolder;

public:
	ADACE();
	void Serialize(CArchive &ar);
	ADACE* duplicate();
	ADACE* duplicateToZero();
	CString toString();
};

class ADDirectorySearch
{
private:
	static CString m_strDomainName;
	static CString m_strUserName;
	static CString m_strPassword;
public:
	static string ws2s(const wstring& ws);
	static wstring s2ws(const string& s);
	static CString itos(int i);
	static int stoi(CString s);
	static LPWSTR CStringW2LPWSTR(CString cstr);
	static PSECURITY_DESCRIPTOR getNTFSSecDescriptor(CString strFileName, CString strServerName, CString strServerIP);
	static PSECURITY_DESCRIPTOR getNTFSSecDescriptor2(CString strFileName, CString strServerName, CString strServerIP);
	static PSECURITY_DESCRIPTOR getNTFSSecDescriptor3(CString strFileName, CString strServerName, CString strServerIP);
	static PSECURITY_DESCRIPTOR getShareSecDescriptor(CString strFileName, CString strServerName, CString strServerIP, BYTE* &byNeedToRelease);
	static PSECURITY_DESCRIPTOR getShareSecDescriptor2(CString strFileName, CString strServerName, CString strServerIP, BYTE* &byNeedToRelease);
public:
	static void ADDirectorySearch::setCredential(CString strDomainName, CString strUserName, CString strPassword);
	static vector<ADEntry*> getEntries(CString strPathName, CString strUserName, CString strPassWord,
		CString strFilter, CString strType);
	static vector<ADEntryForTrustInfo*> getEntriesForTrustInfo(CString strPathName, CString strUserName, CString strPassWord,
		CString strFilter, CString strType);
	static CString convertName(CString strName, CString strDCDNSName, CString strDomainName, CString strUserName, CString strPassWord, 
		int inTypeFlag, int outTypeFlag);
	static CString host2IP(CString strHostName);
	static CString host2IP2(CString strHostName);
	static CString ip2Host(CString strIP);
	static CString host2IP3(CString strHostName);
	static BOOL checkIP(CString strIP);
	static BOOL checkIPAlive(CString strIP);

	static vector<ADACE*> preprocessNTFSEntries(vector<ADACE*> entries);
	static BOOL IPCConnect(CString strServerName, CString strDomainName, CString strUserName, CString strPassWord);
	static BOOL IPCDisconnect(CString strServerName);
	static BOOL IPCDisconnectShare(CString strServerName, CString strFileName);
	static CString IPCConnectAsLocalUser(CString strServerName, CString strUserName, CString strPassWord, CString strServerIP = _T(""));
	static BOOL IPCDisconnectAsLocalUser(CString strServerName, CString strServerIP);

	static BOOL logOnByUserPassword(CString strDomainName, CString strUserName, CString strPassword);
	static BOOL logOff();

	static vector<ADACE*> getShareFolderInfo(CString strFileCNName, CString strServerName, CString strServerIP, CString strFlag, 
		CString strFileDNName);
	static vector<ADShareFolder*> listShareFolders(CString strServerName, CString strServerIP, ADDomain* domain);
	static vector<CString> getGroupMembers(CString strDCAddress, CString strGroupDNName, CString strUserName,
												   CString strPassWord);

	static CString dnNametoString(CString strIn);
	static CString lineFeedToString(CString strIn);

	static ADGroup* getGroupFromDomains(CString strDomainName, CString strCN, vector<ADDomain*> globalDomains);
	static ADUser* getUserFromDomains(CString strDomainName, CString strCN, vector<ADDomain*> globalDomains);

	static void getKeyValueFromLine(CString strLine, CString &strIP, CString &strHostname);
	static BOOL evaluateLine(CString strLine);
	static BOOL deleteFile(CString strFilePathName);
	static void addToHostFile(CString strIP, CString strHostname);
};