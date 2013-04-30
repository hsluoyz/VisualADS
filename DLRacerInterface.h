#include <winsock2.h>
#include <vector>
using namespace std;

class DLRacerInterface
{
public:
	CString m_strIP;
	int m_iPort;

	WSADATA wsaData;
	SOCKET racerSocket;
	SOCKADDR_IN racerAddr;

public:
	DLRacerInterface(CString a_strIP, int a_iPort);
	~DLRacerInterface();
	BOOL openConnection();
	void closeConnection();

	CString readFromSocket();
	CString sendCommand(CString strCommand);
	CString parseResult(CString strCommand, CString strResult);

	CString getWarningFromOK(CString strResult);
	void printWarning(CString strWarning);
	CString getError(CString strResult);
	vector<CString> getResultAndWarningFromAnswer(CString strResult);

};