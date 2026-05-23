#pragma once

#include <QWidget>
#include "ui_ConfigurationEditor.h"

class ConfigurationEditor : public QWidget
{
	Q_OBJECT

public:
	ConfigurationEditor(QWidget *parent = nullptr);
	~ConfigurationEditor();

signals:
	void loadedFinished();

private:
	Ui::ConfigurationEditorClass ui;


public slots:
	void onOpenFile();
	void onSaveFile();
	void onSaveAsFile();
};

