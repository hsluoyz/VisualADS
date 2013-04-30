#include "StdAfx.h"
#include "ADWMISearch.h"

#include <comdef.h>
# pragma comment(lib, "wbemuuid.lib")
# pragma comment(lib, "credui.lib")
# pragma comment(lib, "comsupp.lib")
#include <wincred.h>
#include <strsafe.h>
#include <iostream>
using namespace std;

DWORD ADWMISearch::WIN32_FROM_HRESULT_alternate(HRESULT hr)
{
	DWORD dwResult;
	if (hr < 0)
	{
		dwResult = (DWORD) hr;
		return dwResult;
	}
	else
	{
		MyMessageBox_Error(_T("WIN32_FROM_HRESULT_alternate Error."), _T("Error"));
		return -1;
	}
}

IWbemServices* ADWMISearch::connect(CString strServerName, CString strServerIP, CString strDomainName, CString strUsername,
						 CString strPassword)
{
	CString strComputerName = strServerName;
	CString strFullUsername; //_T("adtest\\Administrator")
	if (strDomainName == _T(""))
	{
		strUsername = _T("");
		strFullUsername = _T("");
		strPassword = _T("");
	}
	else
	{
		strFullUsername = strDomainName + _T("\\") + strUsername;
	}
	

	HRESULT hres;

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
    if (FAILED(hres))
    {
        //cout << "Failed to initialize COM library. Error code = 0x" 
        //    << hex << hres << endl;
		MyMessageBox_Error(_T("connect"));
        return NULL;                  // Program has failed.
    }

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------

    hres =  CoInitializeSecurity(
        NULL, 
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IDENTIFY,    // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );

	//char aaa[100];
	//sprintf(aaa, "%x", hres);

	if (FAILED(hres) && hres != RPC_E_TOO_LATE)
		{
			//cout << "Failed to initialize security. Error code = 0x" 
			//    << hex << hres << endl;
			CoUninitialize();
			MyMessageBox_Error(_T("connect"));
			return NULL;                    // Program has failed.
		}
    
    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &pLoc);
 
    if (FAILED(hres))
    {
        //cout << "Failed to create IWbemLocator object."
        //    << " Err code = 0x"
        //    << hex << hres << endl;
        CoUninitialize();
		MyMessageBox_Error(_T("connect"));
        return NULL;                 // Program has failed.
    }

    // Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    IWbemServices *pSvc = NULL;

	bool useToken = TRUE;
    bool useNTLM = FALSE;
    // Get the user name and password for the remote computer
	/*
    CREDUI_INFO cui;
    bool useToken = TRUE;
    bool useNTLM = FALSE;
    TCHAR pszName[CREDUI_MAX_USERNAME_LENGTH+1] = {0};
    TCHAR pszPwd[CREDUI_MAX_PASSWORD_LENGTH+1] = {0};
    TCHAR pszDomain[CREDUI_MAX_USERNAME_LENGTH+1];
    TCHAR pszUserName[CREDUI_MAX_USERNAME_LENGTH+1];
    TCHAR pszAuthority[CREDUI_MAX_USERNAME_LENGTH+1];
    BOOL fSave;
    DWORD dwErr;

    memset(&cui,0,sizeof(CREDUI_INFO));
    cui.cbSize = sizeof(CREDUI_INFO);
    cui.hwndParent = NULL;
    // Ensure that MessageText and CaptionText identify
    // what credentials to use and which application requires them.
    cui.pszMessageText = _T("Press cancel to use process token");
    cui.pszCaptionText = _T("Enter Account Information");
    cui.hbmBanner = NULL;
    fSave = FALSE;

    dwErr = CredUIPromptForCredentials( 
        &cui,                             // CREDUI_INFO structure
        _T(""),                         // Target for credentials
        NULL,                             // Reserved
        0,                                // Reason
        pszName,                          // User name
        CREDUI_MAX_USERNAME_LENGTH+1,     // Max number for user name
        pszPwd,                           // Password
        CREDUI_MAX_PASSWORD_LENGTH+1,     // Max number for password
        &fSave,                           // State of save check box
        CREDUI_FLAGS_GENERIC_CREDENTIALS |// flags
        CREDUI_FLAGS_ALWAYS_SHOW_UI |
        CREDUI_FLAGS_DO_NOT_PERSIST);  

    if(dwErr == ERROR_CANCELLED)
    {
        useToken = true;
    }
    else if (dwErr)
    {
        cout << "Did not get credentials " << dwErr << endl;
        pLoc->Release();     
        CoUninitialize();
        return 1;      
    }
	*/

    // change the computerName strings below to the full computer name
    // of the remote computer
	CString strAuthority;
    if(!useNTLM)
    {
        //StringCchPrintf(pszAuthority, CREDUI_MAX_USERNAME_LENGTH+1, _T("kERBEROS:%s"), strComputerName);
		strAuthority = _T("kERBEROS:") + strComputerName;

    }

    // Connect to the remote root\cimv2 namespace
    // and obtain pointer pSvc to make IWbemServices calls.
    //---------------------------------------------------------
	CString strNetworkResource = _T("\\\\") + strComputerName + _T("\\root\\cimv2");
	if (strFullUsername == _T(""))
	{
		hres = pLoc->ConnectServer(
			_bstr_t(strNetworkResource),
			NULL,									// User name
			NULL,									// User password
			NULL,									// Locale             
			WBEM_FLAG_CONNECT_USE_MAX_WAIT,			// Security flags
			_bstr_t(strAuthority),					// Authority        
			NULL,									// Context object 
			&pSvc									// IWbemServices proxy
			);
	}
	else
	{
		hres = pLoc->ConnectServer(
			_bstr_t(strNetworkResource),
			_bstr_t(strFullUsername),					// User name
			_bstr_t(strPassword),					// User password
			NULL,									// Locale             
			WBEM_FLAG_CONNECT_USE_MAX_WAIT,			// Security flags
			_bstr_t(strAuthority),					// Authority        
			NULL,									// Context object 
			&pSvc									// IWbemServices proxy
			);
	}
    
    if (FAILED(hres))
    {
		DWORD aaa = WIN32_FROM_HRESULT_alternate(hres);
        //cout << "Could not connect. Error code = 0x" 
        //     << hex << hres << endl;

        pLoc->Release();
        CoUninitialize();
		MyMessageBox_Error(_T("connect"));
        return NULL;                // Program has failed.
    }

    //cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


    // step 5: --------------------------------------------------
    // Create COAUTHIDENTITY that can be used for setting security on proxy

    COAUTHIDENTITY *userAcct = NULL;
    COAUTHIDENTITY authIdent;

    if(!useToken)
    {
        memset(&authIdent, 0, sizeof(COAUTHIDENTITY));
        authIdent.PasswordLength = strPassword.GetLength();
        authIdent.Password = (USHORT*) _bstr_t(strPassword);

		/*
        LPTSTR slash = _tcschr(pszName, _T('\\'));
        if( slash == NULL )
        {
            //cout << _T("Could not create Auth identity. No domain specified\n") ;
            pSvc->Release();
            pLoc->Release();     
            CoUninitialize();
            return 1;               // Program has failed.
        }

        StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH+1, slash+1);
        authIdent.User = (USHORT*)pszUserName;
        authIdent.UserLength = _tcslen(pszUserName);
        StringCchCopyN(pszDomain, CREDUI_MAX_USERNAME_LENGTH+1, pszName, slash - pszName);
		*/

        authIdent.Domain = (USHORT*) _bstr_t(strDomainName);
        authIdent.DomainLength = strDomainName.GetLength();
        authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

        userAcct = &authIdent;

    }

    // Step 6: --------------------------------------------------
    // Set security levels on a WMI connection ------------------

    hres = CoSetProxyBlanket(
       pSvc,                           // Indicates the proxy to set
       RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
       COLE_DEFAULT_PRINCIPAL,         // Server principal name 
       RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
       RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
       userAcct,                       // client identity
       EOAC_NONE                       // proxy capabilities 
    );

    if (FAILED(hres))
    {
        //cout << "Could not set proxy blanket. Error code = 0x" 
        //    << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();     
        CoUninitialize();
		MyMessageBox_Error(_T("connect"));
        return NULL;               // Program has failed.
    }

    // Cleanup
    // ========
	pLoc->Release();

	return pSvc;
}

vector<IWbemClassObject*> ADWMISearch::query(IWbemServices *pSvc, CString strDomainName, CString strPassword, CString strWQL)
{
	vector<IWbemClassObject*> npResultObjects;
	BOOL useToken = TRUE;
	// step 5: --------------------------------------------------
    // Create COAUTHIDENTITY that can be used for setting security on proxy

    COAUTHIDENTITY *userAcct = NULL;
    COAUTHIDENTITY authIdent;

    if(!useToken)
    {
        memset(&authIdent, 0, sizeof(COAUTHIDENTITY));
        authIdent.PasswordLength = strPassword.GetLength();
        authIdent.Password = (USHORT*) _bstr_t(strPassword);

		/*
        LPTSTR slash = _tcschr(pszName, _T('\\'));
        if( slash == NULL )
        {
            //cout << _T("Could not create Auth identity. No domain specified\n") ;
            pSvc->Release();
            pLoc->Release();     
            CoUninitialize();
            return 1;               // Program has failed.
        }

        StringCchCopy(pszUserName, CREDUI_MAX_USERNAME_LENGTH+1, slash+1);
        authIdent.User = (USHORT*)pszUserName;
        authIdent.UserLength = _tcslen(pszUserName);
        StringCchCopyN(pszDomain, CREDUI_MAX_USERNAME_LENGTH+1, pszName, slash - pszName);
		*/

        authIdent.Domain = (USHORT*) _bstr_t(strDomainName);
        authIdent.DomainLength = strDomainName.GetLength();
        authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

        userAcct = &authIdent;

    }

	// Step 7: --------------------------------------------------
    // Use the IWbemServices pointer to make requests of WMI ----

    // For example, get the name of the operating system
    IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hResult;
    hResult = pSvc->ExecQuery(
        _bstr_t(_T("WQL")), 
        _bstr_t(strWQL), //_T("Select * from Win32_Share")
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnumerator);
    
    if (FAILED(hResult))
    {
        //cout << "Query for operating system name failed."
        //    << " Error code = 0x" 
        //    << hex << hres << endl;
		MyMessageBox_Error(_T("query"));
        return npResultObjects;               // Program has failed.
    }

    // Step 8: -------------------------------------------------
    // Secure the enumerator proxy
    hResult = CoSetProxyBlanket(
        pEnumerator,                    // Indicates the proxy to set
        RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
        COLE_DEFAULT_PRINCIPAL,         // Server principal name 
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
        RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
        userAcct,                       // client identity
        EOAC_NONE                       // proxy capabilities 
        );

    if (FAILED(hResult))
    {
        //cout << "Could not set proxy blanket on enumerator. Error code = 0x" 
        //     << hex << hres << endl;
        pEnumerator->Release();
		MyMessageBox_Error(_T("query"));
        return npResultObjects;               // Program has failed.
    }

    // When you have finished using the credentials,
    // erase them from memory.

    //SecureZeroMemory(pszName, sizeof(pszName));
    //SecureZeroMemory(pszPwd, sizeof(pszPwd));
    //SecureZeroMemory(pszUserName, sizeof(pszUserName));
    //SecureZeroMemory(pszDomain, sizeof(pszDomain));


    // Step 9: -------------------------------------------------
    // Get the data from the query in step 7 -------------------
 
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;
   
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }
		
		npResultObjects.push_back(pclsObj);
		/*
        VARIANT vtProp;

        // Get the value of the Name property
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        cout << _T("OS Name: ") << vtProp.bstrVal << endl;

        // Get the value of the FreePhysicalMemory property
        hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
        cout << "Free physical memory(in kilobytes): " << vtProp.uintVal << endl;
        VariantClear(&vtProp);

        pclsObj->Release();
        pclsObj = NULL;
		*/
    }

    // Cleanup
    // ========
    
    pEnumerator->Release();

	return npResultObjects;
}

CString ADWMISearch::getStringFromObject(IWbemClassObject* pObject, CString strPropertyName)
{
	_bstr_t bstrPropertyName = strPropertyName;
	
	VARIANT vtProperty;
	CString strProperty;
	// Get the value of the property
	VariantInit(&vtProperty);
	HRESULT hResult = pObject->Get(bstrPropertyName, 0, &vtProperty, 0, 0);
	if (SUCCEEDED(hResult) && V_VT(&vtProperty) == VT_BSTR)
	{
		strProperty = vtProperty.bstrVal;
		VariantClear(&vtProperty);
		return strProperty;
	}
	else
	{
		VariantClear(&vtProperty);
		MyMessageBox_Error(_T("getStringsFromObjects"));
		return _T("");
	}
}

vector<CString> ADWMISearch::getStringsFromObjects(vector<IWbemClassObject*> npObjects, CString strPropertyName)
{
	_bstr_t bstrPropertyName = strPropertyName;
	vector<CString> nstrProperties;

	for (int i = 0; i < npObjects.size(); i ++)
	{
		VARIANT vtProperty;
		CString strProperty;
		// Get the value of the property
		VariantInit(&vtProperty);
		HRESULT hResult = npObjects[i]->Get(bstrPropertyName, 0, &vtProperty, 0, 0);
		if (SUCCEEDED(hResult) && V_VT(&vtProperty) == VT_BSTR)
		{
			CString strProperty = vtProperty.bstrVal;
			nstrProperties.push_back(strProperty);
			VariantClear(&vtProperty);
		}
		else
		{
			VariantClear(&vtProperty);
			MyMessageBox_Error(_T("getStringsFromObjects"));
			return nstrProperties;
		}
	}
	return nstrProperties;
}

void ADWMISearch::clearObjects(vector<IWbemClassObject*> npObjects)
{
	for (int i = 0; i < npObjects.size(); i ++)
	{
		npObjects[i]->Release();
		npObjects[i] = NULL;
	}
	npObjects.clear();
}

IWbemClassObject* ADWMISearch::getWMIClass(IWbemServices *pSvc, CString strClassName)
{
	//1)取得Provider实例对象
    //调用IWbemServices::GetObject方法可以取得我们想要调用的Provider类型实例，他以一个IWbemClassObject类型指针返回。
	HRESULT hResult;
    IWbemClassObject* pClass = NULL;
    hResult = pSvc->GetObject(_bstr_t(strClassName), 0, NULL, &pClass, NULL); //_T("Win32_Share")
	if (!SUCCEEDED(hResult))
	{
		MyMessageBox_Error(_T("getWMIClass"));
		return NULL;
	}
	else
	{
		return pClass;
	}
}

IWbemClassObject* ADWMISearch::getObjectFromObject(IWbemClassObject *pObject, CString strArgumentName)
{
	if (!pObject)
	{
		MyMessageBox_Error(_T("getObjectFromObject"));
		return NULL;
	}
	
	HRESULT hResult;
	VARIANT varArgument;
	
	hResult = pObject->Get(_bstr_t(strArgumentName), 0, &varArgument, NULL, 0);
	if (!SUCCEEDED(hResult))
	{
		VariantClear(&varArgument);
		MyMessageBox_Error(_T("getObjectFromObject"));
		return NULL;
	}
	
	//IWbemClassObject **ppResultObject;
	IWbemClassObject *pResultObject2;
	if (V_VT(&varArgument) == VT_UNKNOWN)
	{
		//ppResultObject = (IWbemClassObject **)varArgument.ppunkVal;
		pResultObject2 = (IWbemClassObject *)varArgument.punkVal;
	}
	else if (V_VT(&varArgument) == VT_DISPATCH)
	{
		//ppResultObject = (IWbemClassObject **)varArgument.ppdispVal;
		pResultObject2 = (IWbemClassObject *)varArgument.pdispVal;
	}
	else
	{
		VariantClear(&varArgument);
		MyMessageBox_Error(_T("getObjectFromObject"));
		return NULL;
	}
	
	VariantClear(&varArgument);
	
	//IWbemClassObject *pResultObject = *ppResultObject;
	//return pResultObject;
	return pResultObject2;
}

IWbemClassObject* ADWMISearch::getObjectFromObjectWithCheck(IWbemClassObject *pOutParams, CString strArgumentName)
{
	if (!pOutParams)
	{
		MyMessageBox_Error(_T("getObjectFromObjectWithCheck"));
		return NULL;
	}

	HRESULT hResult;
	VARIANT varReturnValue;
	VARIANT varArgument;
	hResult = pOutParams->Get(_bstr_t(L"ReturnValue"), 0, &varReturnValue, NULL, 0);
	if (!SUCCEEDED(hResult))
	{
		VariantClear(&varReturnValue);
		VariantClear(&varArgument);
		MyMessageBox_Error(_T("getObjectFromObjectWithCheck"));
		return NULL;
	}
	DWORD dwResult = varReturnValue.lVal;
	//if (dwResult == 0)
	//{
	//	MyMessageBox_Error(_T("getObjectFromObjectWithCheck"));
	//	return _T("");
	//}

	hResult = pOutParams->Get(_bstr_t(strArgumentName), 0, &varArgument, NULL, 0);
	if (!SUCCEEDED(hResult))
	{
		VariantClear(&varReturnValue);
		VariantClear(&varArgument);
		MyMessageBox_Error(_T("getObjectFromObjectWithCheck"));
		return NULL;
	}

	//IWbemClassObject **ppResultObject;
	IWbemClassObject *pResultObject2;
	if (V_VT(&varArgument) == VT_UNKNOWN)
	{
		//ppResultObject = (IWbemClassObject **)varArgument.ppunkVal;
		pResultObject2 = (IWbemClassObject *)varArgument.punkVal;
	}
	else if (V_VT(&varArgument) == VT_DISPATCH)
	{
		//ppResultObject = (IWbemClassObject **)varArgument.ppdispVal;
		pResultObject2 = (IWbemClassObject *)varArgument.pdispVal;
	}
	else
	{
		VariantClear(&varReturnValue);
		VariantClear(&varArgument);
		MyMessageBox_Error(_T("getObjectFromObjectWithCheck"));
		return NULL;
	}

	VariantClear(&varReturnValue);
	VariantClear(&varArgument);

	//IWbemClassObject *pResultObject = *ppResultObject;
	//return pResultObject;
	return pResultObject2;
}

vector<ADWMIParam> ADWMISearch::genObjectArguments(CString strArgName, IWbemClassObject *pObject)
{
	vector<ADWMIParam> resultArgs;
	resultArgs.push_back(ADWMIParam(strArgName, pObject));
	return resultArgs;
}

IWbemClassObject* ADWMISearch::callStaticMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
												CString strMethodName)
{
	return callStaticMethod(pSvc, pClass, strClassName, strMethodName, vector<ADWMIParam>());
}

IWbemClassObject* ADWMISearch::callStaticMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
												CString strMethodName, vector<ADWMIParam> &arguments)
{
	_bstr_t bstrClassName = _bstr_t(strClassName);
	_bstr_t bstrMethodName = _bstr_t(strMethodName);
	BOOL bInParams;
	BOOL bOutParams;

	HRESULT hResult;
    IWbemClassObject* pClass2 = getWMIClass(pSvc, strClassName);

	//2)取得Provider提供方法参数
	//调用IWbemClassObject::GetMethod方法可以取得我们想要调用方法的参数，他以一个IWbemClassObject类型指针返回
	IWbemClassObject* pInParamsDefinition = NULL;
	IWbemClassObject* pOutParamsDefinition = NULL;
    hResult = pClass2->GetMethod(bstrMethodName, 0, &pInParamsDefinition, &pOutParamsDefinition);
	if (hResult != WBEM_S_NO_ERROR)
	{
		//pClass->Release();
		MyMessageBox_Error(_T("callStaticMethod"));
		return NULL;
	}
	if (pInParamsDefinition == NULL)
	{
		bInParams = FALSE;
	}
	else
	{
		bInParams = TRUE;
	}
	if (pOutParamsDefinition == NULL)
	{
		bOutParams = FALSE;
	}
	else
	{
		bOutParams = TRUE;
	}

	IWbemClassObject* pInParamsInstance = NULL;
	if (bInParams)
	{
		//3)生成Provider提供方法的参数的对象
		//调用IWbemClassObject::SpawnInstance方法生成调用方法的参数实例。需要将第二步得到的参数类型指针调用这个SpawnInstance方法，并且传递给该方法一个IWbemClassObject指针作为生成的参数对象的指针。
		
		hResult = pInParamsDefinition->SpawnInstance(0, &pInParamsInstance);
		//4)设置参数对象的属性
		//调用IWbemClassObject::Put方法就可以设置参数对象的类型。
		//_variant_t varCommand;
		//varCommand.vt = VT_BSTR;
		//varCommand.bstrVal = L"notepad.exe";
		//hResult = pInParamsInstance->Put(L"CommandLine", 0, &varCommand, 0);
		//_tprintf(_T("The command is: %s "), V_BSTR(&varCommand));

		for (int i = 0; i < arguments.size(); i ++)
		{
			_variant_t varArg;
			VariantCopy(&varArg, &(arguments[i].value));
			hResult = pInParamsInstance->Put(_bstr_t(arguments[i].key), 0, &varArg, 0);
			//hResult = pInParamsInstance->Put(_bstr_t(arguments[i].key), 0, &(arguments[i].value), 0);
		}
	}

	//5)调用方法
    //与查询信息相同，调用方法也可以分为同步方式和异步方式，同步方式等待执行进程结束，才继续往下执行；异步方式则利用实现IWbemObjectSink接口的类型，创建一个新的线程后继续运行当前线程，而由新创建的线程完成调用方法，然后回调IWbemObjectSink:: Indicate函数，处理函数返回值。
    //以下是以同步方式调用IWbemServices::ExecMethod方法即调用Provider提供的方法，将之前生成的关于参数的实例传给该方法即可。
	IWbemClassObject* pOutParams = NULL;
    hResult = pSvc->ExecMethod(bstrClassName, bstrMethodName, 0, NULL, pInParamsInstance, &pOutParams, NULL);
	if (hResult != WBEM_S_NO_ERROR)
	{
		//pClass->Release();
		if (pInParamsDefinition != NULL)
		{
			pInParamsDefinition->Release();
		}
		if (pOutParamsDefinition != NULL)
		{
			pOutParamsDefinition->Release();
		}
		if (pInParamsInstance != NULL)
		{
			pInParamsInstance->Release();
		}
		MyMessageBox_Error(_T("callStaticMethod"));
		return NULL;
	}

	if (pInParamsDefinition != NULL)
	{
		pInParamsDefinition->Release();
	}
	if (pOutParamsDefinition != NULL)
	{
		pOutParamsDefinition->Release();
	}
	if (pInParamsInstance != NULL)
	{
		pInParamsInstance->Release();
	}

	if (bOutParams)
	{
		return pOutParams;
	}
	else
	{
		return NULL;
	}
}

IWbemClassObject* ADWMISearch::callMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
										  CString strInstancePath, CString strMethodName)
{
	return callMethod(pSvc, pClass, strClassName, strInstancePath, strMethodName, vector<ADWMIParam>());
}

IWbemClassObject* ADWMISearch::callMethod(IWbemServices *pSvc, IWbemClassObject *pClass, CString strClassName,
										  CString strInstancePath, CString strMethodName,  vector<ADWMIParam> &arguments)
{
	_bstr_t bstrClassName = _bstr_t(strClassName);
	_bstr_t bstrMethodName = _bstr_t(strMethodName);
	BOOL bInParams;
	BOOL bOutParams;

	HRESULT hResult;
    //IWbemClassObject* pClass = getWMIClass(pSvc, strClassName);
	
	//2)取得Provider提供方法参数
	//调用IWbemClassObject::GetMethod方法可以取得我们想要调用方法的参数，他以一个IWbemClassObject类型指针返回
	IWbemClassObject* pInParamsDefinition = NULL;
	IWbemClassObject* pOutParamsDefinition = NULL;
    hResult = pClass->GetMethod(bstrMethodName, 0, &pInParamsDefinition, &pOutParamsDefinition);
	if (hResult != WBEM_S_NO_ERROR)
	{
		//pClass->Release();
		MyMessageBox_Error(_T("callMethod"));
		return NULL;
	}
	if (pInParamsDefinition == NULL)
	{
		bInParams = FALSE;
	}
	else
	{
		bInParams = TRUE;
	}
	if (pOutParamsDefinition == NULL)
	{
		bOutParams = FALSE;
	}
	else
	{
		bOutParams = TRUE;
	}

	IWbemClassObject* pInParamsInstance = NULL;
	if (bInParams)
	{
		//3)生成Provider提供方法的参数的对象
		//调用IWbemClassObject::SpawnInstance方法生成调用方法的参数实例。需要将第二步得到的参数类型指针调用这个SpawnInstance方法，并且传递给该方法一个IWbemClassObject指针作为生成的参数对象的指针。
		
		hResult = pInParamsDefinition->SpawnInstance(0, &pInParamsInstance);
		//4)设置参数对象的属性
		//调用IWbemClassObject::Put方法就可以设置参数对象的类型。
		//VARIANT varCommand;
		//varCommand.vt = VT_BSTR;
		//varCommand.bstrVal = L"notepad.exe";
		//hResult = pInParamsInstance->Put(L"CommandLine", 0, &varCommand, 0);
		//_tprintf(_T("The command is: %s "), V_BSTR(&varCommand));

		for (int i = 0; i < arguments.size(); i ++)
		{
			_variant_t varArg;
			VariantCopy(&varArg, &(arguments[i].value));
			hResult = pInParamsInstance->Put(_bstr_t(arguments[i].key), 0, &varArg, 0);
			//hResult = pInParamsInstance->Put(_bstr_t(arguments[i].key), 0, &(arguments[i].value), 0);
		}
	}

	//5)调用方法
    //与查询信息相同，调用方法也可以分为同步方式和异步方式，同步方式等待执行进程结束，才继续往下执行；异步方式则利用实现IWbemObjectSink接口的类型，创建一个新的线程后继续运行当前线程，而由新创建的线程完成调用方法，然后回调IWbemObjectSink:: Indicate函数，处理函数返回值。
    //以下是以同步方式调用IWbemServices::ExecMethod方法即调用Provider提供的方法，将之前生成的关于参数的实例传给该方法即可。
	IWbemClassObject* pOutParams = NULL;
    hResult = pSvc->ExecMethod(_bstr_t(strInstancePath), bstrMethodName, 0, NULL, pInParamsInstance, &pOutParams, NULL);
	if (hResult != WBEM_S_NO_ERROR)
	{
		//pClass->Release();
		if (pInParamsDefinition != NULL)
		{
			pInParamsDefinition->Release();
		}
		if (pOutParamsDefinition != NULL)
		{
			pOutParamsDefinition->Release();
		}
		if (pInParamsInstance != NULL)
		{
			pInParamsInstance->Release();
		}
		MyMessageBox_Error(_T("callMethod"));
		return NULL;
	}

	if (pInParamsDefinition != NULL)
	{
		pInParamsDefinition->Release();
	}
	if (pOutParamsDefinition != NULL)
	{
		pOutParamsDefinition->Release();
	}
	if (pInParamsInstance != NULL)
	{
		pInParamsInstance->Release();
	}

	if (bOutParams)
	{
		return pOutParams;
	}
	else
	{
		return NULL;
	}
}

void ADWMISearch::disconnect(IWbemServices *pSvc)
{
	pSvc->Release();
	CoUninitialize();
}