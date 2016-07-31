#include "light.h"
#include "Thread.h"
#include "Seh.h"

namespace light
{

Thread::Thread(callback update, callback begin, callback end)
    : m_begin(begin)
    , m_update(update)
    , m_end(end)
    , m_active(false)
{
}

Thread::~Thread()
{
    join();
}

void Thread::wrapped_update()
{
    Seh::Install();
    if (m_begin)
    {
        m_begin();
    }
    
    while (m_active.load())
    {
        SEH_TRY
        {
            if (m_update)
            {
                m_update();
            }
        }
        SEH_CATCH
        {

        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }

    if (m_end)
    {
        m_end();
    }
}

// do not called, in own boost::thread.
// raised boost exception or do not return from join.
void Thread::join()
{
    if(m_thread.get() && m_thread->joinable())
        m_thread->join();
}

void Thread::start()
{
    m_active.exchange(true);
    m_thread.reset(new boost::thread(&Thread::wrapped_update, this));
}

void Thread::stop()
{
    if (m_thread.get() == NULL)
        return;

    m_active.exchange(false);
}

} //namepsace light
