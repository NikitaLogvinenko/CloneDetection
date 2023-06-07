#pragma once
#include "translation_unit_wrapper.h"
#include "func_implementation_info_hash.h"
#include "func_implementation_info_equal_spelling_and_location.h"
#include <unordered_set>

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class functions_analysis_builder_abstract
	{
	protected:
		class analysed_functions_info final
		{
			class const_iterator final
				: public std::vector<func_implementation_info<VarUsageConditionsN>>::const_iterator
			{
			public:
				explicit const_iterator(const typename std::vector<func_implementation_info<VarUsageConditionsN>>::const_iterator& vector_iterator) noexcept
					: std::vector<func_implementation_info<VarUsageConditionsN>>::const_iterator(vector_iterator) {}
			};

			std::vector<func_implementation_info<VarUsageConditionsN>> functions_info_{};

		public:
			analysed_functions_info() = default;
			explicit analysed_functions_info(
				const std::unordered_set<func_implementation_info<VarUsageConditionsN>,
				func_implementation_info_hash<VarUsageConditionsN>, 
				func_implementation_info_equal_spelling_and_location<VarUsageConditionsN>>& functions_info)
				: functions_info_(functions_info.cbegin(), functions_info.cend()) {}

			[[nodiscard]] size_t functions_count() const noexcept
			{
				return functions_info_.size();
			}

			[[nodiscard]] const func_implementation_info<VarUsageConditionsN>& operator[](const size_t func_info_index) const noexcept
			{
				return functions_info_[func_info_index];
			}

			[[nodiscard]] const func_implementation_info<VarUsageConditionsN>& at(const size_t func_info_index) const
			{
				return functions_info_.at(func_info_index);
			}

			[[nodiscard]] auto begin() const noexcept
			{
				return const_iterator(functions_info_.cbegin());
			}

			[[nodiscard]] auto end() const noexcept
			{
				return const_iterator(functions_info_.cend());
			}
		};

	public:
		virtual ~functions_analysis_builder_abstract() = default;

		virtual void analyse_functions(const clang_c_adaptation::translation_unit_wrapper& translation_unit) = 0;
		virtual analysed_functions_info build_analysed_functions_implementation_info() = 0;
	};
}