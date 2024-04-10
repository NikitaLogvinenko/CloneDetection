#pragma once
#include "id_concept.h"
#include "id_hash.h"
#include "clang-c/Index.h"
#include "unique_id_generator_threadsafe.h"
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include <shared_mutex>
#include <unordered_map>

namespace clang_code_analysis
{
	template <utility::id_concept IdT>
	class cursors_storage_threadsafe final
	{
		std::unordered_map<IdT, CXCursor, utility::id_hash<IdT>> cursor_by_id_{};
		std::unordered_map<CXCursor, IdT, clang_c_adaptation::cxcursor_hash, clang_c_adaptation::cxcursors_equal> id_by_cursor_{};
		mutable std::shared_mutex cursor_by_id_mutex_{};
		mutable std::shared_mutex id_by_cursor_mutex_{};

	public:
		[[nodiscard]] static cursors_storage_threadsafe& get_instance()
		{
			static cursors_storage_threadsafe instance{};
			return instance;
		}

		IdT insert(const CXCursor& cursor)
		{
			{
				std::shared_lock id_by_cursor_lock{ id_by_cursor_mutex_ };
				if (id_by_cursor_.contains(cursor))
				{
					return id_by_cursor_.at(cursor);
				}
			}

			std::unique_lock cursor_by_id_lock{ cursor_by_id_mutex_ };
			std::unique_lock id_by_cursor_lock{ id_by_cursor_mutex_ };
			if (id_by_cursor_.contains(cursor))
			{
				return id_by_cursor_.at(cursor);
			}
			
			const IdT id = utility::unique_id_generator_threadsafe<IdT>::generate();
			id_by_cursor_.try_emplace(cursor, id);
			cursor_by_id_.try_emplace(id, cursor);

			return id;
		}

		[[nodiscard]] const CXCursor& at(const IdT id) const
		{
			std::shared_lock lock{ cursor_by_id_mutex_ };
			return cursor_by_id_.at(id);
		}

	private:
		cursors_storage_threadsafe() = default;
	};
}