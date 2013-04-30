#include "StdAfx.h"

#include "ADDomain.h"
#include "ADOU.h"
#include "ADUser.h"
#include "ADGroup.h"
#include "ADComputer.h"
#include "ADDirectorySearch.h"
#include "ADDotReader.h"
#include "MyShape.h"

IMPLEMENT_SERIAL(ADOU, ADObject, 0)

ADOU::ADOU()
{

}

void ADOU::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		ar << cn << dn;

		//Initialization needed: domain
		
		ar << users.size();
		for (i = 0; i < users.size(); i++)
		{
			users[i]->Serialize(ar);
		}
		
		ar << groups.size();
		for (i = 0; i < groups.size(); i++)
		{
			groups[i]->Serialize(ar);
		}
		
		ar << computers.size();
		for (i = 0; i < computers.size(); i++)
		{
			computers[i]-> Serialize(ar);
		}
		
		ar << ous.size();
		for (i = 0; i < ous.size(); i++)
		{
			ous[i] ->Serialize(ar);
		}
	}
	else
	{
		int i;
		int iSize;
		ar >> cn >> dn;
		
		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADUser* pUser = new ADUser();
			pUser->domain = domain;
			pUser->Serialize(ar);
			users.push_back(pUser);
		}
		
		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADGroup* pGroup = new ADGroup();
			pGroup->domain = domain;
			pGroup->Serialize(ar);
			groups.push_back(pGroup);
		}
		
		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADComputer* pComputer = new ADComputer();
			pComputer->domain = domain;
			pComputer->Serialize(ar);
			computers.push_back(pComputer);
		}
		
		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADOU* pOU = new ADOU();
			pOU->domain = domain;
			pOU->Serialize(ar);
			ous.push_back(pOU);
		}
	}
	
	ADObject::Serialize(ar);
}

ADOU::ADOU(CString a_cn, CString a_dn, ADDomain* a_domain)
{
	cn = a_cn;
	dn = a_dn;
	domain = a_domain;

	rectGUI = CRect(-1, -1, -1, -1);
}

ADUser* ADOU::addUser(CString strUserName, CString strDNName)
{
	ADUser *user = new ADUser(strUserName, strDNName, domain);
	users.push_back(user);
	return user;
}

ADGroup* ADOU::addGroup(CString strGroupName, CString strDNName, ADDomain* domain, CString strType)
{
	ADGroup *group = new ADGroup(strGroupName, strDNName, domain, strType);
	groups.push_back(group);
	return group;
}

ADComputer* ADOU::addComputer(CString strComputerName, CString strDNName, ADDomain* domain)
{
	ADComputer *computer = new ADComputer(strComputerName, strDNName, domain);
	computers.push_back(computer);
	return computer;
}

ADOU* ADOU::addOU(CString strOUName, CString strDNName, ADDomain* domain)
{
	ADOU *ou = new ADOU(strOUName, strDNName, domain);
	ous.push_back(ou);
	return ou;
}

ADOU* ADOU::findOU(CString strOUName, CString strDNName)
{
	ADOU* retOU = NULL;
	for (int i = 0; i < ous.size(); i ++)
	{
		if (strOUName == ous[i]->cn)
		{
			return ous[i];
		}
	}
	return addOU(strOUName, strDNName, domain);
}

void ADOU::performEntry(ADEntry* entry)
{
	if (entry->dnKeys.size() == 0)
	{
		return;
	}
	else if (entry->dnKeys[entry->dnKeys.size() - 1] == _T("OU"))
	{
		CString strLastValue = entry->popKeyValue();
		findOU(strLastValue, entry->getOUDN())->performEntry(entry);
	}
	else if (entry->type == _T("computer"))
	{
		ADComputer *computer = addComputer(entry->cn, entry->dn, domain);
		computer->infos = entry->infos;
	}
	else if (entry->type == _T("user"))
	{
		ADUser *user = addUser(entry->cn, entry->dn);
		user->infos = entry->infos;
	}
	else if (entry->type == _T("localgroup"))
	{
		ADGroup *group = addGroup(entry->cn, entry->dn, domain, _T("local"));
		group->infos = entry->infos;
	}
	else if (entry->type == _T("globalgroup"))
	{
		ADGroup *group = addGroup(entry->cn, entry->dn, domain, _T("global"));
		group->infos = entry->infos;
	}
	else
	{
		MyMessageBox_Error(_T("performEntry Error"), _T("Error"));
	}
}

vector<ADUser*> ADOU::getAllUsers()
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

vector<ADGroup*> ADOU::getAllGroups()
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

vector<ADComputer*> ADOU::getAllComputers()
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

vector<ADOU*> ADOU::getAllOUs()
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

vector<ADShareFolder*> ADOU::getAllShareFolders()
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

CString ADOU::Node2String()
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

	for (i = 0; i < computers.size(); i ++)
	{
		strIn += computers[i]->Node2String();
	}

	for (i = 0; i < ous.size(); i ++)
	{
		strIn += ous[i]->Node2String();
	}

	int iZeroFlag = users.size() + groups.size() + computers.size() + ous.size();
	if (iZeroFlag == 0)
		return _T("");
	else
	{
		strOut = _T("subgraph \"cluster_") + dn + _T("\"\n{\n") + strIn + _T("label=\"") + cn + _T("\";\n") + _T("}\n");
		return strOut;
	}
}

void ADOU::parseGUIData(ADDotReader *reader)
{
	int iZeroFlag = users.size() + groups.size() + computers.size() + ous.size();
	if (iZeroFlag != 0)
	{
		rectGUI = reader->getOURectByString(_T("cluster_") + dn);
		shape = reader->addOU((ADObject *) this, cn, rectGUI.left, rectGUI.top, rectGUI.right, rectGUI.bottom);
	
		int i;
		for (i = 0; i < users.size(); i ++)
		{
			users[i]->parseGUIData(reader);
		}
		for (i = 0; i < groups.size(); i ++)
		{
			groups[i]->parseGUIData(reader);
		}
		for (i = 0; i < computers.size(); i ++)
		{
			computers[i]->parseGUIData(reader);
		}
		for (i = 0; i < ous.size(); i ++)
		{
			ous[i]->parseGUIData(reader);
		}
	}
}

void ADOU::generateLink(ADDotReader *reader)
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