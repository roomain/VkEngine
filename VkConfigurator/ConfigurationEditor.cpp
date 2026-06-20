#include "ConfigurationEditor.h"
#include "Reflective.h"
#include <qfiledialog.h>
#include "EditProfileName.h"
#include "EditModel.h"
#include "EngineParameters.h"
#include "EditClassNode.h"
#include "EditionNodeDelegate.h"
#include <map>

ConfigurationEditor::ConfigurationEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QList<int> sizes;
	sizes.append(width() / 3);
	sizes.append(2 * width() / 3);
	ui.splitter->setSizes(sizes);
	QObject::connect(ui.twProfiles, &QTreeView::clicked, this, &ConfigurationEditor::onProfileSelected);
	QObject::connect(ui.twProfiles, &QTreeView::pressed, this, &ConfigurationEditor::onProfileSelected);
	QObject::connect(ui.actionNew_profile, &QAction::triggered, this, &ConfigurationEditor::onNewProfile);

	QObject::connect(ui.actionAdd_Engine_Parameters, &QAction::triggered, this, &ConfigurationEditor::onAddEngineParameters);
	QObject::connect(ui.actionAdd_Queue_Parameters, &QAction::triggered, this, &ConfigurationEditor::onQueueParameters);
	QObject::connect(ui.actionAdd_Device_Parameters, &QAction::triggered, this, &ConfigurationEditor::onDeviceParameters);
	QObject::connect(ui.actionAdd_Device_Features, &QAction::triggered, this, &ConfigurationEditor::onDeviceFeatures);

	ui.confView->setModel(new EditModel());
	auto delegate = new EditionNodeDelegate();
	ui.confView->setItemDelegate(delegate);
	QObject::connect(delegate, &EditionNodeDelegate::expandNode, ui.confView, &QTreeView::expand);
	enableActions();
}

ConfigurationEditor::~ConfigurationEditor()
{
}

void ConfigurationEditor::onNewConfiguration()
{
	ui.twProfiles->clear();
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	pModel->clear();
	Reflective::instance().clear();

	enableActions();
}

void ConfigurationEditor::onOpenFile()
{	
	QString file = QFileDialog::getOpenFileName(nullptr, "Open configuration file", QString(), "Json files (*.json)");
	if (!file.isEmpty())
	{
		Reflective::instance().loadFile(file.toStdString());

		std::map<QString, QTreeWidgetItem*> mapTree;
		ui.twProfiles->clear();
		// fill profiles list
		for (auto iter = Reflective::instance().cbegin(); iter != Reflective::instance().cend(); ++iter)
		{
			QString profileName = QString::fromStdString(iter->profile);
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, profileName);
			item->setData(0, Qt::UserRole, QString::fromStdString(iter->parent));
			item->setIcon(0, QIcon(":/VkConfigurator/resources/profile.png"));
			mapTree[profileName] = item;
		}

		for (auto [name, item] : mapTree)
		{
			QString parent = item->data(0, Qt::UserRole).toString();
			if (parent.isEmpty())
			{
				ui.twProfiles->addTopLevelItem(item);
			}
			else
			{
				if (auto iter = mapTree.find(parent); iter != mapTree.end())
				{
					(*iter).second->addChild(item);
				}
				else
				{
					ui.twProfiles->addTopLevelItem(item);
				}
			}
		}
		ui.twProfiles->expandAll();
		emit loadedFinished();


		enableActions();
		// todo
	}
}

void ConfigurationEditor::onAddEngineParameters()
{
	EngineParameters parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	Reflective::instance().writeProfile(profineName, parameter);
	Reflective::instance().setCurrentProfile(profineName);
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	ui.confView->expand(pModel->addClass(new EditClassNode("EngineParameters", parameter)));
}

void ConfigurationEditor::onQueueParameters()
{
	QueuesParameters parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	Reflective::instance().writeProfile(profineName, parameter);
	Reflective::instance().setCurrentProfile(profineName);
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	ui.confView->expand(pModel->addClass(new EditClassNode("QueuesParameters", parameter)));
}

void ConfigurationEditor::onDeviceParameters()
{
	DeviceParameters parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	Reflective::instance().writeProfile(profineName, parameter);
	Reflective::instance().setCurrentProfile(profineName);
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	ui.confView->expand(pModel->addClass(new EditClassNode("DeviceParameters", parameter)));
}

void ConfigurationEditor::onDeviceFeatures()
{
	DeviceFeatures parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	Reflective::instance().writeProfile(profineName, parameter);
	Reflective::instance().setCurrentProfile(profineName);
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	ui.confView->expand(pModel->addClass(new EditClassNode("DeviceFeatures", parameter)));
}


void ConfigurationEditor::onSaveFile()
{
    //
}

void ConfigurationEditor::onSaveAsFile()
{
    //
}

void ConfigurationEditor::enableActions()
{
	bool isSelected = ui.twProfiles->currentIndex().isValid();
	ui.actionAdd_Engine_Parameters->setEnabled(isSelected);
	ui.actionAdd_Queue_Parameters->setEnabled(isSelected);
	ui.actionAdd_Device_Parameters->setEnabled(isSelected);
	ui.actionAdd_Device_Features->setEnabled(isSelected);
}

void ConfigurationEditor::onProfileSelected(const QModelIndex& current)
{
	// todo save old profile


	enableActions();

	auto pModel = static_cast<EditModel*>(ui.confView->model());
	pModel->clear();

	Reflective::instance().setCurrentProfile(current.data(Qt::DisplayRole).toString().toStdString());
	if (auto iter = std::find_if(Reflective::instance().cbegin(), Reflective::instance().cend(), [&current](const JsonReflectiveProfileData& a_data)
		{
			return a_data.profile == current.data(Qt::DisplayRole).toString().toStdString();
		}); iter != Reflective::instance().cend())
	{
		for (const auto& className : iter->m_classes)
		{
			//
		}
	}
}

void ConfigurationEditor::onNewProfile()
{
	EditProfileName profileDiag([](const QString& name)
		{
			return !Reflective::instance().hasProfile(name.toStdString());
		}, this);
	if (profileDiag.exec() == QDialog::Accepted)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(0, profileDiag.profileName());
		item->setIcon(0, QIcon(":/VkConfigurator/resources/profile.png"));
		ui.twProfiles->addTopLevelItem(item);
	}
}

void ConfigurationEditor::onRenameProfile()
{
	//
}

void ConfigurationEditor::onDeleteProfile()
{
	//
}