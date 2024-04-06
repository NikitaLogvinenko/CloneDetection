#pragma once
#include "var_descriptor.h"
#include "code_entity_location_creator.h"
#include "code_entity_spelling_creator.h"

namespace clang_code_analysis
{
	class code_entity_descriptor_creator final
	{
	public:
		code_entity_descriptor_creator() = delete;

		[[nodiscard]] static code_analysis::code_entity_descriptor create(const CXCursor& cursor)
		{
			code_analysis::code_entity_location location = code_entity_location_creator::create(cursor_to_var_decl);
			code_analysis::code_entity_spelling spelling = code_entity_spelling_creator::create(cursor_to_var_decl);

			code_analysis::code_entity_descriptor descriptor{ std::move(spelling), std::move(location) };
			return descriptor;
		}
	};
}
