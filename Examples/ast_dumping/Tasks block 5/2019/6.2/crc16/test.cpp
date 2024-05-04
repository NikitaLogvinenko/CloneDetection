#include "gtest/gtest.h"
#include "..//CRC16/crc16.hpp"
#include <fstream>
using namespace CRC;
std::vector<std::string> test_strings{"123ABCDEFGHI JKLMNOPQRSTUVWXYZ","Protect me from what i want",
"After graduating from Oxford University, he became one of the Liberals' brightest stars in the 1950s",
"As party leader, Thorpe capitalised on the growing unpopularity of the Conservative and Labour parties to lead the Liberals through a period of electoral success",
"This culminated in the general election of February 1974, when the party won 6 million votes",
"In May 1979 he was tried at the Old Bailey on charges of conspiracy and incitement to murder, arising from an earlier relationship with Norman Scott, a former model",
"Thorpe was acquitted on all charges, but the case, and the scandal, ended his political career",
"By the time of his death he was honoured for his record as an internationalist, a supporter of human rights, and an opponent of apartheid and all forms of racism"};
std::vector<unsigned short> sum = {0xD408,0xEEB6,0x1395,0x95E6,0x1A50,0x1795,0xF32D,0x9BE7 };
TEST(CALC_TEST, Calculating) {
	unsigned short crc = 0;
	for (int i = 0; i < sum.size(); i++) {
		std::stringstream ss;
		ss << test_strings[i];
		crc = crc16::calc(ss);
		EXPECT_EQ(crc, sum[i]);
	}
}
