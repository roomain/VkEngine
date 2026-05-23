#include "VkConfigurator.h"
#include "VulkanTreeModel.h"
#include "VulkanTreeItem.h"
#include "VkCapabiltyVisitorImpl.h"
#include "EngineApplication.h"

VkConfigurator::VkConfigurator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setVisible(false);
    tabifyDockWidget(ui.dwgCapabilities, ui.dwgConf);
    ui.dwgCapabilities->raise();

    m_visitor.visit(EngineApplication::hostCapabilities());
    ui.tvVulkanArchi->setModel(m_visitor.createModel());
	ui.tvVulkanArchi->expandAll();

    QObject::connect(ui.actionOpen, &QAction::triggered, this, &VkConfigurator::onOpenFile);
    QObject::connect(ui.actionSave, &QAction::triggered, this, &VkConfigurator::onSaveFile);
    QObject::connect(ui.actionSave_as, &QAction::triggered, this, &VkConfigurator::onSaveAsFile);

    QObject::connect(ui.tvVulkanArchi, &QTreeView::clicked, this, &VkConfigurator::onCapabilitySelected);
    QObject::connect(ui.tvVulkanArchi, &QTreeView::pressed, this, &VkConfigurator::onCapabilitySelected);
}

VkConfigurator::~VkConfigurator()
{}

void VkConfigurator::onOpenFile()
{
    //
}

void VkConfigurator::onSaveFile()
{
    //
}

void VkConfigurator::onSaveAsFile()
{
    //
}

void VkConfigurator::onCapabilitySelected(const QModelIndex& current)
{
    auto item = static_cast<VulkanTreeItem*>(current.internalPointer());
    if(item)
        ui.tbwVulkanCapacities->setModel(&item->data());
    else
        ui.tbwVulkanCapacities->setModel(nullptr);
}