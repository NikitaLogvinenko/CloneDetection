#pragma once
namespace clang_c_adaptation
{
	enum class var_linkage
	{
		unknown,
		func_param,
		local_var,
		field,
		static_var,
		global_var,
	};
}