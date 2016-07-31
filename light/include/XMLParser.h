#pragma once

#include <map>
#include <vector>

#include "Markup.h"
#include "XMLConverter.h"

namespace light
{

class XMLParser
{
private:
    typedef unsigned short DEPTH;

    typedef std::map<std::wstring, XML_CONVETER::CONVERT_INFO> MAP_BIND;

public:
    XMLParser();

    ~XMLParser();

    bool read_doc(const std::wstring& strXML);

    bool read_file(const std::wstring& strFile);

    bool execute(const std::wstring& Elem);

    /// XML 문자열 읽어옴
    std::wstring read_doc()
    {
        return m_markup.GetDoc();
    }

    bool next();

public:  // get_elem 관련 메소드
    template <typename T>
    bool get_elem(const std::wstring& Elem, T& t)
    {
        XML_CONVETER::TYPE type = XML_CONVETER::convert_type(t);
        if (type == XML_CONVETER::DENIED)
            return false;

        return bind_get_elem(type, Elem, &t, sizeof(t));
    }

    /// 특정 엘리먼트 안에 포함된 Attrib와 문자열 객체와 바인드 시켜주는 메소드
    bool get_elem(const std::wstring& Elem, WCHAR* str, size_t get_length)
    {
        XML_CONVETER::TYPE type = XML_CONVETER::convert_type(str);
        if (type == XML_CONVETER::DENIED)
            return false;

        return bind_get_elem(type, Elem, str, get_length);
    }

    /// 현재 Elem의 값 얻어오는 메소드
    const std::wstring get_elem();

    /// 현재 Elem의 값 얻어오는 메소드
    const int get_elem_to_integer()
    {
        return _ttoi(get_elem().c_str());
    }

    /// 현재 Elem의 값 얻어오는 메소드
    const double get_elem_to_double()
    {
        return _tstof(get_elem().c_str());
    }

    /// Depth를 낮추는 메소드
    bool out();

private:
    /// 특정 엘리먼트 안에 포함된 Attrib와 버퍼와 바인드 시켜주는 메소드
    bool bind_get_elem(const XML_CONVETER::TYPE type, const std::wstring& Elem, void* ptr, size_t get_length)
    {
        if (false == bind_elem(Elem))
            return false;

        if (false == next())
            return false;

        XML_CONVETER::CONVERT_INFO ConvertInfo(type, get_length, ptr);
        if (false == XML_CONVETER::convert(get_elem(), ConvertInfo))
            return false;

        next();
        return true;
    }

    /// 해당 타입이 넘어오면 컴파일 안되게 하기 위한 선언부
    bool get_elem(const std::wstring& Elem, const WCHAR* str);

    /// 해당 타입이 넘어오면 컴파일 안되게 하기 위한 선언부
    bool get_elem(const std::wstring& Elem, WCHAR* str);

public: // bind 계열 메소드

        /// 현재 설정
    bool bind_elem(const std::wstring& strElem);

    /// 특정 엘리먼트 안에 포함된 Attrib와 문자열 객체와 바인드 시켜주는 메소드
    template <typename T>
    bool bind_attrib(const std::wstring& Attrib, T& Data)
    {
        const XML_CONVETER::TYPE type = XML_CONVETER::convert_type(Data);
        if (type == XML_CONVETER::DENIED)
            return false;

        return bind_attrib(type, Attrib, &Data, sizeof(Data));
    }

    /// 특정 엘리먼트 안에 포함된 Attrib와 문자열 객체와 바인드 시켜주는 메소드
    bool bind_attrib(const std::wstring& Attrib, WCHAR* Str, size_t get_length)
    {
        const XML_CONVETER::TYPE type = XML_CONVETER::convert_type(Str);
        if (type == XML_CONVETER::DENIED)
            return false;

        return bind_attrib(type, Attrib, Str, get_length);
    }

private: /// 해당 타입이 넘어오면 컴파일 안되게 하기 위한 선언부
    bool bind_attrib(const std::wstring& Attrib, const WCHAR* Str);

    bool bind_attrib(const std::wstring& Attrib, WCHAR* Str);

private:
    /// 특정 엘리먼트 안에 포함된 Attrib와 버퍼와 바인드 시켜주는 메소드
    bool bind_attrib(const XML_CONVETER::TYPE Type, const std::wstring& Attrib, void* ptr, size_t get_length)
    {
        auto& binds = map();
        auto Ret = binds.insert((MAP_BIND::value_type(elem_wstring() + L"_" + Attrib, XML_CONVETER::CONVERT_INFO(Type, get_length, ptr))));
        return Ret.second;
    }

private:
    /// 컨테이너 크기 재조절 메소드
    void resize();

    /// Depth를 높이는 메소드
    bool into();

    /// 다음 엘리먼트를 읽어 바인드된 문자열에 담는 메소드
    bool read_elem();

    /// 위치 리셋 기능
    void reset_pos()
    {
        m_markup.ResetMainPos();
    }

private: // 단순 랩핑. 어떤 메소드를 사용하는 가를 추적하기 위함.

            /// Elem 정보를 찾는 메소드
    bool find_elem(const std::wstring& Elem);

    /// ChildElem 정보를 찾는 메소드
    bool find_child_elem(const std::wstring& Elem);

    /// Attrib를 값을 얻어내는 메소드
    const std::wstring get_attrib(const std::wstring& Attrib);

    /// ChildAttrib를 값을 얻어내는 메소드
    const std::wstring get_child_attrib(const std::wstring& Attrib);

private: // 현재 Depth에 해당하는 자료구조를 얻어오는 메소드

            /// 현재 Depth에 해당되는 Elem을 얻어오는 메소드
    const std::wstring& elem_wstring();

    /// Element 문자열 저장
    void elem_wstring(const std::wstring& str);

    /// 현재 Depth에 해당되는 Attrib map를 얻어오는 메소드
    MAP_BIND& map();

    /// into 플래그 설정 메소드
    void into_flag(bool bFlag);

    /// into 플래그 반환 메소드
    bool into_flag();

private:
    std::vector<std::wstring> m_elems;

    std::vector<MAP_BIND> m_bind_attribs;

    std::vector<bool> m_into_flag;

    DEPTH m_depth;

    CMarkup m_markup;
};

} //namespace light