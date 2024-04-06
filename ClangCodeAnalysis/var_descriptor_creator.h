#pragma once
#include "var_descriptor.h"
#include "code_entity_location_creator.h"
#include "code_entity_spelling_creator.h"
#include "cursor_classifier.h"
#include "wrong_cursor_kind_error.h"

namespace clang_code_analysis
{
	class var_descriptor_creator final
	{
	public:
		var_descriptor_creator() = delete;

		[[nodiscard]] static code_analysis::var_descriptor create(const CXCursor& cursor_to_var_decl)
		{
			if (!cursor_classifier::is_var_declaration(cursor_to_var_decl))
			{
				throw clang_c_adaptation::wrong_cursor_kind_error(
					"var_descriptor_creator::create: cursor does not represent any kind of variables.");
			}

			code_analysis::code_entity_location location = code_entity_location_creator::create(cursor_to_var_decl);
			code_analysis::code_entity_spelling spelling = code_entity_spelling_creator::create(cursor_to_var_decl);

			code_analysis::code_entity_descriptor descriptor{ std::move(spelling), std::move(location) };
			return code_analysis::var_descriptor{ std::move(descriptor) };
		}
	};
}
