#pragma once
#include "graph_io_exceptions.h"
#include <istream>
#include <regex>
#include <string>
#include <array>

namespace graph_io_ns
{
	class parser_Rn_space final
	{
	public:
		template <size_t Dim>
		struct vertex_params final
		{
			std::string id{};
			std::array<double, Dim> coordinates{};
		};

		template <size_t Dim>
		[[nodiscard]] vertex_params<Dim> get_vertex_params(std::istream& input) const;
	};

	template <size_t Dim>
	parser_Rn_space::vertex_params<Dim> parser_Rn_space::get_vertex_params(std::istream& input) const
	{
		std::string line;
		if (!std::getline(input >> std::ws, line))
		{
			return {};
		}

		const std::regex delim(R"([,;][\t ]*)");
		std::sregex_token_iterator token(line.begin(), line.end(), delim, -1);
		const std::sregex_token_iterator token_end{};
		if (token == token_end || token->str().empty())
		{
			throw invalid_id_exception();
		}

		vertex_params<Dim> params{};
		params.id = *token++;

		if (token == token_end)
		{
			throw no_coords_exception();
		}

		size_t vertex_index = 0;
		for (; token != token_end && vertex_index < Dim; ++token, ++vertex_index)
		{
			try
			{
				params.coordinates[vertex_index] = std::stod(*token);
			}
			catch ([[maybe_unused]] const std::logic_error& ex)
			{
				throw invalid_coord_exception();
			}
		}

		if (vertex_index != Dim || token != token_end)
		{
			throw wrong_dim_exception();
		}
		return params;
	}
}
