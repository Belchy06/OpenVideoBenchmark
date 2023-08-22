#pragma once

#include <memory>
#include <vector>

#include "ovb_common/rtp/packet.h"
#include "ovb_recv/depacketizer/depacketizer_listener.h"

class Depacketizer
{
public:
	static std::shared_ptr<Depacketizer> Create();

	void HandlePacket(RTPPacket InPacket);
	void RegiseterDepacketizerListener(IDepacketizerListener* InDepacketizerListener);

private:
	Depacketizer();

private:
	int					   prevMarker;
	uint32_t			   prevTimestamp;
	uint16_t			   prevSequenceNumber;
	std::vector<RTPPacket> Packets;

	static std::shared_ptr<Depacketizer> Self;
	IDepacketizerListener*				 DepacketizerListener;
};