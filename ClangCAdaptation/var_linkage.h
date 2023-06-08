#pragma once
namespace clang_c_adaptation
{
	enum class var_linkage
	{
		unknown,
		func_param,
		local_var,
		member_field,
		static_field,
		global_var,
	};
}