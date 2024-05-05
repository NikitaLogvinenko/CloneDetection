#pragma once
#include "continuous_similarity_bounded_below.h"

namespace cm
{
	template <utility::continuous_similarity_bounded_below SimilarityT>
	class matrices_similarity final
	{
		SimilarityT similarity_{};

	public:
		constexpr matrices_similarity() noexcept = default;

		constexpr explicit matrices_similarity(const SimilarityT similarity) noexcept
			: similarity_(similarity) {}

		[[nodiscard]] constexpr SimilarityT to_similarity_t() const noexcept
		{
			return similarity_;
		}
	};
}
