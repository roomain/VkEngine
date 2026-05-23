#include "VkCapabiltyVisitorImpl.h"
#include "VulkanTreeModel.h"

VkCapabiltyVisitorImpl::~VkCapabiltyVisitorImpl()
{
	delete m_root;
}

void VkCapabiltyVisitorImpl::beginNode(const char* a_title)
{
	if (m_root == nullptr)
	{
		m_current = new VulkanTreeItem(a_title);
		m_root = m_current;
	}
	else
	{
		m_current = new VulkanTreeItem(a_title, QIcon(), m_current);
	}

}

void VkCapabiltyVisitorImpl::endNode()
{
	m_current = m_current->parent();
}

void VkCapabiltyVisitorImpl::benginDataArray(const char* a_title)
{
	//
}

void VkCapabiltyVisitorImpl::endDataArray()
{
	//
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const bool a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, a_value);
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const int32_t a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, a_value);
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const uint32_t a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, a_value);
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const float a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, a_value);
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const double& a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, a_value);
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const char* a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, QString(a_value));
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const std::string_view& a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, QString::fromStdString(std::string(a_value)));
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const std::vector<std::string>& a_value)
{
	int index = 0;
	for (auto& value : a_value)
	{
		m_current->data().addData(QString("%1 [%2]").arg(a_title).arg(index), QString::fromStdString(value));
		index++;
	}
}

VulkanTreeModel* VkCapabiltyVisitorImpl::createModel()
{
	auto model = new VulkanTreeModel(m_root);
	m_root = nullptr;
	return model;
}