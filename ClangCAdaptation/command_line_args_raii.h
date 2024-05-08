#pragma once
#include "copying_delete.h"
#include <string>
#include <vector>

namespace clang_c_adaptation
{
	class command_line_args_raii final
	{
		size_t argc_{};
		char** argv_{};

	public:
		explicit command_line_args_raii(const std::vector<std::string>& args);

		COPYING_DELETE(command_line_args_raii)

		command_line_args_raii(command_line_args_raii&& other) noexcept
		{
			std::swap(argc_, other.argc_);
			std::swap(argv_, other.argv_);
		}

		command_line_args_raii& operator=(command_line_args_raii&& other) noexcept
		{
			std::swap(argc_, other.argc_);
			std::swap(argv_, other.argv_);
			return *this;
		}

		~command_line_args_raii()
		{
			free();
		}

		[[nodiscard]] size_t argc() const noexcept
		{
			return argc_;
		}

		[[nodiscard]] const char* const* argv() const noexcept
		{
			return argv_;
		}

	private:
		void free() const;
	};
}
