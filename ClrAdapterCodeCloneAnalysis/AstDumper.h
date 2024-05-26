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
		static String^ _directoryPath = "ast_dumping/";
		static String^ _name = "dump.ast";
	public:

		static String^ dumpFile(System::IO::FileInfo^ inputFileInfo)
		{
			String^ newDirectoryName = Path::Combine(inputFileInfo->DirectoryName, _directoryPath);

			std::filesystem::path directory(msclr::interop::marshal_as<std::string>(newDirectoryName));

			if (!std::filesystem::exists(directory)) 
			{
				std::filesystem::create_directory(directory);
			}

			std::filesystem::path directoryPath = (msclr::interop::marshal_as<std::string>(inputFileInfo->FullName));

			String^ newFileName = Path::Combine(newDirectoryName, _name);

			std::filesystem::path newDirectoryPath = (msclr::interop::marshal_as<std::string>(newFileName));

			std::vector<std::string> command_line_args;
			clang_ast_dumping::source_to_ast_arguments args(directoryPath, newDirectoryPath, command_line_args);
			
			clang_ast_dumping::ast_dumper_default dumper;

			dumper.dump(args);

			return newDirectoryName;
		}
	};
}