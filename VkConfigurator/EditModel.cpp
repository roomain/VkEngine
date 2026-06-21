#include "EditModel.h"
#include <ranges>
#include "IEditNode.h"

EditModel::EditModel(QObject *parent)
	: QAbstractItemModel(parent)
{}

EditModel::~EditModel()
{}

void EditModel::beginAddRow(const QModelIndex& model)
{
	int size = rowCount(model);
	beginInsertRows(model, size, size);
}

void EditModel::endAddRow()
{
	endInsertRows();
}

void EditModel::save(const std::string& profile)
{
	for (const auto node : m_classes)
		node->save(profile);
}

void EditModel::startInit()
{
	beginResetModel();
}

void EditModel::endInit()
{
	endResetModel();
}

void EditModel::beginRemRow(const QModelIndex& model)
{
	int size = rowCount(model);
	beginRemoveRows(model, size - 1, size - 1);
}

void EditModel::endRemRow()
{
	endRemoveRows();
}

QVariant EditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return "Optional";
		case 1:
			return "Variable";
		case 2:
			return "Value";
		}
	}
	return QVariant();
}

QModelIndex EditModel::index(int row, int column, const QModelIndex& parent) const
{
	auto item = reinterpret_cast<IEditNode*>(parent.internalPointer());
	if (item)
		return createIndex(row, column, item->child(row));
	return createIndex(row, column, m_classes[row]);
}

QModelIndex EditModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
		return QModelIndex();

	auto node = reinterpret_cast<IEditNode*>(child.internalPointer());
	
	if(node == nullptr)
		return QModelIndex();

	auto parent = node->parent();

	if (parent)
		return createIndex(node->indexInparent(), 0, parent);
	else
	{
		auto iter = std::ranges::find(m_classes, node);
		int index = iter - m_classes.begin();
		return createIndex(index, 0, nullptr);
	}

	return QModelIndex();
}

int EditModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
		return static_cast<int>(m_classes.size());

	auto item = reinterpret_cast<IEditNode*>(parent.internalPointer());
	if (item)
		return static_cast<int>(item->childCount());
	else
		return static_cast<int>(m_classes.size());
	return 0;
}

int EditModel::columnCount(const QModelIndex& parent) const
{
	return COLUMN_COUNT;
}

Qt::ItemFlags EditModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	auto item = reinterpret_cast<IEditNode*>(index.internalPointer());
	if (item)
	{
		switch (index.column())
		{
		case 0:
			return flags_column_0(item);
		case 1:
			return flags_column_1(item);
		}
	}
	return flags;
}

bool EditModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	auto item = reinterpret_cast<IEditNode*>(index.internalPointer());
	if (item)
	{
		item->setValue(value);
	}
	return true;
}

Qt::ItemFlags EditModel::flags_column_0(IEditNode* pNode) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (pNode->checkRole().isValid())
		flags = flags | Qt::ItemIsUserCheckable;
	return flags;
}

Qt::ItemFlags EditModel::flags_column_1(IEditNode* pNode) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (!pNode->isStructure())
		flags = flags | Qt::ItemIsEditable;
	if (pNode->isBoolean())
		flags = flags | Qt::ItemIsUserCheckable;
	return flags;
}

QVariant EditModel::column0(IEditNode* pNode, int role)const
{
	switch (role)
	{
	case Qt::DisplayRole:
		return pNode->displayRole();

	case Qt::CheckStateRole:
		return pNode->checkRole();
	}
	return QVariant();
}

QVariant EditModel::column1(IEditNode* pNode, int role)const
{
	switch (role)
	{
	case Qt::DisplayRole:
		return pNode->isBoolean() ? QVariant() : pNode->getValue();

	case Qt::CheckStateRole:
		if (pNode->isBoolean())
			return pNode->getValue().toBool() ? Qt::Checked : Qt::Unchecked;
		break;
	}
	return QVariant();
}

QVariant EditModel::data(const QModelIndex& index, int role) const
{
	auto item = reinterpret_cast<IEditNode*>(index.internalPointer());
	if (item)
	{
		switch (index.column())
		{
		case 0:
			return column0(item, role);
		case 1:
			return column1(item, role);
		}
	}
	return QVariant();
}
