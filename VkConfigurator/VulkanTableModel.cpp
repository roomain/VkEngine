#include "VulkanTableModel.h"

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
			return m_table[index.row()].second.m_display;
			//if (m_table[index.row()].second.m_link.isEmpty())
			//	return m_table[index.row()].second.m_display;
			//else
			//	return QString("<a href='%1'>%2</a>").arg(m_table[index.row()].second.m_link).arg(m_table[index.row()].second.m_display.toString());
		}
		break;
	case Qt::ToolTipRole:
		if (index.column() == 1)
		{
			if (m_table[index.row()].second.m_link.isEmpty())
			{
				return m_table[index.row()].second.m_display;
			}
			else
			{
				return QString("<a href='https://docs.vulkan.org/refpages/latest/refpages/source/%1'>%2</a>").arg(m_table[index.row()].second.m_link).arg(m_table[index.row()].second.m_display.toString());
			}
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

void VulkanTableModel::addData(const QString& a_title, const Data& a_value)
{
	m_table.append(QPair(a_title, a_value));
}