#pragma once

#include <map>
#include <memory>

#include "Singleton.h"

namespace light
{

template <typename Key, typename T>
class Repository
    : public Singleton< Repository<Key, T> >
{
public:
    bool add(Key key, T* ptr)
    {
        if (m_datas.find(key) != m_datas.end())
            return false;

        m_datas.insert({ key, std::shared_ptr<T>(ptr) });
        return true;
    }

    bool remove(Key key)
    {
        m_datas.remove(key);
        return true;
    }


    std::shared_ptr<T> get(Key key)
    {
        return m_datas[key];
    }

    void clear()
    {
        m_datas.clear();
    }

    template <typename T>
    void each(T t)
    {
        std::for_each(m_datas.begin(), m_datas.end(), t);
    }

private:
    std::map< Key, std::shared_ptr<T> > m_datas;
};

} // namespace light