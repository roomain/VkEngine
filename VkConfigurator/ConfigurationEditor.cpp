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
	QObject::connect(ui.twProfiles, &QTreeWidget::currentItemChanged, this, &ConfigurationEditor::currentItemChanged);
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
		if (Reflective::instance().loadFile(file.toStdString()))
		{
			m_currentFile = file;

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
		}
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
	if (m_currentFile.isEmpty())
		m_currentFile = QFileDialog::getSaveFileName(this, "Save configuration", "", "Json Files (*.json)");
	
	if (!m_currentFile.isEmpty())
		Reflective::instance().writeFile(m_currentFile.toStdString(), true);
}

void ConfigurationEditor::onSaveAsFile()
{
	auto filePath = QFileDialog::getSaveFileName(this, "Save configuration", "", "Json Files (*.json)");
	if (!filePath.isEmpty())
		Reflective::instance().writeFile(filePath.toStdString(), true);
}

void ConfigurationEditor::enableActions()
{
	bool isSelected = ui.twProfiles->currentIndex().isValid();
	ui.actionAdd_Engine_Parameters->setEnabled(isSelected);
	ui.actionAdd_Queue_Parameters->setEnabled(isSelected);
	ui.actionAdd_Device_Parameters->setEnabled(isSelected);
	ui.actionAdd_Device_Features->setEnabled(isSelected);
}

void ConfigurationEditor::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	enableActions();

	auto pModel = static_cast<EditModel*>(ui.confView->model());
	if (previous)
		pModel->save(previous->text(0).toStdString());

	pModel->clear();

	if (current == nullptr)
		return;

	pModel->startInit();
	const auto currentProfile = current->text(0).toStdString();
	Reflective::instance().setCurrentProfile(currentProfile);
	QList<QModelIndex> nodeList;
	if (auto iter = std::find_if(Reflective::instance().cbegin(), Reflective::instance().cend(), [&currentProfile](const JsonReflectiveProfileData& a_data)
		{
			return a_data.profile == currentProfile;
		}); iter != Reflective::instance().cend())
	{
		for (const auto& [className, data] : iter->m_classes)
		{
			if (className.compare("DeviceFeatures") == 0)
			{
				DeviceFeatures parameter;
				nodeList.append(pModel->addClass(new EditClassNode("DeviceFeatures", parameter)));
			}

			if (className.compare("DeviceParameters") == 0)
			{
				DeviceParameters parameter;
				nodeList.append(pModel->addClass(new EditClassNode("DeviceFeatures", parameter)));
			}

			if (className.compare("QueuesParameters") == 0)
			{
				QueuesParameters parameter;
				nodeList.append(pModel->addClass(new EditClassNode("QueuesParameters", parameter)));
			}

			if (className.compare("EngineParameters") == 0)
			{
				EngineParameters parameter;
				nodeList.append(pModel->addClass(new EditClassNode("EngineParameters", parameter)));
			}

			
		}
	}
	pModel->endInit();
	for(const auto &index : nodeList)
		ui.confView->expand(index);
}



void ConfigurationEditor::onNewProfile()
{
	EditProfileName profileDiag([this](const QString& name)
		{
			for (int index = 0; index < ui.twProfiles->topLevelItemCount(); ++index)
			{
				if (name.compare(ui.twProfiles->topLevelItem(index)->text(0), Qt::CaseInsensitive) == 0)
					return true;
			}
			return false;
		}, this);
	if (profileDiag.exec() == QDialog::Accepted)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(0, profileDiag.profileName());
		item->setIcon(0, QIcon(":/VkConfigurator/resources/profile.png"));
		ui.twProfiles->addTopLevelItem(item);
		ui.twProfiles->setCurrentItem(item);
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