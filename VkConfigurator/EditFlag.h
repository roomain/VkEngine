#pragma once

#include <QWidget>
#include "ui_EditFlag.h"
#include <map>

class EditFlag : public QWidget
{
	Q_OBJECT

public:
	EditFlag(QWidget *parent = nullptr);
	~EditFlag();
	unsigned int get()const;
	void set(const unsigned int a_data);
	void init(const std::map<unsigned int, std::string>& a_data);

private:
	Ui::EditFlagClass ui;
	std::map<unsigned int, std::string> m_availables;
	void internalAdd(const QString& a_data, const QVariant& value);

private slots:
	void onAdd();
	void onRemove();

};

