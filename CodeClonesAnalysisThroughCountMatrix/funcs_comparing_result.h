#pragma once
#include "matrices_comparing_result.h"
#include "funcs_comparing_traits.h"
#include "funcs_similarity.h"
#include "var_index.h"
#include "matched_vars_index.h"
#include "distance_between_vars.h"

namespace code_clones_analysis_through_cm
{
	template <funcs_comparing_traits CompTraits>
	class funcs_comparing_result final
	{
		using similarity_t = typename CompTraits::similarity_t;
		using distance_t = typename CompTraits::distance_t;

		cm::matrices_comparing_result<CompTraits> cm_comparing_result_{};

	public:
		constexpr funcs_comparing_result() noexcept = default;

		constexpr explicit funcs_comparing_result(const cm::matrices_comparing_result<CompTraits> cm_comparing_result)
		noexcept : cm_comparing_result_(std::move(cm_comparing_result)) {}

		[[nodiscard]] constexpr funcs_similarity<similarity_t> similarity() const noexcept
		{
			return funcs_similarity<similarity_t>{ cm_comparing_result_.similarity() };
		}

		[[nodiscard]] constexpr size_t matched_variables_count() const noexcept
		{
			return cm_comparing_result_.matched_vectors_count();
		}

		[[nodiscard]] constexpr var_index var_from_first_cm(const matched_vars_index matching_index)
		{
			const cm::matched_vectors_index matched_vectors_index{ matching_index.to_size_t() };
			const cm::count_vector_index cv_from_first_cm = cm_comparing_result_.vector_from_first_cm(matched_vectors_index);

			return var_index{ cv_from_first_cm.to_size_t() };
		}

		[[nodiscard]] constexpr var_index var_from_second_cm(const matched_vars_index matching_index)
		{
			const cm::matched_vectors_index matched_vectors_index{ matching_index.to_size_t() };
			const cm::count_vector_index cv_from_second_cm = cm_comparing_result_.vector_from_second_cm(matched_vectors_index);

			return var_index{ cv_from_second_cm.to_size_t() };
		}

		[[nodiscard]] constexpr distance_between_vars<distance_t> distance_between_matched_vectors(
			const matched_vars_index matching_index)
		{
			const cm::matched_vectors_index matched_vectors_index{ matching_index.to_size_t() };
			const cm::distance_between_count_vectors<distance_t> cv_distance =
				cm_comparing_result_.distance_between_matched_vectors(matched_vectors_index);

			return distance_between_vars<distance_t>{ cv_distance };
		}
	};
}
