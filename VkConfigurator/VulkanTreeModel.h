#pragma once
#include <memory>
#include <vector>
#include <qicon.h>
#include <QAbstractItemModel>

class VulkanTableModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	QList<QPair<QString, QVariant>> m_table;

public:
	explicit VulkanTableModel(QObject* parent = nullptr);
	~VulkanTableModel() = default;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void addData(const QString& a_title, const QVariant& a_value);
};

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

class VulkanTreeModel  : public QAbstractItemModel
{
	Q_OBJECT

private:
	VulkanTreeItem* m_root;

public:
	explicit VulkanTreeModel(VulkanTreeItem* a_root, QObject *parent = nullptr);
	~VulkanTreeModel();

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};

