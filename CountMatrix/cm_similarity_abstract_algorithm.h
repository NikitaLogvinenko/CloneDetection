#pragma once
#include "count_matrices_similarity_data.h"

namespace count_matrix_ns
{
	template <size_t CountVectorDimension>
	class cm_similarity_abstract_algorithm
	{
	public:
		virtual ~cm_similarity_abstract_algorithm() = default;

		[[nodiscard]] virtual count_matrices_similarity_data determine_similarity(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const = 0;
	};
}
