#include "VkConfigurator.h"
#include <qfile.h>
#include <qsettings.h>
#include <qdockwidget.h>

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

    QSettings settingsFile("VkConfigurator_settings.ini", QSettings::IniFormat);
    restoreGeometry(settingsFile.value("VkConfigurator_geom").toByteArray());
    restoreState(settingsFile.value("VkConfigurator_state").toByteArray());
    for (auto dock : findChildren<QDockWidget*>())
    {
        dock->restoreGeometry(settingsFile.value(dock->objectName() + "_geom").toByteArray());
    }
}

VkConfigurator::~VkConfigurator()
{
    QSettings settingsFile("VkConfigurator_settings.ini", QSettings::IniFormat);
    settingsFile.setValue("VkConfigurator_geom", saveGeometry());
    settingsFile.setValue("VkConfigurator_state", saveState());
    for (auto dock : findChildren<QDockWidget*>())
    {
        settingsFile.setValue(dock->objectName() + "_geom", dock->saveGeometry());
    }
}



