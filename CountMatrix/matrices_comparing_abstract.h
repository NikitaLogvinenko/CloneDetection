#pragma once
#include "matrices_comparing_result.h"
#include "bipartite_matching_abstract.h"

namespace cm
{
	template <matrices_comparing_traits CompTraits>
	class matrices_comparing_abstract
	{
	protected:
		constexpr matrices_comparing_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(matrices_comparing_abstract)

		[[nodiscard]] virtual matrices_comparing_result<CompTraits> operator()(
			const count_matrix<CompTraits::count_vector_length>& first_matrix,
			const count_matrix<CompTraits::count_vector_length>& second_matrix) const = 0;
	};
}
