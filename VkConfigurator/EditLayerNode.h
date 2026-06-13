#pragma once
#include "IEditLayerNode.h"

template<typename Type>
class EditLayerNode : public IEditLayerNode
{
private:
    std::reference_wrapper<Type> m_ref;

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
                (appendChild(m_name, tupleArgs.first, parent.*(tupleArgs.second)), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    template<typename MemberType>
    void appendChild(const std::string& baseName, std::string_view memberName, MemberType& member)
    {
        auto pChild = new EditLayerNode<MemberType>(baseName + "." + memberName, member, this);
        m_children.push_back(pChild);
    }

    void genChildrenFromMembers()
    {
        std::apply(
            [this](auto&... tupleArgs)
            {
                (appendChild(m_name, tupleArgs.first, m_ref.get().*(tupleArgs.second)), ...);
            },
            Type::s_reflectiveCtx);
    }


    //------------------------------------------------------------------------------------------------------------

    void resetChildrenFromParentMembers(ParentType& parent, int& index)
    {
        if constexpr (has_base<ParentType>::value)
        {
            // add parent members
            resetChildrenFromParentMembers<ParentType::Base>(data);
        }

        std::apply(
            [this, &parent](auto&... tupleArgs)
            {
                (resetChild(m_ref.get().*(tupleArgs.second), index), ...);
            },
            ParentType::s_reflectiveCtx);
    }

    void resetChildrenFromMembers(int& index)
    {
        std::apply(
            [this](auto&... tupleArgs)
            {
                (resetChild(m_ref.get().*(tupleArgs.second), index), ...);
            },
            Type::s_reflectiveCtx);
    }

    template<typename MemberType>
    void resetChild(MemberType& data, int& index)
    {
        auto pChild = dynamic_cast<EditLayerNode<MemberType>>(m_children[index]);
        ++index;
        pChild->reset(data);
    }

    void resetSelfContainer()
    {
        if (m_ref.size() < m_children.size())
        {
            size_t index = 0;
            for (auto& data : m_ref)
            {
                m_children[index]->reset(data);
                ++index;
            }
            m_children.erase(m_children.begin() + index, m_children.end());
        }
        else
        {
            bool addNewChild = false;
            size_t index = 0;
            for (auto& data : m_ref)
            {
                if (!addNewChild)
                {
                    m_children[index]->reset(data);
                    ++index;
                    addNewChild = index >= m_children.size();
                }
                else
                {
                    appendChild<Type::value_type>(std::format("{}[{}]", m_name, index), data);
                    ++index;
                }

            }
        }
    }

public:
    EditLayerNode() = delete;
    EditLayerNode(const std::string_view name, Type& data, const IEditLayerNode* parent = nullptr) : IEditLayerNode{ name, parent }, m_ref{ data }
    {
        if constexpr (is_reflective_v<Type>)
        {
            if constexpr (has_base<Type>::value)
            {
                // add parent members
                genChildrenFromParentMembers<Type::Base>(data);
            }
            genChildrenFromMembers();
        }
        else if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            int index = 0;
            for (auto& data : m_ref)
            {
                appendChild<Type::value_type>(std::format("{}[{}]", m_name, index), data);
                ++index;
            }
        }
    }

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
        else if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
        {
            resetSelfContainer();
        }
    }

    virtual ~EditLayerNode() = default;
    bool isContainer()const override
    {
        return constexpr(/*is_std_array_v<Type> ||*/ is_std_vector_v<Type> || is_std_list_v<Type>);
    }

    void append()override
    {
        if constexpr (isContainer())
            m_ref.push_back(Type::value_type);
        resetSelfContainer();
    }

    void remove()override
    {
        if constexpr (isContainer())
            m_ref.pop_back(Type::value_type);
        resetSelfContainer();
    }

    QVariant checkRole()const override
    {
        if constexpr (is_std_optional_v<Type>)
            return m_ref.has_value() ? Qt::Checked : Qt::Unchecked;
        return QVariant();
    }

    QVariant displayRole()const override
    {
        return QString::fromStdString(m_name);
    }

    void setCheckState(const Qt::CheckState state) override
    {
        if constexpr (is_std_optional_v<Type>)
        {
            if (Qt::Unchecked)
            {
                m_ref.reset();
            }
            else
            {
                m_ref = Type::value_type{};
            }
        }
    }

    void setValue(const QVariant& value) override
    {
        m_ref.get() = value.value<Type>();
    }

    QVariant getValue()const override
    {
        return m_ref;
    }

    std::type_index type()const override
    {
        return std::type_index{ typeid(Type) };
    }
};
