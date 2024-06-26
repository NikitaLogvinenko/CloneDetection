﻿#pragma once
#include "var_descriptor.h"
#include "func_descriptor_creator_abstract.h"
#include "cursor_classifier.h"
#include "wrong_cursor_kind_error.h"
#include "var_descriptor_creator_abstract.h"
#include "cursors_storage_threadsafe.h"

namespace clang_code_analysis
{
	class var_descriptor_creator_clang final : public code_analysis::var_descriptor_creator_abstract
	{
	public:
		[[nodiscard]] static code_analysis::var_descriptor create(const CXCursor& cursor_to_var_decl)
		{
			if (!cursor_classifier::is_var_declaration(cursor_to_var_decl))
			{
				throw clang_c_adaptation::wrong_cursor_kind_error(
					"var_descriptor_creator_clang::create: cursor does not represent any kind of variables.");
			}

			return code_analysis::var_descriptor{ code_entity_descriptor_creator::create(cursor_to_var_decl) };
		}

		[[nodiscard]] code_analysis::var_descriptor create(const code_analysis::var_id id) const override
		{
			const CXCursor& cursor = cursors_storage_threadsafe<code_analysis::var_id>::get_instance().at(id);
			return create(cursor);
		}
	};
}
