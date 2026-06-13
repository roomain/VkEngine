#include "EditorDelegate.h"


EditorDelegate::EditorDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
	//
}

QWidget* EditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//
	return nullptr;
}

void EditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	//
}

void EditorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	//
}