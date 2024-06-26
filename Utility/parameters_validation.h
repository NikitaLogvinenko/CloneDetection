﻿#pragma once
#include "nullptr_error.h"
#include "param_out_of_range_error.h"
#include "file_not_opened_error.h"
#include <format>
#include <filesystem>

namespace utility
{
	template <class T>
	void throw_if_nullptr(const T* const ptr, const std::string& func_name, const std::string& ptr_name)
	{
		
		if (ptr == nullptr)
		{
			throw common_exceptions::nullptr_error(std::format("{}: {} is nullptr.", 
				func_name, ptr_name));
		}
	}

	inline void throw_if_wrong_threads_count(const size_t threads_count, const size_t max_threads_count,
		const std::string& func_name, const std::string& param_name)
	{
		if (threads_count == 0)
		{
			throw common_exceptions::param_out_of_range_error(std::format(
				"{}: {} can not be 0.", func_name, param_name));
		}

		if (threads_count > max_threads_count)
		{
			throw common_exceptions::param_out_of_range_error(std::format(
				"{}: {} can not be greater than {}.", func_name, param_name, std::to_string(max_threads_count)));
		}
	}

	template <class Exception, class CollectionT>
	requires std::is_constructible_v<Exception, std::string>
	void throw_if_not_enough_elements(const CollectionT& collection, const size_t minimum_elements, 
		const std::string& collection_name, const std::string& func_name)
	{
		if (collection.size() < minimum_elements)
		{
			throw Exception(std::format("{}: minimum {} element(s) is(are) required but {} was(were) provided in {}.",
				func_name, minimum_elements, collection.size(), collection_name));
		}
	}

	template <class Exception> requires std::is_constructible_v<Exception, std::string>
	void throw_if_nonexistent(const std::filesystem::path& path, const std::string& func_name)
	{
		if (!exists(path))
		{
			throw Exception(std::format("{}: \"{}\" does not exist.", func_name, path.string()));
		}
	}

	template <class Exception> requires std::is_constructible_v<Exception, std::string>
	void throw_if_nonexistent_directory(const std::filesystem::path& dir, const std::string& func_name)
	{
		throw_if_nonexistent<Exception>(dir, func_name);

		if (!is_directory(dir))
		{
			throw Exception(std::format("{}: \"{}\" is not a directory.", func_name, dir.string()));
		}
	}

	template <class FileT>
	void throw_if_not_open(const FileT& file, const std::filesystem::path& filename, const std::string& func_name)
	{
		if (!file.is_open())
		{
			throw common_exceptions::file_not_opened_error{ std::format(
				"{}: can not open file \"{}\".", func_name, filename.string()) };
		}
	}

	template <class T, class Exception> requires std::is_constructible_v<Exception, std::string>
	T convert_argument(const std::string& token, std::string exception_description)
	{
		std::istringstream iss(token);
		T argument;
		iss >> argument;

		if (!iss)
		{
			throw Exception(exception_description);
		}

		return argument;
	}
}
