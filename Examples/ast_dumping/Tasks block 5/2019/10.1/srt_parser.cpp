// SRT_parser.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "srt_parser.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>


const std::string SrtParser::SHOW_CMD = "Show";
const std::string SrtParser::HIDE_CMD = "Hide";


bool cmp_start_time(SubTitle const &s1, SubTitle const &s2)
{
	return s1.start_time < s2.start_time;
}
bool cmp_hide_phrases(SubTitle const &s1, SubTitle const &s2)
{
	return s1.all_not_hide_phrases.size() > s2.all_not_hide_phrases.size();
}

SrtParser::SrtParser(std::istream &in) //,bool  ordered)
{
	if (get_data(in))
	{
		subs = put_in_order();
	}
}


std::vector<std::string> token_string(const std::string s, const std::string delimers)
{
	std::vector <std::string> numbers;
	for (size_t pos = 0;;)
	{
		pos = s.find_first_not_of(delimers, pos);
		if (pos == std::string::npos)
			break;

		size_t pos1 = s.find_first_of(delimers, pos);
		if (pos1 + 1 == 0)//возвращает (size_t)-1 если не нашёл
			pos1 = s.length();
		size_t len = (pos1 == std::string::npos) ? std::string::npos : pos1 - pos;

		std::string word(s.substr(pos, len));

		if (pos1 == std::string::npos)
			break;

		pos = pos1;
		numbers.push_back((word));
	}
	return numbers;
}

size_t SrtParser::get_data(std::istream &in)
{
	subs.clear();

	std::string str;
	size_t i = 0;

	if (!in.good()) //невалидный файл
	{
		return 0;
	}
	while (!in.eof())
	{

		std::getline(in, str);//чтобы пропустить номер
		std::getline(in, str); //получение временной строки


		if (str.empty())
		{
			return subs.size();
		}
		
		/*парсим строчку на три составляющие - время начала, стрелка и время конца*/
		std::vector <std::string> temp;
		std::istringstream ist(str);
		std::copy(std::istream_iterator<std::string>(ist), {}, std::back_inserter(temp));
		/*      */

		//if(temp.size() != 3) //значит, был ввод из istream'a как из единой строки

		std::vector<float> init_time = times(temp[0]);
		std::vector<float> end_time = times(temp[2]);
		if (!correct_arrow(temp[1]) || !init_time.size() || !end_time.size())
		{
			throw NotStandartDocumentException();
		}

		float start_time = init_time[0] * 3600 + init_time[1] * 60 + (init_time[2] + (float)0.001 * init_time[3]);
		float final_time = end_time[0] * 3600 + end_time[1] * 60 + (end_time[2] + (float)0.001 * end_time[3]);

		if (final_time < start_time)
		{
			throw IncorrectTimeException();
		}

		subs.push_back({ {},{}, start_time, final_time });

		subs[i].all_not_hide_phrases.push_back(SrtParser::SHOW_CMD);

		for (int n = 0; getline(in, str); n++)
		{
			if (str.empty())  //считываем соотв фразу, до пустой строки между блоками субов
			{
				break;
			}
			subs[i].new_phrase.push_back(str);
		}
		i++;
	}
	return subs.size();
}

std::vector<SubTitle> SrtParser::active_subs(float time)
{
	size_t subs_size = subs.size();
	std::vector<SubTitle> v;
	for (const auto it : subs)
	{
		if (it.start_time <= time && time < it.end_time)
		{
			v.push_back(it);
		}
	}
	/*if (count == 1)
	return false; //нет других активных сабов на этот момент времени - придётся хайдить
	else
	return true;*/
	return v;
}

void SrtParser::hide_phrases()
{
	size_t subs_size = subs.size();
	for (size_t k = 0; k<subs_size; ++k)
	{
		float end_time = subs[k].end_time;
		std::vector<SubTitle> active_subs_v = active_subs(end_time);
		if (active_subs_v.size() == 0) //если нету активных сабов в это время, надо хайдить Show ''
		{
			subs.insert(subs.end(), { { SrtParser::HIDE_CMD },{}, end_time, 0 }); //вставляем в конец, потом отсортируем
		}
		else
		{
			//active_subs_v.insert(active_subs_v.begin(), { {SrtParser::SHOW_CMD} });
			//std::copy(active_subs_v.begin(), active_subs_v.end(), subs.end());
			/*если не было активных сабов - хайдим. Если было - добавляем команду Show c активными сабами
			на момент времени конца этого саба*/

			//sort(active_subs_v.begin(), active_subs_v.end(), cmp_hide_phrases);
			/*auto it = active_subs_v.begin();
			it->start_time = end_time;
			subs.push_back(*it);*/
			sort(active_subs_v.begin(), active_subs_v.end(), cmp_hide_phrases);
			auto it = active_subs_v.begin();
			it->start_time = end_time;
			subs.push_back(*it);
			/*for (auto it : active_subs_v)
			{
			it.start_time = end_time;
			subs.push_back(it);
			}*/
		}
	}
}

std::vector<float> get_times(std::vector<SubTitle> subs)
{
	std::vector<SubTitle>::const_iterator it;
	std::vector<float> v;
	for (it = subs.begin(); it != subs.end(); ++it)
	{
		v.push_back(it->start_time);
		v.push_back(it->end_time);
	}
	return v;
}

bool if_bigger(float a, float b)
{
	return a < b;
}

std::vector<SubTitle> SrtParser:: find_active_subs(std::pair<float, float> gap)
{
	std::vector<SubTitle>::const_iterator it;
	std::vector <SubTitle> res;
	for (it = subs.begin(); it != subs.end(); ++it)
	{
		if (it->end_time >= gap.second && it->start_time <= gap.first)
		{
			res.push_back(*it);
		}
	}
	return res;
}

std::vector<SubTitle> SrtParser::put_in_order()
{
	
	std::vector<SubTitle> res_command_line;
	if (!subs.size())
	{
		return res_command_line;
	}
	sort(subs.begin(), subs.end(), cmp_start_time); //отсортировали по времени старта субтитра
	std::vector<float> sub_times = get_times(subs);
	sort(sub_times.begin(), sub_times.end(), if_bigger);

	size_t sub_times_size = sub_times.size();
	std::pair<float, float> gap;
	std::vector<SubTitle> active_subs;
	SubTitle sub;
	for (size_t i = 0; i < sub_times_size - 1; i++)
	{
		size_t next_ind = i + 1;
		gap = { sub_times[i], sub_times[next_ind] };
		if (gap.first != gap.second) /*в частном случае когда сабы "впритык", без этого условия появляются
									лишни*/
		{
			active_subs = find_active_subs(gap);

			if (active_subs.size() != 0)
			{

				for (auto it : active_subs)
				{
					for (auto str_it : it.new_phrase)
					{
						sub.all_not_hide_phrases.push_back(str_it);
					}
				}
				sub.new_phrase = { SrtParser::SHOW_CMD };
				sub.start_time = gap.first;
				sub.end_time = gap.second;
				res_command_line.push_back(sub);
				sub = { {}, {}, 0, 0 };
			}
			else
			{
				sub = { {}, {SrtParser::HIDE_CMD}, gap.first, 0 };
				res_command_line.push_back(sub);
				sub = { {},{}, 0, 0 };
			}
		}
	}
	//ниже - финальный хайд
	sub = { {},{ SrtParser::HIDE_CMD }, gap.second, 0 };
	res_command_line.push_back(sub);
	sort(res_command_line.begin(), res_command_line.end(), cmp_start_time);
	return res_command_line;
}


std::string SrtParser::make_time_hh_mm_ss(float &time)
{
	int hh = int(time) / 3600;
	int mm = int(time - hh * 3600) / 60;
	int sec = (int)(time - hh * 3600 - mm * 60);
	float msec = (float)(time - hh * 3600 - mm * 60 - sec) * 100;

	std::ostringstream str;
	str << hh << ':' << mm << ':' << sec << ',' << msec << ' ';
	return str.str();
}

bool SrtParser::correct_arrow(const std::string & str) const
{
	return (str == "-->");
}


std::vector<float> SrtParser::times(const std::string & str) const
{
	std::vector <float> v;
	std::vector <std::string> times;
	times = token_string(str, ":,\0");
	if (times.size() != 4 || times[1].size()!=2 || times[2].size()!=2)
	{
		//std::vector<float> v; //не по стандарту, больше встретили "," или ":" возврат нуль вектора
		return v;
	}
	else
	{
		//std::vector<float> v;
		while (times[3].length() != 3)
			times[3] += '0'; //чтобы 0,3 было 0,300 0,03 -> 0,030, 0,003 -> 0,003

		for (const auto it : times)
		{
			v.push_back(std::stof(it));
		}
		int hrs = static_cast<int>(v[0]);
		int min = static_cast<int>(v[1]);
		float sec = (v[2]);
		/*ниже - поддержка разной записи милисекунд - если формат 
		00:00:00,003 - 3 милисек
		00:00:00,3 - 300 милисек
		00:00:00,300 - 300 мсек
		00:00:00,03 - 30 мсек*/
		float milisec = v[3] * 0.001f;

		if (hrs >= 0 && min >= 0 && min <= 60 && sec + milisec >= 0 && sec + milisec <= 60)
		{
			return v; //корректные параметры
		}
		else
		{
			throw IncorrectTimeException();
		}
	}
}


std::vector<std::pair<float, std::vector<std::string>>> SrtParser::make_out_file(std::ostream &out)
{
	//subs = put_in_order();
	
	std::vector< std::pair< float, std::vector<std::string> > > result;
	if (!out.good()) //невалидный файл
	{
		return result;
	}
	size_t subs_size = subs.size();

	std::pair<float, std::vector<std::string>> time_command;
	for (size_t k = 0; k < subs_size; k++)
	{
		std::string time = make_time_hh_mm_ss(subs[k].start_time);
		out << "At " << time << std::endl;
		
		if (subs[k].new_phrase[0] == SrtParser::SHOW_CMD)
		{
			out << "Show" << std::endl;
			for (const auto it : subs[k].all_not_hide_phrases)
			{
				out << it << std::endl;
			}
			time_command.first = subs[k].start_time;
			subs[k].new_phrase.insert(subs[k].new_phrase.end(), subs[k].all_not_hide_phrases.begin(), subs[k].all_not_hide_phrases.end());// , std::inserter());
			time_command.second = subs[k].new_phrase;
			result.push_back(time_command);

		}
		else if (subs[k].new_phrase[0] == SrtParser::HIDE_CMD)
		{
			out << SrtParser::SHOW_CMD << " '' " << std::endl;
		}
	}
	return result;
}

SrtParser::iterator SrtParser::begin() const
{
	return subs.cbegin();
}

SrtParser::iterator SrtParser::end() const
{
	return subs.cend();
}

