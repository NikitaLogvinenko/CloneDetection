#pragma once
#define COPYING_DELETE_MOVE_THROUGH_SWAP(class_name) \
	class_name(const class_name&) = delete; \
	class_name& operator=(const class_name&) = delete; \
	class_name(class_name&& other) noexcept \
	{ \
		std::swap(*this, other); \
	} \
	class_name& operator=(class_name&& other) noexcept \
	{ \
		if (&other == this) \
		{ \
			return *this; \
		} \
		std::swap(*this, other); \
		return *this; \
	}