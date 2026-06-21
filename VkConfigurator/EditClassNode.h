#pragma once
#include "ITEditNode.h"
#include "TEditNode.h"
#include "Reflective.h"

template<typename Type>
class EditClassNode : public ITEditNode<Type>
{
private:
    Type m_ref;

    template<typename ParentType>
    void genChildrenFromParentMembers(ParentType& parent)
    {
        if constexpr (has_base<ParentType>::value)
        {
            // add parent members
            genChildrenFromParentMembers<ParentType::Base>(data);
        }

        std::apply(
            [this, &parent](auto&... tupleArgs)
            {
                (appendChild(IEditNode::m_name, tupleArgs.first, parent.*(tupleArgs.second)), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    void save(const std::string& profile)const override
    {
        Reflective::instance().writeProfile(profile, m_ref);
    }

    template<typename MemberType>
    void appendChild(const std::string& baseName, std::string_view memberName, MemberType& member)
    {
        auto pChild = new TEditNode<MemberType>(baseName + "." + std::string(memberName), member, this);
        IEditNode::m_children.push_back(pChild);
    }

    template<typename MemberType>
    void appendChild(const std::string& memberName, MemberType& member)
    {
        auto pChild = new TEditNode<MemberType>(memberName, member, this);
        IEditNode::m_children.push_back(pChild);
    }

    void genChildrenFromMembers()
    {
        std::apply(
            [this](auto&... tupleArgs)
            {
                (appendChild(IEditNode::m_name, tupleArgs.first, m_ref.*(tupleArgs.second)), ...);
            },
            Type::s_reflectiveCtx);
    }


    //------------------------------------------------------------------------------------------------------------
    template<typename ParentType>
    void resetChildrenFromParentMembers(ParentType& parent)
    {
        size_t index = 0;
        if constexpr (has_base<ParentType>::value)
        {
            // add parent members
            resetChildrenFromParentMembers<ParentType::Base>(parent);
        }

        std::apply(
            [this, &parent, &index](auto&... tupleArgs)
            {
                (this->resetChild(m_ref.*(tupleArgs.second), index), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    void resetChildrenFromMembers()
    {
        size_t index = 0;
        std::apply(
            [this, &index](auto&... tupleArgs)
            {
                (this->resetChild(m_ref.*(tupleArgs.second), index), ...);
            },
            Type::s_reflectiveCtx);
    }


public:
    EditClassNode() = delete;
    explicit EditClassNode(const std::string_view name, Type& data, IEditNode* parent = nullptr) : ITEditNode<Type>{ name, parent }, m_ref{ data }
    {
        if constexpr (is_reflective_v<Type>)
        {
            if constexpr (has_base<Type>::value)
            {
                // add parent members
                genChildrenFromParentMembers<Type::Base>(m_ref);
            }
            genChildrenFromMembers();
        }
        else if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            int index = 0;
            for (auto& data : m_ref)
            {
                appendChild<Type::value_type>(std::format("{}[{}]", IEditNode::m_name, index), data);
                ++index;
            }
        }
    }

    const Type& data()const { return m_ref; }
    Type& data() { return m_ref; }

    void reset(Type& data)
    {
        // reset data of vector or list
        m_ref = data;

        if constexpr (is_reflective_v<Type>)
        {
            if constexpr (has_base<Type>::value)
            {
                // add parent members
                resetChildrenFromParentMembers<Type::Base>(data);
            }
            resetChildrenFromMembers();
        }
    }

    virtual ~EditClassNode() = default;

};