
#pragma once
#include <string>
#include <Windows.h>
using namespace std;



class CBaseStringW;
class CBaseStringU;

class  CBaseStringA
{
public:
	CBaseStringA(void);
	CBaseStringA( const char* lpsz );
	CBaseStringA( const string & str );
	CBaseStringA( const CBaseStringA & str );
	CBaseStringA( const wchar_t* lpsz );
	CBaseStringA( const wstring & str );
	CBaseStringA( const CBaseStringW & str );
	CBaseStringA( const CBaseStringU & str );
public:
	~CBaseStringA(void);
public:
	bool operator==( const CBaseStringA & str );
	bool operator==( const string & str );
	bool operator==( const char* lpsz );

	bool operator!=( const CBaseStringA & str );
	bool operator!=( const string & str );
	bool operator!=( const char* lpsz );

	CBaseStringA & operator=( const CBaseStringA & str );
	CBaseStringA & operator=( const string & str );
	CBaseStringA & operator=( const char* lpsz );
	CBaseStringA & operator=( const CBaseStringW & str );
	CBaseStringA & operator=( const wstring & str );
	CBaseStringA & operator=( const wchar_t* lpsz );
	CBaseStringA & operator=( const CBaseStringU & str );

	CBaseStringA & operator+=( const CBaseStringA & str );
	CBaseStringA & operator+=( const string & str );
	CBaseStringA & operator+=( const char* lpsz );

	bool operator<( const CBaseStringA & str ) const;

	const string & GetStr() const;
	const char* c_str() const;
	operator LPSTR() const;

	int Find( const char* lpsz, int pos = 0 );
	int Find( const string & str, int pos = 0 );
	int Find( const CBaseStringA & str, int pos = 0 );
	int Find( char ch, int pos = 0 );

	int RFind( const char* lpsz );
	int RFind( const string & str );
	int RFind( const CBaseStringA & str );
	int RFind( char ch );

	CBaseStringA substr( int pos = 0, string::size_type _Count = string::npos ) const;
	CBaseStringA Left(int iLength) const;
	CBaseStringA Right(int iLength) const;
	CBaseStringA Mid(int iPos, int iLength=-1) const;

	CBaseStringA Replace( int pos, int count, const char* lpsz );
	int Replace(LPCSTR pstrFrom, LPCSTR pstrTo);
	int CompareNoCase( LPCSTR pstr );
	void MakeUpper();
	void MakeLower();

	void Trim();
	void TrimLeft();
	void TrimRight();

	int GetLength();
	bool IsEmpty();
	void clear();

	int Format(const char* lpFormat, ...);

protected:
	string m_str;
};
class  CBaseStringW
{
public:
	CBaseStringW(void);
	CBaseStringW( const wchar_t* lpsz );
	CBaseStringW( const wstring & str );
	CBaseStringW( const CBaseStringW & str );
	CBaseStringW( const char* lpsz );
	CBaseStringW( const string & str );
	CBaseStringW( const CBaseStringA & str );
	CBaseStringW( const CBaseStringU & str );
public:
	~CBaseStringW(void);
public:
	bool operator==( const CBaseStringW & str );
	bool operator==( const wstring & str );
	bool operator==( const wchar_t* lpsz );

	bool operator!=( const CBaseStringW & str );
	bool operator!=( const wstring & str );
	bool operator!=( const wchar_t* lpsz );

	CBaseStringW & operator=( const CBaseStringW & str );
	CBaseStringW & operator=( const wstring & str );
	CBaseStringW & operator=( const wchar_t* lpsz );
	CBaseStringW & operator=( const CBaseStringA & str );
	CBaseStringW & operator=( const string & str );
	CBaseStringW & operator=( const char* lpsz );
	CBaseStringW & operator=( const CBaseStringU & str );

	CBaseStringW & operator+=( const CBaseStringW & str );
	CBaseStringW & operator+=( const wstring & str );
	CBaseStringW & operator+=( const wchar_t* lpsz );

	bool operator<( const CBaseStringW & str ) const;

	const wstring & GetStr() const;
	const WCHAR* c_str() const;
	operator LPWSTR() const;

	int Find( const wchar_t* lpsz, int pos = 0 );
	int Find( const wstring & str, int pos = 0 );
	int Find( const CBaseStringW & str, int pos = 0 );
	int Find( wchar_t ch, int pos = 0 );

	int RFind( const wchar_t* lpsz );
	int RFind( const wstring & str );
	int RFind( const CBaseStringW & str );
	int RFind( wchar_t ch );

	CBaseStringW substr( int pos = 0, wstring::size_type _Count = wstring::npos ) const;
	CBaseStringW Left(int iLength) const;
	CBaseStringW Right(int iLength) const;
	CBaseStringW Mid(int iPos, int iLength=-1) const;

	CBaseStringW Replace( int pos, int count, const wchar_t* lpsz );
	int Replace(LPCWSTR pstrFrom, LPCWSTR pstrTo);
	int CompareNoCase( LPCWSTR pstr );

	void MakeUpper();
	void MakeLower();

	void Trim();
	void TrimLeft();
	void TrimRight();

	int GetLength();
	bool IsEmpty();
	void clear();

	int Format(const wchar_t* lpFormat, ...);
protected:
	wstring m_str;
};

class  CBaseStringU
{
public:
	CBaseStringU(void);
	CBaseStringU( const char* lpsz );
	CBaseStringU( const string & str );
	CBaseStringU( const CBaseStringA & str );
	CBaseStringU( const CBaseStringU & str );
	CBaseStringU( const wchar_t* lpsz );
	CBaseStringU( const wstring & str );
	CBaseStringU( const CBaseStringW & str );
public:
	~CBaseStringU(void);
public:
	bool operator==( const CBaseStringU & str );
	bool operator!=( const CBaseStringU & str );


	CBaseStringU & operator=( const CBaseStringU & str );
	CBaseStringU & operator=( const CBaseStringA & str );
	CBaseStringU & operator=( const string & str );
	CBaseStringU & operator=( const char* lpsz );
	CBaseStringU & operator=( const CBaseStringW & str );
	CBaseStringU & operator=( const wstring & str );
	CBaseStringU & operator=( const wchar_t* lpsz );

	CBaseStringU & operator+=( const CBaseStringU & str );
	CBaseStringU & operator+=( const string & str );
	CBaseStringU & operator+=( const char* lpsz );

	bool operator<( const CBaseStringU & str ) const;

	const string & GetStr() const;
	const char* c_str() const;
	

	int Find( const char* lpsz, int pos = 0 );
	int Find( const string & str, int pos = 0 );
	int Find( const CBaseStringU & str, int pos = 0 );
	int Find( char ch, int pos = 0 );

	int RFind( const char* lpsz );
	int RFind( const string & str );
	int RFind( const CBaseStringU & str );
	int RFind( char ch );

	CBaseStringU substr( int pos = 0, string::size_type _Count = string::npos ) const;

	CBaseStringU Replace( int pos, int count, const char* lpsz );

	void Trim();
	void TrimLeft();
	void TrimRight();

	void clear();

protected:
	string m_str;
};

#if defined(UNICODE)
typedef CBaseStringW CBaseString;
#else
typedef CBaseStringA CBaseString;
#endif
