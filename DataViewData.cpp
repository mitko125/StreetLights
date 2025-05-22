// DataViewData.cpp: implementation of the CDataViewData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sapsan.h"
#include "DataViewData.h"

#include "../../Grid1_0/GridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CDataViewData, CObject,VARIANT_DATA_ACOCOUTS|VERSIONABLE_SCHEMA)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataViewData::CDataViewData()
{
	pTL=NULL;
	pGrid=NULL;   
	DlgId=0;
	NewPage=false;
}

CDataViewData::~CDataViewData()
{

}

void CDataViewData::Serialize(CArchive &ar)
{
	CObject::Serialize( ar );
	BOOL fl_grid=false;
	
  // now do the stuff for our specific class
  if( ar.IsStoring() ){
		ar<<pTL;
		if(pGrid)
			fl_grid=true;
		ar<<fl_grid;
		if(fl_grid)
			pGrid->Serialize(ar);
		ar<<NewPage<<DlgId;
	}else{
		ar>>pTL;
		ar>>fl_grid;
		if(fl_grid){
			pGrid=new CGridCtrl;
			pGrid->Serialize(ar);
		}
		ar>>NewPage>>DlgId;
	}
}
