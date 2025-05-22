// TracerViewBase.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "TracerViewBase.h"

#include "TV_TracersDoc.h"
#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CFrameWnd * first_activ_tracer;
/////////////////////////////////////////////////////////////////////////////
// CTracerViewBase

IMPLEMENT_DYNCREATE(CTracerViewBase, CFormView)

CTracerViewBase::CTracerViewBase()
	: CFormView(CTracerViewBase::IDD)
{
	//{{AFX_DATA_INIT(CTracerViewBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	id_device = 0;
	old_x = old_y = 0;
}


CTracerViewBase::CTracerViewBase(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{

}

CTracerViewBase::~CTracerViewBase()
{
}

void CTracerViewBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracerViewBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTracerViewBase, CFormView)
	//{{AFX_MSG_MAP(CTracerViewBase)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MW_HARDWARE_STOPED,OnHardwareStoped)
	ON_MESSAGE(MW_REDRAW_FULL_SATUS,OnRedrawFullStatus)
	ON_MESSAGE(MW_ERR_CONNECT,OnErrConnect)
	ON_MESSAGE(MW_END_PROGRAM,OnEndProgram)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTracerViewBase diagnostics

#ifdef _DEBUG
void CTracerViewBase::AssertValid() const
{
	CFormView::AssertValid();
}

void CTracerViewBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTV_TracersDoc* CTracerViewBase::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTV_TracersDoc)));
	return (CTV_TracersDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTracerViewBase message handlers

void CTracerViewBase::MoveInOldPositionAndName(CString name, WORD IdHardware, int ModbusAddrres,int x,int y)
{
	GetDocument()->IdHardware = IdHardware;
	GetDocument()->m_iModbusAddres = ModbusAddrres;

	ResizeParentToFit(false);
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	RECT rect;
	pFrame->GetWindowRect(&rect);
	
	if((x>=0)&&(y>=0)){		
		old_x = x;
		old_y = y;
		rect.right=rect.right-rect.left+x;
		rect.left=x;
		rect.bottom=rect.bottom-rect.top+y;
		rect.top=y;
		pFrame->MoveWindow(&rect);
	}
	pFrame->ShowWindow(SW_SHOW);
	GetDocument()->SetTitle(name);

	if(!first_activ_tracer){
		first_activ_tracer=pFrame;
		pFrame->ActivateFrame();
	}
	UpdateData(false);
}

LRESULT CTracerViewBase::OnHardwareStoped(WPARAM wParam, LPARAM lParam)
{
	first_activ_tracer=NULL;
/*	CSapsanApp * pApp=dynamic_cast<CSapsanApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	if(pApp->m_iNumSelectedDispenser)
		if(pApp->m_iNumSelectedDispenser==GetDocument()->Id_Dispenser){
			pApp->m_iNumSelectedDispenser=0;
		}
*/
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);

	if(id_device){
		CRect rect,rectm;
		pFrame->GetWindowRect(&rect);
		pFrame->ShowWindow(SW_HIDE);   
		
		rectm=rect;
		rectm.OffsetRect(-rect.left,-rect.top);
		
		pFrame->MoveWindow(&rectm);
		pFrame->GetWindowRect(&rectm);
		
		rect.left-=rectm.left;
		rect.top-=rectm.top;
		if( ( rect.left != old_x ) || ( rect.top != old_y) ){
			_CommandPtr m_pCmd=NULL;
			try{
				CString com;
				com.Format(_T("UPDATE devices SET x=%d,y=%d WHERE id_device=%u"),rect.left,rect.top,id_device);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);	
			}
			catch(_com_error &e){
				::GenerateSqlError(e);
			}
		}
	}
	GetDocument()->HardwareStoped(wParam);
	return 0;
}

LRESULT CTracerViewBase::OnRedrawFullStatus(WPARAM wParam, LPARAM lParam)
{
	FULL_STATUS *pFS = (FULL_STATUS*)wParam;

	GetDocument()->NewFullStatus(pFS,lParam);
	return 0;
}

void CTracerViewBase::HardwareStoped(BOOL CloseWindows)
{
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	if(CloseWindows)
		::PostMessage(pFrame->m_hWnd,WM_CLOSE,0,0);
}

LRESULT CTracerViewBase::OnErrConnect(WPARAM wParam, LPARAM lParam)
{
	GetDocument()->ErrConnect();
	return 0;
}

void CTracerViewBase::OnSetFocus(CWnd *pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);

	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	if(pApp->ActiveTracker){
		if(pApp->ActiveTracker != GetDocument()){
			pApp->ActiveTracker->KillFocus();
			if(pApp->ActiveTracker->RemoteCommands){
				COMMANDS * pC = new COMMANDS;
				pC->TargetPos = 0;
				pC->RemoteCommands = 0;
				pApp->ActiveTracker->SetHardwareCommand(pC);
			}
		}
	}
	pApp->ActiveTracker = GetDocument();
	SetFocus();
}

LRESULT CTracerViewBase::OnEndProgram(WPARAM wParam, LPARAM lParam)
{
	GetDocument()->EndProgram(wParam);
	return 0;
}
