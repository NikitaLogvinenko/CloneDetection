#pragma once

class custom_id_for_tests
{
	int x_;
public:
	explicit custom_id_for_tests(const int x) noexcept: x_(x) {}
	[[nodiscard]] int x() const noexcept { return x_; }
	bool operator==(const custom_id_for_tests&) const noexcept = default;
};

template<>
struct std::hash<custom_id_for_tests>
{
	size_t operator() (const custom_id_for_tests& id) const noexcept
	{
		return std::hash<int>{}(id.x());
	}
};

class custom_value_for_tests
{
	int x_;
public:
	explicit custom_value_for_tests(const int x) noexcept : x_(x) {}
	[[nodiscard]] int x() const noexcept { return x_; }
	bool operator==(const custom_value_for_tests&) const noexcept = default;
};