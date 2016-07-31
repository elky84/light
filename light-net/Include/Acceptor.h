#pragma once

#include <memory>
#include "weak_raw_ptr.hpp"

namespace light
{

namespace net
{

class SessionManager;
class Listener;
class Session;

class Acceptor
{
public: 
	Acceptor(Listener* listener);

	bool accept(Session* session);

	virtual ~Acceptor();

private:
    light::weak_raw_ptr<Listener> m_listener;
};

} // namespace net

} // namespace light