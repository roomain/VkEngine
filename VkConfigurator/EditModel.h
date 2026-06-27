#pragma once
#include <vector>
#include "IEditNode.h"
#include <QAbstractItemModel>

class EditModel  : public QAbstractItemModel
{
	Q_OBJECT

private:
	std::vector<IEditNode*> m_classes;
	static const int COLUMN_COUNT = 2;

	QVariant column0(IEditNode* pNode, int role)const;
	QVariant column1(IEditNode* pNode, int role)const;
	Qt::ItemFlags flags_column_0(IEditNode* pNode) const;
	Qt::ItemFlags flags_column_1(IEditNode* pNode) const;

	static IEditNode* find(std::vector<IEditNode*>::const_iterator begin, std::vector<IEditNode*>::const_iterator end, const QString& name);

public:
	EditModel(QObject *parent = nullptr);
	~EditModel();
	void clear()
	{
		beginResetModel();
		for (auto pNode : m_classes)
			delete pNode;
		m_classes.clear();
		endResetModel();
	}

	QModelIndex addClass(IEditNode* pNode)
	{
		beginAddRow(QModelIndex());
		m_classes.push_back(pNode);
		endAddRow();
		return index(static_cast<int>(m_classes.size()) - 1, 0);
	}

	IEditNode* findNode(const QString& name, QModelIndex& index)const;

	void save(const std::string& profile);

	void startInit();
	void endInit();

	void beginAddRow(const QModelIndex& model);
	void endAddRow();
	void beginRemRow(const QModelIndex& model);
	void endRemRow();

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

