#include "EditProfileName.h"
#include "Reflective.h"

EditProfileName::EditProfileName(const CheckProfile& check, QWidget *parent)
	: QDialog(parent), m_check{ check }
{
	ui.setupUi(this);
	QObject::connect(ui.lEdtProfile, &QLineEdit::textChanged, this, &EditProfileName::onProfineNameChanged);
	ui.lEdtProfile->setText(EditProfileName::generateName());
}

EditProfileName::~EditProfileName()
{}

QString EditProfileName::profileName()const
{
	return ui.lEdtProfile->text();
}

void EditProfileName::onProfineNameChanged(const QString& name)
{
	ui.pBtnOk->setEnabled(m_check(name));
}

QString EditProfileName::generateName()
{
	QString baseName = "Profile";
	QString name = baseName;
	auto find = [](const std::string& testName)
		{
			return Reflective::instance().cend() != std::find_if(Reflective::instance().cbegin(), Reflective::instance().cend(),
				[&testName](auto&& data)
				{
					return data.profile.compare(testName) == 0;
				});
		};

	int index = 1;
	while (find(name.toStdString()))
	{
		name = QString("%1_%2").arg(baseName).arg(index);
		++index;
	}
	return name;
}