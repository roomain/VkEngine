#pragma once
#include <type_traits>
#include <qcombobox.h>

template<typename Type> requires std::is_enum_v<Type>
class EnumCombo : public QComboBox
{
public:
	EnumCombo(QWidget* parent) : QComboBox(parent)
	{
		enumerates<Type>(this);
	}

	~EnumCombo() = default;

	void setValue(const Type value)
	{
		setCurrentText(to_string(value));
	}

	Type value()const
	{
		return static_cast<Type>(currentData(Qt::UserRole).toInt());
	}
};

template<typename Type> requires std::is_enum_v<Type>
class FlagCombo : public QComboBox
{
public:
	FlagCombo(QWidget* parent = nullptr) : QComboBox(parent)
	{
		enumeratesFlags<Type>(this);
	}
	~FlagCombo() = default;

	void setValue(const Type value)
	{
		auto flag = static_cast<unsigned int>(value);
		for (int index = 0; index < count(); ++index)
		{
			if (flag & itemData(index, Qt::UserRole).toUInt())
			{
				setItemData(index, Qt::Checked, Qt::CheckStateRole);
			}
			else
			{
				setItemData(index, Qt::Unchecked, Qt::CheckStateRole);
			}
		}
	}

	Type value()const
	{
		unsigned int flag = 0;
		for (int index = 0; index < count(); ++index)
		{
			if (itemData(index, Qt::CheckStateRole).toInt() == Qt::Checked)
				flag |= itemData(index, Qt::UserRole).toUInt();
		}
		return static_cast<Type>(flag);
	}
};