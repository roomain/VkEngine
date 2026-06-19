#include "EditionNodeDelegate.h"
#include "IEditNode.h"
#include "EditModel.h"
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
#include <QPainter>

std::unordered_map<std::type_index, Editor> EditionNodeDelegate::s_editorGenerator = {
	{ std::type_index(typeid(VkQueueFlagBits)), {
			[](QWidget* parent) {return new FlagCombo<VkQueueFlagBits>(parent); },
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
			[](QWidget* parent) {return new QSpinBox(parent); },
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
			[](QWidget* parent) {return new QDoubleSpinBox(parent); },
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
			[](QWidget* parent) {return new QDoubleSpinBox(parent); },
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
			[](QWidget* parent) {return new QLineEdit(parent); },
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
			[](QWidget* parent) {return new QCheckBox(parent); },
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

QStyleOptionButton EditionNodeDelegate::genButton(QPainter* painter, const int index, const int row, const QRect& rect, 
	const QString& label)const
{
	QStyleOptionButton btn;
	btn.rect = rect;
	btn.features = QStyleOptionButton::DefaultButton;
	btn.state = QStyle::State_Active | QStyle::State_Enabled;
	btn.text = label;
	auto pen = painter->pen();
	painter->setPen(QPen(QColor(71, 138, 255), 2));
	if (index == m_btnEvent.buttonIndex && row == m_btnEvent.buttonRow)
	{
		switch (m_btnEvent.evntType)
		{
		case QEvent::MouseButtonPress:
			btn.state |= QStyle::State_Selected | QStyle::State_MouseOver;
			painter->setPen(QPen(QColor(255, 20, 50), 4));
			break;

		case QEvent::MouseMove:
			btn.state |= QStyle::State_MouseOver;
			painter->setPen(QPen(QColor(255, 146, 30), 2));
			break;
		}

	}
	painter->drawRoundedRect(rect, 4, 4);

	QApplication::style()->drawControl(
		QStyle::CE_PushButtonLabel, &btn, painter);

	painter->setPen(pen);
	return btn;
}


void EditionNodeDelegate::paint(QPainter* painter,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditNode*>(index.constInternalPointer());
	if (pNode && pNode->isContainer() && index.column() == 0)
	{
		QStyleOptionButton btnAdd = genButton(painter, 0, index.row(), QRect(
			option.rect.right() - option.rect.height(),
			option.rect.top(),
			option.rect.height(),
			option.rect.height()), "+");

		QStyleOptionButton btnRm = genButton(painter, 1, index.row(), QRect(
			option.rect.right() -  2 * option.rect.height() - 4,
			option.rect.top(),
			option.rect.height(),
			option.rect.height()), "-");

		auto textRect = option.rect;
		textRect.setWidth(option.rect.width() - 2 * option.rect.height() - 4);
		QApplication::style()->drawItemText(painter, textRect, QStyle::State_None, option.palette,
			true, pNode->displayRole().toString());
	}
	else
	{
		QStyledItemDelegate::paint(painter, option, index);
	}
}

QWidget* EditionNodeDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditNode*>(index.constInternalPointer());
	if (auto iter = s_editorGenerator.find(pNode->type()); iter != s_editorGenerator.end())
		return iter->second.genEditor(parent);
	
	return nullptr;
}

void EditionNodeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditNode*>(index.constInternalPointer());
	s_editorGenerator[pNode->type()].setEditorValue(editor, pNode->getValue());
}

void EditionNodeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	auto pNode = static_cast<const IEditNode*>(index.constInternalPointer());
	auto value = s_editorGenerator[pNode->type()].getEditorValue(editor);
	model->setData(index, value);
}

QString EditionNodeDelegate::displayText(const QVariant& value, const QLocale& locale)const
{
	if (value.metaType() == QMetaType::fromType<VkQueueFlagBits>())
		return QString::fromStdString(Flag<VkQueueFlagBits>::to_string(value.value<VkQueueFlagBits>()));

	return QStyledItemDelegate::displayText(value, locale);
}

bool EditionNodeDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
	const QStyleOptionViewItem& option, const QModelIndex& index)
{
	auto pNode = static_cast<IEditNode*>(index.internalPointer());

	if(index.column() != 0 || !pNode->isContainer())
		return QStyledItemDelegate::editorEvent(event, model, option, index);

	m_btnEvent.buttonIndex = -1;
	m_btnEvent.buttonRow = index.row();
	m_btnEvent.evntType = event->type();
	if (auto pEvnt = dynamic_cast<QMouseEvent*>(event))
	{
		QRect btnAdd(option.rect.right() - option.rect.height(),
			option.rect.top(),
			option.rect.height(),
			option.rect.height());

		QRect btnRm(option.rect.right() - 2 * option.rect.height() - 4,
			option.rect.top(),
			option.rect.height(),
			option.rect.height());
		

		EditModel* pModel = static_cast<EditModel*>(model);
		if (btnAdd.contains(pEvnt->pos()))
		{
			m_btnEvent.buttonIndex = 0;
			if (m_btnEvent.evntType == QEvent::MouseButtonRelease)
			{
				pModel->beginAddRow(index);
				pNode->append();
				pModel->endAddRow();
				return true;
			}
		}

		if (btnRm.contains(pEvnt->pos()))
		{
			m_btnEvent.buttonIndex = 1;
			if (m_btnEvent.evntType == QEvent::MouseButtonRelease && pModel->rowCount(index) > 0)
			{
				pModel->beginRemRow(index);
				pNode->remove();
				pModel->endRemRow();
				return true;
			}
		}

		
	}


	return QStyledItemDelegate::editorEvent(event, model, option, index);
}
