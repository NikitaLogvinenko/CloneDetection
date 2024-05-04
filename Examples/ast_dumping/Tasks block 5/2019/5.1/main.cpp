#include <crtdbg.h>
#include "gtest/gtest.h"

#include <string>

#include "parcer.h"
#include "friend_adviser.h"


class TestParcing : public ::testing::Test {
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
TEST_F(TestParcing, parce_c5)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_2_1 user_3_1"
							"\nuser_3_1 user_4_1"
							"\nuser_4_1 user_5_1"
							"\nuser_5_1 user_1_1");
	Friends friends;
	parcer prc(isst);
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_FALSE(prc.get_names(friends));
}
TEST_F(TestParcing, parce_k4)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_1_1 user_2_2"
							"\nuser_1_1 user_1_2"
							"\nuser_1_2 user_2_1"
							"\nuser_1_2 user_2_2"
							"\nuser_2_1 user_2_2");

	Friends friends;
	parcer prc(isst);
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_FALSE(prc.get_names(friends));
}
TEST_F(TestParcing, parce_k3)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_1_1 user_1_2"
							"\nuser_1_2 user_2_1");

	Friends friends;
	parcer prc(isst);
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_TRUE(prc.get_names(friends));
	ASSERT_FALSE(prc.get_names(friends));
}

class TestAdvising : public ::testing::Test {
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
TEST_F(TestAdvising, advise_k3)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_1_1 user_1_2"
							"\nuser_1_2 user_2_1");

	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_1_1", 0.5);

	ASSERT_TRUE(fofriends.size() == 0);
}
TEST_F(TestAdvising, advise_k4)
{
	std::istringstream isst( "user_1_1 user_2_1"
							"\nuser_1_1 user_2_2"
							"\nuser_1_1 user_1_2"
							"\nuser_1_2 user_2_1"
							"\nuser_1_2 user_2_2"
							"\nuser_2_1 user_2_2");

	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_1_1", 0.5);

	ASSERT_TRUE(fofriends.size() == 0);
}
TEST_F(TestAdvising, advise_k33)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_1_1 user_2_2"
							"\nuser_1_1 user_2_3"
							"\nuser_1_2 user_2_1"
							"\nuser_1_2 user_2_2"
							"\nuser_1_2 user_2_3"
							"\nuser_1_3 user_2_1"
							"\nuser_1_3 user_2_2"
							"\nuser_1_3 user_2_3");

	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_1_1", 1);

	ASSERT_TRUE(fofriends.size() == 2);
	ASSERT_TRUE(fofriends.at(0) == "user_1_2" || fofriends.at(1) == "user_1_2");
	ASSERT_TRUE(fofriends.at(0) == "user_1_3" || fofriends.at(1) == "user_1_3");
}
TEST_F(TestAdvising, advise_l5)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_2_1 user_3_1"
							"\nuser_3_1 user_4_1"
							"\nuser_4_1 user_5_1"
							"\nuser_5_1 user_6_1");

	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_1_1", 1);

	ASSERT_TRUE(fofriends.size() == 1);
	ASSERT_TRUE(fofriends.at(0) == "user_3_1");
}
TEST_F(TestAdvising, advise_c5)
{
	std::istringstream isst("user_1_1 user_2_1"
							"\nuser_2_1 user_3_1"
							"\nuser_3_1 user_4_1"
							"\nuser_4_1 user_5_1"
							"\nuser_5_1 user_1_1");
	
	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_1_1", 0.1);

	ASSERT_TRUE(fofriends.size() == 2);
	ASSERT_TRUE(fofriends.at(0) == "user_3_1" || fofriends.at(1) == "user_3_1");
	ASSERT_TRUE(fofriends.at(0) == "user_4_1" || fofriends.at(1) == "user_4_1");
}
TEST_F(TestAdvising, advise_g6543210)
{
	std::istringstream isst("user_0_0 user_1_1"
							"\nuser_0_0 user_1_2"
							"\nuser_0_0 user_1_3"
							"\nuser_0_0 user_1_4"
							"\nuser_0_0 user_1_5"
							"\nuser_0_0 user_1_6"
							""
							"\nuser_1_1 user_2_1"
							"\nuser_1_1 user_2_2"
							"\nuser_1_1 user_2_3"
							"\nuser_1_1 user_2_4"
							"\nuser_1_1 user_2_5"
							"\nuser_1_1 user_2_6"
							""
							"\nuser_1_2 user_2_1"
							"\nuser_1_2 user_2_2"
							"\nuser_1_2 user_2_3"
							"\nuser_1_2 user_2_4"
							"\nuser_1_2 user_2_5"
							""
							"\nuser_1_3 user_2_1"
							"\nuser_1_3 user_2_2"
							"\nuser_1_3 user_2_3"
							"\nuser_1_3 user_2_4"
							""
							"\nuser_1_4 user_2_1"
							"\nuser_1_4 user_2_2"
							"\nuser_1_4 user_2_3"
							""
							"\nuser_1_5 user_2_1"
							"\nuser_1_5 user_2_2"
							"\nuser_1_6 user_2_1");
	
	parcer prc(isst);
	friend_adviser f_adv(prc);

	std::vector<std::string> fofriends;
	fofriends = f_adv.get_new_friends("user_0_0", 0.8);
	ASSERT_TRUE(fofriends.size() == 2);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
