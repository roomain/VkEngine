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
#include "Callbacks.h"


class Compiler;

/*@brief use for callback*/
struct CheckerCallbacks
{
	LogCallback logCallback;				/*!< log compiler errors and warnings*/
	RangeCallback rangeCallback;			/*!< set range*/
	CounterCallback errorCounterCallback;	/*!< set error count*/
};

struct CheckerParameters
{
	CheckerCallbacks callbacks;			/*!< callbacks*/
	std::vector<std::string> extensions;/*!< supported file extensions*/
};

/*@brief Resource checker*/
class ResourceChecker : public Resources
{
private:
	CheckerParameters m_parameters;				/*!< parameters*/
	std::filesystem::path m_resourceDirectory;	/*!< directory of resources*/

	static uint32_t computeSrc(const std::string& a_filename);

	void addTo(BinHeader&& a_header, const Binary&& a_bin, Resources& a_other);
public:
	ResourceChecker() = delete;
	explicit ResourceChecker(const CheckerParameters& a_parameters);
	explicit ResourceChecker(const CheckerParameters& a_parameters, const std::string& a_searchDirectory);
	void load(const std::string& a_directory);

	// compare with old version
	std::vector<Delta> compare(const Resources& a_old)const;

	template<typename CompilerImpl> requires std::is_base_of_v<Compiler, CompilerImpl>
	void mergeTo(Resources& a_other, CompilerImpl& a_compiler)
	{
		auto deltaList = compare(a_other);
		unsigned int errorCounter = 0;
		unsigned int progression = 0;
		const auto range = static_cast<unsigned int>(deltaList.size());
		for (auto& delta : deltaList)
		{
			switch (delta.type)
			{
			case DeltaType::delta_new:
			case DeltaType::delta_updated:
			{
				const auto filename = std::string(delta.heading.filename.data());
				auto compilation = a_compiler.compile(m_resourceDirectory.string() + "\\" + filename);
				if (compilation.has_value())
				{
					a_other.Resources::emplace(std::move(delta.heading), std::move(compilation.value()));
				}
				else
				{
					errorCounter++;
					m_parameters.callbacks.errorCounterCallback(errorCounter);
					m_parameters.callbacks.logCallback(compilation.error().errorMessage());
				}
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
			m_parameters.callbacks.rangeCallback(++progression, range);
		}
	}
};
