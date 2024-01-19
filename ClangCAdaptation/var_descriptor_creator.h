#pragma once
#include "var_descriptor.h"
#include "code_entity_location_creator.h"
#include "code_entity_spelling_creator.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"

namespace clang_c_adaptation
{
	class var_descriptor_creator final
	{
	public:
		var_descriptor_creator() = delete;

		[[nodiscard]] static code_analysis::var_descriptor create(const CXCursor& cursor_to_var_decl)
		{
			if (!common_checks::is_cursor_to_var_decl(cursor_to_var_decl))
			{
				throw wrong_cursor_kind_exception(
					"var_descriptor_creator::create: cursor does not represent any kind of variables.");
			}

			code_analysis::code_entity_location location = code_entity_location_creator::create(cursor_to_var_decl);
			code_analysis::code_entity_spelling spelling = code_entity_spelling_creator::create(cursor_to_var_decl);

			code_analysis::code_entity_descriptor descriptor{ std::move(spelling), std::move(location) };
			return code_analysis::var_descriptor{ std::move(descriptor) };
		}
	};
}
