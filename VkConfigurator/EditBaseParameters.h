#pragma once

#include <QWidget>
#include "ui_EditBaseParameters.h"

class EditBaseParameters : public QWidget
{
	Q_OBJECT

public:
	EditBaseParameters(QWidget *parent = nullptr);
	~EditBaseParameters();

private:
	Ui::EditBaseParametersClass ui;
};

