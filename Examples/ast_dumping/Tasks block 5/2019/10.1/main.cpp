#include "..\str_parser\stdafx.h"
#include <fstream>
#include "..\str_parser\srt_parser.h"
#include "gtest/gtest.h"
#include <string>
#include <algorithm>


static const double ACCURACY = 0.000001;

bool operator != (SubTitle a, SubTitle b)
{
	bool res = (abs(a.end_time - b.end_time) < ACCURACY) && (abs(a.start_time - b.start_time)) < ACCURACY; /*пришлось ввести точность сравнени€ double'ов (врем€) вот так,
																										   а то когда счЄт на милисекунды они хран€тс€ как 0.00700000000303 например*/
	bool res2 = a.all_not_hide_phrases.size() == b.all_not_hide_phrases.size()
		&& a.new_phrase.size() == b.new_phrase.size();
	if (res && res2)
	{
		std::vector<std::string>::const_iterator a_it, b_it;


		for (a_it = a.all_not_hide_phrases.begin(), b_it = b.all_not_hide_phrases.begin();
			a_it != a.all_not_hide_phrases.end() && b_it != b.all_not_hide_phrases.end(); ++a_it, ++b_it)
		{
			bool res = a_it != a.all_not_hide_phrases.end() && b_it != b.all_not_hide_phrases.begin();
			if (*a_it != *b_it)
			{
				return true;
			}
		}


		for (a_it = a.new_phrase.begin(), b_it = b.new_phrase.begin();
			a_it != a.new_phrase.end(), b_it != b.new_phrase.end(); ++a_it, ++b_it)
		{
			if (*a_it != *b_it)
			{
				return true;
			}
		}
		return false;
	}
	else
		return true;
}

static size_t count_size(const SrtParser& parser)
{
	size_t size = 0;
	for (SrtParser::iterator it = parser.begin(); it != parser.end(); ++it, ++size) {  }
	return size;
}

bool if_eq(std::vector<SubTitle> expect_v, const SrtParser& parser)
{
	if (count_size(parser) != expect_v.size())
	{
		return false;
	}
	SrtParser::iterator expect_it = expect_v.begin();
	for (SrtParser::iterator it = parser.begin(); it != parser.end() && expect_it != expect_v.end(); ++it, ++expect_it)
	{
		if (*it != *expect_it)
		{
			return false;
		}
	}
	return true;
}

bool sub_test_get_data(std::string folder_dir, std::vector<SubTitle> expect_v)
{
	std::string out_dir, expect_dir, in_dir;
	std::ifstream input(folder_dir);
	SrtParser parser(input); //конструируем парсер - сбор данных из файла и сборка последовательности команд
	if (count_size(parser) != expect_v.size())
	{
		return false;
	}
	try
	{
		SrtParser::iterator expect_it = expect_v.begin();
		for (SrtParser::iterator it = parser.begin(); it != parser.end() && expect_it != expect_v.end(); ++it, ++expect_it)
		{
			if (*it != *expect_it)
			{
				return false;
			}
		}
		return true;
	}
	catch (NotStandartDocumentException)
	{
		throw "caught_not_stand";
	}
	catch (IncorrectTimeException)
	{
		throw "caught wrong time";
	}
	return true;
}

TEST(REGULAR, regular_test_1)
{
	SubTitle sub = { { "вышел зайчик погул€ть" },{ "Show" }, 10, 15.5 };
	SubTitle sub2 = { {},{ "Hide" }, 15.5, 0 }; //дл€ хайд необ€зательно хранить саб - это просто показать '' - пустоту. ≈сли есть активные сабы - они продолжат показыватьс€
	std::vector<SubTitle> v = { sub, sub2 };
	EXPECT_TRUE(sub_test_get_data("test_1\\in.txt", v));
}

TEST(REGULAR, Show_hide_chain)
{
	SubTitle sub = { { "вышел зайчик погул€ть" },{ "Show" }, 10, 15.5f };
	SubTitle sub2 = { {},{ "Hide" }, 15.5f, 0 };
	SubTitle sub3 = { { "1" },{ "Show" }, 15.7f, 18.9f };
	SubTitle sub4 = { {},{ "Hide" }, 18.9f, 0 };

	std::vector<SubTitle> v = { sub, sub2, sub3, sub4 };
	EXPECT_TRUE(sub_test_get_data("test_2\\in.txt", v));
}

TEST(REGULAR, sub_in_sub)
{
	SubTitle sub = { { "1" },{ "Show" }, 10, 12 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 12, 14 };
	SubTitle sub3 = { { "1" },{ "Show" }, 14, 15 };
	SubTitle sub4 = { {},{ "Hide" },15, 0 };

	std::vector<SubTitle> v = { sub, sub2, sub3, sub4 };
	EXPECT_TRUE(sub_test_get_data("test_3\\in.txt", v));
}
static bool is_gap(const char a)
{
	return a == '\t';
}
TEST(STRSTREAM_CONSTRUCT, regular)
{
	std::string str =
		"1								\n"
		"00:00:01,500 --> 00:00:07,200	\n"
		"1								\n"
		"								\n"
		"2								\n"
		"00:00:07,200 --> 00:00:07,400	\n"
		"2								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'),str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 1.5f, 7.2f };
	SubTitle sub2 = { { "2" },{ "Show" }, 7.2f, 7.4f };
	SubTitle sub3 = { {},{ "Hide" }, 7.4f, 0 };
	std::vector<SubTitle> expect = { sub1, sub2, sub3 };
	EXPECT_TRUE(if_eq(expect, parser));
}

TEST(STRSTREAM_CONSTRUCT, cross_sub)
{
	std::string str =
		"1								\n"
		"00:00:01,000 --> 00:00:03,000	\n"
		"1								\n"
		"								\n"
		"2								\n"
		"00:00:02,000 --> 00:00:04,000	\n"
		"2								\n"
		"								\n"
		"3								\n"
		"00:00:03,000 --> 00:00:05,000	\n"
		"3								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 1, 2 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 2, 3 };
	SubTitle sub3 = { { "2", "3" },{ "Show" }, 3, 4 };
	SubTitle sub4 = { { "3" },{ "Show" }, 4, 5 };
	SubTitle sub5 = { {},{ "Hide" }, 5, 0 };
	std::vector<SubTitle> expect = { sub1, sub2, sub3, sub4, sub5 };
	EXPECT_TRUE(if_eq(expect, parser));
}
/*с часами, с минутами, + врем€ 00 00 001*/

TEST(STRSTREAM_CONSTRUCT, cross_sub_minutes_included)
{
	std::string str =
		"1								\n"
		"00:05:01,000 --> 00:06:03,000	\n"
		"1								\n"
		"								\n"
		"2								\n"
		"00:05:02,000 --> 00:07:04,000	\n"
		"2								\n"
		"								\n"
		"3								\n"
		"00:07:03,000 --> 00:08:05,000	\n"
		"3								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 301, 302 }; //в секундах
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 302, 363 };
	SubTitle sub3 = { { "2" },{ "Show" }, 363, 423 };
	SubTitle sub4 = { { "2", "3" },{ "Show" }, 423, 424 };
	SubTitle sub5 = { { "3" },{ "Show" }, 424, 485 };
	SubTitle sub6 = { {},{ "Hide" }, 485, 0 };
	std::vector<SubTitle> expect = { sub1, sub2, sub3, sub4, sub5, sub6 };
	EXPECT_TRUE(if_eq(expect, parser));
}

TEST(STRSTREAM_CONSTRUCT, cross_sub_hrs_mins_included)
{
	std::string str =
		"1								\n"
		"01:05:01,000 --> 01:06:03,000	\n"
		"1								\n"
		"								\n"
		"2								\n"
		"01:05:02,000 --> 01:07:04,000	\n"
		"2								\n"
		"								\n"
		"3								\n"
		"01:07:03,000 --> 01:08:05,000	\n"
		"3								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 3901, 3902 }; //в секундах
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 3902, 3963 };
	SubTitle sub3 = { { "2" },{ "Show" }, 3963, 4023 };
	SubTitle sub4 = { { "2", "3" },{ "Show" }, 4023, 4024 };
	SubTitle sub5 = { { "3" },{ "Show" }, 4024, 4085 };
	SubTitle sub6 = { {},{ "Hide" }, 4085, 0 };
	std::vector<SubTitle> expect = { sub1, sub2, sub3, sub4, sub5, sub6 };
	EXPECT_TRUE(if_eq(expect, parser));
}

TEST(STRSTREAM_CONSTRUCT, sub_milisec_support)
{
	std::string str =
		"1								\n"
		"00:00:00,003 --> 00:00:00,007	\n"
		"1								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 0.003f, 0.007f }; //в секундах
	SubTitle sub6 = { {},{ "Hide" }, 0.007f, 0.0f };
	std::vector<SubTitle> expect = { sub1, sub6 };
	EXPECT_TRUE(if_eq(expect, parser));
}

TEST(STRSTREAM_CONSTRUCT, incorrect_sec)
{
	std::string str =
		"1								\n"
		"00:00:00,003 --> 00:00:5,007	\n"
		"1								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	EXPECT_THROW(SrtParser parser(ist), NotStandartDocumentException);
}


TEST(STRSTREAM_CONSTRUCT, sub_milisec_support_2)
{
	std::string str =
		"1								\n"
		"00:00:00,3 --> 00:00:00,7		\n"
		"1								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 0.3f, 0.7f }; //в секундах
	SubTitle sub6 = { {},{ "Hide" }, 0.7f, 0.0f };
	std::vector<SubTitle> expect = { sub1, sub6 };
	EXPECT_TRUE(if_eq(expect, parser));
}

TEST(STRSTREAM_CONSTRUCT, hrs_milisec_support)
{
	std::string str =
		"1								\n"
		"02:10:00,3 --> 02:12:00,723	\n"
		"1								\n"
		"								\n"
		"2								\n"
		"03:06:00,03 --> 03:08:00,075	\n"
		"2								\n"
		"								\n"
		"3								\n"
		"10:00:00,3 --> 10:00:00,7		\n"
		"3								\n"
		"								\n";
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	std::istringstream ist(str);
	SrtParser parser(ist);
	SubTitle sub1 = { { "1" },{ "Show" }, 7800.3f, 7920.723f }; //в секундах
	SubTitle sub2 = { {},{ "Hide" }, 7920.723f, 0.0f };
	SubTitle sub3 = { { "2" },{ "Show" }, 11160.03f, 11280.075f }; //в секундах
	SubTitle sub4 = { {},{ "Hide" }, 11280.075f, 0.0f };
	SubTitle sub5 = { { "3" },{ "Show" }, 36000.3f, 36000.7f }; //в секундах
	SubTitle sub6 = { {},{ "Hide" }, 36000.7f, 0.0f };
	std::vector<SubTitle> expect = { sub1, sub2, sub3, sub4, sub5, sub6 };
	EXPECT_TRUE(if_eq(expect, parser));
}




TEST(REGULAR, sub_in_sub_in_sub)
{
	SubTitle sub = { { "1" },{ "Show" }, 10, 11 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 11, 12 };
	SubTitle sub3 = { { "1", "2", "3" },{ "Show" }, 12, 14 };
	SubTitle sub4 = { { "1", "2", },{ "Show" }, 14, 15 };
	SubTitle sub5 = { { "1" },{ "Show" }, 15, 16 };
	SubTitle sub6 = { {},{ "Hide" }, 16, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4, sub5, sub6 };

	EXPECT_TRUE(sub_test_get_data("test_4\\in.txt", v));
}

TEST(REGULAR, equal_range) //когда впритык показывать фразы - от 1 до 2, потом от 2 до 3Єх другую - хайдить не должен
{
	SubTitle sub = { { "1" },{ "Show" },10.5f, 15.5f };
	SubTitle sub2 = { { "2" },{ "Show" }, 15.5f, 17.25f };
	SubTitle sub3 = { { "3" },{ "Show" }, 17.25f, 19.34f };
	SubTitle sub4 = { {},{ "Hide" }, 19.34f, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4 };

	EXPECT_TRUE(sub_test_get_data("test_15\\in.txt", v));
}


TEST(REGULAR, cross_sub)
{
	SubTitle sub = { { "1" },{ "Show" }, 10, 13 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 13, 16 };
	SubTitle sub3 = { { "2" },{ "Show" }, 16, 18 };
	SubTitle sub4 = { {},{ "Hide" }, 18, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4 };

	EXPECT_TRUE(sub_test_get_data("test_5\\in.txt", v));
}

TEST(REGULAR, cross_and_overlap)
{
	SubTitle sub = { { "1" },{ "Show" }, 1, 2 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 2, 4 };
	SubTitle sub3 = { { "1", "2", "3" },{ "Show" }, 4, 5 };
	SubTitle sub4 = { { "1", "3", "4" },{ "Show" }, 5, 6 };
	SubTitle sub5 = { { "1", "4" },{ "Show" }, 6, 6.5 };
	SubTitle sub6 = { { "1" },{ "Show" }, 6.5, 7 };
	SubTitle sub7 = { {},{ "Hide" }, 7, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4, sub5, sub6, sub7 };

	EXPECT_TRUE(sub_test_get_data("test_21\\in.txt", v));
}

TEST(REGULAR, stupid_times)
{
	SubTitle sub = { { "1" },{ "Show" }, 1.752f, 2.242f };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 2.242f, 4.111f };
	SubTitle sub3 = { { "1", "2", "3" },{ "Show" }, 4.111f, 5.999f };
	SubTitle sub4 = { { "1", "3", "4" },{ "Show" }, 5.999f, 6.235f };
	SubTitle sub5 = { { "1", "4" },{ "Show" }, 6.235f, 6.785f };
	SubTitle sub6 = { { "1" },{ "Show" }, 6.785f, 7.241f };
	SubTitle sub7 = { {},{ "Hide" }, 7.241f, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4, sub5, sub6, sub7 };

	EXPECT_TRUE(sub_test_get_data("test_22\\in.txt", v));
}

TEST(REGULAR, multiple_cross_sub)
{
	SubTitle sub = { { "1" },{ "Show" }, 10, 12 };
	SubTitle sub2 = { { "1", "2" },{ "Show" }, 12, 13 };
	SubTitle sub3 = { { "1", "2", "3" },{ "Show" }, 13, 15 };
	SubTitle sub4 = { { "2", "3" },{ "Show" }, 15, 17 };
	SubTitle sub5 = { { "3" },{ "Show" }, 17, 19.5 };
	SubTitle sub6 = { {},{ "Hide" }, 19.5, 0 };
	std::vector<SubTitle> v = { sub, sub2, sub3, sub4, sub5, sub6 };

	EXPECT_TRUE(sub_test_get_data("test_9\\in.txt", v));
}

TEST(BAD_FILE, no_infile_is_OK)
{
	std::vector<SubTitle> v; /*при конструировании с потока, отсутствие файла -->
							 везде возвращаютс€ вектора нулевого размера, этот сценарий нормально проходит,
							 записи в файл не происходит*/
	EXPECT_TRUE(sub_test_get_data("test_6\\in.txt", v));
}

TEST(BAD_FILE, no_file_in_the_directory)
{
	/*имитируем сравнение, думали, файл открылс€*/
	SubTitle sub1 = { { "3" },{ "Show" }, 17, 19.5 };
	SubTitle sub2 = { {},{ "Hide" }, 19.5, 0 };
	std::vector<SubTitle> v = { sub1, sub2 };
	EXPECT_FALSE(sub_test_get_data("test_23\\in.txt", v));
}

TEST(BAD_FILE, no_folder_in_the_directory)
{
	/*имитируем сравнение, думали, файл открылс€*/
	SubTitle sub1 = { { "3" },{ "Show" }, 17, 19.5 };
	SubTitle sub2 = { {},{ "Hide" }, 19.5, 0 };
	std::vector<SubTitle> v = { sub1, sub2 };
	EXPECT_FALSE(sub_test_get_data("test_special\\in.txt", v));
}

TEST(BAD_FILE, no_outfile_OK)
{
	std::vector<SubTitle> v;
	EXPECT_TRUE(sub_test_get_data("test_7\\in.txt", v));
}

TEST(BAD_FILE, has_timeline_no_sub_OK) //после таймлайна нет даже ENTER, пустота - чтобы корректно сработало
{
	std::vector<SubTitle> v;
	SubTitle sub = { { "–аз", "два", "три" },{ "Show" }, 1, 5.5 };
	SubTitle sub2 = { {},{ "Hide" }, 5.5, 0 };
	SubTitle sub3 = { {},{ "Show" }, 6, 7.5 };
	SubTitle sub4 = { {},{ "Hide" }, 7.5, 0 };
	v = { sub, sub2, sub3, sub4 };
	EXPECT_TRUE(sub_test_get_data("test_18\\in.txt", v)); /*по месту пустой строки просто надпись Show, дальше пустота - OK*/
}

TEST(BAD_FILE, has_timeline_empty_sub_OK) //после таймлайна ENTER - чтобы корректно сработало
{
	std::vector<SubTitle> v;
	SubTitle sub = { { "–аз", "два", "три" },{ "Show" }, 1, 5.5 };
	SubTitle sub2 = { {},{ "Hide" }, 5.5, 0 };
	SubTitle sub3 = { {},{ "Show" }, 6, 7.5 };
	SubTitle sub4 = { {},{ "Hide" }, 7.5, 0 };
	v = { sub, sub2, sub3, sub4 };
	EXPECT_TRUE(sub_test_get_data("test_19\\in.txt", v)); /*по месту пустой строки просто надпись Show, дальше пустота - OK*/
}

TEST(BAD_FILE, has_timeline_empty_sub_among_other_subs) //пустой суб среди остальных
{
	std::vector<SubTitle> v;
	SubTitle sub = { { "one" },{ "Show" }, 2, 3 };
	SubTitle sub2 = { {},{ "Show" }, 3, 4 };
	SubTitle sub3 = { { "three" },{ "Show" }, 4, 5 };
	SubTitle sub4 = { {},{ "Hide" }, 5, 0 };
	v = { sub, sub2, sub3, sub4 };
	EXPECT_TRUE(sub_test_get_data("test_20\\in.txt", v)); /*по месту пустой строки просто надпись Show, дальше пустота - OK*/
}


TEST(BAD_FILE, empty)
{
	std::vector<SubTitle> v;
	EXPECT_TRUE(sub_test_get_data("test_10\\in.txt", v));
}

TEST(NOT_STANDARD, end_time_less_start_time)
{
	std::vector<SubTitle> v;
	EXPECT_THROW(sub_test_get_data(("test_11\\in.txt"), v), IncorrectTimeException);
}

TEST(NOT_STANDARD, sec_more_than_sixty)
{
	std::vector<SubTitle> v;
	EXPECT_THROW(sub_test_get_data(("test_12\\in.txt"), v), IncorrectTimeException);
}

TEST(NOT_STANDARD, min_more_than_sixty)
{
	std::vector<SubTitle> v;
	EXPECT_THROW(sub_test_get_data(("test_13\\in.txt"), v), IncorrectTimeException);
}

TEST(NOT_STANDARD, not_standart_arrow)
{
	std::vector<SubTitle> v;
	EXPECT_THROW(sub_test_get_data(("test_14\\in.txt"), v), NotStandartDocumentException);
}

TEST(REFERENCE, input_from_oop)
{
	std::string out_dir = "test_16\\out.txt";
	std::string in_dir = "test_16\\in.txt";

	std::ifstream input(in_dir);
	std::ofstream out(out_dir);
	SrtParser parser(input);
	std::vector <std::pair<float, std::vector<std::string>>>v;
	v = parser.make_out_file(out);
	EXPECT_TRUE(v.size());
}

/*TEST(LARGE_FILE, cinema)
{
std::string out_dir = "test_17\\out.txt";
std::string in_dir = "test_17\\subs.srt";

std::ifstream input(in_dir);
std::ofstream out(out_dir);
SrtParser parser(input);
std::vector <std::pair<float, std::vector<std::string>>>v;
v = parser.make_out_file(out);
EXPECT_TRUE(v.size());
}*/

//int main(int argc, char **argv)
//{
//	::testing::InitGoogleTest(&argc, argv);
//	
//	int res = RUN_ALL_TESTS();
//	getchar();
//	return res;;
//}