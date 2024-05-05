#pragma once
#include <string>

#include "HotKeyMap.hpp"

HotkeyMap readMap(std::istream& is);

std::vector<std::string> readRemoveList(std::istream& is);
