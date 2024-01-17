#pragma once
#include "nullptr_exception.h"
#include "initialized_value_out_of_range.h"
#include <memory>
#include <format>

namespace utility
{
	template <class T>
	void throw_if_nullptr(std::unique_ptr<T> ptr, const std::string& func_name, const std::string& ptr_name)
	{
		if (ptr == nullptr)
		{
			throw common_exceptions::nullptr_exception(std::format("{}: {} is nullptr.", 
				func_name, ptr_name));
		}
	}

	inline void throw_if_wrong_threads_count(const size_t threads_count, const size_t max_threads_count,
		const std::string& func_name, const std::string& param_name)
	{
		if (threads_count == 0)
		{
			throw common_exceptions::initialized_value_out_of_range(std::format(
				"{}: {} can not be 0.", func_name, param_name));
		}

		if (threads_count > max_threads_count)
		{
			throw common_exceptions::initialized_value_out_of_range(std::format(
				"{}: {} can not be greater than {}.", func_name, param_name, std::to_string(max_threads_count)));
		}
	}
}
