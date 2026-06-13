#pragma once
#include <qcombobox.h>
template<typename Type>
void enumerates(QComboBox* combobox) {}

template<typename Type>
void enumeratesFlags(QComboBox* combobox) {}


#define BEGIN_ENUM(Type) \
template<> \
void enumerates<Type>(QComboBox* combobox) \
{ \

#define VALUE_ENUM(value)\
	combobox->addItem(to_string(value), static_cast<int>(value)); \

#define END_ENUM \
}

#define BEGIN_FLAG(Type) \
template<> \
void enumeratesFlags<Type>(QComboBox* combobox) \
{ \

#define VALUE_FLAG(value) \
	combobox->addItem(to_string(value), static_cast<int>(value)); \
	combobox->setItemData(combobox->count(), Qt::Unchecked, Qt::CheckStateRole); \

#define END_FLAG \
}