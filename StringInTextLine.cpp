// StringInTextLine.cpp: implementation of the CStringInTextLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sapsan.h"
#include "StringInTextLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CStringInTextLine, CObject,VARIANT_DATA_ACOCOUTS|VERSIONABLE_SCHEMA)

CStringInTextLine::CStringInTextLine()
{
	text=_T("");
	width=0;
	align=0;
	line=0;
}

CStringInTextLine::~CStringInTextLine()
{

}

void CStringInTextLine::Serialize(CArchive &ar)
{
	CObject::Serialize( ar );
  // now do the stuff for our specific class
  if( ar.IsStoring() ){
		ar<<text<<width<<align<<line;
	}else{
		ar>>text>>width>>align>>line;
	}
}
