#pragma once

#include <QWidget>
#include "ui_StringListWidget.h"
#include <vector>
#include <string>

class StringListWidget : public QWidget
{
	Q_OBJECT

public:
	StringListWidget(QWidget *parent = nullptr);
	~StringListWidget();
	void get(std::vector<std::string>& a_data)const;
	void set(const std::vector<std::string>& a_data);
	void init(const std::vector<std::string>& a_data);

private:
	Ui::StringListWidgetClass ui;
	std::vector<std::string> m_availables;
	void internalAdd(const QString& a_data);

private slots:
	void onAdd();
	void onRemove();
};

