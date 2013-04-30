#include "StdAfx.h"
#include "ADCNDPGenerator.h"

#include "ADDomain.h"
#include "ADShareFolder.h"
#include "ADComputer.h"
#include "ADDirectorySearch.h"
#include "ADGroup.h"
#include "ADUser.h"

ADCNDPGenerator::ADCNDPGenerator(vector<ADDomain*> globalDomains, CString strCNDPPathName)
{
	m_globalDomains = globalDomains;
	m_strCNDPPathName = strCNDPPathName;
}

void ADCNDPGenerator::execute()
{
	//MyMessageBox_Error(_T("haha"));

	CStdioFile pFile;
	pFile.Open(m_strCNDPPathName, CFile::modeCreate | CFile::modeWrite);
	
    m_strCNDPString = "";
    //目前仅一个域
    for (int i = 0; i < m_globalDomains.size(); i ++)
	{
		m_strCNDPString += (m_globalDomains[i]->dNSName +"{" + "\n" + domain2FileString(m_globalDomains[i]) +"}"+"\n");	   
	}

    //m_strCNDPString += m_globalDomains[0]->g_shareFolders[0]->cn;

	if (m_strCNDPString == _T(""))
	{
		MyMessageBox_Error(_T("请先读取域信息！"), _T("错误提示信息"));
		pFile.Close();
		return;
	}
	
	pFile.WriteString(m_strCNDPString);
	pFile.Close();
	
	CString strCmd;
	strCmd = _T("notepad ") + m_strCNDPPathName;
	WinExec(strCmd, SW_SHOW);
}

CString ADCNDPGenerator::domain2FileString(ADDomain* domain)
{
	CString strResult;
	strResult += _T("Authorization:\n");
    //授权策略
    for(int i = 0; i < domain->g_computers.size(); i++)
	{   
		ADComputer* computer = domain->g_computers[i];
        CString computerName = computer ->cn; 
	    for(int j = 0; j < computer-> shareFolders.size(); j++ )
		{
			ADShareFolder* shareFolder =  computer-> shareFolders[j];
			//文件夹名称
			CString shareFolderName =  shareFolder->cn;
			//每一个共享文件夹上的安全权限       
			for(int k = 0; k < shareFolder->ntfsPermissions.size();	 k++)
			{
				ADACE* ntfsPermission =  shareFolder->ntfsPermissions[k]; 
			
				CString strObjectName = ntfsPermission ->name;
				ADDomain *pDomain = NULL;
				if (ntfsPermission->user != NULL)
				{
					ADUser *pUser = ntfsPermission->user;
					pDomain = pUser->domain;
				}
				else
				{
					ADGroup *pGroup = ntfsPermission->group;
					pDomain = pGroup->domain;
				}
				CString strDomainName = pDomain->dNSName;
			
				CString policyMeature = "";
				if (ntfsPermission -> allowed)
				{
					policyMeature = _T("permit");
				}
				else
				{
					policyMeature = _T("deny");
				}
				
				CString policyActivity = "";
				if (ntfsPermission -> r)
				{
					policyActivity += _T("R");
				}
				if (ntfsPermission -> w)
				{
					policyActivity += _T("W");
				}
				if (ntfsPermission -> x)
				{
					policyActivity += _T("X");
				}
                
                strResult += strDomainName + _T("\\") + strObjectName + _T(":") + computerName + _T("\\")+ shareFolderName +  _T(":") + policyActivity + _T(":") + policyMeature + _T("\n"); 
			}

			for(int n = 0; n < shareFolder->sharePermissions.size(); n++)
			{
				ADACE* sharePermission =  shareFolder->sharePermissions[n]; 
				
				CString strOjectName = sharePermission ->name;
				ADDomain *pDomain = NULL;
				if (sharePermission->user != NULL)
				{
					ADUser *pUser = sharePermission->user;
					pDomain = pUser->domain;
				}
				else
				{
					ADGroup *pGroup = sharePermission->group;
					pDomain = pGroup->domain;
				}
				CString strDomainName = pDomain->dNSName;
				
				CString policyMeature = "";
				if (sharePermission -> allowed)
				{
					policyMeature = _T("permit");
				}
				else
				{
					policyMeature = _T("deny");
				}
				
				CString policyActivity = "";
				if (sharePermission -> r)
				{
					policyActivity += _T("R");
				}
				if (sharePermission -> w)
				{
					policyActivity += _T("W");
				}
				if (sharePermission -> x)
				{
					policyActivity += _T("X");
				}
                
                strResult += strDomainName + _T("\\") + strOjectName + _T(":") + computerName + _T("\\")+ shareFolderName +  _T(":") + policyActivity + _T(":") + policyMeature + _T("\n"); 
			}
		}
	}
    //继承策略
    strResult += _T("Inheritance:\n");
	for(int n = 0; n < domain ->g_groups.size(); n++)
	{
        ADGroup* domainGroup = domain ->g_groups[n];
		CString objectName = domainGroup ->cn;
		
        if (domainGroup ->childGroups.size() != NULL || domainGroup ->childUsers.size()!=NULL)
        {  
			strResult += _T("<") +objectName + _T(">") + _T("ChildGroup:");
		if (!(domainGroup ->childGroups.size()))
        {
			strResult = strResult + _T("<") + _T("?") + _T(">");
        } 
        else
        {
			for(int j = 0; j < domainGroup ->childGroups.size(); j++)
			{
				ADGroup* childGroup = domainGroup ->childGroups[j];
				strResult = strResult + _T("<") + childGroup ->cn + _T(">");	
			}
		}
		
        strResult = strResult + _T("ChildUser:");
		
		if (!(domainGroup ->childUsers.size()))
		{
			strResult = strResult +  _T("<") + _T("?")+ _T(">");
		} 
		else
		{
            for (int k = 0; k < domainGroup ->childUsers.size(); k++)
			{
				ADUser* childUser = domainGroup ->childUsers[k];
				strResult += _T("<") + childUser ->cn + _T(">");
			}
		}
		
		strResult += _T("\n");
		}
	}
	
	return strResult;	      
}


