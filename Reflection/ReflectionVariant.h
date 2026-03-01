#pragma once
/***********************************************
* @headerfile ReflectionVariant.h
* @date 25 / 02 / 2026
* @author Roomain
************************************************/

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <unordered_map>
#include "reflection_traits.h"




using ConvertStringToInt = std::function<int(const std::string&)>;
using IReflectionObjectUPtr = std::unique_ptr<class IReflectionObject>;

class ReflectionVariant : public std::variant<bool, int, unsigned int, double, std::string, std::vector<ReflectionVariant>, IReflectionObjectUPtr>
{};


/*@brief visitor use to fill data from variant*/
template<typename Type>
struct ReflectionVisitor
{
	static inline std::unordered_map<std::string, ConvertStringToInt> s_EnumConvertDB;	/*!< database of convertion string to enum int value*/
	Type& m_data;	/*!< reference to data to fill*/

	ReflectionVisitor() = delete;
	explicit ReflectionVisitor(Type& a_data) : m_data(a_data) {}
	
	void operator()(const bool a_value)
	{
		if constexpr (std::is_same_v<Type, bool>)
		{
			m_data = a_value;
		}
		else
		{
			// log
		}
	}

	void operator()(const int a_value)
	{
		if constexpr (std::is_assignable_v<Type&, int> && !std::is_same_v<Type, std::string>)
		{
			m_data = a_value;
		}
		else
		{
			// log
		}
	}

	void operator()(const unsigned int a_value)
	{
		if constexpr (std::is_assignable_v<Type&, unsigned int> && !std::is_same_v<Type, std::string>)
		{
			m_data = a_value;
		}
		else
		{
			// log
		}
	}

	void operator()(const double& a_value)
	{
		if constexpr (std::is_assignable_v<Type&, unsigned int> && !std::is_same_v<Type, std::string>)
		{
			m_data = a_value;
		}
		else
		{
			// log
		}
	}

	void operator()(const std::string& a_value)
	{
		if constexpr (std::is_enum_v<Type>)
		{
			// data is an enum find conversion string value to int
			if (const auto iter = s_EnumConvertDB.find(typeid(Type).name()); iter != s_EnumConvertDB.cend())
			{
				m_data = static_cast<Type>(iter->second(a_value));
			}
			else
			{
				// log error
			}
		}
		else if constexpr (std::is_assignable_v<Type&, std::string>)
		{
			m_data = a_value;
		}
		else
		{
			// log error
		}
	}

	void operator()(const std::vector<ReflectionVariant>& a_value)
	{
		if constexpr (is_std_vector_v<Type>)
		{
			//
		}
		else if constexpr (is_std_list_v<Type>)
		{
			//
		}
		else if constexpr (is_std_array_v<Type>)
		{
			//
		}
		else
		{
			//log
		}
	}

	void operator()(IReflectionObjectUPtr&& a_value)
	{
		// todo
	}
};