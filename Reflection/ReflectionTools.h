#pragma once
/***********************************************
* @headerfile ReflectionTools.h
* @date 23 / 03 / 2026
* @author Roomain
************************************************/
#include <string_view>
#include <boost/json.hpp>

template<typename Callback>
bool getData(const boost::json::object& a_object, const std::string_view a_attribute, Callback a_callback)
{
	if (auto node = a_object.find(a_attribute))
	{
		return a_callback(node);
	}
	return false;
}