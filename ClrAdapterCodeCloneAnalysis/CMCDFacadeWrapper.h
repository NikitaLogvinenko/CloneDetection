#pragma once
#include "../CodeClonesAnalysisTopLevel/cmcd_config_parser_default.h"
#include<fstream>
#include <msclr/marshal_cppstd.h>
#include "../CodeClonesAnalysisTopLevel/funcs_analysis_through_count_matrix_factory_default.h"
#include "FuncAnalysisTraitsWithNoConcept.h"
#include "ConverterCodeImplemetationToCodeMetaData.h"

using namespace code_clones_analysis_top_level;
using namespace System;

namespace CLR
{
	public ref class CMCDFacadeWrapper
	{
	public:
		using func_id = code_analysis::func_id;

		using implementations_info_map = std::unordered_map<func_id,
			code_analysis_through_cm::code_entity_implementation_info<FuncsAnalysisTraits<clang_code_analysis::var_usage_conditions_total>>,
			utility::id_hash<func_id>>;

		static FileMetaData^ ProccesFunctionsCodeMetaData(System::IO::FileInfo^ inputFileInfo)
		{
			cmcd_config_parser_default parser = cmcd_config_parser_default();

			std::string path = msclr::interop::marshal_as<std::string>(inputFileInfo->FullName);

			std::ifstream stream;
			stream.open(path);

			if (!stream.is_open())
			{
				throw gcnew System::Exception("Error open stream of file");
			}

			const cmcd_config config = parser.parse(stream);

			stream.close();

			const auto analysis_factory = funcs_analysis_through_count_matrix_factory_default<clang_code_analysis::var_usage_conditions_total>();

			const auto analysis_director = analysis_factory.create_director(config);
			
			auto project_analysis_builder = analysis_factory.create_builder(config);

			auto project_traversers_factory = analysis_factory.create_traversers_factory(config, config.first_project_dirs());

			auto project_implementations_info = analysis_director->analyse_implementations(std::move(project_analysis_builder), std::move(project_traversers_factory));

			ConverterCodeImplemetationToCodeMetaData converter;

			FileMetaData^ metaData = gcnew FileMetaData();

			for (auto& it : project_implementations_info)
			{
				FunctionId^ funcId = gcnew FunctionId(msclr::interop::marshal_as<String^>(std::to_wstring(it.first.to_size_t())));
				FileMetaDataUnit^ unit = gcnew FileMetaDataUnit(funcId, converter.ConvertToCodeMetaData(it.second));
				metaData->Add(unit);
			}

			return metaData;
		}
	};
}