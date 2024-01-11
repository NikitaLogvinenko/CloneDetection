#pragma once

namespace cm
{
	constexpr size_t max_count_vector_length = 100;

	template <size_t Length>
	concept count_vector_length = Length != 0 && Length <= max_count_vector_length;
}
