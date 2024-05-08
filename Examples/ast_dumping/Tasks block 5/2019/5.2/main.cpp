#include <crtdbg.h>
#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "firewall.h"



class DefaultRule : public ::testing::Test {
protected:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};
TEST_F(DefaultRule, empty_rules_amount)
{
	new int;
	firewall ufw(false);
	ASSERT_EQ((size_t)0, ufw.get_rules_amount());
}
TEST_F(DefaultRule, empty_rule_deleting)
{
	firewall ufw(false);
	ufw.delete_rule({ });
}
TEST_F(DefaultRule, empty_rules_clear)
{
	firewall ufw(false);
	ufw.clear_rules();
}

TEST_F(DefaultRule, package_not_pass)
{
	firewall ufw(false);
	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(DefaultRule, package_pass)
{
	firewall ufw(true);
	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(DefaultRule, change_default)
{
	firewall ufw(true);

	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_TRUE(ufw.check(pack));

	ufw.set_default(false);

	ASSERT_FALSE(ufw.check(pack));
}

class OneRule : public ::testing::Test {
protected:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};
TEST_F(OneRule, package_not_pass)
{
	firewall ufw(false);
	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, package_pass)
{
	firewall ufw(true);
	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, change_default)
{
	firewall ufw(true);

	Package pack = { 1, 0xFFFF, 0xFFFF, 12, 22 };
	ASSERT_TRUE(ufw.check(pack));

	ufw.set_default(false);

	ASSERT_FALSE(ufw.check(pack));
}

TEST_F(OneRule, rule_adding)
{
	firewall ufw(false);
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });

	ASSERT_EQ((size_t)1, ufw.get_rules_amount());
}
TEST_F(OneRule, rule_deleting)
{
	firewall ufw(false);
	ProtocolPort pport(0, 0, 's');
	IpMask ipm(0, 's', 0);
	auto rule_id = ufw.add_rule(true, pport, ipm);
	ASSERT_EQ((size_t)1, ufw.get_rules_amount());
	
	ufw.delete_rule(rule_id);//rule_id.bucket_num, rule_id.rule);
	ASSERT_EQ((size_t)0, ufw.get_rules_amount());
}
TEST_F(OneRule, rule_not_deleting)
{
	firewall ufw(false);
	ProtocolPort pport(0, 0, 's');
	IpMask ipm(0, 's', 0);
	auto rule_id = ufw.add_rule(true, pport, ipm);
	ASSERT_EQ((size_t)1, ufw.get_rules_amount());

	ufw.delete_rule({0, 0, 'd'});
	ASSERT_EQ((size_t)1, ufw.get_rules_amount());
}
TEST_F(OneRule, rules_clear)
{
	firewall ufw(false);
	auto rule_id = ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)1, ufw.get_rules_amount());

	ufw.clear_rules();
	ASSERT_EQ((size_t)0, ufw.get_rules_amount());
}

TEST_F(OneRule, package_empty)
{
	firewall ufw(true);
	ufw.add_rule(false, { 0, 0, 's' }, { 0, 's', 0 });

	Package pack(0, 0, 0, 0, 0);
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, package_empty_default)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFF, 's', 12 });

	Package pack(0, 0, 0, 0, 0);
	ASSERT_FALSE(ufw.check(pack));
}

TEST_F(OneRule, package_pass_0_mask_full_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 0 });

	Package pack(1, 0xFFFFFFFF, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_0_mask_half_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 0 });
	
	Package pack(1, 0xFFFFAAAA, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_0_mask_ip_dismatch)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 0 });

	Package pack(1, 0xAAAAAAAA, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}

TEST_F(OneRule, package_pass_8_mask_full_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 8 });

	Package pack(1, 0xFFFFFFFF, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_8_mask_half_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 8 });

	Package pack(1, 0xFFFFAAAA, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_8_mask_ip_dismatch)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 8 });

	Package pack(1, 0xAAAAAAAA, 0, 12, 0);
	ASSERT_FALSE(ufw.check(pack));
}

TEST_F(OneRule, package_pass_24_mask_full_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 24 });

	Package pack(1, 0xFFFFFFFF, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_24_mask_half_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 24 });

	Package pack(1, 0xFFFFAAAA, 0, 12, 0);
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_24_mask_ip_dismatch)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 24 });

	Package pack(1, 0xAAAAAAAA, 0, 12, 0);
	ASSERT_FALSE(ufw.check(pack));
}

TEST_F(OneRule, package_pass_32_mask_full_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });

	Package pack(1, 0xFFFFFFFF, 0, 12, 0);
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_32_mask_half_ip_match)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });

	Package pack(1, 0xFFFFAAAA, 0, 12, 0);
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, package_pass_32_mask_ip_dismatch)
{
	firewall ufw(false);
	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });

	Package pack(1, 0xAAAAAAAA, 0, 12, 0);
	ASSERT_FALSE(ufw.check(pack));
}

TEST_F(OneRule, test_rule)
{
	firewall ufw(false);
	Package pack(1, 0xFFFFFFFF, 0, 12, 0);

	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });
	ASSERT_TRUE(ufw.check(pack));
}
TEST_F(OneRule, test_rule_protocol_dismatch)
{
	firewall ufw(false);
	Package pack(2, 0xFFFFFFFF, 0, 12, 0);

	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, test_rule_port_dismatch)
{
	firewall ufw(false);
	Package pack(1, 0xFFFFFFFF, 0, 0, 12);

	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(OneRule, test_rule_ip_dismatch)
{
	firewall ufw(false);
	Package pack(1, 0xFFFFAAAA, 0, 0, 32);

	ufw.add_rule(true, { 1, 12, 's' }, { 0xFFFFFFFF, 's', 32 });
	ASSERT_FALSE(ufw.check(pack));
}

class TwoRules : public ::testing::Test {
protected:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};
TEST_F(TwoRules, same_rules_adding)
{
	firewall ufw(false);
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)2, ufw.get_rules_amount());
}
TEST_F(TwoRules, rules_adding)
{
	firewall ufw(false);
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 1, 12, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)2, ufw.get_rules_amount());
}
TEST_F(TwoRules, test_rule_last_added)
{
	firewall ufw(true);
	ufw.add_rule(true, { 1, 12, 's' }, { 0, 's', 0 });
	ufw.add_rule(false, { 0, 0, 's' }, { 0, 's', 0 });
	
	Package pack(0, 0, 0, 0, 0);
	ASSERT_FALSE(ufw.check(pack));
}
TEST_F(TwoRules, test_rule_first_added)
{
	firewall ufw(true);
	ufw.add_rule(false, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 1, 12, 's' }, { 0, 's', 0 });
	
	Package pack(0, 0, 0, 0, 0);
	ASSERT_FALSE(ufw.check(pack));
}

class MultipleRules : public ::testing::Test {
protected:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};
TEST_F(MultipleRules, same_rules_adding)
{
	firewall ufw(false);
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)6, ufw.get_rules_amount());
}
TEST_F(MultipleRules, same_rules_deleting_one)
{
	firewall ufw(false);
	auto rule_id = ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)6, ufw.get_rules_amount());

	ufw.delete_rule(rule_id);
	ASSERT_EQ((size_t)5, ufw.get_rules_amount());
}
TEST_F(MultipleRules, same_rules_deleting_all)
{
	firewall ufw(false);
	auto rule_id = ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)6, ufw.get_rules_amount());

	ufw.delete_rule({ 0, 0, 's' });
	ASSERT_EQ((size_t)0, ufw.get_rules_amount());
}
TEST_F(MultipleRules, different_rules_deleting)
{
	firewall ufw(false);
	auto rule_id = ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 1, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 2, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 3, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 4, 0, 's' }, { 0, 's', 0 });
	ufw.add_rule(true, { 5, 0, 's' }, { 0, 's', 0 });
	ASSERT_EQ((size_t)6, ufw.get_rules_amount());

	ufw.delete_rule(rule_id);
	ASSERT_EQ((size_t)5, ufw.get_rules_amount());
}
//TEST_F(MultipleRules, deleting_unexist)
//{
//	firewall ufw(false);
//	auto rule_id = ufw.add_rule(true, { 0, 0, 's' }, { 0, 's', 0 });
//	ufw.add_rule(true, { 1, 0, 's' }, { 0, 's', 0 });
//	ufw.add_rule(true, { 2, 0, 's' }, { 0, 's', 0 });
//	ufw.add_rule(true, { 3, 0, 's' }, { 0, 's', 0 });
//	ufw.add_rule(true, { 4, 0, 's' }, { 0, 's', 0 });
//	ufw.add_rule(true, { 5, 0, 's' }, { 0, 's', 0 });
//	ASSERT_EQ((size_t)6, ufw.get_rules_amount());
//
//	advance(rule_id, 5);
//	ufw.delete_rule(rule_id);
//	ASSERT_EQ((size_t)6, ufw.get_rules_amount());
//}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}