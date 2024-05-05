#include "pch.h"
#include "interpolation_by_neighborhood_with_weight.h"

namespace
{
	class neighborhood_any_half_side final : interpolation_by_neighborhood_with_weight
	{
	protected:
		[[nodiscard]] double weight(double distance_along_axis) const noexcept override
		{
			return 0;
		}
	public:
		explicit neighborhood_any_half_side(const double neighborhood_half_side) : interpolation_by_neighborhood_with_weight(neighborhood_half_side) {}
	};
}

TEST(interpolation_by_neighborhood_with_weight_ctor, negative_neighborhood_half_side)
{
	ASSERT_THROW(neighborhood_any_half_side{ -1 }, std::invalid_argument);
}

TEST(interpolation_by_neighborhood_with_weight_ctor, zero_neighborhood_half_side)
{
	ASSERT_THROW(neighborhood_any_half_side{ 0 }, std::invalid_argument);
}

TEST(interpolation_by_neighborhood_with_weight_ctor, little_neighborhood_half_side)
{
	ASSERT_THROW(neighborhood_any_half_side{ 0.3 }, std::invalid_argument);
}

TEST(interpolation_by_neighborhood_with_weight_ctor, valid_neighborhood_half_side)
{
	ASSERT_NO_THROW(neighborhood_any_half_side{ 0.5 });
	ASSERT_NO_THROW(neighborhood_any_half_side{ 1 });
	ASSERT_NO_THROW(neighborhood_any_half_side{ 100 });
}

