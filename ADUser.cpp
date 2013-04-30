#include "StdAfx.h"

#include "ADUser.h"
#include "ADGroup.h"
#include "ADDirectorySearch.h"
#include "ADDotReader.h"
#include "MyShape.h"

IMPLEMENT_SERIAL(ADUser, ADObject, 0)

ADUser::ADUser()
{

}

void ADUser::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << cn << dn;
	}
	else
	{
		ar >> cn >> dn;
		//Intialization needed: fathers --> ADDomain::refreshAllGroupMembers [ok]
	}

	ADObject::Serialize(ar);
}

ADUser::ADUser(CString a_cn, CString a_dn, ADDomain* a_domain)
{
	cn = a_cn;
	dn = a_dn;
	domain = a_domain;

	ptGUI = CPoint(-1, -1);
}

CString ADUser::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

void ADUser::addFather(ADGroup* group)
{
	fathers.push_back(group);
}

CString ADUser::Node2String()
{
	CString strOut;
		strOut = _T("\"") + dn + _T("\" [label=\"") + ADDirectorySearch::lineFeedToString(cn) + _T("\", shape=ellipse, width=1.5, color=black, fontsize=10") + _T("];\n");
	return strOut;
}

void ADUser::parseGUIData(ADDotReader *reader)
{
	ptGUI = reader->getElementPointByString(dn);
	shape = reader->addUser((ADObject *) this, cn, ptGUI.x, ptGUI.y);
}

void ADUser::generateLink(ADDotReader *reader)
{
	
}

void ADUser::addToProperties()
{
	for (int i = 0; i < fathers.size(); i ++)
	{
		addPairToFatherInfos(itos(i + 1), fathers[i]->cn);
	}
}