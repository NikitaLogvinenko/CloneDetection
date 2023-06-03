#pragma once
#include <stdexcept>

class cm_similar_vectors final
{
	size_t vector_from_first_cm_{};
	size_t vector_from_second_cm_{};
	double similarity_{};

	inline static const std::string wrong_similarity_msg{"Vectors similarity is relative value and must be from 0 (absolutely different vectors) to 1 (equal vectors)"};

public:
	cm_similar_vectors() noexcept = default;
	explicit cm_similar_vectors(const size_t vector_from_first_cm, const size_t vector_from_second_cm, const double similarity)
		: vector_from_first_cm_(vector_from_first_cm), vector_from_second_cm_(vector_from_second_cm), similarity_(similarity)
	{
		if (similarity < 0 || similarity > 1)
		{
			throw std::range_error(wrong_similarity_msg);
		}
	}

	[[nodiscard]] size_t vector_from_first_cm() const noexcept
	{
		return vector_from_first_cm_;
	}

	[[nodiscard]] size_t vector_from_second_cm() const noexcept
	{
		return vector_from_second_cm_;
	}

	[[nodiscard]] double similarity() const noexcept
	{
		return similarity_;
	}
};