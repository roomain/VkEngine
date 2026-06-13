#include "EditionNodeDelegate.h"
#include "IEditLayerNode.h"
#include <string>
#include <qapplication.h>
#include <vulkan/vulkan.hpp>
#include "vkEnumCombo.h"
#include "EnumCombo.h"
#include <QMouseEvent>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <qlineedit.h>
#include <qcheckbox.h>

std::unordered_map<std::type_index, Editor> EditionNodeDelegate::s_editorGenerator = {
	{ std::type_index(typeid(VkQueueFlagBits)), {
			[]() {return new FlagCombo<VkQueueFlagBits>(); },
			[](QWidget* editor, const QVariant& value) 
			{
				auto pEditor = static_cast<FlagCombo<VkQueueFlagBits>*>(editor);
				pEditor->setValue(value.value<VkQueueFlagBits>());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const FlagCombo<VkQueueFlagBits>*>(editor);
				return pEditor->value();
			}
		}
	},
	{ std::type_index(typeid(int)), {
			[]() {return new QSpinBox(); },
			[](QWidget* editor, const QVariant& value)
			{
				auto pEditor = static_cast<QSpinBox*>(editor);
				pEditor->setValue(value.toInt());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const QSpinBox*>(editor);
				return pEditor->value();
			}
		}
	},
	{ std::type_index(typeid(float)), {
			[]() {return new QDoubleSpinBox(); },
			[](QWidget* editor, const QVariant& value)
			{
				auto pEditor = static_cast<QDoubleSpinBox*>(editor);
				pEditor->setValue(value.toFloat());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const QDoubleSpinBox*>(editor);
				return pEditor->value();
			}
		}
	},
	{ std::type_index(typeid(double)), {
			[]() {return new QDoubleSpinBox(); },
			[](QWidget* editor, const QVariant& value)
			{
				auto pEditor = static_cast<QDoubleSpinBox*>(editor);
				pEditor->setValue(value.toDouble());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const QDoubleSpinBox*>(editor);
				return pEditor->value();
			}
		}
	},
	{ std::type_index(typeid(std::string)), {
			[]() {return new QLineEdit(); },
			[](QWidget* editor, const QVariant& value)
			{
				auto pEditor = static_cast<QLineEdit*>(editor);
				pEditor->setText(value.toString());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const QLineEdit*>(editor);
				return pEditor->text();
			}
		}
	},
	{ std::type_index(typeid(bool)), {
			[]() {return new QCheckBox(); },
			[](QWidget* editor, const QVariant& value)
			{
				auto pEditor = static_cast<QCheckBox*>(editor);
				pEditor->setChecked(value.toBool());
			},
			[](const QWidget* editor)
			{
				auto pEditor = static_cast<const QCheckBox*>(editor);
				return pEditor->isChecked();
			}
		}
	}
};

EditionNodeDelegate::EditionNodeDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

EditionNodeDelegate::~EditionNodeDelegate()
{}

void EditionNodeDelegate::paint(QPainter* painter,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	auto pNode = static_cast<const IEditLayerNode*>(index.constInternalPointer());
	if (pNode && pNode->isContainer())
	{
		QStyleOptionButton btnAdd;
		btnAdd.rect = QRect(option.rect.right() - 80,
			option.rect.top() + 2,
			35,
			option.rect.height() - 4);
		btnAdd.text = "+";

		QStyleOptionButton btnRm;
		btnRm.rect = QRect(option.rect.right() - 40,
			btnRm.rect.top() + 2,
			35,
			option.rect.height() - 4);
		btnRm.text = "-";

		QApplication::style()->drawControl(
			QStyle::CE_PushButton, &btnAdd, painter);

		QApplication::style()->drawControl(
			QStyle::CE_PushButton, &btnRm, painter);
	}
}

QWidget* EditionNodeDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditLayerNode*>(index.constInternalPointer());
	if (auto iter = s_editorGenerator.find(pNode->type()); iter != s_editorGenerator.end())
	{
		return iter->second.genEditor();
	}
	return nullptr;
}

void EditionNodeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditLayerNode*>(index.constInternalPointer());
	s_editorGenerator[pNode->type()];
}

void EditionNodeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditLayerNode*>(index.constInternalPointer());
	s_editorGenerator[pNode->type()];
}


bool EditionNodeDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
	const QStyleOptionViewItem& option, const QModelIndex& index)
{
	auto pNode = static_cast<IEditLayerNode*>(index.internalPointer());
	if (event->type() == QEvent::MouseButtonRelease && pNode->isContainer())
	{
		QMouseEvent* pEvnt = static_cast<QMouseEvent*>(event);

		QRect btnAdd(option.rect.right() - 80,
			option.rect.top() + 2,
			35,
			option.rect.height() - 4);

		QRect btnRm(option.rect.right() - 40,
			option.rect.top() + 2,
			35,
			option.rect.height() - 4);

		if (btnAdd.contains(pEvnt->pos()))
		{
			pNode->append();
			return true;
		}

		if (btnRm.contains(pEvnt->pos()))
		{
			pNode->remove();
			return true;
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}
