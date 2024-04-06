#pragma once
#include "id_concept.h"
#include <atomic>

namespace utility
{
	template <id_concept IdT>
	class unique_id_generator_threadsafe final
	{
		inline static std::atomic_size_t id_{};

	public:
		unique_id_generator_threadsafe() = delete;

		[[nodiscard]] static IdT generate() noexcept(std::is_nothrow_constructible_v<IdT, size_t>)
		{
			return IdT{ ++id_ };
		}
	};
}
