#include "pch.h"
#include "EngineComponent.h"
#include <ranges>

void EngineComponent::removeParent()
{
    m_parent = std::weak_ptr<EngineComponent>();
}

void EngineComponent::removeChild_internal(const EngineComponent* a_component)
{
    if (auto iter = std::ranges::find_if(m_children, [a_component](auto& a_child) {return a_child.get() == a_component; }); iter != m_children.end())
        m_children.erase(iter);
}

void EngineComponent::setParent(const std::weak_ptr<EngineComponent>& a_parent)
{
    if (auto parent = m_parent.lock())
    {
        if (parent != a_parent.lock())
        {
            parent->removeChild_internal(this);
            m_parent = a_parent;
        }
    }
    else
    {
        m_parent = a_parent;
    }
}

void EngineComponent::addChild(const std::shared_ptr<EngineComponent>& a_component)
{
    if (auto iter = std::ranges::find(m_children, a_component); iter != m_children.end())
    {
        m_children.emplace_back(a_component);
        a_component->setParent(weak_from_this());
    }
}

void EngineComponent::removeChild(const std::shared_ptr<EngineComponent>& a_component)
{
    if (auto iter = std::ranges::find(m_children, a_component); iter != m_children.end())
    {
        m_children.erase(iter);
    }
}