#pragma once
/***********************************************
* @headerfile capabilitiesVisitorImpl.h
* @date 11 / 05 / 2026
* @author Roomain
************************************************/
#include "CapabilitiesVisitor.h"

class CapabilitiesConsoleVisitorImpl : public CapabilitiesVisitor
{
private:
	bool m_isArray = false;
	int m_tabCount = 0;
	void setupTabs()const;
public:
	void beginNode(const char* a_title) override;
	void endNode() override;

	/*@brief to display multiple data for single capavity*/
	void benginDataArray(const char* a_title) override;
	void endDataArray() override;

	void visitData(const char* a_title, const bool a_value) override;
	void visitData(const char* a_title, const int32_t a_value) override;
	void visitData(const char* a_title, const uint32_t a_value) override;
	void visitData(const char* a_title, const float a_value) override;
	void visitData(const char* a_title, const double& a_value) override;
	void visitData(const char* a_title, const char* a_value) override;
	void visitData(const char* a_title, const std::string_view& a_value) override;
	void visitData(const char* a_title, const std::vector<std::string>& a_value) override;
};
