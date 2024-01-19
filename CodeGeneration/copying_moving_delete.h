#pragma once
#define COPYING_MOVING_DELETE(class_name) \
	class_name(const class_name&) = delete; \
	class_name(class_name&&) = delete; \
	class_name& operator=(const class_name&) = delete; \
	class_name& operator=(class_name&&) = delete;