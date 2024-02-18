#pragma once
#include "matrices_similarity.h"

namespace funcs_comparing_through_cm
{
	template <cm::continuous_similarity_bounded_below SimilarityT>
	class funcs_similarity final
	{
		cm::matrices_similarity<SimilarityT> cm_similarity_{};

	public:
		constexpr funcs_similarity() noexcept = default;

		constexpr explicit funcs_similarity(const cm::matrices_similarity<SimilarityT> cm_similarity) noexcept
			: cm_similarity_(cm_similarity) {}

		[[nodiscard]] constexpr cm::matrices_similarity<SimilarityT> to_matrices_similarity() const noexcept
		{
			return cm_similarity_;
		}
	};
}
