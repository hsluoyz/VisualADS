// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifdef WIN32
#pragma warning (disable: 4514 4786 4018 4146)
#endif

#define _WIN32_WINNT 0x0500

#if !defined(AFX_STDAFX_H__C1FF9715_87B1_4CBB_9C83_020355D49526__INCLUDED_)
#define AFX_STDAFX_H__C1FF9715_87B1_4CBB_9C83_020355D49526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#if _MSC_VER < 1500
	#define WINVER				0x0400
	#if _MSC_VER >= 1300
		#define _WIN32_WINNT	0x0400
	#endif
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <BCGCBProInc.h>			// BCGControlBar Pro

//#include <fo.H>

#include <FODll.h>
#include <GdiplusH.h>

#include "stdstring.h"
#define WIN32_LEAN_AND_MEAN

#define _WSPIAPI_COUNTOF

#include "MyMessageBox.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C1FF9715_87B1_4CBB_9C83_020355D49526__INCLUDED_)
