#pragma once
#include <concepts>

namespace cm
{
	template <class T>
	concept continuous_similarity_bounded_below = 
		std::is_default_constructible_v<T> &&
		std::is_constructible_v<T, double> &&
		std::is_copy_constructible_v<T> &&
		requires (T t, const T t_const)
	{
		{ T::min_similarity_value() } -> std::same_as<double>;
		{ t.to_double() } -> std::same_as<double>;
		{ t_const.to_double() } -> std::same_as<double>;
	};
}
