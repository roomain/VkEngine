#pragma once
/***********************************************
* @headerfile ResourceChecker.h
* @date 23 / 08 / 2026
* @author Roomain
************************************************/
#include <utility>
#include <filesystem>
#include <string>
#include <type_traits>
#include "Resources.h"

template<typename T>
concept IsCompiler = requires(T compiler, const std::string & filename) {

	{ compiler.compile(filename) } -> std::same_as<Binary>;
};

/*@brief Resource checker*/
class ResourceChecker : public Resources
{
private:
	std::vector<std::string> m_fileExtensions;	/*!< resources files extensions*/
	std::filesystem::path m_resourceDirectory;	/*!< directory of resources*/

	static uint32_t computeSrc(const std::string& a_filename);

	void addTo(BinHeader&& a_header, const Binary&& a_bin, Resources& a_other);
public:
	ResourceChecker() = delete;
	explicit ResourceChecker(const std::vector<std::string>& a_extensions);
	explicit ResourceChecker(const std::vector<std::string>& a_extensions, const std::string& a_directory);
	void load(const std::string& a_directory);

	// compare with old version
	std::vector<Delta> compare(const Resources& a_old)const;

	template<typename Compiler> requires IsCompiler<Compiler>
	void mergeTo(Resources& a_other, Compiler& a_compiler)
	{
		auto deltaList = compare(a_other);
		for (const auto& delta : deltaList)
		{
			switch (delta.type)
			{
			case DeltaType::delta_new:
			case DeltaType::delta_updated:
			{
				const auto filename = std::string(delta.heading.filename.data());
				a_other.Resources::emplace(std::move(delta.heading),
					a_compiler.compile(m_resourceDirectory.string() + "\\" + filename));
			}
			break;
			case DeltaType::delta_removed:
			{
				auto iter = std::ranges::find_if(a_other.m_headings, [&delta](auto&& curHeader)
					{ return std::string(curHeader.filename.data()) == std::string(delta.heading.filename.data()); });
				a_other.m_headings.erase(iter);
				a_other.m_binaryMap.erase(delta.heading.filename.data());
			}
			default:
				break;
			}
		}
	}
};
