#pragma once
namespace clang_c_adaptation
{
	enum class var_origin
	{
		unknown,
		func_param,
		local_var,
		member_field,
		global_var,
	};
}