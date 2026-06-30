#pragma once

#include <QMainWindow>
#include "ui_ConfigurationEditor.h"
class VulkanTreeItem;
class IEditNode;

class ConfigurationEditor : public QMainWindow
{
	Q_OBJECT

public:
	ConfigurationEditor(QWidget *parent = nullptr);
	~ConfigurationEditor() = default;

signals:
	void loadedFinished();

private:
	Ui::ConfigurationEditorClass ui;
	QString m_currentFile;

	void enableActions();

	static void copyTo(IEditNode* pNode, const QVariant& data);
	void copyTo(const QString& path, const QVariant& data);
	void onCopyLayer(VulkanTreeItem* pItem, const QVariant& data);
	void onCopyExtension(VulkanTreeItem* pItem, const QVariant& data);
	void addToVector(std::vector<std::string>& vData, const std::string& text);
	static bool isInstanceProperty(VulkanTreeItem* pItem);

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

	void onProfileCtxMenu(const QPoint& pt);
	void onCopy(VulkanTreeItem* pItem, const QVariant& data);
};

