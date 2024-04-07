﻿#pragma once
#include "func_descriptor.h"
#include "code_entity_location_creator.h"
#include "code_entity_spelling_creator.h"
#include "cursor_classifier.h"
#include "wrong_cursor_kind_error.h"

namespace clang_code_analysis
{
	class func_descriptor_creator final
	{
	public:
		func_descriptor_creator() = delete;

		[[nodiscard]] static code_analysis::func_descriptor create(const CXCursor& cursor_to_func_decl)
		{
			if (!cursor_classifier::is_func_declaration(cursor_to_func_decl))
			{
				throw clang_c_adaptation::wrong_cursor_kind_error(
					"func_descriptor_creator::create: cursor does not represent any kind of function declaration.");
			}

			code_analysis::code_entity_location location = code_entity_location_creator::create(cursor_to_func_decl);
			code_analysis::code_entity_spelling spelling = code_entity_spelling_creator::create(cursor_to_func_decl);

			code_analysis::code_entity_descriptor descriptor{ std::move(spelling), std::move(location) };
			return code_analysis::func_descriptor{ std::move(descriptor) };
		}
	};
}