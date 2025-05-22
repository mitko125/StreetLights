// MyCurrency.h: interface for the CMyCurrency class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCURRENCY_H__C663A1F3_D0FF_4414_AC7A_E2A0FA517A6C__INCLUDED_)
#define AFX_MYCURRENCY_H__C663A1F3_D0FF_4414_AC7A_E2A0FA517A6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyCurrency  
{
public:
//constructors / !!! NB not destructors
	CMyCurrency();
	CMyCurrency(const CMyCurrency& value);
	CMyCurrency(__int64 value);
	CMyCurrency FromDoubleLegal(double value);
	void FromString(CString num);
//assignment operator
	CMyCurrency& operator=(const CMyCurrency& value);
	CMyCurrency& operator=(__int64 value);
	inline __int64& ToRound(void){return data;};
//equality operators
	bool operator ==(const CMyCurrency &value) const;
	bool operator !=(const CMyCurrency &value) const;
	bool operator >=(const CMyCurrency &value) const;
	bool operator <=(const CMyCurrency &value) const;
	bool operator >(const CMyCurrency &value) const;
	bool operator <(const CMyCurrency &value) const;
//arithmetic operators
	friend CMyCurrency operator-(const CMyCurrency& value);
	friend CMyCurrency abs(const CMyCurrency& value);
	CMyCurrency operator+(const CMyCurrency& value);
	CMyCurrency& operator+=(const CMyCurrency& value);
	CMyCurrency operator*(const CMyCurrency& value);
	CMyCurrency& operator*=(const CMyCurrency& value);
	CMyCurrency operator/(const CMyCurrency& value);
	CMyCurrency& operator/=(const CMyCurrency& value);
	CMyCurrency operator-(const CMyCurrency& value);
	CMyCurrency& operator-=(const CMyCurrency& value);
//Operators to convert back to basic types
	inline __int64 ToInt64(void)const{return data;};
	double ToDoubleLegal(void)const;
	
protected:
	__int64 data;
};
int DoubleToInt(double data);
CString FormatPunktI1(INT64 data,BYTE punkt);
CString FormatPunkt1(double data, BYTE punkt);
#endif // !defined(AFX_MYCURRENCY_H__C663A1F3_D0FF_4414_AC7A_E2A0FA517A6C__INCLUDED_)
