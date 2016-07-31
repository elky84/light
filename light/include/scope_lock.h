#pragma once

namespace light
{

template <typename T>
class scope_lock
{
public:
    scope_lock(T& obj)
        : m_object(obj)
    {
        m_object.lock();
    }

    ~scope_lock()
    {
        m_object.unlock();
    }

    void operator =(const scope_lock& rhs)
    {
        m_object = rhs.m_object;
    }

private:
    T & m_object;
};

} // namespace light