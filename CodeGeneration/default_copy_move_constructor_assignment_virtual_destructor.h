#pragma once
#define DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR(class_name) \
	class_name(const class_name&) = default; \
	class_name(class_name&&) = default; \
	class_name& operator=(const class_name&) = default; \
	class_name& operator=(class_name&&) = default; \
	virtual ~class_name() = default;
