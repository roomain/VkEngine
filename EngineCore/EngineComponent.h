#pragma once
/***********************************************
* @headerfile EngineComponent.h
* @date 05 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <ranges>
#include "iterators.h"
#include "RTTI_macros.h"
#include "enginecore_globals.h"


#pragma warning(push)
#pragma warning( disable : 4251 )
class ENGINECORE_EXPORT EngineComponent : public std::enable_shared_from_this<EngineComponent>
{
    DECLARE_RTTI(1, EngineComponent)

private:
    std::weak_ptr<EngineComponent> m_parent;                      /*!< parent component*/
    std::vector<std::shared_ptr<EngineComponent>> m_children;     /*!< children component*/

    void setParent(const std::weak_ptr<EngineComponent>& a_parent);
    void removeParent();
    void removeChild_internal(const EngineComponent* a_component);
public:
    std::weak_ptr<EngineComponent> parent()const { return m_parent; }
    DEFINE_ITER(std::vector<std::shared_ptr<EngineComponent>>, m_children)
    DEFINE_CONST_ITER(std::vector<std::shared_ptr<EngineComponent>>, m_children)
    inline std::shared_ptr<EngineComponent> operator [] (const size_t& a_index) { return m_children[a_index]; }

    template<typename Type>
    std::shared_ptr<Type> findFirst(const bool a_recursive)const
    {
        if (!a_recursive)
        {
            auto desc = Type::definition();
            if (auto iter = std::ranges::find_if(m_children, [desc](const auto& a_child) {return a_child->isKindOf(desc); }); iter != m_children.cend())
                return (*iter)->cast<Type>();
        }
        else
        {
            for (const auto& child : m_children)
            {
                if (child->isKindOf<Type>())
                {
                    return child->cast<Type>();
                }
                else if (auto node = child->findFirst<Type>(true))
                {
                    return node;
                }
            }
        }
        return std::shared_ptr<Type>();
    }

    template<typename Type>
    void find(std::vector<std::shared_ptr<Type>>& a_children, const bool a_recursive)const
    {
        for (const auto& child : m_children)
        {
            if (child->isKindOf<Type>())
                a_children.emplace_bach(child->cast<Type>());

            if (a_recursive)
                child->findFirst<Type>(a_children, true);
        }
    }

    constexpr size_t childCount()const { return m_children.size(); }
    void addChild(const std::shared_ptr<EngineComponent>& a_component);
    void removeChild(const std::shared_ptr<EngineComponent>& a_component);
};


#pragma warning(pop)