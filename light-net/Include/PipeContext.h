#pragma once

#include "weak_raw_ptr.hpp"
#include "Util.h"

namespace light
{

namespace net
{

class Pipe;

class PipeContext : public light::STRUCT_MEMSET_THIS<OVERLAPPED>
{
public:
	PipeContext(Pipe* pPipe, IO::Type io_type);

	virtual ~PipeContext();

	IO::Type io_type()
	{
		return m_io_type;
	}

	HANDLE get_event()
	{
		return hEvent;
	}

	void Proc();

private:
	IO::Type m_io_type;

    light::weak_raw_ptr<Pipe> m_pipe;
};

} // namespace net

} // namespace light
