#include "funcs_definitions_finder.h"
#include "common_checks.h"
#include "cursors_storage_threadsafe.h"

namespace clang_funcs_analysis
{
	namespace
	{
		using func_id = code_analysis::func_id;

		// ReSharper disable twice CppPassValueParameterByConstReference
		CXChildVisitResult visitor_finding_funcs_definitions(const CXCursor current_cursor, const CXCursor,
		                                                     const CXClientData funcs_ids_vector)
		{
			if (clang_Location_isInSystemHeader(clang_getCursorLocation(current_cursor)))
			{
				return CXChildVisit_Continue;
			}

			if (clang_c_adaptation::common_checks::is_cursor_to_func_definition(current_cursor))
			{
				const auto id = cursors_storage_threadsafe<func_id>::get_instance().insert(current_cursor);
				static_cast<std::vector<func_id>* const>(funcs_ids_vector)->push_back(id);
			}

			clang_visitChildren(current_cursor, visitor_finding_funcs_definitions, funcs_ids_vector);

			return CXChildVisit_Continue;
		}
	}

	std::vector<func_id> funcs_definitions_finder::find_nested(const CXCursor& root_cursor)
	{
		std::vector<func_id> funcs_ids{};
		clang_visitChildren(root_cursor, visitor_finding_funcs_definitions, &funcs_ids);
		return funcs_ids;
	}
}
