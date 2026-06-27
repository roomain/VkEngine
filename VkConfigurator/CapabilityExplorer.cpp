#include "CapabilityExplorer.h"
#include "VulkanTreeModel.h"
#include "VulkanTreeItem.h"
#include "VkCapabiltyVisitorImpl.h"
#include "CapabilitiesDatabase.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>

CapabilityExplorer::CapabilityExplorer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_visitor.visit(CapabilitiesDatabase::instance().capabilities());

	ui.tvVulkanArchi->setModel(m_visitor.createModel());
	ui.tvVulkanArchi->expandAll();

	QObject::connect(ui.tvVulkanArchi, &QTreeView::clicked, this, &CapabilityExplorer::onCapabilitySelected);
	QObject::connect(ui.tvVulkanArchi, &QTreeView::pressed, this, &CapabilityExplorer::onCapabilitySelected);

	QObject::connect(ui.tbwVulkanCapacities, &QTableView::doubleClicked, this, &CapabilityExplorer::onCapabilityTableSelected);
    ui.tbwVulkanCapacities->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(ui.tbwVulkanCapacities, &QTreeView::customContextMenuRequested, this, &CapabilityExplorer::onCtxMenu);
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
		QDesktopServices::openUrl(QUrl(link.toString()));
}

void CapabilityExplorer::onCopyData()
{
	auto pModel = ui.tbwVulkanCapacities->model();
	emit sg_copy(static_cast<VulkanTreeItem*>(ui.tvVulkanArchi->currentIndex().internalPointer()), 
		pModel->data(ui.tbwVulkanCapacities->currentIndex()));
}

void CapabilityExplorer::onCtxMenu(const QPoint& pt)
{
	auto modelIndex = ui.tbwVulkanCapacities->currentIndex();
	if (modelIndex.isValid() && modelIndex.column() == 1)
	{		
		QMenu menu(this);
		menu.move(mapToGlobal(pt + ui.tbwVulkanCapacities->pos()));
		auto actAddToProfile = menu.addAction("Add to profile");
		QObject::connect(actAddToProfile, &QAction::triggered, this, &CapabilityExplorer::onCopyData);
		menu.exec();
	}
}