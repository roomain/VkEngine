#include "ResourceChecker.h"
#include <fstream>
#include <crc32c/crc32c.h>

uint32_t ResourceChecker::computeSrc(const std::string& a_filename)
{
	std::ifstream file(a_filename, std::ios_base::ate);
	size_t fileSize = file.tellg();
	file.seekg(0);
	Binary bin(fileSize);
	file.read(bin.data(), fileSize);
	return crc32c::Crc32c(bin.data(), fileSize);
}

ResourceChecker::ResourceChecker(const CheckerParameters& a_parameters) : m_parameters{ a_parameters }
{
	//
}

ResourceChecker::ResourceChecker(const CheckerParameters& a_parameters, const std::string& a_directory) :
	ResourceChecker(a_parameters)
{
	load(a_directory);
}

void ResourceChecker::load(const std::string& a_directory)
{
	m_resourceDirectory = std::filesystem::path(a_directory);
	for (auto const& entry : std::filesystem::recursive_directory_iterator{ m_resourceDirectory })
	{
		if (entry.is_regular_file() && std::ranges::find(m_parameters.extensions, entry.path().extension()) != m_parameters.extensions.cend())
		{
			m_headings.emplace_back(entry.path().filename().string(),
				ResourceChecker::computeSrc(entry.path().string()));
		}
	}
}

std::vector<Resources::Delta> ResourceChecker::compare(const Resources& a_old)const
{
	return a_old.diff(*this);
}
