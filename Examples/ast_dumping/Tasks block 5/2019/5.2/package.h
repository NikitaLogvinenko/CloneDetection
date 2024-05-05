#ifndef __PACKAGE_H_
#define __PACKAGE_H_

#include <cstdint>

struct Package
{
	Package(uint8_t _protocol, uint32_t _src_ip, uint32_t _dst_ip, uint16_t _src_port, uint16_t _dst_port) :
		protocol(_protocol), src_ip(_src_ip), dst_ip(_dst_ip), src_port(_src_port), dst_port(_dst_port) {}

	uint8_t protocol;

	uint32_t src_ip;
	uint32_t dst_ip;

	uint16_t src_port;
	uint16_t dst_port;
};

#endif