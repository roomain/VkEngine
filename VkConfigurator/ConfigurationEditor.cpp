#include "ConfigurationEditor.h"
#include "Reflective.h"
#include <qfiledialog.h>
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
}

ConfigurationEditor::~ConfigurationEditor()
{
}

void ConfigurationEditor::onNewConfiguration()
{
	ui.twProfiles->clear();

	for (int index = 0; index < ui.vLayoutEdit->count(); ++index)
	{
		delete ui.vLayoutEdit->takeAt(index);
	}

	Reflective::instance().clear();
}

void ConfigurationEditor::onOpenFile()
{	
	QString file = QFileDialog::getOpenFileName(nullptr, "Open configuration file", QString(), "Json files (*.json)");
	if (!file.isEmpty())
	{
		Reflective::instance().loadFile(file.toStdString());

		std::map<QString, QTreeWidgetItem*> mapTree;
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

		ui.twProfiles->clear();

		for (int index = 0; index < ui.vLayoutEdit->count(); ++index)
		{
			delete ui.vLayoutEdit->takeAt(index);
		}
	}
}

void ConfigurationEditor::onSaveFile()
{
    //
}

void ConfigurationEditor::onSaveAsFile()
{
    //
}

void ConfigurationEditor::onProfileSelected(const QModelIndex& current)
{
	for (int index = 0; index < ui.vLayoutEdit->count(); ++index)
	{
		delete ui.vLayoutEdit->takeAt(index);
	}

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
	//
}

void ConfigurationEditor::onRenameProfile()
{
	//
}

void ConfigurationEditor::onDeleteProfile()
{
	//
}