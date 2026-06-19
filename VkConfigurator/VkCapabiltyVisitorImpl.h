#pragma once
#include "CapabilitiesVisitor.h"

class VulkanTreeItem;
class VulkanTreeModel;

class VkCapabiltyVisitorImpl : public CapabilitiesVisitor
{
private:
	VulkanTreeItem* m_root = nullptr;
	VulkanTreeItem* m_current = nullptr;
	bool m_isArray = false;

public:
	~VkCapabiltyVisitorImpl();
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
	VulkanTreeModel* createModel();
};
