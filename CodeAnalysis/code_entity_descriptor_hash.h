#pragma once
#include "code_entity_descriptor.h"

namespace code_analysis
{
	class code_entity_descriptor_hash final
	{
	public:
		size_t operator()(const code_entity_descriptor& code_entity_descr) const
		{
			auto& spelling = code_entity_descr.spelling();
			auto& location = code_entity_descr.location();
			auto& first_symbol = location.first_symbol_position();

			return std::hash<std::string>{}(std::format(
				R"({}\{}\{}\{}\{})", spelling.to_string(), location.filename().string(), 
				first_symbol.column(), first_symbol.line(), first_symbol.offset_from_file_start()));
		}
	};
}
