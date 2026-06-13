#pragma once
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <QStyledItemDelegate>

struct Editor
{
	std::function<QWidget* ()> genEditor;
	std::function<void (QWidget*, const QVariant&)> setEditorValue;
	std::function<QVariant(const QWidget*)> getEditorValue;
};

class EditionNodeDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	EditionNodeDelegate(QObject *parent);
	~EditionNodeDelegate();

	void paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	QWidget* createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

protected:
	bool editorEvent(QEvent* event, QAbstractItemModel* model,
		const QStyleOptionViewItem& option, const QModelIndex& index) override;
public:
	static std::unordered_map<std::type_index, Editor> s_editorGenerator;
};

