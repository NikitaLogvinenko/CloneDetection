#pragma once
#include "clang-c/Index.h"
#include "cxstring_raii.h"
#include "code_entity_location.h"

namespace clang_code_analysis
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
				return {};
			}

			const CXSourceLocation location = clang_getRangeStart(range);
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset_from_file_start;

			clang_getFileLocation(location, &file, &line, &column, &offset_from_file_start);

			const auto filename_raii = clang_c_adaptation::cxstring_raii(clang_getFileName(file));
			std::string filename_str{};
			if (filename_raii.is_valid())
			{
				filename_str = clang_c_adaptation::cxstring_raii(clang_getFileName(file)).string();
			}

			std::filesystem::path filename{ filename_str };

			const code_analysis::symbol_position first_symbol_position{ line, column, offset_from_file_start };
			return code_analysis::code_entity_location{ std::move(filename), first_symbol_position };
		}
	};
}
