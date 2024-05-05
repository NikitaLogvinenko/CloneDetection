#pragma once
#define CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(class_name) \
	constexpr class_name(const class_name&) = default; \
	constexpr class_name(class_name&&) = default; \
	constexpr class_name& operator=(const class_name&) noexcept = default; \
	constexpr class_name& operator=(class_name&&) noexcept = default; \
	constexpr virtual ~class_name() = default;