#pragma once

#include "Pipe.h"

namespace light
{

namespace net
{

class PipeServer 
	: public Pipe
{
public:
	PipeServer(const std::wstring& name, Peer* peer);

	virtual ~PipeServer();

	bool listen();

	virtual bool disconnect(RESULT_CODE::Type code);

	virtual void on_disconnect();

	virtual void on_update();

	bool release();
};

} // namespace net

} // namespace light
