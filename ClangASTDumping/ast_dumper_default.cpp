#include "ast_dumper_default.h"
#include "translation_unit_raii.h"

void clang_ast_dumping::ast_dumper_default::dump(const source_to_ast_arguments& args)
{
	const clang_c_adaptation::translation_unit_raii translation_unit{
		cxindex_, args.source_path(), args.command_line_args() };

	// ReSharper disable once CppTooWideScopeInitStatement
	const int result = clang_saveTranslationUnit(translation_unit.translation_unit(),
		args.ast_path().string().c_str(), clang_defaultSaveOptions(translation_unit.translation_unit()));

	if (result == CXSaveError_None)
	{
		return;
	}

	throw clang_c_adaptation::translation_unit_error(
		std::format("ast_dumper_default::dump: can not dump translation unit.\n"
			  "Source filename: \"{}\"\nAST filename: \"{}\"", args.source_path().string(), args.ast_path().string()));
}
