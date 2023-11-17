#pragma once
#include <stdexcept>

namespace common_exceptions
{
	class incorrect_vector_length final : std::invalid_argument
	{
	public:
		explicit incorrect_vector_length(const std::string& what_arg) : std::invalid_argument(what_arg) {}
	};
}
