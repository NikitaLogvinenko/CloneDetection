#ifndef __IPMASK_H_
#define __IPMASK_H_

#include <cstdint>

struct IpMask
{
	IpMask(uint32_t _ip, char _ip_src_dst, size_t _mask = 32) :
		ip(_ip), ip_src_dst(_ip_src_dst), mask(_mask) {}

	bool operator==(const IpMask& ipm) const
	{
		return ip == ipm.ip &&
			ip_src_dst == ipm.ip_src_dst &&
			mask == ipm.mask;
	}

	uint32_t ip;
	char ip_src_dst;
	size_t mask;
};

#endif