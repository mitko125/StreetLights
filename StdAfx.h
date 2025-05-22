// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__562ABDAB_8C26_44C8_9D61_A35D514D49C3__INCLUDED_)
#define AFX_STDAFX_H__562ABDAB_8C26_44C8_9D61_A35D514D49C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(push)
#pragma warning(disable:4146)
#import "c:\Program Files\Common Files\System\ADO\msado28.tlb" \
    no_namespace rename("EOF", "EndOfFile")
#include "icrsint.h"
inline void TESTHR(HRESULT _hr)
{
	if FAILED(_hr) _com_issue_error(_hr);
}

#include <afxmt.h>

#include <afxsock.h>		// MFC socket extensions

#include <mmsystem.h> //Joystick

#include <afxpriv.h>

#include "C:\Program Files (x86)\HTML Help Workshop\include\htmlhelp.h"
#include "html\StreetLights.hm"



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__562ABDAB_8C26_44C8_9D61_A35D514D49C3__INCLUDED_)
