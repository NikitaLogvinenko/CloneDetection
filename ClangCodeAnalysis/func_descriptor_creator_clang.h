#pragma once
#include "func_descriptor.h"
#include "code_entity_location_creator.h"
#include "code_entity_spelling_creator.h"
#include "cursor_classifier.h"
#include "wrong_cursor_kind_error.h"
#include "func_descriptor_creator_abstract.h"
#include "cursors_storage_threadsafe.h"

namespace clang_code_analysis
{
	class func_descriptor_creator_clang final : public code_analysis::func_descriptor_creator_abstract
	{
	public:
		func_descriptor_creator_clang() = delete;

		[[nodiscard]] static code_analysis::func_descriptor create(const CXCursor& cursor_to_func_decl)
		{
			if (!cursor_classifier::is_func_declaration(cursor_to_func_decl))
			{
				throw clang_c_adaptation::wrong_cursor_kind_error(
					"func_descriptor_creator_clang::create: cursor does not represent any kind of function declaration.");
			}

			code_analysis::code_entity_location location = code_entity_location_creator::create(cursor_to_func_decl);
			code_analysis::code_entity_spelling spelling = code_entity_spelling_creator::create(cursor_to_func_decl);

			code_analysis::code_entity_descriptor descriptor{ std::move(spelling), std::move(location) };
			return code_analysis::func_descriptor{ std::move(descriptor) };
		}

		[[nodiscard]] code_analysis::func_descriptor create(const code_analysis::func_id id) const override
		{
			const CXCursor& cursor = cursors_storage_threadsafe<code_analysis::func_id>::get_instance().at(id);
			return create(cursor);
		}
	};
}
