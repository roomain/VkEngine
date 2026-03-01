#pragma once
#pragma once
/***********************************************
* @headerfile ReflectionObject.h
* @date 27 / 02 / 2026
* @author Roomain
************************************************/
#include <functional>
#include <unordered_map>
#include <boost/json.hpp>
//#include "ReflectionVariant.h"

//namespace boost::json
//{
//	class object;
//	class value;
//}

template<typename Type>
using ValueAffect = std::function<void(Type&, const boost::json::value&)>;

/*@brief represents an object in reflective file*/
class IReflectionObject
{
protected:
	bool m_isDeserialized = false;	/*!< indicate if object is already deserialized*/

public:
	virtual void deserialize(const boost::json::object& a_jsonObject) = 0;
	constexpr bool isDeserialize()const { return m_isDeserialized; }
};


template<typename Type>
struct ReflectionVisitor
{
	static inline std::unordered_map<std::string, ConvertStringToInt> s_EnumConvertDB;	/*!< database of convertion string to enum int value*/
	Type& m_data;	/*!< reference to data to fill*/

	void operator()(const json::object& a_value) const
	{
		// todo
		// Get ReflectiveInstance<Type>	via dynamic_pointer_cast
		// deserialize(Type& a_nested, const boost::json::object& a_jsonObject)
	}

	void operator()(const json::array& a_value) const
	{
		// todo
	}

	void operator()(const std::string& a_value) const
	{
		// todo
	}

	void operator()(const std::int64_t& a_value) const
	{
		// todo
	}

	void operator()(const double& a_value) const
	{
		// todo
	}

	void operator()(const bool a_value) const
	{
		// todo
	}

	void operator()(std::nullptr_t) const
	{
		// todo
	}
};

template<typename Type>
class ReflectiveInstance : public IReflectiveObject
{
private:
	static inline std::unordered_map<std::string_view, ValueAffect<Type>> s_affectMap;	/*!< affectation per member*/
	Type m_deserializedObject;															/*!< base instantiated data*/

public:
	virtual void deserialize(const boost::json::object& a_jsonObject) override
	{
		for (auto [name, value] : a_jsonObject)
		{
			if (auto iter = s_affectMap.find(name); iter != s_affectMap.cend())
			{
				// todo
			}
			else
			{
				// throw
			}
		}
	}

	/*@brief deserialize nested data from other ReflectiveInstance*/
	virtual void deserialize(Type& a_nested, const boost::json::object& a_jsonObject)
	{
		for (auto [name, value] : a_jsonObject)
		{
			if (auto iter = s_affectMap.find(name); iter != s_affectMap.cend())
			{
				// todo
			}
			else
			{
				// throw
			}
		}
	}
};