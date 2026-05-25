#pragma once

#include <QWidget>
#include "ui_CapabilityExplorer.h"
#include "VkCapabiltyVisitorImpl.h"
#include "VulkanCapabilities.h"

class CapabilityExplorer : public QWidget
{
	Q_OBJECT

public:
	CapabilityExplorer(QWidget *parent = nullptr);
	~CapabilityExplorer();

private:
	Ui::CapabilityExplorerClass ui;
	VkCapabiltyVisitorImpl m_visitor;

private slots:
	void onCapabilitySelected(const QModelIndex& current);
	void onCapabilityTableSelected(const QModelIndex& current);
};

