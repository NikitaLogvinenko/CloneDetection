#pragma once
#include "code_entity_location.h"
#include "common_checks.h"
#include <stdexcept>

namespace clang_c_adaptation
{
	class func_location final : public code_entity_location
	{
		inline static const std::string not_func_msg{"func_location: cursor does not represent any kind of function declaration"};

	public:
		func_location() noexcept = default;
		explicit func_location(const CXCursor& cursor_to_func_decl) : code_entity_location(cursor_to_func_decl)
		{
			if (!common_checks::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw std::invalid_argument(not_func_msg);
			}
		}

		[[nodiscard]] bool operator==(const func_location& other) const noexcept = default;
	};
}