#pragma once
#include "relative_similarity.h"
#include "funcs_analysis_traits.h"
#include "code_entities_comparing_traits.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	struct funcs_pairwise_comparing_traits final
	{
		funcs_pairwise_comparing_traits() = delete;

		struct matrices_comparing_traits final
		{
			matrices_comparing_traits() = delete;

			static constexpr size_t count_vector_length = ConditionsCount;

			using distance_t = double;

			using similarity_t = utility::relative_similarity;
		};


		using code_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
	};
}