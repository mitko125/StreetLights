// TV_Tracers.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TV_TracersDoc.h"
#include "TV_TracersView.h"
#include "TracerViewSmal.h"

#define MAIN

#include "Message.h"
#include "DynamicLib.h"
#include "DataViewDoc.h"
#include "DataViewView.h"

#include "HardwareTCP.h"
#include "HardwareCom.h"
#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#include "OperatorManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AFX_STATIC_DATA const TCHAR _afxPageMargin[] = _T("Page Margin");
AFX_STATIC_DATA const TCHAR _afxTop[] = _T("Top");
AFX_STATIC_DATA const TCHAR _afxLeft[] = _T("Left");
AFX_STATIC_DATA const TCHAR _afxRight[] = _T("Right");
AFX_STATIC_DATA const TCHAR _afxBottom[] = _T("Bottom");
AFX_STATIC_DATA const TCHAR _afxFitPage[] = _T("FitPage");
AFX_STATIC_DATA const TCHAR _afxBlackWhite[] = _T("BlackWhite");

AFX_STATIC_DATA const TCHAR _afxPcNumber[]=_T("PcNumber");

AFX_STATIC_DATA const TCHAR _afxSunriseSunset[] = _T("SunriseSunset");
AFX_STATIC_DATA const TCHAR _afxLat[] = _T("sLalitude");
AFX_STATIC_DATA const TCHAR _afxLon[] = _T("sLongitude");
AFX_STATIC_DATA const TCHAR _afxOffsetOnOff[] = _T("OffsetOnOff");
AFX_STATIC_DATA const TCHAR _afxAutoOnOff[] = _T("AutoOnOff");

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersApp

BEGIN_MESSAGE_MAP(CTV_TracersApp, CWinApp)
	//{{AFX_MSG_MAP(CTV_TracersApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PAGESETUP, OnFilePagesetup)
	ON_COMMAND(ID_DEVICE_SETUP, OnDeviceSetup)
	ON_COMMAND(ID_SELECT_OPERATOR, OnSelectOperator)
	ON_COMMAND(ID_SELECT_PC, OnSelectPc)
	ON_UPDATE_COMMAND_UI(ID_SELECT_PC, OnUpdateSelectPc)
	ON_COMMAND(ID_COBFIG_OPERATOR, OnCobfigOperator)
	ON_UPDATE_COMMAND_UI(ID_COBFIG_OPERATOR, OnUpdateCobfigOperator)
	ON_COMMAND(ID_CONFIG_DB, OnConfigDb)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_DB, OnUpdateConfigDb)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_SETUP, OnUpdateDeviceSetup)
	ON_COMMAND(ID_ACCOUNTS_SYSTEM, OnAccountsSystem)
	ON_COMMAND(ID_ACCOUNTS_CLEAR, OnAccountsClear)
	ON_UPDATE_COMMAND_UI(ID_ACCOUNTS_CLEAR, OnUpdateAccountsClear)
	ON_UPDATE_COMMAND_UI(ID_LAMPS_SETUP, OnUpdateLampsSetup)
	ON_COMMAND(ID_LAMPS_SETUP, OnLampsSetup)
	ON_COMMAND(ID_TEST_LQI, OnTestLqi)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	//!!!	 Промяна при преминаване към ProjectsVS2015	преместени са в PreTranslateMessage
	//ON_MESSAGE(MW_COM_NOT_AVAILABE,OnPortNotAvailabe)
	//ON_MESSAGE(MW_COM_IN_USE,OnPortNotFree)
	//ON_MESSAGE(MW_COM_UNK_ERR,OnPortUnkErr)
	//ON_MESSAGE(MW_TCP_BAD_CONNECT,BadSocketConnect)
	//ON_MESSAGE(MW_AUTO_UPDATE_TIMER,OnAutoUpdateTimer)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)

END_MESSAGE_MAP()

BOOL CTV_TracersApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message) {
	case MW_COM_NOT_AVAILABE:
		// Извикване на функцията за обработка на това съобщение
		OnPortNotAvailabe(pMsg->wParam, pMsg->lParam);
		// Кажете на MFC, че не е необходима повече обработка
		return TRUE;
		break;
	case MW_COM_IN_USE:
		OnPortNotFree(pMsg->wParam, pMsg->lParam);
		return TRUE;
		break;
	case MW_COM_UNK_ERR:
		OnPortUnkErr(pMsg->wParam, pMsg->lParam);
		return TRUE;
		break;
	case MW_TCP_BAD_CONNECT:
		BadSocketConnect(pMsg->wParam, pMsg->lParam);
		return TRUE;
		break;
	case MW_AUTO_UPDATE_TIMER:
		OnAutoUpdateTimer(pMsg->wParam, pMsg->lParam);
		return TRUE;
		break;
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersApp construction

CTV_TracersApp::CTV_TracersApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bEnableMovingTemp = m_bEnableMoving = false;

	Id_PC = 0;
	

	pFrameExtendedView = NULL;
	ActiveTracker = NULL;
	fl_NeedSetup = false;
	x=y=0;

	m_pOperators= new COperatorManager;
	privilege = 0;
	Id_Operator = 0;
	m_cNameOperator = _T("");
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTV_TracersApp object

CTV_TracersApp theApp;

static CMutex No_New_TV_Tarcers(false,_T("0488905334"));

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersApp initialization

BOOL CTV_TracersApp::InitInstance()
{
	if(::WaitForSingleObject(No_New_TV_Tarcers.m_hObject,0)!=WAIT_OBJECT_0){
	  AfxMessageBox(IDS_ANOTHER_TRAKERS);
		return false ;
	}

	CString strHelpFile = m_pszHelpFilePath;
    strHelpFile.Replace(".HLP", ".chm");
    free((void*)(m_pszHelpFilePath));
    m_pszHelpFilePath = _tcsdup(strHelpFile);

	m_cTitle.LoadString(AFX_IDS_APP_TITLE);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDS_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(m_cTitle);

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	::OpenConnection(m_cTitle);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	BOOL fl_testlink=false;

	hTestView=::AfxLoadLibrary(_T("TestLink.dll"));
	if(hTestView){
		typedef BOOL (*pfnTestView)();
		
		pfnTestView pfn=NULL;
		pfn=(pfnTestView)GetProcAddress(hTestView,_T("SetViews"));
		if(pfn){
			fl_testlink=pfn();
		}
	}	
	if(!fl_testlink){
		pDocTemplate = new CMultiDocTemplate(		//ALWAYS MUST BE FIRST !!!
			IDR_MAINFRAME,
			RUNTIME_CLASS(CDocument),
			RUNTIME_CLASS(CMDIChildWnd),			// standard MDI child frame			
			RUNTIME_CLASS(CScrollView));
		AddDocTemplate(pDocTemplate);
	}
	pDocTemplate = new CMultiDocTemplate(
		IDR_TV_TRAC_DATA,
		RUNTIME_CLASS(CTV_TracersDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTracerViewSmal));//CTV_TracersView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(		//ALWAYS  dasdadefsrtdhaserth MUST 
		IDR_TV_TRAC_ACOUNT,
		RUNTIME_CLASS(CDataViewDoc),
		RUNTIME_CLASS(CMDIChildWnd),			// standard MDI child frame					
		RUNTIME_CLASS(CDataViewView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(		//ALWAYS  dasdadefsrtdhaserth MUST 
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataViewDoc),
		RUNTIME_CLASS(CFrameWnd),					// standard SDI child frame			
		RUNTIME_CLASS(CDataViewView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_TV_TRAC_DATA,
		RUNTIME_CLASS(CTV_TracersDoc),
		RUNTIME_CLASS(CMDIChildWnd),//CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTV_TracersView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

#ifdef SAPSAN
		key
#endif

	LOGFONT lf;                       
		memset(&lf, 0, sizeof(LOGFONT)); 
		lf.lfWeight=FW_BOLD;
		lf.lfHeight = 18; 
		lf.lfCharSet=DEFAULT_CHARSET;
		lf.lfOutPrecision =OUT_CHARACTER_PRECIS;
		lf.lfClipPrecision=CLIP_CHARACTER_PRECIS;
		lf.lfQuality=DEFAULT_QUALITY;
		lf.lfPitchAndFamily=DEFAULT_PITCH | FF_DONTCARE;
		strcpy(lf.lfFaceName, "MS Sans Serif");
		CTracerViewSmal::m_font.CreateFontIndirect(&lf); 

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(false);//true);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);//m_nCmdShow);
	pMainFrame->UpdateWindow();


	if(!m_pOperators->OperatorSelect(true))
		return false;

	BeginWaitCursor();

	{
		
		//make TestLinkView
		CDocument *p_TLDoc=NULL;
		
		POSITION pos;
		CDocTemplate *m_pDocTemplate;
		//		CString name = p_Hardw->GetName();
		
		if( hTestView && (p_TLView == NULL)){//&& (p_Hardw->m_Flags&FL_TEST_LINK_VIEW)){				
			pos=m_pDocManager->GetFirstDocTemplatePosition();
			for(int h = 0 ; h < TEST_LINK_VIEW_POS; h++ )
				m_pDocTemplate=m_pDocManager->GetNextDocTemplate(pos);
			p_TLDoc=m_pDocTemplate->OpenDocumentFile(NULL,true);
			if(p_TLDoc!=NULL){
				p_TLDoc->SetTitle(_T("TestLink"));
				POSITION	pos1 = p_TLDoc->GetFirstViewPosition();
				p_TLView = p_TLDoc->GetNextView(pos1);
				ASSERT_VALID(p_TLView);
				ASSERT(::IsWindow(p_TLView->m_hWnd));
				SendMessage(p_TLView->m_hWnd,MW_MARGING_PTR,(WPARAM)&Margin,0);
			}
		}
	}

	p_STCP = new CServerTCP();

 	p_hardware = new CHardwareTCP(); 
	//Вече нямам нужда от долния ред gatway на vivacom със sim картата на Твърдица и връзка през wifi с gatway и по-висока скорост
	//p_hardware = new CHardwareCom(COM_NUMBER); 


	// Dispatch commands specified on the command line
/*	if (m_lpCmdLine[0] != _T('\0'))
		if (!ProcessShellCommand(cmdInfo))ц
			return false;*/
 
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

typedef BOOL (WINAPI *PFN_VerQueryValue)(const LPVOID, LPTSTR, LPVOID *, PUINT); 	
typedef BOOL (WINAPI *PFN_GetFileVersionInfo)(LPTSTR , DWORD ,DWORD , LPVOID); 	
typedef DWORD (WINAPI *PFN_GetFileVersionInfoSize)(LPTSTR,LPDWORD);
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd *pWnd=GetDlgItem(IDC_STATIC_VERSION_DB);
	CString text,text1;
	pWnd->GetWindowText(text);
	text1.Format(_T("%s%d.%.2d"),text,DATABASE_VERSION,DATABASE_SUB_VERSION);
	pWnd->SetWindowText(text1);

	char    szFullPath[256];	// full path of module
	DWORD   dwVerInfoSize;		// Size of version information block
	DWORD   dwVerHnd=0;			// An 'ignored' parameter, always '0'
	HINSTANCE	hDll=NULL;
	hDll=::AfxLoadLibrary(_T("version.dll"));
	PFN_VerQueryValue VerQueryValueA;
	PFN_GetFileVersionInfo GetFileVersionInfoA;
	PFN_GetFileVersionInfoSize GetFileVersionInfoSizeA;
	bool fl=false;
	if(hDll){
		if((VerQueryValueA=(PFN_VerQueryValue)GetProcAddress(hDll,_T("VerQueryValueA")))!=NULL){
			if((GetFileVersionInfoA=(PFN_GetFileVersionInfo)GetProcAddress(hDll,_T("GetFileVersionInfoA")))!=NULL){
				if((GetFileVersionInfoSizeA=(PFN_GetFileVersionInfoSize)GetProcAddress(hDll,_T("GetFileVersionInfoSizeA")))!=NULL){
					fl=true;
				}
			}
		}
	}
	if(fl){
		GetModuleFileName (AfxGetApp()->m_hInstance, szFullPath, sizeof(szFullPath));
		dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
		if (dwVerInfoSize) {
			LPVOID lpFixedFileInf;           // указатель на структуру VS_VERSIONINFO
			lpFixedFileInf = new char[dwVerInfoSize];
			if(GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpFixedFileInf)){
				VS_FIXEDFILEINFO *pFixedFileInfo; // указатель на структуру VS_FIXEDFILEINFO
				UINT uLen = 0;                   
				if(VerQueryValue((const LPVOID)lpFixedFileInf,"\\",(LPVOID *) (&pFixedFileInfo),&uLen)){
					pWnd=GetDlgItem(IDC_STATIC_VERSION);
					pWnd->GetWindowText(text);
					text1.Format(_T("%s%d.%.2d"),text,HIWORD (pFixedFileInfo->dwFileVersionMS),
						LOWORD(pFixedFileInfo->dwFileVersionMS));
					if(HIWORD(pFixedFileInfo->dwFileVersionLS)){
						text=text1;
						text1.Format(_T("%s.%.2d"),text,HIWORD(pFixedFileInfo->dwFileVersionLS));
					}
					pWnd->SetWindowText(text1);
				}
			}
			delete []lpFixedFileInf;
		}
	}
	if(hDll!=NULL){
		::AfxFreeLibrary(hDll);
		hDll=NULL;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CTV_TracersApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersApp message handlers


int CTV_TracersApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	if( p_hardware != NULL ){
		p_hardware->Close();
		delete p_hardware;
	}

	if( p_STCP != NULL ){
		delete p_STCP;
	}

	delete m_pOperators;
	if(hTestView!=NULL){
		::AfxFreeLibrary(hTestView);
		hTestView=NULL;
	}

	::CloseConnection();
	return CWinApp::ExitInstance();
}

void CTV_TracersApp::LoadStdProfileSettings(unsigned int cou_receptien_files)
{
	CWinApp::LoadStdProfileSettings(cou_receptien_files);

	Margin.top=GetProfileInt(_afxPageMargin,_afxTop,2500);
	Margin.left=GetProfileInt(_afxPageMargin,_afxLeft,1000);
	Margin.right=GetProfileInt(_afxPageMargin,_afxRight,1000);
	Margin.bottom=GetProfileInt(_afxPageMargin,_afxBottom,2500);
	m_bFitPage=GetProfileInt(_afxPageMargin,_afxFitPage,0);
	m_bPrintBlackWhite=GetProfileInt(_afxPageMargin,_afxBlackWhite,0);

	Id_PC=GetProfileInt(_afxPageMargin,_afxPcNumber,1);

	CString text;
	text = AfxGetApp()->GetProfileString(_afxSunriseSunset,_afxLat,_T("42.5"));
	sscanf(text,_T("%lf"),&m_dLat);
	text = AfxGetApp()->GetProfileString(_afxSunriseSunset,_afxLon,_T("25"));
	sscanf(text,_T("%lf"),&m_dLon);

	m_OffsetOnOff = AfxGetApp()->GetProfileInt(_afxSunriseSunset,_afxOffsetOnOff,25);
	m_bAutoOnOff = AfxGetApp()->GetProfileInt(_afxSunriseSunset,_afxAutoOnOff,0);
}

#include "PageSetup.h"
void CTV_TracersApp::OnFilePagesetup() 
{
	// TODO: Add your command handler code here
		CPageSetup PSD;
	
	PSD.m_uTop = Margin.top/100;
	PSD.m_uLeft = Margin.left/100;
	PSD.m_uRight = Margin.right/100;
	PSD.m_uBottom = Margin.bottom/100;
	PSD.m_bFitPage = m_bFitPage;
	PSD.m_bPrintBlackWhite=m_bPrintBlackWhite;

	if(PSD.DoModal()==IDOK){
		Margin.top=PSD.m_uTop*100;
		Margin.left=PSD.m_uLeft*100;
		Margin.right=PSD.m_uRight*100;
		Margin.bottom=PSD.m_uBottom*100;
		m_bFitPage=PSD.m_bFitPage;
		m_bPrintBlackWhite=PSD.m_bPrintBlackWhite;
		WriteProfileInt(_afxPageMargin,_afxTop,Margin.top);
		WriteProfileInt(_afxPageMargin,_afxLeft,Margin.left);
		WriteProfileInt(_afxPageMargin,_afxRight,Margin.right);
		WriteProfileInt(_afxPageMargin,_afxBottom,Margin.bottom);
		WriteProfileInt(_afxPageMargin,_afxFitPage,m_bFitPage);
		WriteProfileInt(_afxPageMargin,_afxBlackWhite,m_bPrintBlackWhite);
	}
}

BOOL CTV_TracersApp::SaveAllModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(CWinApp::SaveAllModified()){
		return true;
	}
	return false;
}

LRESULT CTV_TracersApp::OnPortNotAvailabe(WPARAM wParam, LPARAM lParam)
{
	CString text;
	text.Format(IDS_ERR_HARWARE_NOT_PRESENT,wParam);
	AfxMessageBox(text);
	return 0;
}

LRESULT CTV_TracersApp::OnPortNotFree(WPARAM wParam, LPARAM lParam)
{
	CString text;
	text.Format(IDS_ERR_HARWARE_IS_BUSY,wParam);
	AfxMessageBox(text);
	return 0;
}

LRESULT CTV_TracersApp::OnPortUnkErr(WPARAM wParam, LPARAM lParam)
{
	CString text;
	text.Format(IDS_ERR_HARWARE_UNKNOWN,wParam);
	AfxMessageBox(text);
	return 0;
}

LRESULT CTV_TracersApp::BadSocketConnect(WPARAM wParam, LPARAM lParam)
{
	CString text;
	in_addr addres;
	addres.S_un.S_addr = wParam;
	text.Format(IDS_SOCKET_ERR,inet_ntoa(addres));
	//AfxMessageBox(text);
	return 0;
}

BOOL CTV_TracersApp::SetHardwareCommand(WORD IdHardware, int ModbusAddrres, COMMANDS * Command)
{
	DWORD threadId;
	if((threadId=GetHardwareThreadId(IdHardware))==0)
		return false;
	::PostThreadMessage(threadId,MW_SET_COMMAND,ModbusAddrres,(LPARAM)Command);
	return true;
}

DWORD CTV_TracersApp::GetHardwareThreadId(WORD IdHardware)
{
	DWORD threadId=0;
	
	return threadId;
}

#include "DeviceSetup.h"

void CTV_TracersApp::OnDeviceSetup() 
{
	// TODO: Add your command handler code here
	{//if(CWinApp::SaveAllModified()){
		CDeviceSetup DS;
		DS.privilege = privilege;
		DS.DoModal();
		fl_NeedSetup = true;
	}
}

void CTV_TracersApp::GridPrint(CGridCtrl *pGrid, CDialog *pCDialog)
{
	pCDialog->ShowWindow(SW_HIDE);
	CFrameWnd* pOldFrame=(CFrameWnd*)AfxGetThread()->m_pMainWnd;
	pOldFrame->ShowWindow(SW_HIDE); //added by eric

	CDataViewDoc * pDVDoc=GetNewDataViewDocToDialog();
	CString strTitle;
	pCDialog->GetWindowText(strTitle);
	pDVDoc->SetTitle(strTitle);
	POSITION	pos = pDVDoc->GetFirstViewPosition();
	CDataViewView * pView = (CDataViewView*)pDVDoc->GetNextView(pos);
	ASSERT_VALID(pView);
	pView->m_pOldFrame=pOldFrame;
	pView->m_pCDialog=pCDialog;
	pDVDoc->AddGrid(pGrid);
	CFrameWnd * pFrameWnd=pView->GetParentFrame();
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	AfxGetApp()->m_pMainWnd=pFrameWnd;
	pView->OnFilePrint();
}

CDataViewDoc * CTV_TracersApp::GetNewDataViewDocToDialog()
{
	POSITION pos=m_pDocManager->GetFirstDocTemplatePosition();
	CDocTemplate *m_pDocTemplate;
	for(int h = 0; h < DIALOG_VIEW_POS; h ++)
		m_pDocTemplate=m_pDocManager->GetNextDocTemplate(pos);
	CDataViewDoc * pDVDoc=(CDataViewDoc*)m_pDocTemplate->OpenDocumentFile(NULL);
	ASSERT(pDVDoc!=NULL);
	return pDVDoc;
}

void CTV_TracersApp::GridPrintPreviev(CGridCtrl *pGrid, CDialog *pCDialog)
{
	pCDialog->ShowWindow(SW_HIDE);
	CFrameWnd* pOldFrame=(CFrameWnd*)AfxGetThread()->m_pMainWnd;
	pOldFrame->ShowWindow(SW_HIDE); //added by eric

	CDataViewDoc * pDVDoc=GetNewDataViewDocToDialog();
	CString strTitle;
	pCDialog->GetWindowText(strTitle);
	pDVDoc->SetTitle(strTitle);
	POSITION	pos = pDVDoc->GetFirstViewPosition();
	CDataViewView * pView = (CDataViewView*)pDVDoc->GetNextView(pos);
	ASSERT_VALID(pView);
	pView->m_pOldFrame=pOldFrame;
	pView->m_pCDialog=pCDialog;
	pDVDoc->AddGrid(pGrid);
	CFrameWnd * pFrameWnd=pView->GetParentFrame();
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	AfxGetApp()->m_pMainWnd=pFrameWnd;
	pView->OnFilePrintPreview();
}

void CTV_TracersApp::ExtendedView(CDocument *pDoc)
{
	POSITION pos=m_pDocManager->GetFirstDocTemplatePosition();
	CDocTemplate *m_pDocTemplate;
	for(int h = 0; h < EXTENDED_VIEW_POS; h ++)
		m_pDocTemplate=m_pDocManager->GetNextDocTemplate(pos);

	if(pFrameExtendedView){
		CTV_TracersView * pTV = (CTV_TracersView*)pFrameExtendedView->GetActiveView();
		pTV->StoreOldPosition();
		::SendMessage(pFrameExtendedView->m_hWnd,WM_CLOSE,0,0);
	}

	pFrameExtendedView = m_pDocTemplate->CreateNewFrame(pDoc,NULL);
	m_pDocTemplate->InitialUpdateFrame(pFrameExtendedView, pDoc); 
	
}

void CTV_TracersApp::SetExtendetView(WORD IdHardware, int ModbusAddrres,BOOL fl_Extend)
{
	DWORD threadId;
	if((threadId=GetHardwareThreadId(IdHardware))==0)
		return ;
	::PostThreadMessage(threadId,MW_EXTENDET_VIEW,ModbusAddrres,fl_Extend);
	return ;
}


BOOL CTV_TracersApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL idle = CWinApp::OnIdle(lCount);

	return true;
}

void CTV_TracersApp::OnSelectOperator() 
{
	// TODO: Add your command handler code here
	m_pOperators->OperatorSelect();
}

void CTV_TracersApp::OnUpdateSelectPc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

void CTV_TracersApp::OnUpdateCobfigOperator(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

void CTV_TracersApp::OnUpdateConfigDb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

void CTV_TracersApp::OnUpdateDeviceSetup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

#include "PCNumber.h"
void CTV_TracersApp::OnSelectPc() 
{
	// TODO: Add your command handler code here
	CPCNumber CCN;
	UINT old_Id_PC=CCN.m_iNumber=Id_PC;
	if(CCN.DoModal()==IDOK){
		Id_PC=CCN.m_iNumber;
		WriteProfileInt(_afxPageMargin,_afxPcNumber,Id_PC);
		if(old_Id_PC!=Id_PC){
			::StoreDataConfigMessage(Id_PC,Id_Operator,_CONFIG_PC_NUMBER);
		}
	}
}

void CTV_TracersApp::OnConfigDb() 
{
	// TODO: Add your command handler code here
	{//if(CWinApp::SaveAllModified()){
		::ConfigConnectio();
		::StoreDataConfigMessage(Id_PC,Id_Operator,_CONFIG_CONNECTION_DB);
	}
	
}

#include "OperatorSetup.h"

void CTV_TracersApp::OnCobfigOperator() 
{
	// TODO: Add your command handler code here
	COperatorSetup COS;
	COS.DoModal();
	m_pOperators->SetOperatorPrivilegies(Id_Operator);
}

void CTV_TracersApp::SetNewDecory()
{
	;
}

void CTV_TracersApp::OnUpdateLampsSetup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

#include "LampPS.h"

void CTV_TracersApp::OnLampsSetup() 
{
	// TODO: Add your command handler code here
	{//if(CWinApp::SaveAllModified()){
		CLampPS LPS;
	//	LPS.privilege = privilege;
		LPS.DoModal();
	//	fl_NeedSetup = true;
	}
}

#include "AutoOnOff.h"

LRESULT CTV_TracersApp::OnAutoUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	CAutoOnOff CAOO;

	CAOO.DoModal();
	return 0;
}