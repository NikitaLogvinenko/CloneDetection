#pragma once
#include "count_matrix.h"
#include "cm_similarity_result.h"


template <size_t CountVectorDimension>
class cm_abstract_similarity
{
public:
	virtual ~cm_abstract_similarity() = default;

	[[nodiscard]] virtual cm_similarity_result calc_similarity(
		const count_matrix<CountVectorDimension>& first_cm, 
		const count_matrix<CountVectorDimension>& second_cm) const = 0;
};