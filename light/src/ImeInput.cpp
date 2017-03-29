#include "light.h"
#include "ImeInput.h"

#include "str.h"

#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Version.lib")

#include <VersionHelpers.h>

namespace light
{

#define CHT_IMEFILENAME1    L"TINTLGNT.Ime" // New Phonetic
#define CHT_IMEFILENAME2    L"CINTLGNT.Ime" // New Chang Jie
#define CHT_IMEFILENAME3    L"MSTCIPHA.Ime" // Phonetic 5.1
#define CHS_IMEFILENAME1    L"PINTLGNT.Ime" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    L"MSSCIPYA.Ime" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
    ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // Ime XP(w/WinXP SP1)
#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
enum { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

wchar_t	g_aszIndicator[5][3] =  
{
	L"En",
	L"\x7B80",
	L"\x7E41",
	L"\xAC00",
	L"\x3042",
};

int	ConvertString(UINT codePage, const wchar_t* wText, int wLen, char* text, int len)
{
	if(text == 0) 
		return WideCharToMultiByte(codePage, 0, wText, wLen, NULL, 0, NULL, NULL);

	int tLen = WideCharToMultiByte(codePage, 0, wText, wLen, NULL, 0, NULL, NULL);
	if(tLen > len)
		return 0;

	return WideCharToMultiByte(codePage, 0, wText, wLen, text, tLen, NULL, NULL);
}

ImeInput::ImeInput()
	: m_ulStart(0)
	, m_ulEnd(0)
	, m_hDllIme(NULL)
	, m_ImeState(IMEUI_STATE_OFF)
{
}


ImeInput::~ImeInput()
{
	if( m_hDllIme ) 
		FreeLibrary( m_hDllIme );
}


void ImeInput::InitInput()
{
	m_Ime.initialize();
}

void ImeInput::CarrageReturnCallback(boost::function<void()> func)
{
	m_EnterFunction = func;
}

void ImeInput::TabCallback(boost::function<void()> func)
{
	m_TabFunction = func;
}

void ImeInput::InputCondition(boost::function<size_t()> func)
{
	m_InputConditionFunction = func;
}

void ImeInput::unregister_callback()
{
	m_EnterFunction = NULL;
	m_TabFunction = NULL;
	m_InputConditionFunction = NULL;
}

void ImeInput::OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hkl = (HKL)lParam;
	m_langId = LOWORD(m_hkl);
	m_codePage = GetCodePageFromLang(m_langId);
	m_Input.resize(0);

	DWORD property = ImmGetProperty(GetKeyboardLayout(0), IGP_PROPERTY);

	m_UnicodeIME = (property & IME_PROP_UNICODE) ? true : false;

	CheckToggleState(hWnd);

	GetImeId();

	SetupImeApi(hWnd);
}


bool ImeInput::OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(lParam & GCS_COMPSTR) 
	{
		HIMC hImc = ImmGetContext(hWnd);
		if(hImc)
		{ 
			int tempSize = ImmGetCompositionStringW(hImc, GCS_COMPSTR, NULL, 0);

			wchar_t* temp = (wchar_t*)alloca(tempSize);

			ImmGetCompositionStringW(hImc, GCS_COMPSTR, temp, tempSize);

			m_Comp.assign(temp, temp+tempSize/sizeof(wchar_t));

			ImmReleaseContext(hWnd, hImc);
		}
	}

	if(lParam & GCS_RESULTSTR)
	{
		HIMC hImc = ImmGetContext(hWnd);
		if(hImc) 
		{ 
			int tempSize = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0);

			wchar_t* temp = (wchar_t*)alloca(tempSize);

			ImmGetCompositionStringW(hImc, GCS_RESULTSTR, temp, tempSize);

			m_Input.append(temp, temp+tempSize/sizeof(wchar_t));

			ImmReleaseContext(hWnd, hImc);
		}
	}

	if(lParam & GCS_COMPATTR)
	{
		HIMC hImc = ImmGetContext(hWnd);
		if(hImc)
		{ 
			int tempSize = ImmGetCompositionStringW(hImc, GCS_COMPATTR, NULL, 0);

			BYTE* temp = (BYTE*)alloca(tempSize);

			ImmGetCompositionStringW(hImc, GCS_COMPATTR, temp, tempSize);

			int start = 0;
			int end = 0;
			
			for(start=0; start<tempSize; ++start) 
			{
				if(temp[start]==ATTR_TARGET_CONVERTED || temp[start]==ATTR_TARGET_NOTCONVERTED) 
					break;
			}

			for(end=start; end<tempSize; ++end) 
			{
				if(temp[end]!=temp[start]) 
					break;
			}

			m_ulStart	= start;
			m_ulEnd		= end;

			ImmReleaseContext(hWnd, hImc);
		}
	}

	Assign();
	return true;
}


bool ImeInput::OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_Comp.resize(0);
	m_ulStart = m_ulEnd = 0;
	m_Reading.resize(0);
	Assign();
	return true;
}


bool ImeInput::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HIMC hImc;
	switch (wParam) 
	{
	case IMN_OPENCANDIDATE:
	case IMN_CHANGECANDIDATE:
		{
			hImc = ImmGetContext(hWnd);
			if(hImc) 
			{ 		
				m_Reading.resize(0);

				int candidateLen = ImmGetCandidateListW(hImc, 0, NULL, 0);
				if(candidateLen > 0)
				{
					m_Candidate.resize(candidateLen);
					ImmGetCandidateListW(hImc, 0, (CANDIDATELIST*)&m_Candidate[0], candidateLen);
				}

				ImmReleaseContext(hWnd, hImc);
			}
		}
		return true;
	case IMN_CLOSECANDIDATE:
		m_Candidate.resize(0);
		return true;

	case IMN_SETCONVERSIONMODE:
	case IMN_SETOPENSTATUS:
        CheckToggleState(hWnd);
		return false;

    case IMN_PRIVATE:
        GetPrivateReadingString(hWnd);

        // Trap some messages to hide reading window
        switch( m_Id[0] )
        {
            case IMEID_CHT_VER42:
            case IMEID_CHT_VER43:
            case IMEID_CHT_VER44:
            case IMEID_CHS_VER41:
            case IMEID_CHS_VER42:
				if((lParam==1)||(lParam==2)) 
					return true;

                break;

            case IMEID_CHT_VER50:
            case IMEID_CHT_VER51:
            case IMEID_CHT_VER52:
            case IMEID_CHT_VER60:
            case IMEID_CHS_VER53:
                if((lParam==16)||(lParam==17)||(lParam==26)||(lParam==27)||(lParam==28)) 
					return true;

                break;
        }
        break;
	}

	return false;
}


bool ImeInput::OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(wParam) 
	{
	case '\r':
	case '\n':
		{
			if( m_EnterFunction )
			{
				m_EnterFunction();
			}
		}
		break;
	case '\b':
		if(m_Input.size()) 
			m_Input.resize(m_Input.size() - 1);
		break;
	case '\t':
	case 27:
		{
			if( m_TabFunction )
			{
				m_TabFunction();
			}
		}
		break;
	default:
		{
			if(wParam > 31) 
			{
				wchar_t temp;
				MultiByteToWideChar(m_codePage, 0, (char*)&wParam, 1, &temp, 1);
				m_Input.push_back(temp);

				if(m_InputConditionFunction &&
					false == m_InputConditionFunction() ) // 더 못넣을거 같으면 빼버리자
				{
					m_Input.pop_back();
				}

			}
		}
		break;
	}

	Assign();
	return true;
}

void ImeInput::Clear()
{
	m_Input.clear();
	m_Comp.clear();
	m_Reading.clear();
}

void ImeInput::set(std::wstring& dest)
{
	Clear();
	m_Input = dest;
	m_Destination.reset(&dest);
}

void ImeInput::Assign()
{
	if(m_Destination.get() == NULL)
		return;

	std::wstring editing(m_Input + m_Comp);
	int len = editing.size() * 2;
	char* Temp = new char[len + 1];
	int ConvertLen = ConvertString(m_codePage, editing.c_str(), editing.size(), Temp, len);
	Temp[ConvertLen] = 0;
	m_Destination->assign(CHAR_TO_WCHAR(Temp));
	SAFE_DELETE_ARRAY(Temp);
}

int	ImeInput::GetInput(char* text, int len)
{
	return ConvertString(m_codePage, m_Input.c_str(), m_Input.size(), text, len);
}

int	ImeInput::GetComp(char* text, int len)
{
	return ConvertString(m_codePage, m_Comp.c_str(), m_Comp.size(), text, len);
}

int	ImeInput::GetReading(char* text, int len)
{
	return ConvertString(m_codePage, m_Reading.c_str(), m_Reading.size(), text, len);
}

int	ImeInput::GetCandidate(DWORD index, char* text, int len) 
{
	if(m_Candidate.empty()) 
		return 0;

	CANDIDATELIST* candidateList = (CANDIDATELIST*)&m_Candidate[0];
	if(index >= candidateList->dwCount) 
	{
		return 0;
	} 

	if(m_UnicodeIME) 
	{	
		wchar_t* wText = (wchar_t*)(&m_Candidate[0] + candidateList->dwOffset[index]);
		return ConvertString(m_codePage, wText, wcslen(wText), text, len);

	}
	else 
	{
		char* temp = (char*)(&m_Candidate[0] + candidateList->dwOffset[index]);
		if(text == 0)
			return strlen(temp);

		int tempLen = strlen(temp);
		if(len < tempLen)
			return 0;

		memcpy(text, temp, tempLen);
		return tempLen;
	}
}


int	ImeInput::GetCandidateCount() 
{
	if(m_Candidate.empty()) 
		return 0;

	return ((CANDIDATELIST*)&m_Candidate[0])->dwCount;
}

int	ImeInput::GetCandidateSelection() 
{
	if(m_Candidate.empty()) 
		return 0;

	if(PRIMARYLANGID(m_langId) == LANG_KOREAN)
		return ((CANDIDATELIST*)&m_Candidate[0])->dwCount;
	else
		return ((CANDIDATELIST*)&m_Candidate[0])->dwSelection;
}


int	ImeInput::GetCandidatePageSize() 
{
	if(m_Candidate.empty())
		return 0;

	return ((CANDIDATELIST*)&m_Candidate[0])->dwPageSize;
}


int	ImeInput::GetCandidatePageStart() 
{
	if(m_Candidate.empty()) 
		return 0;

	return ((CANDIDATELIST*)&m_Candidate[0])->dwPageStart;
}


void ImeInput::GetUnderLine(int* start, int* end) 
{ 	
	*start = WideCharToMultiByte(m_codePage, 0, m_Comp.c_str(), m_ulStart, NULL, 0, NULL, NULL); 
	*end = WideCharToMultiByte(m_codePage, 0, m_Comp.c_str(), m_ulEnd, NULL, 0, NULL, NULL); 
}


void ImeInput::GetImeId()
{
	WCHAR    szTmp[1024] = {0, };

	m_Id[0] = m_Id[1] = 0;

    if(!((m_hkl==_CHT_HKL) || (m_hkl==_CHT_HKL2) || (m_hkl==_CHS_HKL)))
		return;
        
	if ( !ImmGetIMEFileName( m_hkl, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
        return;

    if ( !_GetReadingString ) 
	{

        if( ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
            ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
            ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareString( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) )
		{
	        return;
        }
    }

    DWORD   dwVerHandle;
    DWORD   dwVerSize = GetFileVersionInfoSize( szTmp, &dwVerHandle );
    if( dwVerSize )
	{
        LPVOID  lpVerBuffer = alloca( dwVerSize );
        if( GetFileVersionInfo( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) ) 
		{
			LPVOID  lpVerData;
			UINT    cbVerData;

            if( VerQueryValue( lpVerBuffer, L"\\", &lpVerData, &cbVerData ) ) 
			{
                DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
                dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
                if( _GetReadingString ||
                    ( m_langId == LANG_CHT &&
                        ( dwVer == MAKEIMEVERSION(4, 2) || 
                        dwVer == MAKEIMEVERSION(4, 3) || 
                        dwVer == MAKEIMEVERSION(4, 4) || 
                        dwVer == MAKEIMEVERSION(5, 0) ||
                        dwVer == MAKEIMEVERSION(5, 1) ||
                        dwVer == MAKEIMEVERSION(5, 2) ||
                        dwVer == MAKEIMEVERSION(6, 0) ) )
                    ||
                    ( m_langId == LANG_CHS &&
                        ( dwVer == MAKEIMEVERSION(4, 1) ||
                        dwVer == MAKEIMEVERSION(4, 2) ||
                        dwVer == MAKEIMEVERSION(5, 3) ) ) ) 
				{

                    m_Id[0] = dwVer | m_langId;
                    m_Id[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
                }
            }
        }
    }
}



void ImeInput::SetupImeApi(HWND hWnd)
{
    char szImeFile[MAX_PATH + 1];

	_GetReadingString = NULL;
    _ShowReadingWindow = NULL;

	if( ImmGetIMEFileNameA( m_hkl, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) != 0 )
	{
		if( m_hDllIme ) 
			FreeLibrary( m_hDllIme );

		m_hDllIme = LoadLibraryA( szImeFile );
		if ( m_hDllIme )
		{
			_GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))(GetProcAddress( m_hDllIme, "GetReadingString" ));
			_ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))(GetProcAddress( m_hDllIme, "ShowReadingWindow" ));

			if( _ShowReadingWindow ) 
			{
				HIMC hImc = ImmGetContext(hWnd);
				if(hImc) 
				{
					_ShowReadingWindow( hImc, false );
					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
	}
}


bool ImeInput::GetReadingWindowOrientation()
{
    bool bHorizontalReading = ( m_hkl == _CHS_HKL ) || ( m_hkl == _CHT_HKL2 ) || ( m_Id[0] == 0 );
    if( !bHorizontalReading && ( m_Id[0] & 0x0000FFFF ) == LANG_CHT )
    {
        char szRegPath[MAX_PATH];
        HKEY hKey;
        DWORD dwVer = m_Id[0] & 0xFFFF0000;
        strcpy_s( szRegPath, "software\\microsoft\\windows\\currentversion\\" );
        strcat_s( szRegPath, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? "MSTCIPH" : "TINTLGNT" );
        LONG lRc = ::RegOpenKeyExA( HKEY_CURRENT_USER, szRegPath, 0, KEY_READ, &hKey );
        if (lRc == ERROR_SUCCESS)
        {
            DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
            lRc = RegQueryValueExA( hKey, "Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
            if (lRc == ERROR_SUCCESS)
            {
                if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
                       ( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
                     ||
                     ( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
                       (BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
                   )
                {
                    bHorizontalReading = true;
                }
            }
            RegCloseKey( hKey );
        }
    }

	return bHorizontalReading;
}


void ImeInput::GetPrivateReadingString(HWND hWnd)
{
    if( !m_Id[0] ) 
	{
        m_Reading.resize(0);
        return;
    }

	HIMC hImc = ImmGetContext(hWnd);
    if( !hImc ) 
	{
        m_Reading.resize(0);
        return;
    }
    
    DWORD dwErr = 0;
    if( _GetReadingString ) 
	{
        UINT uMaxUiLen;
        BOOL bVertical;
        // Obtain the reading string size
        int wstrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		if( wstrLen == 0 ) 
		{
			m_Reading.resize(0);
		}
		else
		{
			wchar_t *wstr = (wchar_t*)alloca(sizeof(wchar_t) * wstrLen);
            _GetReadingString( hImc, wstrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
			m_Reading.assign(wstr, wstr+wstrLen);
		}

		m_VerticalReading = bVertical ? true : false;

		ImmReleaseContext(hWnd, hImc);

    }
	else 
	{
        // IMEs that doesn't implement Reading String API
		wchar_t* temp = NULL;
		DWORD tempLen = 0;
	    bool bUnicodeIme = false;
		INPUTCONTEXT *lpIC = m_Ime.LockIMC(hImc);

		if(lpIC == NULL) 
		{
			temp = NULL;
			tempLen = 0;
		} 
		else 
		{
			LPBYTE p = 0;
			switch( m_Id[0] )
			{
				case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
				case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
				case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
					p = *(LPBYTE *)((LPBYTE)m_Ime.LockIMCC( lpIC->hPrivate ) + 24 );
					if( !p ) break;
					tempLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
					dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
					temp = (wchar_t *)( p + 56 );
					bUnicodeIme = true;
					break;

				case IMEID_CHT_VER50: // 5.0.x.x // WinME
					p = *(LPBYTE *)( (LPBYTE)m_Ime.LockIMCC( lpIC->hPrivate ) + 3 * 4 );
					if( !p ) break;
					p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
					temp = (wchar_t *)(p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = false;
					break;

				case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
				case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
				case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
					p = *(LPBYTE *)((LPBYTE)m_Ime.LockIMCC( lpIC->hPrivate ) + 4);
					if( !p ) break;
					p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
					temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = true;
					break;

				// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
				case IMEID_CHS_VER41:
					{
						int nOffset;
						nOffset = ( m_Id[1] >= 0x00000002 ) ? 8 : 7;

						p = *(LPBYTE *)((LPBYTE)m_Ime.LockIMCC( lpIC->hPrivate ) + nOffset * 4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 7*4 + 16*2*4);
						dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
						dwErr = std::min( dwErr, tempLen );
						temp = (wchar_t *)(p + 6*4 + 16*2*1);
						bUnicodeIme = true;
					}
					break;

				case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
					{
						int nTcharSize = IsWindowsServer() ? sizeof(wchar_t) : sizeof(char);
						p = *(LPBYTE *)((LPBYTE)m_Ime.LockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
						dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
						temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
						bUnicodeIme = IsWindowsServer() ? true : false;
					}
					break;

				default:
					temp = NULL;
					tempLen = 0;
					break;
			}
		}

		if(tempLen == 0) 
		{
			m_Reading.resize(0);
		} 
		else 
		{
			if( bUnicodeIme )
			{
				m_Reading.assign(temp, tempLen);
			} 
			else 
			{
				int wstrLen = MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, NULL, 0); 
				wchar_t* wstr = (wchar_t*)alloca(sizeof(wchar_t)*wstrLen);
				MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, wstr, wstrLen); 
				m_Reading.assign(wstr, wstrLen);
			}
		}

		m_Ime.UnlockIMCC(lpIC->hPrivate);
		m_Ime.UnlockIMC(hImc);

		m_VerticalReading = !GetReadingWindowOrientation();
    }

	ImmReleaseContext(hWnd, hImc);
}


void ImeInput::CheckToggleState(HWND hWnd)
{
	/* Update Indicator */ 
    switch (PRIMARYLANGID(m_langId)) 
	{
    case LANG_KOREAN:
        m_VerticalCandidate = false;
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_KOREAN];
        break;

    case LANG_JAPANESE:
        m_VerticalCandidate = true;
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_JAPANESE];
        break;

    case LANG_CHINESE:

	    m_VerticalCandidate = true;
		switch(SUBLANGID(m_langId)) 
		{
        case SUBLANG_CHINESE_SIMPLIFIED:
            m_VerticalCandidate = m_Id[0] == 0;
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_CHS];
            break;

        case SUBLANG_CHINESE_TRADITIONAL:
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_CHT];
            break;

        default:
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_NON_IME];
            break;
        }
        break;

    default:
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_NON_IME];
		break;
    }

    if( m_wszCurrIndicator == g_aszIndicator[INDICATOR_NON_IME] )
    {
        char szLang[10];
        GetLocaleInfoA( MAKELCID( m_langId, SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, szLang, sizeof(szLang) );
        m_wszCurrIndicator[0] = szLang[0];
        m_wszCurrIndicator[1] = towlower( szLang[1] );
    }

	
	/* Check Toggle State */ 
	bool bIme = ImmIsIME( m_hkl ) != 0;
	HIMC hImc = ImmGetContext(hWnd);
    if( hImc ) 
	{
        if( ( PRIMARYLANGID(m_langId) == LANG_CHINESE ) && bIme ) 
		{

            DWORD dwConvMode, dwSentMode;
			ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);
            m_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;

        }
		else
		{

            m_ImeState = ( bIme && ImmGetOpenStatus(hImc) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
        }
		ImmReleaseContext(hWnd, hImc);
        
	}
	else 
	{
        m_ImeState = IMEUI_STATE_OFF;
	}
}


int	GetCharsetFromLang( LANGID langid )
{
	switch( PRIMARYLANGID(langid) )
	{
	case LANG_JAPANESE:
		return SHIFTJIS_CHARSET;
	case LANG_KOREAN:
		return HANGEUL_CHARSET;
	case LANG_CHINESE:
		switch( SUBLANGID(langid) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			return GB2312_CHARSET;
		case SUBLANG_CHINESE_TRADITIONAL:
			return CHINESEBIG5_CHARSET;
		default:
			return ANSI_CHARSET;
		}
	case LANG_GREEK:
		return GREEK_CHARSET;
	case LANG_TURKISH:
		return TURKISH_CHARSET;
	case LANG_HEBREW:
		return HEBREW_CHARSET;
	case LANG_ARABIC:
		return ARABIC_CHARSET;
	case LANG_ESTONIAN:
	case LANG_LATVIAN:
	case LANG_LITHUANIAN:
		return BALTIC_CHARSET;
	case LANG_THAI:
		return THAI_CHARSET;
	case LANG_CZECH:
	case LANG_HUNGARIAN:
	case LANG_POLISH:
	case LANG_CROATIAN:
	case LANG_MACEDONIAN:
	case LANG_ROMANIAN:
	case LANG_SLOVAK:
	case LANG_SLOVENIAN:
		return EASTEUROPE_CHARSET;
	case LANG_RUSSIAN:
	case LANG_BELARUSIAN:
	case LANG_BULGARIAN:
	case LANG_UKRAINIAN:
		return RUSSIAN_CHARSET;
	case LANG_VIETNAMESE:
		return VIETNAMESE_CHARSET;
	default:
		return ANSI_CHARSET;
	}
}


int	GetCodePageFromCharset( int charset )
{
	switch( charset )
	{
	case SHIFTJIS_CHARSET:
		return 932;
	case HANGUL_CHARSET:
		return 949;
	case GB2312_CHARSET:
		return 936;
	case CHINESEBIG5_CHARSET:
		return 950;
	case GREEK_CHARSET:
		return 1253;
	case TURKISH_CHARSET:
		return 1254;
	case HEBREW_CHARSET:
		return 1255;
	case ARABIC_CHARSET:
		return 1256;
	case BALTIC_CHARSET:
		return 1257;
	case THAI_CHARSET:
		return 874;
	case EASTEUROPE_CHARSET:
		return 1250;
	case VIETNAMESE_CHARSET:
		return 1258;
	default:
		return 1252;
	}
}


int	GetCodePageFromLang( LANGID langid )
{
	return GetCodePageFromCharset(GetCharsetFromLang(langid));
}

} // namespace light
