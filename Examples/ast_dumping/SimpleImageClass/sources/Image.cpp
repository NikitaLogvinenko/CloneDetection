#include "Image.hpp"

#include <stdexcept>


namespace
{
	Range convert_relative_to_absolute(const Range& entire_range, const Range& relative_subrange)
	{
		if (Range::all().end() - relative_subrange.start() < entire_range.start())
		{
			return  {};
		}

		const int left_boundary = std::min(entire_range.start() + relative_subrange.start(), entire_range.end());
		if (Range::all().end() - relative_subrange.end() < entire_range.start())
		{
			return { left_boundary, entire_range.end() };
		}
		const int right_boundary = std::min(entire_range.start() + relative_subrange.end(), entire_range.end());
		return { left_boundary, right_boundary };
	}
}

Image::Image(const int rows, const int cols, const int channels)
{
	if (invalid_boundaries(rows, cols, channels))
	{
		return;
	}

	create(rows, cols, channels);
}

Image::Image(const int rows, const int cols, const int channels, unsigned char* data)
{
	if (invalid_boundaries(rows, cols, channels) || data == nullptr)
	{
		return;
	}

	pixels_ = SharedPixels(data, rows * cols * channels, false);
	current_boundaries_ = initial_boundaries_ = Boundaries(Range(0, rows), Range(0, cols), Range(0, channels));
}

Image::Image(const Image& image, const Range& rows_range, const Range& cols_range)
{
	pixels_ = image.pixels_;

	if (image.empty())
	{
		return;
	}

	const Range abs_rows_range = convert_relative_to_absolute(image.current_boundaries_.rows_range_, rows_range);
	const Range abs_cols_range = convert_relative_to_absolute(image.current_boundaries_.cols_range_, cols_range);
	if (abs_rows_range.empty() || abs_cols_range.empty())
	{
		return;
	}
	current_boundaries_ = Boundaries(abs_rows_range, abs_cols_range, image.current_boundaries_.channels_range_);
	initial_boundaries_ = image.initial_boundaries_;
}


Image& Image::operator=(const Image& image)
{
	if (this == &image)
	{
		return *this;
	}

	current_boundaries_ = image.current_boundaries_;
	initial_boundaries_ = image.initial_boundaries_;
	pixels_ = image.pixels_;
	return *this;
}

Image Image::operator()(const Range& rows_range, const Range& cols_range) const
{
	return { *this, rows_range, cols_range };
}


Image Image::clone() const
{
	if (empty())
	{
		return {};
	}

	Image new_img(rows(), cols(), channels());
	copy_to_prepared(new_img);
	return new_img;
}

void Image::copyTo(Image& image) const
{
	if (this == &image)
	{
		return;
	}

	image.release();
	if (empty())
	{
		return;
	}

	image.create(rows(), cols(), channels());
	copy_to_prepared(image);
}

void Image::create(const int rows, const int cols, const int channels)
{
	if (invalid_boundaries(rows, cols, channels))
	{
		release();
		return;
	}

	const int data_required = rows * cols * channels;
	auto* data = new unsigned char[data_required]{};
	pixels_ = SharedPixels(data, data_required, true);
	current_boundaries_ = initial_boundaries_ = Boundaries(Range(0, rows), Range(0, cols), Range(0, channels));
}

bool Image::empty() const
{
	return rows() == 0 || cols() == 0 || channels() == 0;
}


void Image::release()
{
	if (empty())
	{
		return;
	}
	pixels_ = SharedPixels();
	current_boundaries_ = initial_boundaries_ = Boundaries();
}


Image Image::col(const int x) const
{
	return { *this, Range::all(), Range(x, x + 1) };
}

Image Image::colRange(const Range& range) const
{
	return { *this, Range::all(), range };
}


Image Image::row(int y) const
{
	return { *this, Range(y, y + 1), Range::all() };
}

Image Image::rowRange(const Range& range) const
{
	return { *this, range, Range::all() };
}


const unsigned char* Image::data() const
{
	return pixels_.data();
}
unsigned char* Image::data()
{
	return pixels_.data();
}


int Image::rows() const
{
	return current_boundaries_.rows();
}

int Image::cols() const
{
	return current_boundaries_.cols();
}

int Image::total() const
{
	return current_boundaries_.total();
}

int Image::channels() const
{
	return current_boundaries_.channels();
}

unsigned char& Image::at(const int index)
{
	const int offset = rel_index_to_offset(index);
	return *(pixels_ + offset);
}

const unsigned char& Image::at(const int index) const
{
	const int offset = rel_index_to_offset(index);
	return *(pixels_ + offset);
}


Image Image::zeros(const int rows, const int cols, const int channels)
{
	return { rows, cols, channels };
}

Image Image::values(const int rows, const int cols, const int channels, const unsigned char value)
{
	Image new_image(rows, cols, channels);
	if (value == 0 || new_image.empty())
	{
		return new_image;
	}

	const Range data_range(0, new_image.total() * new_image.channels());
	for (int data_index = data_range.start(); data_index < data_range.end(); ++data_index)
	{
		new_image.pixels_[data_index] = value;
	}
	return new_image;
}


size_t Image::countRef() const
{
	return pixels_.count_ref();
}


int Image::rel_index_to_offset(const int relative_index) const
{
	if (relative_index < 0 || relative_index >= total() * channels())
	{
		throw std::out_of_range("Invalid pixel index");
	}

	const int channel_relative = relative_index % channels();
	const int pixel_2d_index = (relative_index - channel_relative) / channels();
	const int col_relative = pixel_2d_index % cols();
	const int row_relative = (pixel_2d_index - col_relative) / cols();

	const int channel_abs = current_boundaries_.channels_range_.start() + channel_relative;
	const int col_abs = current_boundaries_.cols_range_.start() + col_relative;
	const int row_abs = current_boundaries_.rows_range_.start() + row_relative;

	const int initial_channels = initial_boundaries_.channels(), initial_cols = initial_boundaries_.cols();
	const int offset = row_abs * initial_cols * initial_channels + col_abs * initial_channels + channel_abs;
	return offset;
}

void Image::copy_to_prepared(Image& image) const
{
	for (int index = 0; index < total() * channels(); ++index)
	{
		image.at(index) = at(index);
	}
}

bool Image::invalid_boundaries(const int rows, const int cols, const int channels)
{
	return rows < 1 || cols < 1 || channels < 1;
}
