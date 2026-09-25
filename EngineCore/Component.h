#pragma once
/***********************************************
* @headerfile Component.h
* @date 05 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <ranges>
#include "Transformation.h"
#include "iterators.h"
#include "RTTI_macros.h"
#include "enginecore_globals.h"


#pragma warning(push)
#pragma warning( disable : 4251 )
class ENGINECORE_EXPORT Component : public std::enable_shared_from_this<Component>
{
    DECLARE_RTTI(1, Component)

private:
    bool m_isEnabled = true;
    bool m_isRelative = false;                              /*!< indicate is transformation is relative*/
    Transformation m_transform;                             /*!< transformation*/
    glm::dmat4 m_workingMatrix;

    std::weak_ptr<Component> m_parent;                      /*!< parent component*/
    std::vector<std::shared_ptr<Component>> m_children;     /*!< children component*/

    void setParent(const std::weak_ptr<Component>& a_parent);
    void removeParent();
    void removeChild_internal(const Component* a_component);

protected:
    /*@brief update function called at each frame (working matrix has been updated)*/
    virtual void internalUpdate(const float a_time) = 0;

public:
    void enable(const bool a_enable) { m_isEnabled = a_enable; }
    constexpr [[nodiscard]] bool isEnabled()const noexcept { return m_isEnabled; }
    void setRelative(const bool a_relative) { m_isRelative = a_relative; }
    constexpr [[nodiscard]] bool isRelative()const noexcept { return m_isRelative; }
    inline [[nodiscard]] Transformation& transform() { return m_transform; }
    inline [[nodiscard]] const glm::dmat4& workingMatrix()const noexcept
    {
        return m_isRelative ? m_workingMatrix : m_transform.matrix();
    }

    std::weak_ptr<Component> parent()const { return m_parent; }
    DEFINE_ITER(std::vector<std::shared_ptr<Component>>, m_children)
    DEFINE_CONST_ITER(std::vector<std::shared_ptr<Component>>, m_children)
    inline std::shared_ptr<Component> operator [] (const size_t& a_index) { return m_children[a_index]; }

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
    void addChild(const std::shared_ptr<Component>& a_component);
    void removeChild(const std::shared_ptr<Component>& a_component);

    /*@brief update function called at each frame*/
    /*@param a_time: ms since last frame*/
    virtual void update(const float a_time, const glm::dmat4& a_absoluteMat);
    virtual void draw(VkCommandBuffer& a_command) = 0;
};


#pragma warning(pop)