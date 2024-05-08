#pragma once
#include <exception>
#include <string>

namespace graph_io_ns
{
	class invalid_line_exception : public std::exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override = 0;
	};

	class invalid_id_exception final : public invalid_line_exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "invalid id"; }
	};

	class invalid_coord_exception final : public invalid_line_exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "invalid coordinate"; }
	};

	class wrong_dim_exception : public invalid_line_exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "dimension of the vertex is wrong"; }
	};

	class no_coords_exception final : public wrong_dim_exception
	{
	public:
		[[nodiscard]] const char* what() const noexcept override { return "coordinates are absent"; }
	};

}
