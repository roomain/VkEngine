#include "VulkanTreeModel.h"
#include <ranges>


VulkanTableModel::VulkanTableModel(QObject* parent) : QAbstractItemModel(parent)
{
	//
}

QModelIndex VulkanTableModel::index(int row, int column, const QModelIndex& parent) const
{
	return createIndex(row, column);
}

QModelIndex VulkanTableModel::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int VulkanTableModel::rowCount(const QModelIndex& parent) const
{
	return m_table.size();
}

int VulkanTableModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant VulkanTableModel::data(const QModelIndex& index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (index.column() == 0)
		{
			return m_table[index.row()].first;
		}
		else
		{
			return m_table[index.row()].second;
		}
		break;
	case Qt::ToolTipRole:
		if (index.column() == 1)
		{
			return m_table[index.row()].second;
		}
		break;
	default:
		break;
	}
	return QVariant();
}

QVariant VulkanTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (orientation == Qt::Orientation::Horizontal)
		{
			if (section == 0)
			{
				return "Title";
			}
			else
			{
				return "Value";
			}
		}
		break;
	default:
		break;
	}
	return QVariant();
}

void VulkanTableModel::addData(const QString& a_title, const QVariant& a_value)
{
	m_table.append(QPair(a_title, a_value));
}


VulkanTreeItem::VulkanTreeItem(const QString& a_name, const QIcon& a_icon, VulkanTreeItem* a_parent) :
	m_name{a_name}, m_icon{a_icon}, m_parent{a_parent}
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

VulkanTreeModel::VulkanTreeModel(VulkanTreeItem* a_root, QObject *parent)
	: QAbstractItemModel(parent), m_root{a_root}
{
}

VulkanTreeModel::~VulkanTreeModel()
{
	delete m_root;
}

QModelIndex VulkanTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	VulkanTreeItem* item = reinterpret_cast<VulkanTreeItem*>(parent.internalPointer());
	if(item)
		return createIndex(row, column, item->child(row));
	return createIndex(row, column, m_root);
}

QModelIndex VulkanTreeModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
		return QModelIndex();

	auto node = reinterpret_cast<VulkanTreeItem*>(child.internalPointer());
	auto parent = node->parent();

	if(parent)
		return createIndex(parent->indexInparent(), 0, parent);

	return QModelIndex();
}

int VulkanTreeModel::rowCount(const QModelIndex& parent) const
{
	VulkanTreeItem* item = reinterpret_cast<VulkanTreeItem*>(parent.internalPointer());
	if (item)
		return item->count();
	else
		return m_root ? 1 : 0;
	return 0;
}

int VulkanTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant VulkanTreeModel::data(const QModelIndex& index, int role) const
{
	VulkanTreeItem* item = reinterpret_cast<VulkanTreeItem*>(index.internalPointer());
	if (item)
	{
		switch (role)
		{
		case Qt::DisplayRole:
			return item->displayRole();
		case Qt::DecorationRole:
			return item->decorationRole();
		case Qt::ToolTipRole:
			return item->tooltipRole();
		default:
			break;
		}
	}
	return QVariant();
}
