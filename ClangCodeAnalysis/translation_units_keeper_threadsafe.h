#pragma once
#include "translation_unit_raii.h"
#include <shared_mutex>
#include <unordered_set>

namespace clang_code_analysis
{
	class translation_units_keeper_threadsafe final
	{
		std::unordered_set<std::shared_ptr<clang_c_adaptation::translation_unit_raii>> translation_units_{};
		std::mutex mutex_{};

	public:
		[[nodiscard]] static translation_units_keeper_threadsafe& get_instance()
		{
			static translation_units_keeper_threadsafe instance{};
			return instance;
		}

		void insert(std::shared_ptr<clang_c_adaptation::translation_unit_raii> translation_unit)
		{
			std::lock_guard guard{ mutex_ };
			translation_units_.emplace(std::move(translation_unit));
		}

	private:
		translation_units_keeper_threadsafe() = default;
	};
}