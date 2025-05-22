// TV_TracersView.cpp : implementation of the CTV_TracersView class
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "TV_TracersDoc.h"
#include "TV_TracersView.h"

#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersView

IMPLEMENT_DYNCREATE(CTV_TracersView, CTracerViewBase)

BEGIN_MESSAGE_MAP(CTV_TracersView, CTracerViewBase)
	//{{AFX_MSG_MAP(CTV_TracersView)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_BUTTON_REPER, OnButtonReper)
	ON_BN_CLICKED(IDC_BUTTON_POSITION, OnButtonPosition)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersView construction/destruction

CTV_TracersView::CTV_TracersView()
	: CTracerViewBase(CTV_TracersView::IDD)
{
	//{{AFX_DATA_INIT(CTV_TracersView)
	m_sHarwareLimits = _T("");
	m_sLoaclMovs = _T("");
	m_sAC_Ready = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	::memset(&FullStatus,0,sizeof(FULL_STATUS));
}

CTV_TracersView::~CTV_TracersView()
{
	OnStop();
	GetDocument()->SetExtendetView(false);
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	pApp->pFrameExtendedView = NULL;
}

void CTV_TracersView::DoDataExchange(CDataExchange* pDX)
{
	CTracerViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTV_TracersView)
	DDX_Text(pDX, IDC_HARDWARE_LIMITS, m_sHarwareLimits);
	DDX_Text(pDX, IDC_LOCAL_MOVS, m_sLoaclMovs);
	DDX_Text(pDX, IDC_AC_READY, m_sAC_Ready);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATUS, FullStatus.ControlerStatus);
	DDX_Text(pDX, IDC_CURRENT_POS, FullStatus.CurrentPos);
	DDX_Text(pDX, IDC_VERSION, FullStatus.Version);
	DDX_Text(pDX, IDC_ERR_PULSES, FullStatus.Err_Pulses);
	DDX_Text(pDX, IDC_CURRENT_PULSES, FullStatus.Lenght_Pulses);
	DDX_Text(pDX, IDC_AC_STATUS, FullStatus.StatusAC);
	DDX_Text(pDX, IDC_AC_RPM, FullStatus.RpmAC);
	DDX_Text(pDX, IDC_AC_CURRENT, FullStatus.CurrentAC);
	DDX_Text(pDX, IDC_AC_HERC, FullStatus.HercAC);
	DDX_Text(pDX, IDC_DAC, FullStatus.Dac_to_AC_Register);
	DDX_Text(pDX, IDC_DC, FullStatus.Adc_DC_Volts);
	DDX_Text(pDX, IDC_VDD, FullStatus.Adc_VDD_Volts);
	DDX_Text(pDX, IDC_50Hz, FullStatus.Time50Hz);
	DDX_Text(pDX, IDC_HELP1, GetDocument()->help1);
	DDX_Text(pDX, IDC_HELP2, GetDocument()->help2);
	DDX_Text(pDX, IDC_OFFSET, GetDocument()->m_sOffset);
	DDX_Text(pDX, IDC_MIN, GetDocument()->m_sMin);
	DDX_Text(pDX, IDC_MAX, GetDocument()->m_sMax);
	DDX_Text(pDX, IDC_POSITION,GetDocument()->m_sPosition);
	DDX_Text(pDX, IDC_STORED_POSITION,GetDocument()->m_sStoredPosition);
}

BOOL CTV_TracersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CTracerViewBase::PreCreateWindow(cs);
}

void CTV_TracersView::OnInitialUpdate()
{
	CTracerViewBase::OnInitialUpdate();

	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());	

	ResizeParentToFit(false);
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	RECT rect;
	pFrame->GetWindowRect(&rect);
	
	if((pApp->x>=0)&&(pApp->y>=0)){		
		rect.right=rect.right-rect.left+pApp->x;
		rect.left=pApp->x;
		rect.bottom=rect.bottom-rect.top+pApp->y;
		rect.top=pApp->y;
		pFrame->MoveWindow(&rect);
	}
	pFrame->ShowWindow(SW_SHOW);

	//ResizeParentToFit();
	GetDocument()->SetExtendetView(true);
	NewFullStatus(LENGHT_FULL_STATUS);
	CButton *pB=(CButton*)GetDlgItem(IDC_PROG_DEVICE);
#ifndef _DEBUG
//	pB->EnableWindow(false);
//	pB->ShowWindow(SW_HIDE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersView printing

BOOL CTV_TracersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTV_TracersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTV_TracersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTV_TracersView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersView diagnostics

#ifdef _DEBUG
void CTV_TracersView::AssertValid() const
{
	CTracerViewBase::AssertValid();
}

void CTV_TracersView::Dump(CDumpContext& dc) const
{
	CTracerViewBase::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersView message handlers

BOOL CTV_TracersView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( wParam ){
		case IDC_BUTTON_UP_SLOW:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				COMMANDS * pC = new COMMANDS;
				pC->TargetPos = 0;
				pC->RemoteCommands = 0;
				if( comand->code == NM_CLICK)
					pC->RemoteCommands = BIT_REMOTE_MOVE_LEFT;
				GetDocument()->SetHardwareCommand(pC);
			}
			return true;
		case IDC_BUTTON_DOWN_SLOW:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				COMMANDS * pC = new COMMANDS;
				pC->TargetPos = 0;
				pC->RemoteCommands = 0;
				if( comand->code == NM_CLICK)
					pC->RemoteCommands = BIT_REMOTE_MOVE_RIGHT;
				GetDocument()->SetHardwareCommand(pC);
			}
			return true;
		case IDC_BUTTON_UP_FAST:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				COMMANDS * pC = new COMMANDS;
				pC->TargetPos = 0;
				pC->RemoteCommands = 0;
				if( comand->code == NM_CLICK)
					pC->RemoteCommands = BIT_REMOTE_MOVE_UP;// | BIT_REMOTE_MOVE_FAST;
				GetDocument()->SetHardwareCommand(pC);
			}
			return true;
		case IDC_BUTTON_DOWN_FAST:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				COMMANDS * pC = new COMMANDS;
				pC->TargetPos = 0;
				pC->RemoteCommands = 0;
				if( comand->code == NM_CLICK)
					pC->RemoteCommands = BIT_REMOTE_MOVE_DOWN;// | BIT_REMOTE_MOVE_FAST;
				GetDocument()->SetHardwareCommand(pC);
			}
			return true;
/*		case IDC_BUTTON_REPER:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				if( comand->code == NM_CLICK){
					COMMANDS * pC = new COMMANDS;
				
					pC->TargetPos = 0;
					pC->RemoteCommands = BIT_REMOTE_REPER;
					GetDocument()->SetHardwareCommand(pC);
				}
			}
			return true;*/
/*		case IDC_BUTTON_POSITION:
			{
				NMHDR * comand = (NMHDR *)lParam; 
				*pResult = 1;
				if( comand->code == NM_CLICK){
					COMMANDS * pC = new COMMANDS;
				
					CString text;
					int data;
					GetDlgItemText( IDC_EDIT_POSITION, text);
					::sscanf(text,_T("%d"),&data);
					pC->TargetPos = data;
					pC->RemoteCommands = BIT_REMOTE_POSITION;
					GetDocument()->SetHardwareCommand(pC);
				}
			}
			return true;*/
	}
	return CTracerViewBase::OnNotify(wParam, lParam, pResult);
}

void CTV_TracersView::OnStop() 
{
	// TODO: Add your control notification handler code here

	COMMANDS * pC = new COMMANDS;
	pC->TargetPos = 0;
	pC->RemoteCommands = 0;
	GetDocument()->SetHardwareCommand(pC);
}

void CTV_TracersView::NewFullStatus(int cou_data)
{
	::memcpy(&FullStatus,&(GetDocument()->FullStatus),sizeof(FULL_STATUS));
	
	if( ( cou_data != LENGHT_FULL_STATUS ) && ( cou_data != LENGHT_SMAL_STATUS ))
		ASSERT(false);

	if(FullStatus.ControlerStatus & BIT_LIMIT_ERR)
		m_sHarwareLimits.LoadString(IDS_LIMIT_ERR);
	else if(FullStatus.ControlerStatus & BIT_Q10)
		m_sHarwareLimits.LoadString(IDS_Q10);
	else /*if(FullStatus.ControlerStatus & BIT_ERR_REMAK)
		m_sHarwareLimits.LoadString(IDS_ERR_REMAK);
	else */if(FullStatus.ControlerStatus & BIT_LIMIT_UP)
		m_sHarwareLimits.LoadString(IDS_LIMIT_UP);
	else if(FullStatus.ControlerStatus & BIT_OVERLOAD)
		m_sHarwareLimits.LoadString(IDS_OVERLOAD);
	else if(FullStatus.ControlerStatus & BIT_LIMIT_DOWN)
		m_sHarwareLimits.LoadString(IDS_LIMIT_DOWN);
	else if(FullStatus.ControlerStatus & BIT_ERR_VAGE)
		m_sHarwareLimits.LoadString(IDS_ERR_VAGE);
	else if(FullStatus.ControlerStatus & BIT_LIMIT_SOFT)
		m_sHarwareLimits.LoadString(IDS_LIMIT_SOFT);
	else if(FullStatus.ControlerStatus & BIT_MOVE_UP)
		m_sHarwareLimits.LoadString(IDS_MOVE_UP);
	else if(FullStatus.ControlerStatus & BIT_MOVE_DOWN)
		m_sHarwareLimits.LoadString(IDS_MOVE_DOWN);
	else
		m_sHarwareLimits.LoadString(IDS_LIMIT_NO);

	if(FullStatus.ControlerStatus & BIT_LIMIT_LEFT)
		m_sLoaclMovs.LoadString(IDS_LIMIT_LEFT);
	else if(FullStatus.ControlerStatus & BIT_LIMIT_RIGHT)
		m_sLoaclMovs.LoadString(IDS_LIMIT_RIGHT);
	else if(FullStatus.ControlerStatus & BIT_MOVE_LEFT)
		m_sLoaclMovs.LoadString(IDS_MOVE_LEFT);
	else if(FullStatus.ControlerStatus & BIT_MOVE_RIGHT)
		m_sLoaclMovs.LoadString(IDS_MOVE_RIGHT);
	else
		m_sLoaclMovs.LoadString(IDS_NO_LEFT_RIGHT);//IDS_LOCAL_NO);

	if(FullStatus.ControlerStatus & BIT_AC_READY)
		m_sAC_Ready.LoadString(IDS_AC_OK);
	else
		m_sAC_Ready.LoadString(IDS_AC_ERR);
	
	UpdateData(false);
}

void CTV_TracersView::ErrConnect()
{
	m_sHarwareLimits.LoadString(IDS_NO_CONNECT);

	UpdateData(false);
}

void CTV_TracersView::EndProgram(BOOL fl_ok)
{
	if(fl_ok)
		AfxMessageBox(IDS_PROGRAM_OK);
	else
		AfxMessageBox(IDS_PROGRAM_ERR);
}

void CTV_TracersView::OnButtonReper() 
{
	// TODO: Add your control notification handler code here
	COMMANDS * pC = new COMMANDS;
				
	pC->TargetPos = 0;
	pC->RemoteCommands = BIT_REMOTE_REPER;
	GetDocument()->SetHardwareCommand(pC);
}

void CTV_TracersView::OnButtonPosition() 
{
	// TODO: Add your control notification handler code here

	CString text;
	double data;
	GetDlgItemText( IDC_EDIT_POSITION, text);
	::sscanf(text,_T("%lf"),&data);
	CMyCurrency newpos;
	newpos.FromDoubleLegal(data);
	GetDocument()->GoToPos(newpos);
}

void CTV_TracersView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTV_TracersView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTV_TracersView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTV_TracersView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTV_TracersView::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(false);
}

void CTV_TracersView::StoreOldPosition()
{
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);


		CRect rect,rectm;
		pFrame->GetWindowRect(&rect);
		pFrame->ShowWindow(SW_HIDE);   
		
		rectm=rect;
		rectm.OffsetRect(-rect.left,-rect.top);
		
		pFrame->MoveWindow(&rectm);
		pFrame->GetWindowRect(&rectm);
		
		rect.left-=rectm.left;
		rect.top-=rectm.top;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		pApp->x=rect.left;
		pApp->y=rect.top;
}
