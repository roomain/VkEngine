#include "pch.h"
#include <ranges>
#include "RenderGroup.h"
#include "Component.h"
#include "EnginePipeline.h"


void RenderGroup::registerComponent(const std::shared_ptr<Component>& a_component)
{
	m_components.emplace_back(a_component);
}

void RenderGroup::releaseComponent(const std::shared_ptr<Component>& a_component)
{
	if (auto iter = std::ranges::find(m_components, a_component); iter != m_components.end())
		m_components.erase(iter);
}

void RenderGroup::process(VkCommandBuffer& a_cmdBuffer)const
{
	vkCmdBindPipeline(a_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipeline->pipeline());
	for (auto& pCmp : m_components)
	{
		if (pCmp->isEnabled())
			pCmp->draw(a_cmdBuffer);
	}
}