#include "VulkanTreeModel.h"
#include "VulkanTreeItem.h"


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
	auto item = reinterpret_cast<VulkanTreeItem*>(parent.internalPointer());
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
	auto item = reinterpret_cast<VulkanTreeItem*>(index.internalPointer());
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
