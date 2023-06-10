#include "Boundaries.hpp"
#include <algorithm> // for std::min

Boundaries::Boundaries(const Range& rows_range, const Range& cols_range, const Range& channels_range)
{
	rows_range_ = rows_range;
	cols_range_ = cols_range;
	channels_range_ = channels_range;
}

int Boundaries::rows() const
{
	return rows_range_.size();
}

int Boundaries::cols() const
{
	return cols_range_.size();
}

int Boundaries::total() const
{
	if (channels() == 0)
	{
		return 0;
	}
	return rows() * cols();
}

int Boundaries::channels() const
{
	return channels_range_.size();
}

bool Boundaries::empty() const
{
	return cols() == 0 || rows() == 0 || channels() == 0;
}
