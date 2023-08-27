#pragma once
#include <stdexcept>

namespace clang_c_adaptation
{
	class create_cxindex_exception final : std::runtime_error
	{
	public:
		explicit create_cxindex_exception(const std::string_view what_arg)
			: std::runtime_error(what_arg.data()) {}
	};
}
