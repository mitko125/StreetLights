// TV_Tracers.h : main header file for the TV_TRACERS application
//

#if !defined(AFX_TV_TRACERS_H__C1A87849_EB22_41EF_9A9D_E748C6399D6F__INCLUDED_)
#define AFX_TV_TRACERS_H__C1A87849_EB22_41EF_9A9D_E748C6399D6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Message.h"

#include "JennicModule.h"

#define DATABASE_VERSION 1
#define DATABASE_SUB_VERSION 3

#define MAX_PROG_ARREA 262144

#define TEST_LINK_VIEW_POS 1
#define DEVICE_VIEW_POS 2
#define ACOUTS_VIEW_POS 3
#define DIALOG_VIEW_POS 4
#define EXTENDED_VIEW_POS 5
/////////////////////////////////////////////////////////////////////////////
// CTV_TracersApp:
// See TV_Tracers.cpp for the implementation of this class
//
#define MAX_ALL_DEVICES 10
typedef enum{
		_CONFIG_ROUTERS = 0,
		_CONFIG_LAMPS,
		_CONFIG_OPERATORS,
		_CONFIG_PC_NUMBER,
		_CONFIG_CONNECTION_DB,
		_CLEAR_ACCOUNTS,
		_CONFIG_ENERGY_METERS,
}config_messages;

typedef enum{
	_UNDEFINET = 0,
	_NONE,//1
	_NO_CONNEC,//2
	_AC_ERR,//3
	_ERR_VAGE,//4
	_LIMIT_UP,//5
	_LIMIT_DOWN,//6
	_LIMIT_SOFT,//7
	_LOCAL_UP,//8
	_LOCAL_DOWN,//9
	_MOVE_UP,//10
	_MOVE_DOWN,//11
	_MOVE_UP_FAST,//12
	_MOVE_DOWN_FAST,//13
	_GO_REPER,//14
	_GO_POS,//15
	_ERR_REMAK,//16
	_LIMIT_ERR,//17
	_Q10,//18
	_OVERLOAD,//19
	_MOVE_LEFT,//20
	_MOVE_RIGHT,//21
	_LIMIT_LEFT,//22
	_LIMIT_RIGHT,//23
}device_messages;


class CProtocolManager;
class CGridCtrl;
class CDataViewDoc;
class CTV_TracersDoc;
class COperatorManager;
class CHardwareBase;
class CTV_TracersApp : public CWinApp
{
	friend CTV_TracersDoc;
public:
	void SetNewDecory();
	COperatorManager * m_pOperators;
	CString m_cNameOperator;
	UINT32 privilege;
	int x,y;
	BOOL m_bEnableMoving;
	BOOL m_bEnableMovingTemp;
	BOOL fl_NeedSetup;
	CFrameWnd * pFrameExtendedView;
	CTV_TracersDoc * ActiveTracker;
	void ExtendedView(CDocument *pDoc);
	WORD Id_Operator;
	void GridPrintPreviev(CGridCtrl *pGrid,CDialog *pCDialog);
	void GridPrint(CGridCtrl *pGrid,CDialog *pCDialog);
	UINT Id_PC;
	double	m_dLat;
	double	m_dLon;
	int		m_OffsetOnOff;
	volatile BOOL	m_bAutoOnOff;

	void LoadStdProfileSettings(unsigned int cou_receptien_files);
	CTV_TracersApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTV_TracersApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTV_TracersApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePagesetup();
	afx_msg void OnDeviceSetup();
	afx_msg void OnSelectOperator();
	afx_msg void OnSelectPc();
	afx_msg void OnUpdateSelectPc(CCmdUI* pCmdUI);
	afx_msg void OnCobfigOperator();
	afx_msg void OnUpdateCobfigOperator(CCmdUI* pCmdUI);
	afx_msg void OnConfigDb();
	afx_msg void OnUpdateConfigDb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeviceSetup(CCmdUI* pCmdUI);
	afx_msg void OnAccountsSystem();
	afx_msg void OnAccountsClear();
	afx_msg void OnUpdateAccountsClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLampsSetup(CCmdUI* pCmdUI);
	afx_msg void OnLampsSetup();
	afx_msg void OnTestLqi();
	//}}AFX_MSG
	CRect Margin;
	BOOL m_bFitPage,m_bPrintBlackWhite;
protected:
	afx_msg LRESULT OnAutoUpdateTimer(WPARAM wParam, LPARAM lParam);
	int NewRown(int * num_line,int depth,CGridCtrl *pGrid,CString MAC_address,int lamp_number);
	void TestILQ(int * num_line,int depth,CGridCtrl *pGrid,CString ip_address,CString Old_MAC_address,int old_row,CString MAC_address,UINT32 id_hardware,int lamp_number);
	CString NewAccountTitle(UINT id);
	void SetDefaultGrid(CGridCtrl *pGrid);
	CGridCtrl * CreateDefaultGrid();
	void SetAccountTitle(CDataViewDoc *pSDoc,UINT ids);
	CDataViewDoc * GetNewDataViewDoc(BOOL bMakeVisible = TRUE);
	BOOL SetHardwareCommand(WORD IdHardware, int ModbusAddrres, COMMANDS * Command);
	void SetExtendetView(WORD IdHardware, int ModbusAddrres,BOOL fl_Extend);
	CDataViewDoc * GetNewDataViewDocToDialog();
	DWORD GetHardwareThreadId(WORD IdHardware);
	afx_msg LRESULT BadSocketConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPortUnkErr(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPortNotFree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPortNotAvailabe(WPARAM wParam, LPARAM lParam);
	CString m_cTitle;

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TV_TRACERS_H__C1A87849_EB22_41EF_9A9D_E748C6399D6F__INCLUDED_)
