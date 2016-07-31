#pragma once

// http://lists.boost.org/boost-users/2016/04/85968.php
#if _MSC_FULL_VER >= 190023918 // VC++ 2015 Update 2 
#define _ENABLE_ATOMIC_ALIGNMENT_FIX 
#endif 

#include <boost/lockfree/stack.hpp>

namespace light
{

template <typename T>
class lockfree_stack
{
public:
    lockfree_stack(int size = 10)
        : m_stack(size)
    {

    }
    void push(const T& t)
    {
        m_stack.push(t);
    }

    bool pop(T& t)
    {
        if (m_stack.empty())
        {
            return false;
        }

        m_stack.pop(t);
        return true;
    }

    bool empty()
    {
        return m_stack.empty();
    }

private:
    boost::lockfree::stack<T> m_stack;
};

} //namespace light
