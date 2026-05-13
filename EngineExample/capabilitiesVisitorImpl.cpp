#include <iostream>
#include "CapabilitiesVisitorImpl.h"

void CapabilitiesConsoleVisitorImpl::setupTabs()const
{
	int index = 0;
	while (index < m_tabCount)
	{
		++index;
		std::cout << "\t";
	}
}

void CapabilitiesConsoleVisitorImpl::beginNode(const char* a_title)
{
	std::cout << "\n";
	setupTabs();
	std::cout << "* " << a_title << "\n";
	m_tabCount++;
}

void CapabilitiesConsoleVisitorImpl::endNode()
{
	m_tabCount--;
}

void CapabilitiesConsoleVisitorImpl::benginDataArray(const char* a_title)
{
	setupTabs();
	std::cout << "+ " << a_title << "\n";
	setupTabs();
	std::cout << "**************************************************\n";
	m_isArray = true;
}

void CapabilitiesConsoleVisitorImpl::endDataArray()
{
	setupTabs();
	std::cout << "**************************************************\n\n";
	m_isArray = false;
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const bool a_value)
{
	setupTabs();
	std::cout << std::boolalpha;
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const int32_t a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const uint32_t a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const float a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const double& a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const char* a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const std::string_view& a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << a_value << "\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << a_value << "\n";
	}
}

void CapabilitiesConsoleVisitorImpl::visitData(const char* a_title, const std::vector<std::string>& a_value)
{
	setupTabs();
	if (m_isArray)
	{
		std::cout << a_title << " : " << "[\n";
		for (const auto& val : a_value)
		{
			setupTabs();
			std::cout << "\t" << val << "\n";
		}
		setupTabs();
		std::cout << "\t]\n";
		setupTabs();
		std::cout << "-------------------------------------------------\n";
	}
	else
	{
		std::cout << "- " << a_title << ": " << "[\n";
		for (const auto& val : a_value)
		{
			setupTabs();
			std::cout << "\t" << val << "\n";
		}
		setupTabs();
		std::cout << "\t]\n";
	}
}
