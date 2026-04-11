#pragma once
/***********************************************
* @headerfile Reflection_traits.h
* @date 22 / 03 / 2026
* @author Roomain
************************************************/
#include <string>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <source_location>
#include <stack>
#include <glaze/glaze.hpp>

namespace boost::json
{
	class object;
	class value;
}

struct ReflectionProfile
{
	std::string profileName;
	std::string parentProfile;
	std::unordered_map<std::string, std::string> reflections;
};

using LogCallback = std::function<void(const std::source_location&, const std::string_view)>;

/*@brief Read configuration file to init class and struct*/
class ReflectionReader
{
private:
	static LogCallback sLogCallback;
	static void internalLog(const std::source_location& a_loc, const std::string_view a_message);
	static constexpr std::string_view CLASS_ARRAY = "ClassArray";
	static constexpr std::string_view PROFILE_NODE = "Id";
	static constexpr std::string_view PROFILE_NAME_ATT = "name";
	static constexpr std::string_view PROFILE_PARENT_ATT = "parent";

	void readProfile(const boost::json::object& a_profile);

	std::string m_profile;								/*!< used profile*/
	std::vector<ReflectionProfile> m_reflectProfiles;	/*!< serialized data per profile*/
	ReflectionReader() = default;

public:
	static void setLogCallback(LogCallback a_callback);
	static ReflectionReader& instance();
	bool loadFile(const std::string& a_file);
	void setProfile(const std::string_view a_profile);
	using const_iterator = std::vector<ReflectionProfile>::const_iterator;
	constexpr const_iterator cbegin()const { return m_reflectProfiles.cbegin(); }
	constexpr const_iterator cend()const { return m_reflectProfiles.cend(); }
	const_iterator findProfile(const std::string_view a_profileName)const;
	bool hasProfile(const  std::string_view a_profileName)const;

	template<typename DataType>
	void deserialize(const std::string_view a_typename, DataType& a_data)const
	{
		std::string strTypename{ a_typename };
		
		auto deserialize = [&strTypename, &a_data, this](ReflectionReader::const_iterator a_iter) -> bool
			{
				if (auto glzError = glz::read_json(a_data, a_iter->reflections.at(strTypename)); glzError) {
					ReflectionReader::internalLog(std::source_location::current(),
						std::format("Can't read {} in profile \'{}\'", strTypename, a_iter->profileName));
					// todo throw
					return false;
				}
				return true;
			};


		auto stackAdd = [&strTypename](std::stack<const_iterator>& stack, ReflectionReader::const_iterator a_iter)
			{
				if (auto reflect = a_iter->reflections.find(strTypename); reflect != a_iter->reflections.cend())
				{
					stack.push(a_iter);
				}
			};

		auto iter = ReflectionReader::findProfile(m_profile);
		if (iter == m_reflectProfiles.cend())
		{
			ReflectionReader::internalLog(std::source_location::current(),
				std::format("Profile {} not found", m_profile));
			// todo throw
			return;
		}

		std::stack<const_iterator> profileStack;
		stackAdd(profileStack, iter);
		while (!iter->parentProfile.empty())
			stackAdd(profileStack, iter);


		while (!profileStack.empty() && deserialize(profileStack.top()))
			profileStack.pop();
	}
};

