#pragma once
#include <filesystem>
#include <iostream>
#include <msclr\marshal_cppstd.h>
#include <string>
#include "ast_dumper_default.h"

using namespace System;
using namespace System::IO;

namespace CLR
{
	public ref class AstDumper abstract sealed
	{
	public:

		static void dumpFile(System::IO::FileInfo^ inputFileInfo, String^ directoryPath, String^ fileName)
		{
			std::filesystem::path directory(msclr::interop::marshal_as<std::string>(directoryPath));

			if (!std::filesystem::exists(directory)) 
			{
				std::filesystem::create_directory(directory);
			}

			std::filesystem::path sourcePath = (msclr::interop::marshal_as<std::string>(inputFileInfo->FullName));

			String^ newFileName = Path::Combine(directoryPath, fileName);

			std::filesystem::path newDirectoryPath = (msclr::interop::marshal_as<std::string>(newFileName));

			std::vector<std::string> command_line_args;
			clang_ast_dumping::source_to_ast_arguments args(sourcePath, newDirectoryPath, command_line_args);
			
			clang_ast_dumping::ast_dumper_default dumper;

			dumper.dump(args);

		}
	};
}