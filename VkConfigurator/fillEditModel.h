#pragma once
/***********************************************
* @headerfile fillEditModel.h
* @date 27/6/2026
* @author genModelData.py
************************************************/
#include <vector>
#include <string>
#include "EditModel.h"
#include "EditClassNode.h"
#include "EngineParameters.h"



inline void fill(std::vector<QModelIndex>& newIndices, const std::string& profile, EditModel* pModel)
{
    if (auto iter = std::find_if(Reflective::instance().cbegin(), Reflective::instance().cend(), [&profile](const JsonReflectiveProfileData& a_data)
	{
		return a_data.profile == profile;
	}); iter != Reflective::instance().cend())
    {
        for (const auto& [className, data] : iter->m_classes)
        {
            if (className.compare("BaseParameters") == 0)
            {
                BaseParameters parameter;
                newIndices.push_back(pModel->addClass(new EditClassNode("BaseParameters", parameter)));
            }

            if (className.compare("EngineParameters") == 0)
            {
                EngineParameters parameter;
                newIndices.push_back(pModel->addClass(new EditClassNode("EngineParameters", parameter)));
            }

            if (className.compare("DeviceFeatures") == 0)
            {
                DeviceFeatures parameter;
                newIndices.push_back(pModel->addClass(new EditClassNode("DeviceFeatures", parameter)));
            }

            if (className.compare("QueuesParameters") == 0)
            {
                QueuesParameters parameter;
                newIndices.push_back(pModel->addClass(new EditClassNode("QueuesParameters", parameter)));
            }

            if (className.compare("DeviceParameters") == 0)
            {
                DeviceParameters parameter;
                newIndices.push_back(pModel->addClass(new EditClassNode("DeviceParameters", parameter)));
            }

        }
    }
}