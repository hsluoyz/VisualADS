#include "StdAfx.h"

#include "ADDomain.h"
#include "ADGroup.h"
#include "ADUser.h"
#include "ADDirectorySearch.h"
#include "ADDotReader.h"
#include "MyShape.h"

IMPLEMENT_SERIAL(ADGroup, ADObject, 0)

ADGroup::ADGroup()
{

}

void ADGroup::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		ar << cn << dn << type;

		ar << groupMembers.size();
		for (i = 0; i < groupMembers.size(); i ++)
		{
			ar << groupMembers[i];
		}
	}
	else
	{
		int iSize;
		ar >> cn >> dn >> type;

		ar >> iSize;
		groupMembers.clear();
		CString strGroupMember;
		for (int i = 0; i < iSize; i ++)
		{
			ar >> strGroupMember;
			groupMembers.push_back(strGroupMember);
		}
		//Intialization needed: domain --> [Father Class Object] [ok]
		//Intialization needed: childGroups, childUsers, fathers --> ADDomain::refreshAllGroupMembers [ok]
	}

	ADObject::Serialize(ar);
}

ADGroup::ADGroup(CString a_cn, CString a_dn, ADDomain* a_domain, CString a_type)
{
	cn = a_cn;
	dn = a_dn;
	domain = a_domain;
	type = a_type;

	ptGUI = CPoint(-1, -1);
}

CString ADGroup::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

void ADGroup::addChildGroup(ADGroup* group)
{
	childGroups.push_back(group);
}

void ADGroup::addChildUser(ADUser* user)
{
	childUsers.push_back(user);
}

void ADGroup::addFather(ADGroup* group)
{
	fathers.push_back(group);
}

CString ADGroup::Node2String()
{
	CString strOut;
	strOut = _T("\"") + dn + _T("\" [label=\"") + ADDirectorySearch::lineFeedToString(cn) + _T("\", shape=hexagon, width=2, color=red, fontsize=10" + _T("];\n"));
	return strOut;
}

CString ADGroup::Edge2String()
{
	CString strOut;
	for (int i = 0; i < childGroups.size(); i ++)
	{
		strOut += (_T("\"") + dn + _T("\" -> \"") + childGroups[i]->dn + _T("\" [color=black];\n"));
	}
	for (i = 0; i < childUsers.size(); i ++)
	{
		strOut += (_T("\"") + dn + _T("\" -> \"") + childUsers[i]->dn + _T("\" [color=black];\n"));
	}
	return strOut;
}

void ADGroup::parseGUIData(ADDotReader *reader)
{
	ptGUI = reader->getElementPointByString(dn);
	shape = reader->addGroup((ADObject *) this, cn, ptGUI.x, ptGUI.y);
}

void ADGroup::generateLink(ADDotReader *reader)
{
	for (int i = 0; i < childGroups.size(); i ++)
	{
		reader->addInheritRelation("", shape, childGroups[i]->shape);
	}
	for (i = 0; i < childUsers.size(); i ++)
	{
		reader->addInheritRelation("", shape, childUsers[i]->shape);
	}
}

void ADGroup::addToProperties()
{
	for (int i = 0; i < fathers.size(); i ++)
	{
		addPairToFatherInfos(itos(i + 1), fathers[i]->cn);
	}

	for (i = 0; i < childGroups.size(); i ++)
	{
		addPairToChildGroupInfos(itos(i + 1), childGroups[i]->cn);
	}

	for (i = 0; i < childUsers.size(); i ++)
	{
		addPairToChildUserInfos(itos(i + 1), childUsers[i]->cn);
	}
}