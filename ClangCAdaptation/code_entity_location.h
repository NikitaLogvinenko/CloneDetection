#pragma once
#include "clang_c_types_handling.h"
#include "clang-c/Index.h"
#include <filesystem>

namespace clang_c_adaptation
{
	class code_entity_location
	{
		std::filesystem::path filename_{ "unknown" };
		unsigned line_{};
		unsigned column_{};
		unsigned offset_{};
		bool is_valid_{ false };

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
			clang_getFileLocation(location, &file, &line_, &column_, &offset_);
			filename_ = std::filesystem::path(clang_c_types_handling::cxstring_to_string(std::make_unique<CXString>(clang_getFileName(file))));
			is_valid_ = true;
		}

		virtual ~code_entity_location() = default;

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

		[[nodiscard]] unsigned offset() const noexcept
		{
			return offset_;
		}

		[[nodiscard]] bool is_valid() const noexcept
		{
			return is_valid_;
		}

		[[nodiscard]] bool operator==(const code_entity_location& other) const noexcept = default;
	};
}
