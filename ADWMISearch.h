#include <Wbemidl.h>

#include <vector>
#include <map>
using namespace std;

class ADWMIParam
{
public:
	CString key;
	_variant_t value;
public:
	ADWMIParam(CString strKey, CString strValue)
	{
		key = strKey;
		VariantInit(&value);
		value.vt = VT_BSTR;
		value.bstrVal = _bstr_t(strValue);
	}
	ADWMIParam(CString strKey, IWbemClassObject *pValue)
	{
		key = strKey;
		VariantInit(&value);
		value.vt = VT_UNKNOWN;
		value.punkVal = pValue;
	}
};

class ADWMISearch
{
public:
	DWORD WIN32_FROM_HRESULT_alternate(HRESULT hr);
	IWbemServices* connect(CString strServerName, CString strServerIP, CString strDomainName,
		CString strUsername, CString strPassword);
	vector<IWbemClassObject*> query(IWbemServices *pSvc, CString strDomainName, CString strPassword, CString strWQL);
	CString getStringFromObject(IWbemClassObject* pObject, CString strPropertyName);
	vector<CString> getStringsFromObjects(vector<IWbemClassObject*> npObjects, CString strPropertyName);
	void clearObjects(vector<IWbemClassObject*> npObjects);
	IWbemClassObject* getWMIClass(IWbemServices *pSvc, CString strClassName);
	IWbemClassObject* getObjectFromObject(IWbemClassObject *pObject, CString strArgumentName);
	IWbemClassObject* getObjectFromObjectWithCheck(IWbemClassObject *pOutParams, CString strArgumentName);
	vector<ADWMIParam> genObjectArguments(CString strArgName, IWbemClassObject *pObject);
	IWbemClassObject* callStaticMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
		CString strMethodName);
	IWbemClassObject* callStaticMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
		CString strMethodName, vector<ADWMIParam> &arguments);
	IWbemClassObject* callMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
		CString strInstancePath, CString strMethodName);
	IWbemClassObject* callMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
		CString strInstancePath, CString strMethodName, vector<ADWMIParam> &arguments);
	void disconnect(IWbemServices *pSvc);
};