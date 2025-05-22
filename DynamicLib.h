// DynamicLib.h: interface for the Clients class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIENTS_H__DD9212E1_2C73_4972_A1D4_2D2BCF6A2543__INCLUDED_)
#define AFX_LIENTS_H__DD9212E1_2C73_4972_A1D4_2D2BCF6A2543__INCLUDED_

#include "HardwareBase.h"
#include "JennicModule.h"
#include "ServerTCP.h"

#ifdef MAIN 
HINSTANCE hTestView=NULL;
CView *p_TLView=NULL;
CJennicModule JennicModule;
CHardwareBase * p_hardware;
CServerTCP * p_STCP;

#else
extern HINSTANCE hTestView;
extern CView *p_TLView;
extern CJennicModule JennicModule;
extern CHardwareBase * p_hardware;
extern CServerTCP * p_STCP;
#endif

#endif // !defined(AFX_LIENTS_H__DD9212E1_2C73_4972_A1D4_2D2BCF6A2543__INCLUDED_)
