#include "VkConfigurator.h"

VkConfigurator::VkConfigurator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setVisible(false);
    tabifyDockWidget(ui.dwgCapabilities, ui.dwgConf);
    ui.dwgCapabilities->raise();

    QObject::connect(ui.actionOpen, &QAction::triggered, ui.dwContentsConf, &ConfigurationEditor::onOpenFile);
    QObject::connect(ui.actionSave, &QAction::triggered, ui.dwContentsConf, &ConfigurationEditor::onSaveFile);
    QObject::connect(ui.actionSave_as, &QAction::triggered, ui.dwContentsConf, &ConfigurationEditor::onSaveAsFile);
    QObject::connect(ui.dwContentsConf, &ConfigurationEditor::loadedFinished, ui.dwgConf, &QDockWidget::raise);

    QObject::connect(ui.dwContentsCapabilities, &CapabilityExplorer::sg_copy, ui.dwContentsConf, &ConfigurationEditor::onCopy);
}

VkConfigurator::~VkConfigurator()
{}



