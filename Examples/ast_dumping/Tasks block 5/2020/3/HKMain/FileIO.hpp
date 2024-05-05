#pragma once
#include <string>

class FileIO final
{
private:
	std::string inputFirstFileName_;
	std::string inputSecondFileName_;
	std::string outputFileName_;
public:
	FileIO(int argc, char* argv[])
	{
		if (argc > 3)
		{
			inputFirstFileName_ = argv[1];
			inputSecondFileName_ = argv[2];
			outputFileName_ = argv[3];
		}
		else if (argc > 2)
		{
			inputFirstFileName_ = argv[1];
			inputSecondFileName_ = argv[2];
		}
		else if (argc > 1)
		{
			inputFirstFileName_ = argv[1];
		}
	}

	std::string getFirstInputName() const
	{
		return inputFirstFileName_;
	}

	std::string getSecondInputName() const
	{
		return inputSecondFileName_;
	}

	std::string getOutputName() const
	{
		return outputFileName_;
	}

	std::istream& setFirstInStream(std::ifstream& fin) const;
	std::istream& setSecondInStream(std::ifstream& fin) const;
	std::ostream& setOutStream(std::ofstream& fout) const;
};
