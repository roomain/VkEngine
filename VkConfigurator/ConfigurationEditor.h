#pragma once

#include <QMainWindow>
#include "ui_ConfigurationEditor.h"

class ConfigurationEditor : public QMainWindow
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
	void onNewConfiguration();
	void onOpenFile();
	void onSaveFile();
	void onSaveAsFile();
	void onProfileSelected(const QModelIndex& current);
	void onNewProfile();

	void onRenameProfile();
	void onDeleteProfile();
};

