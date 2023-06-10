#include "SharedPixels.hpp"
#include <stdexcept>


void SharedPixels::clear() const
{
	--* refs_counter_;
	if (*refs_counter_ == 0)
	{
		delete refs_counter_;
		if (clear_data_if_no_refs_)
		{
			delete[] data_;
		}
	}
}


SharedPixels::SharedPixels() : SharedPixels(nullptr, 0, false) {}

SharedPixels::SharedPixels(unsigned char* data, const size_t data_size, const bool clear_if_no_refs) :
	data_{ data }, refs_counter_{ new size_t(1) }, data_size_{ data_size }, clear_data_if_no_refs_{ clear_if_no_refs } {
}

SharedPixels::SharedPixels(const SharedPixels& copied_data) :
	data_{ copied_data.data_ }, refs_counter_{ &++*copied_data.refs_counter_ }, data_size_{ copied_data.data_size_ }, clear_data_if_no_refs_{ copied_data.clear_data_if_no_refs_ } {
}

SharedPixels& SharedPixels::operator=(const SharedPixels& assigned_data)
{
	if (this == &assigned_data || this->data_ == assigned_data.data_ && assigned_data.data_ != nullptr)
	{
		return *this;
	}
	clear();
	this->data_ = assigned_data.data_;
	this->refs_counter_ = &++*assigned_data.refs_counter_;
	this->data_size_ = assigned_data.data_size_;
	this->clear_data_if_no_refs_ = assigned_data.clear_data_if_no_refs_;
	return *this;
}

SharedPixels::~SharedPixels()
{
	clear();
}


unsigned char* SharedPixels::data()
{
	return this->data_;
}

const unsigned char* SharedPixels::data() const
{
	return this->data_;
}


size_t SharedPixels::data_size() const
{
	return this->data_size_;
}

size_t SharedPixels::count_ref() const
{
	return *this->refs_counter_;
}

bool SharedPixels::clears_data_itself() const
{
	return clear_data_if_no_refs_;
}


unsigned char& SharedPixels::operator[](const int i)
{
	return this->data_[i];
}

const unsigned char& SharedPixels::operator[](const int i) const
{
	return this->data_[i];
}

unsigned char& SharedPixels::at(const int i)
{
	if (i < 0 || i >= this->data_size_)
	{
		throw std::out_of_range("Invalid pixel index");
	}
	return this->data_[i];
}

const unsigned char& SharedPixels::at(const int i) const
{
	if (i < 0 || i >= this->data_size_)
	{
		throw std::out_of_range("Invalid pixel index");
	}
	return this->data_[i];
}


unsigned char* SharedPixels::operator+(const int i)
{
	return this->data_ + i;
}

const unsigned char* SharedPixels::operator+(const int i) const
{
	return this->data_ + i;
}
