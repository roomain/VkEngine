#include "VulkanTreeItem.h"
#include <ranges>

VulkanTreeItem::VulkanTreeItem(const QString& a_name, const QIcon& a_icon, VulkanTreeItem* a_parent) :
	m_name{ a_name }, m_icon{ a_icon }, m_parent{ a_parent }
{
	if (m_parent)
		m_parent->addChild(this);
}

VulkanTreeItem::~VulkanTreeItem()
{
	if (m_parent)
		m_parent->removeChild(this);

	std::vector<VulkanTreeItem*> temp;
	swap(temp, m_children);
	for (auto child : temp)
		delete child;
}

void VulkanTreeItem::removeChild(VulkanTreeItem* a_child)
{
	if (auto iter = std::ranges::find(m_children, a_child); iter != m_children.cend())
		m_children.erase(iter);
}

void VulkanTreeItem::addChild(VulkanTreeItem* a_child)
{
	m_children.emplace_back(a_child);
}

QVariant VulkanTreeItem::displayRole()const
{
	return m_name;
}

QVariant VulkanTreeItem::decorationRole()const
{
	return m_icon;
}

QVariant VulkanTreeItem::tooltipRole()const
{
	return m_name;
}

VulkanTreeItem* VulkanTreeItem::child(const int a_index)
{
	return m_children[a_index];
}

int VulkanTreeItem::count()const
{
	return static_cast<int>(m_children.size());
}

int VulkanTreeItem::indexInparent()const
{
	int index = 0;
	if (m_parent)
	{
		if (auto iter = std::ranges::find(m_parent->m_children, this); iter != m_parent->m_children.cend())
		{
			index = iter - m_parent->m_children.begin();
		}
	}
	return index;
}

VulkanTreeItem* VulkanTreeItem::parent()const
{
	return m_parent;
}