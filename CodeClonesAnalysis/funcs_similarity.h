#pragma once
#include "continuous_similarity_bounded_below.h"

namespace code_clones_analysis
{
	template <utility::continuous_similarity_bounded_below SimilarityT>
	class funcs_similarity final
	{
		SimilarityT similarity_{};

	public:
		constexpr funcs_similarity() noexcept = default;

		constexpr explicit funcs_similarity(const SimilarityT similarity) noexcept
			: similarity_(similarity) {}

		[[nodiscard]] constexpr SimilarityT to_similarity_t() const noexcept
		{
			return similarity_;
		}
	};
}
