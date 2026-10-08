#pragma once
/***********************************************
* @headerfile StaticMeshPipeline.h
* @date 06 / 09 / 2026
* @author Roomain
************************************************/
#include "Pipeline.h"
#include "enginecore_globals.h"

class ENGINECORE_EXPORT StaticMeshPipeline : public Pipeline
{
public:
	StaticMeshPipeline() = delete;
	explicit StaticMeshPipeline(const std::string& a_name, std::weak_ptr<Device> a_device);
	~StaticMeshPipeline()final = default;
};