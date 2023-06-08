#pragma once
#include <string>
#include <stdexcept>

namespace cpp_code_analysis
{
	class matrix_index_upper_diagonal final
	{
		size_t row_;
		size_t column_;

		inline static const std::string column_must_be_greater_msg{ "matrix_index_upper_diagonal: column must be strictly greater than row." };

	public:
		explicit matrix_index_upper_diagonal(const size_t row, const size_t column) : row_(row), column_(column)
		{
				if (row >= column)
				{
					throw std::invalid_argument(column_must_be_greater_msg);
				}
		}

		[[nodiscard]] size_t row() const noexcept
		{
			return row_;
		}

		[[nodiscard]] size_t column() const noexcept
		{
			return column_;
		}

		[[nodiscard]] auto operator<=>(const matrix_index_upper_diagonal& other) const noexcept = default;
	};
}
