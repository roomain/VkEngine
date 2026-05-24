#include "EditFlag.h"

EditFlag::EditFlag(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.pBtnAdd, qOverload<bool>(&QPushButton::clicked), this, &EditFlag::onAdd);
}

EditFlag::~EditFlag()
{}


unsigned int EditFlag::get()const
{
	unsigned int flag = 0;
	for (int row = 0; row < ui.twdgData->rowCount(); ++row)
	{
		flag |= ui.twdgData->item(row, 1)->data(Qt::UserRole).toUInt();
	}
	return flag;
}

void EditFlag::set(const unsigned int a_data)
{
	init(m_availables);
	for (int index = 0; index < ui.cboData->count(); ++index)
	{
		if ((ui.cboData->itemData(index, Qt::UserRole).toUInt() & a_data) != 0)
		{
			internalAdd(ui.cboData->itemText(index), ui.cboData->itemData(index, Qt::UserRole));
			--index;
		}
	}
}

void EditFlag::init(const std::map<unsigned int, std::string>& a_data)
{
	if (&m_availables != &a_data)
	{
		m_availables.clear();
		m_availables = a_data;
	}
	ui.twdgData->clear();
	ui.cboData->clear();

	for (const auto& [value, str] : a_data)
	{
		ui.cboData->addItem(QIcon(), QString::fromStdString(str), value);
	}
}

void EditFlag::internalAdd(const QString& a_data, const QVariant& value)
{
	QTableWidgetItem* item = new QTableWidgetItem(a_data);
	item->setData(Qt::UserRole, value);
	int iRow = ui.twdgData->rowCount();
	ui.twdgData->setRowCount(iRow + 1);

	QPushButton* pRemoveBtn = new QPushButton("-");
	ui.twdgData->setItem(iRow, 1, item);
	ui.twdgData->setCellWidget(iRow, 0, pRemoveBtn);
	QObject::connect(pRemoveBtn, qOverload<bool>(&QPushButton::clicked), this, &EditFlag::onRemove);

	int index = 0;
	while (a_data != ui.cboData->itemText(index))
		index++;
	ui.cboData->removeItem(index);
}

void EditFlag::onAdd()
{
	internalAdd(ui.cboData->currentText(), ui.cboData->currentData(Qt::UserRole));
}

void EditFlag::onRemove()
{
	auto widget = static_cast<QWidget*>(sender());
	int row = 0;
	while (ui.twdgData->cellWidget(row, 0) != widget)
		row++;
	ui.twdgData->removeRow(row);
}