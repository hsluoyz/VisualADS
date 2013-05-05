#include "StdAfx.h"

#include "ADDomain.h"
#include "ADUser.h"
#include "ADGroup.h"
#include "ADComputer.h"
#include "ADOU.H"
#include "ADDirectorySearch.h"
#include "ADPermissionSearch.h"
#include "ADShareFolder.h"
#include "ADDotReader.h"
#include "MyShape.h"

#define _WIN32_WINNT 0x0500

#include <NtDsAPI.h>
#pragma comment(lib, "NtDsAPI.lib")

#include <DsGetDC.h>
#include <LM.h>
#pragma comment(lib, "netapi32.lib")

IMPLEMENT_SERIAL(ADDomain, ADObject, 0)

ADDomain::ADDomain()
{

}

void ADDomain::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		ar << dNSName << dnName << netBIOSName << forestName << siteName << dcName << dcDNSName << dcAddress << userName << passWord;

		ar << trustInfos.size();
		for (i = 0; i < trustInfos.size(); i ++)
		{
			trustInfos[i]->Serialize(ar);
		}

		//Intialization needed: globalDomains --> MainFrm.cpp [ok]

		ar << users.size();
		for (i = 0; i < users.size(); i ++)
		{
			users[i]->Serialize(ar);
		}

		ar << groups.size();
		for (i = 0; i < groups.size(); i ++)
		{
			groups[i]->Serialize(ar);
		}

		everyone->Serialize(ar);

		ar << computers.size();
		for (i = 0; i < computers.size(); i ++)
		{
			computers[i]->Serialize(ar);
		}

		ar << ous.size();
		for (i = 0; i < ous.size(); i ++)
		{
			ous[i]->Serialize(ar);
		}

		//Intialization needed: g_users, g_groups, g_computers, g_ous, g_shareFolders, g_ADACEs --> getAllXXX() [ok]
	}
	else
	{
		int i;
		int iSize;
		ar >> dNSName >> dnName >> netBIOSName >> forestName >> siteName >> dcName >> dcDNSName >> dcAddress >> userName >> passWord;

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADEntryForTrustInfo *pTrustInfo = new ADEntryForTrustInfo();
			pTrustInfo->Serialize(ar);
			trustInfos.push_back(pTrustInfo);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADUser *pUser = new ADUser();
			pUser->domain = this;
			pUser->Serialize(ar);
			users.push_back(pUser);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADGroup *pGroup = new ADGroup();
			pGroup->domain = this;
			pGroup->Serialize(ar);
			groups.push_back(pGroup);
		}

		ADGroup *pEveryoneGroup = new ADGroup();
		pEveryoneGroup->domain = this;
		pEveryoneGroup->Serialize(ar);
		everyone = pEveryoneGroup;

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADComputer *pComputer = new ADComputer();
			pComputer->domain = this;
			pComputer->Serialize(ar);
			computers.push_back(pComputer);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADOU *pOU = new ADOU();
			pOU->domain = this;
			pOU->Serialize(ar);
			ous.push_back(pOU);
		}

		//Intialization response: g_users, g_groups, g_computers, g_ous, g_shareFolders, g_ADACEs
		g_users = getAllUsers();
		g_groups = getAllGroups();
		g_computers = getAllComputers();
		g_ous = getAllOUs();
		g_shareFolders = getAllShareFolders();
		//g_ADACEs = getAllACEs(); //only used for DLInterface

		//Intialization response: fatherFolder
		connectFolderWithACEs();
	}

	ADObject::Serialize(ar);
}

ADDomain::ADDomain(CString a_dcName, CString a_userName, CString a_passWord)
{
	Output_ConsoleList(_T("Start parsing unknown domain with DC: ") + a_dcName + _T("..."));
	userName = a_userName;
	passWord = a_passWord;
	if (getDomainInfo(a_dcName) == 0)
	{
		dNSName = _T("");
		dnName = _T("");
		netBIOSName = _T("");
	}
	else
	{
		getTrustInfo();
		getAndPerformEntries();
		everyone = new ADGroup(_T("Everyone"), _T("CN=Everyone,") + dnName, this, _T("global"));

		g_users = getAllUsers();
		g_groups = getAllGroups();
		g_computers = getAllComputers();
		g_ous = getAllOUs();

		refreshDCName();
		refreshAllGroupMembers();
		//refreshAllShareFolderInfos();
	}
	Output_ConsoleList(_T("Suspend parsing domain: ") + dNSName + _T(" for a while..."));
	rectGUI = CRect(-1, -1, -1, -1);
}

ADDomain::~ADDomain()
{
	ADDirectorySearch::logOff();

	for (int i = 0; i < g_users.size(); i ++)
	{
		delete g_users[i];
	}
	g_users.clear();
	for (i = 0; i < g_groups.size(); i ++)
	{
		delete g_groups[i];
	}
	g_groups.clear();
	for (i = 0; i < g_computers.size(); i ++)
	{
		delete g_computers[i];
	}
	g_computers.clear();
	for (i = 0; i < g_ous.size(); i ++)
	{
		delete g_ous[i];
	}
	g_ous.clear();
}

CString ADDomain::getDCNameFromDNSName(CString strDNSName)
{
	int iDot = strDNSName.Find(_T("."), 0);
	if (iDot == -1)
	{
		return strDNSName;
	}
	else
	{
		return strDNSName.Mid(0, iDot);
	}
}



int ADDomain::getDomainInfo(CString strDCName)
{
	Output_ConsoleList(_T("Getting domain information for DC: ") + strDCName + _T("..."));
	dcName = strDCName;
	BOOL bIsIP = FALSE;
	if (ADDirectorySearch::checkIP(dcName))
	{
		bIsIP = TRUE;
	}

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		//列出域的基本信息
		//cout << "succeed 1" << endl;
		PDOMAIN_CONTROLLER_INFO psDCInfo;
		ULONG  uFlags = DS_RETURN_DNS_NAME;// | DS_GC_SERVER_REQUIRED;

		//CString aaa = ADDirectorySearch::ip2Host("192.168.1.179"); //参照dns和hosts进行反查
		CString strTempDCAddress;
		if (!bIsIP)
		{
			strTempDCAddress = ADDirectorySearch::host2IP3(dcName);
			if (strTempDCAddress == _T(""))
			{
				MyMessageBox_Error(_T("您输入的Windows域控制器名称") + dcName+ _T("不存在。"),_T("错误提示信息"));
				return 0;
			}
		}
		else
		{
			strTempDCAddress = dcName;
			BOOL bAlive = ADDirectorySearch::checkIPAlive(dcName);
			if (!bAlive)
			{
				MyMessageBox_Error(_T("您输入的Windows域控制器IP地址") + dcName + _T("不存在。"),_T("错误提示信息"));
				return 0;
			}
		}

		//NetQueryDisplayInformation()
		//CString strValidIPCName = ADDirectorySearch::IPCConnectAsLocalUser(dcName, userName, passWord, strTempDCAddress); //strong the method with ip address
		int iFlag = 0;
LL:		DWORD dwResult = DsGetDcName(dcName, NULL/*"domaintest.net"*/, NULL, NULL, uFlags, &psDCInfo);
		//goto LL2;
		if (dwResult != ERROR_SUCCESS)
		{
			dwResult = DsGetDcName(strTempDCAddress, NULL/*"domaintest.net"*/, NULL, NULL, uFlags, &psDCInfo);
			if (dwResult == ERROR_ACCESS_DENIED)
			{
				if (iFlag == 0)
				{
					//ADDirectorySearch::logOnByUserPassword(_T("tech.domaintest.net"), userName, passWord);
					BOOL bRet = ADDirectorySearch::IPCConnect(dcName, _T("tech"), userName, passWord);
					iFlag ++;
					goto LL;
				}
			}
		}
		if (dwResult == ERROR_SUCCESS)
		{
			//cout << "succeed 2" << endl;
			/*
			wprintf(L"DC: %s\n", psDCInfo->DomainControllerName);
			wprintf(L"Address: %s\n", psDCInfo->DomainControllerAddress);
			wprintf(L"Domain: %s\n", psDCInfo->DomainName);
			wprintf(L"Forest: %s\n", psDCInfo->DnsForestName);
			wprintf(L"Site: %s\n", psDCInfo->DcSiteName);
			*/

			dcDNSName = psDCInfo->DomainControllerName;
			dcDNSName.Delete(0, 2);
			if (bIsIP)
			{
				dcName = getDCNameFromDNSName(dcDNSName);
			}
			dcAddress = psDCInfo->DomainControllerAddress;
			dcAddress.Delete(0, 2);
			dNSName = psDCInfo->DomainName;
			forestName = psDCInfo->DnsForestName;
			siteName = psDCInfo->DcSiteName;
			Output_ConsoleList(_T("[dcDNSName]:") + dcDNSName + _T("  [dcAddress]:") + dcAddress);
			Output_ConsoleList(_T("[dnsName]:") + dNSName + _T("  [forestName]:") + forestName + _T("  [siteName]:") + siteName);
			NetApiBufferFree(psDCInfo);
			CoUninitialize();
		}
		else
		{
			MyMessageBox_Error(_T("getDomainInfo"));
			CoUninitialize();
			return 0;
		}

		ADDirectorySearch::addToHostFile(dcAddress, dcDNSName);
		Output_ConsoleList(_T("Adding (") + dcAddress + _T(", ") + dcDNSName + _T(") to host file..."));
		//ADDirectorySearch::logOnByUserPassword(dNSName, userName, passWord);
//LL2:	//dNSName = "tech.adtest.net";
		//dcDNSName = "kira.tech.adtest.net";
		//dcAddress = "192.168.1.201";
		//Output_ConsoleList(_T("[dcDNSName]:") + dcDNSName + _T("  [dcAddress]:") + dcAddress);
		//Output_ConsoleList(_T("[dnsName]:") + dNSName + _T("  [forestName]:") + forestName + _T("  [siteName]:") + siteName);
		dnName = ADDirectorySearch::convertName(dNSName + _T("/"), dcDNSName, dNSName, userName, passWord, DNS_NAME, DN_NAME);
		netBIOSName = ADDirectorySearch::convertName(dNSName + _T("/"), dcDNSName, dNSName, userName, passWord, DNS_NAME, NETBIOS_NAME);
		netBIOSName.Delete(netBIOSName.GetLength() - 1, 1);
		Output_ConsoleList(_T("[dnName]:") + dnName + _T("  [netBIOSName]:") + netBIOSName);
		if (dnName != _T(""))
		{
			return 1;
		}
		else
		{
			MyMessageBox_Error(_T("您输入的AD域管理员用户名或密码错误。"), _T("错误提示信息"));
			return 0;
		}

		ADDirectorySearch::IPCDisconnectAsLocalUser(dcName, strTempDCAddress);
	}
	else
	{
		MyMessageBox_Error(_T("getDomainInfo Error."),_T("Error"));
		return 0;
	}
}

void ADDomain::getTrustInfo()
{
	CString strPathName = _T("LDAP://");
	strPathName += dcAddress;
	strPathName += _T("/");
	strPathName += dnName;
	Output_ConsoleList(_T("Getting trust information for DC: ") + dcName + _T(" by accessing ") + strPathName + _T("..."));

	trustInfos = ADDirectorySearch::getEntriesForTrustInfo(strPathName,
		userName, passWord, _T("(& (objectClass=trustedDomain) (objectCategory=*) (name=*))"), _T("ou"));
}

void ADDomain::getUsers()
{
	
}

void ADDomain::getComputers()
{

}

ADUser* ADDomain::addUser(CString strUserName, CString strDNName)
{
	ADUser *user = new ADUser(strUserName, strDNName, this);
	users.push_back(user);
	return user;
}

ADGroup* ADDomain::addGroup(CString strGroupName, CString strDNName, ADDomain* domain, CString strType)
{
	ADGroup *group = new ADGroup(strGroupName, strDNName, domain, strType);
	groups.push_back(group);
	return group;
}

ADComputer* ADDomain::addComputer(CString strComputerName, CString strDNName, ADDomain* domain)
{
	ADComputer *computer = new ADComputer(strComputerName, strDNName, domain);
	computers.push_back(computer);
	return computer;
}

ADOU* ADDomain::addOU(CString strOUName, CString strDNName, ADDomain* domain)
{
	ADOU *ou = new ADOU(strOUName, strDNName, domain);
	ous.push_back(ou);
	return ou;
}

ADOU* ADDomain::findOU(CString strOUName, CString strDNName)
{
	ADOU* retOU = NULL;
	for (int i = 0; i < ous.size(); i ++)
	{
		if (strOUName == ous[i]->cn)
		{
			return ous[i];
		}
	}
	return addOU(strOUName, strDNName, this);
}

void ADDomain::performEntry(ADEntry* entry)
{
	if (entry->dnKeys.size() == 0)
	{
		return;
	}
	else if (entry->dnKeys[entry->dnKeys.size() - 1] == _T("OU") ||
		entry->dnValues[entry->dnValues.size() - 1] == _T("Computers") ||
		entry->dnValues[entry->dnValues.size() - 1] == _T("Users") ||
		entry->dnValues[entry->dnValues.size() - 1] == _T("Builtin"))
	{
		CString strLastValue = entry->popKeyValue();
		findOU(strLastValue, entry->getOUDN())->performEntry(entry);
	}
	else if (entry->type == _T("computer"))
	{
		ADComputer *computer = addComputer(entry->cn, entry->dn, this);
		computer->infos = entry->infos;
	}
	else if (entry->type == _T("user"))
	{
		ADUser *user = addUser(entry->cn, entry->dn);
		user->infos = entry->infos;
	}
	else if (entry->type == _T("localgroup"))
	{
		ADGroup *group = addGroup(entry->cn, entry->dn, this, _T("local"));
		group->infos = entry->infos;
	}
	else if (entry->type == _T("globalgroup"))
	{
		ADGroup *group = addGroup(entry->cn, entry->dn, this, _T("global"));
		group->infos = entry->infos;
	}
	else
	{
		MyMessageBox_Error(_T("performEntry Error"), _T("Error"));
	}
}

void ADDomain::performEntries(vector<ADEntry*> entries)
{
	for (int i = 0; i < entries.size(); i ++)
	{
		performEntry(entries[i]);
	}
}

void ADDomain::getAndPerformEntries()
{
	CString strPathName = _T("LDAP://");
	strPathName += dcAddress;
	strPathName += _T("/");
	strPathName += dnName;
	Output_ConsoleList(_T("Getting and performing entries for DC: ") + dcName + _T(" by accessing ") + strPathName + _T("..."));

	Output_ConsoleList(_T("Querying OU entries..."));
	vector<ADEntry*> entriesOU = ADDirectorySearch::getEntries(strPathName,
		userName, passWord, _T("(& (objectClass=organizationalunit) (objectCategory=*) (name=*))"), _T("ou"));
	performEntries(entriesOU);

	Output_ConsoleList(_T("Querying Container entries..."));
	vector<ADEntry*> entriesContainer = ADDirectorySearch::getEntries(strPathName,
		userName, passWord, _T("(& (objectCategory=container) (| (name=Computers) (name=Users)))"), _T("container"));
	performEntries(entriesContainer);

	Output_ConsoleList(_T("Querying Computer entries..."));
	vector<ADEntry*> entriesComputer = ADDirectorySearch::getEntries(strPathName,
		userName, passWord, _T("(& (objectClass=*) (objectCategory=computer) (name=*))"), _T("computer"));
	performEntries(entriesComputer);
	Output_ConsoleList(_T("Computer entries finished performing."));

	Output_ConsoleList(_T("Querying User entries..."));
	vector<ADEntry*> entriesUser = ADDirectorySearch::getEntries(strPathName,
		userName, passWord, _T("(& (objectClass=user) (objectCategory=person) (name=*))"), _T("user"));
	performEntries(entriesUser);

	Output_ConsoleList(_T("Querying Group entries..."));
	vector<ADEntry*> entriesGroup = ADDirectorySearch::getEntries(strPathName,
		userName, passWord, _T("(& (objectClass=group) (objectCategory=group) (name=*))"), _T("group"));
	performEntries(entriesGroup);
}

vector<ADUser*> ADDomain::getAllUsers()
{
	vector<ADUser*> resultUsers;
	resultUsers.insert(resultUsers.end(), users.begin(), users.end());
	for (int i = 0; i < ous.size(); i ++)
	{
		vector<ADUser*> temp = ous[i]->getAllUsers();
		resultUsers.insert(resultUsers.end(), temp.begin(), temp.end());
	}
	return resultUsers;
}

vector<ADGroup*> ADDomain::getAllGroups()
{
	vector<ADGroup*> resultGroups;
	resultGroups.insert(resultGroups.end(), groups.begin(), groups.end());
	for (int i = 0; i < ous.size(); i ++)
	{
		vector<ADGroup*> temp = ous[i]->getAllGroups();
		resultGroups.insert(resultGroups.end(), temp.begin(), temp.end());
	}
	return resultGroups;
}

vector<ADComputer*> ADDomain::getAllComputers()
{
	vector<ADComputer*> resultComputers;
	resultComputers.insert(resultComputers.end(), computers.begin(), computers.end());
	for (int i = 0; i < ous.size(); i ++)
	{
		vector<ADComputer*> temp = ous[i]->getAllComputers();
		resultComputers.insert(resultComputers.end(), temp.begin(), temp.end());
	}
	return resultComputers;
}

vector<ADOU*> ADDomain::getAllOUs()
{
	vector<ADOU*> resultOUs;
	resultOUs.insert(resultOUs.end(), ous.begin(), ous.end());
	for (int i = 0; i < ous.size(); i ++)
	{
		vector<ADOU*> temp = ous[i]->getAllOUs();
		resultOUs.insert(resultOUs.end(), temp.begin(), temp.end());
	}
	return resultOUs;
}

vector<ADShareFolder*> ADDomain::getAllShareFolders()
{
	vector<ADShareFolder*> resultShareFolders;
	for (int i = 0; i < computers.size(); i ++)
	{
		resultShareFolders.insert(resultShareFolders.end(), computers[i]->shareFolders.begin(), computers[i]->shareFolders.end());
	}
	for (i = 0; i < ous.size(); i ++)
	{
		vector<ADShareFolder*> temp = ous[i]->getAllShareFolders();
		resultShareFolders.insert(resultShareFolders.end(), temp.begin(), temp.end());
	}
	return resultShareFolders;
}

void ADDomain::connectFolderWithACEs()
{
	for (int i = 0; i < g_shareFolders.size(); i ++)
	{
		g_shareFolders[i]->connectFolderWithACEs();
	}
}

vector<ADACE*> ADDomain::getAllACEs(int iMixOrShareOrNTFS)
{
	int i;
	vector<ADACE*> resultACEs;
	if (iMixOrShareOrNTFS == 2)
	{
		for (i = 0; i < g_shareFolders.size(); i ++)
		{
			resultACEs.insert(resultACEs.end(), g_shareFolders[i]->sharePermissions.begin(), g_shareFolders[i]->sharePermissions.end());
		}
		for (i = 0; i < g_shareFolders.size(); i ++)
		{
			resultACEs.insert(resultACEs.end(), g_shareFolders[i]->ntfsPermissions.begin(), g_shareFolders[i]->ntfsPermissions.end());
		}
	}
	else if (iMixOrShareOrNTFS == 1)
	{
		for (i = 0; i < g_shareFolders.size(); i ++)
		{
			resultACEs.insert(resultACEs.end(), g_shareFolders[i]->sharePermissions.begin(), g_shareFolders[i]->sharePermissions.end());
		}
	}
	else if (iMixOrShareOrNTFS == 0)
	{
		for (i = 0; i < g_shareFolders.size(); i ++)
		{
			resultACEs.insert(resultACEs.end(), g_shareFolders[i]->ntfsPermissions.begin(), g_shareFolders[i]->ntfsPermissions.end());
		}
	}
	else
	{
		MyMessageBox_Error(_T("getAllACEs"));
	}
	g_ADACEs = resultACEs;
	return resultACEs;
}

void ADDomain::refreshDCName()
{
	Output_ConsoleList(_T("Refreshing DC name to: ") + dcName + _T("..."));
	for (int i = 0; i < g_computers.size(); i ++)
	{
		if (dcAddress == g_computers[i]->ip)
		{
			dcName = g_computers[i]->cn;
			break;
		}
	}
}

void ADDomain::refreshAllShareFolderInfos()
{
	Output_ConsoleList(_T("Continue parsing domain: ") + dNSName + _T("..."));
	Output_ConsoleList(_T("Refreshing all shared folders' information in domain: ") + dNSName + _T("..."));
	ADDirectorySearch::setCredential(netBIOSName, userName, passWord);

	BOOL bFirstConnectedWithDC = FALSE;
	bFirstConnectedWithDC = ADDirectorySearch::IPCConnect(dcName, dNSName, userName, passWord);
	if (!bFirstConnectedWithDC)
	{
		if (!ADDirectorySearch::IPCConnect(dcAddress, dNSName, userName, passWord))
		{
			CString strMessage = _T("请确认域控制器") + dcName + _T("是否已经启动，否则无法任何共享文件夹信息！");
			MyMessageBox_Error(strMessage, _T("错误提示信息"));
			return;
		}
	}
	ADDirectorySearch::logOnByUserPassword(dNSName, userName, passWord);
	for (int i = 0; i < g_computers.size(); i ++)
	{
		BOOL bFirstConnectedWithServer = FALSE;
		if (g_computers[i]->run == 0)
		{
			Output_ConsoleList(_T("Connecting to ") + g_computers[i]->cn + _T(" failed because it's not running."));
			continue;
		}
		else
		{
			Output_ConsoleList(_T("Connecting to ") + g_computers[i]->cn + _T(" and its IP is ") + g_computers[i]->ip + _T("..."));
		}

		if (g_computers[i]->ip != dcAddress)
		{
			bFirstConnectedWithServer = ADDirectorySearch::IPCConnect(g_computers[i]->cn, dNSName, userName, passWord);
			if (!bFirstConnectedWithServer)
			{
				if (!ADDirectorySearch::IPCConnect(g_computers[i]->ip, dNSName, userName, passWord))
				{
					CString strMessage = _T("请确认计算机") + g_computers[i]->cn + _T("是否已经启动，否则无法显示其共享文件夹信息！");
					MyMessageBox_Error(strMessage, _T("错误提示信息"));
					continue;
				}
			}
			//if (!ADDirectorySearch::IPCConnect(g_computers[i]->ip, dNSName, userName, passWord))
			//{
			//	continue;
			//}
			//ADDirectorySearch::logOnByUserPassword(dNSName, userName, passWord);
			//ADDirectorySearch::logOnByUserPassword("athrun", userName, "123456As");
		}
		BOOL bAdded = ADPermissionSearch::Simple_addToLocalAdminGroup(g_computers[i]->cn, dNSName, userName);
// 		if (bAdded == 0)
// 		{
// 			continue; //akisn0w
// 		}
		ADPermissionSearch::initSharePathes(g_computers[i]->cn, g_computers[i]->ip);
		//ADPermissionSearch::IPCDisconnectShares(g_computers[i]->cn);
		g_computers[i]->shareFolders = ADDirectorySearch::listShareFolders(g_computers[i]->cn, g_computers[i]->ip, this);
		for (int j = 0; j < g_computers[i]->shareFolders.size(); j ++)
		{
			Output_ConsoleList(_T("Retrieving ACLs of folder ") + g_computers[i]->shareFolders[j]->cn + _T(" on ") + g_computers[i]->cn + _T("..."));

			vector<ADACE*> entries = ADDirectorySearch::getShareFolderInfo(g_computers[i]->shareFolders[j]->cn,
				g_computers[i]->cn, g_computers[i]->ip, _T("NTFS"), g_computers[i]->shareFolders[j]->dn);
			g_computers[i]->shareFolders[j]->ntfsPermissions = ADDirectorySearch::preprocessNTFSEntries(entries);
			
			entries = ADDirectorySearch::getShareFolderInfo(g_computers[i]->shareFolders[j]->cn,
				g_computers[i]->cn, g_computers[i]->ip, _T("SHARE"), g_computers[i]->shareFolders[j]->dn);
			g_computers[i]->shareFolders[j]->sharePermissions = ADDirectorySearch::preprocessNTFSEntries(entries);
			
			g_computers[i]->shareFolders[j]->clarifyAllGroupsAndUsers2();
			g_computers[i]->shareFolders[j]->clarifyPermissionTypes();
			//g_computers[i]->shareFolders[j]->generateFinalPermissions();
		}
		if (bAdded == 1)
		{
			ADPermissionSearch::Simple_removeFromLocalAdminGroup(g_computers[i]->cn, dNSName, userName);
		}
		if (g_computers[i]->ip != dcAddress)
		{
			if (bFirstConnectedWithServer)
			{
				ADDirectorySearch::IPCDisconnect(g_computers[i]->cn);
			}
			else
			{
				ADDirectorySearch::IPCDisconnect(g_computers[i]->ip);
			}
		}
	}
	ADDirectorySearch::logOff();
	if (bFirstConnectedWithDC)
	{
		ADDirectorySearch::IPCDisconnect(dcName);
	}
	else
	{
		ADDirectorySearch::IPCDisconnect(dcAddress);
	}
	g_shareFolders = getAllShareFolders();
}

void ADDomain::clarifyAllGroupsAndUsers() //used only for load mode
{
	for (int i = 0; i < g_computers.size(); i ++)
	{
		for (int j = 0; j < g_computers[i]->shareFolders.size(); j ++)
		{
			g_computers[i]->shareFolders[j]->clarifyAllGroupsAndUsers2();
			g_computers[i]->shareFolders[j]->clarifyPermissionTypes();
			//g_computers[i]->shareFolders[j]->generateFinalPermissions();
		}
	}
}

void ADDomain::updateComputerCNs()
{
	for (int i = 0; i < g_computers.size(); i ++)
	{
		g_computers[i]->cnOld = g_computers[i]->cn;
		if (g_computers[i]->cn.GetAt(g_computers[i]->cn.GetLength() - 1) == _T('-'))
		{
			g_computers[i]->cn = g_computers[i]->cn.Left(g_computers[i]->cn.GetLength() - 1);
		}
	}
}

void ADDomain::addToProperties()
{
	Output_ConsoleList(_T("Adding properties to objects in domain: ") + dNSName + _T("..."));
	for (int i = 0; i < g_users.size(); i ++)
	{
		g_users[i]->clearInfos();
		g_users[i]->addToProperties();
	}

	for (i = 0; i < g_groups.size(); i ++)
	{
		g_groups[i]->clearInfos();
		g_groups[i]->addToProperties();
	}

	for (i = 0; i < g_computers.size(); i ++)
	{
		g_computers[i]->clearInfos();
		g_computers[i]->addToProperties();
	}

	for (i = 0; i < g_shareFolders.size(); i ++)
	{
		g_shareFolders[i]->clearInfos();
		g_shareFolders[i]->addToProperties();
	}
	Output_ConsoleList(_T("Finished parsing domain: ") + dNSName + _T("."));
}

ADGroup* ADDomain::findGlobalGroupByDN(CString strGroupDNName)
{
	for (int i = 0; i < g_groups.size(); i ++)
	{
		if (g_groups[i]->dn == strGroupDNName)
		{
			return g_groups[i];
		}
	}
	return NULL;
}

ADUser* ADDomain::findGlobalUserByDN(CString strUserDNName)
{
	for (int i = 0; i < g_users.size(); i ++)
	{
		if (g_users[i]->dn == strUserDNName)
		{
			return g_users[i];
		}
	}
	return NULL;
}

void ADDomain::refreshAllGroupMembers(BOOL bDisplay)
{
	if (bDisplay)
	{
		Output_ConsoleList(_T("Refreshing all group members..."));
	}

	ADGroup* childGroup;
	ADUser* childUser;

	for (int i = 0; i < g_groups.size(); i ++)
	{
		//vector<CString> nstrMembers = ADDirectorySearch::getGroupMembers("192.168.1.5",
		//	"CN=Administrators,CN=Builtin,DC=domaintest,DC=net", "Administrator", "123456Ad");
		vector<CString> nstrMembers = ADDirectorySearch::getGroupMembers(dcAddress, g_groups[i]->dn, userName, passWord);
		g_groups[i]->groupMembers = nstrMembers;
		for (int j = 0; j < nstrMembers.size(); j ++)
		{
			if ((childGroup = findGlobalGroupByDN(nstrMembers[j])) != NULL)
			{
				g_groups[i]->addChildGroup(childGroup);
				childGroup->addFather(g_groups[i]);
			}
			else if ((childUser = findGlobalUserByDN(nstrMembers[j])) != NULL)
			{
				g_groups[i]->addChildUser(childUser);
				childUser->addFather(g_groups[i]);
			}
			else
			{
				//MyMessageBox_Error(_T("refreshAllGroupMembers Error"), _T("Error"));
				//return;
			}
		}
	}
}

void ADDomain::refreshAllGroupMembers_LoadMode(BOOL bDisplay)
{
	if (bDisplay)
	{
		Output_ConsoleList(_T("Refreshing all group members..."));
	}
	
	ADGroup* childGroup;
	ADUser* childUser;
	
	for (int i = 0; i < g_groups.size(); i ++)
	{
		//vector<CString> nstrMembers = ADDirectorySearch::getGroupMembers("192.168.1.5",
		//	"CN=Administrators,CN=Builtin,DC=domaintest,DC=net", "Administrator", "123456Ad");
		vector<CString> nstrMembers = g_groups[i]->groupMembers;
		for (int j = 0; j < nstrMembers.size(); j ++)
		{
			if ((childGroup = findGlobalGroupByDN(nstrMembers[j])) != NULL)
			{
				g_groups[i]->addChildGroup(childGroup);
				childGroup->addFather(g_groups[i]);
			}
			else if ((childUser = findGlobalUserByDN(nstrMembers[j])) != NULL)
			{
				g_groups[i]->addChildUser(childUser);
				childUser->addFather(g_groups[i]);
			}
			else
			{
				//MyMessageBox_Error(_T("refreshAllGroupMembers Error"), _T("Error"));
				//return;
			}
		}
	}
}

CString ADDomain::Node2String()
{
	CString strIn;
	CString strOut;
	int i;
	
	for (i = 0; i < users.size(); i ++)
	{
		strIn += users[i]->Node2String();
	}
	
	for (i = 0; i < groups.size(); i ++)
	{
		strIn += groups[i]->Node2String();
	}

	strIn += everyone->Node2String();
	
	for (i = 0; i < computers.size(); i ++)
	{
		strIn += computers[i]->Node2String();
	}
	
	for (i = 0; i < ous.size(); i ++)
	{
		strIn += ous[i]->Node2String();
	}
	
	strOut = _T("subgraph \"cluster_") + dnName + _T("\"\n{\n") + strIn + _T("label=\"") + dNSName + _T("\";\n") + _T("}\n");
	return strOut;
}

CString ADDomain::Edge2String()
{
	CString strOut;
	for (int i = 0; i < g_groups.size(); i ++)
	{
		strOut += g_groups[i]->Edge2String();
	}
	//g_shareFolders = getAllShareFolders();
	for (i = 0; i < g_shareFolders.size(); i ++)
	{
		strOut += g_shareFolders[i]->Edge2String2();
	}
	return strOut;
}

void ADDomain::parseGUIData(ADDotReader *reader)
{
	rectGUI = reader->getOURectByString(_T("cluster_") + dnName);
	shape = reader->addDomain((ADObject *) this, dNSName, rectGUI.left, rectGUI.top, rectGUI.right, rectGUI.bottom);

	int i;
	for (i = 0; i < users.size(); i ++)
	{
		users[i]->parseGUIData(reader);
	}
	for (i = 0; i < groups.size(); i ++)
	{
		groups[i]->parseGUIData(reader);
	}
	everyone->parseGUIData(reader);
	for (i = 0; i < computers.size(); i ++)
	{
		computers[i]->parseGUIData(reader);
	}
	for (i = 0; i < ous.size(); i ++)
	{
		ous[i]->parseGUIData(reader);
	}
}

void ADDomain::generateLink(ADDotReader *reader)
{
	int i;
	for (i = 0; i < users.size(); i ++)
	{
		users[i]->generateLink(reader);
	}
	for (i = 0; i < groups.size(); i ++)
	{
		groups[i]->generateLink(reader);
	}
	for (i = 0; i < computers.size(); i ++)
	{
		computers[i]->generateLink(reader);
	}
	for (i = 0; i < ous.size(); i ++)
	{
		ous[i]->generateLink(reader);
	}
}

void ADDomain::setConsoleText(CString strText)
{

}

void ADDomain::setGlobalDomains(vector<ADDomain*> a_globalDomains)
{
	globalDomains = a_globalDomains;
}