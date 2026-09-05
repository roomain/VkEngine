#pragma once
#include <string>
#include <functional>

using LogCallback = std::function<void(const std::string&)>;
using RangeCallback = std::function<void(const unsigned int, const unsigned int)>;
using CounterCallback = std::function<void(const unsigned int)>;