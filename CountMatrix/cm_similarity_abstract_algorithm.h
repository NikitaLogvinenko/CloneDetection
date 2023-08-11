#pragma once
#include "count_matrices_similarity_data.h"
#include "default_copy_move_constructor_assignment_virtual_destructor.h"

namespace cm
{
	template <size_t CountVectorDimension>
	class cm_similarity_abstract_algorithm
	{
	public:
		cm_similarity_abstract_algorithm() noexcept = default;

		DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR(cm_similarity_abstract_algorithm)

		[[nodiscard]] virtual count_matrices_similarity_data determine_similarity(
			const count_matrix<CountVectorDimension>& first_cm,
			const count_matrix<CountVectorDimension>& second_cm) const = 0;
	};
}
