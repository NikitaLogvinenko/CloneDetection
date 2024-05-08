#ifndef __PROTOCOLPORT_H_
#define __PROTOCOLPORT_H_

#include <cstdint>

struct ProtocolPort
{
	ProtocolPort(uint8_t _protocol, uint16_t _port = 0, char _port_src_dst = '\0') :
		protocol(_protocol), port(_port), port_src_dst(_port_src_dst) {}

	uint8_t protocol;
	uint16_t port;
	char port_src_dst;

	bool operator==(const ProtocolPort& pport) const
	{
		return protocol == pport.protocol &&
			port_src_dst == pport.port_src_dst &&
			port == pport.port;
	}
};

#endif