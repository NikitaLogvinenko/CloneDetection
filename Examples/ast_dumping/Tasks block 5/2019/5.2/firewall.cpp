#include "firewall.h"
#include <list>


namespace
{
	const char SRC_CHAR = 's';
	const char DST_CHAR = 'd';
	const size_t MAX_MASK = 32;
	
	bool ip_equal(uint32_t ip_check, const IpMask& masked_ip)
	{
		uint32_t mask;
		
		if (masked_ip.mask == 0)
		{
			mask = 0;
		}
		else
		{
			mask = (uint32_t)-1 << (MAX_MASK - masked_ip.mask);
		}

		size_t res1 = ip_check & mask;
		size_t res2 = masked_ip.ip & mask;
		return res1 == res2;
	}
	bool check_package(const std::list<std::pair<ProtocolPort, IpMaskResult>>& rules, const Package& pack, bool& rule_result)
	{
		if (rules.empty())
		{
			return false;
		}

		for (const auto& it : rules)
		{
			if (it.second.masked_address.ip_src_dst == SRC_CHAR)
			{
				if (ip_equal(pack.src_ip, it.second.masked_address))
				{
					rule_result = it.second.rule_result;
					return true;
				}
			}
			if (it.second.masked_address.ip_src_dst == DST_CHAR)
			{
				if (ip_equal(pack.dst_ip, it.second.masked_address))
				{
					rule_result = it.second.rule_result;
					return true;
				}
			}
		}

		return false;
	}
}

void firewall::set_default(bool result)
{
	default_result = result;
}


rule_id firewall::add_rule(bool result, const ProtocolPort& pport, const IpMask& ipm)
{
	IpMaskResult ipmr(ipm, result);
	return rules.insert({pport, ipmr});
}

void firewall::delete_rule(const rule_id& rule_id)
{
	if (rules.empty() || rule_id == rules.end())
	{
		return;
	}

	rules.erase(rule_id);
}
void firewall::delete_rule(const ProtocolPort& pport)
{
	if (rules.empty())
	{
		return;
	}

	rules.erase(pport);
}
void firewall::clear_rules()
{
	rules.clear();
}


size_t firewall::get_rules_amount() const
{
	return rules.size();
}
bool firewall::check(const Package& pack) const
{
	if (rules.empty())
	{
		return default_result;
	}

	ProtocolPort src_pp(pack.protocol, pack.src_port, SRC_CHAR);
	ProtocolPort dst_pp(pack.protocol, pack.dst_port, DST_CHAR);

	bool result;
	auto id_range = rules.equal_range(src_pp);
	std::list<std::pair<ProtocolPort, IpMaskResult>> src_rule_list(id_range.first, id_range.second);

	//rule_list.insert(rule_list.begin(), id_range.first, id_range.second);
	if (check_package(src_rule_list, pack, result))
	{
		return result;
	}
	

	id_range = rules.equal_range(dst_pp);
	std::list<std::pair<ProtocolPort, IpMaskResult>> dst_rule_list(id_range.first, id_range.second);
	
	//rule_list.insert(rule_list.begin(), id_range.first, id_range.second);
	if (check_package(dst_rule_list, pack, result))
	{
		return result;
	}

	return default_result;
}


//
//
//#include "firewall.h"
//
//namespace
//{
//	const char SRC_CHAR = 's';
//	const char DST_CHAR = 'd';
//	const size_t MAX_MASK = 32;
//
//	bool ip_equal(uint32_t ip_check, const IpMask& masked_ip)
//	{
//		uint32_t mask;
//
//		if (masked_ip.mask == 0)
//		{
//			mask = 0;
//		}
//		else
//		{
//			mask = (uint32_t)-1 << (MAX_MASK - masked_ip.mask);
//		}
//
//		size_t res1 = ip_check & mask;
//		size_t res2 = masked_ip.ip & mask;
//		return res1 == res2;
//	}
//	bool check_package(const std::list<IpMaskResult>& rules, const Package& pack, bool& rule_result)
//	{
//		if (rules.empty())
//		{
//			return false;
//		}
//
//		for (const auto& it : rules)
//		{
//			if (it.masked_address.ip_src_dst == SRC_CHAR)
//			{
//				if (ip_equal(pack.src_ip, it.masked_address))
//				{
//					rule_result = it.rule_result;
//					return true;
//				}
//			}
//			if (it.masked_address.ip_src_dst == DST_CHAR)
//			{
//				if (ip_equal(pack.dst_ip, it.masked_address))
//				{
//					rule_result = it.rule_result;
//					return true;
//				}
//			}
//		}
//
//		return false;
//	}
//}
//
//RuleId firewall::add_rule(bool result, const ProtocolPort& pport, const IpMask& ipm)
//{
//	IpMaskResult ipmr(ipm, result);
//	rules[pport].emplace_front(ipmr);
//
//	size_t bucket_num = rules.bucket(pport);
//	RuleId id_result(bucket_num, ipmr);
//	return id_result;
//}
//
//void firewall::delete_rule(const RuleId& rule_id)
//{
//	if (rules.empty())
//	{
//		return;
//	}
//
//	for (auto& it : rules)
//	{
//		if (rules.bucket(it.first) == rule_id.bucket_num)
//		{
//			rules[it.first].remove(rule_id.rule);
//			return;
//		}
//	}
//}
//void firewall::clear_rules()
//{
//	rules.clear();
//}
//void firewall::set_default(bool result)
//{
//	default_result = result;
//}
//
//size_t firewall::get_rules_amount() const
//{
//	size_t result = 0;
//	for (const auto& it : rules)
//	{
//		result += it.second.size();
//	}
//	return result;
//}
//bool firewall::check(const Package& pack) const
//{
//	if (rules.empty())
//	{
//		return default_result;
//	}
//
//	ProtocolPort src_pp(pack.protocol, pack.src_port, SRC_CHAR);
//	ProtocolPort dst_pp(pack.protocol, pack.dst_port, DST_CHAR);
//
//	bool result;
//	try
//	{
//		if (check_package(rules.at(src_pp), pack, result))
//		{
//			return result;
//		}
//	}
//	catch (std::out_of_range&) {}
//
//	try
//	{
//		if (check_package(rules.at(dst_pp), pack, result))
//		{
//			return result;
//		}
//	}
//	catch (std::out_of_range&) {}
//
//	return default_result;
//}