#pragma once
#include <qcombobox.h>

#define BEGIN_ENUM(type) \
template<typename Type> \
void setEditorValue(QComboBox* a_cbo, const type a_value) { \
	int index = a_cbo->findData(static_cast<unsigned int>(a_value), Qt::UserRole); \
	a_cbo->setCurrentIndex(index); \



#define END_ENUM \
} \

//-------------------------------------------------------------------------------------

#define BEGIN_FLAG(FlagBits, Flags) \
template<typename FlagBits> \
void setEditorValue(QComboBox* a_cbo, const FlagBits a_value) { \
	int selected = -1;													\
	for(int index = 0; index < a_cbo->count(); ++index)					\
	{																	\
		unsigned int value = a_cbo->data(index, Qt::UserRole).toUInt(); \
		if (static_cast<unsigned int>(FlagBits) & value != 0)			\
		{																\
			a_cbo->setData(index, Qt::Checked, Qt::CheckStateRole);		\
			selected = index;											\
		}																\
		else															\
		{																\
			a_cbo->setData(index, Qt::Unchecked, Qt::CheckStateRole);	\
		}																\
	}																	\
	a_cbo->setCurrentIndex(selected);

#define END_FLAG \
} \
