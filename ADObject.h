#include <MAP>
using namespace std;

#ifndef _ADOBJECT
#define _ADOBJECT

class ADObject : public CObject
{
	DECLARE_SERIAL(ADObject)
public:
	map<CString, CString> infos;
	map<CString, CString> fatherInfos;
	map<CString, CString> childGroupInfos;
	map<CString, CString> childUserInfos;
	map<CString, CString> accessInfos;
	map<CString, CString> networkInfos;

public:
	ADObject();
	void Serialize(CArchive &ar);
	void addPairToInfos(CString strKey, CString strValue);
	void addPairToFatherInfos(CString strKey, CString strValue);
	void addPairToChildGroupInfos(CString strKey, CString strValue);
	void addPairToChildUserInfos(CString strKey, CString strValue);
	void addPairToAccessInfos(CString strKey, CString strValue);
	void addPairToNetworkInfos(CString strKey, CString strValue);
	virtual void onButttonDown();
	virtual void onButtonUp();
};

#endif