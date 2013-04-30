#include "StdAfx.h"

#include "ADPermissionSearch.h"
#include "ADDirectorySearch.h"

#include <windows.h>
#include <stdio.h>
#include <lm.h> //addToLocalAdminGroup used

#include <accctrl.h> //takeOwnership & addReadPermission used
#include <aclapi.h> //takeOwnership & addReadPermission used

//#pragma comment(lib, "cmcfg32.lib") //setPrivilege used

int ADPermissionSearch::m_bAlreadyInLocalAdmins = FALSE;
PSID ADPermissionSearch::m_sid = NULL;
PACL ADPermissionSearch::m_acl1 = NULL;
PACL ADPermissionSearch::m_acl2 = NULL;
map<CString, CString> ADPermissionSearch::m_sharePathes = map<CString, CString>();

BOOL ADPermissionSearch::setPrivilegeToToken(
				  HANDLE hToken,          // access token handle
				  LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
				  BOOL bEnablePrivilege   // to enable or disable privilege
				  ) 
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
	
    if ( !LookupPrivilegeValue( 
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid ) )        // receives LUID of privilege
    {
        //printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
		MyMessageBox_Error(_T("setPrivilege"));
        return FALSE; 
    }
	
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;
	
    // Enable the privilege or disable all privileges.
	
    if ( !AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL) )
    { 
		//printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
		MyMessageBox_Error(_T("setPrivilege"));
		return FALSE; 
    } 
	
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		
    {
		//printf("The token does not have the specified privilege. \n");
		MyMessageBox_Error(_T("setPrivilege"));
		return FALSE;
    } 
	
    return TRUE;
}

int ADPermissionSearch::setPrivilegeToProcess(HANDLE ProcessHandle, LPCTSTR lpszPrivilege)
{
	TOKEN_PRIVILEGES TPrivileges;
	LUID luid;
	DWORD lasterror;
	
	if (LookupPrivilegeValue(NULL, lpszPrivilege, &luid) == 0) {
		lasterror = GetLastError();
		printf("Error: LookupPrivilegeValue() - %lu.\n", lasterror); 
		return 1;
	}
	
	TPrivileges.PrivilegeCount = 1;
	TPrivileges.Privileges[0].Luid = luid;
	TPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if (AdjustTokenPrivileges(ProcessHandle, FALSE, &TPrivileges, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL) == 0) { 
		lasterror = GetLastError();
		printf("Error: AdjustTokenPrivileges() - %lu.\n", lasterror); 
		return 1;
	} 
	
	lasterror = GetLastError();
	if (lasterror == ERROR_NOT_ALL_ASSIGNED) {
		printf("Error: AdjustTokenPrivilege().\n");
		return 1;
	}
	
	return 0;
}

int ADPermissionSearch::setPrivilege(CString strPrivilege)
{
	HANDLE ProcessHandle = NULL;
	int iResult;
	
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessHandle))
	{
		printf("OpenProcessToken failed.\n");
		return 0;
	}
	
	iResult = setPrivilegeToProcess(ProcessHandle, SE_RESTORE_NAME); //SE_BACKUP_NAME SE_SECURITY_NAME
	CloseHandle(ProcessHandle);
	return iResult;
}

HRESULT ADPermissionSearch::modifyPrivilege(LPCTSTR szPrivilege, BOOL fEnable)
{
    HRESULT hr = S_OK;
    TOKEN_PRIVILEGES NewState;
    LUID             luid;
    HANDLE hToken    = NULL;
	
    // Open the process token for this process.
    if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ))
    {
        printf("Failed OpenProcessToken\n");
        return ERROR_FUNCTION_FAILED;
    }
	
    // Get the local unique ID for the privilege.
    if ( !LookupPrivilegeValue( NULL,
		szPrivilege,
		&luid ))
    {
        CloseHandle( hToken );
        printf("Failed LookupPrivilegeValue\n");
        return ERROR_FUNCTION_FAILED;
    }
	
    // Assign values to the TOKEN_PRIVILEGE structure.
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Luid = luid;
    NewState.Privileges[0].Attributes = 
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);
	
    // Adjust the token privilege.
    if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		0,
		NULL,
		NULL))
    {
        printf("Failed AdjustTokenPrivileges\n");
        hr = ERROR_FUNCTION_FAILED;
    }
	
    // Close the handle.
    CloseHandle(hToken);
	
    return hr;
}

int ADPermissionSearch::modifyPrivilege(CString strPrivilege, BOOL bEnable)
{
	HRESULT hr;
	_bstr_t bstrPrivilege = _bstr_t(strPrivilege);
    hr = modifyPrivilege(bstrPrivilege, bEnable);
	
    if (!SUCCEEDED(hr))
	{
		return 0;
	}
    else
	{
		return 1;
	}
}

HANDLE ADPermissionSearch::getAccessTokenByCredential(CString strDomainName, CString strUserName, CString strPassword)
{
	CString strUPNUserName = strUserName + _T("@") + strDomainName;
	
	HANDLE hToken;
	BOOL bResult;
	//bResult = LogonUser(strUserName, strDomainName, strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT,
	//	&hToken);
	if (strDomainName != _T(""))
	{
		bResult = LogonUser(strUPNUserName, _T(""), strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT, 
			&hToken);
	}
	else
	{
		bResult = LogonUser(strUserName, _T("."), strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT, 
			&hToken);
	}
	if (bResult == FALSE)
	{
		MyMessageBox_Error(_T("getAccessTokenByCredential Error."), _T("Error"));
		return FALSE;
	}
	else
	{
		return hToken;
	}
}

void ADPermissionSearch::closeToken(HANDLE &hToken)
{
	if (hToken)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}
}

int ADPermissionSearch::addToLocalAdminGroup(CString strMachineName, CString strGroupName, CString strDomainName,
											 CString strUserName)
{
	CString strMachineNameWithSlash = _T("\\\\") + strMachineName;
    CString strFullUserName = strDomainName + _T("\\") + strUserName;
    _bstr_t bstrFullUserName = _bstr_t(strFullUserName);

    LOCALGROUP_MEMBERS_INFO_3 lgmi3;
    DWORD dwLevel = 3;
    DWORD dwTotalEntries = 1;
    NET_API_STATUS nStatus;

    // Set up the LOCALGROUP_MEMBERS_INFO_3 structure.
    // Assign the member account name in form of DomainName\AccountName
    lgmi3.lgrmi3_domainandname = bstrFullUserName;
    // Call the NetLocalGroupAddMembers() function, specifying level 3. Level 0 can use SID
    nStatus = NetLocalGroupAddMembers(_bstr_t(strMachineNameWithSlash),
                                      _bstr_t(strGroupName),
                                      dwLevel,
                                      (LPBYTE) &lgmi3,
                                      dwTotalEntries);
	//Athrun机器调用出错 Error:1789 ERROR_TRUSTED_RELATIONSHIP_FAILURE

    // If the call succeeds, inform the user.
    if (nStatus == ERROR_MEMBER_IN_ALIAS) {
        return 2;
    }
    if (nStatus == NERR_Success) {
        //fwprintf(stderr, L"%s has been added successfully to %s group on %s machine.\n", argv[3], argv[2], argv[1]);
        return 1;
    }
    // Otherwise, print the system error.
    else {
        //fwprintf(stderr, L"A system error has occurred: %d\n", nStatus);
        //MyMessageBox_Error(_T("addToLocalAdminGroup"));
		//Windows 2000的机器调用NetLocalGroupAddMembers会返回ERROR_ACCESS_DENIED
        return 0;
    }
}

int ADPermissionSearch::removeFromLocalAdminGroup(CString strMachineName, CString strGroupName, CString strDomainName,
												  CString strUserName)
{
    CString strFullUserName = strDomainName + _T("\\") + strUserName;
    _bstr_t bstrFullUserName = _bstr_t(strFullUserName);

    LOCALGROUP_MEMBERS_INFO_3 lgmi3;
    DWORD dwLevel = 3;
    DWORD dwTotalEntries = 1;
    NET_API_STATUS nStatus;

    // Set up the LOCALGROUP_MEMBERS_INFO_3 structure.
    // Assign the member account name in form of DomainName\AccountName
    lgmi3.lgrmi3_domainandname = bstrFullUserName;
    // Call the NetLocalGroupAddMembers() function, specifying level 3. Level 0 can use SID
    nStatus = NetLocalGroupDelMembers(_bstr_t(strMachineName),
                                      _bstr_t(strGroupName),
                                      dwLevel,
                                      (LPBYTE) &lgmi3,
                                      dwTotalEntries);
    // If the call succeeds, inform the user.
    if (nStatus == ERROR_MEMBER_NOT_IN_ALIAS) {
        return 2;
    }
    if (nStatus == NERR_Success) {
        //fwprintf(stderr, L"%s has been added successfully to %s group on %s machine.\n", argv[3], argv[2], argv[1]);
        return 1;
    }
    // Otherwise, print the system error.
    else {
        //fwprintf(stderr, L"A system error has occurred: %d\n", nStatus);
        MyMessageBox_Error(_T("removeFromLocalAdminGroup"));
        return 0;
    }
}

int ADPermissionSearch::takeOwnership(CString strServerName, CString strFileName, CString strDomainName, CString strUserName, 
									  CString strPassword, __out PSID &pOriginSID)
{
	CString strUNCFileName = _T("\\\\") + strServerName + _T("\\") + strFileName;
	_bstr_t bstrUNCFileName = _bstr_t(strUNCFileName);

	PSID pSIDAdmin = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    HANDLE hToken = NULL;
    DWORD dwRes;

	// Create a SID for the BUILTIN\Administrators group.
    if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSIDAdmin))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

    // If the preceding call failed because access was denied,
    // enable the SE_TAKE_OWNERSHIP_NAME privilege, create a SID for
    // the Administrators group, take ownership of the object, and
    // disable the privilege. Then try again to set the object's DACL.

    // Open a handle to the access token for the calling process.
	/*
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES,
                          &hToken))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }
	*/
	if ((hToken = getAccessTokenByCredential(strDomainName, strUserName, strPassword)) == NULL)
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

    // Enable the SE_TAKE_OWNERSHIP_NAME privilege.
    if (!setPrivilegeToToken(hToken, SE_TAKE_OWNERSHIP_NAME, TRUE))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

	// Get the original owner in the object's security descriptor.
    dwRes = GetNamedSecurityInfo(
		bstrUNCFileName,             // name of the object
		SE_LMSHARE,                  // type of object
		OWNER_SECURITY_INFORMATION,  // change only the object's owner
		&pOriginSID,                 // SID of Administrator group
		NULL,
		NULL,
		NULL,
		NULL);
    if (dwRes != ERROR_SUCCESS)
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

    // Set the owner in the object's security descriptor.
    dwRes = SetNamedSecurityInfo(
                bstrUNCFileName,             // name of the object
                SE_LMSHARE,                  // type of object
                OWNER_SECURITY_INFORMATION,  // change only the object's owner
                pSIDAdmin,                   // SID of Administrator group
                NULL,
                NULL,
                NULL);
    if (dwRes != ERROR_SUCCESS)
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

    // Disable the SE_TAKE_OWNERSHIP_NAME privilege.
    if (!setPrivilegeToToken(hToken, SE_TAKE_OWNERSHIP_NAME, FALSE))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("takeOwnership"));
		return 0;
    }

	return 1;
}

int ADPermissionSearch::restoreOwnership(CString strServerName, CString strFileName, PSID pSID)
{
	CString strUNCFileName = _T("\\\\") + strServerName + _T("\\") + strFileName;
	_bstr_t bstrUNCFileName = _bstr_t(strUNCFileName);

    HANDLE hToken = NULL;
    DWORD dwRes;


    // If the preceding call failed because access was denied,
    // enable the SE_TAKE_OWNERSHIP_NAME privilege, create a SID for
    // the Administrators group, take ownership of the object, and
    // disable the privilege. Then try again to set the object's DACL.

    // Open a handle to the access token for the calling process.
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES,
                          &hToken))
	{
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("restoreOwnership"));
		return 0;
    }

    // Enable the SE_TAKE_OWNERSHIP_NAME privilege.
    if (!setPrivilegeToToken(hToken, SE_TAKE_OWNERSHIP_NAME, TRUE))
	{
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("restoreOwnership"));
		return 0;
    }

    // Set the owner in the object's security descriptor.
    dwRes = SetNamedSecurityInfo(
                bstrUNCFileName,             // name of the object
                SE_LMSHARE,                  // type of object
                OWNER_SECURITY_INFORMATION,  // change only the object's owner
                pSID,                        // SID of Administrator group
                NULL,
                NULL,
                NULL);
    if (dwRes != ERROR_SUCCESS)
	{
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("restoreOwnership"));
		return 0;
    }

    // Disable the SE_TAKE_OWNERSHIP_NAME privilege.
    if (!setPrivilegeToToken(hToken, SE_TAKE_OWNERSHIP_NAME, FALSE))
	{
		if (hToken)
			CloseHandle(hToken);
		MyMessageBox_Error(_T("restoreOwnership"));
		return 0;
    }

	return 1;
}

void ADPermissionSearch::releaseSID(__out PSID &pSID)
{
	if (pSID)
	{
		FreeSid(pSID);
		pSID = NULL;
	}
}

int ADPermissionSearch::addReadPermission(CString strServerName, CString strFileName, __out PACL &pOriginACL, BOOL bShareOrNTFS)
{
	CString strUNCFileName = _T("\\\\") + strServerName + _T("\\") + strFileName;
	_bstr_t bstrUNCFileName = _bstr_t(strUNCFileName);
	SE_OBJECT_TYPE iObjectType = (bShareOrNTFS == TRUE)? SE_LMSHARE : SE_FILE_OBJECT;

    PSID pSIDAdmin = NULL;
    PACL pACL = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    const int NUM_ACES = 1;
    EXPLICIT_ACCESS ea[NUM_ACES];
    DWORD dwRes;
	
    // Create a SID for the BUILTIN\Administrators group.
    if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSIDAdmin))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		MyMessageBox_Error(_T("addReadPermission"));
		return 0;
    }
	
    ZeroMemory(&ea, NUM_ACES * sizeof(EXPLICIT_ACCESS));
	
    // Set read access for Everyone.
	/*
    ea[0].grfAccessPermissions = GENERIC_ALL;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR) pSIDEveryone;
	*/
	
    // Set full control for Administrators.
    ea[0].grfAccessPermissions = GENERIC_ALL;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR) pSIDAdmin;
	
    if (ERROR_SUCCESS != SetEntriesInAcl(NUM_ACES,
		ea,
		NULL,
		&pACL))
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		MyMessageBox_Error(_T("addReadPermission"));
		return 0;
    }

	// Get the object's original DACL.
    dwRes = GetNamedSecurityInfo(
		bstrUNCFileName,             // name of the object
		iObjectType,                 // type of object, SE_LMSHARE or SE_FILE_OBJECT
		DACL_SECURITY_INFORMATION,   // change only the object's DACL
		NULL, NULL,                  // do not change owner or group
		&pOriginACL,                 // DACL specified
		NULL,                        // do not change SACL
		NULL);

	if (dwRes == ERROR_SUCCESS)
	{
		goto Label_out;//return 1;
    }
    else if (dwRes != ERROR_ACCESS_DENIED)
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		MyMessageBox_Error(_T("addReadPermission"));
		return 0;
    }
	else
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		return 5; //ERROR_ACCESS_DENIED
	}
Label_out:;
	// Try to modify the object's DACL.
    dwRes = SetNamedSecurityInfo(
		bstrUNCFileName,             // name of the object
		iObjectType,                 // type of object, SE_LMSHARE or SE_FILE_OBJECT
		DACL_SECURITY_INFORMATION,   // change only the object's DACL
		NULL, NULL,                  // do not change owner or group
		pACL,                        // DACL specified
		NULL);                       // do not change SACL
	
    if (dwRes == ERROR_SUCCESS)
	{
		return 1;
    }
    else if (dwRes != ERROR_ACCESS_DENIED)
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		MyMessageBox_Error(_T("addReadPermission"));
		return 0;
    }
	else
	{
		if (pSIDAdmin)
			FreeSid(pSIDAdmin); 
		if (pACL)
			LocalFree(pACL);
		return 5; //ERROR_ACCESS_DENIED
	}
}

int ADPermissionSearch::restorePermission(CString strServerName, CString strFileName, PACL pACL, BOOL bShareOrNTFS)
{
	CString strUNCFileName = _T("\\\\") + strServerName + _T("\\") + strFileName;
	_bstr_t bstrUNCFileName = _bstr_t(strUNCFileName);
	SE_OBJECT_TYPE iObjectType = (bShareOrNTFS == TRUE)? SE_LMSHARE : SE_FILE_OBJECT;

    DWORD dwRes;

	// Try to modify the object's DACL.
    dwRes = SetNamedSecurityInfo(
		bstrUNCFileName,             // name of the object
		iObjectType,                 // type of object, SE_LMSHARE or SE_FILE_OBJECT
		DACL_SECURITY_INFORMATION,   // change only the object's DACL
		NULL, NULL,                  // do not change owner or group
		pACL,                        // DACL specified
		NULL);                       // do not change SACL
	
    if (dwRes == ERROR_SUCCESS)
	{
		return 1;
    }
    else if (dwRes != ERROR_ACCESS_DENIED)
	{
		MyMessageBox_Error(_T("restorePermission"));
		return 0;
    }
	else
	{
		MyMessageBox_Error(_T("restorePermission"));
		return 5; //ERROR_ACCESS_DENIED
	}
}

void ADPermissionSearch::releaseACL(PACL &pACL)
{
	if (pACL)
	{
		LocalFree(pACL);
		pACL = NULL;
	}
}

int ADPermissionSearch::checkSharePermission(CString strServerName, CString strFileName)
{
	PSECURITY_DESCRIPTOR pSecDescriptorBuf = NULL;
	BYTE* byNeedToRelease;
	pSecDescriptorBuf = ADDirectorySearch::getShareSecDescriptor(strFileName, strServerName, "", byNeedToRelease); //akisn0w
	if (pSecDescriptorBuf)
	{
		NetApiBufferFree(byNeedToRelease);
		return 1;
	}
	else
	{
		return 0;
	}
}

int ADPermissionSearch::checkNTFSPermission(CString strServerName, CString strFileName)
{
	PSECURITY_DESCRIPTOR pSecDescriptorBuf = NULL;
	pSecDescriptorBuf = ADDirectorySearch::getNTFSSecDescriptor(strFileName, strServerName, _T(""));
	if (pSecDescriptorBuf)
	{
		delete[] pSecDescriptorBuf;
		return 1;
	}
	else
	{
		return 0;
	}
}

int ADPermissionSearch::Simple_addToLocalAdminGroup(CString strMachineName, CString strDomainName, CString strUserName)
{
	//return 1;
	int iResult = addToLocalAdminGroup(strMachineName, _T("Administrators"), strDomainName, strUserName);
	if (iResult == 2)
	{
		m_bAlreadyInLocalAdmins = TRUE;
	}
	else
	{
		m_bAlreadyInLocalAdmins = FALSE;
	}
	return iResult;
}

int ADPermissionSearch::Simple_removeFromLocalAdminGroup(CString strMachineName, CString strDomainName, CString strUserName)
{
	if (m_bAlreadyInLocalAdmins)
	{
		return 1;
	}
	else
	{
		return removeFromLocalAdminGroup(strMachineName, _T("Administrators"), strDomainName, strUserName);
	}
}

int ADPermissionSearch::Simple_takeOwnershipAndAddReadPermission(CString strServerName, CString strFileName, 
																 CString strDomainName, CString strUserName,
																 CString strPassword)
{
	return 1;
	releaseSID(m_sid);
	releaseACL(m_acl1);
	releaseACL(m_acl2);
	takeOwnership(strServerName, strFileName, strDomainName, strUserName, strPassword, m_sid); //?
	addReadPermission(strServerName, strFileName, m_acl1, TRUE);
	addReadPermission(strServerName, strFileName, m_acl2, FALSE); //?
	return 1;
}

int ADPermissionSearch::Simple_RestoreOwnershipAndPermission(CString strServerName, CString strFileName)
{
	if (m_acl1)
	{
		restorePermission(strServerName, strFileName, m_acl1, TRUE);
		releaseACL(m_acl1);
	}
	if (m_acl2)
	{
		restorePermission(strServerName, strFileName, m_acl2, FALSE);
		releaseACL(m_acl2);
	}
	if (m_sid)
	{
		restoreOwnership(strServerName, strFileName, m_sid);
		releaseSID(m_sid);
	}
	
	return 1;
}

	/*
	CString strServerName = _T("athrun");
	CString strFileName = _T("no one can read me haha");

    ADDirectorySearch::IPCConnect("athrun", "tech", "administrator", "123456Ad");

	ADPermissionSearch::checkSharePermission(strServerName, strFileName);
	ADPermissionSearch::checkNTFSPermission(strServerName, strFileName);

	PSID pSID = NULL;
	PACL pACL = NULL;
	ADPermissionSearch::takeOwnership(strServerName, strFileName, pSID);
	ADPermissionSearch::addReadPermission(strServerName, strFileName, pACL, TRUE);
	ADPermissionSearch::addReadPermission(strServerName, strFileName, pACL, FALSE);

	ADPermissionSearch::checkSharePermission(strServerName, strFileName);
	ADPermissionSearch::checkNTFSPermission(strServerName, strFileName);

	ADPermissionSearch::restorePermission(strServerName, strFileName, pACL, TRUE);
	ADPermissionSearch::restorePermission(strServerName, strFileName, pACL, FALSE);
	ADPermissionSearch::restoreOwnership(strServerName, strFileName, pSID);

	ADPermissionSearch::releaseSID(pSID);
	ADPermissionSearch::releaseACL(pACL);

	ADDirectorySearch::IPCDisconnect("athrun");
	*/

/*
ADPermissionSearch::Simple_addToLocalAdminGroup("athrun", "tech", "administrator");
ADPermissionSearch::Simple_takeOwnershipAndAddReadPermission(strServerName, strFileName);

ADPermissionSearch::checkSharePermission(strServerName, strFileName);
ADPermissionSearch::checkNTFSPermission(strServerName, strFileName);

ADPermissionSearch::Simple_RestoreOwnershipAndPermission(strServerName, strFileName);
ADPermissionSearch::Simple_removeFromLocalAdminGroup("athrun", "tech", "administrator");
*/


PSECURITY_DESCRIPTOR ADPermissionSearch::getNTFSSecDescriptorByCreateFile(CString strServerName, CString strServerIP, CString strFileName)
{
	//CString strUNCFileName = _T("\\\\") + strServerName + _T("\\") + strFileName;
	//_bstr_t bstrUNCFileName = _bstr_t(strUNCFileName);
	int iTryTime = 4;

	CString strPathFileName = strFileName;
	for (int i = 0; i < strPathFileName.GetLength(); i ++)
	{
		if (strPathFileName.GetAt(i) == _T(':'))
		{
			strPathFileName.SetAt(i, _T('$'));
		}
	}
	CString strUNCPathFileName = _T("\\\\") + strServerName + _T("\\") + strPathFileName;
	CString strUNCPathFileName2 = _T("\\\\") + strServerIP + _T("\\") + strPathFileName;
	_bstr_t bstrUNCPathFileName = _bstr_t(strUNCPathFileName);

XX:	HANDLE hFolder = CreateFile(bstrUNCPathFileName, GENERIC_READ | ACCESS_SYSTEM_SECURITY, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hFolder != INVALID_HANDLE_VALUE)
	{
		PSECURITY_DESCRIPTOR pSD = NULL;
		DWORD dwSDLen;
		if (!GetKernelObjectSecurity(hFolder, DACL_SECURITY_INFORMATION, pSD, 0, &dwSDLen))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				pSD = (PSECURITY_DESCRIPTOR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSDLen);
				if (pSD == NULL)
				{
					CloseHandle(hFolder);
					MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
					return NULL;
				}
				
				//再次调用才正确得到安全描述符pSD
				if (!GetKernelObjectSecurity(hFolder, DACL_SECURITY_INFORMATION, pSD, dwSDLen, &dwSDLen))
				{
					HeapFree(GetProcessHeap(), 0, pSD);
					CloseHandle(hFolder);
					MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
					return NULL;
				}
				else
				{
					CloseHandle(hFolder);
					return pSD;
				}
			}
			else
			{
				CloseHandle(hFolder);
				MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
				return NULL;
			}
		}
		else
		{
			CloseHandle(hFolder);
			MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
			return NULL;
		}
	}
	else
	{
		DWORD dwError = GetLastError();
		iTryTime --;
		if (iTryTime == 3)
		{
			_bstr_t bstrUNCPathFileName = _bstr_t(strUNCPathFileName2);
		}
		if (iTryTime > 0)
		{
			goto XX;
		}
		//akisn0w NO way !!!
		//MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
		return NULL;
	}

/*
		WIN32_STREAM_ID *pStreamID;
		DWORD dwBytesIn;
		DWORD dwBytesOut;
		VOID *pContext = NULL;

		dwBytesIn = sizeof(WIN32_STREAM_ID) + 10000;
		if ((pStreamID = (WIN32_STREAM_ID *) malloc(dwBytesIn)) == NULL)
		{
			MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
			return 0;
		}
		memset(pStreamID, 0, dwBytesIn);
		if (BackupRead(hFolder, (byte *) pStreamID, dwBytesIn, &dwBytesOut, FALSE, TRUE, &pContext) == 0)
		{
			MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
			return 0;
		}
		if (dwBytesOut >= dwBytesIn)
		{
			MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
			return 0;
		}

		if (pStreamID)
		{

		}

		BackupRead(NULL, NULL, NULL, NULL, TRUE, NULL, &pContext);
		free(pStreamID);
		CloseHandle(hFolder);
		return 1;
	}
	else
	{
		DWORD dwError = GetLastError();
		MyMessageBox_Error(_T("getNTFSSecDescriptorByCreateFile"));
		return 0;
	}
	*/
}

void ADPermissionSearch::releaseSD(PSECURITY_DESCRIPTOR &pSD)
{
	if (pSD)
	{
		HeapFree(GetProcessHeap(), 0, pSD);
		pSD = NULL;
	}
}

CString ADPermissionSearch::getStringFromBytes(BYTE *lpData, DWORD cbData)
{
	char *pszData = (char *) malloc((cbData + 2) * sizeof(char));
	memcpy(pszData, lpData, cbData);
	for (int i = 0; i < cbData; i ++)
	{
		if (lpData[i] == 0x00)
		{
			pszData[i] = ',';
		}
	}
	if (pszData[cbData - 1] == ',')
	{
		pszData[cbData - 1] = 0;
	}
	else
	{
		pszData[cbData] = 0;
	}

	CString strResult = pszData;
	free(pszData);
	return strResult;
}

CString ADPermissionSearch::getValueFromKey(CString strText, CString strKey)
{
	int iStart = strText.Find(strKey, 0);
	if (iStart == -1)
	{
		MyMessageBox_Error(_T("getValueFromKey"));
		return _T("");
	}

	int iEnd = strText.Find(_T(','), iStart);
	if (iEnd == -1)
	{
		MyMessageBox_Error(_T("getValueFromKey"));
		return _T("");
	}

	iStart += strKey.GetLength();
	if (iStart >= iEnd)
	{
		MyMessageBox_Error(_T("getValueFromKey"));
		return _T("");
	}
	CString strResult = strText.Mid(iStart, iEnd - iStart);
	return strResult;
}

map<CString, CString> ADPermissionSearch::queryRegistryForShares(CString strServerName, CString strServerIP)
{
	int iTryTime = 1;
	map<CString, CString> nSharePathes;

	CString strUNCServerName;
// 	if (strServerName != _T("queryRegistryForShares"))
// 	{
// 		strUNCServerName = _T("\\\\") + strServerName;
// 	}
// 	else
// 	{
// 		strUNCServerName = _T("queryRegistryForShares");
// 	}
	strUNCServerName = _T("\\\\") + strServerName;
	CString strUNCServerIP = _T("\\\\") + strServerIP;
	_bstr_t bstrUNCServerName = _bstr_t(strUNCServerName);

	HKEY lHKeyhandle;
	HKEY lhkey;
	LONG lRetVal;
XY:	lRetVal = RegConnectRegistry(bstrUNCServerName, HKEY_LOCAL_MACHINE, &lHKeyhandle);
	if (lRetVal != ERROR_SUCCESS)
	{
		iTryTime --;
		bstrUNCServerName = _bstr_t(strUNCServerIP);
		if (iTryTime >= 0)
		{
			goto XY;
		}
		MyMessageBox_Error(_T("queryRegistryForShares"));
		return nSharePathes;
	}

	lRetVal = RegOpenKeyEx(lHKeyhandle, _T("SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Shares"), 0, KEY_READ, &lhkey);
	if (lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(lHKeyhandle);
		MyMessageBox_Error(_T("queryRegistryForShares"));
		return nSharePathes;
	}

	DWORD dwMaxValueNameLen = 0;
	DWORD dwMaxValueLen = 0;
	lRetVal = RegQueryInfoKey(lhkey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueNameLen, &dwMaxValueLen,
		NULL, NULL);
	if (lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(lHKeyhandle);
		MyMessageBox_Error(_T("queryRegistryForShares"));
		return nSharePathes;
	}
	dwMaxValueNameLen += 10;
	dwMaxValueLen += 10;

	LONG rc;
	DWORD dwSubKeyIndex = 0;
	char *pszSubKey = (char *) malloc(dwMaxValueNameLen * sizeof(char)); //_MAX_FNAME
	DWORD dwSubKeyLength = dwMaxValueNameLen;
	BYTE *lpData = (BYTE *) malloc(dwMaxValueLen * sizeof(BYTE)); //_MAX_FNAME
	DWORD lpcbData = dwMaxValueLen;
	while ((rc = RegEnumValue(lhkey, dwSubKeyIndex, pszSubKey, &dwSubKeyLength, NULL, NULL, lpData, &lpcbData)) !=
		ERROR_NO_MORE_ITEMS)
	{
		if (rc == ERROR_SUCCESS)
		{
			CString strShareName = pszSubKey;
			CString strShareText = getStringFromBytes(lpData, lpcbData);
			CString strSharePath = getValueFromKey(strShareText, _T("Path="));
			nSharePathes.insert(map<CString, CString>::value_type(strShareName, strSharePath));
			//if (stricmp(szSubKey, "DefaultUserName") == 0)
			//{
			//	printf("%s", lpData);
			//}
			dwSubKeyIndex++;
			dwSubKeyLength = dwMaxValueNameLen;
			lpcbData = dwMaxValueLen;
		}
		else
		{
			free(pszSubKey);
			free(lpData);
			RegCloseKey(lhkey);
			RegCloseKey(lHKeyhandle);
			MyMessageBox_Error(_T("queryRegistryForShares"));
			return nSharePathes;
		}
	}

	free(pszSubKey);
	free(lpData);
	RegCloseKey(lhkey);
	RegCloseKey(lHKeyhandle);
	return nSharePathes;
}

void ADPermissionSearch::initSharePathes(CString strServerName, CString strServerIP)
{
	m_sharePathes.clear();
	m_sharePathes = queryRegistryForShares(strServerName, strServerIP);
}

int ADPermissionSearch::IPCDisconnectShares(CString strServerName)
{
	int iResult;
	map<CString, CString>::iterator it;
	for (it = m_sharePathes.begin(); it != m_sharePathes.end(); it ++)
	{
		CString strFileName = it->first;
		iResult = ADDirectorySearch::IPCDisconnectShare(strServerName, strFileName);
	}
	return iResult;
}

CString ADPermissionSearch::getPathForShare(CString strFileName)
{
	CString strPathFileName = m_sharePathes[strFileName];
	if (strPathFileName == _T(""))
	{
		MyMessageBox_Error(_T("getPathForShare"));
		return _T("");
	}
	else
	{
		return strPathFileName;
	}
}