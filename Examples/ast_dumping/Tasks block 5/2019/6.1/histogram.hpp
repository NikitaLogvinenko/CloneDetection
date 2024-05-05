#pragma once
#include <vector>
#include <map>
	class Histogram {
	public:
		typedef std::map<std::string, int>::const_iterator hist_iterator;
		Histogram() {};
		Histogram(std::istream& stream);
		Histogram(std::vector<std::string>& book);
		Histogram(const Histogram& hist);
		~Histogram();
		const std::map<const std::string, int>& get_hist();
		friend  Histogram operator+(const Histogram& left, const Histogram& right);
		friend  Histogram operator-(const Histogram& left, const Histogram& right);
		friend bool operator==(const Histogram& left, const Histogram& right);
		void operator+=(const Histogram& right);
		void operator-=(const Histogram& right);

		void add_el(const std::string& element);

		hist_iterator begin() { return vocab.cbegin(); };

		hist_iterator end()   { return vocab.cend(); };
	private:
		std::map<const std::string, int> vocab;
		void init(std::vector<std::string>& book);
	};


