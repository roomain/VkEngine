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
	void enableActions();

public slots:
	void onNewConfiguration();
	void onOpenFile();
	void onSaveFile();
	void onSaveAsFile();
	void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
	void onNewProfile();

	void onRenameProfile();
	void onDeleteProfile();

	void onAddEngineParameters();
	void onQueueParameters();
	void onDeviceParameters();
	void onDeviceFeatures();
};

