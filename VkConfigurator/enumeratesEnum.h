#pragma once
#include <qcombobox.h>
#include <qstandarditemmodel.h>
template<typename Type>
void enumerates(QComboBox* combobox) {}

template<typename Type>
void enumeratesFlags(QComboBox* combobox) {}


#define BEGIN_ENUM(Type) \
template<> \
void enumerates<Type>(QComboBox* combobox) \
{ 

#define VALUE_ENUM(value) \
	combobox->addItem(QString(#value), static_cast<int>(value)); 

#define END_ENUM \
}

#define BEGIN_FLAG(Base, Type) \
template<> \
void enumeratesFlags<Base>(QComboBox* combobox) \
{ \
	QStandardItem* item = nullptr;

#define VALUE_FLAG(value) \
	item = new QStandardItem(QString(#value)); \
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable); \
	item->setData(static_cast<unsigned int>(value), Qt::UserRole); \
	static_cast<QStandardItemModel*>(combobox->model())->appendRow(item);

#define END_FLAG \
}