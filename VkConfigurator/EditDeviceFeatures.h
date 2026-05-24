#pragma once

#include <QWidget>
#include "ui_EditDeviceFeatures.h"
#include "EngineParameters.h"

class EditDeviceFeatures : public QWidget
{
	Q_OBJECT

public:
	EditDeviceFeatures(QWidget *parent = nullptr);
	~EditDeviceFeatures();
	DeviceFeatures get();

private:
	Ui::EditDeviceFeaturesClass ui;
};

