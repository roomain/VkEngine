#pragma once
#include <qstyleditemdelegate.h>

class EditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EditorDelegate(QObject* parent = nullptr);
    ~EditorDelegate() = default;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

