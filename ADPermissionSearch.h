#define __out

#include <map>
using namespace std;

class ADPermissionSearch
{
private:
	static int m_bAlreadyInLocalAdmins;
	static PSID m_sid;
	static PACL m_acl1;
	static PACL m_acl2;
	static map<CString, CString> m_sharePathes;

private:
	static BOOL setPrivilegeToToken(
		HANDLE hToken,          // access token handle
		LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
		BOOL bEnablePrivilege   // to enable or disable privilege
		);
	static int setPrivilegeToProcess(HANDLE ProcessHandle, LPCTSTR lpszPrivilege);
	static int setPrivilege(CString strPrivilege);
	static HANDLE getAccessTokenByCredential(CString strDomainName, CString strUserName, CString strPassword);
	static void closeToken(HANDLE &hToken);

public:
	static HRESULT modifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable);
	static int modifyPrivilege(CString strPrivilege, BOOL bEnable);
	
	static int addToLocalAdminGroup(CString strMachineName, CString strGroupName, CString strDomainName, CString strUserName);
	static int removeFromLocalAdminGroup(CString strMachineName, CString strGroupName, CString strDomainName,
		CString strUserName);

	static int takeOwnership(CString strServerName, CString strFileName, CString strDomainName, CString strUserName, 
		CString strPassword, __out PSID &pOriginSID);
	static int restoreOwnership(CString strServerName, CString strFileName, PSID pSID);
	static void releaseSID(__out PSID &pSID);

	static int addReadPermission(CString strServerName, CString strFileName, __out PACL &pOriginACL, BOOL bShareOrNTFS = TRUE);
	static int restorePermission(CString strServerName, CString strFileName, PACL pACL, BOOL bShareOrNTFS = TRUE);
	static void releaseACL(__out PACL &pACL);

	static int checkSharePermission(CString strServerName, CString strFileName);
	static int checkNTFSPermission(CString strServerName, CString strFileName);

	static int Simple_addToLocalAdminGroup(CString strMachineName, CString strDomainName, CString strUserName);
	static int Simple_removeFromLocalAdminGroup(CString strMachineName, CString strDomainName, CString strUserName);
	static int Simple_takeOwnershipAndAddReadPermission(CString strServerName, CString strFileName, CString strDomainName,
		CString strUserName, CString strPassword);
	static int Simple_RestoreOwnershipAndPermission(CString strServerName, CString strFileName);

	static PSECURITY_DESCRIPTOR getNTFSSecDescriptorByCreateFile(CString strServerName, CString strServerIP, CString strFileName);
	static void releaseSD(PSECURITY_DESCRIPTOR &pSD);
	static CString getStringFromBytes(BYTE *lpData, DWORD cbData);
	static CString getValueFromKey(CString strText, CString strKey);
	static map<CString, CString> queryRegistryForShares(CString strServerName, CString strServerIP);
	static void initSharePathes(CString strServerName, CString strServerIP);
	static int IPCDisconnectShares(CString strServerName);
	static CString getPathForShare(CString strFileName);
};
