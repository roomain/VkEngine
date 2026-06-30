#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <QVariant>
#include <QString>
#include "Reflective_traits.h"

using EditGetter = std::function<QVariant()>;
using EditSetter = std::function<void(const QVariant&)>;

//template<typename, typename = std::void_t<>>
//struct has_base : std::false_type {};
//
//template<typename T>
//struct has_base<T, std::void_t<typename T::Base>> : std::true_type {};
