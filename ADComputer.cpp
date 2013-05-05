#include "StdAfx.h"

#include "ADDomain.h"
#include "ADComputer.h"
#include "ADShareFolder.h"
#include "ADDirectorySearch.h"
#include "ADDotReader.h"
#include "MyShape.h"

#include <COMUTIL.H>

IMPLEMENT_SERIAL(ADComputer, ADObject, 0)

ADComputer::ADComputer()
{

}

void ADComputer::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		ar << cn << dn << ip << run;

		ar << shareFolders.size();
		for (i = 0; i < shareFolders.size(); i ++)
		{
			shareFolders[i] ->Serialize(ar);
		}
	}
	else
	{
		int iSize;
		ar >> cn >> dn >> ip >> run;
		
		
		ar >> iSize;
		for (int i = 0; i < iSize; i ++)
		{
			ADShareFolder *pShareFolder = new ADShareFolder();
			pShareFolder->domain = domain;
			pShareFolder->Serialize(ar);
			shareFolders.push_back(pShareFolder);
		}
	}
	
	ADObject::Serialize(ar);
}

ADComputer::ADComputer(CString a_cn, CString a_dn, ADDomain* a_domain)
{
	cn = a_cn;
	dn = a_dn;
	domain = a_domain;
	ip = ADDirectorySearch::host2IP2(cn);

	if (ip == _T(""))
	{
		if (cn.CompareNoCase(a_domain->dcName) == 0)
			ip = a_domain->dcAddress;
	}

	if (ip != _T(""))
	{
		run = ADDirectorySearch::checkIPAlive(ip);
	}
	else
	{
		run = 0;
	}

	ptGUI = CPoint(-1, -1);
	rectGUI = CRect(-1, -1, -1, -1);
}

CString ADComputer::Node2String()
{
	CString strOut;

	if (shareFolders.size() == 0)
	{
		CString strOut;
		strOut = _T("\"") + dn + _T("\" [label=\"") + ADDirectorySearch::lineFeedToString(cn) + _T("\", shape=box, width=2, color=red, fontsize=10") + _T("];\n");
		return strOut;
	}
	else
	{
		CString strIn;
		int i;
		for (i = 0; i < shareFolders.size(); i ++)
		{
			strIn += shareFolders[i]->Node2String();
		}
		strOut = _T("subgraph \"cluster_") + dn + _T("\"\n{\n") + strIn + _T("color=red;\nlabel=\"") + cn + _T("\";\n") + _T("}\n");
		return strOut;
	}
}

void ADComputer::parseGUIData(ADDotReader *reader)
{
	if (shareFolders.size() == 0)
	{
		ptGUI = reader->getElementPointByString(dn);
		shape = reader->addComputer((ADObject *) this, cn, ptGUI.x, ptGUI.y);
	}
	else
	{
		rectGUI = reader->getBigComputerRectByString(dn);
		shape = reader->addBigComputer((ADObject *) this, cn, rectGUI.left, rectGUI.top, rectGUI.right, rectGUI.bottom);

		int i;
		for (i = 0; i < shareFolders.size(); i ++)
		{
			shareFolders[i]->parseGUIData(reader);
		}
	}
}

void ADComputer::generateLink(ADDotReader *reader)
{
	for (int i = 0; i < shareFolders.size(); i ++)
	{
		shareFolders[i]->generateLink(reader);
	}
}

void ADComputer::addToProperties()
{
	if (Global_GetLanguage())
	{
		if (run)
		{
			addPairToNetworkInfos(_T("IPµÿ÷∑"), ip);
		}
		else
		{
			addPairToNetworkInfos(_T("IPµÿ÷∑"), _T("Œ¥∆Ù∂Ø"));
		}
	}
	else
	{
		if (run)
		{
			addPairToNetworkInfos(_T("IP Address"), ip);
		}
		else
		{
			addPairToNetworkInfos(_T("IP Address"), _T("Not Alive"));
		}
	}
}