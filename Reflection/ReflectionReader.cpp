#include "pch.h"
#include "ReflectionReader.h"
#include <fstream>
#include <format>
#include <ranges>
#include <boost/json.hpp>
#include "ReflectionTools.h"

LogCallback ReflectionReader::sLogCallback;

void ReflectionReader::setLogCallback(LogCallback a_callback)
{
	sLogCallback = a_callback;
}

void ReflectionReader::internalLog(const std::source_location& a_loc, const std::string_view a_message)
{
	if (sLogCallback)
		sLogCallback(a_loc, a_message);
}

ReflectionReader& ReflectionReader::instance()
{
	static ReflectionReader instance;
	return instance;
}

bool ReflectionReader::loadFile(const std::string& a_file)
{
	m_reflectProfiles.clear();
	std::ifstream jsonFile(a_file);
	boost::system::error_code erroCode;
	auto jsonRoot = boost::json::parse(jsonFile, erroCode);
	if (jsonRoot == nullptr)
	{
		ReflectionReader::internalLog(std::source_location::current(), erroCode.message());
		return false;
	}

	if (auto classArray = jsonRoot.as_object().find(ReflectionReader::CLASS_ARRAY))
	{
		try
		{
			for (const auto& nodeProfile : classArray->value().as_array())
			{
				readProfile(nodeProfile.as_object());
			}
		}
		catch (std::exception& except)
		{
			ReflectionReader::internalLog(std::source_location::current(), except.what());
			return false;
		}
	}

	return true;
}

void ReflectionReader::readProfile(const boost::json::object& a_profile)
{
	ReflectionProfile reflectProfile;
	for (const auto& node : a_profile)
	{
		if (node.key() == ReflectionReader::PROFILE_NODE)
		{
			const auto object = node.value().as_object();
			reflectProfile.profileName = object.at(ReflectionReader::PROFILE_NAME_ATT).as_string();

			if(auto parentValue = object.at(ReflectionReader::PROFILE_PARENT_ATT); !parentValue.is_null())
				reflectProfile.parentProfile = parentValue.as_string();
		}
		else
		{
			reflectProfile.reflections[node.key()] = boost::json::serialize(node.value());
		}
	}

	if (!reflectProfile.profileName.empty())
	{
		m_reflectProfiles.emplace_back(std::move(reflectProfile));
	}

}

void ReflectionReader::setProfile(const std::string_view a_profile)
{
	m_profile = a_profile;
}

ReflectionReader::const_iterator ReflectionReader::findProfile(const std::string_view a_profileName)const
{
	return std::ranges::find_if(m_reflectProfiles.cbegin(), m_reflectProfiles.cend(), [a_profileName](const auto& a_profile)
		{
			return a_profile.profileName.compare(a_profileName) == 0;
		});
}


bool ReflectionReader::hasProfile(const std::string_view a_profileName)const
{
	return findProfile(a_profileName) != cend();
}