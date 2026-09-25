#pragma once
/***********************************************
* @headerfile RenderGroup.h
* @date 21 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

class EnginePipeline;
class Component;

/*@brief Group for rendering: regroup all component width same pipeline*/
class RenderGroup
{
private:
	std::unique_ptr<EnginePipeline> m_pPipeline;			/*!< common pipeline*/
	std::vector<std::shared_ptr<Component>> m_components;	/*!< list of component*/

public:
	void registerComponent(const std::shared_ptr<Component>& a_component);
	void releaseComponent(const std::shared_ptr<Component>& a_component);
	/*@brief process all active components with the same pipeline*/
	void process(VkCommandBuffer& a_cmdBuffer)const;
};