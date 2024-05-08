#pragma once
#include <unordered_map>
#include <map>

namespace spanning_tree_ns
{
	template <typename T>
   class disjoint_set final
	{
	public:
		class subset final
		{
			size_t index_ {0};
		public:
			subset() noexcept = default;
			explicit subset(const size_t index) noexcept : index_(index) {}
			[[nodiscard]] size_t index() const noexcept { return index_; }
			[[nodiscard]] bool operator<(const subset& other) const noexcept { return index_ < other.index_; }
			[[nodiscard]] bool operator==(const subset& other) const noexcept { return index_ == other.index_; }
		};

	private:
		std::unordered_map<T, subset> subsets_by_values_{};
		std::map<subset, std::vector<T>> subsets_{};
		size_t subset_end_index_{ 0 };

	public:
		disjoint_set() noexcept = default;

		[[nodiscard]] size_t subsets_count() const noexcept;
		[[nodiscard]] subset end() const noexcept;

		subset insert(const T& value);
		void join(subset subset_to, subset subset_from);
		[[nodiscard]] subset find(const T& value) const;

		[[nodiscard]] std::vector<subset> existed_subsets() const;
	};

	template <typename T>
	size_t disjoint_set<T>::subsets_count() const noexcept
	{
		return subsets_.size();
	}

	template <typename T>
	typename disjoint_set<T>::subset disjoint_set<T>::end() const noexcept
	{
		return subset(subset_end_index_);
	}

	template <typename T>
	typename disjoint_set<T>::subset disjoint_set<T>::insert(const T& value)
	{
		if (const auto& [iterator, inserted] = subsets_by_values_.try_emplace(value, subset_end_index_); !inserted)
		{
			return iterator->second;
		}
		subsets_.insert({ subset(subset_end_index_), std::vector<T>{ value } });
		return subset(subset_end_index_++);
	}

	template <typename T>
	void disjoint_set<T>::join(subset subset_to, subset subset_from)
	{
		const auto subset_to_it = subsets_.find(subset_to);
		const auto subset_from_it = subsets_.find(subset_from);
		if (subset_to_it == subsets_.end() || subset_from_it == subsets_.end() || *subset_to_it == *subset_from_it)
		{
			return;
		}

		auto& values_subset_to = subset_to_it->second;
		for (const auto& value : subset_from_it->second)
		{
			subsets_by_values_[value] = subset_to;
			values_subset_to.push_back(value);
		}
		subsets_.erase(subset_from_it);
	}

	template <typename T>
	typename disjoint_set<T>::subset disjoint_set<T>::find(const T& value) const
	{
		const auto subset_it = subsets_by_values_.find(value);
		if (subset_it == subsets_by_values_.end())
		{
			return end();
		}
		return subset_it->second;
	}

	template <typename T>
	std::vector<typename disjoint_set<T>::subset> disjoint_set<T>::existed_subsets() const
	{
		std::vector<subset> subsets{};
		subsets.reserve(subsets_.size());
		for (const auto& [subset, _] : subsets_)
		{
			subsets.push_back(subset);
		}
		return subsets;
	}
}
