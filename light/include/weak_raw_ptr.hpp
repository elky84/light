#pragma once

namespace light
{

template<typename T>
class weak_raw_ptr
{
private:
    T* m_ptr;

public:
    weak_raw_ptr(T* ptr = 0)
        : m_ptr(ptr)
    {

    }

    weak_raw_ptr(const weak_raw_ptr& rhs)
        : m_ptr(0)
    {
        operator =(rhs);
    }

    weak_raw_ptr(const std::shared_ptr<T>& rhs)
        : m_ptr(0)
    {
        operator =(rhs);
    }

    weak_raw_ptr(const std::unique_ptr<T>& rhs)
        : m_ptr(0)
    {
        operator =(rhs);
    }

    void operator =(const T* ptr)
    {
        reset(const_cast<T*>(ptr));
    }

    void operator =(const T& ref)
    {                                                           
        reset(const_cast<T&>(ref));
    }


    void operator =(const weak_raw_ptr& rhs)
    {
        reset(rhs.get());
    }

    void operator =(const std::shared_ptr<T>& rhs)
    {
        reset(rhs.get());
    }

    void operator =(const std::unique_ptr<T>& rhs)
    {
        reset(rhs.get());
    }

    bool operator <(const weak_raw_ptr& rhs) const
    {
        return m_ptr < rhs.get();
    }

    T* operator ->() const
    {
        return m_ptr;
    }


    T *get() const
    {
        return m_ptr;
    }


    void release()
    {
        reset();
    }

    void reset(const T* ptr = 0)
    {
        m_ptr = const_cast<T*>(ptr);
    }

    void reset(const weak_raw_ptr& rhs)
    {
        operator=(rhs);
    }

    void reset(const std::shared_ptr<T>& rhs)
    {
        operator=(rhs);
    }

    void reset(const std::unique_ptr<T>& rhs)
    {
        operator=(rhs);
    }

    bool operator==(const T* rhs) const
    {
        return m_ptr == rhs;
    }

    bool operator!=(const T* rhs) const
    {
        return !operator==(rhs);
    }

    bool operator==(const weak_raw_ptr& rhs) const
    {
        return m_ptr == rhs.get();
    }

    bool operator!=(const weak_raw_ptr& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator==(const std::shared_ptr<T>& rhs) const
    {
        return m_ptr == rhs.get();
    }

    bool operator!=(const std::shared_ptr<T>& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator==(const std::unique_ptr<T>& rhs) const
    {
        return m_ptr == rhs.get();
    }

    bool operator!=(const std::unique_ptr<T>& rhs) const
    {
        return !operator==(rhs);
    }
};

} //namespace light