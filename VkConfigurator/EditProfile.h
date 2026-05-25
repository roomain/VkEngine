#pragma once

#include <QDialog>
#include "ui_EditProfile.h"

class EditProfile : public QDialog
{
	Q_OBJECT

public:
	EditProfile(QWidget *parent = nullptr);
	~EditProfile();

private:
	Ui::EditProfileClass ui;
};

