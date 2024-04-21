#include "command_line_args_raii.h"

clang_c_adaptation::command_line_args_raii::command_line_args_raii(const std::vector<std::string>& args)
{
	if (args.empty())
	{
		return;
	}

	argc_ = args.size();

	argv_ = new char* [argc_];

	try
	{
		for (size_t argument_i = 0; argument_i < argc_; ++argument_i)
		{
			const size_t argument_size = args[argument_i].size() + 1;
			argv_[argument_i] = new char[argument_size];
			strcpy_s(argv_[argument_i], argument_size, args[argument_i].c_str());
		}
	}
	catch (const std::exception&)
	{
		free();
		throw;
	}
}

void clang_c_adaptation::command_line_args_raii::free() const
{
	for (size_t argument_i = 0; argument_i < argc_; ++argument_i)
	{
		delete argv_[argument_i];
	}

	delete argv_;
}
