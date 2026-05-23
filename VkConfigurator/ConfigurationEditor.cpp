#include "ConfigurationEditor.h"
#include "Reflective.h"
#include <qfiledialog.h>
#include <map>

ConfigurationEditor::ConfigurationEditor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ConfigurationEditor::~ConfigurationEditor()
{
}

void ConfigurationEditor::onOpenFile()
{
	ui.twProfiles->clear();
	ui.tbwConf->clear();
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