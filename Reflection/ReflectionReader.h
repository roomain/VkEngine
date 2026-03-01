#pragma once
/***********************************************
* @headerfile ReflectionReader.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <string_view>
#include <memory>
#include <unordered_map>
#include "reflection_globals.h"

namespace boost::json
{
	class object;
	class value;
}

/*
* @brief Sigleton reading json configuration files.
* These file contains serialized structures.
* Then structures are instancied their contant will
* correspond to serialized data from configuration files
*/
class REFLECTION_EXPORT ReflectionReader
{
private:
	ReflectionReader() = default;
	
	/*@brief serialized data per structure typename*/
	std::unordered_map<std::string_view, boost::json::value> m_serializedValue;

public:
	static ReflectionReader& instance();
	void read(const std::string_view& a_profile, const std::string_view& a_filename);
	[[nodiscard]] const boost::json::value& operator [](const std::string_view& a_key)const;
	[[nodiscard]] boost::json::value take(const std::string_view& a_key);
	[[nodiscard]] bool exist(const std::string_view& a_structName)const noexcept;
};

/*deserialization steps:
* 1- read files => each class name has his json value 
* 2- At first data instanciation the ReflectiveInstance deserialize from m_serializedValue
*/