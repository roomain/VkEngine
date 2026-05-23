#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VkConfigurator.h"

class VkConfigurator : public QMainWindow
{
    Q_OBJECT

public:
    VkConfigurator(QWidget *parent = nullptr);
    ~VkConfigurator();

private:
    Ui::VkConfiguratorClass ui;
};

