#include "pch.h"
#include "Component.h"
#include <ranges>

void Component::removeParent()
{
    m_parent = std::weak_ptr<Component>();
}

void Component::removeChild_internal(const Component* a_component)
{
    if (auto iter = std::ranges::find_if(m_children, [a_component](auto& a_child) {return a_child.get() == a_component; }); iter != m_children.end())
        m_children.erase(iter);
}

void Component::setParent(const std::weak_ptr<Component>& a_parent)
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

void Component::addChild(const std::shared_ptr<Component>& a_component)
{
    if (auto iter = std::ranges::find(m_children, a_component); iter != m_children.end())
    {
        m_children.emplace_back(a_component);
        a_component->setParent(weak_from_this());
    }
}

void Component::removeChild(const std::shared_ptr<Component>& a_component)
{
    if (auto iter = std::ranges::find(m_children, a_component); iter != m_children.end())
    {
        m_children.erase(iter);
    }
}

void Component::update(const float a_time, const glm::dmat4& a_absoluteMat)
{
    if (!m_isEnabled)
        return;

    if (m_isRelative)
        m_workingMatrix = a_absoluteMat * m_transform.matrix();

    internalUpdate(a_time);

    for (const auto& pChild : m_children)
        pChild->update(a_time, workingMatrix());
}