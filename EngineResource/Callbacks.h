#pragma once
#include <string>
#include <functional>

using ResourceLogCallback = std::function<void(const std::string&)>;
using ResourceRangeCallback = std::function<void(const unsigned int, const unsigned int)>;
using ResourceCounterCallback = std::function<void(const unsigned int)>;