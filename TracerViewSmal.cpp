// TracerViewSmal.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "TracerViewSmal.h"

#include "TV_TracersDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NO_OFF_LED
#define ACTIVE_COLOR RGB(255,127,127)
CFont CTracerViewSmal::m_font;

/////////////////////////////////////////////////////////////////////////////
// CTracerViewSmal

IMPLEMENT_DYNCREATE(CTracerViewSmal, CTracerViewBase)

CTracerViewSmal::CTracerViewSmal()
	: CTracerViewBase(CTracerViewSmal::IDD)
{
	//{{AFX_DATA_INIT(CTracerViewSmal)
	//}}AFX_DATA_INIT
	::memset(&FullStatus,0,sizeof(FULL_STATUS));
	m_bFirstTime = false;
#ifdef NO_OFF_LED
	m_bFocus = true;
#else
	m_bFocus = false;
#endif
	m_bFocus1 = false;
	m_sTextStatus = _T("");
	OldControlerStatus = 0;
	m_bErrConnect = false;
}

CTracerViewSmal::~CTracerViewSmal()
{

}

void CTracerViewSmal::DoDataExchange(CDataExchange* pDX)
{
	CTracerViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracerViewSmal)
	DDX_Control(pDX, IDC_LED_SELECT, m_LedSelect);
	DDX_Control(pDX, IDC_POSITION, m_ColorPosition);
	DDX_Control(pDX, IDC_NAME_DEVICE, m_ColorName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_POSITION,GetDocument()->m_sPosition);
	DDX_Text(pDX, IDC_NAME_DEVICE,GetDocument()->GetName());
	DDX_Control(pDX, IDC_LED,	m_dynLED);
	if(!m_bFirstTime&&!pDX->m_bSaveAndValidate){
		m_bFirstTime=true;

		CWnd *pWndLed = (CWnd *)GetDlgItem(IDC_LED);
		m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);

		pWndLed = (CWnd *)GetDlgItem(IDC_LED_SELECT);
		m_LedSelect.SetLED(pWndLed,ID_LED_BLUE,0/*ID_SHAPE_SQUARE*/,0,false);

		m_ColorName.SetBkColor(GetDocument()->colorBk);
		m_ColorName.SetTextColor(GetDocument()->colorTx);
		if(m_bFocus1)
			m_ColorPosition.SetBkColor(ACTIVE_COLOR);
		else
			m_ColorPosition.SetBkColor(::GetSysColor(COLOR_3DFACE));
		m_ColorPosition.SetTextColor(RGB(0,0,0));

		m_ColorName.SetFont(&m_font);
		m_ColorPosition.SetFont(&m_font);
	}
}


BEGIN_MESSAGE_MAP(CTracerViewSmal, CTracerViewBase)
	//{{AFX_MSG_MAP(CTracerViewSmal)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNeedText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTracerViewSmal diagnostics

#ifdef _DEBUG
void CTracerViewSmal::AssertValid() const
{
	CTracerViewBase::AssertValid();
}

void CTracerViewSmal::Dump(CDumpContext& dc) const
{
	CTracerViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTracerViewSmal message handlers

void CTracerViewSmal::OnInitialUpdate() 
{
	CTracerViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	ResizeParentToFit();
}

void CTracerViewSmal::NewFullStatus(int cou_data)
{
	::memcpy(&FullStatus,&(GetDocument()->FullStatus),sizeof(FULL_STATUS));

	if( ( cou_data != LENGHT_FULL_STATUS ) && ( cou_data != LENGHT_SMAL_STATUS ))
		ASSERT(false);

	if( (OldControlerStatus != FullStatus.ControlerStatus) || m_bErrConnect){
		m_bErrConnect = false;
		/*if((FullStatus.ControlerStatus & BIT_AC_READY)==0){
			m_sTextStatus.LoadString(IDS_AC_ERR);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);
		}else*/ if(FullStatus.ControlerStatus & BIT_LIMIT_ERR){
			m_sTextStatus.LoadString(IDS_LIMIT_ERR);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_Q10){
			m_sTextStatus.LoadString(IDS_Q10);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);
		}else /*if(FullStatus.ControlerStatus & BIT_ERR_REMAK){
			m_sTextStatus.LoadString(IDS_ERR_REMAK);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);
		}else*/ if(FullStatus.ControlerStatus & BIT_LIMIT_UP){
			m_sTextStatus.LoadString(IDS_LIMIT_UP);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_UP,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_OVERLOAD){
			m_sTextStatus.LoadString(IDS_OVERLOAD);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_UP,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_LIMIT_DOWN){
			m_sTextStatus.LoadString(IDS_LIMIT_DOWN);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_DOWN,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_ERR_VAGE){
			m_sTextStatus.LoadString(IDS_ERR_VAGE);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_DOWN,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_LIMIT_SOFT){
			m_sTextStatus.LoadString(IDS_LIMIT_SOFT);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_ROUND,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_LIMIT_LEFT){
			m_sTextStatus.LoadString(IDS_LIMIT_LEFT);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_LEFT,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_LIMIT_RIGHT){
			m_sTextStatus.LoadString(IDS_LIMIT_RIGHT);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_YELLOW,ID_SHAPE_RIGHT,0,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_MOVE_LEFT){
			m_sTextStatus.LoadString(IDS_MOVE_LEFT);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_GREEN,ID_SHAPE_LEFT,500,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_MOVE_RIGHT){
			m_sTextStatus.LoadString(IDS_MOVE_RIGHT);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_GREEN,ID_SHAPE_RIGHT,500,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_MOVE_UP){
			m_sTextStatus.LoadString(IDS_MOVE_UP);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_GREEN,ID_SHAPE_UP,500,m_bFocus);
		}else if(FullStatus.ControlerStatus & BIT_MOVE_DOWN){
			m_sTextStatus.LoadString(IDS_MOVE_DOWN);
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_GREEN,ID_SHAPE_DOWN,500,m_bFocus);
		}else{
			CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
			m_dynLED.SetLED(pWndLed,ID_LED_GREEN,ID_SHAPE_ROUND,0,m_bFocus);
			m_sTextStatus = _T("");
		}
	}
	OldControlerStatus = FullStatus.ControlerStatus;
	UpdateData(false);
}

void CTracerViewSmal::ErrConnect()
{
	m_bErrConnect = true;
	m_sTextStatus.LoadString(IDS_NO_CONNECT);
	CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED);
	m_dynLED.SetLED(pWndLed,ID_LED_RED,ID_SHAPE_ROUND,0,m_bFocus);
	UpdateData(false);
}

void CTracerViewSmal::SetFocus()
{
	if(!m_bFocus1){
		m_bFocus1 = true;
		if(m_bFirstTime){
			m_ColorPosition.SetBkColor(ACTIVE_COLOR);
			UpdateData(false);
		}
	}
#ifndef NO_OFF_LED
	m_bFocus = true;
	if(m_bFirstTime){
		m_dynLED.SwitchOn();
	}
#endif
}

void CTracerViewSmal::KillFocus()
{
	if(m_bFocus1){
		m_bFocus1 = false;
		if(m_bFirstTime){
			m_ColorPosition.SetBkColor(::GetSysColor(COLOR_3DFACE));
			UpdateData(false);
		}
	}
#ifndef NO_OFF_LED
	m_bFocus = false;

	if(m_bFirstTime)
		m_dynLED.SwitchOff();
#endif
}

BOOL CTracerViewSmal::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (::IsWindow(m_ToolTip.m_hWnd) && pMsg->hwnd == m_hWnd)
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN:	
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:	
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:	
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			m_ToolTip.RelayEvent(pMsg);
			break;
		}
	}
	return CTracerViewBase::PreTranslateMessage(pMsg);
}

void CTracerViewSmal::MoveInOldPositionAndName(CString name, WORD IdHardware, int ModbusAddrres, int x, int y)
{
	CTracerViewBase::MoveInOldPositionAndName(name,IdHardware,ModbusAddrres,x,y);


  if (m_ToolTip.Create(this, TTS_ALWAYSTIP) && m_ToolTip.AddTool(this))
	{
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 200);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, 200);
		m_ToolTip.Activate(TRUE);
	}
	else
	{
		TRACE("Error in creating ToolTip");
	}
}

BOOL CTracerViewSmal::OnToolTipNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	BOOL bHandledNotify = FALSE;

	CPoint CursorPos;
	VERIFY(::GetCursorPos(&CursorPos));
	ScreenToClient(&CursorPos);

	CRect ClientRect;
	GetClientRect(ClientRect);

	// Make certain that the cursor is in the client rect, because the
	// mainframe also wants these messages to provide tooltips for the
	// toolbar.
	if (ClientRect.PtInRect(CursorPos))
	{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		{
			// Adjust the text by filling in TOOLTIPTEXT
			CString strTip;
			
				strTip.Format(_T("%s\n%s    %s\n%s\n%s"),GetDocument()->GetTitle(),
				GetDocument()->m_sPosition,
				GetDocument()->m_sStoredPosition,
				m_sTextStatus,
				GetDocument()->help1);
			
			ASSERT(strTip.GetLength() < sizeof(pTTT->szText));
			::strcpy(pTTT->szText,strTip);

			// Set the text color to same color as circle
			//COLORREF Color=RGB(50,50,50);
			//m_ToolTip.SendMessage(TTM_SETTIPTEXTCOLOR, Color, 0L);
		}
		bHandledNotify = TRUE;
	}
	return bHandledNotify;
}

void CTracerViewSmal::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);

	if(pApp->privilege)
		pApp->ExtendedView(GetDocument());

	CTracerViewBase::OnRButtonDown(nFlags, point);
}

void CTracerViewSmal::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	if(pApp->privilege)
		pApp->ExtendedView(GetDocument());
	CTracerViewBase::OnLButtonDblClk(nFlags, point);
}

void CTracerViewSmal::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTracerViewSmal::InGroup()
{
	CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED_SELECT);
	m_LedSelect.SetLED(pWndLed,ID_LED_BLUE,ID_SHAPE_SQUARE,0,false);
}

void CTracerViewSmal::OutGroup()
{
	CWnd * pWndLed = (CWnd *)GetDlgItem(IDC_LED_SELECT);
	m_LedSelect.SetLED(pWndLed,ID_LED_BLUE,0/*ID_SHAPE_SQUARE*/,0,false);
}
