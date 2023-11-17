#pragma once
#include <concepts>
#include "relative_similarity.h"
namespace cm
{
	template <class T>
	concept similarity_concept = 
		std::is_default_constructible_v<T> &&
		std::is_constructible_v<T, double> &&
		std::is_copy_constructible_v<T> &&
		requires (T t, const T t_const)
	{
		{ T::min_similarity_value() } -> std::same_as<double>;
		{ T::max_similarity_value() } -> std::same_as<double>;
		{ t.to_double() } -> std::same_as<double>;
		{ t_const.to_double() } -> std::same_as<double>;
	};
}