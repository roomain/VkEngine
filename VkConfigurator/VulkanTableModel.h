#pragma once
#include <QAbstractItemModel>


class VulkanTableModel : public QAbstractItemModel
{
	friend class VkCapabiltyVisitorImpl;
	Q_OBJECT
private:
	struct Data
	{
		QVariant m_display;
		QString m_link;
	};

	QList<QPair<QString, Data>> m_table;
	void addData(const QString& a_title, const Data& a_value);

public:
	explicit VulkanTableModel(QObject* parent = nullptr);
	~VulkanTableModel() = default;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

