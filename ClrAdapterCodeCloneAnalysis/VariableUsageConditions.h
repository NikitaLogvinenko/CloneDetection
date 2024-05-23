#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace CLR
{
	public ref class VariableUsageConditions {
	public:
		static List<String^>^ values = gcnew List<String^>();

		static VariableUsageConditions() {
			values->Add("islocalvar");
			values->Add("isparam");
			values->Add("ismemberfield");
			values->Add("isglobalvar");
			values->Add("changedbyoperator");
			values->Add("usedinsidecallexpr");
			values->Add("usedwithsquarebrackets");
			values->Add("usedinsidesquarebrackets");
			values->Add("usedforadditionorsubtraction");
			values->Add("usedformultiplicationordivision");
			values->Add("usedformodulus");
			values->Add("usedforcomparison");
			values->Add("usedinsideconditionalstatement");
			values->Add("usedinsideloop");
			values->Add("definedwithcallexpr");
			values->Add("definedwithadditionorsubtraction");
			values->Add("definedwithmultiplicationordivision");
			values->Add("definedwithmodulus");
			values->Add("definedwithcomparison");
			values->Add("definedwithliterals");
		}
	};
}