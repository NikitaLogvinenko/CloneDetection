#pragma once

#include <iterator>

namespace code_generation
{
	template <std::input_iterator UnderlyingIt>
	class iterator_wrapper final
	{
	public:
		using iterator_category = typename std::iterator_traits<UnderlyingIt>::iterator_category;
		using difference_type = typename std::iterator_traits<UnderlyingIt>::difference_type;
		using value_type = typename std::iterator_traits<UnderlyingIt>::value_type;
		using pointer = typename std::iterator_traits<UnderlyingIt>::pointer;
		using reference = typename std::iterator_traits<UnderlyingIt>::reference;

	private:
		UnderlyingIt underlying_iterator_{};

	public:
		iterator_wrapper() noexcept = default;
		explicit iterator_wrapper(UnderlyingIt underlying_iterator) noexcept
			: underlying_iterator_(std::move(underlying_iterator)) {}


		[[nodiscard]] reference operator*() const
		{
			return *underlying_iterator_;
		}

		[[nodiscard]] pointer operator->() const
		{
			return underlying_iterator_.operator->();
		}


		[[nodiscard]] bool operator==(const iterator_wrapper& other) const
		{
			return underlying_iterator_ == other.underlying_iterator_;
		}

		[[nodiscard]] bool operator!=(const iterator_wrapper& other) const
		{
			return underlying_iterator_ != other.underlying_iterator_;
		}
	};


	template <std::forward_iterator UnderlyingIt>
	class iterator_wrapper<UnderlyingIt> final
	{
	public:
		using iterator_category = typename std::iterator_traits<UnderlyingIt>::iterator_category;
		using difference_type = typename std::iterator_traits<UnderlyingIt>::difference_type;
		using value_type = typename std::iterator_traits<UnderlyingIt>::value_type;
		using pointer = typename std::iterator_traits<UnderlyingIt>::pointer;
		using reference = typename std::iterator_traits<UnderlyingIt>::reference;

	private:
		UnderlyingIt underlying_iterator_{};

	public:
		iterator_wrapper() noexcept = default;
		explicit iterator_wrapper(UnderlyingIt underlying_iterator) noexcept
			: underlying_iterator_(std::move(underlying_iterator)) {}


		[[nodiscard]] reference operator*() const
		{
			return *underlying_iterator_;
		}

		[[nodiscard]] pointer operator->() const
		{
			return underlying_iterator_.operator->();
		}


		[[nodiscard]] bool operator==(const iterator_wrapper& other) const
		{
			return underlying_iterator_ == other.underlying_iterator_;
		}

		[[nodiscard]] bool operator!=(const iterator_wrapper& other) const
		{
			return underlying_iterator_ != other.underlying_iterator_;
		}


		iterator_wrapper& operator++()
		{
			++underlying_iterator_;
			return *this;
		}

		iterator_wrapper operator++(int)
		{
			iterator_wrapper tmp = *this;
			++*this;
			return tmp;
		}
	};


	template <std::bidirectional_iterator UnderlyingIt>
	class iterator_wrapper<UnderlyingIt> final
	{
	public:
		using iterator_category = typename std::iterator_traits<UnderlyingIt>::iterator_category;
		using difference_type = typename std::iterator_traits<UnderlyingIt>::difference_type;
		using value_type = typename std::iterator_traits<UnderlyingIt>::value_type;
		using pointer = typename std::iterator_traits<UnderlyingIt>::pointer;
		using reference = typename std::iterator_traits<UnderlyingIt>::reference;

	private:
		UnderlyingIt underlying_iterator_{};

	public:
		iterator_wrapper() noexcept = default;
		explicit iterator_wrapper(UnderlyingIt underlying_iterator) noexcept
			: underlying_iterator_(std::move(underlying_iterator)) {}


		[[nodiscard]] reference operator*() const
		{
			return *underlying_iterator_;
		}

		[[nodiscard]] pointer operator->() const
		{
			return underlying_iterator_.operator->();
		}


		[[nodiscard]] bool operator==(const iterator_wrapper& other) const
		{
			return underlying_iterator_ == other.underlying_iterator_;
		}

		[[nodiscard]] bool operator!=(const iterator_wrapper& other) const
		{
			return underlying_iterator_ != other.underlying_iterator_;
		}


		iterator_wrapper& operator++()
		{
			++underlying_iterator_;
			return *this;
		}

		iterator_wrapper operator++(int)
		{
			iterator_wrapper tmp = *this;
			++*this;
			return tmp;
		}


		iterator_wrapper& operator--()
		{
			--underlying_iterator_;
			return *this;
		}

		iterator_wrapper operator--(int)
		{
			iterator_wrapper tmp = *this;
			--*this;
			return tmp;
		}
	};


	template <std::random_access_iterator UnderlyingIt>
	class iterator_wrapper<UnderlyingIt> final
	{
	public:
		using iterator_category = typename std::iterator_traits<UnderlyingIt>::iterator_category;
		using difference_type = typename std::iterator_traits<UnderlyingIt>::difference_type;
		using value_type = typename std::iterator_traits<UnderlyingIt>::value_type;
		using pointer = typename std::iterator_traits<UnderlyingIt>::pointer;
		using reference = typename std::iterator_traits<UnderlyingIt>::reference;

	private:
		UnderlyingIt underlying_iterator_{};

	public:
		iterator_wrapper() noexcept = default;
		explicit iterator_wrapper(UnderlyingIt underlying_iterator) noexcept
			: underlying_iterator_(std::move(underlying_iterator)) {}


		[[nodiscard]] reference operator*() const
		{
			return *underlying_iterator_;
		}

		[[nodiscard]] pointer operator->() const
		{
			return underlying_iterator_.operator->();
		}


		iterator_wrapper& operator++()
		{
			++underlying_iterator_;
			return *this;
		}

		iterator_wrapper operator++(int)
		{
			iterator_wrapper tmp = *this;
			++*this;
			return tmp;
		}


		iterator_wrapper& operator--()
		{
			--underlying_iterator_;
			return *this;
		}

		iterator_wrapper operator--(int)
		{
			iterator_wrapper tmp = *this;
			--*this;
			return tmp;
		}


		[[nodiscard]] auto operator<=>(const iterator_wrapper&) const = default;

		iterator_wrapper& operator+=(const difference_type n)
		{
			underlying_iterator_ += n;
			return *this;
		}

		[[nodiscard]] iterator_wrapper operator+(const difference_type n)
		{
			return iterator_wrapper{ underlying_iterator_ + n };
		}

		iterator_wrapper& operator-=(const difference_type n)
		{
			underlying_iterator_ -= n;
			return *this;
		}

		[[nodiscard]] iterator_wrapper operator-(const difference_type n)
		{
			return iterator_wrapper{ underlying_iterator_ - n };
		}

		[[nodiscard]] difference_type operator-(const iterator_wrapper& other) const
		{
			return underlying_iterator_ - other.underlying_iterator_;
		}

		[[nodiscard]] friend iterator_wrapper operator+(const difference_type n, const iterator_wrapper& iterator)
		{
			return iterator_wrapper{ n + iterator.underlying_iterator_ };
		}

		[[nodiscard]] reference operator[](const difference_type n) const
		{
			return underlying_iterator_[n];
		}
	};
}