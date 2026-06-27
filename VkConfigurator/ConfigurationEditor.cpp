#include "ConfigurationEditor.h"
#include "Reflective.h"
#include <qfiledialog.h>
#include <QMenu>
#include "EditProfileName.h"
#include "EditModel.h"
#include "EngineParameters.h"
#include "EditionNodeDelegate.h"
#include "EditClassNode.h"
#include "fillEditModel.h"
#include "VulkanTreeItem.h"
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
	QObject::connect(ui.twProfiles, &QTreeView::customContextMenuRequested, this, &ConfigurationEditor::onProfileCtxMenu);
	ui.twProfiles->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.confView->setModel(new EditModel());
	auto delegate = new EditionNodeDelegate();
	ui.confView->setItemDelegate(delegate);
	QObject::connect(delegate, &EditionNodeDelegate::expandNode, ui.confView, &QTreeView::expand);
	enableActions();
}

void ConfigurationEditor::onProfileCtxMenu(const QPoint& pt)
{
	if (auto item = ui.twProfiles->currentItem())
	{
		QMenu menu(ui.twProfiles);
		
		menu.move(mapToGlobal(pt));
		auto actRemove = menu.addAction("Remove profile");
		auto actRename = menu.addAction("Rename profile");

		QObject::connect(actRemove, &QAction::triggered, this, &ConfigurationEditor::onDeleteProfile);
		QObject::connect(actRename, &QAction::triggered, this, &ConfigurationEditor::onRenameProfile);
		menu.exec();
	}
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
	if (!Reflective::instance().hasClass(profineName, "EngineParameters"))
	{
		Reflective::instance().writeProfile(profineName, parameter);
		Reflective::instance().setCurrentProfile(profineName);
		auto pModel = static_cast<EditModel*>(ui.confView->model());
		ui.confView->expand(pModel->addClass(new EditClassNode("EngineParameters", parameter)));
	}
}

void ConfigurationEditor::onQueueParameters()
{
	QueuesParameters parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();

	if (!Reflective::instance().hasClass(profineName, "QueuesParameters"))
	{
		Reflective::instance().writeProfile(profineName, parameter);
		Reflective::instance().setCurrentProfile(profineName);
		auto pModel = static_cast<EditModel*>(ui.confView->model());
		ui.confView->expand(pModel->addClass(new EditClassNode("QueuesParameters", parameter)));
	}
}

void ConfigurationEditor::onDeviceParameters()
{
	DeviceParameters parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	if (!Reflective::instance().hasClass(profineName, "DeviceParameters"))
	{
		Reflective::instance().writeProfile(profineName, parameter);
		Reflective::instance().setCurrentProfile(profineName);
		auto pModel = static_cast<EditModel*>(ui.confView->model());
		ui.confView->expand(pModel->addClass(new EditClassNode("DeviceParameters", parameter)));
	}
}

void ConfigurationEditor::onDeviceFeatures()
{
	DeviceFeatures parameter;
	auto profineName = ui.twProfiles->currentItem()->text(0).toStdString();
	if (!Reflective::instance().hasClass(profineName, "DeviceFeatures"))
	{
		Reflective::instance().writeProfile(profineName, parameter);
		Reflective::instance().setCurrentProfile(profineName);
		auto pModel = static_cast<EditModel*>(ui.confView->model());
		ui.confView->expand(pModel->addClass(new EditClassNode("DeviceFeatures", parameter)));
	}
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

	const auto pModel = static_cast<EditModel*>(ui.confView->model());
	if (previous)
		pModel->save(previous->text(0).toStdString());

	pModel->clear();

	if (current == nullptr)
		return;

	pModel->startInit();
	const auto currentProfile = current->text(0).toStdString();
	Reflective::instance().setCurrentProfile(currentProfile);
	std::vector<QModelIndex> nodeList;
	fill(nodeList, currentProfile, pModel);
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
	profileDiag.setWindowTitle("New profile");
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
	if (auto item = ui.twProfiles->currentItem())
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
		profileDiag.setWindowTitle(QString("Rename profile %1").arg(item->text(0)));
		if (profileDiag.exec() == QDialog::Accepted)
		{
			item->setText(0, profileDiag.profileName());
		}
	}
}

void ConfigurationEditor::onDeleteProfile()
{
	if (auto item = ui.twProfiles->currentItem())
	{
		ui.twProfiles->setCurrentItem(nullptr);
		ui.twProfiles->removeItemWidget(item, 0);
		delete item;
	}
}

void ConfigurationEditor::addToVector(std::vector<std::string>& vData, const std::string& text)
{
	auto iter = std::ranges::find_if(vData, [&text](const std::string& value)
		{
			return value.empty() || value == text;
		});
	if (iter == vData.end())
	{
		vData.emplace_back(text);
	}
	else
	{
		*iter = text;
	}
}

void ConfigurationEditor::onCopyExtension(VulkanTreeItem* pItem, const QVariant& data)
{
	auto extension = data.toString().split(" ")[0];
	if (ConfigurationEditor::isInstanceProperty(pItem))
	{
		QString path = "EngineParameters.extensions";
		copyTo(path, extension);
	}
	else
	{
		QString path = "DeviceParameters.layers";
		copyTo(path, extension);
	}
}

void ConfigurationEditor::copyTo(IEditNode* pNode, const QVariant& data)
{
	if (data.canConvert<QString>())
	{
		static_cast<TEditNode<std::string>*>(pNode)->reference().get() = data.toString().toStdString();
	}
	if (data.canConvert<int>())
	{
		static_cast<TEditNode<int>*>(pNode)->reference().get() = data.toInt();
	}
	if (data.canConvert<double>())
	{
		static_cast<TEditNode<double>*>(pNode)->reference().get() = data.toDouble();
	}
	if (data.canConvert<unsigned int>())
	{
		static_cast<TEditNode<unsigned int>*>(pNode)->reference().get() = data.toUInt();
	}
	if (data.canConvert<float>())
	{
		static_cast<TEditNode<float>*>(pNode)->reference().get() = data.toFloat();
	}
}

void ConfigurationEditor::copyTo(const QString& path, const QVariant& data)
{
	auto pModel = static_cast<EditModel*>(ui.confView->model());
	QModelIndex modelIndex;
	if (auto pNode = pModel->findNode(path, modelIndex))
	{
		if (pNode->isContainer())
		{
			pModel->beginAddRow(modelIndex);
			pNode->append();	
			ConfigurationEditor::copyTo(*(pNode->end() - 1), data);
			pModel->endAddRow();
		}
		else
		{
			ConfigurationEditor::copyTo(pNode, data);
		}
	}

}

void ConfigurationEditor::onCopyLayer(VulkanTreeItem* pItem, const QVariant& data)
{
	if (ConfigurationEditor::isInstanceProperty(pItem))
	{
		QString path = "EngineParameters.layers";
		copyTo(path, data);
	}
	else
	{
		QString path = "DeviceParameters.layers";
		copyTo(path, data);
	}
}


bool ConfigurationEditor::isInstanceProperty(VulkanTreeItem* pItem)
{
	if (auto parent = pItem->parent())
		return parent->parent() == nullptr;
	return false;
}

void ConfigurationEditor::onCopy(VulkanTreeItem* pItem, const QVariant& data)
{
	QString name = pItem->displayRole().toString();
	if (0 == name.compare("Extensions", Qt::CaseInsensitive))
	{
		onCopyExtension(pItem, data);
	}
	else if (auto parent = pItem->parent())
	{
		name = parent->displayRole().toString();
		if (0 == name.compare("Layers", Qt::CaseInsensitive))
		{
			onCopyLayer(parent, pItem->displayRole());
		}
	}
	else if (0 == name.compare("Queue family", Qt::CaseInsensitive))
	{
		//
	}
	// todo
}