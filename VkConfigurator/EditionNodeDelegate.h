#pragma once
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <QStyledItemDelegate>

struct Editor
{
	std::function<QWidget* (QWidget*)> genEditor;
	std::function<void (QWidget*, const QVariant&)> setEditorValue;
	std::function<QVariant(const QWidget*)> getEditorValue;
};

class EditionNodeDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	EditionNodeDelegate(QObject *parent = nullptr);
	~EditionNodeDelegate() = default;

	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	QWidget* createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
	QString displayText(const QVariant& value, const QLocale& locale)const override;

signals:
	void expandNode(const QModelIndex&);

protected:
	bool editorEvent(QEvent* event, QAbstractItemModel* model,
		const QStyleOptionViewItem& option, const QModelIndex& index) override;
public:
	static std::unordered_map<std::type_index, Editor> s_editorGenerator;

private:
	QStyleOptionButton genButton(QPainter* painter, const int index, const int row, const QRect& rect,
		const QString& label)const;

	struct EventBtn
	{
		int buttonIndex = -1;
		int buttonRow = -1;
		int evntType = 0;
	};

	EventBtn m_btnEvent;
};

