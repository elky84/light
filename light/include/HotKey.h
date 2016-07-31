#pragma once

#include <Windows.h>
#include "Str.h"

namespace light
{
/**
@brief 윈도우 글로벌 핫키를 편하게 지원하는 클래스.
*/
class HotKey
{
public:
	/// 조작자 상수.
	typedef DWORD MODIFIER;

	/// 조작자 상수 종류 정의
	enum
	{
		MODIFIER_NONE = 0x0000,
		MODIFIER_ALT = MOD_ALT,
		MODIFIER_CONTROL = MOD_CONTROL,
		MODIFIER_SHIFT = MOD_SHIFT,
		MODIFIER_WIN = MOD_WIN,
	};

	/// 핫키 정보 구조체
	struct INFORMATION
	{
	public:
		/**
		@breif 생성자
		*/
		INFORMATION()
			: m_Modifier(MODIFIER_NONE)
			, m_uVK(0)
		{
		}

		/**
		@breif 생성자
		@param Modifier 조작자
		@param uVK 가상키 코드
		*/
		INFORMATION(const MODIFIER Modifier, const UINT uVK)
			: m_Modifier(Modifier)
			, m_uVK(uVK)
		{
		}

		/**
		@breif 초기화 메소드
		*/
		void initialize()
		{
			m_Modifier = MODIFIER_NONE;
			m_uVK = 0;
		}

		/**
		@brief 대입 연산자
		@param rhs INFORMATION클래스 참조자
		@return 대입된 결과 클래스 참조자
		*/
		INFORMATION& operator=(const INFORMATION& rhs)
		{
			this->m_Modifier = rhs.m_Modifier;
			this->m_uVK = rhs.m_uVK;
			return *this;
		}

		/** 
		@brief 비교 연산자
		@param Information 키 정보
		@return 비교 결과
		*/
		bool operator == (const INFORMATION& Information) const
		{
			return this->Modifier() == Information.Modifier() && this->VirtualKey() == Information.VirtualKey() ? true : false;
		}

		/**
		@brief 조작자 반환
		@return 조작자
		*/
		const MODIFIER Modifier() const
		{
			return m_Modifier;
		}

		/**
		@brief 가상 키 코드 반환
		@return 가상 키 코드
		*/
		const UINT VirtualKey() const
		{
			return m_uVK;
		}

		/**
		@brief 값이 있는 정보인지 여부ㅡ
		@return 가상 키 코드 값이 있는지 여부
		*/
		const bool Valid() const
		{
			return VirtualKey() != 0;
		}

	private:
		/// 조작자
		MODIFIER m_Modifier;

		/// 가상 키 코드
		UINT m_uVK;
	};

public:
/** 
	@brief 비교 연산자 (==)
	@param Information 키 정보
	@return 비교 결과
	*/
	bool operator == (const INFORMATION& Information) const
	{
		return m_Information == Information ? true : false;
	}

	/** 
	@brief 비교 연산자 (!=)
	@param Information 키 정보
	@return 비교 결과
	*/
	bool operator != (const INFORMATION& Information) const
	{
		return !operator==(Information);
	}

	/** 
	@brief 비교 연산자 (==)
	@param hotkey 비교할 hotkey 클래스 객체
	@return 비교 결과
	*/
	bool operator == (const HotKey& hotkey) const
	{
		return m_Information == hotkey.m_Information ? true : false;
	}

	/** 
	@brief 비교 연산자 (!=)
	@param hotkey 비교할 hotkey 클래스 객체
	@return 비교 결과
	*/
	bool operator != (const HotKey& Hotkey) const
	{
		return !operator==(Hotkey);
	}

public:
	/// 생성자.
	HotKey(const HWND& hWnd);

	/// 소멸자. 
	virtual ~HotKey();

	/// 핫키 등록 메소드
	bool set(const INFORMATION& Information);

	/// 갖고 있는 핫키 등록 메소드
	bool set();

	/// 핫키 등록 해제 메소드
	bool clear();

	/// 갖고 있는 정보 클리어 하는 메소드
	bool Clear();

	/**
	@brief 핫키 정보를 문자열로 반환하는 메소드
	@return 핫키 정보 문자열
	*/
	const std::wstring String() const
	{
		return InformationToString(m_Information);
	}

	/**
	@brief 핫키 정보 반환
	@return 핫키 정보
	*/
	const INFORMATION& Information() const
	{
		return m_Information;
	}

	/**
	@brief 대입 연산자
	@param rhs HotKey클래스 참조자
	@return 대입된 결과 클래스 참조자
	*/
	HotKey& operator=(const HotKey& rhs)
	{
		m_Information = rhs.m_Information;
		return *this;
	}

	/**
	@brief 대입 연산자
	@param rhs INFORMATION클래스 참조자
	@return 대입된 결과 클래스 참조자
	*/
	HotKey& operator=(const INFORMATION& rhs)
	{
		m_Information = rhs;
		return *this;
	}

	/**
	@brief 저장용 데이터로 변환해주는 메소드
	@return 저장용 데이터로 인코딩
	*/
	const std::wstring Encode() const
	{
		std::wstringstream strStream;
		strStream << m_Information.Modifier() << L"|" << m_Information.VirtualKey();
		return strStream.str();
	}

	/**
	@brief 저장용 데이터를 실제 데이터로 변환해주는 메소드
	@param str 저장했던 데이터
	*/
	void Decode(std::wstring& str)
	{
		MODIFIER Modifier = MODIFIER_NONE;
		UINT vk = 0;
		std::wstringstream strStream(str);
		WCHAR delimeter;
		strStream >> Modifier >> delimeter >> vk;
		INFORMATION Info(Modifier, vk);

		if(Info.Valid() != 0)
		{
			set(Info);
		}
	}

private:
	/// 핫키 정보를 문자열로 반환하는 메소드
	static std::wstring InformationToString(const INFORMATION& Information);

	/// 조작자를 문자열로 반환하는 메소드
	static std::wstringstream ModifierToString(const MODIFIER Modifier);

	/// 가상키를 문자열로 반환하는 메소드
	static std::wstring VirtualKeyToString(const UINT vk);

private:
	/// 윈도우 컨트롤 ID
	const DWORD m_ID;

	/// 키 지정 여부
	bool m_register;

	/// 키 정보
	INFORMATION m_Information;

	/// 윈도우 핸들
	const HWND m_hWnd;

	/// 순차적인 컨트롤 ID
	static DWORD m_CountableID;
};
} //namespace light
