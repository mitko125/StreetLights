// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "MainFrm.h"
#include "TV_TracersDoc.h"

#include "DynamicLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_CASCADE, OnWindowCascade)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_MESSAGE(MM_JOY1MOVE,OnJoystickMove)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SO, OnUpdateSO)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LIM, OnUpdateLim)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	/*ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,*/

	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_SO,
	ID_INDICATOR_LIM,
//	ID_INDICATOR_SD,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	joyReleaseCapture(JOYSTICKID1);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if(joySetCapture(m_hWnd, JOYSTICKID1, 0, TRUE)){//FALSE)){
		//MessageBeep(MB_ICONEXCLAMATION);
		//AfxMessageBox("Couldn't capture the joystick", NULL,MB_OK | MB_ICONEXCLAMATION);
  }   
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_HSCROLL | WS_VSCROLL ;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::OnJoystickMove(WPARAM wParam, LPARAM lParam)
{
	CFrameWnd * pFr = GetActiveFrame();
	if(pFr){
		CDocument * pDoc = pFr->GetActiveDocument();
		if( pDoc ){
			if( pDoc->IsKindOf(RUNTIME_CLASS(CTV_TracersDoc))){
				CTV_TracersDoc * m_pDocument = (CTV_TracersDoc*) pDoc;
				
				int x, y;
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				x -= 0x7FFF;
				y -= 0x7FFF;
				if( y > 2000){//Down
					COMMANDS * pC = new COMMANDS;
					pC->TargetPos = 0;
					pC->RemoteCommands = BIT_REMOTE_MOVE_DOWN;
					m_pDocument->SetHardwareCommand(pC);
				}else if( y < -2000 ){//Up
					COMMANDS * pC = new COMMANDS;
					pC->TargetPos = 0;
					pC->RemoteCommands = BIT_REMOTE_MOVE_UP;
					m_pDocument->SetHardwareCommand(pC);
				}else if( x < -2000 ){//Left
					COMMANDS * pC = new COMMANDS;
					pC->TargetPos = 0;
					pC->RemoteCommands = BIT_REMOTE_MOVE_LEFT;
					m_pDocument->SetHardwareCommand(pC);
				}else if( x > 2000 ){//Right
					COMMANDS * pC = new COMMANDS;
					pC->TargetPos = 0;
					pC->RemoteCommands = BIT_REMOTE_MOVE_RIGHT;
					m_pDocument->SetHardwareCommand(pC);
				}else{
					COMMANDS * pC = new COMMANDS;
					pC->TargetPos = 0;
					pC->RemoteCommands = 0;
					m_pDocument->SetHardwareCommand(pC);
				}
			}
		}
	}
	return 0;
}

void CMainFrame::OnWindowCascade() 
{
	// TODO: Add your command handler code here
	HideTracers();

	::SendMessage(m_hWndMDIClient,WM_MDICASCADE,0,0);

	ShowTracers();
}

void CMainFrame::OnWindowTileHorz() 
{
	// TODO: Add your command handler code here
	HideTracers();

	::SendMessage(m_hWndMDIClient,WM_MDITILE,MDITILE_HORIZONTAL,0);

	ShowTracers();
}

void CMainFrame::OnWindowTileVert() 
{
	// TODO: Add your command handler code here
	HideTracers();

	::SendMessage(m_hWndMDIClient,WM_MDITILE,0,0);

	ShowTracers();
}

void CMainFrame::HideTracers()
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);

	CDocTemplate *m_pDocTemplate;
	POSITION pos=pApp->m_pDocManager->GetFirstDocTemplatePosition();
	for(int h = 0 ; h < DEVICE_VIEW_POS; h++ )
		m_pDocTemplate=pApp->m_pDocManager->GetNextDocTemplate(pos);

	POSITION pos1=m_pDocTemplate->GetFirstDocPosition();
	while(pos1){
		CDocument * pDoc=m_pDocTemplate->GetNextDoc(pos1);
		ASSERT(pDoc!=NULL);
		POSITION pos2=pDoc->GetFirstViewPosition();
		while(pos2){
			CView * pV=pDoc->GetNextView(pos2);
			pV->GetParentFrame()->ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::ShowTracers()
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);

	CDocTemplate *m_pDocTemplate;
	POSITION pos=pApp->m_pDocManager->GetFirstDocTemplatePosition();
	for(int h = 0 ; h < DEVICE_VIEW_POS; h++ )
		m_pDocTemplate=pApp->m_pDocManager->GetNextDocTemplate(pos);

	POSITION pos1=m_pDocTemplate->GetFirstDocPosition();
	while(pos1){
		CDocument * pDoc=m_pDocTemplate->GetNextDoc(pos1);
		ASSERT(pDoc!=NULL);
		POSITION pos2=pDoc->GetFirstViewPosition();
		while(pos2){
			CView * pV=pDoc->GetNextView(pos2);
			pV->GetParentFrame()->ShowWindow(SW_SHOW);
		}
	}
}

void CMainFrame::OnUpdateSO(CCmdUI *pCmdUI)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	CString text;
	text=pApp->m_cNameOperator;
	pCmdUI->SetText(text);
}

void CMainFrame::OnUpdateLim(CCmdUI *pCmdUI)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	if( p_hardware != NULL )
		pCmdUI->SetText(p_hardware->m_sConnectServer);//text);
	else
		pCmdUI->SetText("");
}

void CMainFrame::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//CMDIFrameWnd::WinHelp(dwData, nCmd);
	TRACE("Подсказка для IDH=0x%X\n",dwData);
	//!!!	 Промяна при преминаване към ProjectsVS2015 така или наче няма Help, най-правилен се оказа HtmlHelp(dwData, nCmd);
	//но трябва да се направи help
	//!!!  if( HtmlHelp(m_hWnd, AfxGetApp()->m_pszHelpFilePath, HH_HELP_CONTEXT,nCmd == HELP_CONTEXT ? dwData : 0) == NULL )
	//!!!    	HtmlHelp(m_hWnd, AfxGetApp()->m_pszHelpFilePath, HH_HELP_FINDER, 0 );
	//HtmlHelp(dwData, nCmd);
}
