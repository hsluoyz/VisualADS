#include "BCGPRibbonEdit.h"

class CMyRibbonEdit : public CBCGPRibbonEdit  
{
	friend class CBCGPRibbonEditCtrl;
	friend class CBCGPClearButton;
	friend class CBCGPRibbonCollector;

	//DECLARE_DYNCREATE(CMyRibbonEdit)

// Construction:
public:
	CMyRibbonEdit();
	CMyRibbonEdit(UINT nID, int nWidth, LPCTSTR lpszLabel = NULL, int nImage = -1);
	~CMyRibbonEdit();

// Overrides
public:
	CBCGPRibbonEditCtrl* CreateEdit(CWnd* pWndParent, DWORD dwEditStyle);
};