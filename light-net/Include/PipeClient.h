#pragma once

#include "Pipe.h"

namespace light
{

namespace net
{

class PipeClient : public Pipe
{
public:
	PipeClient(const std::wstring& name, Peer* peer);

	virtual ~PipeClient();

	bool connect();

	bool release();

	virtual bool disconnect(RESULT_CODE::Type code);

	virtual void on_update();

	virtual void on_disconnect();
};

} // namespace net

} // namespace light
