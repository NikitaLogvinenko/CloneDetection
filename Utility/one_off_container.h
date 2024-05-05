#pragma once
#include "invalid_index_error.h"
#include <queue>
#include <list>

namespace utility
{
	template <std::move_constructible T>
	class one_off_container final
	{
		std::queue<T, std::deque<T>> elements_{};

	public:
		one_off_container() = default;

		explicit one_off_container(std::vector<T> elements)
		{
			for (auto& element : elements)
			{
				elements_.emplace(std::move(element));
			}
		}

		[[nodiscard]] bool empty() const
		{
			return elements_.empty();
		}

		[[nodiscard]] T pop_front()
		{
			if (elements_.empty())
			{
				throw common_exceptions::invalid_index_error(
					"one_off_container<T>::pop_front: container is empty.");
			}

			T front_element = std::move(elements_.front());
			elements_.pop();
			return front_element;
		}
	};
}
