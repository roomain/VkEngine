#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <QVariant>
#include <QString>
#include "Reflective_traits.h"

using EditGetter = std::function<QVariant()>;
using EditSetter = std::function<void(const QVariant&)>;

template<typename, typename = std::void_t<>>
struct has_base : std::false_type {};

template<typename T>
struct has_base<T, std::void_t<typename T::Base>> : std::true_type {};

/*@brief used by model to display and edit value*/
struct ValueEditionLayer
{
	QString dataName;
	bool isOptional = false;
	bool hasValue = true;
	EditGetter getter;
	EditSetter setter;

	void set(const QVariant& value)
	{
		if(isOptional)
			hasValue = value.isValid();

		if(setter)
			setter(value);
	}

	QVariant get()
	{
		if (getter)
			return getter();
	}
};

// Forward declaration
template<typename Type> requires is_reflective_v<Type>
std::vector<ValueEditionLayer> generateValueEditionLayer(const std::string& name, Type& edited);

template<typename Type>
void appendValue(std::vector<ValueEditionLayer>& vEdition, const std::string structName,
	const std::string_view memberName, Type& value)
{
	if constexpr (is_reflective_v<Type>)
	{
		vEdition.emplace_back(generateValueEditionLayer(structName + "." + memberName, edited));
	}
	else if constexpr(is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
	{
		// add container node
		if constexpr (is_reflective_v<Type::value_type>)
		{
			for (auto& value : edited)
			{
				//
			}
		}
		else
		{
			for (auto& value : edited)
			{
				//
			}
		}
	}
	else
	{
		vEdition.emplace_back(
			ValueEditionLayer{
				.dataName = QString::fromStdString(structName + "." + std::string(memberName)),
				.isOptional = is_std_optional_v<Type>,
				.hasValue = constexpr(is_std_optional_v<Type>) ? edited.has_value() : true,
				.getter = [&edited]()
				{
					// todo
				},
				.setter = [&edited](const QVariant& value)
				{
					// todo
				}
			}
		)
	}
}

template<typename Type> requires is_reflective_v<Type>
std::vector<ValueEditionLayer> generateValueEditionLayer(const std::string& name, Type& edited)
{
	std::vector<ValueEditionLayer> vData;

	if constexpr (has_base<Type>::value)
	{
		// add parent values
		// C++ 23
		vData.append_range(generateValueEditionLayer<typename Type::Base>(name, edited));
	}

	std::apply(
		[&vData, &edited, &name](auto&... tupleArgs)
		{
			(appendValue(vData, name, tupleArgs.first, edited.*(tupleArgs.second)), ...);
		},
		Type::s_reflectiveCtx);

	return vData;
}