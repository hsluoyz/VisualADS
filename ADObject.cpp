#include "StdAfx.h"

#include "ADObject.h"

IMPLEMENT_SERIAL(ADObject, CObject, 0)

ADObject::ADObject()
{

}

void ADObject::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		map<CString, CString>::iterator it;

		ar << infos.size();
		for (i = 0, it = infos.begin(); i < infos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}

		ar << fatherInfos.size();
		for (i = 0, it = fatherInfos.begin(); i < fatherInfos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}

		ar << childGroupInfos.size();
		for (i = 0, it = childGroupInfos.begin(); i < childGroupInfos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}

		ar << childUserInfos.size();
		for (i = 0, it = childUserInfos.begin(); i < childUserInfos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}

		ar << accessInfos.size();
		for (i = 0, it = accessInfos.begin(); i < accessInfos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}

		ar << networkInfos.size();
		for (i = 0, it = networkInfos.begin(); i < networkInfos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}
	}
	else
	{
		int i;
		int iSize;
		CString strFirst;
		CString strSecond;

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			infos.insert(pair<CString, CString>(strFirst, strSecond));
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			fatherInfos.insert(pair<CString, CString>(strFirst, strSecond));
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			childGroupInfos.insert(pair<CString, CString>(strFirst, strSecond));
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			childUserInfos.insert(pair<CString, CString>(strFirst, strSecond));
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			accessInfos.insert(pair<CString, CString>(strFirst, strSecond));
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			networkInfos.insert(pair<CString, CString>(strFirst, strSecond));
		}
	}

	CObject::Serialize(ar);
}

void ADObject::addPairToInfos(CString strKey, CString strValue)
{
	infos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::addPairToFatherInfos(CString strKey, CString strValue)
{
	fatherInfos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::addPairToChildGroupInfos(CString strKey, CString strValue)
{
	childGroupInfos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::addPairToChildUserInfos(CString strKey, CString strValue)
{
	childUserInfos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::addPairToAccessInfos(CString strKey, CString strValue)
{
	accessInfos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::addPairToNetworkInfos(CString strKey, CString strValue)
{
	networkInfos.insert(pair<CString, CString>(strKey, strValue));
}

void ADObject::clearInfos()
{
	//infos.clear();
	fatherInfos.clear();
	childGroupInfos.clear();
	childUserInfos.clear();
	accessInfos.clear();
	networkInfos.clear();
}

void ADObject::onButttonDown()
{
	
}

void ADObject::onButtonUp()
{

}