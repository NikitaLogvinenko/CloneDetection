#pragma once
#include "code_entity_location.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"

namespace clang_c_adaptation
{
	class var_location final
	{
		internal::code_entity_location location_{};

	public:
		var_location() noexcept = default;
		explicit var_location(const CXCursor& cursor_to_var_decl) : location_(cursor_to_var_decl)
		{
			if (!common_checks::is_cursor_to_var_decl(cursor_to_var_decl))
			{
				throw wrong_cursor_kind_exception(
					"var_location: cursor does not represent any kind of variables");
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

		[[nodiscard]] bool operator==(const var_location& other) const noexcept = default;
	};
}