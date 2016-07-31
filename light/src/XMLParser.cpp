#include "light.h"
#include "XMLParser.h"

#include "str.h"

namespace light
{

    XMLParser::XMLParser()
        : m_depth(0)
    {
    }

    XMLParser::~XMLParser()
    {
    }

    bool XMLParser::read_doc(const std::wstring& strXML)
    {
        return m_markup.SetDoc(strXML);
    }

    bool XMLParser::read_file(const std::wstring& strFile)
    {
        return m_markup.Load(strFile);
    }

    bool XMLParser::execute(const std::wstring& Elem)
    {
        if (Elem.empty())
            return false;

        std::wstring str = Elem;
        std::wstring::size_type pos = str.find_first_of(L'/');
        if (pos == 0)
        {
            str.erase(str.begin());
        }

        auto elems = str::split(str, L"/");

        size_t nMatchingCount = 0;
        for (auto it = elems.begin(); it != elems.end(); ++it)
        {
            std::wstring& strElem = (*it);
            if (m_markup.FindElem(strElem))
            {
                ++nMatchingCount;
            }
            else if (m_markup.FindChildElem(strElem))
            {
                ++nMatchingCount;
            }

            if (false == into())
            {
                return false;
            }
        }

        if (nMatchingCount != elems.size())
        {
            return false;
        }
        return true;
    }

    void XMLParser::resize()
    {
        m_bind_attribs.resize(m_depth);
        m_elems.resize(m_depth);
        m_into_flag.resize(m_depth);
    }

    bool XMLParser::into()
    {
        if (false == m_markup.IntoElem())
            return false;

        ++m_depth;
        resize();
        into_flag(true);
        return true;
    }

    bool XMLParser::out()
    {
        if (false == m_markup.OutOfElem())
            return false;

        into_flag(false);

        --m_depth;
        resize();
        return true;
    }

    bool XMLParser::read_elem()
    {
        if (false == find_elem(elem_wstring()))
            return false;

        return true;
    }

    bool XMLParser::bind_elem(const std::wstring& strElem)
    {
        if (strElem == elem_wstring())
            return false;

        reset_pos();

        elem_wstring(strElem);

        // bind_elem을 콜 했다는 것은 next (Fetch) 메소드를 통해 다음 Depth로 into하려 시도한다는 의미이므로 이렇게 처리.
        into_flag(false);
        return true;
    }

    bool XMLParser::next()
    {
        if (true == into_flag())
        {
            if (false == out())
            {
                return false;
            }
        }

        if (false == read_elem())
            return false;

        // next을 콜 했다는 것은, 다음 Depth로 into할 것이기에 현재 Depth의 into_flag를 true해놓음으로써, 
        // 기타 작업들 이후 next가 호출 될 때 into_flag가 true가 반환되게끔 유도해 into/out의 짝이 맞도록 처리하기 위함.
        into_flag(true);

        auto& binds = map();
        for each(auto it in binds)
        {
            auto keys = str::split(it.first, L"_");
            auto& info = it.second;

            auto attrib = get_attrib(keys[1]);
            if (true == attrib.empty())
                continue;

            if (false == XML_CONVETER::convert(attrib, info))
                return false;
        }
        return into();
    }

    const std::wstring XMLParser::get_elem()
    {
        if (false == out())
        {
            LOG_ERROR(L"out() failed.");
            return L"";
        }

        std::wstring str = m_markup.get();
        if (false == into())
        {
            LOG_ERROR(L"into() failed.");
            return L"";
        }

        return str;
    }

    bool XMLParser::find_elem(const std::wstring& Elem)
    {
        return m_markup.FindElem(Elem);
    }

    bool XMLParser::find_child_elem(const std::wstring& Elem)
    {
        return m_markup.FindChildElem(Elem);
    }

    const std::wstring XMLParser::get_attrib(const std::wstring& Attrib)
    {
        return m_markup.GetAttrib(Attrib);
    }

    const std::wstring XMLParser::get_child_attrib(const std::wstring& Attrib)
    {
        return m_markup.GetChildAttrib(Attrib);
    }

    const std::wstring& XMLParser::elem_wstring()
    {
        return m_elems[m_depth - 1];
    }

    void XMLParser::elem_wstring(const std::wstring& str)
    {
        m_elems[m_depth - 1] = str;
    }

    XMLParser::MAP_BIND& XMLParser::map()
    {
        return m_bind_attribs[m_depth - 1];
    }

    bool XMLParser::into_flag()
    {
        return m_into_flag[m_depth - 1];
    }

    void XMLParser::into_flag(bool bFlag)
    {
        m_into_flag[m_depth - 1] = bFlag;
    }

} //namespace light