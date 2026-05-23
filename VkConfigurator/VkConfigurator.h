#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VkConfigurator.h"
#include "VkCapabiltyVisitorImpl.h"

class VkConfigurator : public QMainWindow
{
    Q_OBJECT

public:
    VkConfigurator(QWidget *parent = nullptr);
    ~VkConfigurator();

private:
    Ui::VkConfiguratorClass ui;
    VkCapabiltyVisitorImpl m_visitor;

private slots:
    void onOpenFile();
    void onSaveFile();
    void onSaveAsFile();
    void onCapabilitySelected(const QModelIndex& current);
};

