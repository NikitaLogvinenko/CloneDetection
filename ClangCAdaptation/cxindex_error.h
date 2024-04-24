#pragma once
#include <stdexcept>

namespace clang_c_adaptation
{
	class cxindex_error final : public std::runtime_error
	{
	public:
		explicit cxindex_error(const std::string_view what_arg)
			: std::runtime_error(what_arg.data()) {}
	};
}
