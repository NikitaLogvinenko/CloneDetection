#ifndef __FIREWALL_H_
#define __FIREWALL_H_


#include <functional>
#include <unordered_map>
#include <iterator>
#include "package.h"
#include "ip_mask.h"
#include "protocol_port.h"


struct IpMaskResult
{
	IpMaskResult(const IpMask& _masked_address, bool _rule_result) :
		masked_address(_masked_address), rule_result(_rule_result) {}

	IpMask masked_address;
	bool rule_result;

	bool operator==(const IpMaskResult& ipmr) const
	{
		return masked_address == ipmr.masked_address &&
			rule_result == ipmr.rule_result;
	}
};


typedef std::unordered_multimap<ProtocolPort, IpMaskResult>::iterator rule_id;
class firewall final
{
	struct Hash
	{
		size_t operator()(const ProtocolPort& pport) const
		{
			size_t result = pport.port_src_dst;

			result <<= 8;
			result &= pport.protocol;

			result <<= 16;
			result &= pport.port;

			std::hash<size_t> hash_s;

			return hash_s(result);
		}
	};

public:
	firewall(bool _default_result = false) : default_result(_default_result) {}

	void set_default(bool result);

	rule_id add_rule(bool result, const ProtocolPort& protocol, const IpMask& ip);
	void delete_rule(const rule_id& rule_id);
	void delete_rule(const ProtocolPort& pport);
	void clear_rules();

	size_t get_rules_amount() const;
	bool check(const Package& pack) const;


private:
	std::unordered_multimap<ProtocolPort, IpMaskResult, Hash> rules;
	bool default_result;
};

#endif


/*firewall(bool _default_result = false) : default_result(_default_result) {}

	RuleId add_rule(bool result, const ProtocolPort& protocol, const IpMask& ip);
	void delete_rule(const RuleId& rule_id);
	void clear_rules();
	void set_default(bool result);

	size_t get_rules_amount() const;
	bool check(const Package& pack) const;*/