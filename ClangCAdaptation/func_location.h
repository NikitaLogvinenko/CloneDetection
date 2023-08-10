#pragma once
#include "code_entity_location.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"

namespace clang_c_adaptation
{
	class func_location final
	{
		internal::code_entity_location location_{};

	public:
		func_location() noexcept = default;
		explicit func_location(const CXCursor& cursor_to_func_decl) : location_(cursor_to_func_decl)
		{
			if (!common_checks::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw wrong_cursor_kind_exception(
					"func_location: cursor does not represent any kind of function declaration");
			}
		}

		[[nodiscard]] const std::filesystem::path& filename() const noexcept
		{
			return location_.filename();
		}

		[[nodiscard]] unsigned line() const noexcept
		{
			return location_.line();
		}

		[[nodiscard]] unsigned column() const noexcept
		{
			return location_.column();
		}

		[[nodiscard]] unsigned offset_from_file_start() const noexcept
		{
			return location_.offset_from_file_start();
		}

		[[nodiscard]] bool is_valid() const noexcept
		{
			return location_.is_valid();
		}

		[[nodiscard]] bool operator==(const func_location& other) const noexcept = default;
	};
}