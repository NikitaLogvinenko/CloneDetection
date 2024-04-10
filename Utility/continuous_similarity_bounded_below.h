#pragma once
#include <concepts>

namespace utility
{
	template <class T>
	concept continuous_similarity_bounded_below =
		std::is_nothrow_default_constructible_v<T> &&
		std::is_nothrow_copy_constructible_v<T> &&
		std::is_constructible_v<T, double> &&
		requires (T t, const T t_const)
	{
		{ T::min_similarity_value() } noexcept -> std::same_as<double>;
		{ t.to_double() } noexcept -> std::same_as<double>;
		{ t_const.to_double() } noexcept -> std::same_as<double>;
	};
}
