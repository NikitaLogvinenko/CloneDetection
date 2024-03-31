#pragma once
#include "copying_moving_delete.h"
#include <atomic>

namespace utility
{
	class spinlock final
	{
		std::atomic_bool locked_{};

	public:
		spinlock() = default;

		COPYING_MOVING_DELETE(spinlock)

		~spinlock() = default;

		void lock()
		{
			bool expected = false;
			while (!locked_.compare_exchange_weak(expected, true, std::memory_order_acquire)) 
			{
				expected = false;
			}
		}

		void unlock()
		{

			locked_.store(false, std::memory_order_release);
		}
	};
}