#include "CapabilityExplorer.h"
#include "VulkanTreeModel.h"
#include "VulkanTreeItem.h"
#include "VkCapabiltyVisitorImpl.h"
#include "EngineApplication.h"
#include <QDesktopServices>
#include <QUrl>

CapabilityExplorer::CapabilityExplorer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_capabilities = EngineApplication::hostCapabilities();
	m_visitor.visit(m_capabilities);

	ui.tvVulkanArchi->setModel(m_visitor.createModel());
	ui.tvVulkanArchi->expandAll();

	QObject::connect(ui.tvVulkanArchi, &QTreeView::clicked, this, &CapabilityExplorer::onCapabilitySelected);
	QObject::connect(ui.tvVulkanArchi, &QTreeView::pressed, this, &CapabilityExplorer::onCapabilitySelected);

	QObject::connect(ui.tbwVulkanCapacities, &QTableView::doubleClicked, this, &CapabilityExplorer::onCapabilityTableSelected);
}

CapabilityExplorer::~CapabilityExplorer()
{}

void CapabilityExplorer::onCapabilitySelected(const QModelIndex& current)
{
    auto item = static_cast<VulkanTreeItem*>(current.internalPointer());
    if (item)
        ui.tbwVulkanCapacities->setModel(&item->data());
    else
        ui.tbwVulkanCapacities->setModel(nullptr);
}

void CapabilityExplorer::onCapabilityTableSelected(const QModelIndex& current)
{
    auto link = current.data(Qt::UserRole);
    if (link.isValid())// is link
    {
        QDesktopServices::openUrl(QUrl(link.toString()));
    }
}