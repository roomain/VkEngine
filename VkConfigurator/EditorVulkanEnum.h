#pragma once
#include <qstandarditemmodel.h>
#include <qcombobox.h>

#define BEGIN_ENUM(type) \
template<typename Type> \
QComboBox* genEditor() { \
	QComboBox* cmbo = new QComboBox(); \
	auto *model = new QStandardItemModel(cmbo); \
	QStandardItem* item = nullptr; \

#define VALUE_ENUM(value) \
	item = new QStandardItem(#value); \
	item->setData(static_cast<unsigned int>(value), Qt::UserRole); \
	model->appendRow(item);


#define END_ENUM \
	cmbo->setModel(model); \
	return cmbo; \
} \

//-------------------------------------------------------------------------------------

#define BEGIN_FLAG(FlagBits, Flags) \
template<typename FlagBits> \
QComboBox* genEditor() { \
	\
		QComboBox* cmbo = new QComboBox(); \
		auto* model = new QStandardItemModel(cmbo); \
		QStandardItem* item = nullptr; \

#define VALUE_FLAG(FlagBits) \
	item = new QStandardItem(#FlagBits); \
	item->setData(static_cast<unsigned int>(FlagBits), Qt::UserRole); \
	item->setData(Qt::Unchecked, Qt::CheckStateRole); \
	model->appendRow(item);

#define END_FLAG \
	cmbo->setModel(model); \
	return cmbo; \
} \