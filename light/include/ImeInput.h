#pragma once

#include "Singleton.h"
#include "Ime.h"

#include <boost/function.hpp>
#include <vector>

#include "weak_raw_ptr.hpp"

namespace light
{

int GetCharsetFromLang(LANGID);
int	GetCodePageFromLang( LANGID langid );

class ImeInput
{
private:
	UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

public:
	ImeInput();
	~ImeInput();

	void CarrageReturnCallback(boost::function<void()> func);
	void TabCallback(boost::function<void()> func);
	void InputCondition(boost::function<size_t()> func);

	void unregister_callback();

	void InitInput();

	void Clear();

public:	// messages
	void OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam);

	void set(std::wstring& dest);

	int GetInput(char* text, int len);
	int GetComp(char* text, int len);
	
	int GetReading(char* text, int len);

	int GetCandidate(DWORD index, char* text, int len);
	int GetCandidateCount();
	int GetCandidateSelection();
	int GetCandidatePageSize();
	int GetCandidatePageStart();
	void GetUnderLine(int* start, int* end);

public:	// property
	bool IsVerticalReading()
	{
		return m_VerticalReading; 
	}

	bool IsVerticalCandidate()
	{
		return m_VerticalCandidate; 
	}

	INT GetCodePage() 
	{
		return m_codePage; 
	}

	LANGID GetLangId() 
	{
		return m_langId; 
	}

	int GetCharSet() 
	{
		return GetCharsetFromLang(m_langId); 
	}

	wchar_t* GetIndicator() 
	{
		return m_wszCurrIndicator; 
	}

	int GetImeState() 
	{
		return m_ImeState; 
	}

protected:
	void SetupImeApi(HWND hWnd);
	void GetImeId();
	bool GetReadingWindowOrientation();
	void GetPrivateReadingString(HWND hWnd);
	void CheckToggleState(HWND hWnd);

private:
	void Assign();

public:
	STATIC_SINGLETON(ImeInput);

protected:
	std::wstring m_Input;

	std::wstring m_Comp;

	std::wstring m_Reading;

protected:
	boost::function<void()> m_EnterFunction;

	boost::function<void()> m_TabFunction;

	boost::function<size_t()> m_InputConditionFunction;

	/// 문자열 입력마다, 대입해줄 대상 std::wstring 포인터
	light::weak_raw_ptr<std::wstring> m_Destination;

	std::vector<BYTE> m_Candidate;

	int m_ulStart;
	int	m_ulEnd;

	HKL m_hkl;
	LANGID m_langId;
	INT	m_codePage;

	bool m_UnicodeIME;
	bool m_VerticalReading;
	bool m_VerticalCandidate;
	int	m_ImeState;
	wchar_t* m_wszCurrIndicator;

	DWORD m_Id[2];

	HINSTANCE m_hDllIme;

	light::Ime m_Ime;
};

} // namespace light
