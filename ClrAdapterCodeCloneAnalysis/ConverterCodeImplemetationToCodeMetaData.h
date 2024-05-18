#pragma once
#include "count_matrix.h"
#include <vector>
#include "code_entity_implementation_info.h"
#include "funcs_analysis_traits.h"
#include <var_usage_conditions.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace CodeMetaData;
using namespace cm;

namespace CLR
{
	public ref class ConverterCodeImplemetationToCodeMetaData
	{
		static VariableClasses::VariableUsage^ ConvertToUsage(cm::count_vector<clang_code_analysis::var_usage_conditions_total> vector)
		{
			VariableClasses::VariableUsage^ usage = gcnew VariableClasses::VariableUsage();

			size_t i = 0;
			for (auto& it : vector)
			{
				Operations::OperationId^ id = gcnew Operations::OperationId(msclr::interop::marshal_as<String^>(std::to_wstring(i)));
				Operations::UnaryOperation^ operation = gcnew Operations::UnaryOperation(id);
				Operations::OperationCounter^ counter = gcnew Operations::OperationCounter(operation, it.to_size_t());
				usage->Add(counter);
				i++;
			}
			
			return usage;
		}

	public:
		static FunctionCodeMetaData^ ConvertToCodeMetaData(const
			code_analysis_through_cm::code_entity_implementation_info<code_analysis_through_cm::funcs_analysis_traits<clang_code_analysis::var_usage_conditions_total>>& implementationInfo)
		{
			FunctionCodeMetaData^ metaData = gcnew FunctionCodeMetaData();

			auto vector = implementationInfo.nested_entities_ids();
			
			auto matrix = implementationInfo.nested_entities_conditions_cm();

			for(size_t i = 0; i < vector.size(); i++)
			{
				auto vec_usages = matrix.at(index_of_count_vector(i));

				String^ variable_id = msclr::interop::marshal_as<String^>((std::to_wstring(vector[i].to_size_t())));
				VariableClasses::Variable^ variable = gcnew VariableClasses::Variable(variable_id);
				VariableClasses::VariableUsage^ usage = ConvertToUsage(vec_usages);
				metaData->AddVariable(usage, variable);
			}

			return metaData;
		}
	};
}
