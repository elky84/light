#pragma once

#include "light-net.h"

#include "LinearBuffer.h"
#include "Util.h"

#include <atomic>

namespace light
{

namespace net
{

class Session;

struct WSABUFEx : public WSABUF
{
public:
	WSABUFEx()
	{
		buf = NULL;
		len = 0;
	}
};

class IocpDelegate 
{
public:
	virtual ~IocpDelegate()
	{
        if (NULL == m_buffer)
            return;

        delete m_buffer;
	}

	virtual void initialize()
	{
		if( NULL == m_buffer )
			return;

		m_buffer->initialize();
        set_register(false);
	}

	inline char* get_head_buffer()
	{
		if(m_buffer)
		{
			return m_buffer->get_head_buffer();
		}
		return NULL;
	}

	inline IO::Type io_type()
	{
		return m_io_type;
	}

	inline unsigned int get_size()
	{
		return m_buffer->get_size();
	}

	inline unsigned int get_length()
	{
		return m_buffer->get_length();
	}

	inline LPWSABUF get_wsa_buf()
	{
		return &m_wsa_buf;
	}

    inline bool extend(int len)
    {
        return m_buffer->extend(len);
    }

	virtual void begin()
	{
	}

	virtual void end()
	{
	}

	virtual bool read(int len)
	{
        UNREFERENCED_PARAMETER(len);
        return false;
    }

	virtual bool write(int len)
	{
		UNREFERENCED_PARAMETER(len);
		return false;
	}

    virtual bool write(int len, const void* ptr)
    {
        UNREFERENCED_PARAMETER(len);
        UNREFERENCED_PARAMETER(ptr);
        return false;
    }

    bool is_register() const
    {
        return m_register.load();
    }

    void set_register(bool flag)
    {
        m_register.exchange(flag);
    }

protected:
	IocpDelegate(IO::Type io_type, LinearNormalBuffer* buffer) 
		: m_io_type(io_type)
		, m_buffer(buffer)
        , m_register(false)
	{
	}


protected:
	IO::Type m_io_type;
	
	LinearNormalBuffer* m_buffer;

	WSABUFEx m_wsa_buf;

    std::atomic<bool> m_register;
};

class IocpDelegateSend : public IocpDelegate
{
public:
	IocpDelegateSend(LinearNormalBuffer* buffer = new LinearNormalBuffer(DEFAULT_SEND_BUFFER_SIZE))
		: IocpDelegate(IO::SEND, buffer)
	{
	}

	virtual ~IocpDelegateSend()
	{
	}

	virtual void begin()
	{
        m_wsa_buf.buf = m_buffer->get_head_buffer();
        m_wsa_buf.len = m_buffer->get_length();
        __super::begin();
	}

	virtual void end()
	{
        m_buffer->compress();
        __super::end();
	}

    virtual bool write(int len, const void* ptr)
    {
        return m_buffer->write(len, ptr);
    }

    virtual bool read(int len)
    {
        return m_buffer->jump_head(len);
    }
};

class IocpDelegateReceive : public IocpDelegate
{
public:
	IocpDelegateReceive(LinearNormalBuffer* buffer = new LinearNormalBuffer(DEFAULT_RECEIVE_BUFFER_SIZE))
		: IocpDelegate(IO::RECV, buffer)
	{
		initialize();
	}

	virtual ~IocpDelegateReceive()
	{
	}

	void buffer_to_wsabuf()
	{
		m_wsa_buf.buf = m_buffer->get_tail_buffer();
		m_wsa_buf.len = m_buffer->get_remain_length();
	}

    virtual void begin()
    {
        buffer_to_wsabuf();
        __super::begin();
    }

	virtual void end()
	{
		m_buffer->compress();
        __super::end();
	}

	virtual bool read(int len)
	{
		return m_buffer->jump_head(len);
	}

	virtual bool write(int len)
	{
		return m_buffer->jump_tail(len);
	}


    virtual bool write(int len, const void* ptr)
    {
        return m_buffer->write(len, ptr);
    }
};

class IocpDelegateConnect : public IocpDelegate
{
public:
	IocpDelegateConnect() 
		: IocpDelegate(IO::CONNECT, new LinearNormalBuffer(ACCEPT_BUFFER_SIZE))
	{
		initialize();
	}

	virtual ~IocpDelegateConnect()
	{
	}
};

class IocpDelegateReuse : public IocpDelegate
{
public:
	IocpDelegateReuse() 
		: IocpDelegate(IO::REUSE, NULL)
	{
		initialize();
	}

	virtual ~IocpDelegateReuse()
	{
	}
};

class IocpContext : public light::STRUCT_MEMSET_THIS<OVERLAPPED>
{
public:
	IocpContext(Session* session, IocpDelegate* iocp_delegate) 
		: m_session(session)
		, m_iocp_delegate(iocp_delegate)
	{
	}

	~IocpContext()
	{
		SAFE_DELETE(m_iocp_delegate);
	}

	inline void initialize()
	{
		m_iocp_delegate->initialize();
	}

	inline Session* get_session()
	{
		return m_session;
	}

	inline unsigned int get_size()
	{
		return m_iocp_delegate->get_size();
	}

	inline unsigned int get_length()
	{
		return m_iocp_delegate->get_length();
	}

	inline bool is_full()
	{
		return get_length() == get_size();
	}

    inline bool extend(int len)
    {
        return m_iocp_delegate->extend(len);
    }

	inline bool read(int len)
	{
		return m_iocp_delegate->read(len);
	}

	inline bool write(int len)
	{
		return m_iocp_delegate->write(len);
	}

    inline bool write(int len, const void* ptr)
    {
        return m_iocp_delegate->write(len, ptr);
    }

	inline LPWSABUF get_wsa_buf()
	{
		return m_iocp_delegate->get_wsa_buf();
	}

	inline void begin()
	{
		return m_iocp_delegate->begin();
	}

	inline void end()
	{
		return m_iocp_delegate->end();
	}

	inline char* get_head_buffer()
	{
		return m_iocp_delegate->get_head_buffer();
	}

	inline IO::Type io_type() const
	{
		return m_iocp_delegate->io_type();
	}

    void set_register(bool flag)
    {
        return m_iocp_delegate->set_register(flag);
    }

    bool is_register() const
    {
        return m_iocp_delegate->is_register();
    }

private:
    // 별도의 클래스로 들고 있는 이유는, IocpContext 클래스에서 가상 함수 사용시, 가상함수 테이블로 인해 GQCS에서 반환 됐을 때 포인터 캐싱해서 사용하기 번거롭기 때문이다.
	IocpDelegate* m_iocp_delegate;

	Session* m_session;
};

} // namespace net

} // namespace light
