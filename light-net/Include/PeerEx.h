#pragma once

#include "Peer.h"

namespace light
{

namespace net
{

class PeerEx 
	: public Peer
{
	const int m_idx;

public:
	PeerEx(const int& idx);

	virtual ~PeerEx();

	const int get_idx()
    {
        return m_idx;
    }
};

} // namespace net

} // namespace light
