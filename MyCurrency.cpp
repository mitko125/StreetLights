// MyCurrency.cpp: implementation of the CMyCurrency class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sapsan.h"
#include "MyCurrency.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCurrency::CMyCurrency()
{
	data=0;
}

CMyCurrency& CMyCurrency::operator=(const CMyCurrency& value)
{
	data=value.data;
	return *this;
}

CMyCurrency& CMyCurrency::operator=(const __int64 value)
{
	data=value;
	return *this;
}

CMyCurrency::CMyCurrency(const CMyCurrency& value)
{
	*this=value;
}

CMyCurrency::CMyCurrency(const __int64 value)
{
	data=value;
}

double CMyCurrency::ToDoubleLegal(void) const
{
	return ((double)data)/(double)10000;
}

CMyCurrency CMyCurrency::FromDoubleLegal(double value)
{
	_variant_t d;
	d.vt = VT_R8;
	d=value;
	COleCurrency cy=d;
	data=cy.m_cur.int64;
	return *this;
}

bool CMyCurrency::operator ==(const CMyCurrency &value) const
{
	return data==value.data;
}

bool CMyCurrency::operator !=(const CMyCurrency &value) const
{
	return data!=value.data;
}

bool CMyCurrency::operator <=(const CMyCurrency &value) const
{
	return data<=value.data;
}

bool CMyCurrency::operator >=(const CMyCurrency &value) const
{
	return data>=value.data;
}

bool CMyCurrency::operator <(const CMyCurrency &value) const
{
	return data<value.data;
}

bool CMyCurrency::operator >(const CMyCurrency &value) const
{
	return data>value.data;
}

CMyCurrency operator-(const CMyCurrency& value)
{
	CMyCurrency ret(value);
	ret.data=-ret.data;
	return ret;
}

CMyCurrency abs(const CMyCurrency& value)
{
	CMyCurrency ret(value);
	if(ret.data<0)
		ret.data=-ret.data;
	return ret;
}

CMyCurrency CMyCurrency::operator+(const CMyCurrency& value)
{
	CMyCurrency ret(value);
	ret.data+=data;
	return ret;
}

CMyCurrency& CMyCurrency::operator+=(const CMyCurrency& value)
{
	data+=value.data;
	return *this;
}

CMyCurrency CMyCurrency::operator-(const CMyCurrency& value)
{
	CMyCurrency ret(*this);
	ret.data-=value.data;
	return ret;
}

CMyCurrency& CMyCurrency::operator-=(const CMyCurrency& value)
{
	data-=value.data;
	return *this;
}

CMyCurrency CMyCurrency::operator*(const CMyCurrency& value)
{
	CMyCurrency ret(value);
	ret.data*=data;
	ret.data/=10000;
	return ret;
}

CMyCurrency& CMyCurrency::operator*=(const CMyCurrency& value)
{
	data*=value.data;
	data/=10000;
	return *this;
}
 
CMyCurrency CMyCurrency::operator/(const CMyCurrency& value)
{
	CMyCurrency ret(*this);
	ret.data*=10000;
	ret.data/=value.data;
	return ret;
}

CMyCurrency& CMyCurrency::operator/=(const CMyCurrency& value)
{
	data*=10000;
	data/=value.data;
	return *this;
}

void CMyCurrency::FromString(CString num)
{
	BOOL fl_negativ=false;
	int punkt=0;
	data=0;
	int c;
	for(int i=0;i<num.GetLength();i++){
		c=num.GetAt(i);
		if((i==0)&&(c=='-')){
			fl_negativ=true;
		}else	if((i==0)&&(c=='+')){
			;
		}else if(c=='.'){
			if(punkt)
				break;
			else
				punkt=1;
		}else if((c>='0')&&(c<='9')){
			c-='0';
			switch(punkt){
				case 0:
					data*=10;
					data+=(c*10000);
					break;
				case 1:
					data+=(c*1000);
					break;
				case 2:
					data+=(c*100);
					break;
				case 3:
					data+=(c*10);
					break;
				case 4:
					data+=(c);
					break;
			}
			if(punkt)
				if(++punkt==5)
					break;
		}else
			break;
	}
	if(fl_negativ)
		data=-data;
}

int DoubleToInt(double data)
{
	if(data<0)
		data-=0.5;
	else
		data+=0.5;
	return (int)data;
}

CString FormatPunktI1(INT64 data, BYTE punkt)
{
	CString text,text1;
	double d=((double)data)/10000;
	text.Format(_T("%%.%dlf"),punkt);
	text1.Format(text,d);
	return text1;
}

CString FormatPunkt1(double data, BYTE punkt)
{
	CString text,text1;

	text.Format(_T("%%.%dlf"),punkt);
	text1.Format(text,data);
	return text1;
}