﻿#pragma once
#include "clang-c/Index.h"
#include "cxstring_raii.h"
#include <filesystem>

namespace clang_c_adaptation::internal
{
	class code_entity_location final
	{
		std::filesystem::path filename_{};
		unsigned line_{};
		unsigned column_{};
		unsigned offset_from_file_start_{};

		inline static const CXSourceRange null_range = clang_getNullRange();

	public:
		code_entity_location() noexcept = default;
		explicit code_entity_location(const CXCursor& cursor)
		{
			const CXSourceRange range = clang_getCursorExtent(cursor);
			if (clang_equalRanges(range, null_range))
			{
				return;
			}
			const CXSourceLocation location = clang_getRangeStart(range);
			CXFile file;
			clang_getFileLocation(location, &file, &line_, &column_, &offset_from_file_start_);
			filename_ = std::filesystem::path(cxstring_raii(clang_getFileName(file)).c_str());
		}

		[[nodiscard]] const std::filesystem::path& filename() const noexcept
		{
			return filename_;
		}

		[[nodiscard]] unsigned line() const noexcept
		{
			return line_;
		}

		[[nodiscard]] unsigned column() const noexcept
		{
			return column_;
		}

		[[nodiscard]] unsigned offset_from_file_start() const noexcept
		{
			return offset_from_file_start_;
		}

		[[nodiscard]] bool is_valid() const noexcept
		{
			return !filename_.empty();
		}

		[[nodiscard]] bool operator==(const code_entity_location& other) const noexcept = default;
	};
}
