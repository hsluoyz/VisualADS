#include <VECTOR>
using namespace std;

class ADDomain;

class ADCNDPGenerator
{
public:
	vector<ADDomain*> m_globalDomains;
	CString m_strCNDPPathName;
	CString m_strCNDPString;

public:
	ADCNDPGenerator(vector<ADDomain*> globalDomains, CString strCNDPPathName);
	void execute();
	CString domain2FileString(ADDomain* domain);
};