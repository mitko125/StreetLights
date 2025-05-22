// TV_TracersDoc.cpp : implementation of the CTV_TracersDoc class
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "TV_TracersDoc.h"

#include "TracerViewBase.h"
#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersDoc
CString CTV_TracersDoc::m_strPathNameTracer=_T("");

IMPLEMENT_DYNCREATE(CTV_TracersDoc, CDocument)

BEGIN_MESSAGE_MAP(CTV_TracersDoc, CDocument)
	//{{AFX_MSG_MAP(CTV_TracersDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersDoc construction/destruction

CTV_TracersDoc::CTV_TracersDoc()
{
	// TODO: add one-time construction code here
	IdHardware = 0;
	m_iModbusAddres = 0;
	::memset(&FullStatus,0,sizeof(FULL_STATUS));
	RemoteCommands = 0;
	program_ok=program_err=false;
	help1 = _T("");
	help2 = _T("");
	m_sOffset = _T("");
	m_sMin = _T("");
	m_sMax = _T("");
	m_sPosition = _T("");
	m_bToReperOrPos = false;
	m_bToDown = false;
	m_bToUp = false;
	dwStartToDwonOrUp = 0;
	m_bConnected = false;
	m_bGoCommand = false;
	OldControlerStatus = 0;
	colorBk=RGB(255,255,255);
	colorTx=RGB(0,0,0);
	m_nStatus=m_nOldStatus=_UNDEFINET;
	StoredPosition = 0;
	m_sStoredPosition = _T("");
	m_bInGroup = false;
}

CTV_TracersDoc::~CTV_TracersDoc()
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	if(pApp->ActiveTracker == this )
		pApp->ActiveTracker = NULL;
}

BOOL CTV_TracersDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTV_TracersDoc serialization

void CTV_TracersDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersDoc diagnostics

#ifdef _DEBUG
void CTV_TracersDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTV_TracersDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTV_TracersDoc commands


void CTV_TracersDoc::NewFullStatus(FULL_STATUS *pFS, int cou_data)
{
	::memcpy(&FullStatus,pFS,sizeof(FULL_STATUS));

	delete pFS;
	
	m_bConnected = true;

	if(m_bToReperOrPos){
		if(FullStatus.ControlerStatus & BIT_ACK){
			m_bToReperOrPos = false;
		}
	}
	if(m_bToDown){
		if((FullStatus.ControlerStatus & ( BIT_LIMIT_DOWN | BIT_ERR_VAGE) ) ){
			m_bToDown = false;
		}
	}
	if(m_bToUp){
		if((FullStatus.ControlerStatus & ( BIT_LIMIT_UP | BIT_OVERLOAD) ) ){
			m_bToUp = false;
		}
	}


	OldControlerStatus = FullStatus.ControlerStatus;

	m_sPosition = FormatPunktI1(FullStatus.Position.ToInt64(),2);

	/*if((FullStatus.ControlerStatus & BIT_AC_READY)==0){
		SetStatus(_AC_ERR);
	}else */if(FullStatus.ControlerStatus & BIT_LIMIT_ERR){
		SetStatus(_LIMIT_ERR);
	}else if(FullStatus.ControlerStatus & BIT_Q10){
		SetStatus(_Q10);
	}else /*if(FullStatus.ControlerStatus & BIT_ERR_REMAK){
		SetStatus(_ERR_REMAK);
	}else */if(FullStatus.ControlerStatus & BIT_LIMIT_UP){
		SetStatus(_LIMIT_UP);
	}else if(FullStatus.ControlerStatus & BIT_OVERLOAD){
		SetStatus(_OVERLOAD);
	}else if(FullStatus.ControlerStatus & BIT_LIMIT_DOWN){
		SetStatus(_LIMIT_DOWN);
	}else if(FullStatus.ControlerStatus & BIT_LIMIT_SOFT){
		SetStatus(_LIMIT_SOFT);
	}else if(FullStatus.ControlerStatus & BIT_LIMIT_LEFT){
		SetStatus(_LIMIT_LEFT);
	}else if(FullStatus.ControlerStatus & BIT_LIMIT_RIGHT){
		SetStatus(_LIMIT_RIGHT);
	}else if(FullStatus.ControlerStatus & BIT_MOVE_LEFT){
		SetStatus(_MOVE_LEFT);
	}else if(FullStatus.ControlerStatus & BIT_MOVE_RIGHT){
		SetStatus(_MOVE_RIGHT);
	}else if(FullStatus.ControlerStatus & BIT_ERR_VAGE){
		SetStatus(_ERR_VAGE);
	}else if(FullStatus.ControlerStatus & BIT_ACK){
		SetStatus(_NONE);
		if( RemoteCommands & (BIT_REMOTE_REPER | BIT_REMOTE_POSITION) )
			RemoteCommands = 0;
	}else if(FullStatus.ControlerStatus & BIT_MOVE_UP){
		;
	}else if(FullStatus.ControlerStatus & BIT_MOVE_DOWN){
		;
	}else{
		if( RemoteCommands == 0)
			SetStatus(_NONE);
	}
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->NewFullStatus(cou_data);
	}
}

void CTV_TracersDoc::HardwareStoped(BOOL CloseWindows)
{
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->HardwareStoped(CloseWindows);
	}
}

void CTV_TracersDoc::ErrConnect()
{
	m_bConnected = false;
	SetStatus(_NO_CONNEC);
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->ErrConnect();
	}
}

BOOL CTV_TracersDoc::SetHardwareCommand(COMMANDS *Command)
{
	switch(RemoteCommands = Command->RemoteCommands){
	case BIT_REMOTE_MOVE_UP:
		SetStatus(_MOVE_UP);
		break;
	case BIT_REMOTE_MOVE_DOWN:
		SetStatus(_MOVE_DOWN);
		break;
	case BIT_REMOTE_MOVE_UP | BIT_REMOTE_MOVE_FAST:
		SetStatus(_MOVE_UP_FAST);
		break;
	case BIT_REMOTE_MOVE_DOWN | BIT_REMOTE_MOVE_FAST:
		SetStatus(_MOVE_DOWN_FAST);
		break;
	case BIT_REMOTE_POSITION:
		SetStatus(_GO_POS);
		break;
	case BIT_REMOTE_REPER:
		SetStatus(_GO_REPER);
		break;
	}

	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	return pApp->SetHardwareCommand(IdHardware,m_iModbusAddres,Command);
}

void CTV_TracersDoc::SetExtendetView(BOOL fl_Extend)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	pApp->SetExtendetView(IdHardware,m_iModbusAddres,fl_Extend);
}

void CTV_TracersDoc::EndProgram(BOOL fl_ok)
{
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->EndProgram(fl_ok);
	}
	if(fl_ok){
		program_ok=true;
		program_err=false;
	}else{
		program_ok=false;
		program_err=true;
	}
}

void CTV_TracersDoc::KillFocus()
{
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->KillFocus();
	}
}

void CTV_TracersDoc::InitSetings()
{
	m_sOffset = FormatPunktI1(Setings.Offset.ToInt64(),2);
	m_sMin = FormatPunktI1(Setings.Min.ToInt64(),2);
	m_sMax = FormatPunktI1(Setings.Max.ToInt64(),2);
}

void CTV_TracersDoc::GoToPos(CMyCurrency new_pos)
{
	COMMANDS * pC = new COMMANDS;
				
	new_pos -= Setings.Offset;

	pC->TargetPos = -(int)(new_pos.ToInt64()/10);
	pC->RemoteCommands = BIT_REMOTE_POSITION;
	SetHardwareCommand(pC);
}

CString CTV_TracersDoc::GetName()
{
	return GetTitle();
}

void CTV_TracersDoc::StoreStatus(device_messages Satus)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	_CommandPtr m_pCmd=NULL;
	try{
		CString com;
		com.Format(_T("INSERT INTO accounts_device(id_device,id_pc,id_operator,id_message,top_m,date_time) VALUES (%u,%u,%u,%u,'%s','%s')"),
			id_device,pApp->Id_PC,pApp->Id_Operator,Satus,::FormatPunktI1(FullStatus.Position.ToInt64(),2),::TimeToSQL(COleDateTime::GetCurrentTime()));
		TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
		m_pCmd->ActiveConnection=GetConnection();
		m_pCmd->CommandText=(LPCTSTR)com;
		m_pCmd->Execute(NULL, NULL, adCmdText);
	}
	catch(_com_error &e){
	  GenerateSqlError(e);
	}
}

void CTV_TracersDoc::SetStatus(device_messages Satus)
{
  if( m_nOldStatus != (m_nStatus = Satus) ){
		switch(m_nOldStatus){
		case _UNDEFINET:
			break;
		default:
			StoreStatus(m_nStatus);
			break;
		}
		m_nOldStatus = m_nStatus;
	}
}

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

void CTV_TracersDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here

	CDocTemplate* pTemplate = GetDocTemplate();

	DWORD lFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CString fileName;


	CFileDialog dlgFile(true);

	CString title;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	if( dlgFile.DoModal() == IDOK){
		
		fileName.ReleaseBuffer();
		OpenDocument(fileName);
		
	}
}

BOOL CTV_TracersDoc::DoFileSave()
{
	DWORD dwAttrib = GetFileAttributes(m_strPathNameTracer);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
		{
			TRACE0("Warning: File save with new name failed.\n");
			return FALSE;
		}
	}
	else
	{
		if (!DoSave(m_strPathNameTracer))
		{
			TRACE0("Warning: File save failed.\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CTV_TracersDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	// TODO: Add your command handler code here
	CDocTemplate* pTemplate = GetDocTemplate();

	BOOL m_bToScene = false;
	POSITION pos1=pTemplate->GetFirstDocPosition();
	while(pos1){
		CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
		ASSERT(pDoc!=NULL);
		if(pDoc->m_bInGroup){
			m_bToScene = true;
			break;
		}
	}

	if(!m_bToScene){
		AfxMessageBox(IDS_NO_SCENE);
		return false;
	}

	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathNameTracer;
		if (bReplace && newName.IsEmpty())
		{
			newName = _T("");
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;
	{
		CStdioFile aFile;

		CFileException fx;
		if( !aFile.Open( newName,CFile::modeCreate|CFile::modeWrite|CFile::typeText, &fx ) )
		{
			TCHAR buf[ 255 ];
			fx.GetErrorMessage( buf, 255 );
			CString strPrompt( buf );
			AfxMessageBox( strPrompt );
			return false;
		}
		pos1=pTemplate->GetFirstDocPosition();
		CString str;
		while(pos1){
			CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
			ASSERT(pDoc!=NULL);
			if(pDoc->m_bInGroup){				
				str.Format(_T("%s=%s\n"),pDoc->GetName(),pDoc->m_sPosition);
				aFile.WriteString(str);
				pDoc->StoredPosition = pDoc->FullStatus.Position;
				pDoc->m_sStoredPosition = FormatPunktI1(pDoc->StoredPosition.ToInt64(),2);
			}
			pDoc->SetModifiedFlag(false);
		}
		aFile.WriteString(str);
		aFile.Close();
	}

	// reset the title and change the document name
	if (bReplace){
		m_strPathNameTracer = newName;
		AfxGetApp()->AddToRecentFileList(m_strPathNameTracer);
	}

	return TRUE;        // success
}

BOOL CTV_TracersDoc::OnOpenRecentFile(UINT nID)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	//ASSERT_VALID(this);
	ASSERT(pApp->m_pRecentFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)pApp->m_pRecentFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*pApp->m_pRecentFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*pApp->m_pRecentFileList)[nIndex]);

	

	CDocTemplate* pTemplate = GetDocTemplate();
	CString strExt;
	if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
	  !strExt.IsEmpty())
		{
				ASSERT(strExt[0] == '.');
				strExt;
		}
	CString name = (*pApp->m_pRecentFileList)[nIndex];
	int size = name.GetLength();
	int sizeExt = strExt.GetLength();
	if( size && sizeExt && (size > sizeExt) ){
		if(name.Find(strExt,size-sizeExt)>0){
			if(!OpenDocument((*pApp->m_pRecentFileList)[nIndex]))
				pApp->m_pRecentFileList->Remove(nIndex);
			return true;
		}
	}

	return pApp->OnOpenRecentFile(nID);
}

BOOL CTV_TracersDoc::OpenDocument(CString name)
{
	if(!SaveModified())
		return true;

		CStdioFile aFile;
		CDocTemplate* pTemplate = GetDocTemplate();
		
		CFileException fx;
		if( !aFile.Open( name,CFile::modeRead|CFile::typeText, &fx ) )
		{
			TCHAR buf[ 255 ];
			fx.GetErrorMessage( buf, 255 );
			CString strPrompt( buf );
			AfxMessageBox( strPrompt );
			return false;
		}
		POSITION pos1=pTemplate->GetFirstDocPosition();
		while(pos1){
			CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
			ASSERT(pDoc!=NULL);
			pDoc->OutGroup();
			pDoc->SetModifiedFlag(false);
		}
		CString str=_T(" ");
		do{
			aFile.ReadString(str);
			if(!str.IsEmpty()){
				CString name;
				int i;
				if(i=str.Find(_T('='))){
					name=str.Left(i);
					if(!name.IsEmpty()){
						{
							pos1=pTemplate->GetFirstDocPosition();
							BOOL fl_ok = false;
							while(pos1){
								CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
								ASSERT(pDoc!=NULL);
								if(pDoc->GetName() == name){
									pDoc->InGroup();
									pDoc->SetModifiedFlag(false);
									name = str.Mid(i+1);
									pDoc->StoredPosition.FromString(name);
									pDoc->m_sStoredPosition = FormatPunktI1(pDoc->StoredPosition.ToInt64(),2);
									fl_ok = true;
									break;
								}
							}
							if( !fl_ok ){
								str.Format(IDS_NO_DEVICE,name);
								AfxMessageBox(str);
							}
						}
					}
				}
			}
		}while(!str.IsEmpty());
		aFile.Close();
		
		AfxGetApp()->AddToRecentFileList(m_strPathNameTracer=name);

		return true;
}

void CTV_TracersDoc::InGroup()
{
	SetModifiedFlag(true);
	m_bInGroup = true;
	StoredPosition = FullStatus.Position;
	m_sStoredPosition = FormatPunktI1(StoredPosition.ToInt64(),2);

	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->InGroup();
	}
}

void CTV_TracersDoc::OutGroup()
{
	SetModifiedFlag(true);
	m_bInGroup = false;
	StoredPosition = 0;
	m_sStoredPosition=_T("");
	POSITION pos=GetFirstViewPosition();
	while(pos){
		CTracerViewBase *pTV=(CTracerViewBase*)GetNextView(pos);
		ASSERT(pTV->IsKindOf(RUNTIME_CLASS(CTracerViewBase)));
		pTV->OutGroup();
	}
}

BOOL CTV_TracersDoc::SaveModified()
{
	CDocTemplate* pTemplate = GetDocTemplate();
	POSITION pos1;
	pos1=pTemplate->GetFirstDocPosition();
	while(pos1){
		CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
		ASSERT(pDoc!=NULL);
		if(pDoc->IsModified())
			goto modifed;
		if(pDoc->m_bInGroup){
			CMyCurrency raz = pDoc->StoredPosition - pDoc->FullStatus.Position;
			if(raz.ToInt64()>500)
				goto modifed;
			else if(raz.ToInt64()<-500)
				goto modifed;
		}
	}

	return TRUE;        // ok to continue

modifed:
	// get name/title of document
	CString name;
	if (m_strPathNameTracer.IsEmpty())
	{
		// get name based on caption
		//name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathNameTracer;
	//	if (afxData.bMarked4)
		{
	//		AfxGetFileTitle(m_strPathNameTracer, name.GetBuffer(_MAX_PATH), _MAX_PATH);
			name.ReleaseBuffer();
		}
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (!DoFileSave())
			return FALSE;       // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		{
			pos1=pTemplate->GetFirstDocPosition();
			while(pos1){
				CTV_TracersDoc * pDoc=(CTV_TracersDoc *)pTemplate->GetNextDoc(pos1);
				ASSERT(pDoc!=NULL);
				pDoc->SetModifiedFlag(false);
				if(pDoc->m_bInGroup){
					pDoc->StoredPosition = pDoc->FullStatus.Position;
				}
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}
