﻿#pragma once
#include "funcs_pair_comparing_result.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <ostream>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_pair_comparing_result_saver_abstract
	{
	protected:
		funcs_pair_comparing_result_saver_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_pair_comparing_result_saver_abstract)

	public:
		virtual void save(std::ostream& output, const funcs_pair_comparing_result<ConditionsCount>& result) const = 0;
	};
}