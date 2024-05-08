#pragma once

class main_executor final
{
public:
	main_executor() = delete;
	static int execute(int argc, const char** argv);
};
