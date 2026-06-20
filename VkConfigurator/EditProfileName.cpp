#include "EditProfileName.h"
#include "Reflective.h"

EditProfileName::EditProfileName(const CheckProfile& check, QWidget *parent)
	: QDialog(parent), m_check{ check }
{
	ui.setupUi(this);
	QObject::connect(ui.lEdtProfile, &QLineEdit::textChanged, this, &EditProfileName::onProfineNameChanged);
	ui.lEdtProfile->setText(generateName());
}

EditProfileName::~EditProfileName()
{}

QString EditProfileName::profileName()const
{
	return ui.lEdtProfile->text();
}

void EditProfileName::onProfineNameChanged(const QString& name)
{
	ui.pBtnOk->setEnabled(!m_check(name));
}

QString EditProfileName::generateName()
{
	QString baseName = "Profile";
	QString name = baseName;

	int index = 1;
	while (m_check(name))
	{
		name = QString("%1_%2").arg(baseName).arg(index);
		++index;
	}
	return name;
}