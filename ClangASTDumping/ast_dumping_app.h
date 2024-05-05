#pragma once

namespace clang_ast_dumping
{
	class ast_dumping_app final
	{
	public:
		ast_dumping_app() = delete;

		static void exec(int argc, const char* const* argv);
	};
}
