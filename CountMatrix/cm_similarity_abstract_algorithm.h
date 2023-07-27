#pragma once
#include "count_matrices_similarity_data.h"

namespace count_matrix
{
	template <size_t CountVectorDimension>
	class cm_similarity_abstract_algorithm
	{
	public:
		cm_similarity_abstract_algorithm() noexcept = default;

		cm_similarity_abstract_algorithm(const cm_similarity_abstract_algorithm& other) noexcept = default;
		cm_similarity_abstract_algorithm(cm_similarity_abstract_algorithm&& other) noexcept = default;
		cm_similarity_abstract_algorithm& operator=(const cm_similarity_abstract_algorithm& other) noexcept = default;
		cm_similarity_abstract_algorithm& operator=(cm_similarity_abstract_algorithm&& other) noexcept = default;
		virtual ~cm_similarity_abstract_algorithm() = default;

		[[nodiscard]] virtual count_matrices_similarity_data determine_similarity(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const = 0;
	};
}
