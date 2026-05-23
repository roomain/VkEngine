#include "VkCapabiltyVisitorImpl.h"
#include "VulkanTreeModel.h"
#include "VulkanTreeItem.h"
#include "VulkanTableModel.h"

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
		QIcon icon;
		QString temp(a_title);
		if (temp == "Extensions")
		{
			icon = QIcon(":/VkConfigurator/resources/extension.png");
		}
		else if (temp == "Layers")
		{
			icon = QIcon(":/VkConfigurator/resources/layer.png");
		}
		else if (temp == "Queue family")
		{
			icon = QIcon(":/VkConfigurator/resources/queueList.png");
		}
		else if (temp.contains("Queue family"))
		{
			icon = QIcon(":/VkConfigurator/resources/queue.png");
		}
		else if (m_current)
		{
			if(m_current->displayRole().toString() == "Devices")
				icon = QIcon(":/VkConfigurator/resources/device.png");
		}
		m_current = new VulkanTreeItem(a_title, icon, m_current);
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
		m_current->data().addData(a_title, VulkanTableModel::Data{ a_value });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const int32_t a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ a_value });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const uint32_t a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ a_value });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const float a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ a_value });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const double& a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ a_value });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const char* a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ QString(a_value) });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const std::string_view& a_value)
{
	if (m_current)
	{
		m_current->data().addData(a_title, VulkanTableModel::Data{ QString::fromStdString(std::string(a_value)) });
	}
}

void VkCapabiltyVisitorImpl::visitData(const char* a_title, const std::vector<std::string>& a_value)
{
	int index = 0;
	bool isExtension = std::string(a_title).compare("Extensions") == 0;
	for (auto& value : a_value)
	{
		QString link = isExtension ? QString("https://docs.vulkan.org/refpages/latest/refpages/source/%1.html").arg(QString::fromStdString(value).split(" ")[0]) : "";
		m_current->data().addData(QString("%1 [%2]").arg(a_title).arg(index), VulkanTableModel::Data{ QString::fromStdString(value), link });
		index++;
	}
}

VulkanTreeModel* VkCapabiltyVisitorImpl::createModel()
{
	auto model = new VulkanTreeModel(m_root);
	m_root = nullptr;
	return model;
}