#pragma once
#include "../CodeClonesAnalysisTopLevel/cmcd_config.h"
#include <filesystem>
#include <iostream>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::IO;

namespace CLR
{
	public ref class ConfigMaker abstract sealed
	{
	public:
		static code_clones_analysis_top_level::cmcd_config makeConfig(String^ inputFileDir)
		{
			code_clones_analysis_top_level::cmcd_config config;

			std::filesystem::path directory = (msclr::interop::marshal_as<std::string>(inputFileDir));

			config.add_first_project_dir(directory);

			return config;
		}
	};
}