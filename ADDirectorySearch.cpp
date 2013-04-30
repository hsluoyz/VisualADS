#include "StdAfx.h"

#include "ADDirectorySearch.h"
#include "ADPermissionSearch.h"
#include "ADShareFolder.h"
#include "ADDomain.h"
#include "ADGroup.h"
#include "ADUser.h"
#include "CPing.h"

#include <Iads.h>
#include <AdsHlp.h>
#include <activeds.h>
#pragma comment(lib, "activeds.lib")
#pragma comment(lib, "adsiid.lib")

#include <COMUTIL.H>
#pragma comment(lib, "comsupp.lib")

#include <ntdsapi.h>
#pragma comment(lib, "ntdsapi.lib")

#include <lm.h>
#pragma comment(lib, "mpr.lib")
#pragma comment(lib, "netapi32.lib")

#include <sddl.h>

#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <ACLAPI.H>

IMPLEMENT_SERIAL(ADEntry, CObject, 0)

ADEntry::ADEntry()
{

}

void ADEntry::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		int i;
		map<CString, CString>::iterator it;

		ar << cn << dn << type;

		ar << dnKeys.size();
		for (i = 0; i < dnKeys.size(); i ++)
		{
			ar << dnKeys[i];
		}

		ar << dnValues.size();
		for (i = 0; i < dnValues.size(); i ++)
		{
			ar << dnValues[i];
		}

		ar << infos.size();
		for (i = 0, it = infos.begin(); i < infos.size(); i ++, it ++)
		{
			ar << it->first << it->second;
		}
	}
	else
	{
		int i;
		int iSize;
		CString strTemp;
		CString strFirst;
		CString strSecond;

		ar >> cn >> dn >> type;

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strTemp;
			dnKeys.push_back(strTemp);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strTemp;
			dnValues.push_back(strTemp);
		}

		ar >> iSize;
		for (i = 0; i < iSize; i ++)
		{
			ar >> strFirst >> strSecond;
			infos.insert(pair<CString, CString>(strFirst, strSecond));
		}
	}
}

CString ADEntry::getOUDN()
{
	if (dnKeys.size() == 0)
		return dn;
	int iPos = 0;
	int iCount = 0;
	for (int i = 0; i < dn.GetLength(); i ++)
	{
		if (dn[i] == _T(','))
		{
			iCount ++;
			if (iCount == dnKeys.size())
			{
				iPos = i;
				break;
			}
		}
	}
	if (iCount != dnKeys.size())
	{
		MyMessageBox_Error(_T("getOUDN Error."), _T("Error"));
		return _T("");
	}
	iPos ++;
	return dn.Right(dn.GetLength() - iPos);
}

void ADEntry::setCN(CString a_cn)
{
	cn = a_cn;
}

void ADEntry::setDN(CString a_dn)
{
	dn = a_dn;
	CString strToSplit = dn;
	vector<CString> nstrSplited;
	while (TRUE)
	{
		CString n = strToSplit.SpanExcluding(_T(","));
		nstrSplited.push_back(n);
		strToSplit = strToSplit.Right(strToSplit.GetLength()-n.GetLength()-1);
		if (strToSplit.IsEmpty())
		{
			break;
		}
	}
	
	for (int i = 0; i < nstrSplited.size(); i ++)
	{
		if (nstrSplited[i].GetLength() <= 3)
		{
			MyMessageBox_Error(_T("setDN Error."), _T("Error"));
		}
		else
		{
			dnKeys.push_back(nstrSplited[i].Left(2));
			dnValues.push_back(nstrSplited[i].Right(nstrSplited[i].GetLength() - 3));
		}
	}
	
	while (dnKeys[dnKeys.size() - 1] == _T("DC"))
	{
		dnKeys.pop_back();
		dnValues.pop_back();
	}
}

CString ADEntry::popKeyValue()
{
	CString strLastValue = dnValues[dnValues.size() - 1];
	dnKeys.pop_back();
	dnValues.pop_back();
	return strLastValue;
}

void ADEntry::setType(CString a_type)
{
	type = a_type;
}

void ADEntry::addKeyValue2Info(CString strKey, CString strValue)
{
	infos.insert(pair<CString, CString>(strKey, strValue));
	//infos[strKey] = strValue;
}

IMPLEMENT_SERIAL(ADEntryForTrustInfo, CObject, 0)

ADEntryForTrustInfo::ADEntryForTrustInfo()
{

}

void ADEntryForTrustInfo::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << cn << dn << trustAttributes << trustDirection << trustType;
	}
	else
	{
		ar >> cn >> dn >> trustAttributes >> trustDirection >> trustType;
	}

	CObject::Serialize(ar);
}

void ADEntryForTrustInfo::setCN(CString a_cn)
{
	cn = a_cn;
}

void ADEntryForTrustInfo::setDN(CString a_dn)
{
	dn = a_dn;
}

IMPLEMENT_SERIAL(ADACE, CObject, 0)

ADACE::ADACE()
{
	allowed = -1;
	group = NULL;
	user = NULL;
	r = -1;
	w = -1;
	x = -1;
	type = -1;
	fatherFolder = NULL;
}

void ADACE::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << dn << allowed << name << refDomain << r << w << x << type;
		//Intialization needed: group, user --> ADShareFolder::clarifyAllGroupsAndUsers(2) [OK]
		//Intialization needed: fatherFolder --> ADShareFolder::connectFolderWithACEs [OK]
	}
	else
	{
		ar >> dn >> allowed >> name >> refDomain >> r >> w >> x >> type;
	}
	
	CObject::Serialize(ar);
}

ADACE* ADACE::duplicate()
{
	ADACE* resultACE = new ADACE();
	resultACE->allowed = allowed;
	resultACE->name = name;
	resultACE->refDomain = refDomain;
	resultACE->group = group;
	resultACE->user = user;
	resultACE->r = r;
	resultACE->w = w;
	resultACE->x = x;
	resultACE->type = type;
	resultACE->fatherFolder = fatherFolder;
	return resultACE;
}

ADACE* ADACE::duplicateToZero()
{
	ADACE* resultACE = new ADACE();
	resultACE->allowed = allowed;
	resultACE->name = name;
	resultACE->refDomain = refDomain;
	resultACE->group = group;
	resultACE->user = user;
	resultACE->r = 0;
	resultACE->w = 0;
	resultACE->x = 0;
	resultACE->type = -1;
	resultACE->fatherFolder = fatherFolder;
	return resultACE;
}

CString ADACE::toString()
{
	CString strResult;
	if (allowed)
	{
		strResult += _T("允许");
	}
	else
	{
		strResult += _T("禁止");
	}
	
	if (name == _T("Everyone"))
	{
		strResult += (name + _T("的"));
	}
	else
	{
		strResult += (refDomain + _T("\\") + name + _T("的"));
	}
	
	if (r)
	{
		strResult += _T("R");
	}
	if (w)
	{
		strResult += _T("W");
	}
	if (x)
	{
		strResult += _T("X");
	}
	
	strResult += _T("的");
	
	if (type == 0)
	{
		strResult += _T("NTFS");
	}
	else if (type == 1)
	{
		strResult += _T("Share");
	}
	
	strResult += _T("权限");
	
	return strResult;
}

CString ADDirectorySearch::m_strDomainName = _T("");
CString ADDirectorySearch::m_strUserName = _T("");
CString ADDirectorySearch::m_strPassword = _T("");

string ADDirectorySearch::ws2s(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring ADDirectorySearch::s2ws(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

CString ADDirectorySearch::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

int ADDirectorySearch::stoi(CString s)
{
	return atoi(s);
}

void ADDirectorySearch::setCredential(CString strDomainName, CString strUserName, CString strPassword)
{
	m_strDomainName = strDomainName;
	m_strUserName = strUserName;
	m_strPassword = strPassword;
}

vector<ADEntry*> ADDirectorySearch::getEntries(CString strPathName, CString strUserName, CString strPassWord,
											   CString strFilter, CString strType)
{
	vector<ADEntry*> entries;

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		//cout << "succeed 1" << endl;
		IDirectorySearch* pSearch = NULL;
		//hr = ADsGetObject(L"LDAP://192.168.1.5", IID_IDirectorySearch, (void **) &pSearch);
		//hr = ADsOpenObject(L"LDAP://192.168.1.5/OU=XiangDingZu,DC=domaintest,DC=net",
		//	_bstr_t("Administrator"), _bstr_t("123456Ad"), ADS_SECURE_AUTHENTICATION, IID_IDirectorySearch, (void**)&pSearch);
		hr = ADsOpenObject(_bstr_t(strPathName),
			_bstr_t(strUserName), _bstr_t(strPassWord), ADS_SECURE_AUTHENTICATION, IID_IDirectorySearch, (void**)&pSearch);
		if (SUCCEEDED(hr))
		{
			//cout << "succeed 2" << endl;
			ADS_SEARCH_HANDLE hSearch;
			LPWSTR pszAttr[] =
			{
				L"name", L"distinguishedName", L"sAMAccountType", L"canonicalName", L"cn", L"name", L"distinguishedName",
				L"createTimeStamp", L"modifyTimeStamp", L"whenChanged", L"whenCreated",
				L"objectClass", L"objectCategory", L"objectGUID", L"objectSid", 
			};
			DWORD dwCount = sizeof(pszAttr) / sizeof(LPWSTR);
			//hr = pSearch->ExecuteSearch(L"(& (objectClass=*) (objectCategory=*) (name=*))", pszAttr, dwCount, &hSearch);
			hr = pSearch->ExecuteSearch(_bstr_t(strFilter), pszAttr, dwCount, &hSearch);
			if (SUCCEEDED(hr))
			{
				//cout << "succeed 3" << endl;
				for (hr = pSearch->GetFirstRow(hSearch); SUCCEEDED(hr) && hr != S_ADS_NOMORE_ROWS; hr = pSearch->GetNextRow(hSearch))
				{
					//wprintf(L"********************************************************\n");
					ADEntry *entry = new ADEntry();
					entry->setType(strType);
					LPWSTR pszAttrName = NULL;
					while (hr = pSearch->GetNextColumnName(hSearch, &pszAttrName) != S_ADS_NOMORE_COLUMNS)
					{
						ADS_SEARCH_COLUMN sColumn;
						hr = pSearch->GetColumn(hSearch, pszAttrName, &sColumn);
						if (SUCCEEDED(hr))
						{
							//wprintf(L"********************************************************\n");
							//wprintf(L"%s\n", sColumn.pszAttrName);
							CString strKey = sColumn.pszAttrName;
							CString strValue;
							
							unsigned int nValue;
							switch (sColumn.dwADsType)
							{
							case ADSTYPE_DN_STRING:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									//wprintf(L"%s\r\n", sColumn.pADsValues[nValue].DNString);
									strValue += sColumn.pADsValues[nValue].DNString;
								break;
							case ADSTYPE_CASE_EXACT_STRING:
							case ADSTYPE_CASE_IGNORE_STRING:
							case ADSTYPE_PRINTABLE_STRING:
							case ADSTYPE_NUMERIC_STRING:
							case ADSTYPE_TYPEDNAME:
							case ADSTYPE_FAXNUMBER:
							case ADSTYPE_PATH:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									//wprintf(L"%s\r\n", sColumn.pADsValues[nValue].CaseIgnoreString);
									strValue += sColumn.pADsValues[nValue].CaseIgnoreString;
								break;
							case ADSTYPE_BOOLEAN:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									LPOLESTR pszBool = NULL;
									DWORD dwBool = 0;
									dwBool = sColumn.pADsValues[nValue].Boolean;
									pszBool = dwBool ? L"TRUE" : L"FALSE";
									//wprintf(L"%s\r\n", pszBool);
									strValue += pszBool;
								}
								break;
							case ADSTYPE_INTEGER:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									wchar_t temp[255];
									swprintf(temp, L"%X", sColumn.pADsValues[nValue].Integer);
									strValue += temp;
									//wprintf(L"%d\r\n", sColumn.pADsValues[nValue].Integer);
								}
								break;
							case ADSTYPE_OCTET_STRING:
								if (_wcsicmp(sColumn.pszAttrName, L"objectSID") == 0)
								{
									LPOLESTR szSID = NULL;
									for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									{
										PSID pObjectSID = (PSID)(sColumn.pADsValues[nValue].OctetString.lpValue); 
										// Convert SID to string. 
										ConvertSidToStringSidW(pObjectSID, &szSID); 
										//wprintf(L"%s\r\n", szSID); 
										strValue += szSID;
										LocalFree(szSID);
									}
								}
								else if ((_wcsicmp(sColumn.pszAttrName, L"objectGUID") == 0))
								{
									LPGUID pObjectGUID = NULL;
									LPOLESTR szDSGUID = new WCHAR[39];
									for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									{
										// Cast to LPGUID. 
										pObjectGUID = (LPGUID)(sColumn.pADsValues[nValue].OctetString.lpValue); 
										// Convert GUID to string. 
										::StringFromGUID2(*pObjectGUID, szDSGUID, 39);   
										// Print the GUID. 
										//wprintf(L"%s\r\n", szDSGUID);
										strValue += szDSGUID;
									}
								}
								else
									//wprintf(L"Value of type Octet String. No Conversion.\n"); 
								break;
							case ADSTYPE_UTC_TIME:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									SYSTEMTIME systemtime;
									DATE date;
									systemtime = sColumn.pADsValues[nValue].UTCTime; 
									if (SystemTimeToVariantTime(&systemtime, &date) != 0)
									{
										//   Pack   in   variant.vt. 
										VARIANT varDate;
										varDate.vt = VT_DATE; 
										varDate.date = date; 
										VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR); 
										//wprintf(L"%s\r\n", varDate.bstrVal);
										strValue += varDate.bstrVal;
										VariantClear(&varDate);
									}
									else
										;//wprintf(L"Could not convert UTC-Time.\n");
								} 
								break;
							case ADSTYPE_LARGE_INTEGER:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									LARGE_INTEGER liValue;
									FILETIME filetime;
									SYSTEMTIME systemtime;
									VARIANT varDate;
									DATE date;
									liValue = sColumn.pADsValues[nValue].LargeInteger; 
									filetime.dwLowDateTime = liValue.LowPart; 
									filetime.dwHighDateTime = liValue.HighPart; 
									if ((filetime.dwHighDateTime == 0) && (filetime.dwLowDateTime == 0))
									{
										//wprintf(L"No value set.\n");
									}
									else
									{
										//   Verify   properties   of   type   LargeInteger   that   represent   time. 
										//   If   TRUE,   then   convert   to   variant   time. 
										if ((0 == wcscmp(L"accountExpires", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"badPasswordTime", sColumn.pszAttrName))||
											(0 == wcscmp(L"lastLogon", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"lastLogoff", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"lockoutTime", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"pwdLastSet", sColumn.pszAttrName)))
										{
											//   Handle   special   case   for   Never   Expires   where   low   part   is   -1. 
											if (filetime.dwLowDateTime == -1)
											{
												//wprintf(L"Never Expires.\n");
											}
											else
											{
												if (FileTimeToLocalFileTime(&filetime, &filetime) != 0)
												{
													if (FileTimeToSystemTime(&filetime, &systemtime) != 0)
													{
														if (SystemTimeToVariantTime(&systemtime, &date) != 0)
														{
															//   Pack   in   variant.vt. 
															varDate.vt = VT_DATE; 
															varDate.date = date; 
															VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR); 
															//wprintf(L"%s\r\n", varDate.bstrVal); 
															VariantClear(&varDate);
														}
														else
														{
															//wprintf(L"FileTimeToVariantTime failed\n");
														}
													}
													else
													{
														//wprintf(L"FileTimeToSystemTime failed\n");
													}
												}
												else
												{
													//wprintf(L"FileTimeToLocalFileTime failed\n");
												}
											}
										}
										else
										{
											//   Print   the   LargeInteger. 
											wprintf(L"high:%d low:%d\r\n", filetime.dwHighDateTime, filetime.dwLowDateTime);
										}
									}
								} 
								break; 
							case ADSTYPE_NT_SECURITY_DESCRIPTOR:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									//wprintf(L"Security descriptor.\n");
								} 
								break; 
							default:
								;//wprintf(L"Unknown type %d.\n", sColumn.dwADsType);
							}

							entry->addKeyValue2Info(strKey, strValue);
							if (strKey == _T("name"))
								entry->setCN(strValue);
							else if (strKey == _T("distinguishedName"))
								entry->setDN(strValue);
							else if (strKey == _T("sAMAccountType"))
							{
								if (strType == _T("group"))
								{
									if (strValue == _T("20000000"))
										entry->setType(L"localgroup");
									else if (strValue == L"10000000")
										entry->setType(L"globalgroup");
									else
										MyMessageBox_Error(_T("getEntries Error."), _T("Error"));
								}
							}
							//else
							//	MyMessageBox_Error(_T("getEntries Error."), _T("Error"));

							pSearch->FreeColumn(&sColumn);
						}
						FreeADsMem(pszAttrName);
					}
					entries.push_back(entry);
				}
				pSearch->CloseSearchHandle(hSearch);
			}
			pSearch->Release();
		}
		CoUninitialize();
	}
	return entries;
}

vector<ADEntryForTrustInfo*> ADDirectorySearch::getEntriesForTrustInfo(CString strPathName, CString strUserName, CString strPassWord,
											   CString strFilter, CString strType)
{
	vector<ADEntryForTrustInfo*> entries;

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		//cout << "succeed 1" << endl;
		IDirectorySearch* pSearch = NULL;
		//hr = ADsGetObject(L"LDAP://192.168.1.5", IID_IDirectorySearch, (void **) &pSearch);
		//hr = ADsOpenObject(L"LDAP://192.168.1.5/OU=XiangDingZu,DC=domaintest,DC=net",
		//	_bstr_t("Administrator"), _bstr_t("123456Ad"), ADS_SECURE_AUTHENTICATION, IID_IDirectorySearch, (void**)&pSearch);
		hr = ADsOpenObject(_bstr_t(strPathName),
			_bstr_t(strUserName), _bstr_t(strPassWord), ADS_SECURE_AUTHENTICATION, IID_IDirectorySearch, (void**)&pSearch);
		if (SUCCEEDED(hr))
		{
			//cout << "succeed 2" << endl;
			ADS_SEARCH_HANDLE hSearch;
			LPWSTR pszAttr[] =
			{
				L"name", L"distinguishedName", L"trustAttributes", L"trustDirection", L"trustType"
			};
			DWORD dwCount = sizeof(pszAttr) / sizeof(LPWSTR);
			//hr = pSearch->ExecuteSearch(L"(& (objectClass=*) (objectCategory=*) (name=*))", pszAttr, dwCount, &hSearch);
			hr = pSearch->ExecuteSearch(_bstr_t(strFilter), pszAttr, dwCount, &hSearch);
			if (SUCCEEDED(hr))
			{
				//cout << "succeed 3" << endl;
				for (hr = pSearch->GetFirstRow(hSearch); SUCCEEDED(hr) && hr != S_ADS_NOMORE_ROWS; hr = pSearch->GetNextRow(hSearch))
				{
					//wprintf(L"********************************************************\n");
					ADEntryForTrustInfo *entry = new ADEntryForTrustInfo();
					LPWSTR pszAttrName = NULL;
					while (hr = pSearch->GetNextColumnName(hSearch, &pszAttrName) != S_ADS_NOMORE_COLUMNS)
					{
						ADS_SEARCH_COLUMN sColumn;
						hr = pSearch->GetColumn(hSearch, pszAttrName, &sColumn);
						if (SUCCEEDED(hr))
						{
							//wprintf(L"********************************************************\n");
							//wprintf(L"%s\n", sColumn.pszAttrName);
							CString strKey = sColumn.pszAttrName;
							CString strValue;
							
							unsigned int nValue;
							switch (sColumn.dwADsType)
							{
							case ADSTYPE_DN_STRING:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									//wprintf(L"%s\r\n", sColumn.pADsValues[nValue].DNString);
									strValue += sColumn.pADsValues[nValue].DNString;
								break;
							case ADSTYPE_CASE_EXACT_STRING:
							case ADSTYPE_CASE_IGNORE_STRING:
							case ADSTYPE_PRINTABLE_STRING:
							case ADSTYPE_NUMERIC_STRING:
							case ADSTYPE_TYPEDNAME:
							case ADSTYPE_FAXNUMBER:
							case ADSTYPE_PATH:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									//wprintf(L"%s\r\n", sColumn.pADsValues[nValue].CaseIgnoreString);
									strValue += sColumn.pADsValues[nValue].CaseIgnoreString;
								break;
							case ADSTYPE_BOOLEAN:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									LPOLESTR pszBool = NULL;
									DWORD dwBool = 0;
									dwBool = sColumn.pADsValues[nValue].Boolean;
									pszBool = dwBool ? L"TRUE" : L"FALSE";
									//wprintf(L"%s\r\n", pszBool);
									strValue += pszBool;
								}
								break;
							case ADSTYPE_INTEGER:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									wchar_t temp[255];
									swprintf(temp, L"%X", sColumn.pADsValues[nValue].Integer);
									strValue += temp;
									//wprintf(L"%d\r\n", sColumn.pADsValues[nValue].Integer);
								}
								break;
							case ADSTYPE_OCTET_STRING:
								if (_wcsicmp(sColumn.pszAttrName, L"objectSID") == 0)
								{
									LPOLESTR szSID = NULL;
									for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									{
										PSID pObjectSID = (PSID)(sColumn.pADsValues[nValue].OctetString.lpValue); 
										// Convert SID to string. 
										ConvertSidToStringSidW(pObjectSID, &szSID); 
										//wprintf(L"%s\r\n", szSID); 
										LocalFree(szSID);
									}
								}
								else if ((_wcsicmp(sColumn.pszAttrName, L"objectGUID") == 0))
								{
									LPGUID pObjectGUID = NULL;
									LPOLESTR szDSGUID = new WCHAR[39];
									for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
									{
										// Cast to LPGUID. 
										pObjectGUID = (LPGUID)(sColumn.pADsValues[nValue].OctetString.lpValue); 
										// Convert GUID to string. 
										::StringFromGUID2(*pObjectGUID, szDSGUID, 39);   
										// Print the GUID. 
										//wprintf(L"%s\r\n", szDSGUID);
									}
								}
								else
									//wprintf(L"Value of type Octet String. No Conversion.\n"); 
								break;
							case ADSTYPE_UTC_TIME:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									SYSTEMTIME systemtime;
									DATE date;
									systemtime = sColumn.pADsValues[nValue].UTCTime; 
									if (SystemTimeToVariantTime(&systemtime, &date) != 0)
									{
										//   Pack   in   variant.vt. 
										VARIANT varDate;
										varDate.vt = VT_DATE; 
										varDate.date = date; 
										VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR); 
										//wprintf(L"%s\r\n", varDate.bstrVal); 
										VariantClear(&varDate);
									}
									else
										;//wprintf(L"Could not convert UTC-Time.\n");
								} 
								break;
							case ADSTYPE_LARGE_INTEGER:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									LARGE_INTEGER liValue;
									FILETIME filetime;
									SYSTEMTIME systemtime;
									VARIANT varDate;
									DATE date;
									liValue = sColumn.pADsValues[nValue].LargeInteger; 
									filetime.dwLowDateTime = liValue.LowPart; 
									filetime.dwHighDateTime = liValue.HighPart; 
									if ((filetime.dwHighDateTime == 0) && (filetime.dwLowDateTime == 0))
									{
										//wprintf(L"No value set.\n");
									}
									else
									{
										//   Verify   properties   of   type   LargeInteger   that   represent   time. 
										//   If   TRUE,   then   convert   to   variant   time. 
										if ((0 == wcscmp(L"accountExpires", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"badPasswordTime", sColumn.pszAttrName))||
											(0 == wcscmp(L"lastLogon", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"lastLogoff", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"lockoutTime", sColumn.pszAttrName)) ||
											(0 == wcscmp(L"pwdLastSet", sColumn.pszAttrName)))
										{
											//   Handle   special   case   for   Never   Expires   where   low   part   is   -1. 
											if (filetime.dwLowDateTime == -1)
											{
												//wprintf(L"Never Expires.\n");
											}
											else
											{
												if (FileTimeToLocalFileTime(&filetime, &filetime) != 0)
												{
													if (FileTimeToSystemTime(&filetime, &systemtime) != 0)
													{
														if (SystemTimeToVariantTime(&systemtime, &date) != 0)
														{
															//   Pack   in   variant.vt. 
															varDate.vt = VT_DATE; 
															varDate.date = date; 
															VariantChangeType(&varDate, &varDate, VARIANT_NOVALUEPROP, VT_BSTR); 
															//wprintf(L"%s\r\n", varDate.bstrVal); 
															VariantClear(&varDate);
														}
														else
														{
															//wprintf(L"FileTimeToVariantTime failed\n");
														}
													}
													else
													{
														//wprintf(L"FileTimeToSystemTime failed\n");
													}
												}
												else
												{
													//wprintf(L"FileTimeToLocalFileTime failed\n");
												}
											}
										}
										else
										{
											//   Print   the   LargeInteger. 
											wprintf(L"high:%d low:%d\r\n", filetime.dwHighDateTime, filetime.dwLowDateTime);
										}
									}
								} 
								break; 
							case ADSTYPE_NT_SECURITY_DESCRIPTOR:
								for (nValue = 0; nValue < sColumn.dwNumValues; nValue ++)
								{
									//wprintf(L"Security descriptor.\n");
								} 
								break; 
							default:
								;//wprintf(L"Unknown type %d.\n", sColumn.dwADsType);
							}
							if (strKey == _T("name"))
							{
								entry->cn = strValue;
							}
							else if (strKey == _T("distinguishedName"))
							{
								entry->dn = strValue;
							}
							else if (strKey == _T("trustAttributes"))
							{
								if (strValue.GetLength() == 0)
								{
									MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));	
								}
								else
								{
									switch (strValue[0])
									{
									case _T('1'):
										strValue = _T("NON_TRANSITIVE");
										break;
									case _T('2'):
										strValue = _T("UPLEVEL_ONLY");
										break;
									case _T('4'):
										strValue = _T("TREE_PARENT");
										break;
									case _T('8'):
										strValue = _T("TREE_ROOT");
										break;
									default:
										MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));
										break;
									}
								}
								entry->trustAttributes = strValue;
							}
							else if (strKey == _T("trustDirection"))
							{
								if (strValue.GetLength() == 0)
								{
									MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));
								}
								{
									switch (strValue[0])
									{
									case _T('0'):
										strValue = _T("DISABLED");
										break;
									case _T('1'):
										strValue = _T("INBOUND");
										break;
									case _T('2'):
										strValue = _T("OUTBOUND");
										break;
									case _T('3'):
										strValue = _T("BIDIRECTIONAL");
										break;
									default:
										MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));
										break;
									}
								}
								entry->trustDirection = strValue;
							}
							else if (strKey == _T("trustType"))
							{
								if (strValue.GetLength() == 0)
								{
									MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));
								}
								{
									switch (strValue[0])
									{
									case _T('1'):
										strValue = _T("DOWNLEVEL");
										break;
									case _T('2'):
										strValue = _T("UPLEVEL");
										break;
									case _T('3'):
										strValue = _T("MIT");
										break;
									case _T('4'):
										strValue = _T("DCE");
										break;
									default:
										MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));
										break;
									}
								}
								entry->trustType = strValue;
							}
							else
								MyMessageBox_Error(_T("getEntriesForTrustInfo Error."), _T("Error"));

							pSearch->FreeColumn(&sColumn);
						}
						FreeADsMem(pszAttrName);
					}
					entries.push_back(entry);
				}
				pSearch->CloseSearchHandle(hSearch);
			}
			pSearch->Release();
		}
		CoUninitialize();
	}
	return entries;
}

CString ADDirectorySearch::convertName(CString strName, CString strDCDNSName, CString strDomainName, CString strUserName, CString strPassWord, 
									   int inTypeFlag, int outTypeFlag)
{
	CString strResult;
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		//名字转换
		//cout << "succeed 1" << endl;
		RPC_AUTH_IDENTITY_HANDLE hCreds;
		DWORD dwResult = DsMakePasswordCredentialsW(_bstr_t(strUserName), _bstr_t(strDomainName), _bstr_t(strPassWord), &hCreds);
		if (dwResult == ERROR_SUCCESS)
		{
			//cout << "succeed 2" << endl;
			HANDLE hDS = NULL;
			DWORD dwResult = DsBindWithCredW(_bstr_t(strDCDNSName), NULL, hCreds, &hDS);

			if (dwResult == ERROR_SUCCESS)
			{
				//cout << "succeed 3" << endl;
				//LPTSTR apszNames[] = {L"domaintest.net/"};
				
				DS_NAME_RESULTW* psNames;
				//LPWSTR apszName = CStringW2LPWSTR(strName);
				LPWSTR lpStrName = _bstr_t(strName);
				dwResult = DsCrackNamesW(hDS, DS_NAME_NO_FLAGS, (DS_NAME_FORMAT) inTypeFlag, (DS_NAME_FORMAT) outTypeFlag, 1, &lpStrName, &psNames);
				//delete[] apszName;

				if (dwResult == ERROR_SUCCESS)
				{
					//cout << "succeed 4" << endl;
					if (psNames->cItems != 1)
					{
						MyMessageBox_Error(_T("convertName Error."), _T("Error"));
						return _T("");
					}
					for (unsigned int nName = 0; nName < psNames->cItems; nName ++)
					{
						if (psNames->rItems[nName].status == ERROR_SUCCESS)
						{
							//wprintf(L"Name:%s Domain:%s\n", psNames->rItems[nName].pName, psNames->rItems[nName].pDomain);
							strResult = psNames->rItems[nName].pName;
						}
					}
					DsFreeNameResultW(psNames);
				}
				DsUnBindW(&hDS);
			}
			DsFreePasswordCredentials(hCreds);
		}
		CoUninitialize();
	}
	return strResult;
}

CString ADDirectorySearch::host2IP(CString strHostName)
{
	WSADATA wsa; 
	WORD wVersionRequested; 
	wVersionRequested = MAKEWORD(1, 1);  
	if (WSAStartup(wVersionRequested , &wsa)!=0) 
	{
		MyMessageBox_Error(_T("host2IP Error."), _T("Error"));
		return _T("");
	}
	else
	{
		CString strResult;
		CStringArray nstrIPAddress;
		//wstring wstrTemp = strHostName;
		//string strTemp = ws2s(wstrTemp);
		//struct hostent FAR* lpHostEnt = gethostbyname(strTemp.c_str());
		struct hostent FAR* lpHostEnt = gethostbyname(_bstr_t("AkiSn0w-LabPC"));
		nstrIPAddress.RemoveAll();
		if (lpHostEnt == NULL)
		{
			//没找到主机
			//MessageBoxW(NULL, L"host2IP Error.", L"Error", MB_OK);
			int a = WSAGetLastError();
			WSACleanup();
			return _T("");
		}
		//获取IP
		int i = 0;
		LPSTR lpAddr = lpHostEnt->h_addr_list[i];
		CString temp;
		while (lpAddr)
		{
			i ++;
			struct in_addr inAddr;
			memmove(&inAddr, lpAddr, 4);
			//转换为标准格式
			temp = inet_ntoa(inAddr);
			if(temp.IsEmpty())
				break;
			nstrIPAddress.Add(temp);
			lpAddr = lpHostEnt->h_addr_list[i];
		}

		for (i = 0; i < nstrIPAddress.GetSize(); i ++)
		{
			strResult += nstrIPAddress[i];
		}
		WSACleanup();
		return strResult;
	}
}

CString ADDirectorySearch::host2IP2(CString strHostName)
{
	WSADATA wsa; 
	WORD wVersionRequested; 
	wVersionRequested = MAKEWORD(1, 1);  
	if (WSAStartup(wVersionRequested , &wsa)!=0) 
	{
		MyMessageBox_Error(_T("host2IP2 Error."), _T("Error"));
		return _T("");
	}
	else
	{
		DWORD dwRetval;
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_IP;

		dwRetval = getaddrinfo(_bstr_t(strHostName), NULL, &hints, &result);
		if (dwRetval != 0)
		{
			//printf("getaddrinfo failed with error: %d\n", dwRetval);
			WSACleanup();
			return L"";
		}

		struct sockaddr_in *sockaddr_ipv4;
		CString strResult;
		for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			strResult = inet_ntoa(sockaddr_ipv4->sin_addr);
			if (strResult != "0.0.0.0" && checkIPAlive(strResult))
			{
				break;
			}
		}
		WSACleanup();
		return strResult;
	}
}

CString ADDirectorySearch::host2IP3(CString strHostName)
{
    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    DWORD dwError;
    struct hostent *remoteHost;
    //char *host_name;
	CString host_name;
    struct in_addr addr;
	
    // Initialize Winsock
	WORD wVersionRequested =MAKEWORD(1, 1);
    iResult = WSAStartup(wVersionRequested, &wsaData);
    if (iResult != 0)
	{
        //printf("WSAStartup failed: %d\n", iResult);
        //return 1;
		//MyMessageBox_Error(_T("host2IP3"));
        return _T("");
    }
	
    host_name = strHostName;
	// If the user input is an alpha name for the host, use gethostbyname()
	// If not, get host by addr (assume IPv4)
    if (isalpha(host_name[0]))
	{   /* host address is a name */
        //printf("Calling gethostbyname with %s\n", host_name);
        remoteHost = gethostbyname(host_name);
    }
	else
	{
        //printf("Calling gethostbyaddr with %s\n", host_name);
        addr.s_addr = inet_addr(host_name);
        if (addr.s_addr == INADDR_NONE)
		{
            //printf("The IPv4 address entered must be a legal address\n");
			//MyMessageBox_Error(_T("host2IP3"));
            return _T("");
        }
		else
		{
            remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
		}
    }
	
    if (remoteHost == NULL)
	{
        dwError = WSAGetLastError();
        if (dwError != 0)
		{
            if (dwError == WSAHOST_NOT_FOUND)
			{
                //printf("Host not found\n");
                //return 1;
				//MyMessageBox_Error(_T("host2IP3"));
				return _T("");
            }
			else if (dwError == WSANO_DATA)
			{
                //printf("No data record found\n");
                //return 1;
				//MyMessageBox_Error(_T("host2IP3"));
				return _T("");
            }
			else
			{
                //printf("Function failed with error: %ld\n", dwError);
                //return 1;
				//MyMessageBox_Error(_T("host2IP3"));
				return _T("");
            }
        }
		else
		{
			//MyMessageBox_Error(_T("host2IP3"));
			return _T("");
		}
    }
	else
	{
        printf("Function returned:\n");
        printf("\tOfficial name: %s\n", remoteHost->h_name);
        printf("\tAlternate names: %s\n", remoteHost->h_aliases);
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype)
		{
        case AF_INET:
            printf("AF_INET\n");
            break;
        case AF_INET6:
            printf("AF_INET6\n");
            break;
        case AF_NETBIOS:
            printf("AF_NETBIOS\n");
            break;
        default:
            printf(" %d\n", remoteHost->h_addrtype);
            break;
        }
        printf("\tAddress length: %d\n", remoteHost->h_length);
        addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
        printf("\tFirst IP Address: %s\n", inet_ntoa(addr));
		
		CString strHostName = inet_ntoa(addr);
		return strHostName;
    }
}

CString ADDirectorySearch::ip2Host(CString strIP)
{
	char *ip = strIP.GetBuffer(0);
	WSADATA wsaData;
	int iResult;
	DWORD dwRetval;
	struct sockaddr_in saGNI;
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	// Assume we plan to use TCP port 27015
	u_short port = 27015;
	
	// Validate the parameters
	// Initialize Winsock
	WORD wVersionRequested =MAKEWORD(1, 1);
	iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0)
	{
		//Handle Error iResult
		return ("Error");
	}
	//—————————————–
	// Set up sockaddr_in structure which is passed
	// to the getnameinfo function
	saGNI.sin_family = AF_INET;
	saGNI.sin_addr.s_addr = inet_addr(ip);
	saGNI.sin_port = htons(port);
	//—————————————–
	// Call getnameinfo
	dwRetval = getnameinfo((struct sockaddr *) &saGNI, sizeof (struct sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);
	if (dwRetval != 0)
	{
		//Handle Error dwRetval
		return("Error");
	}
	// Terminate the use of Winsock
	WSACleanup();
	
	if (dwRetval != 0)
	{
		//Handle Error dwRetval
		return ("Error");
	}
	else
	{
		//Return the Computer name if found
		CString strHostName = hostname;
		return strHostName;
	}
}

BOOL ADDirectorySearch::checkIP(CString strIP)
{
	char *str = strIP.GetBuffer(0);

    int segs = 0;   /* Segment count. */
    int chcnt = 0;  /* Character count within segment. */
    int accum = 0;  /* Accumulator for segment. */

    /* Catch NULL pointer. */

    if (str == NULL)
        return 0;

    /* Process every character in string. */

    while (*str != '\0') {
        /* Segment changeover. */

        if (*str == '.') {
            /* Must have some digits in segment. */

            if (chcnt == 0)
                return 0;

            /* Limit number of segments. */
			
            if (++segs == 4)
                return 0;
			
            /* Reset segment values and restart loop. */
			
            chcnt = accum = 0;
            str++;
            continue;
        }
		
		
        /* Check numeric. */
		
        if ((*str < '0') || (*str > '9'))
            return 0;
		
        /* Accumulate and check segment. */
		
        if ((accum = accum * 10 + *str - '0') > 255)
            return 0;
		
        /* Advance other segment specific stuff and continue loop. */
		
        chcnt++;
        str++;
    }
	
    /* Check enough segments and enough characters in last segment. */
	
    if (segs != 3)
        return 0;
	
    if (chcnt == 0)
        return 0;
	
    /* Address okay. */
	
    return 1;
}

BOOL ADDirectorySearch::checkIPAlive(CString strIP)
{
	CPing ping;
	BOOL bResult = ping.Ping(_bstr_t(strIP));
	return bResult;
}

BOOL ADDirectorySearch::IPCConnect(CString strServerName, CString strDomainName, CString strUserName, CString strPassWord)
{
	CString strServerNameWithSlash = _T("\\\\") + strServerName;
	CString strFullUserName = strDomainName + _T("\\") + strUserName;
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;
	_bstr_t bstrFullUserName = strFullUserName;
	_bstr_t bstrPassWord = strPassWord;
	DWORD dwResult;
	NETRESOURCEW netResource;
	memset(&netResource, 0, sizeof(netResource));
	netResource.dwScope = RESOURCE_GLOBALNET;  
	netResource.dwType = RESOURCETYPE_DISK;
	netResource.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;  
	netResource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	netResource.lpProvider = L"";
	netResource.lpRemoteName = bstrServerNameWithSlash;//远程主机名或IP如://192.168.1.11
 	dwResult = WNetAddConnection2W(&netResource, bstrPassWord, bstrFullUserName, CONNECT_INTERACTIVE);
	if (dwResult == ERROR_SESSION_CREDENTIAL_CONFLICT)
	{
		WNetCancelConnection2W(bstrServerNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
		dwResult = WNetAddConnection2W(&netResource, bstrPassWord, bstrFullUserName, CONNECT_INTERACTIVE);
		if (dwResult == ERROR_SESSION_CREDENTIAL_CONFLICT)
		{
			WinExec("NET USE * /DELETE /Y", SW_HIDE);
			//Sleep(1000);
			dwResult = WNetAddConnection2W(&netResource, bstrPassWord, bstrFullUserName, CONNECT_INTERACTIVE);
		}
	}

	if (dwResult == NO_ERROR)
	{
		return TRUE;
	}
	else if (dwResult == ERROR_SESSION_CREDENTIAL_CONFLICT)
	{
		return TRUE;
	}
	else
	{
		//MyMessageBox_Error(_T("IPCConnect Error."), _T("Error"));
		return FALSE;
	}
}

BOOL ADDirectorySearch::IPCDisconnect(CString strServerName)
{
	CString strServerNameWithSlash = _T("\\\\") + strServerName;
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;
	DWORD dwResult = WNetCancelConnection2W(bstrServerNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
	if (dwResult == NO_ERROR)
	{
		return TRUE;
	}
	else
	{
		//MyMessageBox_Error(_T("IPCDisconnect Error."), _T("Error"));
		return FALSE;
	}
}

BOOL ADDirectorySearch::IPCDisconnectShare(CString strServerName, CString strFileName)
{
	CString strServerFileNameWithSlash = _T("\\\\") + strServerName + _T("\\") + strFileName;
	_bstr_t bstrServerFileNameWithSlash = strServerFileNameWithSlash;
	DWORD dwResult = WNetCancelConnection2W(bstrServerFileNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
	if (dwResult == NO_ERROR)
	{
		return TRUE;
	}
	else
	{
		//MyMessageBox_Error(_T("IPCDisconnectShare Error."), _T("Error"));
		return FALSE;
	}
}

CString ADDirectorySearch::IPCConnectAsLocalUser(CString strServerName, CString strUserName, CString strPassWord, CString strServerIP)
{
	int iAllDone = 0;
	CString strValidIPCName = strServerName;
	CString strServerNameWithSlash = _T("\\\\") + strServerName;
	CString strFullUserName = strUserName;
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;
	_bstr_t bstrFullUserName = strFullUserName;
	_bstr_t bstrPassWord = strPassWord;
	DWORD dwResult;
	NETRESOURCEW netResource;
	memset(&netResource, 0, sizeof(netResource));
	netResource.dwScope = RESOURCE_CONNECTED; //RESOURCE_GLOBALNET;
	netResource.dwType = RESOURCETYPE_ANY; //RESOURCETYPE_DISK;
	netResource.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;  
	netResource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	netResource.lpProvider = L"";
	//CString strIP = _T("\\\\192.168.1.110");
	//_bstr_t bstrIP = strIP;
	//netResource.lpRemoteName = bstrIP;
L1:	netResource.lpRemoteName = bstrServerNameWithSlash; //远程主机名或IP如://192.168.1.11
	dwResult = WNetAddConnection2W(&netResource, bstrPassWord, bstrFullUserName, CONNECT_INTERACTIVE);
	if ((dwResult != NO_ERROR) && (dwResult == ERROR_SESSION_CREDENTIAL_CONFLICT || iAllDone == 0))
	{
		//return strValidIPCName; //直接用建好的连接即可，下面的似乎过于繁琐
		if (dwResult == ERROR_SESSION_CREDENTIAL_CONFLICT)
		{
			WinExec("NET USE * /DELETE /Y", SW_HIDE);
			dwResult = WNetCancelConnection2W(bstrServerNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
			goto L1;
		}
		else
		{
			strValidIPCName = strServerIP;
			CString strServerIPWithSlash = _T("\\\\") + strServerIP;
			bstrServerNameWithSlash = strServerIPWithSlash;
			iAllDone = 1;
			goto L1;
		}
	}

	if (dwResult == NO_ERROR)
	{
		return strValidIPCName;
	}
	else
	{
		MyMessageBox_Error(_T("IPCConnectAsLocalUser Error.\n请确定网络是否连通."), _T("Error"));
		return _T("");
	}
}

BOOL ADDirectorySearch::IPCDisconnectAsLocalUser(CString strServerName, CString strServerIP)
{
	CString strServerNameWithSlash = _T("\\\\") + strServerName;
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;
	DWORD dwResult = WNetCancelConnection2W(bstrServerNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
	if (dwResult == NO_ERROR)
	{
		return TRUE;
	}
	else
	{
		CString strServerIPWithSlash = _T("\\\\") + strServerIP;
		bstrServerNameWithSlash = strServerIPWithSlash;
		dwResult = WNetCancelConnection2W(bstrServerNameWithSlash, CONNECT_UPDATE_PROFILE, TRUE);
		if (dwResult == NO_ERROR)
		{
			return TRUE;
		}
		else
		{
			MyMessageBox_Error(_T("IPCDisconnectAsLocalUser Error."), _T("Error"));
			return FALSE;
		}
	}
}

BOOL ADDirectorySearch::logOnByUserPassword(CString strDomainName, CString strUserName, CString strPassword)
{
	CString strUPNUserName = strUserName + _T("@") + strDomainName;

	HANDLE hToken;
	BOOL bResult;
	//bResult = LogonUser(strUserName, strDomainName, strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT,
	//	&hToken);
	if (strDomainName != _T(""))
	{
		bResult = LogonUser(strUPNUserName, _T(""), strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT, 
			&hToken); //LOGON32_LOGON_NEW_CREDENTIALS or LOGON32_LOGON_NETWORK_CLEARTEXT
	}
	else
	{
		bResult = LogonUser(strUserName, _T("."), strPassword, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT, 
			&hToken); //LOGON32_LOGON_NEW_CREDENTIALS
	}
	if (bResult == FALSE)
	{
		MyMessageBox_Error(_T("logOnByUserPassword Error."), _T("Error"));
		return FALSE;
	}
	else
	{
		bResult = ImpersonateLoggedOnUser(hToken);
		if (bResult == FALSE)
		{
			MyMessageBox_Error(_T("logOnByUserPassword Error."), _T("Error"));
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL ADDirectorySearch::logOff()
{
	BOOL bResult;
	bResult = RevertToSelf();
	if (bResult == FALSE)
	{
		MyMessageBox_Error(_T("logOff Error."), _T("Error"));
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

PSECURITY_DESCRIPTOR ADDirectorySearch::getNTFSSecDescriptor(CString strFileName, CString strServerName, 
															 CString strServerIP)
{
	CString strServerNameWithSlash = _T("\\\\") + strServerName;//"\\\\veotax3";
	CString strFilePathName = _T("\\\\") + strServerName + _T("\\") + strFileName;//"\\\\veotax3\\nrdc1001";
	CString strFilePathName2 = _T("\\\\") + strServerIP + _T("\\") + strFileName;//"\\\\192.168.1.7\\nrdc1001";
	_bstr_t bstrFilePathName = strFilePathName;

	BOOL bSuccess = FALSE;
	BYTE* pSecDescriptorBuf;
	DWORD dwSizeNeeded = 0;

label1:;
	bSuccess = GetFileSecurityW(bstrFilePathName, DACL_SECURITY_INFORMATION, NULL, 0, &dwSizeNeeded);
	if (0 == dwSizeNeeded)
	{
		if (strFilePathName != strFilePathName2 && strServerIP != _T(""))
		{
			strFilePathName = strFilePathName2;
			bstrFilePathName = strFilePathName2;
			goto label1;
		}
		else
		{
			MyMessageBox_Error(_T("getNTFSSecDescriptor Error."), _T("Error"));
			return NULL;
		}
		//dwSizeNeeded = 500;
		//pSecDescriptorBuf = new BYTE[dwSizeNeeded];
	}
	else
	{
		pSecDescriptorBuf = new BYTE[dwSizeNeeded];
	}

	// Retrieve security descriptor with DACL information
	bSuccess = GetFileSecurityW((BSTR) _bstr_t(strFilePathName),
								DACL_SECURITY_INFORMATION,
								pSecDescriptorBuf,
								dwSizeNeeded,
								&dwSizeNeeded);

	int a = GetLastError();

	// Check if we successfully retrieved security descriptor with DACL information
	if (!bSuccess)
	{
		MyMessageBox_Error(_T("getNTFSSecDescriptor Error."), _T("Error"));
		return NULL;
		//DWORD dwError = GetLastError();
		//cout << "Failed to get file security information (" << dwError << ")\n";
		//return E_FAIL;
	}
	else
	{
		return pSecDescriptorBuf;
	}
}

PSECURITY_DESCRIPTOR ADDirectorySearch::getNTFSSecDescriptor2(CString strFileName, CString strServerName, 
															 CString strServerIP)
{
	//CString strServerNameWithSlash = _T("\\\\") + strServerName;//"\\\\veotax3";
	CString strFilePathName = _T("\\\\") + strServerName + _T("\\") + strFileName;//"\\\\veotax3\\nrdc1001";
	CString strFilePathName2 = _T("\\\\") + strServerIP + _T("\\") + strFileName;//"\\\\192.168.1.7\\nrdc1001";
	CString strFilePathName3 = strFilePathName;
	_bstr_t bstrFilePathName = strFilePathName;
	
	BOOL bSuccess = FALSE;
	PSECURITY_DESCRIPTOR pSecDescriptorBuf = NULL;
	DWORD dwSizeNeeded = 0;
	BOOL iStartTakeOwnership = FALSE;
	
label2:;
	bSuccess = GetNamedSecurityInfoW(bstrFilePathName, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSecDescriptorBuf);
	//bSuccess = GetFileSecurityW(bstrFilePathName, DACL_SECURITY_INFORMATION, NULL, 0, &dwSizeNeeded);
	if (ERROR_SUCCESS != bSuccess)
	{	
		if (iStartTakeOwnership == FALSE)
		{
			ADPermissionSearch::Simple_takeOwnershipAndAddReadPermission(strServerName, strFileName, m_strDomainName,
				m_strUserName, m_strPassword);
			iStartTakeOwnership = TRUE;
			goto label2;
		}
		else if (strFilePathName != strFilePathName2)
		{
			strServerName = strServerIP;
			strFilePathName = strFilePathName2;
			bstrFilePathName = strFilePathName2;
			iStartTakeOwnership = FALSE;
			goto label2;
		}
		else
		{
			ADPermissionSearch::Simple_RestoreOwnershipAndPermission(strServerName, strFileName);
			MyMessageBox_Error(_T("getNTFSSecDescriptor2 Error."), _T("Error"));
			return NULL;
		}
	}
	else
	{
		ADPermissionSearch::Simple_RestoreOwnershipAndPermission(strServerName, strFileName);
		return pSecDescriptorBuf;
	}
}

PSECURITY_DESCRIPTOR ADDirectorySearch::getNTFSSecDescriptor3(CString strFileName, CString strServerName, 
															  CString strServerIP)
{
	int iTryTime = 1;
	int iTryTime2 = 1;
	//CString strServerNameWithSlash = _T("\\\\") + strServerName;//"\\\\veotax3";
	CString strFilePathName = _T("\\\\") + strServerName + _T("\\") + strFileName;//"\\\\veotax3\\nrdc1001";
	CString strFilePathName2 = _T("\\\\") + strServerIP + _T("\\") + strFileName;//"\\\\192.168.1.7\\nrdc1001";
	_bstr_t bstrFilePathName = strFilePathName;
	
	BOOL bSuccess = FALSE;
	PSECURITY_DESCRIPTOR pSecDescriptorBuf = NULL;
	DWORD dwSizeNeeded = 0;
	BOOL iAlreadyRegistryMethod = FALSE;
	
label3:;
	   bSuccess = GetNamedSecurityInfoW(bstrFilePathName, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSecDescriptorBuf);
	   //bSuccess = GetFileSecurityW(bstrFilePathName, DACL_SECURITY_INFORMATION, NULL, 0, &dwSizeNeeded);
	   if (ERROR_SUCCESS != bSuccess)
	   {
		   if (iAlreadyRegistryMethod == FALSE)
		   {
			   iAlreadyRegistryMethod = TRUE;
			   CString strPathFileName = ADPermissionSearch::getPathForShare(strFileName);
			   pSecDescriptorBuf = ADPermissionSearch::getNTFSSecDescriptorByCreateFile(strServerName, strServerIP, strPathFileName);
			   if (pSecDescriptorBuf)
			   {
				   return pSecDescriptorBuf;
			   }
			   else
			   {
				   goto label3;
			   }
		   }

		   iTryTime --;
		   if (iTryTime >= 0)
		   {
			   bstrFilePathName = strFilePathName2;
			   iAlreadyRegistryMethod = FALSE;
			   goto label3;
		   }

		   iTryTime2 --;
		   if (bSuccess == 87 && iTryTime2 >= 0)
		   {
			   iTryTime = 1;
			   bstrFilePathName = strFilePathName;
			   goto label3;
		   }

		   MyMessageBox_Error(_T("getNTFSSecDescriptor3 Error."), _T("Error"));
		   return NULL;
	   }
	   else
	   {
		   return pSecDescriptorBuf;
	   }
}

PSECURITY_DESCRIPTOR ADDirectorySearch::getShareSecDescriptor(CString strFileName, CString strServerName, CString strServerIP, BYTE* &byNeedToRelease)
{
	int iTryTime = 1;
	CString strServerNameWithSlash = _T("\\\\") + strServerName;//"\\\\veotax3";
	CString strServerIPWithSlash = _T("\\\\") + strServerIP;//"\\\\192.168.1.191";
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;
	//CString strFileName = strFileName;//"nrdc1001";
	_bstr_t bstrFileName = strFileName;

	BOOL bSuccess = FALSE;
	NET_API_STATUS dwStatus;
	SHARE_INFO_502* pShareInfo;
	PSECURITY_DESCRIPTOR pSecDescriptorBuf;
	BYTE* pBuffer = NULL; 

AA:	dwStatus = NetShareGetInfo(bstrServerNameWithSlash, bstrFileName, 502, &pBuffer); 
	if (dwStatus == NERR_Success)
	{
		pShareInfo = (SHARE_INFO_502 *) pBuffer; 
		pSecDescriptorBuf = pShareInfo->shi502_security_descriptor;
		//NetApiBufferFree(pBuffer);
		byNeedToRelease = pBuffer;
		return pSecDescriptorBuf;
	}
	else
	{
		iTryTime --;
		bstrServerNameWithSlash = strServerIPWithSlash;
		if (iTryTime >= 0)
		{
			goto AA;
		}
		MyMessageBox_Error(_T("getShareSecDescriptor Error."), _T("Error"));
		return NULL;
	}
}

PSECURITY_DESCRIPTOR ADDirectorySearch::getShareSecDescriptor2(CString strFileName, CString strServerName, CString strServerIP, BYTE* &byNeedToRelease)
{
	CString strServerNameWithSlash = _T("\\\\") + strServerName;//"\\\\veotax3";
	_bstr_t bstrServerNameWithSlash = strServerNameWithSlash;

	CString strFilePathName = _T("\\\\") + strServerName + _T("\\") + strFileName;//"\\\\veotax3\\nrdc1001";
	CString strFilePathName2 = _T("\\\\") + strServerIP + _T("\\") + strFileName;//"\\\\192.168.1.7\\nrdc1001";
	_bstr_t bstrFilePathName = strFilePathName;
	
	BOOL bSuccess = FALSE;
	NET_API_STATUS dwStatus;
	SHARE_INFO_502* pShareInfo;
	PSECURITY_DESCRIPTOR pSecDescriptorBuf;
	BYTE* pBuffer = NULL; 
label3:;
	dwStatus = NetShareGetInfo(bstrServerNameWithSlash, bstrFilePathName, 502, &pBuffer); 
	if (dwStatus != NERR_Success)
	{
		if (strFilePathName != strFilePathName2)
		{
			strFilePathName = strFilePathName2;
			bstrFilePathName = strFilePathName2;
			goto label3;
		}
		else
		{
			MyMessageBox_Error(_T("getShareSecDescriptor2 Error."), _T("Error"));
			return NULL;
		}
	}
	else
	{
		pShareInfo = (SHARE_INFO_502 *) pBuffer; 
		pSecDescriptorBuf = pShareInfo->shi502_security_descriptor;
		//NetApiBufferFree(pBuffer);
		byNeedToRelease = pBuffer;
		return pSecDescriptorBuf;
	}
}

vector<ADACE*> ADDirectorySearch::preprocessNTFSEntries(vector<ADACE*> entries)
{
	vector<ADACE*> resultEntries;
	for (int i = 0; i < entries.size(); i ++)
	{
		if (entries[i]->refDomain.CompareNoCase(_T("BUILTIN")) == 0 ||
			entries[i]->refDomain.CompareNoCase(_T("NT AUTHORITY")) == 0 ||
			entries[i]->name.CompareNoCase(_T("CREATOR OWNER")) == 0)
		{
			delete entries[i];
		}
		else
		{
			resultEntries.push_back(entries[i]);
		}
	}
	return resultEntries;
}

vector<ADACE*> ADDirectorySearch::getShareFolderInfo(CString strFileCNName, CString strServerName,
													 CString strServerIP, CString strFlag, CString strFileDNName)
{
	vector<ADACE*> entries;
	ADACE* entry;
	CString strServerNameWithSlash = _T("\\\\") + strServerName;

	PSECURITY_DESCRIPTOR pSecDescriptorBuf;
	BYTE* byNeedToRelease;

	if (strFlag == _T("NTFS"))
	{
		strFlag = _T("NTFS3");
	}

	if (strFlag == _T("NTFS"))
	{
		pSecDescriptorBuf = getNTFSSecDescriptor(strFileCNName, strServerName, strServerIP);
	}
	else if (strFlag == _T("NTFS2"))
	{
		pSecDescriptorBuf = getNTFSSecDescriptor2(strFileCNName, strServerName, strServerIP);
	}
	else if (strFlag == _T("NTFS3"))
	{
		pSecDescriptorBuf = getNTFSSecDescriptor3(strFileCNName, strServerName, strServerIP);
	}
	else if (strFlag == _T("SHARE"))
	{
		pSecDescriptorBuf = getShareSecDescriptor(strFileCNName, strServerName, strServerIP, byNeedToRelease);
	}
	else if (strFlag == _T("SHARE2"))
	{
		pSecDescriptorBuf = getShareSecDescriptor2(strFileCNName, strServerName, strServerIP, byNeedToRelease);
	}
	else
	{
		MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
		return entries;
	}
	PACL pacl;
	BOOL bDaclPresent, bDaclDefaulted;
	DWORD bSuccess = FALSE;

	if(pSecDescriptorBuf == NULL)
	{
		return entries;
	}
	bSuccess = GetSecurityDescriptorDacl((SECURITY_DESCRIPTOR*)pSecDescriptorBuf, &bDaclPresent, &pacl, &bDaclDefaulted);
	if (bSuccess && bDaclPresent)
	{
		for (USHORT i = 0; i < pacl->AceCount; i++)
		{
			ACE_HEADER* pAce;
			bSuccess = GetAce(pacl, i, (LPVOID*) &pAce);
			if (bSuccess)
			{
				entry = new ADACE();
				//NO=xx,TP=xx,FileDN
				if (strFlag == _T("SHARE"))
				{
					entry->dn = _T("NO=") + itos(i) + _T(",TP=Share,") + strFileDNName;
				}
				else //strFlag == _T("NTFS3")
				{
					entry->dn = _T("NO=") + itos(i) + _T(",TP=NTFS,") + strFileDNName;
				}

				switch (pAce->AceType)
				{
				case ACCESS_ALLOWED_ACE_TYPE:
						entry->allowed = 1;
						break;
				case ACCESS_DENIED_ACE_TYPE:
						entry->allowed = 0;
						break;
				}

				SID* pAceSid;
				ACCESS_MASK maskPermissions;
				if (entry->allowed)
				{
					ACCESS_ALLOWED_ACE* pAllowed = (ACCESS_ALLOWED_ACE*) pAce;
					pAceSid = (SID*) (&(pAllowed->SidStart));
					maskPermissions = pAllowed->Mask;
					if ((maskPermissions & READ_PERMISSIONS) == READ_PERMISSIONS)
						entry->r = 1;
					else
						entry->r = 0;
					if ((maskPermissions & WRITE_PERMISSIONS) == WRITE_PERMISSIONS)
						entry->w = 1;
					else
						entry->w = 0;
					if ((maskPermissions & EXECUTE_PERMISSIONS) == EXECUTE_PERMISSIONS)
						entry->x = 1;
					else
						entry->x = 0;
				}
				else
				{
					ACCESS_DENIED_ACE* pDenied = (ACCESS_DENIED_ACE*) pAce;
					pAceSid = (SID*) (&(pDenied->SidStart));
					maskPermissions = pDenied->Mask;
					if ((maskPermissions & READ_PERMISSIONS) != 0)
						entry->r = 1;
					else
						entry->r = 0;
					if ((maskPermissions & WRITE_PERMISSIONS) != 0)
						entry->w = 1;
					else
						entry->w = 0;
					if ((maskPermissions & EXECUTE_PERMISSIONS) != 0)
						entry->x = 1;
					else
						entry->x = 0;
				}
				
				wchar_t bufName[MAX_PATH] = L"NULL";
				wchar_t bufDomain[MAX_PATH] = L"NULL";
				DWORD dwCbName = sizeof(bufName);
				DWORD dwCbDomainName = sizeof(bufDomain);
				SID_NAME_USE SidNameUse;
				// Get account name for SID
				BOOL bSuccess = LookupAccountSidW(_bstr_t(strServerNameWithSlash), //"\\\\Win2008R2"
					pAceSid,
					bufName,
					&dwCbName,
					bufDomain,
					&dwCbDomainName,
					&SidNameUse);
				//SECURITY_WORLD_SID_AUTHORITY

				if (!bSuccess)
				{
					CString strDomainNetBIOSNameWithSlash = _T("\\\\") + m_strDomainName;
					bSuccess = LookupAccountSidW(_bstr_t(strDomainNetBIOSNameWithSlash), //"\\\\TECH"
						pAceSid,
						bufName,
						&dwCbName,
						bufDomain,
						&dwCbDomainName,
						&SidNameUse);
				}
				if (bSuccess)
				{
					entry->name = bufName;
					entry->refDomain = bufDomain;
					entries.push_back(entry);
				}
				else
				{
					DWORD dwError = GetLastError();
					delete entry;
					if (dwError != 0)
					{
						MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
					}
				}

			}
			else
			{
				MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
				if (strFlag == _T("NTFS"))
				{
					delete[] pSecDescriptorBuf;
				}
				else if (strFlag == _T("NTFS2"))
				{
					LocalFree(pSecDescriptorBuf);
				}
				else if (strFlag == _T("NTFS3"))
				{
					ADPermissionSearch::releaseSD(pSecDescriptorBuf);
				}
				else if (strFlag == _T("SHARE"))
				{
					NetApiBufferFree(byNeedToRelease);
				}
				else if (strFlag == _T("SHARE2"))
				{
					NetApiBufferFree(byNeedToRelease);
				}
				else
				{
					MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
				}
				return entries;
				//DWORD dwError = GetLastError();
				//cout << "Failed to get ace " << i << " (" << dwError << ")\n";
				//continue;
			}

		}
		if (strFlag == _T("NTFS"))
		{
			delete[] pSecDescriptorBuf;
		}
		else if (strFlag == _T("NTFS2"))
		{
			LocalFree(pSecDescriptorBuf);
		}
		else if (strFlag == _T("NTFS3"))
		{
			ADPermissionSearch::releaseSD(pSecDescriptorBuf);
		}
		else if (strFlag == _T("SHARE"))
		{
			NetApiBufferFree(byNeedToRelease);
		}
		else if (strFlag == _T("SHARE2"))
		{
			NetApiBufferFree(byNeedToRelease);
		}
		else
		{
			MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
		}
		return entries;
	}
	else
	{
		MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
		if (strFlag == _T("NTFS"))
		{
			delete[] pSecDescriptorBuf;
		}
		else if (strFlag == _T("NTFS2"))
		{
			LocalFree(pSecDescriptorBuf);
		}
		else if (strFlag == _T("NTFS3"))
		{
			ADPermissionSearch::releaseSD(pSecDescriptorBuf);
		}
		else if (strFlag == _T("SHARE"))
		{
			NetApiBufferFree(byNeedToRelease);
		}
		else if (strFlag == _T("SHARE2"))
		{
			NetApiBufferFree(byNeedToRelease);
		}
		else
		{
			MyMessageBox_Error(_T("getShareFolderInfo Error."), _T("Error"));
		}
		return entries;
	}
}

vector<ADShareFolder*> ADDirectorySearch::listShareFolders(CString strServerName, CString strServerIP, ADDomain* domain)
{
	int iTryTime = 1;
	Output_ConsoleList(_T("Listing share folders for ") + strServerName + _T("..."));
	vector<ADShareFolder*> resultShareFolders;
	ADShareFolder* shareFolder;

	CString strServerNameWithSlash = _T("\\\\") + strServerName; //"\\\\veotax3";
	CString strServerIPWithSlash = _T("\\\\") + strServerIP; //"\\\\192.168.1.x";

	DWORD dwResult;
	NETRESOURCEW netResource;
	memset(&netResource, 0, sizeof(netResource));
	netResource.dwScope = RESOURCE_GLOBALNET;  
	netResource.dwType = RESOURCETYPE_DISK;
	netResource.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;  
	netResource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	netResource.lpRemoteName = _bstr_t(strServerNameWithSlash);//远程主机名或IP如://192.168.1.11

	DWORD dwResultEnum;
	HANDLE hEnum;
	BYTE buf[16384];
	DWORD cbBuffer = 16384;     // 16K is a good size
	LPNETRESOURCEW lpnrLocal = (LPNETRESOURCEW) buf;
	DWORD cEntries = -1;  
ZZ:	dwResult = WNetOpenEnumW(RESOURCE_GLOBALNET, // all network resources
		RESOURCETYPE_DISK,   //只枚举共享文件夹
		0,  // enumerate all resources
		&netResource,       // NULL first time the function is called
		&hEnum);    // handle to the resource
	if (dwResult != NO_ERROR) 
	{
		iTryTime --;
		if (iTryTime >= 0)
		{
			netResource.lpRemoteName = _bstr_t(strServerIPWithSlash);
			goto ZZ;
		}
		MyMessageBox_Error(_T("listShareFolders Error."), _T("Error"));
		return resultShareFolders;
	}
	do 
	{
		cbBuffer = 16384;
		memset(lpnrLocal, 0, 16384);
		dwResultEnum = WNetEnumResourceW(hEnum,  // resource handle
			&cEntries,      // defined locally as -1
			lpnrLocal,      // LPNETRESOURCE
			&cbBuffer);     // buffer size
		if (dwResultEnum == NO_ERROR) 
		{
			CString strDisplay;
			for (DWORD i = 0; i < cEntries; i ++) 
			{
				CString str_remote = lpnrLocal[i].lpRemoteName;//得到共享名称
				//NeiborNode* child = node->addChild(str_remote);

				//lpRemoteName = "\\\\Win2003-SP2-1\\temp"
				shareFolder = new ADShareFolder(str_remote, domain);
				resultShareFolders.push_back(shareFolder);
				strDisplay += str_remote + _T(", ");
			}
			if (cEntries == 0)
			{
				int aaa = 0;
			}
			strDisplay.Left(strDisplay.GetLength() - 2);
			Output_ConsoleList(_T("Shared folders in ") + strServerName + _T(" are: ") + strDisplay);
		}
		else
		{
			if (dwResultEnum != ERROR_NO_MORE_ITEMS) 
			{
				break;
			}
		}	
	} 
	while (dwResultEnum != ERROR_NO_MORE_ITEMS);
	WNetCloseEnum(hEnum);
	return resultShareFolders;
}

vector<CString> ADDirectorySearch::getGroupMembers(CString strDCAddress, CString strGroupDNName, CString strUserName,
												   CString strPassWord)
{
	vector<CString> resultMembers;
	CString strLDAPPath = _T("LDAP://") + strDCAddress + _T("/") + strGroupDNName;
	_bstr_t bstrLDAPPath = strLDAPPath;
	_bstr_t bstrUserName = strUserName;
	_bstr_t bstrPassWord = strPassWord;
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		//cout << "succeed 1" << endl;
		IADsGroup* pGroup;
		IADsMembers* pMembers;
		//hr = ADsOpenObject(L"LDAP://192.168.1.5/CN=Administrators,CN=Builtin,DC=domaintest,DC=net", L"Administrator", L"123456Ad",
		//	ADS_SECURE_AUTHENTICATION, IID_IADsGroup, (void**)&pGroup);
		hr = ADsOpenObject(bstrLDAPPath, bstrUserName, bstrPassWord,
			ADS_SECURE_AUTHENTICATION, IID_IADsGroup, (void**) &pGroup);
		if (SUCCEEDED(hr))
		{
			//cout << "succeed 2" << endl;
			hr = pGroup->Members(&pMembers);
			if (SUCCEEDED(hr))
			{
				//cout << "succeed 3" << endl;
				IEnumVARIANT* pEnum = NULL;
				hr = ADsBuildEnumerator((IADsContainer*) pMembers, &pEnum);
				if (SUCCEEDED(hr))
				{
					//cout << "succeed 4" << endl;
					ULONG ulCount;
					_variant_t varEntry;
					
					while (ADsEnumerateNext(pEnum, 1, &varEntry, &ulCount) == S_OK && ulCount == 1)
					{
						IADs* pEntry = NULL;
						
						hr = ((IDispatch *) varEntry)->QueryInterface(IID_IADs, (void **) &pEntry);
						if (SUCCEEDED(hr))
						{
							BSTR strName;
							pEntry->get_ADsPath(&strName);
							CString strMemberDNName = strName;
							int iPos = strMemberDNName.ReverseFind(_T('/'));
							resultMembers.push_back(strMemberDNName.Right(strMemberDNName.GetLength() - iPos - 1));
							//wprintf(L"%s\n", strName);
							//pEntry->get_Name(&strName);
							//wprintf(L"%s\n", strName);
							pEntry->Release();
						}
						else
						{
							MyMessageBox_Error(_T("getGroupMembers Error."), _T("Error"));
							return resultMembers;
						}
						((IDispatch *) varEntry)->Release();
					}
					pEnum->Release();
				}
				else
				{
					MyMessageBox_Error(_T("getGroupMembers Error."), _T("Error"));
					return resultMembers;
				}
				pMembers->Release();
			}
			else
			{
				MyMessageBox_Error(_T("getGroupMembers Error."), _T("Error"));
				return resultMembers;
			}
			pGroup->Release();
		}
		else
		{
			MyMessageBox_Error(_T("getGroupMembers Error."), _T("Error"));
			return resultMembers;
		}
		CoUninitialize();
	}

	return resultMembers;
}

CString ADDirectorySearch::dnNametoString(CString strIn)
{
	for (int i = 0; i < strIn.GetLength(); i ++)
	{
		if (strIn[i] == _T(',') || strIn[i] == _T('=') || strIn[i] == _T(' '))
			strIn.SetAt(i, _T('_'));
	}
	return strIn;
}

CString ADDirectorySearch::lineFeedToString(CString strIn)
{
	if (strIn.GetLength() <= 15)
		return strIn;

	int iMid = (strIn.GetLength() - 1) / 2;
	for (int i = 0; i <= (strIn.GetLength() - 1) / 2; i ++)
	{
		if (strIn.GetAt(iMid + i) == _T(' '))
		{
			strIn.SetAt(iMid + i, _T('\\'));
			strIn.Insert(iMid + i + 1, _T("n"));
			break;
		}
		else if (strIn.GetAt(iMid - i) == _T(' '))
		{
			strIn.SetAt(iMid - i, _T('\\'));
			strIn.Insert(iMid - i + 1, _T("n"));
			break;
		}
	}
	return strIn;
}

ADGroup* ADDirectorySearch::getGroupFromDomains(CString strDomainName, CString strCN, vector<ADDomain*> globalDomains)
{
	for (int i = 0; i < globalDomains.size(); i ++)
	{
		if (globalDomains[i]->netBIOSName == strDomainName)
			break;
	}
	if (i == globalDomains.size())
		return NULL;

	for (int j = 0; j < globalDomains[i]->g_groups.size(); j ++)
	{
		if (globalDomains[i]->g_groups[j]->cn == strCN)
			return globalDomains[i]->g_groups[j];
	}
	return NULL;
}

ADUser* ADDirectorySearch::getUserFromDomains(CString strDomainName, CString strCN, vector<ADDomain*> globalDomains)
{
	for (int i = 0; i < globalDomains.size(); i ++)
	{
		if (globalDomains[i]->netBIOSName == strDomainName)
			break;
	}
	if (i == globalDomains.size())
		return NULL;
	
	for (int j = 0; j < globalDomains[i]->g_users.size(); j ++)
	{
		if (globalDomains[i]->g_users[j]->cn == strCN)
			return globalDomains[i]->g_users[j];
	}
	return NULL;
}

void ADDirectorySearch::getKeyValueFromLine(CString strLine, CString &strIP, CString &strHostname)
{
	int i = 0;
	int iStart;
	int iEnd;

	while (strLine.GetAt(i) == _T('\t') || strLine.GetAt(i) == _T(' ') || strLine.GetAt(i) == _T('\n'))
	{
		i ++;
		if (i >= strLine.GetLength())
		{
			MyMessageBox_Error(_T("getKeyValueFromLine Error."), _T("Error"));
			return;
		}
	}
	iStart = i;
	while ((strLine.GetAt(i) <= _T('9') && strLine.GetAt(i) >= _T('0')) || strLine.GetAt(i) == _T('.'))
	{
		i ++;
		if (i >= strLine.GetLength())
		{
			MyMessageBox_Error(_T("getKeyValueFromLine Error."), _T("Error"));
			return;
		}
	}
	iEnd = i;

	strIP = strLine.Mid(iStart, iEnd - iStart);

	while (strLine.GetAt(i) == _T('\t') || strLine.GetAt(i) == _T(' ') || strLine.GetAt(i) == _T('\n'))
	{
		i ++;
		if (i >= strLine.GetLength())
		{
			MyMessageBox_Error(_T("getKeyValueFromLine Error."), _T("Error"));
			return;
		}
	}
	iStart = i;
	while (strLine.GetAt(i) != _T(' ') && strLine.GetAt(i) != _T('\t') && strLine.GetAt(i) != _T('\n'))
	{
		i ++;
		if (i >= strLine.GetLength())
		{
			break;
		}
	}

	iEnd = i;
	strHostname = strLine.Mid(iStart, iEnd - iStart);
}

BOOL ADDirectorySearch::evaluateLine(CString strLine)
{
	for (int i = 0; i < strLine.GetLength(); i ++)
	{
		if (strLine.GetAt(i) != _T(' ') && strLine.GetAt(i) != _T('\t') && strLine.GetAt(i) != _T('\n'))
			return TRUE;
	}
	return FALSE;
}

BOOL ADDirectorySearch::deleteFile(CString strFilePathName)
{
	SHFILEOPSTRUCT FileOp={0};
	FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
		FOF_NOCONFIRMATION; //不出现确认对话框
	FileOp.pFrom = strFilePathName;
	FileOp.pTo = NULL;      //一定要是NULL
	FileOp.wFunc = FO_DELETE;    //删除操作
	return SHFileOperation(&FileOp) == 0;
}

void ADDirectorySearch::addToHostFile(CString strIP, CString strHostname)
{
	if (strIP == _T("") || strHostname == _T(""))
		return;
	TCHAR path[MAX_PATH];
	GetSystemDirectory(path, MAX_PATH);
	CString strHostPathName = path;
	strHostPathName += _T("\\drivers\\etc\\HOSTS");
	CString strHostPathNameBak = path;
	strHostPathNameBak += _T("\\drivers\\etc\\HOSTS_BAK");

	//deleteFile(strHostPathNameBak);
	if (!CopyFileEx(strHostPathName, strHostPathNameBak, (LPPROGRESS_ROUTINE)NULL, NULL, FALSE, COPY_FILE_RESTARTABLE))
	{
		MyMessageBox_Error(_T("addToHostFile Error."), _T("Error"));
		return;
	}

	CStdioFile file;
	if (file.Open(strHostPathName, CFile::modeReadWrite) == FALSE)
	{
		MyMessageBox_Error(_T("addToHostFile Error."), _T("Error"));
		return;
	}

	vector<CString> nstrLines;

	int iNeedToAdd = 1;
	CString strLine;
	while (file.ReadString(strLine))
	{
		if (strLine.GetLength() == 0 || evaluateLine(strLine) == FALSE)
			continue;

		nstrLines.push_back(strLine);
		if (strLine.GetAt(0) != _T('#'))
		{
			CString strLineIP;
			CString strLineHostname;
			getKeyValueFromLine(strLine, strLineIP, strLineHostname);
			if (strIP == strLineIP && strHostname == strLineHostname)
			{
				iNeedToAdd = 0;
			}
			else if (strIP != strLineIP && strHostname == strLineHostname)
			{
				iNeedToAdd = 0;
				strLine.Replace(strLineIP, strIP);
				nstrLines.pop_back();
				nstrLines.push_back(strLine);
			}
		}
	}
	
	if (iNeedToAdd)
	{
		CString strEntry = (strIP + _T(" ") + strHostname);
		nstrLines.push_back(strEntry);
	}

	file.Close();
	file.Open(strHostPathName, CFile::modeCreate | CFile::modeWrite);
	CString strReturn = _T("\n");
	for (int i = 0; i < nstrLines.size(); i ++)
	{
		file.WriteString(nstrLines[i]);
		file.WriteString(strReturn);
	}
	file.Close();
}