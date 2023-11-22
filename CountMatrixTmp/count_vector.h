#pragma once
#include "count_vector_dimension.h"
#include "counted_value.h"
#include "index_of_counted_value.h"
#include "invalid_iterator_operation.h"
#include "incorrect_vector_length.h"
#include <vector>
#include <string>

namespace cm
{
	template <size_t Dimension> requires count_vector_dimension<Dimension>
	class count_vector final
	{
		class iterator final
		{
			using underlying_iterator = std::vector<counted_value>::const_iterator;

		public:
			using difference_type = long long;
			using value_type = std::iterator_traits<underlying_iterator>::value_type;
			using pointer = std::iterator_traits<underlying_iterator>::pointer;
			using reference = std::iterator_traits<underlying_iterator>::reference;
			using iterator_category = std::random_access_iterator_tag;

		private:
			underlying_iterator iter_{};
			size_t index_{};
			bool from_default_count_vector_{};

		public:
			constexpr iterator() noexcept = default;
			constexpr explicit iterator(underlying_iterator iter, const size_t index, 
				const bool from_default_count_vector = false) noexcept
			: iter_(std::move(iter)), index_(index), from_default_count_vector_(from_default_count_vector) {}

			[[nodiscard]] reference operator*() const
			{
				throw_if_end_it( "count_vector::iterator: can not dereference end iterator");
				return *iter_;
			}

			[[nodiscard]] const underlying_iterator& operator->() const
			{
				return iter_;
			}

			[[nodiscard]] const auto& operator[](const difference_type n) const
			{
				return *this->operator+(n);
			}

			iterator& operator++()
			{
				return *this += 1;
			}

			iterator operator++(int)
			{
				const iterator tmp(*this);
				++*this;
				return tmp;
			}

			iterator& operator--()
			{
				return *this -= 1;
			}

			iterator operator--(int)
			{
				const iterator tmp(*this);
				--*this;
				return tmp;
			}

			[[nodiscard]] iterator operator+(const difference_type n) const
			{
				iterator new_it(*this);
				shift_with_validation(new_it, n);
				return new_it;
			}

			[[nodiscard]] iterator operator-(const difference_type n) const
			{
				iterator new_it(*this);
				shift_with_validation(new_it, -n);
				return new_it;
			}

			[[nodiscard]] friend iterator operator+(const difference_type n, const iterator& it)
			{
				return it + n;
			}

			iterator& operator+=(const difference_type n)
			{
				shift_with_validation(*this, n);
				return *this;
			}

			iterator& operator-=(const difference_type n)
			{
				shift_with_validation(*this, -n);
				return *this;
			}

			[[nodiscard]] difference_type operator-(const iterator& other) const noexcept
			{
				return index_ - other.index_;
			}

			[[nodiscard]] bool operator==(const iterator& other) const noexcept
			{
				return index_ == other.index_;
			}

			[[nodiscard]] bool operator!=(const iterator& other) const noexcept
			{
				return index_ != other.index_;
			}

			[[nodiscard]] auto operator<=>(const iterator& other) const
			{
				return index_ <=> other.index_;
			}

		private:
			[[nodiscard]] difference_type get_changed_index_with_validation(const difference_type increase_by) const
			{
				const difference_type changed_index = index_ + increase_by;
				throw_if_invalid_index(changed_index, "count_vector::iterator: attempt to get iterator out of range");
				return changed_index;
			}

			void throw_if_end_it(const std::string& msg) const
			{
				if (index_ == Dimension)
				{
					throw common_exceptions::invalid_iterator_operation(msg);
				}
			}

			static void throw_if_invalid_index(const difference_type index, const std::string& msg)
			{
				if (index < 0 || index > Dimension)
				{
					throw common_exceptions::invalid_iterator_operation(msg);
				}
			}

			static void shift_with_validation(iterator& it, const difference_type shift_by)
			{
				it.index_ = it.get_changed_index_with_validation(shift_by);
				if (it.from_default_count_vector_)
				{
					return;
				}

				it.iter_ += shift_by;
			}
		};


		std::vector<counted_value> counted_values_{std::vector<count_vector>(default_count_vector_size)};

		static constexpr size_t default_count_vector_size = 1;

	public:
		constexpr count_vector() = default;

		constexpr explicit count_vector(std::vector<counted_value> counted_values) : counted_values_(std::move(counted_values))
		{
			if (counted_values_.size() != Dimension)
			{
				throw common_exceptions::incorrect_vector_length("count_vector: counted_values.size() must be equal to Dimension.");
			}
		}

		[[nodiscard]] constexpr const counted_value& at(const index_of_counted_value value_index) const
		{
			return *(begin() + value_index);
		}

		[[nodiscard]] constexpr auto begin() const noexcept
		{
			return iterator{ counted_values_.cbegin(), 0, is_default() };
		}

		[[nodiscard]] constexpr auto end() const noexcept
		{
			return iterator{ counted_values_.cend(), Dimension, is_default() };
		}

		[[nodiscard]] static constexpr size_t size() noexcept
		{
			return Dimension;
		}

	private:
		[[nodiscard]] constexpr bool is_default() const noexcept
		{
			return Dimension != default_count_vector_size && counted_values_.size() == default_count_vector_size;
		}
	};
}
