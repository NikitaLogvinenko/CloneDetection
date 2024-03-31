#pragma once
#include "count_vector_length.h"
#include "id_concept.h"

namespace code_analysis_through_cm
{
	template <class Traits>
	concept code_analysis_traits = requires
	{
		typename Traits::analyzed_entity_id;
		requires utility::id_concept<typename Traits::analyzed_entity_id>;

		typename Traits::nested_entity_id;
		requires utility::id_concept<typename Traits::nested_entity_id>;

		Traits::conditions_count;
		requires cm::count_vector_length<Traits::conditions_count>;
	};
}