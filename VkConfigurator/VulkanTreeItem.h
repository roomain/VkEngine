#pragma once
#include <vector>
#include <qicon.h>
#include "VulkanTableModel.h"

class VulkanTreeItem
{
private:
	VulkanTreeItem* m_parent = nullptr;
	std::vector<VulkanTreeItem*> m_children;
	QString m_name;
	QIcon m_icon;
	VulkanTableModel m_data;


public:
	VulkanTreeItem() = delete;
	explicit VulkanTreeItem(const QString& a_name, const QIcon& a_icon = QIcon(), VulkanTreeItem* a_parent = nullptr);
	~VulkanTreeItem();
	QVariant displayRole()const;
	QVariant decorationRole()const;
	QVariant tooltipRole()const;
	void addChild(VulkanTreeItem* a_child);
	VulkanTreeItem* child(const int a_index);
	int count()const;
	int indexInparent()const;
	VulkanTreeItem* parent()const;
	void removeChild(VulkanTreeItem* a_child);
	VulkanTableModel& data() { return m_data; }
};