#pragma once
#include <filesystem>
#include <functional>

namespace utility
{
	class nested_paths_provider final
	{
	public:
		nested_paths_provider() = default;

		template <class DirIterator> requires (std::is_same_v<DirIterator, std::filesystem::directory_iterator> 
		|| std::is_same_v<DirIterator, std::filesystem::recursive_directory_iterator>)
		static std::vector<std::filesystem::path> get_nested_paths(DirIterator dir_iterator,
				const std::function<bool(const std::filesystem::directory_entry&)>& nested_entity_predicate)
		{
			std::vector<std::filesystem::path> nested_paths{};

			for (const std::filesystem::directory_entry& nested_entry : dir_iterator)
			{
				if (nested_entity_predicate(nested_entry))
				{
					nested_paths.emplace_back(nested_entry.path());
				}
			}

			return nested_paths;
		}

		inline static const std::function is_directory_predicate =
			[](const std::filesystem::directory_entry& entry)
			{
				return entry.is_directory();
			};
	};
}