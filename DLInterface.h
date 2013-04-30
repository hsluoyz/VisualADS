#include "StdAfx.h"
#include <vector>
using namespace std;

class DLRacerInterface;
class ADDomain;
class ADACE;
class ADUser;
class ADGroup;
class ADShareFolder;

class DLInterface
{
public:
	int m_iMixOrShareOrNTFS;
	BOOL m_bFileInterface;
	DLRacerInterface *m_racerpro;
	CString m_strInputDir;
	CString m_strOntologyPathFile;
	CString m_strQueryPathFile;
	CString m_strOutputPathFile;

	CString m_strReportPathFile;
	vector<CString> m_nstrReports;

	vector<CString> m_nstrTBox;
	vector<CString> m_nstrSWRLR;
	vector<CString> m_nstrABox;
	vector<CString> m_nstrReset;
	vector<CString> m_nstrOntology;
	vector<CString> m_nstrQuerys;
	vector<CString> m_nstrAnswers;

	int m_iCurConflict;
	int m_iCurReason;
	int m_iCurCommonInstance;
	int m_iConflictCount;
	int m_iReasonCount;
	int m_iCommonInstanceCount;
	vector<CString> m_nstrConflicts;
	vector<CString> m_nstrReasons;
	vector<CString> m_nstrCommonInstances;

	int m_iSelfStartedEngine;

	vector<ADDomain*> m_npDomains;

public:
	DLInterface();
	DLInterface(CString strRacerIP);
	DLInterface(vector<ADDomain*> npDomains, int iMixOrShareOrNTFS);
	DLInterface(vector<ADDomain*> npDomains, CString strRacerIP, int iMixOrShareOrNTFS);
	DLInterface(vector<ADDomain*> npDomains, int iMixOrShareOrNTFS, BOOL bFileInterface);
	~DLInterface();

	void addToReport(CString strText);
	void writeReportFile();

	int getNumberFromOutputFileName();
	void updateOutputFileName();

	void generateOntologyFile();
	void generateQueryFile();
	void clearQueryString();
	BOOL deleteFile(CString strFilePathName);
	void deleteOutputFile();
	void MyWinExec(CString strCmdLine);
	void executeRacerForFile();
	BOOL evaluateLine(CString strLine);
	BOOL FileExist(CString strFileName);
	void parseOutputFileToAnswerString();
	void answerString2ConflictString();
	void answerString2ReasonString();
	void answerString2CommonInstanceString();

	BOOL getSelfStarted();
	static BOOL checkConnection(CString strRacerIP);
	CString itos(int i);
	int stoi(CString s);
	BOOL FolderExist(CString strPath);
	void initInputDir();
	CString doubleBackSlash(CString strLine);
	BOOL createProcess(CString strExeName, CString strCmdLine);
	BOOL killProcess(CString strWindowTitle);
	BOOL startEngine();
	static BOOL checkEngine();
	static void endEngine();
	void strVev2File(CString strFile, vector<CString> nsString);
	void file2StrVec(CString strFile, vector<CString>& nsString);

	void initTBoxVectors();
	void initSWRLRVectors();
	void initResetVectors();
	void initABoxVectors();
	void insertInstanceAssertion(CString strInstance, CString strConcept);
	void insertRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation);

	//TCP Interface
	void submitBlankInformation();
	void submitInformation();

	//File Interface
	void prepareOntologyString();
	void submitOntologyAndQueryString();

	int toBoolean(CString strResult);
	int isNil(CString strResult);
	
	CString x(int i);
	CString parseX(CString strResult, int i);

	void addGroup(CString strGroup);
	void addUser(CString strUser);
	void addShareFolder(CString strShareFolder);
	void addChildRelation(CString strParent, CString strChild);
	void addAccessRelation(CString strADACE, CString strGroupOrUser, CString strShareFolder, CString strAction, int iPermit);

	CString dnName2dlName(CString strDNName);
	CString dlName2dnName(CString strDLName);
	void addDomain(ADDomain *domain, int iMixOrShareOrNTFS);

	int queryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType);
	int queryRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation);
	int queryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType);
	vector<CString> queryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation);
	CString queryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation);

	void prepareQueryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType);
	void prepareQueryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType);
	void prepareQueryRelationAssertion(CString strInstance1, CString strInstance2, CString strRelation);
	void prepareQueryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation);
	void prepareQueryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation);

	int getQueryAccessRelationConflict(CString strADACE1, CString strADACE2, CString strActionType);
	int getQueryAccessRelationConflict2(CString strADACE1, CString strADACE2, CString strActionType);
	int getQueryRelationAssertion_Conflict(CString strInstance1, CString strInstance2, CString strRelation, int iIndex);
	int getQueryRelationAssertion_Reason(CString strInstance1, CString strInstance2, CString strRelation);
	vector<CString> getQueryRelationAssertionLink(CString strInstance1, CString strInstance2, CString strRelation);
	CString getQueryCommonInstanceForTwo(CString strInstance1, CString strInstance2, CString strRelation);

	CString ACE2Action(ADACE *aDACE);
	CString ACE2Type(ADACE *aDACE);
	CString ACE2Permission(ADACE *aDACE);
	CString user2Description(ADUser *user);
	CString group2Description(ADGroup *group);
	CString shareFolder2Description(ADShareFolder *shareFolder);
	CString getUserOrGroupDNNameFromACE(ADACE *ace);
	CString getUserOrGroupCNNameFromACE(ADACE *ace);
	CString getUserOrGroupDescriptionFromACE(ADACE *ace);
	CString getShareFolderDescriptionFromACE(ADACE *ace);
	CString dn2cn(CString strDN);
	BOOL checkGroupOrUserByDN(CString strDN);
	CString dl2Description(CString strDL);
	CString getConflictReasonFromIdentical(CString strNode);
	CString getConflictReasonFromStrings(vector<CString> nstrNodes);
	void queryOneKindOfConflict(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo,
		int iMixOrShareOrNTFS);
	void queryOneKindOfConflict2(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo,
		int iMixOrShareOrNTFS);
	void queryAllConflicts(vector<ADDomain*> npDomains, int &iConflictNo);
	void queryOneKindOfConflict_FileInterface(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo,
		int iMixOrShareOrNTFS);
	void queryOneKindOfConflict2_FileInterface(ADDomain *domain, CString strActionType, CString strActionWord, int &iConflictNo,
		int iMixOrShareOrNTFS);
	void queryAllConflicts_FileInterface(vector<ADDomain*> npDomains, int &iConflictNo);
};