#include "StdAfx.h"

#include "ADDomain.h"
#include "ADGroup.h"
#include "ADUser.h"
#include "ADShareFolder.h"
#include "ADDirectorySearch.h"
#include "ADDotReader.h"
#include "MyShape.h"

IMPLEMENT_SERIAL(ADShareFolder, ADObject, 0)

ADShareFolder::ADShareFolder()
{

}

void ADShareFolder::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		ar << cn << dn;
		//Intialization needed: domain --> [Teamwork] [ok]

		ar << ntfsPermissions.size();
		for (i = 0; i < ntfsPermissions.size(); i ++)
		{
			ntfsPermissions[i]->Serialize(ar);
		}

		ar << sharePermissions.size();
		for (i = 0; i < sharePermissions.size(); i ++)
		{
			sharePermissions[i]->Serialize(ar);
		}

		ar << finalPermissions.size();
		for (i = 0; i < finalPermissions.size(); i ++)
		{
			finalPermissions[i]->Serialize(ar);
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
			ADACE *pNtfsPermission = new ADACE();
			pNtfsPermission->Serialize(ar);
			ntfsPermissions.push_back(pNtfsPermission);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADACE *pSharePermission = new ADACE();
			pSharePermission->Serialize(ar);
			sharePermissions.push_back(pSharePermission);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ADACE *pFinalPermission = new ADACE();
			pFinalPermission->Serialize(ar);
			finalPermissions.push_back(pFinalPermission);
		}
	}
	
	ADObject::Serialize(ar);
}

ADShareFolder::ADShareFolder(CString strRemoteName, ADDomain* a_domain)
{
	dn = remoteName2DNName(strRemoteName);
	int iPos = strRemoteName.ReverseFind(_T('\\'));
	cn = strRemoteName.Right(strRemoteName.GetLength() - iPos - 1);
	domain = a_domain;

	ptGUI = CPoint(-1, -1);
}

/*
class ADACE
{
int allowed;
CString name;
CString refDomain;
int r;
int w;
int x;
};
*/

CString ADShareFolder::remoteName2DNName(CString strRemoteName)
{
	//strRemoteName = "\\\\Win2003-SP2-1\\temp"
	int iSTart1, iEnd1;
	int iStart2, iEnd2;
	iSTart1 = strRemoteName.Find(_T("\\\\"));
	if (iSTart1 == -1)
	{
		MyMessageBox_Error(_T("remoteName2DNName"));
		return _T("remoteName2DNName Error");
	}
	iSTart1 += 2;

	iStart2 = strRemoteName.Find(_T("\\"), iSTart1);
	if (iStart2 == -1)
	{
		MyMessageBox_Error(_T("remoteName2DNName"));
		return _T("remoteName2DNName Error");
	}
	iEnd1 = iStart2;
	iStart2 += 1;
	iEnd2 = strRemoteName.GetLength();

	CString strMachineName = strRemoteName.Mid(iSTart1, iEnd1 - iSTart1);
	CString strShareFolderName = strRemoteName.Mid(iStart2, iEnd2 - iStart2);
	CString strDNName = _T("FI=") + strShareFolderName + _T(",MA=") + strMachineName;
	return strDNName;
}

void ADShareFolder::generateFinalPermissions()
{
	ADACE* finalPermission;

	for (int i = 0; i < sharePermissions.size(); i ++)
	{
		int iHasMetPermit = 0;
		finalPermission = sharePermissions[i]->duplicate();
		for (int j = 0; j < ntfsPermissions.size(); j ++)
		{
			if (sharePermissions[i]->name == ntfsPermissions[j]->name &&
				sharePermissions[i]->refDomain == ntfsPermissions[j]->refDomain)
			{
				if (sharePermissions[i]->allowed == 1 && ntfsPermissions[j]->allowed == 1)
				{
					iHasMetPermit = 1;
					if (ntfsPermissions[j]->r == 0)
						finalPermission->r = 0;
					if (ntfsPermissions[j]->w == 0)
						finalPermission->w = 0;
					if (ntfsPermissions[j]->x == 0)
						finalPermission->x = 0;
				}
				else if (sharePermissions[i]->allowed == 0 && ntfsPermissions[j]->allowed == 0)
				{
					if (ntfsPermissions[j]->r == 1)
						finalPermission->r = 1;
					if (ntfsPermissions[j]->w == 1)
						finalPermission->w = 1;
					if (ntfsPermissions[j]->x == 1)
						finalPermission->x = 1;
				}
				else if (sharePermissions[i]->allowed == 1 && ntfsPermissions[j]->allowed == 0)
				{
					if (ntfsPermissions[j]->r == 1)
						finalPermission->r = 0;
					if (ntfsPermissions[j]->w == 1)
						finalPermission->w = 0;
					if (ntfsPermissions[j]->x == 1)
						finalPermission->x = 0;
				}
			}
		}
		if (sharePermissions[i]->allowed == 1 && iHasMetPermit == 0)
		{
			//finalPermission->r = 0;
			//finalPermission->w = 0;
			//finalPermission->x = 0;
			delete finalPermission;
		}
		else if (sharePermissions[i]->allowed == 1 && finalPermission->r == 0 && finalPermission->w == 0 && finalPermission->x == 0)
			delete finalPermission;
		else
			finalPermissions.push_back(finalPermission);
	}
}

void ADShareFolder::generateFinalPermissions2()
{
	finalPermissions = ntfsPermissions;
}

void ADShareFolder::generateFinalPermissions3()
{
	finalPermissions = sharePermissions;
}

void ADShareFolder::clarifyPermissionTypes()
{
	for (int i = 0; i < ntfsPermissions.size(); i ++)
	{
		ntfsPermissions[i]->type = 0;
	}

	for (i = 0; i < sharePermissions.size(); i ++)
	{
		sharePermissions[i]->type = 1;
	}
}

void ADShareFolder::clarifyAllGroupsAndUsers()
{
	ADGroup* targetGroup;
	ADUser* targetUser;
	
	for (int i = 0; i < finalPermissions.size(); i ++)
	{
		if ((targetGroup = ADDirectorySearch::getGroupFromDomains(finalPermissions[i]->refDomain, finalPermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			finalPermissions[i]->group = targetGroup;
		}
		else if ((targetUser = ADDirectorySearch::getUserFromDomains(finalPermissions[i]->refDomain, finalPermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			finalPermissions[i]->user = targetUser;
		}
		else if (finalPermissions[i]->name == _T("Everyone") && finalPermissions[i]->refDomain == _T(""))
		{
			finalPermissions[i]->group = domain->everyone;
		}
		else
		{
			MyMessageBox_Error(_T("clarifyAllGroupsAndUsers Error"), _T("Error"));
			return;
		}
	}
}

void ADShareFolder::clarifyAllGroupsAndUsers2()
{
	ADGroup* targetGroup;
	ADUser* targetUser;
	
	for (int i = 0; i < ntfsPermissions.size(); i ++)
	{
		CString strName = ntfsPermissions[i]->name;
		if (strName.Right(1) == '$')
		{
			deleteNTFSPermission(i);
			i --;
			break;
		}
		if ((targetGroup = ADDirectorySearch::getGroupFromDomains(ntfsPermissions[i]->refDomain, ntfsPermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			ntfsPermissions[i]->group = targetGroup;
		}
		else if ((targetUser = ADDirectorySearch::getUserFromDomains(ntfsPermissions[i]->refDomain, ntfsPermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			ntfsPermissions[i]->user = targetUser;
		}
		else if (ntfsPermissions[i]->name == _T("Everyone") && ntfsPermissions[i]->refDomain == _T(""))
		{
			ntfsPermissions[i]->group = domain->everyone;
		}
		else
		{
			MyMessageBox_Error(_T("clarifyAllGroupsAndUsers2 Error"), _T("Error"));
			return;
		}
	}

	for (i = 0; i < sharePermissions.size(); i ++)
	{
		CString strName = sharePermissions[i]->name;
		if (strName.Right(1) == '$')
		{
			deleteSharePermission(i);
			i --;
			break;
		}
		if ((targetGroup = ADDirectorySearch::getGroupFromDomains(sharePermissions[i]->refDomain, sharePermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			sharePermissions[i]->group = targetGroup;
		}
		else if ((targetUser = ADDirectorySearch::getUserFromDomains(sharePermissions[i]->refDomain, sharePermissions[i]->name,
			domain->globalDomains)) != NULL)
		{
			sharePermissions[i]->user = targetUser;
		}
		else if (sharePermissions[i]->name == _T("Everyone") && sharePermissions[i]->refDomain == _T(""))
		{
			sharePermissions[i]->group = domain->everyone;
		}
		else
		{
			MyMessageBox_Error(_T("clarifyAllGroupsAndUsers2 Error"), _T("Error"));
			return;
		}
	}
}

void ADShareFolder::deleteNTFSPermission(int iIndex)
{
	ADACE *pADACE = ntfsPermissions[iIndex];
	vector<ADACE*>::iterator iter = ntfsPermissions.begin();
	iter += iIndex;
	ntfsPermissions.erase(iter);
	delete pADACE;
	pADACE = NULL;
}

void ADShareFolder::deleteSharePermission(int iIndex)
{
	ADACE *pADACE = sharePermissions[iIndex];
	vector<ADACE*>::iterator iter = sharePermissions.begin();
	iter += iIndex;
	sharePermissions.erase(iter);
	delete pADACE;
	pADACE = NULL;
}

void ADShareFolder::connectFolderWithACEs()
{
	for (int i = 0; i < ntfsPermissions.size(); i ++)
	{
		ntfsPermissions[i]->fatherFolder = this;
	}
	for (i = 0; i < sharePermissions.size(); i ++)
	{
		sharePermissions[i]->fatherFolder = this;
	}
	for (i = 0; i < finalPermissions.size(); i ++)
	{
		finalPermissions[i]->fatherFolder = this;
	}
}

CString ADShareFolder::Node2String()
{
	CString strOut;
	strOut = _T("\"") + dn + _T("\" [label=\"") + ADDirectorySearch::lineFeedToString(cn) + _T("\", shape=ellipse, width=1.5, color=blue, fontsize=10") + _T("];\n");
	return strOut;
}

void ADShareFolder::parseGUIData(ADDotReader *reader)
{
	ptGUI = reader->getElementPointByString(dn);
	shape = reader->addResource((ADObject *) this, cn, ptGUI.x, ptGUI.y);
}

CString ADShareFolder::Edge2String()
{
	CString strAction;
	CString strOut;
	for (int i = 0; i < finalPermissions.size(); i ++)
	{
		if (finalPermissions[i]->allowed == 1)
			strAction = _T("Permit:");
		else
			strAction = _T("Deny:");
		if (finalPermissions[i]->r == 1)
			strAction += _T("R");
		if (finalPermissions[i]->w == 1)
			strAction += _T("W");
		if (finalPermissions[i]->x == 1)
			strAction += _T("X");
		
		if (finalPermissions[i]->group != NULL)
		{
			if (finalPermissions[i]->allowed == 1)
				strOut += (_T("\"") + finalPermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut += (_T("\"") + finalPermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else if (finalPermissions[i]->user != NULL)
		{
			if (finalPermissions[i]->allowed == 1)
				strOut += (_T("\"") + finalPermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut += (_T("\"") + finalPermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else
		{
			MyMessageBox_Error(_T("Edge2String Error"), _T("Error"));
			return _T("");
		}
	}
	return strOut;
}

CString ADShareFolder::Edge2String2()
{
	CString strAction;
	CString strOut;
	for (int i = 0; i < ntfsPermissions.size(); i ++)
	{
		if (ntfsPermissions[i]->allowed == 1)
			strAction = _T("NTFS-Permit:");
		else
			strAction = _T("NTFS-Deny:");
		if (ntfsPermissions[i]->r == 1)
			strAction += _T("R");
		if (ntfsPermissions[i]->w == 1)
			strAction += _T("W");
		if (ntfsPermissions[i]->x == 1)
			strAction += _T("X");
		
		if (ntfsPermissions[i]->group != NULL)
		{
			if (ntfsPermissions[i]->allowed == 1)
				strOut += (_T("\"") + ntfsPermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut += (_T("\"") + ntfsPermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else if (ntfsPermissions[i]->user != NULL)
		{
			if (ntfsPermissions[i]->allowed == 1)
				strOut += (_T("\"") + ntfsPermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut += (_T("\"") + ntfsPermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else
		{
			MyMessageBox_Error(_T("Edge2String2 Error"), _T("Error"));
			return _T("");
		}
	}

	CString strOut2;
	for (i = 0; i < sharePermissions.size(); i ++)
	{
		if (sharePermissions[i]->allowed == 1)
			strAction = _T("Share-Permit:");
		else
			strAction = _T("Share-Deny:");
		if (sharePermissions[i]->r == 1)
			strAction += _T("R");
		if (sharePermissions[i]->w == 1)
			strAction += _T("W");
		if (sharePermissions[i]->x == 1)
			strAction += _T("X");
		
		if (sharePermissions[i]->group != NULL)
		{
			if (sharePermissions[i]->allowed == 1)
				strOut2 += (_T("\"") + sharePermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut2 += (_T("\"") + sharePermissions[i]->group->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else if (sharePermissions[i]->user != NULL)
		{
			if (sharePermissions[i]->allowed == 1)
				strOut2 += (_T("\"") + sharePermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=blue, style=bold];\n"));
			else
				strOut2 += (_T("\"") + sharePermissions[i]->user->dn + _T("\" -> \"") + dn + _T("\" [label=\"") + strAction + _T("\", fontsize=10, color=red, style=bold];\n"));
		}
		else
		{
			MyMessageBox_Error(_T("Edge2String2 Error"), _T("Error"));
			return _T("");
		}
	}

	return strOut + strOut2;
}

void ADShareFolder::generateLink(ADDotReader *reader)
{
	CString strTitle;
	int iPermission;
	CString strOut;
	for (int i = 0; i < ntfsPermissions.size(); i ++)
	{
		if (ntfsPermissions[i]->allowed == 1)
		{
			strTitle = _T("NTFS-Permit:");
			iPermission = 0;
		}
		else
		{
			strTitle = _T("NTFS-Deny:");
			iPermission = 1;
		}
		if (ntfsPermissions[i]->r == 1)
			strTitle += _T("R");
		if (ntfsPermissions[i]->w == 1)
			strTitle += _T("W");
		if (ntfsPermissions[i]->x == 1)
			strTitle += _T("X");
		
		if (ntfsPermissions[i]->group != NULL)
		{
			reader->addNTFSRelation(strTitle, ntfsPermissions[i]->group->shape, shape, iPermission, TRUE);
		}
		else if (ntfsPermissions[i]->user != NULL)
		{
			reader->addNTFSRelation(strTitle, ntfsPermissions[i]->user->shape, shape, iPermission, TRUE);
		}
		else
		{
			MyMessageBox_Error(_T("generateLink Error"), _T("Error"));
		}
	}
	
	CString strTitle2;
	for (i = 0; i < sharePermissions.size(); i ++)
	{
		if (sharePermissions[i]->allowed == 1)
		{
			strTitle2 = _T("Share-Permit:");
			iPermission = 0;
		}
		else
		{
			strTitle2 = _T("Share-Deny:");
			iPermission = 1;
		}
		if (sharePermissions[i]->r == 1)
			strTitle2 += _T("R");
		if (sharePermissions[i]->w == 1)
			strTitle2 += _T("W");
		if (sharePermissions[i]->x == 1)
			strTitle2 += _T("X");
		
		if (sharePermissions[i]->group != NULL)
		{
			reader->addShareRelation(strTitle2, sharePermissions[i]->group->shape, shape, iPermission, TRUE);
		}
		else if (sharePermissions[i]->user != NULL)
		{
			reader->addShareRelation(strTitle2, sharePermissions[i]->user->shape, shape, iPermission, TRUE);
		}
		else
		{
			MyMessageBox_Error(_T("generateLink Error"), _T("Error"));
		}
	}
}

void ADShareFolder::addToProperties()
{
	for (int i = 0; i < sharePermissions.size(); i ++)
	{
		addPairToAccessInfos(sharePermissions[i]->dn, sharePermissions[i]->toString());
	}

	for (i = 0; i < ntfsPermissions.size(); i ++)
	{
		addPairToAccessInfos(ntfsPermissions[i]->dn, ntfsPermissions[i]->toString());
	}
}