#pragma once
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include "func_entity_type.h"
#include "var_origin_and_usage_counter_pair.h"
#include "cursor_classifier.h"
#include <unordered_map>
#include <unordered_set>

namespace cpp_code_analysis::internal
{
	class func_entities_classifier final
	{
		std::unordered_map<
			CXCursor,
			var_origin_and_usage_counter_pair,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal> var_origin_and_usage_counter_by_decl_cursor_{};

		std::unordered_map<
			func_entity_type,
			std::unordered_set<
				CXCursor,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>> cursors_by_entity_type_{};

	public:
		func_entities_classifier();

		bool try_insert(const CXCursor& cursor);

		[[nodiscard]] const auto& var_origin_and_usage_counter_by_decl_cursors() const noexcept
		{
			return var_origin_and_usage_counter_by_decl_cursor_;
		}

		[[nodiscard]] const std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& cursors_to_entities(const func_entity_type entity_type) const noexcept
		{
			return cursors_by_entity_type_.at(entity_type);
		}

	private:
		bool try_insert_reference_to_var(const CXCursor& cursor, const CXCursorKind& kind);
		bool try_insert_variable(const CXCursor& cursor);
		bool try_insert_cursor(const CXCursor& cursor, const CXCursorKind& kind);

		[[nodiscard]] static clang_c_adaptation::var_origin determine_var_origin(const CXCursor& cursor_to_var_decl);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types(
			const CXCursor& cursor, const CXCursorKind& kind);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_binary_operator(
			const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_unary_operator(
			const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_compound_assign(
			const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_call_expr(const CXCursor& cursor);

		[[nodiscard]] static CXChildVisitResult visitor_direct_children_counter(
			CXCursor cursor, CXCursor parent, const CXClientData void_ptr_to_size_t_counter)
		{
			clang_c_adaptation::cursor_classifier::throw_if_null(void_ptr_to_size_t_counter);
			++*static_cast<size_t* const>(void_ptr_to_size_t_counter);
			return CXChildVisit_Continue;
		}
	};
}
