#include "funcs_definitions_finder.h"
#include "common_checks.h"

namespace clang_funcs_analysis
{
	namespace
	{
		// ReSharper disable twice CppPassValueParameterByConstReference
		CXChildVisitResult visitor_finding_funcs_definitions(const CXCursor current_cursor, const CXCursor,
		                                                     const CXClientData funcs_definitions_vector_void_ptr)
		{
			if (clang_Location_isInSystemHeader(clang_getCursorLocation(current_cursor)))
			{
				return CXChildVisit_Continue;
			}



			if (clang_c_adaptation::common_checks::is_cursor_to_func_definition(current_cursor))
			{
				static_cast<std::vector<CXCursor>* const>(funcs_definitions_vector_void_ptr)->push_back(current_cursor);
			}

			else
			{
				clang_visitChildren(current_cursor, visitor_finding_funcs_definitions, funcs_definitions_vector_void_ptr);
			}

			return CXChildVisit_Continue;
		}
	}

	std::vector<CXCursor> funcs_definitions_finder::find_nested(const CXCursor& root_cursor)
	{
		std::vector<CXCursor> funcs_definitions_cursors{};
		clang_visitChildren(root_cursor, visitor_finding_funcs_definitions, &funcs_definitions_cursors);
		return funcs_definitions_cursors;
	}
}
