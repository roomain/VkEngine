#pragma once
#include <functional>
#include <QString>
#include <QDialog>
#include "ui_EditProfileName.h"

using CheckProfile = std::function<bool(const QString&)>;

class EditProfileName : public QDialog
{
	Q_OBJECT

public:
	explicit EditProfileName(const CheckProfile& check, QWidget *parent = nullptr);
	~EditProfileName();
	QString profileName()const;

private:
	Ui::EditProfileNameClass ui;
	CheckProfile m_check;

	static QString generateName();

private slots:
	void onProfineNameChanged(const QString& name);
};

