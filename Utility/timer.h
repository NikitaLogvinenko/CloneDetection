#pragma once
#include <chrono>

namespace utility
{
	class timer final
	{
		std::chrono::time_point<std::chrono::steady_clock> t_start_{};
		std::chrono::time_point<std::chrono::steady_clock> t_stop_{};

	public:
		void start()
		{
			t_start_ = std::chrono::steady_clock::now();
		}

		void stop()
		{
			t_stop_ = std::chrono::steady_clock::now();
		}

		template <class Duration = std::chrono::seconds>
		[[nodiscard]] auto interval() const
		{
			return std::chrono::duration_cast<Duration>(t_stop_ - t_start_).count();
		}
	};
}