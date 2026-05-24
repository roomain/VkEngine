#include "StringListWidget.h"

StringListWidget::StringListWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.pBtnAdd, qOverload<bool>(&QPushButton::clicked), this, &StringListWidget::onAdd);
}

StringListWidget::~StringListWidget()
{}

void StringListWidget::get(std::vector<std::string>& a_data)const
{
	a_data.clear();
	for (int row = 0; row < ui.twdgData->rowCount(); ++row)
	{
		a_data.push_back(ui.twdgData->item(row, 1)->text().toStdString());
	}
}

void StringListWidget::set(const std::vector<std::string>& a_data)
{
	init(m_availables);
	for (const auto& str : a_data)
		internalAdd(QString::fromStdString(str));
}

void StringListWidget::init(const std::vector<std::string>& a_data)
{
	if (&m_availables != &a_data)
	{
		m_availables.clear();
		m_availables = a_data;
	}
	ui.twdgData->clear();
	ui.cboData->clear();

	for (const auto& str : a_data)
	{
		ui.cboData->addItem(QIcon(), QString::fromStdString(str));
	}
}

void StringListWidget::internalAdd(const QString& a_data)
{
	QTableWidgetItem* item = new QTableWidgetItem(a_data);
	int iRow = ui.twdgData->rowCount();
	ui.twdgData->setRowCount(iRow + 1);

	QPushButton* pRemoveBtn = new QPushButton("-");
	ui.twdgData->setItem(iRow, 1, item);
	ui.twdgData->setCellWidget(iRow, 0, pRemoveBtn);
	QObject::connect(pRemoveBtn, qOverload<bool>(&QPushButton::clicked), this, &StringListWidget::onRemove);

	int index = 0;
	while (a_data != ui.cboData->itemText(index))
		index++;
	ui.cboData->removeItem(index);
}

void StringListWidget::onAdd()
{
	internalAdd(ui.cboData->currentText());
}

void StringListWidget::onRemove()
{
	auto widget = static_cast<QWidget*>(sender());
	int row = 0;
	while (ui.twdgData->cellWidget(row, 0) != widget)
		row++;
	ui.twdgData->removeRow(row);
}