﻿#pragma once
#include "clang-c/Index.h"
#include "cxstring_raii.h"
#include "code_entity_location.h"
#include "clang_range_exception.h"

namespace clang_c_adaptation
{
	class code_entity_location_creator final
	{
	public:
		code_entity_location_creator() = delete;

		[[nodiscard]] static code_analysis::code_entity_location create(const CXCursor& cursor)
		{
			const CXSourceRange range = clang_getCursorExtent(cursor);
			if (clang_Range_isNull(range))
			{
				throw clang_range_exception("code_entity_location_creator::create: range of cursor is null.");
			}

			const CXSourceLocation location = clang_getRangeStart(range);
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset_from_file_start;

			clang_getFileLocation(location, &file, &line, &column, &offset_from_file_start);
			const std::string filename_str = internal::cxstring_raii(clang_getFileName(file)).string();
			std::filesystem::path filename{ filename_str };

			const code_analysis::symbol_position first_symbol_position{ line, column, offset_from_file_start };
			return code_analysis::code_entity_location{ std::move(filename), first_symbol_position };
		}
	};
}
