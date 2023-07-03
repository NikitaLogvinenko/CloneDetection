#include "Range.hpp"
#include <limits> // for std::numeric_limits<int>::max()

Range::Range(const int start, const int end)
{
	if (start >= 0 && start < end)
	{
		start_ = start;
		end_ = end;
	}
}

int Range::size() const
{
	return end_ - start_;
}

bool Range::empty() const
{
	return end_ == start_;
}

int Range::start() const
{
	return start_;
}

int Range::end() const
{
	return end_;
}

Range Range::all()
{
	return { 0, std::numeric_limits<int>::max() };
}
