#ifndef SRTPARSER_H
#define SRTPARSER_H
#include <iostream>
#include <vector>

struct SubTitle
{
	std::vector<std::string> all_not_hide_phrases;
	std::vector<std::string> new_phrase;
	float start_time;
	float end_time;
};

class NotStandartDocumentException : public std::exception
{
public:
	NotStandartDocumentException() : std::exception("Not standart STD input doc") {}
};

class IncorrectTimeException : public std::exception
{
public:
	IncorrectTimeException() : std::exception("Incorrect time, end_time < start_time for sub") {}
};

class SrtParser
{
private:
	static const std::string SHOW_CMD;
	static const std::string HIDE_CMD;
	std::vector<SubTitle> subs;

	void hide_phrases();
	std::vector<SubTitle> find_active_subs(std::pair<float, float> gap);
	std::vector<SubTitle> put_in_order();
	std::string make_time_hh_mm_ss(float &time);

	bool correct_arrow(const std::string& str) const;
	std::vector <SubTitle> active_subs(float time); //ищет, если наш субтитр окончен, есть ли на этот момент другие активные сабы - если нет --> команда Hide доабвляем
	std::vector<float>  times(const std::string& str) const; //парсинг строки типа 00:00:15,12

public:
	SrtParser() {}
	SrtParser(std::istream &in);
	~SrtParser() {}
	size_t get_data(std::istream &in);
	std::vector<std::pair<float, std::vector<std::string>>> make_out_file(std::ostream &out);

	using iterator = std::vector<SubTitle>::const_iterator;
	iterator begin() const; //возможность итерировать по сабам для тестов, да и вообще
	iterator end() const;
};
#endif // SRTPARSER_H